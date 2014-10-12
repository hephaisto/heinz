#ifndef H_CONFIG_LOADER
#define H_CONFIG_LOADER

#include <string>
using std::string;
#include "../common.hpp"
#include <map>
using std::map;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
using boost::property_tree::ptree;

#include "../Endpoint.hpp"

namespace heinz
{

struct Config
{
	map<string,shared_ptr<Endpoint> > endpoints;
	vector<shared_ptr<PollingObject> > pollingObjects;
	map<string, map<string,vector<shared_ptr<Endpoint> > > > groups;
	vector<string> scriptFiles;
	map<string, string> macros;
	uint64_t pollingInterval;
	string wtConfigFile;
	int logLevel;
};

shared_ptr<Config> load_config();

}

#endif // H_CONFIG_LOADER
