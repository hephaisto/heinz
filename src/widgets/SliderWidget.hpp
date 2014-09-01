#include <Wt/WContainerWidget>
#include <Wt/WSlider>
#include "../ScalarEndpoint.hpp"
#include "EndpointWidget.hpp"
#include "../common.hpp"

/** @todo (hephaisto#1#): Add thread synchronization to widgets (Wt-threads AND heinz-threads access this class!) */


namespace heinz
{

class SliderWidget : public EndpointWidget, public ScalarEndpoint::ObservingWidget
{
public:
	SliderWidget(shared_ptr<ScalarEndpoint> endpoint, Wt::WContainerWidget *parent=NULL);
	virtual ~SliderWidget();
	void sliderUpdated(int value);
	virtual void internalUpdate();
protected:
	Wt::WSlider *slider;
	shared_ptr<ScalarEndpoint> endpoint;
};

}
