#include "ScalarEndpoint.hpp"
#include "config/config_loader.hpp"

namespace heinz
{

class MultiplexerEndpoint;

class MultiplexerObserverHelper : public ScalarEndpointObserver
{
public:
	MultiplexerObserverHelper(shared_ptr<ScalarEndpoint> observable, weak_ptr<MultiplexerEndpoint> multiplexer);
	virtual void internalUpdate();
protected:
	weak_ptr<MultiplexerEndpoint> multiplexer;
	shared_ptr<ScalarEndpoint> observable;
};

class MultiplexerEndpoint : public ScalarEndpoint
{
public:
	static shared_ptr<MultiplexerEndpoint> createMultiplexerEndpoint(ptree &pt, shared_ptr<Config> config);
	shared_ptr<MultiplexerEndpoint> getSharedMultiplexerEndpoint();

	virtual void setValue(int64_t value, ScalarEndpointObserver *source);
	virtual int64_t getValue();
	virtual bool isValid();
private:
	MultiplexerEndpoint(string description, EnRangeType rangeType, vector<shared_ptr<ScalarEndpoint> > endpoints);
	MultiplexerEndpoint(ptree &pt, shared_ptr<Config> config);

	friend void MultiplexerObserverHelper::internalUpdate();
	void subEndpointChanged();
	void addSubEndpoint(string sub_name, shared_ptr<ScalarEndpoint> sub_endpoint);

	vector<shared_ptr<ScalarEndpoint> > endpoints;
	vector<shared_ptr<MultiplexerObserverHelper> > subObservers;
};

}
