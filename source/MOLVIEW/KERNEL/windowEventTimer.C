// $Id: windowEventTimer.C,v 1.3.4.1 2002/10/18 14:48:31 amoll Exp $
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

	} // namespace MOLVIEW

} // namespace BALL
