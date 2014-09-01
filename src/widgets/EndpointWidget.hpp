#ifndef ENDPOINT_WIDGET_H
#define ENDPOINT_WIDGET_H
#include <Wt/WContainerWidget>

#include "../Endpoint.hpp"
#include "../common.hpp"

namespace heinz
{

class EndpointWidget : public Wt::WContainerWidget
{
public:
	EndpointWidget(shared_ptr<Endpoint> endpoint, Wt::WContainerWidget *parent=NULL);
	//virtual ~EndpointWidget();
protected:
	Wt::WContainerWidget *widgetContainer;
};

}

#endif // ENDPOINT_WIDGET_H
