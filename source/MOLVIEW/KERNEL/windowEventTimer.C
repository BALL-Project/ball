// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: windowEventTimer.C,v 1.3.2.1 2003/01/07 13:21:35 anker Exp $
#include <BALL/MOLVIEW/KERNEL/windowEventTimer.h>

namespace BALL
{
	namespace MOLVIEW
	{

		WindowEventTimer::WindowEventTimer()
			: QTTimer(),
				application_(0)
		{
		}

		WindowEventTimer::WindowEventTimer(const QApplication& rQApplication)
			: QTTimer(),
				application_((QApplication *)& rQApplication)
		{
		}

		WindowEventTimer::WindowEventTimer(const WindowEventTimer& rWindowEventTimer, bool deep)
			: QTTimer(rWindowEventTimer, deep),
				application_(rWindowEventTimer.application_)
		{
		}

		WindowEventTimer::~WindowEventTimer()
		{
			#ifdef BALL_MOLVIEW_DEBUG
				cout << "Destructing object " << (void *)this << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void WindowEventTimer::timer()
		{
			if (application_ != 0)
			{
				stopTimer();

				while (true)
				{
					application_->processOneEvent();
				}
			}
		}

#		ifdef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/MOLVIEW/KERNEL/windowEventTimer.iC>
#		endif 

	} // namespace MOLVIEW

} // namespace BALL
