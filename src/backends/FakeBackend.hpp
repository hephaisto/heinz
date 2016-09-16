#ifndef FAKE_ENDPOINT_H
#define FAKE_ENDPOINT_H

#include "../HardwareEndpoint.hpp"
#include "../config/config_loader.hpp"
#include "BackendPlugin.hpp"

namespace heinz
{

class FakeBackend : public BackendPlugin
{
public:
	virtual void backendConfig(ptree &pt);
	virtual shared_ptr<Endpoint> createEndpoint(shared_ptr<Config> config, ptree &pt);
};


/**
A fake endpoint class which just prints status changes to the log.
**/
class FakeEndpoint : public HardwareEndpoint, public PollingObject
{
public:
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
