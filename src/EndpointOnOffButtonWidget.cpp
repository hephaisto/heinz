#include "EndpointOnOffButtonWidget.hpp"

#include <Wt/WApplication>
#include <iostream>

namespace heinz
{
EndpointOnOffButtonWidget::EndpointOnOffButtonWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent)
:EndpointWidget(endpoint,parent),
ScalarEndpoint::ObservingWidget(endpoint),
endpoint(endpoint)
{
	btnOn=new Wt::WPushButton("ON",widgetContainer);
	btnOff=new Wt::WPushButton("OFF",widgetContainer);

	btnOn->clicked().connect(this,&EndpointOnOffButtonWidget::btnOnClicked);
	btnOff->clicked().connect(this,&EndpointOnOffButtonWidget::btnOffClicked);

	if(endpoint->isValid())
	{
		if(endpoint->getValue())
			btnOn->setEnabled(false);
		else
			btnOff->setEnabled(false);
	}
	else
	{
		// show lonebuttons
	}
}

EndpointOnOffButtonWidget::~EndpointOnOffButtonWidget()
{
}

void EndpointOnOffButtonWidget::btnOnClicked()
{
	endpoint->setValue(1);
	internalUpdate();
}
void EndpointOnOffButtonWidget::btnOffClicked()
{
	endpoint->setValue(0);
	internalUpdate();
}

void EndpointOnOffButtonWidget::internalUpdate()
{
	if(endpoint->isValid())
	{
		if(endpoint->getValue())
		{
			btnOn->setEnabled(false);
			btnOff->setEnabled(true);
		}
		else
		{
			btnOn->setEnabled(true);
			btnOff->setEnabled(false);
		}
	}
	else
	{
		btnOn->setEnabled(true);
		btnOff->setEnabled(true);
	}
	Wt::WApplication::instance()->triggerUpdate();
}


}
