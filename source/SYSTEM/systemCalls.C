// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: systemCalls.C,v 1.1.6.2 2007/08/08 09:21:34 oliver Exp $
//

#include <BALL/SYSTEM/systemCalls.h>

#ifdef BALL_OS_WINDOWS
#include <windows.h>
#else
#ifdef BALL_HAS_UNISTD_H
#include <unistd.h>
#endif
#endif

namespace BALL
{
	void sleepFor(Size mseconds)
	{
#ifdef BALL_OS_WINDOWS
 		Sleep(mseconds);
#else
		usleep(mseconds);
#endif
	}
} // namespace BALL

