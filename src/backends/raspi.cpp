#include <boost/log/trivial.hpp>
#include <wiringPi.h>
#include "raspi.hpp"
#include "../exceptions.hpp"

namespace heinz
{

bool EndpointRaspberry::initialized=false;

EndpointRaspberry::EndpointRaspberry(string description, int pinNumber, bool input)
:HardwareEndpoint(description, RANGE_U1,input),
pinNumber(pinNumber)
{
	if(!initialized)
	{
		if(wiringPiSetup())
			throw HeinzException("unable to initialize wiringPi");
		initialized=true;
	}
	pinMode(pinNumber,input?INPUT:OUTPUT);
}

EndpointRaspberry::EndpointRaspberry(ptree &pt)
:HardwareEndpoint(pt),
pinNumber(pt.get<int>("pin")),
invert(pt.get<bool>("invert",false))
{
	if(!initialized)
	{
		if(wiringPiSetup())
			throw HeinzException("unable to initialize wiringPi");
		initialized=true;
	}
	pinMode(pinNumber,getIsInput()?INPUT:OUTPUT);
}

void EndpointRaspberry::setValue(int64_t value)
{
	checkValueForValidity(value);
	digitalWrite(pinNumber,(invert?!value:value)?HIGH:LOW);
	{
		boost::unique_lock<EndpointRaspberry> guard(*this);
		this->cachedValue=value;
	}
	triggerUpdates();
}

bool EndpointRaspberry::isValid()
{
	return true;
}

void EndpointRaspberry::poll()
{
	if(getIsInput())
		setCachedValue(digitalRead(pinNumber)==HIGH?1:0);
}

bool EndpointRaspberry::updatesAvailable()
{
	if(!input)
		return false;
	// TODO: check for updates
	return false;
}
void EndpointRaspberry::postUpdates()
{
	this->triggerUpdates();
}

/*
#ifndef ENABLE_BACKEND_RASPI

extern "C"
{

void wiringPiSetup()
{
	BOOST_LOG_TRIVIAL(info)<<"wiringPiSetup()";
}
void pinMode(int pinNumber,int mode)
{
	BOOST_LOG_TRIVIAL(info)<<"pinMode("<<pinNumber<<","<<mode<<")";
}
void digitalWrite(int pinNumber,int value)
{
	BOOST_LOG_TRIVIAL(info)<<"digitalWrite("<<pinNumber<<","<<value<<")";
}
int digitalRead(int pinNumber)
{
	BOOST_LOG_TRIVIAL(info)<<"digitalRead("<<pinNumber<<")";
	return 0;
}

}	// extern "C"


#endif // ENABLE_BACKEND_RASPI
*/

}	// namespace
