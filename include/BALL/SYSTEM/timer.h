// $Id: timer.h,v 1.1 1999/08/26 07:53:20 oliver Exp $

#ifndef BALL_SYSTEM_TIMER_H
#define BALL_SYSTEM_TIMER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <sys/time.h>
#include <iostream.h>

namespace BALL {

	/**	Timer Class
	*/
	class Timer
	{
		public:
		
		/**	@name	Constructors and Destructors 
		*/
		//@{

		/**	Default constructor.
				Create a new timer. The timer is stopped.
		*/
		Timer();			        

		/**	Copy constructor.
				Create a new timer from an existing timer.
		*/
		Timer(Timer& timer);			        

		/**	Destructor.
				Destructs a timer object.
		*/
		virtual ~Timer();			        
		//@}

		/**	Starting, Stopping and Resetting the timer 
		*/
		//@{

		/**	Clear and stop the timer.
				This sets the timer to zero and stops it when running.
				@see	reset
		*/
		void clear();			
	
		/** Start the timer.
				The timer is started. If the timer is already running, {\bf false}
				is returned.
				@return bool {\bf false} if the timer was already running, {\bf true} otherwise
		*/
		bool start();			

		/** Stop the timer.
				The timer is stopped. If the timer was not running, {\bf false}
				is returned.
				@return bool {\bf false} if the was not running, {\bf true} otherwise
		*/
		bool stop();

		/** Clear the timer without stopping.
				The timer is cleared, but not stopped (if running).
				@see clear
		*/
		void reset();

		//@}
			
		/**	@name Readout of the Timer 
		*/
		//@{

		/**	Get clock time.
				Return the accumulated clock (real) time in seconds.
		*/
		float getClockTime() const;

		/**	Get user time.
				Return the accumulated user time in seconds.
		*/
		float getUserTime() const;		

		/**	Get user time.
				Return the accumulated system time in seconds.
		*/
		float getSystemTime() const;

		/**	Get CPU time.
				Return the accumulated CPU time in seconds.
				CPU time is the sum of user time and system time.
		*/
		float getCPUTime() const;
		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns a timer from another. The two timer will then run 
				synchronously.
				@return {\tt *this}
		*/
		Timer& operator = (const Timer& timer);
		//@}

		/**	@name	Predicates 
		*/
		//@{

		/**	Return true if the timer is running.
				@return {\bf true} if the timer is running, {\bf false} otherwise
		*/
		bool isRunning() const;

		/**	Equality operator.
				Return {\bf true} if two timers are equal, i.e. they contain exactly 
				the same time intervals for clock, user, and system time and have the
				same running status.
				@param timer the timer to compare with
				@return bool {\bf true} on equality, {\bf false} otherwise
		*/
		bool operator == (const Timer& timer) const;

		/**	Inequality operator.
				Return {\bf false} if two timers differ in any way, i.e. they differ
				in either the clock, user, or system time or have a different 
				running status.
				@param timer the timer to compare with
				@return bool {\bf true} on inequality, {\bf false} otherwise
		*/
		bool operator != (const Timer& timer) const;

		/**	Lesser than operator.
				Return true, if the timer is in all timings lesser than the
				timer to be compared with (clock, user, and system time).
				@param timer the timer to compare with
				@return bool {\bf true} if all times are lesser
		*/
		bool operator < (const Timer& timer) const;

		/**	Lesser or equal operator.
				Return true, if the timer is in all timings lesser or equal than the
				timer to be compared with (clock, user, and system time).
				@param timer the timer to compare with
				@return bool {\bf true} if all times are lesser or equal
		*/
		bool operator <= (const Timer& timer) const;

		/**	Greater or equal operator.
				Return true, if the timer is in all timings greater or equal than the
				timer to be compared with (clock, user, and system time).
				@param timer the timer to compare with
				@return bool {\bf true} if all times are greater or equal
		*/
		bool operator >= (const Timer& timer) const;

		/**	Greater operator.
				Return true, if the timer is in all timings greater than the
				timer to be compared with (clock, user, and system time).
				@param timer the timer to compare with
				@return bool {\bf true} if all times are greater 
		*/
		bool operator > (const Timer& timer) const;
		//@}

		/**	@name	Debugging and Diagnostics 
		*/
		//@{
		
		/**	
		*/
		bool isValid() const;
		
		/**	Dump the timer's contents.
		*/
		void dump(ostream& s = cout, unsigned long depth = 0L) const;
		//@}

		protected:

		private:

		/* CPU speed for times() call */
		static long cpu_speed_;

		/* state of timer, either true(on) or false(off) */
		bool is_running_;

		/* clock seconds value when the timer was last started */
		long last_secs_;	

		/* clock useconds value when the timer was last started */
		long last_usecs_;		

		/* user time when the timer was last started */
		clock_t last_user_time_;   

		/* system time when the timer was last started */
		clock_t last_system_time_; 
		 
		/* current accumulated clock seconds */
		long current_secs_;		

		/* current accumulated clock useconds */
		long current_usecs_;		
		
		/* current accumulated user time */
		clock_t current_user_time_;		

		/* current accumulated user time */
		clock_t current_system_time_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include "timer.iC"
#	endif

} // namespace BALL

#endif // BALL_SYSTEM_TIMER_H
