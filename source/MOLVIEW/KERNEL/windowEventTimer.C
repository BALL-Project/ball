// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: windowEventTimer.C,v 1.4 2002/02/27 12:23:50 sturm Exp $
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
