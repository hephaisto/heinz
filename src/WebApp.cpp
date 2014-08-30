#include <Wt/WBootstrapTheme>
#include <boost/foreach.hpp>

#include "WebApp.hpp"
#include "EndpointListWidget.hpp"
#include "MultiplexerEndpoint.hpp"
#include "exceptions.hpp"

namespace heinz
{

WebApp::WebApp(const Wt::WEnvironment &env, shared_ptr<Config> config)
:Wt::WApplication(env),
config(config)
{
	if(!config)
		throw HeinzException("invalid config");

	Wt::WBootstrapTheme *theme=new Wt::WBootstrapTheme();
	//theme->setVersion(3);
	setTheme(theme);
	Wt::WContainerWidget *tmp=new Wt::WContainerWidget(root());

	// replace with groups
	vector<shared_ptr<Endpoint> > currentlist;
	BOOST_FOREACH(auto p,config->endpoints)
	{
		currentlist.push_back(p.second);
	}


	EndpointListWidget *list=new EndpointListWidget(currentlist,tmp);
	enableUpdates(true);
}




}
