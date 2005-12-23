// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.sip,v 1.2 2005/12/23 17:03:00 amoll Exp $
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
