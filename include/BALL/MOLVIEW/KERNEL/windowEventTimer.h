// $Id: windowEventTimer.h,v 1.3.4.1 2002/10/18 07:49:43 amoll Exp $

#ifndef BALL_MOLVIEW_KERNEL_WINDOWEVENTTIMER_H
#define BALL_MOLVIEW_KERNEL_WINDOWEVENTTIMER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_VIEW_KERNEL_QTTIMER_H
#	include <BALL/VIEW/KERNEL/QTTimer.h>
#endif

#	include <qapplication.h>

namespace BALL
{

	namespace MOLVIEW
	{

		using VIEW::QTTimer;

		/**
		*/
		class WindowEventTimer
			: public QTTimer
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			WindowEventTimer();

			WindowEventTimer
				(const QApplication& application);

			WindowEventTimer
				(const WindowEventTimer& window_event_timer, bool deep = true);

			virtual ~WindowEventTimer();
			//@}


			protected:

			virtual void timer();
			

			private:

			QApplication* application_;
		};


	} // namespace MOLVIEW

} // namespace BALL

#endif // BALL_MOLVIEW_KERNEL_WINDOWEVENTTIMER_H
