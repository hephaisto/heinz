#ifndef FAKE_ENDPOINT_H
#define FAKE_ENDPOINT_H

#include "HardwareEndpoint.hpp"
#include "config/config_loader.hpp"

namespace heinz
{

/**
A fake endpoint class which just prints status changes to the log.
**/
class FakeEndpoint : public HardwareEndpoint, public PollingObject
{
public:
	FakeEndpoint(string description, EnRangeType rangeType,bool isInput);
	FakeEndpoint(ptree &pt);
	virtual void setValue(int64_t value);
	virtual bool isValid();
	// inherited from PollingObject
	virtual bool updatesAvailable();
	virtual void postUpdates();
	static shared_ptr<Endpoint> create(shared_ptr<Config> config, ptree &pt);
};

}

#endif
