// $Id: windowEventTimer.C,v 1.2 1999/12/19 17:14:29 oliver Exp $
#include <BALL/MOLVIEW/KERNEL/windowEventTimer.h>


namespace BALL
{

	namespace MOLVIEW
	{

		WindowEventTimer::WindowEventTimer
			()
				:
				QTTimer(),
				application_(0)
		{
		}

		WindowEventTimer::WindowEventTimer
			(const QApplication &__rQApplication)
				:
				QTTimer(),
				application_((QApplication *)&__rQApplication)
		{
		}

		WindowEventTimer::WindowEventTimer
			(const WindowEventTimer &__rWindowEventTimer,
			 bool deep)
				:
				QTTimer(__rWindowEventTimer, deep),
				application_(__rWindowEventTimer.application_)
		{
		}

		WindowEventTimer::~WindowEventTimer
			()
		{
			#ifdef BALL_MOLVIEW_DEBUG
				cout << "Destructing object " << (void *)this 
			 << " of class " << getBallClass().getName() << endl;
			#endif 

			destroy();
		}

		void
		WindowEventTimer::timer
			()
		{
			if (application_ != 0)
			{
				stopTimer();

				while (1)
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
