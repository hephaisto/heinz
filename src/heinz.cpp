#include "heinz.hpp"

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
		BOOST_FOREACH(shared_ptr<PollingObject> p,pollingObjects)
		{
			if(p->updatesAvailable())
				std::cerr<<"updates available! - post to thread pool here";	// TODO
		}
		boost::this_thread::interruption_point();	// allow interrupting
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
:config(load_config(configFilename))
{}

}
