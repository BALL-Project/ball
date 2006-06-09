// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: systemCalls.C,v 1.1.4.1 2006/06/09 15:00:26 leonhardt Exp $

#include <BALL/SYSTEM/systemCalls.h>

#ifdef BALL_PLATFORM_WINDOWS
 #include <windows.h>
#endif

namespace BALL
{
	void sleepFor(Size mseconds)
	{
#ifdef BALL_PLATFORM_WINDOWS
 		Sleep(mseconds);
#else
		usleep(mseconds);
#endif
	}
} // namespace BALL

