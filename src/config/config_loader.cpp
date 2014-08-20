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
#include "../backends/raspi.hpp"

namespace heinz
{

Config load_config(const string &filename)
{
	using boost::property_tree::ptree;
	ptree pt;
	try
	{
		read_info(filename,pt);
	}
	catch(std::exception &e)
	{
		throw ConfigException((boost::format("unable to load config file: %1%") % e.what()).str());
	}
	Config config;

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
				ptr=make_shared<FakeEndpoint>(v.second);
			}
			// multiplexer endpoint
			else if(type=="multiplex")
			{
				ptr=make_shared<MultiplexerEndpoint>(v.second,config);
			}
			// raspi
			else if(type=="raspi")
			{
				ptr=make_shared<EndpointRaspberry>(v.second);
			}
			// no matching endpoint type found
			else
				throw ConfigException((boost::format("unknown endpoint type: %1%") % type).str());
			config.endpoints.insert(std::make_pair(name,ptr));
		}
		catch(std::exception &e)
		{
			throw ConfigException((boost::format("error while parsing endpoint %1%: %2%") % name % e.what()).str());
		}
		//std::cout<<name<<" ("<<v.second.data()<<"): "<<desc<<"\n";
		//shared_ptr<Endpoint>(new )
		//endpoints.insert(v.second.data());
	}
	return config;
}

}
