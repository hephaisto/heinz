#include <boost/thread.hpp>
#include "Endpoint.hpp"
#include "common.hpp"
#include "config/config_loader.hpp"
#include "WebApp.hpp"

namespace heinz
{

class Heinz
{
public:
	Heinz(string configFilename);
	virtual ~Heinz();
	shared_ptr<Config> getConfig();
	boost::function<WebApp* (const Wt::WEnvironment &env)> getAppCreator();
private:
	WebApp* createApp(const Wt::WEnvironment &env);
	shared_ptr<Config> config;
	void pollingLoop();
	boost::thread pollingThread;
};

}
