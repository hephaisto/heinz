#include <map>
using std::map;
#include <Wt/WApplication>
#include "common.hpp"
#include "FakeEndpoint.hpp"

namespace heinz
{

class WebApp : public Wt::WApplication
{
public:
	WebApp(const Wt::WEnvironment &env);
private:
	static map<string,shared_ptr<Endpoint> > endpoints;
};

}
