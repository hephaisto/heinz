#include "EndpointWidget.hpp"

#include <Wt/WText>

namespace heinz
{

EndpointWidget::EndpointWidget(Endpoint *endpoint,Wt::WContainerWidget *parent)
:Wt::WContainerWidget(parent)
{
	Wt::WText *text=new Wt::WText(endpoint->getDescription(),this);
	text->addStyleClass("endpoint_title");
}

}
