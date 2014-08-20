#include "exceptions.hpp"
#include <boost/format.hpp>
#include <iostream>

namespace heinz
{

HeinzException::HeinzException(string text)
:text(text)
{
	std::cerr<<text<<"\n";
}

const char* HeinzException::what()
{
	return text.c_str();
}

InvalidEndpointRangeException::InvalidEndpointRangeException(EnRangeType type, int64_t usedValue)
:HeinzException((boost::format("Used value %1% for datatype %2%") % usedValue % type).str())
{}

InvalidStateDetectedException::InvalidStateDetectedException(string text)
:HeinzException(text)
{}

InvalidValueException::InvalidValueException()
:HeinzException("Trying to access invalid value!")
{}

InvalidParameterException::InvalidParameterException(string text)
:HeinzException(text)
{}

InputEndpointValueSetException::InputEndpointValueSetException()
:HeinzException("Trying to set value of an input endpoint!")
{}


ConfigException::ConfigException(string text)
:HeinzException(text)
{}

}
