// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: sysinfo.h,v 1.1 2005/01/25 11:27:02 amoll Exp $
//

#ifndef BALL_SYSTEM_TCPTRANSFER
#define BALL_SYSTEM_TCPTRANSFER

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

namespace BALL
{
	/** Namespace for methods to obtain informations about the systems hardware.
	 		@see http://msdn.microsoft.com/library/en-us/memory/base/memorystatus_str.asp
	 \ingroup System
	 */
	namespace SysInfo
	{
		///
		Index getFreeMemory();
		
		///
		Index getTotalMemory();

		///
		Index getBufferdMemory();

		///
//   		Index getAvailableMemory();
		
		///
		float getUptime();
	}
}

#endif // BALL_SYSTEM_TCPTRANSFER_H 
