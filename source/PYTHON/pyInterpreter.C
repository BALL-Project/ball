// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pyInterpreter.C,v 1.13.18.1 2007/03/25 21:42:50 oliver Exp $
//

#include <Python.h>
#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/FORMAT/lineBasedFile.h>

namespace BALL
{

	String error_message_;
	PyObject* context_;

	String PyInterpreter::start_log_;
	bool PyInterpreter::valid_ = false;

	PyInterpreter::PyInterpreter()
	{
	}

	PyObject* runSingleString_(const String& str, int mode)
	{
		// clear previous errors
		PyErr_Clear();
		error_message_ = "";
		
		// run the string through the interpreter
		PyObject* result = PyRun_String(const_cast<char*>(str.c_str()), mode, context_, context_);
		if (PyErr_Occurred())
		{
			PyErr_Print();
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
				error_message_ += " (error message could not be parsed)";
			}
			
			error_message_ += "\n";

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
			// TODO: for some reason, finalize crashes on Windows, at least if no python
			//       module has been found. We need to fix this correctly.
#ifndef BALL_COMPILER_MSVC
			Py_Finalize();
#endif
		}

		valid_ = false;
	}

	void PyInterpreter::initialize()
	{
		valid_ = false;
		start_log_.clear();

		// finalize the interpreter if it is already running
		if (Py_IsInitialized())
		{
			Py_Finalize();
		}

		// initialize the interpreter
		Py_Initialize();

		PyObject *module = PyImport_ImportModule("site");
		if (module == 0) 
		{
			Log.error() << "Could not import Python module \"site\"! No Python support available." << std::endl;
			return;
		}

		context_ = PyModule_GetDict(module);

		if (context_ == 0) 
		{
			Log.error() << "Could not get dict for Python module \"site\"! No Python support available." << std::endl;
			return;
		}

		Py_DECREF(module);

		// import the modules required for the output redirection
		// and the system stuff
		runSingleString_("import cStringIO, sys", Py_single_input);
		start_log_ += error_message_;

		// Add the BALL library path to the Python search path
		// to make sure Python can find the BALL extensions.
#ifndef BALL_COMPILER_MSVC
		runSingleString_("sys.path.append(\"" BALL_PATH "/lib/" "\")", Py_single_input);
#else
		// on windows, we put our python packages one step above the data directory
		Path p;
		String python_path = String(p.find("..\\BALL.py")).before("BALL.py");
		python_path.trim();
		while (python_path.substitute("\\", "/") != String::EndPos) {};

		if (python_path != "")
			sys_path_.push_back(python_path);

		python_path = String(p.find("startup.py")).before("startup.py");
		python_path.trim();
		while (python_path.substitute("\\", "/") != String::EndPos) {};

		if (python_path != "")
			sys_path_.push_back(python_path);

#endif
		start_log_ += error_message_;

		// Add additional paths (user-defined) to the end of the search path.
		// 
		std::vector<String>::const_iterator it(sys_path_.begin());
		for (; it != sys_path_.end(); ++it)
		{
			runSingleString_("sys.path.append(\"" + *it + "\")", Py_single_input);
			start_log_ += error_message_;
		}

		PyObject *sip_module = PyImport_ImportModule("sip");
		if (sip_module== 0) 
		{
			Log.error() << "Could not import Python module \"sip\"! No Python support available." << std::endl;
			return;
		}
	
		valid_ = true;
		// import the BALL module
		start_log_ += run("from BALL import *", valid_);

		if (!valid_) 
		{
			Log.error() << "Could not import the BALL library! No Python support available." << std::endl;
		}
	}


	String PyInterpreter::run(const String& s, bool& state)
	{
		if (!valid_) return "";

		state = false;
		if (runSingleString_("CIO = cStringIO.StringIO()", Py_single_input) == 0 ||
		    runSingleString_("sys.stdout=CIO", Py_single_input) == 0 ||
		    runSingleString_("sys.stderr=CIO", Py_single_input) == 0) 
		{
			return error_message_;
		}
		
		state = (runSingleString_(s, Py_single_input) != 0);

		// retrieve output
		char* buf = 0;
		PyObject* result = runSingleString_("str(CIO.getvalue())", Py_eval_input);
		if (result != 0)
		{
			PyArg_Parse(result, "s", &buf);
		}
		
		return buf;
	}

	String PyInterpreter::runFile(const String& filename)
		throw(Exception::FileNotFound)
	{
		if (!valid_) return "";

		if (runSingleString_("CIO = cStringIO.StringIO()", Py_single_input) == 0 ||
		    runSingleString_("sys.stdout = CIO", Py_single_input) == 0 || 
		    runSingleString_("sys.stderr = CIO", Py_single_input) == 0) 
		{
			return error_message_;
		}
		
		String result_string;
		LineBasedFile file(filename);
		while (file.readLine())
		{
			if (runSingleString_(file.getLine(), Py_single_input) == 0) 
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

	void PyInterpreter::setSysPath(const PathStrings& path_strings)
	{
		sys_path_ = path_strings;
	}
	
	const PyInterpreter::PathStrings& PyInterpreter::getSysPath()
	{
		return sys_path_;
	}

	PyInterpreter::PathStrings PyInterpreter::sys_path_;

} // namespace BALL
