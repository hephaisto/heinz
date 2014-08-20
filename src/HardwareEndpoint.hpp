#ifndef HARDWARE_ENDPOINT_H
#define HARDWARE_ENDPOINT_H

#include "ScalarEndpoint.hpp"

namespace heinz
{

/**
Base class for all endpoints which actually have a hardware equivalent (opposed to virtual endpoints, which don't).
**/
class HardwareEndpoint : public ScalarEndpoint
{
public:
	HardwareEndpoint(string description, EnRangeType rangeType,bool isInput);
	HardwareEndpoint(ptree &pt);
	void setCachedValue(int64_t value);
};

}

#endif
