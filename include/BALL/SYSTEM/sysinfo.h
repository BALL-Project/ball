// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.h,v 1.10 2005/12/23 17:02:07 amoll Exp $
//

#ifndef BALL_SYSTEM_SYSINFO_H
#define BALL_SYSTEM_SYSINFO_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

namespace BALL
{
	/** Namespace for methods to obtain informations about the systems hardware.
	 		All memory values are returned in amounts of byte.
	 		See the man page for sysinfo, GetSystemInfo and /proc/cpuinfo under Linux.
	 		@see http://msdn.microsoft.com/library/en-us/memory/base/memorystatus_str.asp
			@see http://msdn.microsoft.com/library/en-us/memory/base/globalmemorystatusex.asp
			\ingroup System
	 */
	namespace SysInfo
	{
		/** The the total amount of free memory (under linux + getBufferedMemory()).
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT LongIndex getAvailableMemory();
	
		/** The the amount of freee memory.
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT LongIndex getFreeMemory();
		
		/** The the total amount of memory.
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT LongIndex getTotalMemory();

		/** Return the buffered memory under Linux
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT LongIndex getBufferedMemory();

		/** Return the the amount of free swap memory.
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT LongIndex getFreeSwapSpace();

		/** Get the uptime of the system.
		 		Works currently only under Unix, not under Windows.
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT Time getUptime();

		/** Get the number of processors
				@return -1 if no valid value could be read
		*/
		BALL_EXPORT Index getNumberOfProcessors();
	}
}

#endif // BALL_SYSTEM_SYSINFO_H
