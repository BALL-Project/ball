// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.1 2005/01/25 11:27:15 amoll Exp $
//

#include <BALL/SYSTEM/sysinfo.h>

#ifndef _WIN32
//    #include <linux/kernel.h>
//    #include <linux/sys.h>
 #include <sys/sysinfo.h>
#else
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
#endif

namespace BALL
{
	namespace SysInfo
	{
		Index getFreeMemory()
		{
#ifdef _WIN32
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (Index) ms.dwAvailPhys;
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeram;
#endif
		}

		Index getTotalMemory()
		{
#ifdef _WIN32
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (Index) ms.dwTotalPhys;
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
			if (result == -1) return result;
			return info.totalram;
#endif
		}

		Index getBufferdMemory()
		{
#ifdef _WIN32
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
			if (result == -1) return result;
			return info.bufferram;
#endif
		}

		float getUptime()
		{
#ifdef _WIN32
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
			if (result == -1) return result;
			return info.uptime;
#endif
		}

	} // namespace SysInfo
} // namespace BALL

