#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <Wt/WApplication>

#include "MultiplexerEndpoint.hpp"
#include "exceptions.hpp"

namespace heinz
{

MultiplexerEndpoint::MultiplexerEndpoint(string description, EnRangeType rangeType, vector<shared_ptr<ScalarEndpoint> > endpoints)
:ScalarEndpoint(description,rangeType,false /* multiplexer endpoints have to be output! */),
endpoints(endpoints)
{
	if(endpoints.empty())
		throw HeinzException("Multiplexer endpoints need to have at least one sub-endpoint!");
	for(auto it=endpoints.begin();it!=endpoints.end();it++)
	{
		addSubEndpoint("",*it);
	}
}

shared_ptr<MultiplexerEndpoint> MultiplexerEndpoint::createMultiplexerEndpoint(ptree &pt, shared_ptr<Config> config)
{
	shared_ptr<MultiplexerEndpoint> ptr(new MultiplexerEndpoint(pt,config));
	BOOST_FOREACH(shared_ptr<ScalarEndpoint> ep,ptr->endpoints)
		ptr->subObservers.push_back(std::make_shared<MultiplexerObserverHelper>(ep, ptr));
	return ptr;
}

MultiplexerEndpoint::MultiplexerEndpoint(ptree &pt, shared_ptr<Config> config)
:ScalarEndpoint(pt,false)
{
	BOOST_FOREACH( ptree::value_type &v2, pt.get_child("endpoints") )
	{
		string sub_name=v2.first.data();
		try
		{
			shared_ptr<Endpoint> subEndpoint(config->endpoints.at(sub_name));

			shared_ptr<ScalarEndpoint> sub_endpoint_scalar(std::dynamic_pointer_cast<ScalarEndpoint>(subEndpoint));
			if(sub_endpoint_scalar==nullptr)
				throw ConfigException((boost::format("endpoint %1% is not scalar") % sub_name).str());

			addSubEndpoint(sub_name,sub_endpoint_scalar);
		}
		catch(std::out_of_range &e)
		{
			throw ConfigException((boost::format("endpoint %1% not found") % sub_name).str());
		}
	}
}

void MultiplexerEndpoint::addSubEndpoint(string sub_name, shared_ptr<ScalarEndpoint> subEndpoint)
{
	if(subEndpoint->getIsInput())
		throw ConfigException((boost::format("endpoint %1% is an input-endpoint") % sub_name).str());


	if(subEndpoint->getRange()!=rangeType)
		throw ConfigException((boost::format("endpoint %1% has not the same range as the multiplexer endpoint") % sub_name).str());

	endpoints.push_back(subEndpoint);
}

void MultiplexerEndpoint::setValue(int64_t value, ScalarEndpointObserver *source)
{
	{
		boost::shared_lock<ScalarEndpoint> guard(*this);
		std::cerr<<"changing "<<endpoints.size()<<"endpoints by multiplexer\n";
		for(auto it=endpoints.begin();it!=endpoints.end();it++)
			(*it)->setValue(value, source);
		cachedValue=value;
	}
}
int64_t MultiplexerEndpoint::getValue()
{
	boost::shared_lock<ScalarEndpoint> guard(*this);
	if(!isValid())
		throw InvalidValueException(description);
	return (*endpoints.begin())->getValue();
}
bool MultiplexerEndpoint::isValid()
{
	boost::shared_lock<ScalarEndpoint> guard(*this);
	int64_t val;
	auto it=endpoints.begin();
	if(!(*it)->isValid())
		return false;
	val=(*it)->getValue();
	it++;
	for(;it!=endpoints.end();it++)
	{
		if(!(*it)->isValid())
			return false;
		if(!(*it)->getValue()!=val)
			return false;
	}
	return true;
}

void MultiplexerEndpoint::subEndpointChanged()
{
	triggerUpdates(NULL);
}


MultiplexerObserverHelper::MultiplexerObserverHelper(std::shared_ptr<ScalarEndpoint> observable, std::weak_ptr<MultiplexerEndpoint> multiplexer)
:ScalarEndpointObserver("",observable),
multiplexer(multiplexer)
{}

void MultiplexerObserverHelper::internalUpdate()
{
	std::cerr<<"MultiplexerObserverHelper::internalUpdate\n";
	if(auto m=multiplexer.lock())
		m->subEndpointChanged();
	else
		std::cerr<<"unable to get lock on multiplexer - pointer already deleted?\n";
}

}
