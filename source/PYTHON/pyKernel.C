#include <BALL/PYTHON/pyKernel.h>

#include <BALL/SYSTEM/path.h>

#include <sstream>

namespace BALL
{
	void PyKernel::loadScriptDir()
	{
		Path p;
		std::stringstream ss { p.getDataPath().c_str() };
		string line;
		run("import sys");
		while(std::getline(ss, line))
		{
			run("sys.path.append('" + line + "python')");
		}
	}
}
