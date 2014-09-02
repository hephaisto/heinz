#ifndef ENDPOINT_H
#define ENDPOINT_H

#include <string>
using std::string;
#include <Wt/WContainerWidget>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/signals2.hpp>

namespace heinz
{
namespace bs2=boost::signals2;


class Endpoint : public boost::shared_lockable_adapter<boost::shared_mutex>
{
public:
	typedef bs2::signal<void ()> UpdateSignal;
	class Observer
	{
	public:
		Observer(bs2::connection connection);
		virtual ~Observer();
	protected:
		virtual void internalUpdate()=0;
		//shared_ptr<ScalarEndpoint> endpoint;
		virtual void update()=0;
	protected:
		bs2::connection connection;
	};
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
	/**
	Checks whether updates are available for this object.

	This will be checked by the global polling thread.
	**/
	virtual bool updatesAvailable()=0;
	/**
	Posts updates to the system.

	This will be run from one of the threads in the worker pool, since duration of the invocation can possibly be very long.
	**/
	virtual void postUpdates()=0;
	void executeScript();
	void setScript(string command);
private:
	string command;
};

}

#endif
