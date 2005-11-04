// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.sip,v 1.1.2.1 2005/11/04 20:04:50 oliver Exp $
//

namespace SysInfo
{
%TypeHeaderCode
	#include <BALL/SYSTEM/sysinfo.h>
	using namespace BALL;
%End
	LongIndex getAvailableMemory();
	LongIndex getFreeMemory();
	LongIndex getTotalMemory();
	LongIndex getBufferedMemory();
	LongIndex getFreeSwapSpace();
	Time getUptime();
	Index getNumberOfProcessors();
};
