#include "HardwareEndpoint.hpp"

namespace heinz
{

HardwareEndpoint::HardwareEndpoint(string description, EnRangeType rangeType,bool isInput)
:ScalarEndpoint(description,rangeType,isInput)
{}

HardwareEndpoint::HardwareEndpoint(ptree &pt)
:ScalarEndpoint(pt)
{}

void HardwareEndpoint::setCachedValue(int64_t value)
{
	cachedValue=value;
}

}
