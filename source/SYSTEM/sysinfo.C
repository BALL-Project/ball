// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.C,v 1.17 2005/03/10 19:38:30 oliver Exp $
//

#include <BALL/SYSTEM/sysinfo.h>

//   #define BALL_PLATFORM_SOLARIS

#ifdef BALL_PLATFORM_SOLARIS
# undef BALL_HAS_SYS_SYSINFO_H
# include <stdlib.h>
# include <stdio.h>
# include <kstat.h>
# include <sys/sysinfo.h>
# include <sys/stat.h>
# include <sys/swap.h>
# include <sys/param.h>
# include <unistd.h>
#endif

#ifdef BALL_HAS_SYS_SYSINFO_H
#	 include <sys/sysinfo.h>
#	 include <BALL/SYSTEM/file.h>
#else
# ifdef BALL_COMPILER_MSVC
#	  define WIN32_LEAN_AND_MEAN
#	  include <windows.h>
# endif
# ifdef BALL_OS_DARWIN
#  include <sys/sysctl.h>
# endif
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
#ifdef BALL_COMPILER_MSVC

		LongIndex getAvailableMemory()
		{
			return getFreeMemory();
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

#else
#ifdef BALL_PLATFORM_SOLARIS

	LongIndex getFreeMemory()
	{
	 	return ((LongIndex)sysconf(_SC_AVPHYS_PAGES)) * sysconf(_SC_PAGE_SIZE);
	}

	Index getNumberOfProcessors()
	{
		return sysconf(_SC_NPROCESSORS_CONF);
	}

	LongIndex getTotalMemory()
	{
		return ((LongIndex)sysconf(_SC_PHYS_PAGES)) * sysconf(_SC_PAGE_SIZE);
	}

	LongIndex getAvailableMemory()
	{
		return getFreeMemory();
	}

	LongIndex getFreeSwapSpace()
	{
		long pgsize_in_kbytes = sysconf(_SC_PAGE_SIZE) / 1024L;

		int swap_count=swapctl(SC_GETNSWP, NULL);
		if (swap_count == -1 || swap_count == 0) return swap_count;

		/*
		 *  Although it's not particularly clear in the documentation, you're
		 *  responsible for creating a variable length structure (ie. the
		 *  array is within the struct rather than being pointed to
		 *  by the struct).  Also, it is necessary for you to allocate space
		 *  for the path strings (see /usr/include/sys/swap.h).
		 */
		swaptbl_t* st = (swaptbl_t*)malloc(sizeof(int) + swap_count * sizeof(struct swapent));
		if (st == NULL) return -1;

		st->swt_n = swap_count;
		for (int i=0; i < swap_count; i++) 
		{
			if ((st->swt_ent[i].ste_path = (char*)malloc(MAXPATHLEN)) == NULL)
			{
				return -1;
			}
		}

		swap_count = swapctl(SC_LIST, (void*)st);
		if (swap_count == -1) return -1;

		long swap_avail = 0;
		for (int i = 0; i < swap_count; i++) 
		{
			swap_avail += st->swt_ent[i].ste_free * pgsize_in_kbytes;
		}

		delete st;

		return ((LongIndex)swap_avail) * 1024;
	}

	LongIndex getBufferedMemory()
	{
		return 0;
	}

	Time getUptime()
	{
		return -1;
	}


#else
#ifdef BALL_OS_DARWIN

		LongIndex getAvailableMemory()
		{
			return getFreeMemory();
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
			int mib[2] = { CTL_HW, HW_NCPU };
			int num_proc = 0;
			size_t len = sizeof(num_proc);
			sysctl(mib, 2, &num_proc, &len, NULL, 0);
			return (Index)num_proc;
		}

		LongIndex getFreeSwapSpace()
		{
			return -1;
		}


#else // We have no idea how to retrieve that information on this
			// platform, so we just return -1 everywhere

		LongIndex getAvailableMemory()
		{
			return -1;
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
#endif
#endif

	} // namespace SysInfo

} // namespace BALL

