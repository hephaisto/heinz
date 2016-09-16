#ifndef BACKEND_PLUGIN_H
#define BACKEND_PLUGIN_H

#include "../plugin.hpp"
#include "../Endpoint.hpp"

namespace heinz
{

class BackendPlugin
{
public:
	virtual void backendConfig(ptree &pt) = 0;
	virtual shared_ptr<Endpoint> createEndpoint(shared_ptr<Config> config, ptree &pt) = 0;
};

}

#endif
