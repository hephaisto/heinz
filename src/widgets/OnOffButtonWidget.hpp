#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include "../ScalarEndpoint.hpp"
#include "EndpointWidget.hpp"
#include "../common.hpp"

/** @todo (hephaisto#1#): Add thread synchronization to widgets (Wt-threads AND heinz-threads access this class!) */


namespace heinz
{

class OnOffButtonWidget : public EndpointWidget, public ScalarEndpoint::ObservingWidget
{
public:
	OnOffButtonWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent=NULL);
	virtual ~OnOffButtonWidget();
	void btnOnClicked();
	void btnOffClicked();
	virtual void internalUpdate();
protected:
	Wt::WPushButton *btnOn;
	Wt::WPushButton *btnOff;
	shared_ptr<ScalarEndpoint> endpoint;
};

}
