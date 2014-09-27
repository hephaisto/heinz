#include "ScalarEndpoint.hpp"
#include "config/config_loader.hpp"

namespace heinz
{

class MultiplexerEndpoint;

class MultiplexerEndpoint : public ScalarEndpoint
{
public:
	static shared_ptr<Endpoint> create(shared_ptr<Config> config, ptree &pt);
	shared_ptr<MultiplexerEndpoint> getSharedMultiplexerEndpoint();
	virtual ~MultiplexerEndpoint();

	virtual void setValue(int64_t value);
	virtual int64_t getValue();
	virtual bool isValid();
	void internalUpdate();
protected:
	vector<bs2::connection> signalConnections;
private:
	MultiplexerEndpoint(string description, EnRangeType rangeType, vector<shared_ptr<ScalarEndpoint> > endpoints);
	MultiplexerEndpoint(ptree &pt, shared_ptr<Config> config);

	//friend void MultiplexerObserverHelper::internalUpdate();
	void subEndpointChanged();
	void addSubEndpoint(string sub_name, shared_ptr<ScalarEndpoint> sub_endpoint);

	vector<shared_ptr<ScalarEndpoint> > endpoints;
	//vector<shared_ptr<MultiplexerObserverHelper> > subObservers;
};

}
