#ifndef WEBAPP_H
#define WEBAPP_H

#include <map>
using std::map;
#include <Wt/WApplication>
#include "common.hpp"
#include "FakeEndpoint.hpp"
#include "config/config_loader.hpp"

namespace heinz
{

class WebApp : public Wt::WApplication
{
public:
	WebApp(const Wt::WEnvironment &env, shared_ptr<Config> config);
private:
};

}

#endif // WEBAPP_H
