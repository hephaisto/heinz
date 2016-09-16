#ifndef BACKEND_RASPI_H
#define BACKEND_RASPI_H

#include <set>
using std::set;

#include "BackendPlugin.hpp"
#include "../HardwareEndpoint.hpp"

namespace heinz
{

class RaspberryPlugin : public BackendPlugin
{
public:
	RaspberryPlugin();
	virtual void backendConfig(ptree &pt);
	virtual shared_ptr<Endpoint> createEndpoint(shared_ptr<Config> config, ptree &pt);
	set<int> allocatedPins;
private:
	bool initialized;
};

class EndpointRaspberry : public HardwareEndpoint, public PollingObject
{
public:
	EndpointRaspberry(ptree &pt);
	virtual ~EndpointRaspberry();
	virtual void setValue(int64_t value);
	virtual bool isValid();

	// inherited from PollingObject
	virtual bool updatesAvailable();
	virtual void postUpdates();
	static shared_ptr<Endpoint> create(shared_ptr<Config> config, ptree &pt);
private:
	int pinNumber;
	bool invert;
};

}



#endif // BACKEND_RASPI_H
