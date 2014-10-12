#include <Wt/WBootstrapTheme>
#include <Wt/WTabWidget>
#include <Wt/WTextArea>
#include <Wt/WMenuItem>
#include <Wt/WPushButton>
#include <boost/foreach.hpp>

#include "WebApp.hpp"
#include "EndpointListWidget.hpp"
#include "MultiplexerEndpoint.hpp"
#include "exceptions.hpp"
#include "python/python_wrapper.hpp"

namespace heinz
{

WebApp::WebApp(const Wt::WEnvironment &env, shared_ptr<Config> config)
:Wt::WApplication(env)
{
	if(!config)
		throw HeinzException("invalid config");

	Wt::WBootstrapTheme *theme=new Wt::WBootstrapTheme();
	//theme->setVersion(3);
	setTheme(theme);
	Wt::WContainerWidget *tmp=new Wt::WContainerWidget(root());

	Wt::WTabWidget *mainTabs = new Wt::WTabWidget(tmp);

	if(!config->macros.empty())
	{
		Wt::WContainerWidget *macros=new Wt::WContainerWidget();
		BOOST_FOREACH(auto it, config->macros)
		{
			Wt::WPushButton *btn=new Wt::WPushButton(it.first,macros);
			btn->clicked().connect(boost::bind(runUpdateCommand,it.second));
		}
		mainTabs->addTab(macros,"Macros", Wt::WTabWidget::PreLoading);
	}

	BOOST_FOREACH(auto supergroup, config->groups)
	{
		Wt::WTabWidget *subTabs = new Wt::WTabWidget(NULL);
		BOOST_FOREACH(auto group, supergroup.second)
		{
			EndpointListWidget *listWidget=new EndpointListWidget(group.second,NULL);
			subTabs->addTab(listWidget,group.first, Wt::WTabWidget::PreLoading);
		}
		mainTabs->addTab(subTabs,supergroup.first, Wt::WTabWidget::PreLoading);
	}
	mainTabs->addTab(new Wt::WTextArea("Settings"),"Settings", Wt::WTabWidget::PreLoading);
	mainTabs->setStyleClass("tabwidget");


	enableUpdates(true);
}


}
