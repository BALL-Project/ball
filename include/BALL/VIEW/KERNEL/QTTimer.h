// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: QTTimer.h,v 1.22 2004/02/26 08:41:39 anhi Exp $
//

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
		/** QTTimer class.		
				The QTTimer class is a timer class that is useful to have a job repeated
				unhindered by other jobs that are already processing.
				The protected method <b> timer</b> will be called every <b> intervall</b> msec
				to process its contents. It is important that the process time for executing
				the code in the <b> timer</b> method is minimal else the application will hang
				for the time needed by the <b> timer</b> method.
				The <b> timer</b> method will not save any values when it exits. Therefore
				the necessary initialization and value savings must be done by the user in the
				<b> timer</b> method.
				The class is derived from the QObject class from the QT library. 
				Therefore it will only function within a qt application.
				\ingroup ViewKernelClient				 		
		*/
		class BALL_EXPORT QTTimer: public QObject
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** NoValidInterval Exception class.
					This exeption will be thrown if a timer interval is an invalid value
					(e.g a negative value is an invalid interval).
					\see         Exception::GeneralException			
			*/
			class NoValidInterval:	public Exception::GeneralException
			{
				public:

				NoValidInterval(const char* file, int line, const string& data)
  				throw();
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					The state is:
					  - interval is set to 100
						- timer is halted
					\par
					See documentation of the QT-library for information about QObjects.
			*/
			QTTimer(QObject* parent=0, const char* name=0)
				throw();

			/** Copy constructor.
			*/
			QTTimer(const QTTimer &qt_timer, QObject* parent=0, const char* name=0)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
			*/
			virtual ~QTTimer()
				throw();

			/** Explicit default initialization.
					Sets the state of this qtTimer to the default values.
					The state is:
					  - interval is set to 100
						- timer is halted
					\par
			*/
			virtual void clear()
				throw();

			//@}		
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment with cloning facility.
					The timer is halted.\par
					\param       qt_timer the QTTimer to be copied
			*/
			void set(const QTTimer& qt_timer)
				throw();

			/** Assignment operator.
					Calls set.
			*/
			const QTTimer& operator = (const QTTimer& qt_timer)
				throw();

			/** Swapping of QTTimer's.
					The timer is halted.
			*/
			void swap(QTTimer& qt_timer)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Changes the interval of this qtTimer to the value
					represented by the parameter <b> interval</b>. This QTTimer will
					call the method <b> timer</b> every <b> interval</b> msec.
					\param       interval the new interval of this qtTimer
					\exception   NoValidInterval thrown if the value of <b> interval</b> is lower or equal <tt> 0</tt>
					\see         startTimer
					\see         stopTimer
					\see         timer
			*/
			void setInterval(int interval)
				throw(NoValidInterval);

			/** Accesses the interval of this qtTimer.
					\return      int the value of the interval of this qtTimer
					\see         setInterval
			*/
			int getInterval() const
				throw();

			/** Starts the timer.
					After this method is called the method <b> timer</b> will be called
					every <b> interval</b> msec.
					\see     stopTimer
					\see     timer
			*/
			void startTimer()
				throw();
			
			/** Stops the timer.
					After this method is called the execution of the <b> timer</b> method
					will be stopped.
					\see     startTimer
					\see     timer
			*/
			void stopTimer()
				throw();

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Tests if this qtTimer is running.
					\see     startTimer
					\see     stopTimer
			*/
			bool isTimerEnabled() const
				throw();

			/** Tests if this qtTimer is stopped.
					\see     startTimer
					\see     stopTimer
			*/
			bool isTimerDisabled() const
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current state of this QTTimer to 
					the output ostream <b> s</b> with dumping depth <b> depth</b>.
					\param   s output stream where to output the state of this qtTimer
					\param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}

			protected:


			/** @name Timer method.
			*/
			//@{
			
			/** Timer method.
					Virtually overridable method. Code in this method will be executed every
					<b> interval</b> msec. It is important that the execution time of the code
					in this method must be minimal.
					The method is not responsible for maintaining, saving and restoring
					the values of the code used in this method. Therefore it is necessary
					to store and restore any necessary values needed by the code.
					If the execution time of the code in this method is too long the main 
					application	will hang. Any loops in the code that need a lot of time must
					be rewritten so every timer call will advance the "loop" a step further.
					Try to avoid loops.
					\see    startTimer
					\see    stopTimer
			*/
			virtual void timer();
			//@}

			private:

			virtual void timerEvent(QTimerEvent* timer_event);

			int timer_id_;
			int interval_;

			bool timer_enabled_;
			bool timer_running_;
		};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/VIEW/KERNEL/QTTimer.iC>
#	endif
  
} } // namespaces

#endif // BALL_VIEW_KERNEL_QTTIMER_H
