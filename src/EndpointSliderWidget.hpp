#include <Wt/WContainerWidget>
#include <Wt/WSlider>
#include "ScalarEndpoint.hpp"
#include "EndpointWidget.hpp"
#include "common.hpp"

/** @todo (hephaisto#1#): Add thread synchronization to widgets (Wt-threads AND heinz-threads access this class!) */


namespace heinz
{

class EndpointSliderWidget : public EndpointWidget, public ScalarEndpointObserver
{
public:
	EndpointSliderWidget(ScalarEndpoint *endpoint, Wt::WContainerWidget *parent=NULL);
	virtual ~EndpointSliderWidget();
	void sliderUpdated(int value);
	virtual void internalUpdate(int64_t value);
protected:
	Wt::WSlider *slider;
};

}
