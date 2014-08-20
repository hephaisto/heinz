#include <Wt/WBootstrapTheme>
#include <boost/foreach.hpp>

#include "WebApp.hpp"
#include "EndpointListWidget.hpp"
#include "MultiplexerEndpoint.hpp"
#include "config/config_loader.hpp"

namespace heinz
{

WebApp::WebApp(const Wt::WEnvironment &env)
:Wt::WApplication(env)
{
	Wt::WBootstrapTheme *theme=new Wt::WBootstrapTheme();
	//theme->setVersion(3);
	setTheme(theme);
	Wt::WContainerWidget *tmp=new Wt::WContainerWidget(root());
	if(endpoints.empty())
	{
		try
		{
			Config config=load_config("config/heinz.conf");
			/*shared_ptr<ScalarEndpoint> ep1(new FakeEndpoint("first endpoint (boolean output)", RANGE_U1,false));
			shared_ptr<ScalarEndpoint> ep2(new FakeEndpoint("second endpoint (boolean output)", RANGE_U1,false));
			//shared_ptr<Endpoint> ep3(new MultiplexerEndpoint("third endpoint (multiplexing 1 and 2)", RANGE_U1, {ep1,ep2}));
			shared_ptr<Endpoint> ep4(new FakeEndpoint("fourth endpoint (range output)", RANGE_U8,false));
			endpoints={ep1,ep2,ep4};*/
			std::cerr<<"loading config finished\n";
			endpoints=config.endpoints;
		}
		catch(std::exception &e)
		{
			std::cerr<<"Caught exception while loading config: "<<e.what()<<"\n";
		}
	}
	vector<shared_ptr<Endpoint> > currentlist;
	BOOST_FOREACH(auto p,endpoints)
	{
		currentlist.push_back(p.second);
	}
	EndpointListWidget *list=new EndpointListWidget(currentlist,tmp);
	enableUpdates(true);
	if(!updatesEnabled())
		std::cerr<<"***** ENABLING UPDATES FAILED *****!\n";
}

map<string,shared_ptr<Endpoint> > WebApp::endpoints;


}
