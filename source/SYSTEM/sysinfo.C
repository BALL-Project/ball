// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.10 2005/02/28 20:20:31 oliver Exp $
//

#include <BALL/SYSTEM/sysinfo.h>

#ifdef BALL_HAS_SYSINFO
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

		LongIndex getAvailableMemory()
		{
			LongIndex mem = getFreeMemory();
#ifndef BALL_PLATFORM_WINDOWS
			mem += getBufferedMemory();
#endif
			return mem;
		}

		LongIndex getFreeMemory()
		{
#ifdef BALL_HAS_SYSINFO
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.freeram * info.mem_unit;
#else
#	ifdef BALL_PLATFORM_WINDOWS
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullAvailPhys);
#	else
			return -1;
#	endif
#endif
		}

		LongIndex getTotalMemory()
		{
#ifdef BALL_HAS_SYSINFO
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.totalram * info.mem_unit;
#else
#	ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullTotalPhys);
#	else
			return -1;
#endif
#endif
		}

		LongIndex getBufferedMemory()
		{
#ifdef BALL_HAS_SYSINFO
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.bufferram * info.mem_unit;
#else
#	ifdef BALL_PLATFORM_WINDOWS
			return -1;
#	else
			return -1;
#	endif
#endif
		}

		Time getUptime()
		{
#ifdef BALL_HAS_SYSINFO
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.uptime;
#else
#	ifdef BALL_PLATFORM_WINDOWS
			return -1;
#	else
			return -1;
#	endif
#endif
		}

		Index getNumberOfProcessors()
		{
#ifdef BALL_HAS_SYSINFO
			return get_nprocs();
#else
#	ifdef BALL_PLATFORM_WINDOWS
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;
#	else
			return -1;
#	endif
#endif
		}


		LongIndex getFreeSwapSpace()
		{
#ifdef BALL_HAS_SYSINFO
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeswap * info.mem_unit;
#else
#	ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (LongIndex) statex.ullAvailPageFile;
#	else
			return -1;
#	endif
#endif
		}

	} // namespace SysInfo

} // namespace BALL

