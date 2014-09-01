#include "WebApp.hpp"
#include "heinz.hpp"

int main(int argc, char** argv)
{
	using namespace heinz;
	shared_ptr<Heinz> heinz(nullptr);
	try
	{
		heinz=std::make_shared<Heinz>("config/heinz.conf");
	}
	catch(std::exception &e)
	{
		std::cerr<<"Caught exception while starting heinz: "<<e.what()<<"\n";
		return 1;
	}
	return Wt::WRun(argc,argv,heinz->getAppCreator());
}
