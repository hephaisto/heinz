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
	EndpointOnOffButtonWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent=NULL);
	virtual ~EndpointOnOffButtonWidget();
	void btnOnClicked();
	void btnOffClicked();
	virtual void internalUpdate();
protected:
	Wt::WPushButton *btnOn;
	Wt::WPushButton *btnOff;
};

}
