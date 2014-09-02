#ifndef SCALAR_ENDPOINT_H
#define SCALAR_ENDPOINT_H

#include <boost/property_tree/ptree.hpp>

#include "common.hpp"
#include "constants.hpp"
#include "Endpoint.hpp"

namespace heinz
{

class ScalarEndpoint;

using boost::property_tree::ptree;
namespace bs2=boost::signals2;

/**
Base class for all endpoints which have scalar (or say: integral) values.
**/
class ScalarEndpoint : public Endpoint,  public std::enable_shared_from_this<ScalarEndpoint>
{
public:
	class ObservingWidget : public Observer
	{
	public:
		ObservingWidget(shared_ptr<ScalarEndpoint> endpoint);
		//virtual ~ObservingWidget();
		virtual void update();
	private:
		string sessionID;
	};
	ScalarEndpoint(string description, EnRangeType rangeType,bool isInput);
	ScalarEndpoint(ptree &pt);
	ScalarEndpoint(ptree &pt, bool isInput);
	shared_ptr<ScalarEndpoint> getSharedScalarEndpoint();
	virtual Wt::WContainerWidget* addEndpointWidgetToContainer(Wt::WContainerWidget *parent);
	virtual void setValue(int64_t value)=0;
	virtual void switchOnOff();
	virtual int64_t getValue();
	bool getIsInput();
	EnRangeType getRange();
	void checkValueForValidity(int64_t value);

	UpdateSignal& getUpdateSignal();

	void webButtonClicked(Wt::WPushButton *btn);
protected:
	UpdateSignal updateSignal;
	int64_t cachedValue;
	EnRangeType rangeType;
	bool isInput;
	void triggerUpdates();
};


}
#endif
