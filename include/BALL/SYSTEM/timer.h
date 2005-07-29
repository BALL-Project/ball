// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: timer.h,v 1.17 2005/07/29 12:38:15 amoll Exp $
//

#ifndef BALL_SYSTEM_TIMER_H
#define BALL_SYSTEM_TIMER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifdef BALL_HAS_SYS_TIME_H
#	include <sys/time.h>
#endif
#ifdef BALL_HAS_TIME_H
#	include <time.h>
#endif



#include <iostream>

namespace BALL
{
	/**	Timer Class
	\ingroup System
	*/
	class BALL_EXPORT Timer
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
				The timer is started. If the timer is already running, <b>false</b>
				is returned.
				@return bool <b>false</b> if the timer was already running, <b>true</b> otherwise
		*/
		bool start();			

		/** Stop the timer.
				The timer is stopped. If the timer was not running, <b>false</b>
				is returned.
				@return bool <b>false</b> if the was not running, <b>true</b> otherwise
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
		double getClockTime() const;

		/**	Get user time.
				Return the accumulated user time in seconds.
		*/
		double getUserTime() const;		

		/**	Get user time.
				Return the accumulated system time in seconds.
		*/
		double getSystemTime() const;

		/**	Get CPU time.
				Return the accumulated CPU time in seconds.
				CPU time is the sum of user time and system time.
		*/
		double getCPUTime() const;

		//@}

		/**	@name	Assignment
		*/
		//@{

		/**	Assignment operator.
				Assigns a timer from another. The two timer will then run 
				synchronously.
				@return Timer <tt>*this</tt>
		*/
		Timer& operator = (const Timer& timer);

		//@}

		/**	@name	Predicates 
		*/
		//@{

		/**	Return true if the timer is running.
				@return bool <b>true</b> if the timer is running, <b>false</b> otherwise
		*/
		bool isRunning() const;

		/**	Equality operator.
				Return <b>true</b> if two timers are equal, i.e. they contain exactly 
				the same time intervals for clock, user and system time and have the
				same running status.
				@param timer the timer to compare with
				@return bool <b>true</b> on equality, <b>false</b> otherwise
		*/
		bool operator == (const Timer& timer) const;

		/**	Inequality operator.
				Return <b>false</b> if two timers differ in any way, i.e. they differ
				in either the clock, user, or system time or have a different 
				running status.
				@param timer the timer to compare with
				@return bool <b>true</b> on inequality, <b>false</b> otherwise
		*/
		bool operator != (const Timer& timer) const;

		/**	Lesser than operator.
				Return true, if the timer is in all timings lesser than the
				timer to be compared with (clock, user and system time).
				@param timer the timer to compare with
				@return bool <b>true</b> if all times are lesser
		*/
		bool operator < (const Timer& timer) const;

		/**	Lesser or equal operator.
				Return true, if the timer is in all timings lesser or equal than the
				timer to be compared with (clock, user and system time).
				@param timer the timer to compare with
				@return bool <b>true</b> if all times are lesser or equal
		*/
		bool operator <= (const Timer& timer) const;

		/**	Greater or equal operator.
				Return true, if the timer is in all timings greater or equal than the
				timer to be compared with (clock, user and system time).
				@param timer the timer to compare with
				@return bool <b>true</b> if all times are greater or equal
		*/
		bool operator >= (const Timer& timer) const;

		/**	Greater operator.
				Return true, if the timer is in all timings greater than the
				timer to be compared with (clock, user and system time).
				@param timer the timer to compare with
				@return bool <b>true</b> if all times are greater 
		*/
		bool operator > (const Timer& timer) const;

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{
		
		/**	Test if the instance is valid.
				Allways returns true.
		*/
		bool isValid() const;
		
		/**	Dump the timer's contents.
		*/
		void dump(::std::ostream& s = ::std::cout, Size depth = 0L) const;

		//@}

		protected:

		private:

		static LongSize cpu_speed_;

		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			static PointerSizeUInt clock_speed_;
		#endif

		// state of timer, either true(on) or false(off) 
		bool is_running_;

		// clock seconds value when the timer was last started 
		LongSize last_secs_;	

		// clock useconds value when the timer was last started 
		LongSize last_usecs_;		

		// user time when the timer was last started 
		clock_t last_user_time_;   

		// system time when the timer was last started 
		clock_t last_system_time_; 
		 
		// current accumulated clock seconds 
		LongSize current_secs_;		

		// current accumulated clock useconds 
		LongSize current_usecs_;		
		
		// current accumulated user time 
		clock_t current_user_time_;		

		// current accumulated user time 
		clock_t current_system_time_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include "timer.iC"
#	endif
  
}

#endif // BALL_SYSTEM_TIMER_H
