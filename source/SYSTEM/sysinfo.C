// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.6 2005/01/27 12:39:15 amoll Exp $
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

		float getAvailableMemory()
		{
			float mem = getFreeMemory();
#ifndef BALL_PLATFORM_WINDOWS
			mem += getBufferdMemory();
#endif
			return mem;
		}

		float getFreeMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			/*
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (float) ms.dwAvailPhys;
			*/
			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (float) statex.ullAvailPhys;
#else
			try
			{
				File cpuinfo("/proc/meminfo");
				char buffer[1024];
				String line;
				while (!cpuinfo.eof())
				{
					cpuinfo.getline(buffer, 1024);
					line.assign(buffer);
					if (line.hasPrefix("MemFree:"))
					{
						line = line.after(":");
						line.trimLeft();
						line = line.before(" ");
						return line.toFloat() * 1024;
					}
				}
			}
			catch(...)
			{
			}

			// sysinfo seems to return somewhat unsane values, but better than nothing...
			struct sysinfo info;
			float result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeram;
#endif
		}

		float getTotalMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			/*
			MEMORYSTATUS ms;
			GlobalMemoryStatus (&ms);
			return (float) ms.dwTotalPhys;
			*/
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (float) statex.ullTotalPhys;
#else
			struct sysinfo info;
			float result = sysinfo(&info);
			if (result == -1) return result;
			return info.totalram;
#endif
		}

		float getBufferdMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
#else
			try
			{
				File cpuinfo("/proc/meminfo");
				char buffer[1024];
				String line;
				while (!cpuinfo.eof())
				{
					cpuinfo.getline(buffer, 1024);
					line.assign(buffer);
					if (line.hasPrefix("Cached:"))
					{
						line = line.after(":");
						line.trimLeft();
						line = line.before(" ");
						return line.toFloat() * 1024;
					}
				}
			}
			catch(...)
			{
			}

			struct sysinfo info;
			float result = sysinfo(&info);
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
			float result = sysinfo(&info);
			if (result == -1) return result;
			return info.uptime;
#endif
		}

		Index getNumberOfProcessors()
		{
#ifdef BALL_PLATFORM_WINDOWS
			SYSTEM_INFO sysinfo;
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


		float getFreeSwapSpace()
		{
#ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (float) statex.ullAvailPageFile;
#else
			struct sysinfo info;
			float result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeswap;
#endif

		}

	} // namespace SysInfo
} // namespace BALL

