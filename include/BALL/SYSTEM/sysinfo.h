// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.h,v 1.4 2005/01/25 15:14:27 amoll Exp $
//

#ifndef BALL_SYSTEM_SYSINFO_H
#define BALL_SYSTEM_SYSINFO_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

namespace BALL
{
	/** Namespace for methods to obtain informations about the systems hardware.
	 		See the man page for sysinfo, GetSystemInfo and /proc/cpuinfo under Linux.
	 		@see http://msdn.microsoft.com/library/en-us/memory/base/memorystatus_str.asp
			@see http://msdn.microsoft.com/library/en-us/memory/base/globalmemorystatusex.asp
			\ingroup System
	 */
	namespace SysInfo
	{
		/** The the total amount of free memory (under linux + getBufferdMemory()).
				@return -1 if no valid value could be read
		*/
		long getAvailableMemory();
	
		/** The the amount of freee memory.
				@return -1 if no valid value could be read
		*/
		long getFreeMemory();
		
		/** The the total amount of memory.
				@return -1 if no valid value could be read
		*/
		long getTotalMemory();

		/** Return the bufferd memory under Linux
				@return -1 if no valid value could be read
		*/
		long getBufferdMemory();

		/** Return the the amount of free swap memory.
				@return -1 if no valid value could be read
		*/
		long getFreeSwapSpace();

		/** Get the uptime of the system.
		 		Work currently only under Linux.
				@return -1 if no valid value could be read
		*/
		float getUptime();

		/** Get the number of processors
				@return -1 if no valid value could be read
		*/
		Index getNumberOfProcessors();
	}
}

#endif // BALL_SYSTEM_SYSINFO_H
