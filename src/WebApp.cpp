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
#include "version.hpp"

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

	internalPathChanged().connect(this, &WebApp::onInternalPathChanged);

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
		Wt::WMenuItem* macroItem=mainTabs->addTab(macros,"Macros", Wt::WTabWidget::PreLoading);
		macroItem->setPathComponent("macros");
	}
	internalPathCallbacks.insert(std::make_pair(
		"/macros",
		[mainTabs]
		{
			mainTabs->setCurrentIndex(0);
		}
		));

	BOOST_FOREACH(auto supergroup, config->groups)
	{
		Wt::WTabWidget *subTabs = new Wt::WTabWidget(NULL);
		Wt::WMenuItem* subMenu=mainTabs->addTab(subTabs,supergroup.first, Wt::WTabWidget::PreLoading);
		internalPathCallbacks.insert(std::make_pair(
			"/group/"+supergroup.first,
			[mainTabs,subTabs]
			{
				mainTabs->setCurrentIndex(mainTabs->indexOf(subTabs));
			}
			));

		BOOST_FOREACH(auto group, supergroup.second)
		{
			EndpointListWidget *listWidget=new EndpointListWidget(group.second,NULL);
			Wt::WMenuItem* subSubMenu=subTabs->addTab(listWidget,group.first, Wt::WTabWidget::PreLoading);
			internalPathCallbacks.insert(std::make_pair(
				"/group/"+supergroup.first+"/"+group.first,
				[mainTabs,subTabs,listWidget]
				{
					mainTabs->setCurrentIndex(mainTabs->indexOf(subTabs));
					subTabs->setCurrentIndex(subTabs->indexOf(listWidget));
				}
				));
		}
	}
	mainTabs->addTab(new Wt::WTextArea(VERSION_STRING),"Settings", Wt::WTabWidget::PreLoading);
	mainTabs->setStyleClass("tabwidget");


	enableUpdates(true);
	onInternalPathChanged();
}

void WebApp::onInternalPathChanged()
{
	auto it=internalPathCallbacks.find(internalPath());
	if(it!=internalPathCallbacks.end())
	{
		it->second();
	}
	else
	{
		BOOST_FOREACH(auto it, internalPathCallbacks)
			std::cout<<it.first<<"\n";
	}
}


}
