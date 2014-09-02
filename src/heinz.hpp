#ifndef HEINZ_H
#define HEINZ_H

#include <boost/thread.hpp>
#include "Endpoint.hpp"
#include "common.hpp"
#include "config/config_loader.hpp"
#include "WebApp.hpp"

namespace heinz
{

class Heinz : public std::enable_shared_from_this<Heinz>, public boost::shared_lockable_adapter<boost::shared_mutex>
{
public:
	Heinz(string configFilename);
	virtual ~Heinz();
	shared_ptr<Config> getConfig();
	boost::function<WebApp* (const Wt::WEnvironment &env)> getAppCreator();
	shared_ptr<Endpoint> getEndpoint(string name);
	shared_ptr<ScalarEndpoint> getScalarEndpoint(string name);
	static shared_ptr<Heinz> getInstance();
private:
	WebApp* createApp(const Wt::WEnvironment &env);
	shared_ptr<Config> config;
	void pollingLoop();
	boost::thread pollingThread;
	static Heinz* singletonInstance;
};

}

#endif // HEINZ_H
