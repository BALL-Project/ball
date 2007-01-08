// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: path.sip,v 1.2 2005/07/03 09:43:26 oliver Exp $
//

class Path 
{
%TypeHeaderCode
#include <BALL/SYSTEM/path.h>
using namespace BALL;
%End

	public:
	Path();
	String getDataPath();
	void setDataPath(const String& /path/);
	void addDataPath(const String& /path/);
	String getDefaultDataPath();
	String find(const String& /name/);
	String findStrict(const String& /name/);
};
