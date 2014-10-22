#ifndef BACKEND_RASPI_H
#define BACKEND_RASPI_H

#include <set>
using std::set;

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
	shared_ptr<EndpointRaspberry> create(shared_ptr<Config> config, ptree &pt);
private:
	static bool initialized;
	int pinNumber;
	bool invert;
	static set<int> allocatedPins;
};

}



#endif // BACKEND_RASPI_H
