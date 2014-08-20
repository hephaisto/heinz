#ifndef SCALAR_ENDPOINT_H
#define SCALAR_ENDPOINT_H

#include <boost/property_tree/ptree.hpp>

#include "common.hpp"
#include "constants.hpp"
#include "Endpoint.hpp"

namespace heinz
{

class ScalarEndpoint;
class ScalarEndpointObserver;

using boost::property_tree::ptree;

/**
Base class for all endpoints which have scalar (or say: integral) values.
**/
class ScalarEndpoint : public Endpoint
{
public:
	ScalarEndpoint(string description, EnRangeType rangeType,bool isInput);
	ScalarEndpoint(ptree &pt);
	ScalarEndpoint(ptree &pt, bool isInput);
	virtual Wt::WContainerWidget* addEndpointWidgetToContainer(Wt::WContainerWidget *parent);
	virtual void setValue(int64_t value, ScalarEndpointObserver *source)=0;
	virtual int64_t getValue();
	bool getIsInput();
	EnRangeType getRange();
	void checkValueForValidity(int64_t value);

	void registerObserver(ScalarEndpointObserver *observer);
	void unRegisterObserver(ScalarEndpointObserver *observer);

	void webButtonClicked(Wt::WPushButton *btn);
protected:
	vector<ScalarEndpointObserver*> observers;
	int64_t cachedValue;
	EnRangeType rangeType;
	bool isInput;
	void triggerUpdates(ScalarEndpointObserver *observer);
};

/**
Base class for all widgets that respond to endpoint updates.

This class registers itself as an observer by the corresponding endpoint and gets updates from it whenever it changes.
**/
class ScalarEndpointObserver
{
public:
	ScalarEndpointObserver(string sessionID, ScalarEndpoint* endpoint);
	virtual ~ScalarEndpointObserver();
	void update(int64_t value);
protected:
	ScalarEndpoint *endpoint;
	virtual void internalUpdate(int64_t value)=0;
	string sessionID;
};

}
#endif
