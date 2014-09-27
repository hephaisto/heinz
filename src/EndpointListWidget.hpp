#include <Wt/WContainerWidget>
#include "Endpoint.hpp"
#include <memory>
using std::shared_ptr;
#include <vector>
using std::vector;

namespace heinz
{

/**
Widget that holds a list of endpoint widgets.
**/
class EndpointListWidget : public Wt::WContainerWidget
{
public:
	EndpointListWidget(vector<shared_ptr<Endpoint> > endpoints, Wt::WContainerWidget *parent=NULL);
protected:
	vector<shared_ptr<Endpoint> > endpoints;
};

}
