// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.11 2005/03/01 07:33:10 oliver Exp $
//

#include <BALL/SYSTEM/sysinfo.h>

#ifdef BALL_HAS_SYS_SYSINFO_H
#	include <sys/sysinfo.h>
#	include <BALL/SYSTEM/file.h>
#else
# ifdef BALL_PLATFORM_WINDOWS
#	  define WIN32_LEAN_AND_MEAN
#	  include <windows.h>
# endif
# ifdef BALL_PLATFORM_DARWIN
#  include <sys/sysctl.h>
#	endif
#endif

namespace BALL
{
	namespace SysInfo
	{


#ifdef BALL_HAS_SYS_SYSINFO_H

		LongIndex getAvailableMemory()
		{
			LongIndex mem = getFreeMemory();
			return mem;
		}

		LongIndex getFreeMemory()
		{
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.freeram * info.mem_unit;
		}

		LongIndex getTotalMemory()
		{
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.totalram * info.mem_unit;
		}

		LongIndex getBufferedMemory()
		{
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.bufferram * info.mem_unit;
		}

		Time getUptime()
		{
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.uptime;
		}

		Index getNumberOfProcessors()
		{
			return get_nprocs();
		}


		LongIndex getFreeSwapSpace()
		{
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeswap * info.mem_unit;
		}
#else
#ifdef BALL_PLATFORM_WINDOWS

		LongIndex getAvailableMemory()
		{
			return getFreememory();
		}

		LongIndex getFreeMemory()
		{
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullAvailPhys);
		}

		LongIndex getTotalMemory()
		{
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullTotalPhys);
		}

		LongIndex getBufferedMemory()
		{
			return 0;
		}

		Time getUptime()
		{
			return -1;
		}

		Index getNumberOfProcessors()
		{
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;
		}

		LongIndex getFreeSwapSpace()
		{
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (LongIndex) statex.ullAvailPageFile;
		}

#else // We have no idea how to retrieve that information on this
			// platform, so we just return -1 everywhere

		LongIndex getAvailableMemory()
		{
			LongIndex mem = getFreeMemory();
			return mem;
		}

		LongIndex getFreeMemory()
		{
			return -1;
		}

		LongIndex getTotalMemory()
		{
			return -1;
		}

		LongIndex getBufferedMemory()
		{
			return -1;
		}

		Time getUptime()
		{
			return -1;
		}

		Index getNumberOfProcessors()
		{
			return -1;
		}

		LongIndex getFreeSwapSpace()
		{
			return -1;
		}

#endif
#endif

	} // namespace SysInfo

} // namespace BALL

