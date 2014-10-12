#include "SliderWidget.hpp"

#include <Wt/WApplication>
#include <iostream>

namespace heinz
{
SliderWidget::SliderWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent)
:EndpointWidget(endpoint,parent),
ScalarEndpoint::ObservingWidget(endpoint),
endpoint(endpoint)
{
	slider=new Wt::WSlider(Wt::Horizontal,widgetContainer);
	slider->setRange(0,255);
	slider->setValue(endpoint->getValue());

	if(!endpoint->getIsInput())
	{
		slider->sliderMoved().connect(this,&SliderWidget::sliderUpdated);
	}
	//slider->resize(300, 50);
	//slider->setTickInterval(5);
	//slider->setTickPosition(Wt::WSlider::TicksBothSides);
	//scaleSlider->valueChanged().connect(this, &ThisClass::scaleShape);
	//slider->setNativeControl(true);
}

SliderWidget::~SliderWidget()
{
}

void SliderWidget::sliderUpdated(int value)
{
	endpoint->setValue(value);
}

void SliderWidget::internalUpdate()
{
	if(endpoint->isValid())
	{
		slider->setValue(endpoint->getValue());
		Wt::WApplication::instance()->triggerUpdate();
	}
}


}
