namespace SysInfo
{
%TypeHeaderCode
	#include <BALL/SYSTEM/sysinfo.h>
%End
	LongIndex getAvailableMemory();
	LongIndex getFreeMemory();
	LongIndex getTotalMemory();
	LongIndex getBufferedMemory();
	LongIndex getFreeSwapSpace();
	Time getUptime();
	Index getNumberOfProcessors();
};
