// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/PYTHON/pyInterpreter.h>
#include <BALL/PYTHON/BALLPythonConfig.h>

#include <BALL/FORMAT/lineBasedFile.h>

#include <Python.h>

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

			PyErr_Print();

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

		// on some platforms, we have to point the system to our own python packages
#ifdef BALL_OS_WINDOWS
		// TODO: decide through a registry key which python version to use
		Py_SetProgramName(BALL_PATH "/bin/python.exe");
#endif

		// initialize the interpreter
		Py_Initialize();

		// get a borrowed reference to the main - module
		PyObject* main_module = PyImport_AddModule("__main__");

		if (main_module == 0) 
		{
			Log.error() << "Python intialization failed: could not add \"__main__\" module! No Python support available." << std::endl;
			return;
		}

		// and create a valid context for our further Python - calls
		context_ = PyModule_GetDict(main_module);
	
		if (context_ == 0) 
		{
			Log.error() << "Could not get valid context for Python module \"__main__\"! No Python support available." << std::endl;
			return;
		}

		// import sys
		runSingleString_("import sys", Py_single_input);
		start_log_ += error_message_;

		// Add the BALL library path to the Python search path
		// to make sure Python can find the BALL extensions.
		Path p;

#ifndef BALL_COMPILER_MSVC
		runSingleString_("sys.path.append(\"" BALL_PATH "/lib/" "\")", Py_single_input);

#ifdef BALL_OS_DARWIN
	// on MacOS, try to find the python packages in the frameworks directory of our bundle
	String site_path = String(p.find("../Frameworks"));
	if (site_path != "")
		sys_path_.push_back(site_path);
#endif

#else
		// first, make sure that the site packages of our python installation will be found
		String site_path = String(p.find("Python\\lib"));
		site_path.trim();
		while (site_path.substitute("\\", "/") != String::EndPos) {};
		if (site_path != "")
			sys_path_.push_back(site_path);

		// on windows, we put our python packages one step above the data directory
		String python_path = String(p.find("..\\BALL.py")).before("BALL.py");
		python_path.trim();
		while (python_path.substitute("\\", "/") != String::EndPos) {};

		if (python_path != "")
			sys_path_.push_back(python_path);
