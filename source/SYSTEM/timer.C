// $Id: timer.C,v 1.1 1999/08/26 08:02:36 oliver Exp $

#include <BALL/SYSTEM/timer.h>

#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <sys/times.h>
#include <sys/time.h>

namespace BALL {

	long Timer::cpu_speed_ = 0L;

	Timer::Timer()
	{
		if (cpu_speed_ == 0L)
		{
			cpu_speed_ = sysconf(_SC_CLK_TCK);
		}

		clear();
	}

	Timer::Timer(Timer& timer)
	{
		*this = timer;
	}

	Timer::~Timer()
	{
	}

	void Timer::clear()
	{
		is_running_ = false;
		current_secs_ = 
		current_usecs_ = 0L;
		current_user_time_ = 
		current_system_time_ = (clock_t)0;
	}

	bool Timer::start()
	{
		if (is_running_ == true)
		{ /* tried to start a running timer */
			return false;
		}
		
		struct tms tms_buffer; 
		struct timeval timeval_buffer;	
		struct timezone timezone_buffer; 

		gettimeofday(&timeval_buffer, &timezone_buffer);
		times(&tms_buffer);
		
		last_secs_ = timeval_buffer.tv_sec;
		last_usecs_ = timeval_buffer.tv_usec;
		last_user_time_ = tms_buffer.tms_utime;
		last_system_time_ = tms_buffer.tms_stime;
		is_running_ = true;

		return true;
	}

	bool Timer::stop()
	{
		if (is_running_ == false)
		{ /* tried to stop a stopped timer */
			return false;
		}

		struct tms tms_buffer;
		struct timeval timeval_buffer;
		struct timezone timezone_buffer;

		gettimeofday(&timeval_buffer, &timezone_buffer);
		times(&tms_buffer);

		current_secs_ += timeval_buffer.tv_sec - last_secs_;
		current_usecs_ += timeval_buffer.tv_usec - last_usecs_;
		current_user_time_ += tms_buffer.tms_utime - last_user_time_;
		current_system_time_ += tms_buffer.tms_stime - last_system_time_;
		is_running_ = false;

		return true;
	}

	void Timer::reset()
	{
		if (is_running_ == false)
		{
			clear();
		} else {
			stop();
			clear();
			start();
		}
	}

	/************************************************************************/
	/*																																			*/
	/*     getClockTime returns the current amount of real (clock) time			*/
	/*  accumulated by this timer.  If the timer is stopped, this is just		*/
	/*  the total accumulated time.  If the timer is running, this is the		*/
	/*  accumulated time + the time since the timer was last started.				*/
	/*																																			*/
	/************************************************************************/
	float Timer::getClockTime() const
	{
		struct timeval timeval_buffer;
		struct timezone timezone_buffer;
		long elapsed_seconds;
		long micro_useconds;

		if (is_running_ == false)
		{ /* timer is currently off, so just return accumulated time */
			elapsed_seconds = current_secs_;
			micro_useconds = current_usecs_;
		} else { 
			/* timer is currently running, so add the elapsed time since */
			/* the timer was last started to the accumulated time        */

			gettimeofday(&timeval_buffer, &timezone_buffer);

			elapsed_seconds = current_secs_ + timeval_buffer.tv_sec - last_secs_;
			micro_useconds = current_usecs_ + timeval_buffer.tv_usec - last_usecs_;
		}

		/* Adjust for the fact that the useconds may be negative. */
		/* If they are, take away 1 second and add 1 million      */
		/* microseconds until they are positive.                  */
		while (micro_useconds < 0L)
		{
			micro_useconds += 1000000L;
			elapsed_seconds--;
		}

		/* convert into floating point number of seconds */
		return (float)((float)elapsed_seconds + (float) micro_useconds / 1000000.0);
	}

