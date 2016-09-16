#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
using std::string;

#include <memory>
using std::shared_ptr;

#include <map>
using std::map;

#include <set>

#include "exceptions.hpp"

namespace heinz
{

class PluginException : public HeinzException
{
public:
	PluginException(const string &msg);
};


template<class PluginType>
class PluginRegistrar
{
public:
	PluginRegistrar(const string &name, PluginType *instance);
};

template<class PluginType>
class PluginManager
{
public:
	static PluginManager& instance();
	void registerPlugin(const string &name, PluginType *plugin);
	PluginType* get(const string &name) const;
	std::set<string> getNames();
private:
	map<string, PluginType*> plugins;
};


// IMPLEMENTATIONS

template<class PluginType>
PluginManager<PluginType>& PluginManager<PluginType>::instance()
{
	static PluginManager<PluginType> instance;
	return instance;
}

template<class PluginType>
void PluginManager<PluginType>::registerPlugin(const string &name, PluginType *plugin)
{
	auto find = plugins.find(name);
	if(find != plugins.end())
		throw PluginException("Plugin already registered: "+name);
	plugins.insert(std::make_pair(name, plugin));
}

template<class PluginType>
PluginType* PluginManager<PluginType>::get(const string &name) const
{
	auto find = plugins.find(name);
	if(find == plugins.end())
		throw PluginException("Trying to access unavailable plugin: "+name);
	return find->second;
}

template<class PluginType>
std::set<string> PluginManager<PluginType>::getNames()
{
	std::set<string> result;
	for(auto it = plugins.begin(); it != plugins.end(); it++)
		result.insert(it->first);
	return result;
}

template<class PluginType>
PluginRegistrar<PluginType>::PluginRegistrar(const string &name, PluginType *instance)
{
	PluginManager<PluginType>::instance().registerPlugin(name, instance);
}

} // namespace heinz

#endif
