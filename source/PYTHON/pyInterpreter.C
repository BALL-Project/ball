// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/PYTHON/pyInterpreter.h>

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/PYTHON/BALLPythonConfig.h>
#include <BALL/PYTHON/pyCAPIKernel.h>

using std::string;
using std::tie;

namespace BALL
{
	PyKernel* PyInterpreter::kernel_ = nullptr;
	PyInterpreter::PathStrings PyInterpreter::sys_path_;

	void PyInterpreter::finalize()
	{
		if(kernel_) delete kernel_;
		kernel_ = nullptr;
	}

	void PyInterpreter::initialize()
	{
		// finalize the interpreter if it is already running
		if(kernel_) finalize();

		kernel_ = new PyCAPIKernel();

		// import sys
		run("import sys");

		// Add the BALL library path to the Python search path
		// to make sure Python can find the BALL extensions.
		Path p;

#ifndef BALL_COMPILER_MSVC
		run("sys.path.append('" BALL_PATH "/lib/')");

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
		if (python_startup_path != "") sys_path_.push_back(python_startup_path);

		// Add additional paths (user-defined) to the end of the search path.
		for (const auto& s: sys_path_)
		{
			run(string("sys.path.append(\"") + s.c_str() + "\")");
		}

		// make sure that we found the correct sip version
		run("import sip");

		bool ok;
		string res;
		tie(ok, res) = run("sip.SIP_VERSION");
		auto module_sip_version = std::stoul(res);


		string module_sip_version_str;
		tie(ok, module_sip_version_str) = run("sip.SIP_VERSION_STR");

		auto module_major_minor   = module_sip_version & 0xFFFFFF00;
		auto module_bugfix        = module_sip_version & 0x000000FF;
		unsigned ball_major_minor = BALL_SIP_VERSION & 0xFFFFFF00;
		unsigned ball_bugfix      = BALL_SIP_VERSION & 0x000000FF;

		if ((module_major_minor != ball_major_minor) || (module_bugfix < ball_bugfix))
		{
			tie(ok, res) = run("sip");
			Log.error() << "ERROR: Version of imported sip module is not compatible with the version BALL "
			               "was built against.\n"
			               "If BALL was compiled using SIP version x.y.z then it can be used with any SIP "
			               "version x.y.z' where 'z' >= z.\n"
			               "Got (from " + res + ") " + module_sip_version_str + "; Expected " +
			               BALL_SIP_VERSION_STR +"\n";
			finalize();
			return;
		}

		// import the BALL module
		tie(ok, res) = run("from BALL import *");
		if (!ok)
		{
			Log.error() << "Could not import the BALL library! No Python support available." << std::endl;
			finalize();
		}
	}

	std::pair<bool, string> PyInterpreter::run(const string& s)
	{
		if (!isInitialized())
		{
			Log.error() << "Python: Interpreter is not initialized.\n";
			return { false, "" };
		}

		auto res = kernel_->run(s);
		if (!res.first) Log.error() << "Python: " << kernel_->getErrorMessage() << '\n';
		return res;
	}

	String PyInterpreter::run(const String& s, bool& state)
	{
		String ret;
		tie(state, ret) = run(string(s.c_str()));
		return ret;
	}

	String PyInterpreter::runFile(const String& filename)
	{
		if (!isInitialized()) return "";

		kernel_->runFile(filename.c_str());
		return "";
	}

	bool PyInterpreter::execute(const string& module, const string& func_name, const PyKernel::KeyValArgs& params)
	{
		return isInitialized() ? kernel_->execute(module, func_name, params) : false;
	}

	bool PyInterpreter::execute(const QString& module, const QString& func_name, const QList<QPair<QString, QString> >& params)
	{
		if (!isInitialized()) return false;

		PyKernel::KeyValArgs args;
		for (const auto& pair: params)
		{
			if (pair.first == "action" || pair.first == "module" || pair.first == "method") continue;
			args[pair.first.toStdString()] = pair.second.toStdString();
		}

		return execute(module.toStdString(), func_name.toStdString(), args);
	}
} // namespace BALL
