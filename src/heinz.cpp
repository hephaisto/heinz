#include "heinz.hpp"
#include <Wt/WServer>
#include <Wt/WIOService>
#include <boost/foreach.hpp>
#include <boost/thread/thread.hpp>

namespace heinz
{


shared_ptr<Config> Heinz::getConfig()
{
	return config;
}

void Heinz::pollingLoop()
{
	while(true)
	{
		BOOST_FOREACH(shared_ptr<PollingObject> p,config->pollingObjects)
		{
			if(p->updatesAvailable())
			{
				Wt::WServer::instance()->ioService().post(boost::bind(&PollingObject::postUpdates, p.get()));
				//std::cerr<<"updates available! - post to thread pool here";	// TODO
			}
		}
		boost::this_thread::sleep_for(boost::chrono::milliseconds(config->pollingInterval));	// wait, allow interrupting
	}
}
WebApp* Heinz::createApp(const Wt::WEnvironment &env)
{
	return new heinz::WebApp(env,config);
}
boost::function<WebApp* (const Wt::WEnvironment &env)> Heinz::getAppCreator()
{
	return boost::bind(&Heinz::createApp,this,_1);
}
Heinz::Heinz(string configFilename)
:config(load_config(configFilename)),
pollingThread(&Heinz::pollingLoop,this)
{
}
Heinz::~Heinz()
{
	pollingThread.interrupt();
	pollingThread.join();
}

}
