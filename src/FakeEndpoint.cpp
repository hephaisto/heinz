#include <boost/log/trivial.hpp>
#include "FakeEndpoint.hpp"

namespace heinz
{

FakeEndpoint::FakeEndpoint(string description, EnRangeType rangeType,bool isInput)
:HardwareEndpoint(description,rangeType,isInput)
{}

FakeEndpoint::FakeEndpoint(ptree &pt)
:HardwareEndpoint(pt)
{}

void FakeEndpoint::setValue(int64_t value)
{
	checkValueForValidity(value);
	{
		boost::unique_lock<FakeEndpoint> guard(*this);
		this->cachedValue=value;
	}
	triggerUpdates();
	BOOST_LOG_TRIVIAL(info)<<"endpoint "<<getDescription()<<" set to "<<value;
}

bool FakeEndpoint::isValid()
{
	return true;
}

bool FakeEndpoint::updatesAvailable()
{
	if(!getIsInput())
		return false;
	switch(rangeType)
	{
	case RANGE_U1:
		cachedValue=cachedValue?0:1;
		break;
	case RANGE_U8:
		cachedValue=cachedValue==255?0:cachedValue+1;
		break;
	default:
		break;
	}
	return true;
}
void FakeEndpoint::postUpdates()
{
	triggerUpdates();
	executeScript();
}

shared_ptr<Endpoint> FakeEndpoint::create(shared_ptr<Config> config, ptree &pt)
{
	shared_ptr<FakeEndpoint> tmp=make_shared<FakeEndpoint>(pt);
	if(tmp->getIsInput())
	{
		shared_ptr<PollingObject> po=tmp;
		config->pollingObjects.push_back(po);
		tmp->setScriptIfAvailable(pt);
	}
	return tmp;
}

}
