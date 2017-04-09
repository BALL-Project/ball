// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/PYTHON/pyInterpreter.h>

#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/PYTHON/BALLPythonConfig.h>
#include <BALL/PYTHON/legacyKernel.h>

using std::string;

namespace BALL
{
	PyKernel* PyInterpreter::kernel_ = nullptr;
	String PyInterpreter::start_log_;
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

		kernel_ = new LegacyKernel();
		bool ok;
		start_log_.clear();

		// import sys
		run("import sys", ok);
		if (!ok) start_log_ += kernel_->getErrorMessage();

		// Add the BALL library path to the Python search path
		// to make sure Python can find the BALL extensions.
		Path p;

#ifndef BALL_COMPILER_MSVC
		run("sys.path.append(\"" BALL_PATH "/lib/" "\")", ok);
		if (!ok) start_log_ += kernel_->getErrorMessage();

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
			run(string("sys.path.append(\"") + s.c_str() + "\")", ok);
			if (!ok) start_log_ += kernel_->getErrorMessage();
		}

		// make sure that we found the correct sip version
		run("import sip", ok);
		if (!ok) start_log_ += kernel_->getErrorMessage();
		auto module_sip_version = run("sip.SIP_VERSION", ok).trim().toUnsignedInt();
		if (!ok) start_log_ += kernel_->getErrorMessage();
		auto module_sip_version_str = run("sip.SIP_VERSION_STR", ok).trim();
		if (!ok) start_log_ += kernel_->getErrorMessage();

		auto module_major_minor = module_sip_version & 0xFFFFFF00;
		auto module_bugfix      = module_sip_version & 0x000000FF;
		unsigned ball_major_minor   = BALL_SIP_VERSION & 0xFFFFFF00;
		unsigned ball_bugfix        = BALL_SIP_VERSION & 0x000000FF;

		if ((module_major_minor != ball_major_minor) || (module_bugfix < ball_bugfix))
		{
			auto sip_module_path = run("sip", ok).trim();
			if (!ok) start_log_ += kernel_->getErrorMessage();
			start_log_ += "ERROR: Version of imported sip module is not compatible with the version BALL "
			              "was built against.\n"
			              "If BALL was compiled using SIP version x.y.z then it can be used with any SIP "
			              "version x.y.z' where 'z' >= z.\n"
			              "Got (from " + sip_module_path + ") " + module_sip_version_str + "; Expected " +
			              BALL_SIP_VERSION_STR +"\n";
			finalize();
			return;
		}

		// import the BALL module
		run("from BALL import *", ok);
		if (!ok)
		{
			start_log_ += kernel_->getErrorMessage();
			Log.error() << "Could not import the BALL library! No Python support available." << std::endl;
			finalize();
		}
	}

	String PyInterpreter::run(const String& s, bool& state)
	{
		state = false;
		if (!isInitialized()) return "";

		String ret;
		std::tie(state, ret) = kernel_->run(s.c_str());
		return ret;
	}

	String PyInterpreter::runFile(const String& filename)
	{
		if (!isInitialized()) return "";

		kernel_->runFile(filename.c_str());
		return "";
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

		return kernel_->execute(module.toStdString(), func_name.toStdString(), args);
	}
} // namespace BALL
