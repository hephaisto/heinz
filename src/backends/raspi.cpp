#include <boost/log/trivial.hpp>
#include <boost/format.hpp>

#ifdef FAKE_WIRINGPI
bool wiringPiSetup(){return false;}
void digitalWrite(int, int){}
int digitalRead(int){return 0;}
void pinMode(int, int){}
void pullUpDnControl(int, int){}
enum {INPUT, OUTPUT, PUD_UP, PUD_DOWN, PUD_OFF, HIGH, LOW};
#else
#include <wiringPi.h>
#endif
#include "raspi.hpp"
#include "../exceptions.hpp"

namespace heinz
{

namespace
{
	std::unique_ptr<RaspberryPlugin> backendInstance(new RaspberryPlugin);
	PluginRegistrar<BackendPlugin> registrar("raspi", backendInstance.get());
}


RaspberryPlugin::RaspberryPlugin()
:initialized(false)
{
}

void RaspberryPlugin::backendConfig(ptree &pt)
{
	if(wiringPiSetup())
		throw HeinzException("unable to initialize wiringPi");
	initialized=true;
}

shared_ptr<Endpoint> RaspberryPlugin::createEndpoint(shared_ptr<Config> config, ptree &pt)
{
	if(!initialized)
	{
		throw HeinzException("raspberry plugin not initialized before endpoint definition");
	}
	shared_ptr<EndpointRaspberry> tmp(new EndpointRaspberry(pt));
	if(tmp->getIsInput())
	{
		config->pollingObjects.push_back(tmp);
		tmp->setScriptIfAvailable(pt);
	}
	return tmp;
}

EndpointRaspberry::EndpointRaspberry(ptree &pt)
:HardwareEndpoint(pt),
pinNumber(pt.get<int>("pin")),
invert(pt.get<bool>("invert",false))
{
	string pull_mode_s=pt.get<string>("pull","off");
	int pull_mode=PUD_OFF;
	if(pull_mode_s=="off")
		pull_mode=PUD_OFF;
	else if(pull_mode_s=="up")
		pull_mode=PUD_UP;
	else if(pull_mode_s=="down")
		pull_mode=PUD_DOWN;
	else
		BOOST_THROW_EXCEPTION(ConfigException()<<ExErrorMessage((boost::format("unknown pull mode: %1%")%pull_mode_s).str()));

	if(backendInstance->allocatedPins.find(pinNumber) != backendInstance->allocatedPins.end())
		BOOST_THROW_EXCEPTION(ConfigException()<<ExErrorMessage((boost::format("pin %1% already in use")%pinNumber).str()));
	backendInstance->allocatedPins.insert(pinNumber);

	pinMode(pinNumber,getIsInput()?INPUT:OUTPUT);
	if(getIsInput())
		pullUpDnControl(pinNumber, pull_mode);
}
EndpointRaspberry::~EndpointRaspberry()
{
	backendInstance->allocatedPins.erase(pinNumber);
}

void EndpointRaspberry::setValue(int64_t value)
{
	checkValueForValidity(value);
	digitalWrite(pinNumber,(invert?!value:value)?HIGH:LOW);
	{
		boost::unique_lock<EndpointRaspberry> guard(*this);
		this->cachedValue=value;
	}
	BOOST_LOG_TRIVIAL(info)<<"endpoint "<<getDescription()<<" set to "<<value;
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
	triggerUpdates();
	executeScript();
}

}	// namespace
