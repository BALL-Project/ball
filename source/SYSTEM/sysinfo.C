// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.2 2005/01/25 14:42:36 amoll Exp $
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
		Index getFreeMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			/*
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (Index) ms.dwAvailPhys;
			*/
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (Index) statex.ullAvailPhys
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeram;
#endif
		}

		Index getTotalMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			/*
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (Index) ms.dwTotalPhys;
			*/
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (Index) statex.ullFullPhys
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
			if (result == -1) return result;
			return info.totalram;
#endif
		}

		Index getBufferdMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
#else
			struct sysinfo info;
			Index result = sysinfo(&info);
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
			Index result = sysinfo(&info);
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


	} // namespace SysInfo
} // namespace BALL

