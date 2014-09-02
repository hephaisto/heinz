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
void PollingObject::setScript(string command)
{
	this->command=command;
}

}
