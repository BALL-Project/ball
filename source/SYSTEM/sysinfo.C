// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.4 2005/01/25 15:14:29 amoll Exp $
//

#include <BALL/SYSTEM/sysinfo.h>

#ifndef BALL_PLATFORM_WINDOWS
 #include <sys/sysinfo.h>
 #include <BALL/SYSTEM/file.h>
#else
 #define WIN32_LEAN_AND_MEAN
 #include <windows.h>
#endif

namespace BALL
{
	namespace SysInfo
	{

		long getAvailableMemory()
		{
			long mem = getFreeMemory();
#ifndef BALL_PLATFORM_WINDOWS
			mem += getBufferdMemory();
#endif
			return mem;
		}

		long getFreeMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			/*
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (long) ms.dwAvailPhys;
			*/
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (long) statex.ullAvailPhys
#else
			struct sysinfo info;
			long result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeram;
#endif
		}

		long getTotalMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			/*
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (long) ms.dwTotalPhys;
			*/
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (long) statex.ullFullPhys
#else
			struct sysinfo info;
			long result = sysinfo(&info);
			if (result == -1) return result;
			return info.totalram;
#endif
		}

		long getBufferdMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
#else
			struct sysinfo info;
			long result = sysinfo(&info);
			if (result == -1) return result;
			return info.bufferram;
#endif
		}

		float getUptime()
		{
#ifdef BALL_PLATFORM_WINDOWS
			return -1;
#else
			struct sysinfo info;
			long result = sysinfo(&info);
			if (result == -1) return result;
			return info.uptime;
#endif
		}

		Index getNumberOfProcessors()
		{
#ifdef BALL_PLATFORM_WINDOWS
			struct SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;
#else
			try
			{
				File cpuinfo("/proc/cpuinfo");
				char buffer[1024];
				String line;
				Index nr_processors = 0;
				while (!cpuinfo.eof())
				{
					cpuinfo.getline(buffer, 1024);
					line.assign(buffer);
					if (line.hasPrefix("processor"))
					{
						nr_processors++;
					}
				}

				if (nr_processors == 0) return -1;

				return nr_processors;
			}
			catch(...)
			{
			}

			return -1;
#endif
		}


		long getFreeSwapSpace()
		{
#ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (long) statex.ullAvailPageFile;
#else
			struct sysinfo info;
			long result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeswap;
#endif

		}

	} // namespace SysInfo
} // namespace BALL

