#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include "ScalarEndpoint.hpp"
#include "EndpointWidget.hpp"
#include "common.hpp"

/** @todo (hephaisto#1#): Add thread synchronization to widgets (Wt-threads AND heinz-threads access this class!) */


namespace heinz
{

class EndpointOnOffButtonWidget : public EndpointWidget, public ScalarEndpointObserver
{
public:
	EndpointOnOffButtonWidget(ScalarEndpoint *endpoint, Wt::WContainerWidget *parent=NULL);
	virtual ~EndpointOnOffButtonWidget();
	void btnOnClicked();
	void btnOffClicked();
	virtual void internalUpdate(int64_t value);
protected:
	Wt::WPushButton *btnOn;
	Wt::WPushButton *btnOff;
};

}
