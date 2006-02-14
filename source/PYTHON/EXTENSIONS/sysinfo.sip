// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.sip,v 1.2.6.1 2006/02/14 15:02:51 amoll Exp $
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
