#include "OnOffButtonWidget.hpp"

#include <Wt/WApplication>
#include <iostream>

namespace heinz
{
OnOffButtonWidget::OnOffButtonWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent)
:EndpointWidget(endpoint,parent),
ScalarEndpoint::ObservingWidget(endpoint),
endpoint(endpoint)
{
	btnOn=new Wt::WPushButton("ON",widgetContainer);
	btnOff=new Wt::WPushButton("OFF",widgetContainer);

	if(!endpoint->getIsInput())
	{
		btnOn->clicked().connect(this,&OnOffButtonWidget::btnOnClicked);
		btnOff->clicked().connect(this,&OnOffButtonWidget::btnOffClicked);
	}

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

OnOffButtonWidget::~OnOffButtonWidget()
{
}

void OnOffButtonWidget::btnOnClicked()
{
	endpoint->setValue(1);
	internalUpdate();
}
void OnOffButtonWidget::btnOffClicked()
{
	endpoint->setValue(0);
	internalUpdate();
}

void OnOffButtonWidget::internalUpdate()
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
