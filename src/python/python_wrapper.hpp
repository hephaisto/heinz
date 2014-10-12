#include <boost/python.hpp>
#include "../heinz.hpp"

using namespace boost::python;


#ifndef PYTHON_WRAPPER_H
#define PYTHON_WRAPPER_H


namespace heinz
{

extern object main_namespace;
void initPython();
void runUpdateCommand(string command);
void executeScript(string script);

}

#endif // PYTHON_WRAPPER_H
