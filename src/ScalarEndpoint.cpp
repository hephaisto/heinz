#include "ScalarEndpoint.hpp"

#include <Wt/WPushButton>
#include <Wt/WSlider>
#include <Wt/WServer>

#include "exceptions.hpp"
#include "EndpointSliderWidget.hpp"
#include "EndpointOnOffButtonWidget.hpp"

namespace heinz
{

/**
@param rangeType The range type (e.g. boolean, int16, ... this endpoint represents)
@param isInput true for sensors, false for actors
**/
ScalarEndpoint::ScalarEndpoint(string description, EnRangeType rangeType,bool isInput)
:Endpoint(description),
rangeType(rangeType),
isInput(isInput)
{}

ScalarEndpoint::ScalarEndpoint(ptree &pt, bool isInput)
:Endpoint(pt.get<string>("description")),
rangeType(rangeByString[pt.get<string>("range")]),
isInput(isInput)
{}

ScalarEndpoint::ScalarEndpoint(ptree &pt)
:Endpoint(pt.get<string>("description")),
rangeType(rangeByString[pt.get<string>("range")]),
isInput(pt.get<bool>("input"))
{}

/**
If used on a sensor endpoint or with an invalid input range, it will throw an exception.
@param value The new value. Use 0/1 for booleans.
**/
void ScalarEndpoint::checkValueForValidity(int64_t value)
{
	if(getIsInput())
		throw InputEndpointValueSetException();
	boost::shared_lock<ScalarEndpoint> guard(*this);
	switch(rangeType)
	{
	case RANGE_U1:
	{
		if( (value!=0) && (value!=1) )
			throw InvalidEndpointRangeException(rangeType,value);
	}break;
	case RANGE_U8:
	{
		if( (value<0) || (value>255) )
			throw InvalidEndpointRangeException(rangeType,value);
	}break;
	case RANGE_S8:
	{
		if( (value<-128) || (value>127) )
			throw InvalidEndpointRangeException(rangeType,value);
	}break;
	default:
		throw InvalidStateDetectedException("invalid range type");
	}
}

/**
Return the current (cached!) value for this endpoint.
**/
int64_t ScalarEndpoint::getValue()
{
	boost::shared_lock<ScalarEndpoint> guard(*this);
	return cachedValue;
}

/**
Tell whether this is a sensor (true) or actor (false) endpoint.
**/
bool ScalarEndpoint::getIsInput()
{
	boost::shared_lock<ScalarEndpoint> guard(*this);
	return isInput;
}

/**
Get the possible data-range for this endpoint.
**/
EnRangeType ScalarEndpoint::getRange()
{
	boost::shared_lock<ScalarEndpoint> guard(*this);
	return rangeType;
}

//void ScalarEndpoint::webButtonClicked(Wt::WPushButton *btn)
//{
//	if(rangeType!=RANGE_U1)
//		throw InvalidStateDetectedException("button click on non-boolean endpoint");
//	this->setValue(cachedValue?0:1);
//	btn->setChecked(cachedValue);
//}


Wt::WContainerWidget* ScalarEndpoint::addEndpointWidgetToContainer(Wt::WContainerWidget *parent)
{
	switch(rangeType)
	{
		case RANGE_U1:
		{
			return new EndpointOnOffButtonWidget(this,parent);
		}break;
		case RANGE_U8:
		case RANGE_S8:
		{
			return new EndpointSliderWidget(this, parent);
		}break;
		default:
			throw InvalidStateDetectedException("invalid range type");
	}

	/// @todo create widget
	/*
	switch(rangeType)
	{
	case RANGE_U1:
	{
		Wt::WPushButton *btn=new Wt::WPushButton("switch",w);
		btn->setCheckable(true);
		btn->setChecked(cachedValue);
		btn->clicked().connect(boost::bind(&ScalarEndpoint::webButtonClicked, this, btn));
	}break;
	case RANGE_U8:
	{
		Wt::WSlider *slider=new Wt::WSlider(Wt::Horizontal,w);
		slider->setRange(0,255);
		slider->setValue(cachedValue);
		slider->valueChanged().connect(boost::bind(&ScalarEndpoint::webSliderUpdated, this, slider));
		//slider->resize(300, 50);
		//slider->setTickInterval(5);
		//slider->setTickPosition(Wt::WSlider::TicksBothSides);
		//scaleSlider->valueChanged().connect(this, &ThisClass::scaleShape);
		//slider->setNativeControl(true);
	}break;
	case RANGE_S8:
	{
		Wt::WSlider *slider=new Wt::WSlider(Wt::Horizontal,w);
		slider->setRange(-128,127);
		slider->setValue(cachedValue);
		slider->resize(300, 50);
		slider->setNativeControl(true);
	}break;
	default:
		throw InvalidStateDetectedException("invalid range type");
	}

	return w;*/
}
void ScalarEndpoint::registerObserver(ScalarEndpointObserver *observer)
{
	boost::unique_lock<ScalarEndpoint> guard(*this);
	observers.push_back(observer);
	std::cerr<<"added observer; new count: "<<observers.size()<<"\n";
}
void ScalarEndpoint::unRegisterObserver(ScalarEndpointObserver *observer)
{
	boost::unique_lock<ScalarEndpoint> guard(*this);
	for(auto it=observers.begin();it!=observers.end();it++)
		if(*it==observer)
		{
			observers.erase(it);
			break;
		}
}
void ScalarEndpoint::triggerUpdates(ScalarEndpointObserver *observer)
{
	std::cerr<<description<<": triggerUpdates()\n";
	boost::shared_lock<ScalarEndpoint> guard(*this);
	std::cerr<<"***** starting to trigger updates for "<<observers.size()<<" observers\n";
	for(auto it=observers.begin();it!=observers.end();it++)
	{
		std::cerr<<"observer\n";
		if(*it==observer)
			continue;	// skip the source of the event; updating would deadlock!
		std::cerr<<"updating\n";
		(*it)->update(cachedValue);
	}
}

ScalarEndpointObserver::ScalarEndpointObserver(string sessionID,ScalarEndpoint* endpoint)
:endpoint(endpoint),
sessionID(sessionID)
{
	endpoint->registerObserver(this);
}
ScalarEndpointObserver::~ScalarEndpointObserver()
{
	endpoint->unRegisterObserver(this);
}
void ScalarEndpointObserver::update(int64_t value)
{
	Wt::WServer::instance()->post(sessionID, boost::bind(&ScalarEndpointObserver::internalUpdate,this,value));
}


}
