#include "ScalarEndpoint.hpp"

#include <Wt/WPushButton>
#include <Wt/WSlider>
#include <Wt/WServer>

#include "exceptions.hpp"
#include "widgets/SliderWidget.hpp"
#include "widgets/OnOffButtonWidget.hpp"

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

ScalarEndpoint::UpdateSignal& ScalarEndpoint::getUpdateSignal()
{
	return updateSignal;
}


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

shared_ptr<ScalarEndpoint> ScalarEndpoint::getSharedScalarEndpoint()
{
	shared_ptr<ScalarEndpoint> tmp=std::dynamic_pointer_cast<ScalarEndpoint>(shared_from_this());
	if(!tmp)
		throw std::bad_cast();
	return tmp;
}


Wt::WContainerWidget* ScalarEndpoint::addEndpointWidgetToContainer(Wt::WContainerWidget *parent)
{
	switch(rangeType)
	{
		case RANGE_U1:
		{
			return new OnOffButtonWidget(getSharedScalarEndpoint(),parent);
		}break;
		case RANGE_U8:
		case RANGE_S8:
		{
			return new SliderWidget(getSharedScalarEndpoint(), parent);
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
void ScalarEndpoint::triggerUpdates()
{
	std::cerr<<description<<": triggerUpdates()\n";
	boost::shared_lock<ScalarEndpoint> guard(*this);
	updateSignal();
}


ScalarEndpoint::Observer::Observer(bs2::connection connection)
://endpoint(endpoint)
connection(connection)
{}


ScalarEndpoint::Observer::~Observer()
{
	connection.disconnect();
}

void ScalarEndpoint::ObservingWidget::update()
{
	if((Wt::WApplication::instance()) && (sessionID==Wt::WApplication::instance()->sessionId()) )
		internalUpdate();
	else
		Wt::WServer::instance()->post(sessionID, boost::bind(&ScalarEndpoint::ObservingWidget::internalUpdate,this));
}

ScalarEndpoint::ObservingWidget::ObservingWidget(shared_ptr<ScalarEndpoint> endpoint)
:Observer(endpoint->getUpdateSignal().connect(boost::bind(&ScalarEndpoint::ObservingWidget::update,this))),
sessionID(Wt::WApplication::instance()->sessionId())
{}
/*void ScalarEndpointObserver::update()
{
	std::cerr<<"updating. this->sessionID="<<sessionID<<"\n";
	if((sessionID=="") || (Wt::WApplication::instance()->sessionId()==sessionID))	// no sessionID: non-widget observer; same sessionID: no posting necessary
		internalUpdate();
	else
		Wt::WServer::instance()->post(sessionID, boost::bind(&ScalarEndpointObserver::internalUpdate,this));
}*/


}
