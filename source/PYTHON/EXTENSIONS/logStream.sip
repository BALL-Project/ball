// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: logStream.sip,v 1.1.6.1 2006/05/15 23:18:40 amoll Exp $
//
// Author:
//   Oliver Kohlbacher
//

%ModuleHeaderCode
  #include <BALL/COMMON/logStream.h>
	#include <fstream>
	using namespace BALL;
	static std::ofstream py_oflogstream_;
%End

	void redirectLogToFile(String /filename/);
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
