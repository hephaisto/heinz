#include "WebApp.hpp"

heinz::WebApp* createApp(const Wt::WEnvironment &env)
{
	return new heinz::WebApp(env);
}


int main(int argc, char** argv)
{
	return Wt::WRun(argc,argv,&createApp);
}
