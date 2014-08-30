#include "ScalarEndpoint.hpp"
#include "config/config_loader.hpp"

namespace heinz
{

class MultiplexerEndpoint;

class MultiplexerObserverHelper : public ScalarEndpointObserver
{
public:
	MultiplexerObserverHelper(string sessionID, ScalarEndpoint* observable, MultiplexerEndpoint *multiplexer);
	virtual void internalUpdate(int64_t value);
protected:
	MultiplexerEndpoint *multiplexer;
};

class MultiplexerEndpoint : public ScalarEndpoint
{
public:
	MultiplexerEndpoint(string description, EnRangeType rangeType, vector<shared_ptr<ScalarEndpoint> > endpoints);
	MultiplexerEndpoint(ptree &pt, shared_ptr<Config> config);
	virtual void setValue(int64_t value, ScalarEndpointObserver *source);
	virtual int64_t getValue();
	virtual bool isValid();
private:
	friend void MultiplexerObserverHelper::internalUpdate(int64_t value);
	void subEndpointChanged();

	vector<shared_ptr<ScalarEndpoint> > endpoints;
	vector<shared_ptr<MultiplexerObserverHelper> > subObservers;
};

}
