#include <boost/foreach.hpp>
#include <boost/format.hpp>

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
		if((*it)->getIsInput())
			throw HeinzException("multiplexer endpoints can only multiplex outputs!");
		if((*it)->getRange()!=rangeType)
			throw HeinzException("all multiplexed endpoints need to have the same range type as the multiplexer endpoint!");
	}
}

MultiplexerEndpoint::MultiplexerEndpoint(ptree &pt, shared_ptr<Config> config)
:ScalarEndpoint(pt,false)
{
	BOOST_FOREACH( ptree::value_type &v2, pt.get_child("endpoints") )
	{
		string sub_name=v2.first.data();
		try
		{
			shared_ptr<Endpoint> sub_endpoint(config->endpoints.at(sub_name));
			shared_ptr<ScalarEndpoint> sub_endpoint_scalar(std::dynamic_pointer_cast<ScalarEndpoint>(sub_endpoint));
			if(sub_endpoint_scalar==nullptr)
				throw ConfigException((boost::format("endpoint %1% is not scalar") % sub_name).str());
			if(sub_endpoint_scalar->getIsInput())
				throw ConfigException((boost::format("endpoint %1% is an input-endpoint") % sub_name).str());
			endpoints.push_back(sub_endpoint_scalar);
		}
		catch(std::out_of_range &e)
		{
			throw ConfigException((boost::format("endpoint %1% not found") % sub_name).str());
		}
	}
}

void MultiplexerEndpoint::setValue(int64_t value, ScalarEndpointObserver *source)
{
	{
		boost::unique_lock<ScalarEndpoint> guard(*this);
		std::cerr<<"changing "<<endpoints.size()<<"endpoints by multiplexer\n";
		for(auto it=endpoints.begin();it!=endpoints.end();it++)
			(*it)->setValue(value, source);
		cachedValue=value;
	}
	triggerUpdates(NULL);
}
int64_t MultiplexerEndpoint::getValue()
{
	boost::shared_lock<ScalarEndpoint> guard(*this);
	if(!getIsInput())
		throw InvalidValueException();
	return endpoints[0]->getValue();
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


MultiplexerObserverHelper::MultiplexerObserverHelper(string sessionID, ScalarEndpoint* observable, MultiplexerEndpoint *multiplexer)
:ScalarEndpointObserver(sessionID,observable),
multiplexer(multiplexer)
{}

void MultiplexerObserverHelper::internalUpdate(int64_t value)
{
	multiplexer->subEndpointChanged();
}

}
