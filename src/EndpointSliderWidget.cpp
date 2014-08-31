#include "EndpointSliderWidget.hpp"

#include <Wt/WApplication>
#include <iostream>

namespace heinz
{
EndpointSliderWidget::EndpointSliderWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent)
:EndpointWidget(endpoint,parent),
ScalarEndpointObserver(Wt::WApplication::instance()->sessionId(),endpoint)
{
	slider=new Wt::WSlider(Wt::Horizontal,widgetContainer);
	slider->setRange(0,255);
	slider->setValue(endpoint->getValue());
	slider->sliderMoved().connect(this,&EndpointSliderWidget::sliderUpdated);
	//slider->resize(300, 50);
	//slider->setTickInterval(5);
	//slider->setTickPosition(Wt::WSlider::TicksBothSides);
	//scaleSlider->valueChanged().connect(this, &ThisClass::scaleShape);
	//slider->setNativeControl(true);
}

EndpointSliderWidget::~EndpointSliderWidget()
{
}

void EndpointSliderWidget::sliderUpdated(int value)
{
	std::cerr<<"slider update received...\n";
	endpoint->setValue(value,this);
}

void EndpointSliderWidget::internalUpdate()
{
	if(endpoint->isValid())
	{
		slider->setValue(endpoint->getValue());
		Wt::WApplication::instance()->triggerUpdate();
	}
}


}
