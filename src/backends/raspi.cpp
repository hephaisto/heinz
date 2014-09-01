#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

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
	string pull_mode_s=pt.get<string>("pull","off");
	int pull_mode=PUD_OFF;
	if(pull_mode_s=="off")
		pull_mode=PUD_OFF;
	else if(pull_mode_s=="up")
		pull_mode=PUD_UP;
	else if(pull_mode_s=="down")
		pull_mode=PUD_DOWN;
	else
		throw ConfigException((boost::format("unknown pull mode: %1%")%pull_mode_s).str());
	pinMode(pinNumber,getIsInput()?INPUT:OUTPUT);
	if(getIsInput())
		pullUpDnControl(pinNumber, pull_mode);
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

bool EndpointRaspberry::updatesAvailable()
{
	if(!getIsInput())
		return false;
	bool v=digitalRead(pinNumber)==HIGH;
	if((cachedValue==1)!=v)
	{
		cachedValue=v?1:0;
		return true;
	}
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
