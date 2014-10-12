#include <boost/log/trivial.hpp>
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
		.def("setValue",&ScalarEndpoint::setValue)
		;

	class_<Heinz, shared_ptr<Heinz>, boost::noncopyable>("Heinz",no_init)
		.def("getEndpoint",&Heinz::getEndpoint)
		.def("getScalarEndpoint",&Heinz::getScalarEndpoint)
		;

	def("getInstance",&Heinz::getInstance);
}

void initPython()
{
	BOOST_LOG_TRIVIAL(info)<<"initializing python...";
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
	BOOST_LOG_TRIVIAL(debug)<<"running command: "<<command;
	try
	{
		object result = exec(str(command), main_namespace, main_namespace);
	}
	catch( error_already_set )
	{
		PyObject* ptype;
		PyObject* pvalue;
		PyObject* ptraceback;

		PyErr_Fetch(&ptype, &pvalue, &ptraceback);

		handle<> hType(ptype);
		object extype(hType);
		handle<> hTraceback(ptraceback);
		object traceback(hTraceback);

		string strErrorMessage = extract<string>(pvalue);

		long lineno = extract<long> (traceback.attr("tb_lineno"));
		string filename = extract<string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
		string funcname = extract<string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(ptraceback);
		BOOST_LOG_TRIVIAL(error)<<"error: "<<strErrorMessage;
		BOOST_LOG_TRIVIAL(error)<<"in "<<funcname<<"("<<filename<<": "<<lineno<<")";
	}
}
void executeScript(string script)
{
	BOOST_LOG_TRIVIAL(info)<<"loading script: "<<script;
	try
	{
		object result = exec_file(str(script), main_namespace, main_namespace);
	}
	catch( error_already_set )
	{
		PyErr_Print();
		PyObject* ptype;
		PyObject* pvalue;
		PyObject* ptraceback;
		std::cerr<<"1";

		PyErr_Fetch(&ptype, &pvalue, &ptraceback);
		std::cerr<<"2";

		handle<> hType(ptype);
		object extype(hType);
		handle<> hTraceback(ptraceback);
		object traceback(hTraceback);

		std::cerr<<"3";
		string strErrorMessage = extract<string>(pvalue);

		std::cerr<<"4";
		long lineno = extract<long> (traceback.attr("tb_lineno"));
		std::cerr<<"5";
		string filename = extract<string>(traceback.attr("tb_frame").attr("f_code").attr("co_filename"));
		std::cerr<<"6";
		string funcname = extract<string>(traceback.attr("tb_frame").attr("f_code").attr("co_name"));
		std::cerr<<"7";
		Py_XDECREF(ptype);
		Py_XDECREF(pvalue);
		Py_XDECREF(ptraceback);
		BOOST_LOG_TRIVIAL(error)<<"error: "<<strErrorMessage;
		BOOST_LOG_TRIVIAL(error)<<"in "<<funcname<<"("<<filename<<": "<<lineno<<")";
	}
}
object main_namespace;

}
