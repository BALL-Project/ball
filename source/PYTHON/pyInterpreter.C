// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyInterpreter.C,v 1.7 2003/04/01 00:50:27 amoll Exp $

#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <Python.h>

namespace BALL
{

	String error_message_;
	PyObject* context_;

	PyObject* runSingleString_(const String& str, int mode)
	{
		// clear previous errors
		PyErr_Clear();
		error_message_ = "";
		
		// run the string through the interpreter
		PyObject* result = PyRun_String(const_cast<char*>(str.c_str()), mode, context_, context_);
		if (PyErr_Occurred())
		{
			PyObject* type;
			PyObject* value;
			PyObject* range;

			char* message;
			error_message_ = "ERROR: ";
			PyErr_Fetch(&type, &value, &range);
			if (PyArg_Parse(value, "s", &message))
			{
				error_message_ += message;
			}
			else
			{	
				error_message_ = "Syntax error";
			}
			error_message_ += "\n";

			PyErr_Clear();
			return 0;
		}
		
		return result;
	}

	bool PyInterpreter::isInitialized()
	{
		return Py_IsInitialized();
	}

	void PyInterpreter::finalize()
	{
		if (isInitialized())
		{
			Py_Finalize();
		}
	}

	void PyInterpreter::initialize()
	{
		// finalize the interpreter if it is already running
		if (Py_IsInitialized())
		{
			Py_Finalize();
		}

		// initialize the interpreter
		Py_Initialize();

		PyObject *module = PyImport_ImportModule("site");
		context_ = PyModule_GetDict(module);
		Py_DECREF(module);

		// import the modules required for the output redirection
		// and the system stuff
		runSingleString_("import cStringIO, sys", Py_single_input);

		// add the BALL library path to the Python search path
		// to make sure Python can find the BALL extensions
		runSingleString_("sys.path.append(\"" BALL_PATH "/lib/" BALL_BINFMT "\")", Py_single_input);
		
		// import the BALL module
		runSingleString_("from BALL import *", Py_single_input);
	}


	String PyInterpreter::run(const String& s, bool& state)
	{
		state = false;
		if (runSingleString_("OLDSTDOUT=sys.stdout", Py_single_input) == 0) return error_message_;
		if (runSingleString_("CIO=cStringIO.StringIO()", Py_single_input) == 0) return error_message_;
		if (runSingleString_("sys.stdout=CIO", Py_single_input) == 0) return error_message_;
		if (runSingleString_("sys.stderr=CIO", Py_single_input) == 0) return error_message_;
		
		PyErr_Clear();
		if (runSingleString_(const_cast<char*>(s.c_str()), Py_single_input) == 0) return error_message_;
		state = true;

		// retrieve output
		PyObject* result = runSingleString_("str(CIO.getvalue())", Py_eval_input);
		if (result != 0)
		{
			char* buf;
			PyArg_Parse(result, "s", &buf);
			return buf;
		}
		else
		{
			return "";
		}
	}

	String PyInterpreter::runFile(const String& filename)
		throw(Exception::FileNotFound)
	{
		if (runSingleString_("OLDSTDOUT=sys.stdout", Py_single_input) == 0) return error_message_;
		if (runSingleString_("CIO=cStringIO.StringIO()", Py_single_input) == 0) return error_message_;
		if (runSingleString_("sys.stdout=CIO", Py_single_input) == 0) return error_message_;
		if (runSingleString_("sys.stderr=CIO", Py_single_input) == 0) return error_message_;
		PyErr_Clear();
		
		String result_string;
		LineBasedFile file(filename);
		while (file.readLine())
		{
			if (runSingleString_(const_cast<char*>(file.getLine().c_str()), Py_single_input) == 0) 
			{
				result_string += "Error in Line " + String(file.getLineNumber()) + " in file " + filename;
				return result_string;
			}

			// retrieve output
			PyObject* result = runSingleString_("str(CIO.getvalue())", Py_eval_input);
			if (result != 0)
			{
				char* buf;
				PyArg_Parse(result, "s", &buf);
				result_string += String(buf);
			}
		}

		return result_string;
	}

} // namespace BALL
