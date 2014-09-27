#ifndef BACKEND_RASPI_H
#define BACKEND_RASPI_H

#include "../HardwareEndpoint.hpp"

namespace heinz
{

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
private:
	static bool initialized;
	int pinNumber;
	bool invert;
	static set<int> allocatedPins;
};

}



#endif // BACKEND_RASPI_H
