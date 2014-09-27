#include "Endpoint.hpp"

#include "python/python_wrapper.hpp"

namespace heinz
{

Endpoint::Endpoint(string description)
:description(description)
{}

string Endpoint::getDescription()
{
	return description;
}

void PollingObject::executeScript()
{
	runUpdateCommand(command);
}
void PollingObject::setScriptIfAvailable(ptree &pt)
{
	try
	{
		command=pt.get<string>("update_command");
	}
	catch(...)
	{}
}

}
