#ifndef HEINZ_EXCEPTIONS_H
#define HEINZ_EXCEPTIONS_H

#include <exception>
#include <boost/exception/all.hpp>
#include <string>
using std::string;

#include "constants.hpp"

namespace heinz
{

/**
@defgroup exceptions Exceptions thrown by Heinz
@addtogroup exceptions
@{
**/

typedef boost::error_info<struct tagErrorMessage,string> ExErrorMessage;

/**
Base class for all exceptions that are thrown in Heinz.
**/
class HeinzException : public std::exception
{
public:
	HeinzException(string text);
	virtual const char* what() const noexcept;
private:
	string text;
};

/**
This exception will be thrown, when an invalid state is detected (e.g. invalid enums).

This exception should not be thrown at all, if it happens, some function did not perform proper checks on it's parameters.
**/
class InvalidStateDetectedException : public HeinzException
{
public:
	InvalidStateDetectedException(string text);
};

/**
Thrown for values that don't match the supposed range.
**/
class InvalidEndpointRangeException : public HeinzException
{
public:
	InvalidEndpointRangeException(EnRangeType type, int64_t usedValue);
};

/**
Thrown when trying to access the current value of an endpoint with invalid value.
**/
class InvalidValueException : public HeinzException
{
public:
	InvalidValueException(string endpointName);
};

/**
Thrown when trying to use a parameter combination that is not available.
**/
class InvalidParameterException : public HeinzException
{
public:
	InvalidParameterException(string text);
};

/**
Thrown when setting the value of an input endpoint.
**/
class InputEndpointValueSetException : public HeinzException
{
public:
	InputEndpointValueSetException();
};


typedef boost::error_info<struct tagConfigFilename,string> ExConfigFilename;
typedef boost::error_info<struct tagEndpointName,string> ExEndpointName;
/**
Errors in config file
**/
struct ConfigException : virtual boost::exception, virtual std::exception {};
/*class ConfigException : virtual boost::exception, public HeinzException
{
public:
	ConfigException(string text);
	virtual const char* what() const noexcept;
};*/

}

/** @} **/ // end group exceptions

#endif
