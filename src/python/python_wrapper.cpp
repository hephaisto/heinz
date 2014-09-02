#include "python_wrapper.hpp"
#include "../exceptions.hpp"

namespace heinz
{

BOOST_PYTHON_MODULE(Heinz)
{
	class_<ScalarEndpoint, shared_ptr<ScalarEndpoint>, boost::noncopyable>("ScalarEndpoint",no_init)
		.def("getDescription",&ScalarEndpoint::getDescription)
		.def("switchOnOff",&ScalarEndpoint::switchOnOff)
		.def("getValue",&ScalarEndpoint::getValue)
		;

	class_<Heinz, shared_ptr<Heinz>, boost::noncopyable>("Heinz",no_init)
		.def("getEndpoint",&Heinz::getEndpoint)
		.def("getScalarEndpoint",&Heinz::getScalarEndpoint)
		;

	def("getInstance",&Heinz::getInstance);
}

void initPython()
{
	std::cerr<<"initializing python...\n";
	try
	{
		PyImport_AppendInittab( "Heinz", initHeinz );
		Py_Initialize();
		object main_module = import("__main__");
		main_namespace = main_module.attr("__dict__");
		object cpp_module( (handle<>(PyImport_ImportModule("Heinz"))) );
		main_namespace["Heinz"] = cpp_module;
	}
	catch( error_already_set )
	{
		PyErr_Print();
		throw HeinzException("unable to initialize python interpreter");
	}
}

void runUpdateCommand(string command)
{
	std::cerr<<"running command: "<<command<<"\n";
	try
	{
		object result = exec(str(command), main_namespace, main_namespace);
	}
	catch( error_already_set )
	{
		PyErr_Print();
		throw HeinzException("error executing python code");
	}
}
object main_namespace;

}
