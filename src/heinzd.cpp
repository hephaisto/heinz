#include "WebApp.hpp"

namespace heinz
{

WebApp* createApp(const Wt::WEnvironment &env,shared_ptr<Config> config)
{
	return new heinz::WebApp(env,config);
}

}


int main(int argc, char** argv)
{
	using namespace heinz;
	shared_ptr<Config> config(nullptr);
	try
	{
		config=load_config("config/heinz.conf");
	}
	catch(std::exception &e)
	{
		std::cerr<<"Caught exception while loading config: "<<e.what()<<"\n";
		return 1;
	}
	return Wt::WRun(argc,argv,boost::bind(createApp,_1,config));
}
