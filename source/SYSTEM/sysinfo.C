// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.8 2005/01/28 13:29:04 amoll Exp $
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
						return line.toLong() * 1024;
					}
				}
			}
			catch(...)
			{
			}

			// sysinfo seems to return somewhat insane values, but better than nothing...
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.freeram;
#endif
		}

		LongIndex getTotalMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return static_cast<LongIndex>(statex.ullTotalPhys);
#else
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) 
			{
				return result;
			}
			return info.totalram;
#endif
		}

		LongIndex getBufferedMemory()
		{
#ifdef BALL_PLATFORM_WINDOWS
			return -1;
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
						return line.toLong() * 1024;
					}
				}
			}
			catch(...)
			{
			}

			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.bufferram;
#endif
		}

		Time getUptime()
		{
#ifdef BALL_PLATFORM_WINDOWS
			return -1;
#else
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
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


		LongIndex getFreeSwapSpace()
		{
#ifdef BALL_PLATFORM_WINDOWS
 			MEMORYSTATUSEX statex;
			GlobalMemoryStatusEx (&statex);
			return (LongIndex) statex.ullAvailPageFile;
#else
			struct sysinfo info;
			LongIndex result = sysinfo(&info);
			if (result == -1) return result;
			return info.freeswap;
#endif

		}

	} // namespace SysInfo
} // namespace BALL

