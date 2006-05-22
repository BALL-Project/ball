// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: systemCalls.C,v 1.1.2.1 2006/05/22 06:39:47 amoll Exp $

#include <BALL/SYSTEM/systemCalls.h>

#ifdef BALL_PLATFORM_WINDOWS
 #include <windows.h>
#endif

namespace BALL
{
	void sleep(Size mseconds)
	{
#ifdef BALL_PLATFORM_WINDOWS
 		Sleep(mseconds);
#else
		usleep(mseconds);
#endif
	}
} // namespace BALL

