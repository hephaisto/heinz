#include "exceptions.hpp"
#include <boost/format.hpp>
#include <iostream>

namespace heinz
{

HeinzException::HeinzException(string text)
:text(text)
{
}

const char* HeinzException::what() const noexcept
{
	return text.c_str();
}

InvalidEndpointRangeException::InvalidEndpointRangeException(EnRangeType type, int64_t usedValue)
:HeinzException((boost::format("Used value %1% for datatype %2%") % usedValue % type).str())
{}

InvalidStateDetectedException::InvalidStateDetectedException(string text)
:HeinzException(text)
{}

InvalidValueException::InvalidValueException(string endpointName)
:HeinzException((boost::format("Trying to access invalid value of endpoint %1%!") % endpointName).str())
{}

InvalidParameterException::InvalidParameterException(string text)
:HeinzException(text)
{}

InputEndpointValueSetException::InputEndpointValueSetException()
:HeinzException("Trying to set value of an input endpoint!")
{}

/*
ConfigException::ConfigException(string text)
:HeinzException(text)
{}

const char* ConfigException::what() const noexcept
{
	std::stringstream ss;
	ss<<"Error loading config file ";
	if(string const * filename=boost::get_error_info<ExConfigFilename>(*this))
		ss<<filename;
	if(string const * msg=boost::get_error_info<ExErrorMessage>(*this))
		ss<<": "<<msg;
	return ss.str().c_str();
}
*/

}
