// $Id: QTTimer.h,v 1.3 1999/12/28 18:37:45 oliver Exp $

#ifndef BALL_VIEW_KERNEL_QTTIMER_H
#define BALL_VIEW_KERNEL_QTTIMER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef QOBJECT_H
#	include <qobject.h>
#endif

namespace BALL
{
	
	namespace VIEW
	{

		/**
		*/
		class QTTimer
			: protected QObject
		{
			public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			QTTimer();

			QTTimer(const QTTimer& timer, bool deep = true);

			virtual ~QTTimer();

			virtual void clear();

			virtual void destroy();
			//@}
		
			/**	@name	Assignment
			*/
			//@{

			void set(const QTTimer& timer, bool deep = true);

			QTTimer& operator = (const QTTimer& timer);

			void get(QTTimer& timer, bool deep = true) const;

			void swap(QTTimer& timer);
			//@}

			/**	@name	Accessors
			*/
			//@{

			void setIntervall(int intervall);

			int getIntervall() const;

			void startTimer();

			void stopTimer();
			//@}

			/**	@name	Predicates
			*/
			//@{

			bool isTimerEnabled() const;

			bool isTimerDisabled() const;
			//@}

			/**	@name	Debugging and Diagnostics
			*/
			//@{
			virtual bool isValid() const;

			virtual void dump
				(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{
			/**	
			*/
			virtual void read(std::istream& s);

			/**	
			*/
			virtual void write(std::ostream& s) const;
			//@}


			protected:

			virtual void timer();



			private:

			virtual void timerEvent(QTimerEvent* timer_event);

			int timer_id_;
			int intervall_;

			bool timer_enabled_;
			bool timer_running_;
		};



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/QTTimer.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_QTTIMER_H
