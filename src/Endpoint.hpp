#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
using std::string;
#include <Wt/WContainerWidget>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/shared_mutex.hpp>

namespace heinz
{

class Endpoint : public boost::shared_lockable_adapter<boost::shared_mutex>
{
public:
	Endpoint(string description);
	/**
	Adds a widget to control this endpoint to the specified parent Widget.
	@param parent Widget used as a parent for the new widget. Can be NULL to create a parent-less widget (that can be added later).
	@returns A pointer to the newly created widget.
	**/
	virtual Wt::WContainerWidget* addEndpointWidgetToContainer(Wt::WContainerWidget *parent)=0;
	/**
	Checks whether this endpoint has a valid status which can be read out.

	Examples for an invalid status: A multiplexer Endpoint with one on-endpoint und one off-endpoint.
	**/
	virtual bool isValid()=0;
	string getDescription();
protected:
	string description;
};

class PollingObject
{
public:
	virtual void poll()=0;
};

}

#endif
