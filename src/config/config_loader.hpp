#ifndef H_CONFIG_LOADER
#define H_CONFIG_LOADER

#include <string>
using std::string;
#include "../common.hpp"
#include <map>
using std::map;

#include "../Endpoint.hpp"

namespace heinz
{

struct Config
{
	map<string,shared_ptr<Endpoint> > endpoints;
};

Config load_config(const string &filename);

}

#endif // H_CONFIG_LOADER
