#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
using std::string;
#include <map>
using std::map;

namespace heinz
{

/**
Represents the data type of endpoints.
**/
enum EnRangeType
{
	RANGE_U1,	//< boolean
	RANGE_U8,	//< unsigned 8-bit integer
	RANGE_S8,	//< signed 8-bit integer
	RANGE_U16,	//< unsigned 16-bit integer
	RANGE_S16,	//< signed 16-bit integer
	RANGE_U32,	//< unsigned 32-bit integer
	RANGE_S32,	//< signed 32-bit integer
	RANGE_S64	//< signed 64-bit integer
};

extern map<string,EnRangeType> rangeByString;

}

#endif
