#include <Wt/WServer>
#include "WebApp.hpp"
#include "heinz.hpp"

int main(int argc, char** argv)
{
	using namespace heinz;
	shared_ptr<Heinz> heinz(nullptr);
	try
	{
		heinz=std::make_shared<Heinz>();
	}
	catch(std::exception &e)
	{
		std::cerr<<"Caught exception while starting heinz: "<<e.what()<<"\n";
		return 1;
	}

	// run HTTP
	try
	{
		Wt::WServer server("heinz");
		string server_config=heinz->getConfig()->wtConfigFile;
		std::cerr<<"loading config file "<<server_config<<"\n";
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
	catch(std::exception &e)
	{
		std::cerr<<"error: "<<e.what()<<"\n";
		return 1;
	}
	return 0;
}
