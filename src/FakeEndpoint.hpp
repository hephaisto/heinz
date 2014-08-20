#ifndef FAKE_ENDPOINT_H
#define FAKE_ENDPOINT_H

#include "HardwareEndpoint.hpp"

namespace heinz
{

/**
A fake endpoint class which just prints status changes to the log.
**/
class FakeEndpoint : public HardwareEndpoint
{
public:
	FakeEndpoint(string description, EnRangeType rangeType,bool isInput);
	FakeEndpoint(ptree &pt);
	virtual void setValue(int64_t value, ScalarEndpointObserver *source);
	virtual bool isValid();
};

}

#endif
