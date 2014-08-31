#include "EndpointWidget.hpp"

#include <Wt/WText>

namespace heinz
{

EndpointWidget::EndpointWidget(shared_ptr<Endpoint> endpoint,Wt::WContainerWidget *parent)
:Wt::WContainerWidget(parent)
{
	Wt::WText *text=new Wt::WText(endpoint->getDescription(),this);
	text->addStyleClass("endpoint_title");
	widgetContainer=new Wt::WContainerWidget(this);
	widgetContainer->addStyleClass("endpoint_widget_container");
}

}
