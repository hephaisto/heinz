#ifndef BACKEND_RASPI_H
#define BACKEND_RASPI_H

#include "../HardwareEndpoint.hpp"

namespace heinz
{

class EndpointRaspberry : public HardwareEndpoint, public PollingObject
{
public:
	EndpointRaspberry(string description, int pinNumber, bool input);
	EndpointRaspberry(ptree &pt);
	virtual void setValue(int64_t value);
	virtual bool isValid();

	// inherited from PollingObject
	virtual bool updatesAvailable();
	virtual void postUpdates();
private:
	static bool initialized;
	int pinNumber;
	bool invert;
};

}



#endif // BACKEND_RASPI_H
