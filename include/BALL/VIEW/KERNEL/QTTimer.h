// $Id: QTTimer.h,v 1.10 2001/07/16 14:49:32 amoll Exp $

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
				The protected method {\em timer} will be called every {\em intervall} msec
				to process its contents. It is important that the process time for executing
				the code in the {\em timer} method is minimal else the application will hang
				for the time needed by the {\em timer} method.
				The {\em timer} method will not save any values when it exits. Therefore
				the necessary initialization and value savings must be done by the user in the
				{\em timer} method.
				The class is derived from the \Ref{QObject} class from the QT library. 
				Therefore it will only function within a qt application.
				{\bf Definition:} \URL{BALL/VIEW/KERNEL/QTTimer.h}
		*/
		class QTTimer: public QObject
		{
			public:

			/**	@name	Exceptions
			*/
			//@{
			
			/** NoValidInterval Exception class.
					This exeption will be thrown if a timer interval is an invalid value
					(e.g a negative value is an invalid interval).
					@see         Exception::GeneralException			
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
					Constructs new QTTimer.
					The state of {\em *this} qtTimer is:
					\begin{itemize}
					  \item interval is set to 100
						\item timer is halted
					\end{itemize}
					See documentation of the QT-library for information about QObjects.
					@return      QTTimer new constructed qtTimer
			*/
			QTTimer(QObject* parent=0, const char* name=0)
				throw();

			/** Copy constructor.
					Constructs new QTTimer by copying the qtTimer {\em qt_timer}.
					The state of {\em *this} qtTimer is initialized to the state of the qtTimer {\em qt_timer}.\\
					@param       qt_timer the qtTimer to be copied
					@return      QTTimer new constructed qtTimer copied from {\em qt_timer}
			*/
			QTTimer(const QTTimer &qt_timer, QObject* parent=0, const char* name=0)
				throw();

			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} qtTimer.
					Calls \Ref{destroy}.
					@see         destroy
			*/
			virtual ~QTTimer()
				throw();

			/** Explicit default initialization.
					Sets the state of {\em *this} qtTimer to the default values.
					The state of {\em *this} qtTimer is:
					\begin{itemize}
					  \item interval is set to 100
						\item timer is halted
					\end{itemize}
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Destroys {\em *this} qtTimer.
					Halts the timer.
			*/
			virtual void destroy()
				throw();

			//@}		
			/**	@name	Assignment methods
			*/
			//@{

			/** Assignment with cloning facility.
					Assigns the qtTimer {\em qt_timer} to {\em *this} qtTimer.
					The state of {\em *this} qtTimer is initialized to the state of 
					the qtTimer {\em qt_timer}.
					The timer is halted.\\
					@param       qt_timer the qtTimer to be copied
			*/
			void set(const QTTimer& qt_timer)
				throw();

			/** Assignment operator.
					Assigns the qtTimer {\em qt_timer} to {\em *this} qtTimer.
					Calls \Ref{set}.
					The state of {\em *this} qtTimer is initialized to the state 
					of the qtTimer {\em qt_timer}.\\
					@param       qt_timer the qtTimer to be copied
					@return      QTTimer& {\em *this} qtTimer
					@see         set
			*/
			const QTTimer& operator = (const QTTimer& qt_timer)
				throw();

			/** Copying.
					Copies {\em *this} qtTimer to the qtTimer {\em qt_timer}.
					Calls \Ref{set}.
					The state of the qtTimer {\em qt_timer} is initialized to the
					state of {\em *this} qtTimer.\\
					@param       qt_timer the qtTimer to be assigned to
					@see         set
			*/
			void get(QTTimer& qt_timer) const
  			throw();

			/** Swapping of qtTimer's.
					Swaps the states of {\em *this} qtTimer with the qtTimer
					{\em qt_timer}. The timer is halted.
					@param       qt_timer the qtTimer being swapped with {\em *this} qtTimer 
			*/
			void swap(QTTimer& qt_timer)
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Changes the interval of {\em *this} qtTimer.
					Changes the interval of {\em *this} qtTimer to the value
					represented by the parameter {\em interval}. {\em *this} qtTimer will
					call the method {\em timer} every {\em interval} msec.
					@param       interval the new interval of {\em *this} qtTimer
					@exception   NoValidInterval thrown if the value of {\em interval} is lower or equal {\tt 0}
					@see         startTimer
					@see         stopTimer
					@see         timer
			*/
			void setInterval(int interval)
				throw(NoValidInterval);

			/** Inspection of the interval.
					Accesses the interval of {\em *this} qtTimer.
					@return      int the value of the interval of {\em *this} qtTimer
					@see         setInterval
			*/
			int getInterval() const
				throw();

			/** Starts the timer.
					Starts the timer of {\em *this} qtTimer.
					After this method is called the method {\em timer} will be called
					every {\em interval} msec.
					@see     stopTimer
					@see     timer
			*/
			void startTimer()
				throw();
			
			/** Stops the timer.
					Stops the timer of {\em *this} qtTimer.
					After this method is called the execution of the {\em timer} method
					will be stopped.
					@see     startTimer
					@see     timer
			*/
			void stopTimer()
				throw();

			//@}
			/**	@name	Predicates
			*/
			//@{

			/** Tests if {\em *this} qtTimer is running.
					@return  bool -	{\tt true} if {\em *this} qtTimer is running, {\tt false} otherwise
					@see     startTimer
					@see     stopTimer
			*/
			bool isTimerEnabled() const
				throw();

			/** Tests if {\em *this} qtTimer is stopped.
					@return  bool -	{\tt true} if {\em *this} qtTimer is stopped, {\tt false} otherwise
					@see     startTimer
					@see     stopTimer
			*/
			bool isTimerDisabled() const
				throw();

			//@}
			/**	@name	debuggers and diagnostics
			*/
			//@{

			/** Internal value dump.
					Dumps the current state of {\em *this} qtTimer to 
					the output ostream {\em s} with dumping depth {\em depth}.
					@param   s output stream where to output the state of {\em *this} qtTimer
					@param   depth the dumping depth
			*/
			virtual void dump(std::ostream& s = std::cout, Size depth = 0) const
				throw();

			//@}
			/**	@name	Storers
			*/
			//@{

			/** Persistent stream output and state restorage.
  			  Reads persistent qtTimer data from the input stream {\em s} and 
				  restores the state of {\em *this}.	 \\
				  {\bf Note:} Not yet implemented.		 
				  @param       s input stream from where to restore the internal state of {\em *this} qtTimer
			*/
			virtual void read(std::istream& s)
				throw();

			/** Persistent stream output and state storage.
  			  Writes persistent qtTimer data to the output stream {\em s} and 
				  stores the state of {\em *this}. \\
				  {\bf Note:} Not yet implemented.	 
				  @param       s output stream to where to store the internal state of {\em *this} qtTimer
			*/
			virtual void write(std::ostream& s) const
				throw();

			protected:

			//@}
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
					@see    startTimer
					@see    stopTimer
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
