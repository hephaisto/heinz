#include "Endpoint.hpp"

#include <Wt/WText>

namespace heinz
{

Endpoint::Endpoint(string description)
:description(description)
{}

string Endpoint::getDescription()
{
	return description;
}

}