#endif
		// try to locate startup.py as well
		String python_startup_path = String(p.find("startup.py")).before("startup.py");
		python_startup_path.trim();
		while (python_startup_path.substitute("\\", "/") != String::EndPos) {};

		if (python_startup_path != "")
			sys_path_.push_back(python_startup_path);

		start_log_ += error_message_;

		// Add additional paths (user-defined) to the end of the search path.
		// 
		std::vector<String>::const_iterator it(sys_path_.begin());
		for (; it != sys_path_.end(); ++it)
		{
			runSingleString_("sys.path.append(\"" + *it + "\")", Py_single_input);
			start_log_ += error_message_;
		}

		// import the site module, and the one required for the output redirection
		runSingleString_("import site, cStringIO", Py_single_input);
		start_log_ += error_message_;

		PyObject *sip_module = PyImport_ImportModule("sip");
		if (sip_module == 0) 
		{
			Log.error() << "Could not import Python module \"sip\"! No Python support available." << std::endl;
			return;
		}

		Py_DECREF(sip_module);

		// make sure that we found the correct sip version
		valid_ = true;

		run("import sip", valid_);
		const unsigned int module_sip_version = run("print(sip.SIP_VERSION)", valid_).trim().toUnsignedInt();
		const String module_sip_version_str = run("print(sip.SIP_VERSION_STR)", valid_).trim();

		const unsigned int module_major_minor = module_sip_version & 0xFFFFFF00;
		const unsigned int module_bugfix      = module_sip_version & 0x000000FF;
		const unsigned int ball_major_minor   = BALL_SIP_VERSION   & 0xFFFFFF00;
		const unsigned int ball_bugfix        = BALL_SIP_VERSION   & 0x000000FF;

		if ((module_major_minor != ball_major_minor) || (module_bugfix < ball_bugfix))
		{
			String sip_module_path = run("print(sip)", valid_).trim();

			error_message_ += "ERROR: Version of imported sip module is not compatible with the version BALL was built against.\n"
			                  "If BALL was compiled using SIP version x.y.z then it can be used with any SIP version x.y.z' where "
												"z' >= z.\n";
			error_message_ += "Got (from " + sip_module_path + ") " + module_sip_version_str + "; Expected " + BALL_SIP_VERSION_STR +"\n";

			Log.error() << error_message_ << std::endl;

			start_log_ += error_message_;

			valid_ = false;

			return;
		}

		// import the BALL module
		start_log_ += run("from BALL import *", valid_);

		if (!valid_) 
		{
			Log.error() << "Could not import the BALL library! No Python support available." << std::endl;
		}
	}

	PyObject* loadModule_(const QString& name)
	{
		PyObject* module_dict = PyImport_GetModuleDict();

		if(!module_dict)
		{
			Log.error() << "Could not obtain module dictionary" << std::endl;
			return 0;
		}

		PyObject* mod_name = PyString_FromString(name.toLatin1().data());

		PyObject* module = 0;

		if(PyDict_Contains(module_dict, mod_name))
		{
			module = PyDict_GetItem(module_dict, mod_name);
		}
		else
		{
			//This could leak the imported module. Need to check...
			module = PyImport_ImportModule(name.toLatin1().data());
		}

		Py_XDECREF(mod_name);


		if(!module || PyErr_Occurred())
		{
			PyErr_Print();
			return 0;
		}

		return module;
	}

	bool PyInterpreter::execute(const QString& module, const QString& func_name, const QList<QPair<QString, QString> >& params)
	{
		PyObject* mod = loadModule_(module);

		if(mod == 0)
		{
			Log.error() << "Could not load module " << module.toStdString() << std::endl;
			return false;
		}

		PyObject* func = PyObject_GetAttrString(mod, func_name.toLatin1().data());

		if(!func || !PyCallable_Check(func))
		{
			PyErr_Print();

			return false;
		}

		PyObject* dict = PyDict_New();

		if(!dict)
		{
			Log.error() << "Error: Could not create named arguments dictionary" << std::endl;
			return false;
		}

		for(QList<QPair<QString, QString> >::const_iterator it = params.begin(); it != params.end(); ++it)
		{
			if(it->first == "action" || it->first == "module" || it->first == "method")
			{
				continue;
			}

			PyObject* val = PyString_FromString(it->second.toLatin1().data());

			if(!val)
			{
				Log.error() << "Could not create parameter" << it->first.toStdString() << "=" << it->second.toStdString() << " Skipping." << std::endl;
				continue;
			}

			PyDict_SetItemString(dict, it->first.toLatin1().data(), val);
		}

		Py_DECREF(dict);


		PyObject* dummy = PyTuple_New(0);

		if(!dummy)
		{
			Log.error() << "Could not allocate dummy tuple" << std::endl;
			return false;
		}

		PyObject* res = PyObject_Call(func, dummy, dict);

		Py_XDECREF(res);
		Py_DECREF(dummy);

		return true;
	}

	String PyInterpreter::run(const String& s, bool& state)
	{
		if (!valid_) return "";

		state = false;
		if (runSingleString_("CIO = cStringIO.StringIO()", Py_single_input) == 0 ||
		    runSingleString_("stdout=sys.stdout", Py_single_input) == 0 ||
		    runSingleString_("stderr=sys.stderr", Py_single_input) == 0 ||
		    runSingleString_("sys.stdout=CIO", Py_single_input) == 0 ||
		    runSingleString_("sys.stderr=CIO", Py_single_input) == 0) 
		{
			return error_message_;
		}
		
		state = (runSingleString_(s, Py_single_input) != 0);

		if (!state)
		{
			// save the error message, because resetting stdout would overwrite it
			String result = error_message_;

			runSingleString_("sys.stdout=stdout", Py_single_input);
			runSingleString_("sys.stderr=stderr", Py_single_input);

			return result;
		}

		// retrieve output
		char* buf = 0;
		PyObject* result = runSingleString_("str(CIO.getvalue())", Py_eval_input);
		if (result != 0)
		{
			PyArg_Parse(result, "s", &buf);
		}

		runSingleString_("sys.stdout=stdout", Py_single_input);
		runSingleString_("sys.stderr=stderr", Py_single_input);

		return String(buf);
	}

	String PyInterpreter::runFile(const String& filename)
	{
		if (!valid_) return "";

		FILE* file = fopen(filename.c_str(), "r");

		if(!file)
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}

		PyErr_Clear();
		PyRun_SimpleFileEx(file, filename.c_str(), true);

		if (PyErr_Occurred())
		{
			std::cout << "Error occured while executing " << filename << "\nPrinting Traceback:" << std::endl;
			PyErr_Print();
		}

		return "";
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
