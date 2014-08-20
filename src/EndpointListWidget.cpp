#include "EndpointListWidget.hpp"

namespace heinz
{


EndpointListWidget::EndpointListWidget(vector<shared_ptr<Endpoint> > endpoints, Wt::WContainerWidget *parent)
:Wt::WContainerWidget(parent),
endpoints(endpoints)
{
	for(auto it=endpoints.begin();it!=endpoints.end();it++)
	{
		(*it)->addEndpointWidgetToContainer(this);
	}
}

}
