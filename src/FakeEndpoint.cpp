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
	BOOST_LOG_TRIVIAL(info)<<"endpoint set to "<<value;
}

bool FakeEndpoint::isValid()
{
	return true;
}

}