	/************************************************************************/
	/*																																			*/
	/*      getUserTime reports the current amount of user cpu time         */
	/*   accumulated by this Timer.  If the timer is currently off,					*/
	/*   this is just the accumulated time.  If the Timer is running, this	*/
	/*   is the accumulated time plust the time since the timer was last    */
	/*   started.																														*/
	/*																																			*/
	/************************************************************************/
	float Timer::getUserTime() const
	{
		float temp_value;
		struct tms tms_buffer;	

		if (is_running_ == false)
		{ /* timer is off, just return accumulated time */
			temp_value = (float)current_user_time_;
		}	else { 
			/* timer is on, add current running time to accumulated time */
			times(&tms_buffer);

			temp_value = (float)(current_user_time_ + tms_buffer.tms_utime - last_user_time_);
		}

		/* convert from clock ticks to seconds using the */
		/* cpu-speed value obtained by the constructor   */
		return (float)(temp_value / (float)cpu_speed_);
	}

	/************************************************************************/
	/*																																			*/
	/*      system_time reports the current amount of system cpu time       */
	/*   accumulated by this Timer.  If the timer is currently off,					*/
	/*   this is just the accumulated time.  If the Timer is running, this	*/
	/*   is the accumulated time plus  the time since the timer was last    */
	/*   started.																														*/
	/*																																			*/
	/************************************************************************/
	float Timer::getSystemTime() const
	{
		float temp_value;
		struct tms tms_buffer;
												
		if (is_running_ == false)
		{ /* timer is off, just return accumulated time */
			temp_value = (float)current_system_time_;
		} else { 
			/* timer is on, return accumulated plus current */
			times(&tms_buffer);

			temp_value = (float)(current_system_time_ + tms_buffer.tms_stime - last_system_time_);
		}

		/* convert from clock ticks to seconds using the */
		/* cpu-speed value obtained by the constructor   */
		return (temp_value / (float)cpu_speed_);
	}

	Timer& Timer::operator = (const Timer& timer)
	{
		if (this == &timer)
		{
			return *this;
		}

		is_running_ = timer.is_running_;
		last_secs_ = timer.last_secs_;	
		last_usecs_ = timer.last_usecs_;		
		last_user_time_ = timer.last_user_time_;   
		last_system_time_ = timer.last_system_time_; 
		current_secs_ = timer.current_secs_;		
		current_usecs_ = timer.current_usecs_;		
		current_user_time_ = timer.current_user_time_;		
		current_system_time_ = timer.current_system_time_;

		return *this;
	}

	bool Timer::operator == (const Timer& timer) const
	{
		return (bool)(last_secs_ == timer.last_secs_
									&& last_usecs_ == timer.last_usecs_
									&& last_user_time_ == timer.last_user_time_
									&& last_system_time_ == timer.last_system_time_
									&& current_secs_ == timer.current_secs_
									&& current_usecs_ == timer.current_usecs_
									&& current_user_time_ == timer.current_user_time_
									&& current_system_time_ == timer.current_system_time_);
	}

	bool Timer::operator < (const Timer& timer) const
	{
		return (bool)(current_secs_ + last_secs_ < timer.current_secs_ + timer.last_secs_
									&& current_usecs_ + last_usecs_ < timer.current_usecs_ + timer.last_usecs_
									&& current_user_time_ + last_user_time_ < timer.current_user_time_ + timer.last_user_time_
									&& current_system_time_ + last_system_time_ < timer.current_system_time_ + timer.last_system_time_);
	}

	void Timer::dump(ostream& s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);

		BALL_DUMP_DEPTH(s, depth);
		BALL_DUMP_CLASS_HEADER(s, Timer, this);

		BALL_DUMP_DEPTH(s, depth);
		s << "CPU speed: " << cpu_speed_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "is running: " << (is_running_ ? "true" : "false") << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "last clock seconds: " << last_secs_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "last user seconds: " << last_usecs_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "last user time: " << last_user_time_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "last system time: " << last_system_time_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "current clock seconds: " << current_secs_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "current user seconds: " << current_usecs_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "current user time: " << current_user_time_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "current system time: " << current_system_time_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "effective clock time: " << getClockTime() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "effective user time: " << getUserTime() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "effective system time: " << getSystemTime() << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "effective CPU time: " << getCPUTime() << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/SYSTEM/timer.iC>
#	endif

} // namespace BALL
