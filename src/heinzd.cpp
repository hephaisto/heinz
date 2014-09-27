#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <Wt/WServer>
#include "WebApp.hpp"
#include "heinz.hpp"

int main(int argc, char** argv)
{
	using namespace heinz;
	shared_ptr<Heinz> heinz(nullptr);
	boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);

	try
	{
		heinz=std::make_shared<Heinz>();
	}
	catch(boost::exception &e)
	{
		BOOST_LOG_TRIVIAL(fatal)<<"error: "<<boost::diagnostic_information(e,false);
		return 1;
	}
	catch(std::exception &e)
	{
		BOOST_LOG_TRIVIAL(fatal)<<"error: "<<e.what();
		return 1;
	}
	boost::log::core::get()->set_filter(boost::log::trivial::severity >= heinz->getConfig()->logLevel);

	// run HTTP
	try
	{
		Wt::WServer server("heinz");
		string server_config=heinz->getConfig()->wtConfigFile;
		BOOST_LOG_TRIVIAL(info)<<"loading config file "<<server_config;
		server.setServerConfiguration(argc,argv,server_config);
		server.addEntryPoint(Wt::Application, heinz->getAppCreator());
		if(server.start())
		{
			int sig=Wt::WServer::waitForShutdown("heinz");
			server.stop();
			//if(sig==Wt::WServer::SIGHUP)
			//	;	// TODO: reload
		}
	}
	catch(boost::exception &e)
	{
		BOOST_LOG_TRIVIAL(fatal)<<"error: "<<boost::diagnostic_information(e);
		return 1;
	}
	catch(std::exception &e)
	{
		BOOST_LOG_TRIVIAL(fatal)<<"error: "<<e.what();
		return 1;
	}
	return 0;
}
