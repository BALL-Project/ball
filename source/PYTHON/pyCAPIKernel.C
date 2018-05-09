#include <BALL/PYTHON/pyCAPIKernel.h>

#include <BALL/COMMON/logStream.h>
#include <BALL/FORMAT/lineBasedFile.h>

using std::map;
using std::string;
using std::stringstream;

namespace BALL
{
	PyCAPIKernel::PyCAPIKernel() :
		PyKernel{}
	{
		Py_Initialize();

		// get a borrowed reference to the main module
		main_module_ = PyImport_AddModule("__main__");
		if (!main_module_)
		{
			Log.error() << "[PyInterpreter] Python intialization failed: could not add '__main__' module! "
			            << "No Python support available." << std::endl;
			return;
		}

		// and create a valid context for our further Python calls
		context_ = PyModule_GetDict(main_module_);
		if (!context_)
		{
			Log.error() << "[PyInterpreter] Could not get valid context for Python module '__main__'! "
			            << "No Python support available." << std::endl;
			return;
		}

		// import modules for output redirection
		PyRun_SimpleString("import sys, cStringIO");

		// Add path for BALL-specific Python scripts
		loadScriptDir();
	}

	PyCAPIKernel::~PyCAPIKernel()
	{
		// free additionally loaded modules
		for (auto module: modules_) Py_DecRef(module);

		/* TODO: for some reason, finalize crashes on Windows, at least if no python
		 * module has been found. We need to fix this correctly.
		 */
#ifndef BALL_COMPILER_MSVC
			Py_Finalize();
#endif
	}

	bool PyCAPIKernel::isStarted() const
	{
		return bool(Py_IsInitialized());
	}

	bool PyCAPIKernel::errorOccurred()
	{
		if (!PyErr_Occurred()) return false;

		if (PyErr_ExceptionMatches(PyExc_SyntaxError))
		{
			last_err_ = "Syntax error";
			return true;
		}

		PyObject* type =  nullptr;
		PyObject* value = nullptr;
		PyObject* trace = nullptr;

		PyErr_Fetch(&type, &value, &trace);
		stringstream err;

		// error type
		if(type)
		{
			auto name  = PyObject_GetAttrString(type, "__name__");
			auto sname = name ? PyString_AsString(name) : nullptr;
			if(sname)  err << sname << ": ";
			Py_DecRef(name);
			Py_DecRef(type);
		}

		// error message
		auto svalue = value ? PyString_AsString(value) : nullptr;
		err << (svalue ? svalue : "(no error message given)");
		Py_DecRef(value);

		// stack trace (TODO)
		Py_DecRef(trace);

		last_err_ = err.str();
		return true;
	}

	std::pair<bool, string> PyCAPIKernel::run(string str)
	{
#ifdef BALL_DEBUG
		Log.info() << "[PyInterpreter] >>> " << str << std::endl;
#endif

		// clear previous errors
		PyErr_Clear();

		// redirect output to StringIO
		PyRun_SimpleString(
			"__BALL_CIO = cStringIO.StringIO()\n"
			"sys.stdout = __BALL_CIO\n"
			"sys.stderr = __BALL_CIO\n"
		);

		// run the string through the interpreter
		// (with Py_file_input, return value is always Py_None)
		Py_DecRef(PyRun_String(str.c_str(), Py_file_input, context_, context_));

		if (errorOccurred()) return {false, ""};

		auto cio = PyObject_GetAttrString(main_module_, "__BALL_CIO");
		auto cio_val = PyObject_GetAttrString(cio, "getvalue");
		auto cio_f = PyObject_CallFunction(cio_val, nullptr);
		auto ret = std::make_pair<bool, string>(true, PyString_AsString(cio_f));
		Py_DecRef(cio_f);
		Py_DecRef(cio_val);
		Py_DecRef(cio);
		return ret;
	}

	bool PyCAPIKernel::execute(const string& module, const string& func_name, const KeyValArgs& args)
	{
		PyErr_Clear();

		PyObject* mod = loadModule(module);

		if (!mod)
		{
			Log.error() << "[PyInterpreter] Could not load module " << module << std::endl;
			return false;
		}

		PyObject* func = PyObject_GetAttrString(mod, func_name.c_str());

		if (!func || !PyCallable_Check(func))
		{
			PyErr_Print();
			Py_DecRef(func);
			return false;
		}

		PyObject* dict = PyDict_New();

		if (!dict)
		{
			Log.error() << "[PyInterpreter] Could not create named arguments dictionary" << std::endl;
			Py_DecRef(func);
			return false;
		}

		for (const auto& pair: args)
		{
			auto val = PyString_FromString(pair.second.c_str());
			if (!val)
			{
				Log.error() << "[PyInterpreter] Could not create parameter "
				            << pair.first << " = " << pair.second << "! Skipping..." << std::endl;
				continue;
			}
			PyDict_SetItemString(dict, pair.first.c_str(), val);
			Py_DecRef(val);
		}

		auto dummy = PyTuple_New(0);

		if (!dummy)
		{
			Log.error() << "[PyInterpreter] Could not allocate dummy tuple" << std::endl;
			Py_DecRef(func);
			Py_DecRef(dict);
			return false;
		}

		auto res = PyObject_Call(func, dummy, dict);

		Py_DecRef(res);
		Py_DecRef(func);
		Py_DecRef(dummy);
		Py_DecRef(dict);
		return true;
	}

	PyObject* PyCAPIKernel::loadModule(const string& name)
	{
		PyObject* module_dict = PyImport_GetModuleDict();

		if (!module_dict)
		{
			Log.error() << "[PyInterpreter] Could not obtain module dictionary" << std::endl;
			return nullptr;
		}

		auto mod_name = PyString_FromString(name.c_str());

		PyObject* module = nullptr;
		if (PyDict_Contains(module_dict, mod_name))
		{
			module = PyDict_GetItem(module_dict, mod_name);
		}
		else
		{
			module = PyImport_ImportModule(name.c_str());
			if (module) modules_.push_back(module); // module will be de-allocated on kernel destruction
		}

		Py_DecRef(mod_name);

		if (PyErr_Occurred())
		{
			PyErr_Print();
			return nullptr;
		}

		return module;
	}
}
