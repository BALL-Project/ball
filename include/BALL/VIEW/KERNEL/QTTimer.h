// $Id: QTTimer.h,v 1.5 2001/02/04 15:58:20 hekl Exp $

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
				
				{\bf Framework:} BALL/VIEW/KERNEL\\
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/QTTimer.h}
				\\
				The QTTimer class is a timer class that is usefull for having repeated a job
				unhindered by other jobs that are already processing.
				The protected method {\em timer} will be called every {\em intervall} msec
				to process its contents. It is important that the process time for executing
				the code in the {\em timer} method is minimal else the application will hang
				for the time needed by the {\em timer} method.
				The {\em timer} method will not save any values when it exits. Therefore
				the necessary initialization and value savings must be done by the user in the
				{\em timer} method.
				The class is derived from the \Ref{QObject} class from the QT library. 
				Therefore it only functions with qt objects.
				
				@memo    QTTimer class (BALL VIEW kernel framework)
				@author  $Author: hekl $
				@version $Revision: 1.5 $
				@date    $Date: 2001/02/04 15:58:20 $
		*/
		class QTTimer: public QObject
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** NoValidInterval Exception class.
					This exeption will be thrown if a timer interval is an invalid value
					(e.g a negative value is an invalid interval)

					@see         Exception::GeneralException			
			*/
			class NoValidInterval:	public Exception::GeneralException
			{
				public:

				NoValidInterval(const char* file, int line, const string& data);
			};
			//@}

			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new QTTimer.
					The state of {\em *this} qtTimer is:
					\begin{itemize}
					  \item interval is set to 100
						\item timer is halted
					\end{itemize}

					@return      QTTimer - new constructed qtTimer
					@see         QObject::QOBject
			*/
			QTTimer(QObject* parent=0, const char* name=0);

			/** Copy constructor.
					Construct new QTTimer by copying the qtTimer {\em timer}.
					The state of {\em *this} qtTimer is initialized to the state of the qtTimer {\em timer}.\\

					@param       timer the qtTimer to be copied
					@return      QTTimer - new constructed qtTimer copied from {\em timer}
					@see         QTTimer::QTTimer
			*/
			QTTimer(const QTTimer &timer, QObject* parent=0, const char* name=0);

			//@}

			/** @name Destructors */
			//@{

			/** Destructor.
					Default destruction of {\em *this} qtTimer.
					Calls \Ref{QTTimer::destroy}.
					@see         QTTimer::destroy
			*/
			virtual ~QTTimer();

			/** Explicit default initialization.
					Set the state of {\em *this} qtTimer to the default values.
					The state of {\em *this} qtTimer is:
					\begin{itemize}
					  \item interval is set to 100
						\item timer is halted
					\end{itemize}
			*/
			virtual void clear();

			/** Explicit destructor.
					Destroy {\em *this} qtTimer.
					Halts the timer.
			*/
			virtual void destroy();
			//@}
		
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment with cloning facility.
					Assign the qtTimer {\em timer} to {\em *this} qtTimer.
					The state of {\em *this} qtTimer is initialized to the state of 
					the qtTimer {\em timer}.
					The timer is halted.\\

					@param       timer the qtTimer to be copied
					@see         QTTimer::QTTimer
			*/
			void set(const QTTimer& timer);

			/** Assignment operator.
					Assign the qtTimer {\em timer} to {\em *this} qtTimer.
					Calls \Ref{QTTimer::set}.
					The state of {\em *this} qtTimer is initialized to the state 
					of the qtTimer {\em timer}.\\

					@param       timer the qtTimer to be copied
					@return      QTTimer& - {\em *this} qtTimer
					@see         QTTimer::set
			*/
			QTTimer& operator = (const QTTimer& timer);

			/** Copying.
					Copy {\em *this} qtTimer to the qtTimer {\em timer}.
					Calls \Ref{QTTimer::set}.
					The state of the qtTimer {\em timer} is initialized to the
					state of {\em *this} qtTimer.\\

					@param       timer the qtTimer to be assigned to
					@see         QTTimer::set
			*/
			void get(QTTimer& timer) const;

			/** Swapping of qtTimer's.
					Swap the states of {\em *this} qtTimer with the qtTimer
					{\em timer}. The timer is halted.

					@param       timer the qtTimer being swapped with {\em *this} qtTimer 
					@see         QTTimer::QTTimer
			*/
			void swap(QTTimer& timer);
			//@}

			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Change the interval of {\em *this} qtTimer.
					Change the interval of {\em *this} qtTimer to the value
					represented by the parameter {\em interval}. {\em *this} qtTimer will
					call the method {\em timer} every {\em interval} msec.

					@param       interval the new interval of {\em *this} qtTimer
					@exception   NoValidInterval - thrown if the value of {\em interval} is lower or equal {\tt 0}
					@see         QTTimer::startTimer
					@see         QTTimer::stopTimer
					@see         QTTimer::timer
			*/
			void setInterval(int interval);

			/** Inspection of the interval.
					Access the interval of {\em *this} qtTimer.

					@return      int - the value of the interval of {\em *this} qtTimer
					@see         QTTimer::setInterval
			*/
			int getInterval() const;

			/** Start the timer.
					Start the timer of {\em *this} qtTimer.
					After this method is called the method {\em timer} will be called
					every {\em interval} msec.

					@see     QTTimer::stopTimer
					@see     QTTimer::timer
			*/
			void startTimer();
			
			/** Stop the timer.
					Stop the timer of {\em *this} qtTimer.
					After this method is called the execution of the {\em timer} method
					will be stopped.

					@see     QTTimer::startTimer
					@see     QTTimer::timer
			*/
			void stopTimer();
			//@}

			/**	@name	Predicates
			*/
			//@{

			/** Test if {\em *this} qtTimer is running.

					@return  bool -	{\tt true} if {\em *this} qtTimer is running, {\tt false} otherwise
					@see     QTTimer::startTimer
					@see     QTTimer::stopTimer
			*/
			bool isTimerEnabled() const;

			/** Test if {\em *this} qtTimer is stopped.

					@return  bool -	{\tt true} if {\em *this} qtTimer is stopped, {\tt false} otherwise
					@see     QTTimer::startTimer
					@see     QTTimer::stopTimer
			*/
			bool isTimerDisabled() const;
			//@}

			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dump the current state of {\em *this} qtTimer to 
					the output ostream {\em s} with dumping depth {\em depth}.

					@param   s output stream where to output the state of {\em *this} qtTimer
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
			//@}

			/**	@name	Storers
			*/
			//@{
			/** Persistent stream output and state restorage.
  			 Read persistent qtTimer data from the input stream {\em s} and 
				 restore the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s input stream from where to restore the internal state of {\em *this} qtTimer
					@exception   NotImplemented - always
			*/
			virtual void read(std::istream& s);

			/** Persistent stream output and state storage.
  			 Write persistent qtTimer data to the output stream {\em s} and 
				 store the state of {\em *this}.
				 \\
				 {\bf Note:} Not yet implemented.
		 
				 @param       s output stream to where to store the internal state of {\em *this} qtTimer
					@exception   NotImplemented - always
			*/
			virtual void write(std::ostream& s) const;
			//@}


			protected:

			/** @name Timer method.
			*/
			//@{
			
			/** Timer method.
					Virtually overridable method. Code in this method will be executed every
					{\em interval} msec. It is important that the execution time of the code
					in this method must be minimal.
					The method is not responsible for maintaining, saving and restoring
					the values of the code used in this method. Therefore it is necessary
					to store and restore any necessary values needed by the code.
					If the execution time of the code in this method is to long the main 
					application	will hang. Any loops in the code that need a lot of time must
					be rewritten so every timer call will advance the "loop" a step further.
					Try to avoid loops.

					@see    QTTimer::startTimer
					@see    QTTimer::stopTimer
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



#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/KERNEL/QTTimer.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_KERNEL_QTTIMER_H
