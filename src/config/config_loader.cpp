#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <iostream>

#include "config_loader.hpp"
#include "../Endpoint.hpp"
#include "../common.hpp"
#include "../constants.hpp"
#include "../exceptions.hpp"

#include "../FakeEndpoint.hpp"
#include "../MultiplexerEndpoint.hpp"

#ifdef HAS_WIRINGPI
#include "../backends/raspi.hpp"
#endif

namespace heinz
{

using boost::property_tree::ptree;
void addScriptIfAvailable(shared_ptr<PollingObject> object, ptree &pt)
{
	try
	{
		object->setScript(pt.get<string>("update_command"));
	}
	catch(...)
	{}
}

shared_ptr<Config> load_config(const string &filename)
{
	ptree pt;
	try
	{
		read_info(filename,pt);
	}
	catch(std::exception &e)
	{
		throw ConfigException((boost::format("unable to load config file: %1%") % e.what()).str());
	}
	shared_ptr<Config> config=make_shared<Config>();

	// ENDPOINTS
	BOOST_FOREACH( ptree::value_type &v, pt.get_child("endpoints") )
	{
		string name=v.first.data();
		string type=v.second.data();

		string description=v.second.get<string>("description","");

		//string desc=v.second.get<string>("description");
		shared_ptr<Endpoint> ptr(nullptr);

		try
		{
			// fake endpoint
			if(type=="fake")
			{
				shared_ptr<FakeEndpoint> tmp=make_shared<FakeEndpoint>(v.second);
				ptr=tmp;
				if(tmp->getIsInput())
				{
					config->pollingObjects.push_back(tmp);
					addScriptIfAvailable(tmp,v.second);
				}
			}
			// multiplexer endpoint
			else if(type=="multiplex")
			{
				ptr=MultiplexerEndpoint::createMultiplexerEndpoint(v.second,config);
			}
			// raspi
			else if(type=="raspi")
			{
				#ifdef HAS_WIRINGPI
				shared_ptr<EndpointRaspberry> tmp=make_shared<EndpointRaspberry>(v.second);
				ptr=tmp;
				if(tmp->getIsInput())
				{
					config->pollingObjects.push_back(tmp);
					addScriptIfAvailable(tmp,v.second);
				}
				#else
				throw HeinzException("This version has been built without support for wiringPi!");
				#endif
			}
			// no matching endpoint type found
			else
				throw ConfigException((boost::format("unknown endpoint type: %1%") % type).str());
			config->endpoints.insert(std::make_pair(name,ptr));
		}
		catch(std::exception &e)
		{
			throw ConfigException((boost::format("error while parsing endpoint %1%: %2%") % name % e.what()).str());
		}
		//std::cout<<name<<" ("<<v.second.data()<<"): "<<desc<<"\n";
		//shared_ptr<Endpoint>(new )
		//endpoints.insert(v.second.data());
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
					throw ConfigException((boost::format("endpoint %1% not found") % endpointname).str());
				}
			}
			supergrp.insert(std::make_pair(groupname,vec));
		}
		config->groups.insert(std::make_pair(supername,supergrp));
	}
	config->pollingInterval=pt.get<uint64_t>("polling_interval",5000);

	return config;
}

}
