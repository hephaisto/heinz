#include "EndpointOnOffButtonWidget.hpp"

#include <Wt/WApplication>
#include <iostream>

namespace heinz
{
EndpointOnOffButtonWidget::EndpointOnOffButtonWidget(ScalarEndpoint *endpoint, Wt::WContainerWidget *parent)
:EndpointWidget(endpoint,parent),
ScalarEndpointObserver(Wt::WApplication::instance()->sessionId(),endpoint)
{
	btnOn=new Wt::WPushButton("ON",this);
	btnOff=new Wt::WPushButton("OFF",this);

	btnOn->clicked().connect(this,&EndpointOnOffButtonWidget::btnOnClicked);
	btnOff->clicked().connect(this,&EndpointOnOffButtonWidget::btnOffClicked);

	if(endpoint->isValid())
	{
		if(endpoint->getValue())
			btnOn->hide();
		else
			btnOff->hide();
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
	endpoint->setValue(1,this);
	internalUpdate(1);
}
void EndpointOnOffButtonWidget::btnOffClicked()
{
	endpoint->setValue(0,this);
	internalUpdate(0);
}

void EndpointOnOffButtonWidget::internalUpdate(int64_t value)
{
	std::cerr<<"internal update OnOffButtonWidget "<<value<<"\n";
	if(value)
	{
		btnOn->hide();
		btnOff->show();
	}
	else
	{
		btnOn->show();
		btnOff->hide();
	}
	Wt::WApplication::instance()->triggerUpdate();
}


}
