#ifndef ENDPOINT_WIDGET_H
#define ENDPOINT_WIDGET_H
#include <Wt/WContainerWidget>

#include "Endpoint.hpp"

namespace heinz
{

class EndpointWidget : public Wt::WContainerWidget
{
public:
	EndpointWidget(Endpoint *endpoint, Wt::WContainerWidget *parent=NULL);
	//virtual ~EndpointWidget();
};

}

#endif // ENDPOINT_WIDGET_H
