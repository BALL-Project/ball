// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: systemCalls.C,v 1.1.6.1 2007/03/25 22:00:35 oliver Exp $

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

