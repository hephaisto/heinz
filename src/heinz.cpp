#include <boost/log/trivial.hpp>
#include "heinz.hpp"
#include <Wt/WServer>
#include <Wt/WIOService>
#include <boost/foreach.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include "exceptions.hpp"
#include "python/python_wrapper.hpp"

namespace heinz
{


shared_ptr<Config> Heinz::getConfig()
{
	return config;
}

void Heinz::pollingLoop()
{
	BOOST_LOG_TRIVIAL(info)<<"starting polling loop";
	while(true)
	{
		BOOST_FOREACH(shared_ptr<PollingObject> p,config->pollingObjects)
		{
			if(p->updatesAvailable())
			{
				//BOOST_LOG_TRIVIAL(debug)<<"updates available for endpoint "<<p->getDescription();
				Wt::WServer::instance()->ioService().post(boost::bind(&PollingObject::postUpdates, p.get()));
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
Heinz::Heinz()
:config(load_config())
{
	if(singletonInstance)
		throw HeinzException("There is already a heinz instance!");
	initPython();
	singletonInstance=this;
	pollingThread=boost::thread(&Heinz::pollingLoop,this);
}
Heinz::~Heinz()
{
	pollingThread.interrupt();
	pollingThread.join();
	singletonInstance=NULL;
}
shared_ptr<Endpoint> Heinz::getEndpoint(string name)
{
	try
	{
		return config->endpoints.at(name);
	}
	catch(std::out_of_range)
	{
		throw HeinzException((boost::format("endpoint %1% not found")%name).str());
	}
}
shared_ptr<ScalarEndpoint> Heinz::getScalarEndpoint(string name)
{
	shared_ptr<Endpoint> ep=getEndpoint(name);
	shared_ptr<ScalarEndpoint> sep=std::static_pointer_cast<ScalarEndpoint>(ep);
	if(!sep)
		throw HeinzException((boost::format("%1% is not a scalar endpoint")%name).str());
	return sep;
}
Heinz* Heinz::singletonInstance=NULL;
shared_ptr<Heinz> Heinz::getInstance()
{
	if(!singletonInstance)
		throw HeinzException("No heinz instance available");
	return singletonInstance->shared_from_this();
}

}
