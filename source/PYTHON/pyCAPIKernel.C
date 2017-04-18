#include <BALL/PYTHON/pyCAPIKernel.h>

#include <BALL/COMMON/logStream.h>
#include <BALL/FORMAT/lineBasedFile.h>

using std::map;
using std::string;

namespace BALL
{
	PyCAPIKernel::PyCAPIKernel() :
		PyKernel{},
		main_module_{nullptr},
		context_{nullptr}
	{
		Py_Initialize();

		// get a borrowed reference to the main module
		main_module_ = PyImport_AddModule("__main__");
		if (!main_module_)
		{
			Log.error() << "Python intialization failed: could not add \"__main__\" module! No Python support available.\n";
			return;
		}

		// and create a valid context for our further Python calls
		context_ = PyModule_GetDict(main_module_);
		if (!context_)
		{
			Log.error() << "Could not get valid context for Python module \"__main__\"! No Python support available.\n";
			return;
		}

		// import modules for output redirection
		PyRun_SimpleString("import sys, cStringIO");
	}

	PyCAPIKernel::~PyCAPIKernel()
	{
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

	string PyCAPIKernel::getErrorMessage() const
	{
		if(!PyErr_Occurred()) return "";

#ifdef BALL_VIEW_DEBUG
		PyErr_Print();
#endif

		string err;
		PyObject* type;
		PyObject* value;
		PyObject* range;

		char* message;
		PyErr_Fetch(&type, &value, &range);
		err = "ERROR: " +
		      PyArg_Parse(value, "s", &message) ? message : "(unknown error)"
		      "\n";

		Py_DecRef(type);
		Py_DecRef(value);
		Py_DecRef(range);

		return err;
	}

	std::pair<bool, string> PyCAPIKernel::run(string str)
	{
#ifdef BALL_VIEW_DEBUG
		Log.info() << ">>> " << str << '\n';
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
		PyRun_String(str.c_str(), Py_single_input, context_, context_);

		if (PyErr_Occurred()) return {false, ""};

		auto cio = PyObject_GetAttrString(main_module_, "__BALL_CIO");
		auto cio_val = PyObject_GetAttrString(cio, "getvalue");
		auto ret = std::make_pair(true, PyString_AsString(PyObject_CallFunction(cio_val, nullptr)));
		Py_DecRef(cio_val);
		Py_DecRef(cio);
		return ret;
	}

	bool PyCAPIKernel::runFile(string filename)
	{
		FILE* file = fopen(filename.c_str(), "r");
		if(!file) throw Exception::FileNotFound(__FILE__, __LINE__, filename.c_str());

		PyErr_Clear();
		PyRun_SimpleFileEx(file, filename.c_str(), true);

		if (PyErr_Occurred())
		{
			Log.error() << "Error occurred while executing " << filename << "\n";
			PyErr_Print();
			return false;
		}
		return true;
	}

	bool PyCAPIKernel::execute(const string& module, const string& func_name, const KeyValArgs& args)
	{
		PyErr_Clear();

		PyObject* mod = loadModule(module);

		if (!mod)
		{
			Log.error() << "Could not load module " << module << std::endl;
			return false;
		}

		PyObject* func = PyObject_GetAttrString(mod, func_name.c_str());

		if (!func || !PyCallable_Check(func))
		{
			PyErr_Print();
			return false;
		}

		PyObject* dict = PyDict_New();

		if (!dict)
		{
			Log.error() << "Error: Could not create named arguments dictionary" << std::endl;
			return false;
		}

		for (const auto& pair: args)
		{
			PyObject* val = PyString_FromString(pair.second.c_str());
			if (!val)
			{
				Log.error() << "Could not create parameter" << pair.first << "=" << pair.second << " Skipping." << std::endl;
				continue;
			}
			PyDict_SetItemString(dict, pair.first.c_str(), val);
		}

		Py_DecRef(dict);

		auto dummy = PyTuple_New(0);

		if (!dummy)
		{
			Log.error() << "Could not allocate dummy tuple" << std::endl;
			return false;
		}

		auto res = PyObject_Call(func, dummy, dict);

		Py_DecRef(res);
		Py_DecRef(dummy);

		return true;
	}

	PyObject* PyCAPIKernel::loadModule(const string& name)
	{
		PyObject* module_dict = PyImport_GetModuleDict();

		if (!module_dict)
		{
			Log.error() << "Could not obtain module dictionary" << std::endl;
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
			//This could leak the imported module. Need to check...
			module = PyImport_ImportModule(name.c_str());
		}

		Py_DecRef(mod_name);


		if (!module || PyErr_Occurred())
		{
			PyErr_Print();
			return nullptr;
		}

		return module;
	}
}
