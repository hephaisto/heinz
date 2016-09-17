
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

#include "config_loader.hpp"
#include "../Endpoint.hpp"
#include "../common.hpp"
#include "../constants.hpp"
#include "../exceptions.hpp"

#include "../backends/BackendPlugin.hpp"

namespace heinz
{


template class PluginManager<BackendPlugin>;

namespace bfs=boost::filesystem;

namespace
{
	PluginManager<BackendPlugin> &pm = PluginManager<BackendPlugin>::instance();
}


shared_ptr<Endpoint> createEndpoint(shared_ptr<Config> config, ptree &pt)
{
	BackendPlugin *plugin = pm.get(pt.data());
	return plugin->createEndpoint(config, pt);
}


bfs::path getFirstExistentFile(vector<string> files)
{
	BOOST_FOREACH(auto f,files)
	{
		bfs::path p(f);
		if(bfs::exists(p))
			return p;
	}
	throw HeinzException("unable to load config file (no suitable config file found)");
}

shared_ptr<Config> load_config()
{
	ptree pt;

	shared_ptr<Config> config=make_shared<Config>();
	bfs::path configPath=getFirstExistentFile({"heinz.conf","config/heinz.conf","/etc/heinz/heinz.conf"});
	string configFile=configPath.string();
	bfs::path configBase=configPath.parent_path();
	try
	{
		try
		{
			read_info(configFile,pt);
		}
		catch(std::exception &e)
		{
			BOOST_THROW_EXCEPTION(ConfigException()<<ExErrorMessage((boost::format("unable to load config from %2%: %1%") % e.what() % configFile).str()));
		}

		// PLUGIN CONFIG
		BOOST_FOREACH( ptree::value_type &v, pt.get_child("endpoint_plugins") )
		{
			string name = v.first.data();
			try
			{
				BackendPlugin *plugin = pm.get(name);
				plugin->backendConfig(v.second);
			}
			catch(boost::exception &e)
			{
				e<<ExEndpointName(name);
				throw;
			}
		}

		// ENDPOINTS
		BOOST_FOREACH( ptree::value_type &v, pt.get_child("endpoints") )
		{
			string name=v.first.data();
			try
			{
				config->endpoints.insert(std::make_pair(name,createEndpoint(config, v.second)));
			}
			catch(boost::exception &e)
			{
				e<<ExEndpointName(name);
				throw;
			}
		}

		// SCRIPTFILES
		BOOST_FOREACH( ptree::value_type &script, pt.get_child("scripts"))
		{
			string name=script.first.data();
			config->scriptFiles.push_back((configBase/name).string());
		}

		// MACROS
		BOOST_FOREACH( ptree::value_type &macro, pt.get_child("macros"))
		{
			string name=macro.first.data();
			string cmd=macro.second.data();
			config->macros.insert(std::make_pair(name,cmd));
		}

		// GROUPS
		BOOST_FOREACH( ptree::value_type &supergroup, pt.get_child("groups") )
		{
			string supername=supergroup.first.data();
			map<string, vector<shared_ptr<Endpoint> > > supergrp;
			BOOST_FOREACH( ptree::value_type &group, supergroup.second )
			{
				string groupname=group.first.data();
				vector<shared_ptr<Endpoint> > vec;
				BOOST_FOREACH( ptree::value_type &endpoint, group.second )
				{
					string endpointname=endpoint.first.data();
					try
					{
						vec.push_back(config->endpoints.at(endpointname));
					}
					catch(std::out_of_range &e)
					{
						BOOST_THROW_EXCEPTION(ConfigException()<<ExErrorMessage((boost::format("endpoint %1% not found") % endpointname).str()));
					}
				}
				supergrp.insert(std::make_pair(groupname,vec));
			}
			config->groups.insert(std::make_pair(supername,supergrp));
		}
		config->pollingInterval=pt.get<uint64_t>("polling_interval",5000);
		config->wtConfigFile=pt.get<string>("wt_config","http_config");
		config->logLevel=pt.get<int>("logging.log_level",3);
		if((config->logLevel<0)||(config->logLevel>5))
			BOOST_THROW_EXCEPTION(ConfigException()<<ExErrorMessage((boost::format("log level %1% is invalid (has to be between 0 and 5)") % config->logLevel).str()));
	}
	catch(boost::exception &e)
	{
		e<<ExConfigFilename(configFile);
		throw;
	}

	return config;
}

}
