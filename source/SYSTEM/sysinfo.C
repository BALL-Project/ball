// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.9 2005/01/29 18:02:02 oliver Exp $
//

#include <BALL/SYSTEM/sysinfo.h>

#ifndef BALL_PLATFORM_WINDOWS
#	include <sys/sysinfo.h>
#	include <BALL/SYSTEM/file.h>
#else
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
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
#ifdef BALL_PLATFORM_WINDOWS
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullAvailPhys);
#else
#ifdef BALL_OS_LINUX
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.freeram * info.mem_unit;
#else
			return -1;
#endif
#endif
		}

		LongIndex getTotalMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullTotalPhys);
#else
#ifdef BALL_OS_LINUX
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.totalram * info.mem_unit;
#else
			return -1;
#endif
#endif
		}

		LongIndex getBufferedMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			return -1;
#else
#ifdef BALL_OS_LINUX
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.bufferram * info.mem_unit;
#else
			return -1;
#endif
#endif
		}

		Time getUptime()
		{
#ifdef BALL_PLATFORM_WINDOWS
			return -1;
#else
#ifdef BALL_OS_LINUX
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.uptime;
#else
			return -1;
#endif
#endif
		}

		Index getNumberOfProcessors()
		{
#ifdef BALL_PLATFORM_WINDOWS
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;
#else
#ifdef BALL_OS_LINUX
			return get_nprocs();
#else
			return -1;
#endif
#endif
		}


		LongIndex getFreeSwapSpace()
		{
#ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (LongIndex) statex.ullAvailPageFile;
#else
#ifdef BALL_OS_LINUX
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeswap * info.mem_unit;
#else
			return -1;
#endif
#endif
		}

	} // namespace SysInfo

} // namespace BALL

