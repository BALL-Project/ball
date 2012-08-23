// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
// Author:
//   Oliver Kohlbacher
//

%ModuleCode
  #include <BALL/COMMON/logStream.h>
	#include <fstream>
	using namespace BALL;
	static std::ofstream py_oflogstream_;
%End

	void redirectLogToFile(String);
%MethodCode
	if (py_oflogstream_.is_open())
	{
		py_oflogstream_.close();
	}
	py_oflogstream_.open(a0->c_str());
	Log.insert(py_oflogstream_);
%End

	void endLogRedirection();
%MethodCode
	Log.remove(py_oflogstream_);
	py_oflogstream_.close();
%End
