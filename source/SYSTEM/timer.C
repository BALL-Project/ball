// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: timer.C,v 1.17 2005/02/14 17:06:45 amoll Exp $

#include <BALL/SYSTEM/timer.h>

#ifdef BALL_HAS_UNISTD_H
#	include <unistd.h>
#endif
#ifdef BALL_HAS_TIME_H
#	include <time.h>
#endif
#ifdef BALL_HAS_SYS_TYPES_H
#	include <sys/types.h>
#endif
#ifdef BALL_HAS_SYS_TIMES_H
#	include <sys/times.h>
#endif
#ifdef BALL_HAS_SYS_TIME_H
#	include <sys/time.h>
#endif

#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
#	include <windows.h>
#	include <sys/timeb.h>
#endif


using std::cout;
using std::endl;
using std::ostream;

namespace BALL 
{

	LongSize Timer::cpu_speed_ = 0L;

	#ifdef BALL_COMPILER_MSVC
		PointerSizeUInt Timer::clock_speed_ = 0L;
	#endif

	Timer::Timer()
		:	is_running_(false),
			last_secs_(0),
			last_usecs_(0),
			last_user_time_(0),
			last_system_time_(0),
			current_secs_(0),
			current_usecs_(0),
			current_user_time_(0),
			current_system_time_(0)
	{
		#ifdef BALL_HAS_SYSCONF
		if (cpu_speed_ == 0L)
		{
			cpu_speed_ = sysconf(_SC_CLK_TCK);
		}
		#endif
	
		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			if (cpu_speed_ == 0L)
			{
				LARGE_INTEGER ticks;
				if (QueryPerformanceFrequency(&ticks))
				{
					cpu_speed_ = (PointerSizeUInt) ticks.QuadPart;
				}
				else 
				{
					cpu_speed_ = 1L;
				}
				clock_speed_ = CLOCKS_PER_SEC;
			}
		#endif
	}

	Timer::Timer(Timer& timer)
		:	is_running_(timer.is_running_),
			last_secs_(timer.last_secs_),
			last_usecs_(timer.last_usecs_),
			last_user_time_(timer.last_user_time_),
			last_system_time_(timer.last_system_time_),
			current_secs_(timer.current_secs_),
			current_usecs_(timer.current_usecs_),
			current_user_time_(timer.current_user_time_),
			current_system_time_(timer.current_system_time_)
	{
	}

	Timer::~Timer()
	{
	}

	void Timer::clear()
	{
		is_running_ = false;
		current_secs_ = 0L;
		current_usecs_ = 0L;
		current_user_time_ = 0L;
		current_system_time_ = (clock_t)0;
	}

	bool Timer::start()
	{
		if (is_running_ == true)
		{ 
			/* tried to start a running timer */
			return false;
		}

		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			LARGE_INTEGER tms;
			FILETIME kt,ut,ct,et;
			
			QueryPerformanceCounter(&tms);
			HANDLE my_id = GetCurrentProcess();
			GetProcessTimes(my_id, &ct, &et, &kt, &ut);
			ULARGE_INTEGER kernel_time; 
			kernel_time.HighPart = kt.dwHighDateTime;
			kernel_time.LowPart = kt.dwLowDateTime;
			ULARGE_INTEGER user_time; 
			user_time.HighPart = ut.dwHighDateTime;
			user_time.LowPart = ut.dwLowDateTime;

			last_secs_  = tms.QuadPart / cpu_speed_;			
			last_usecs_ = (PointerSizeUInt)((double)(tms.QuadPart - (last_secs_*cpu_speed_)) / (double)(cpu_speed_) * 1000000.0);

			last_user_time_ = user_time.QuadPart / 10;
			last_system_time_ = kernel_time.QuadPart / 10;

		#else

			struct tms tms_buffer; 
			struct timeval timeval_buffer;	
			struct timezone timezone_buffer; 

			gettimeofday(&timeval_buffer, &timezone_buffer);
			times(&tms_buffer);
		
			last_secs_ = timeval_buffer.tv_sec;
			last_usecs_ = timeval_buffer.tv_usec;
			last_user_time_ = tms_buffer.tms_utime;
			last_system_time_ = tms_buffer.tms_stime;
		#endif

		is_running_ = true;

		return true;
	}

	bool Timer::stop()
	{
		if (is_running_ == false)
		{ /* tried to stop a stopped timer */
			return false;
		}
		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			LARGE_INTEGER tms;
	
			QueryPerformanceCounter(&tms);
			FILETIME kt,ut,ct,et;
			
			HANDLE my_id=GetCurrentProcess();
			GetProcessTimes(my_id,&ct,&et,&kt, &ut);
			
			ULARGE_INTEGER kernel_time; 
			kernel_time.HighPart = kt.dwHighDateTime;
			kernel_time.LowPart = kt.dwLowDateTime;
			ULARGE_INTEGER user_time; 
			user_time.HighPart = ut.dwHighDateTime;
			user_time.LowPart = ut.dwLowDateTime;

			PointerSizeUInt secs_to_add = tms.QuadPart/cpu_speed_;
			current_secs_ += secs_to_add - last_secs_;
			PointerSizeUInt usecs_to_add = (PointerSizeUInt)((double)(tms.QuadPart - secs_to_add*cpu_speed_) /(double)(cpu_speed_) * 1000000.0);
			current_usecs_ += usecs_to_add - last_usecs_;
			
			current_user_time_ += user_time.QuadPart / 10 - last_user_time_;
			current_system_time_ += kernel_time.QuadPart / 10 - last_system_time_;
		#else
			struct tms tms_buffer;
			struct timeval timeval_buffer;
			struct timezone timezone_buffer;

			gettimeofday(&timeval_buffer, &timezone_buffer);
			times(&tms_buffer);

			current_secs_ += timeval_buffer.tv_sec - last_secs_;
			current_usecs_ += timeval_buffer.tv_usec - last_usecs_;

			current_user_time_ += tms_buffer.tms_utime - last_user_time_;
			current_system_time_ += tms_buffer.tms_stime - last_system_time_;
		#endif
		
		is_running_ = false;

		return true;
	}

	void Timer::reset()
	{
		if (is_running_ == false)
		{
			clear();
		} 
		else 
		{
			stop();
			clear();
			start();
		}
	}

	/************************************************************************/
	/*																																			*/
	/*  getClockTime returns the current amount of real (clock) time			*/
	/*  accumulated by this timer.  If the timer is stopped, this is just		*/
	/*  the total accumulated time.  If the timer is running, this is the		*/
	/*  accumulated time + the time since the timer was last started.				*/
	/*																																			*/
	/************************************************************************/
	double Timer::getClockTime() const
	{
		LongIndex elapsed_seconds;
		LongIndex elapsed_useconds;

		if (is_running_ == false)
		{ 
			/* timer is currently off, so just return accumulated time */
			elapsed_seconds = current_secs_;
			elapsed_useconds = current_usecs_;
		} 
		else 
		{ 
			/* timer is currently running, so add the elapsed time since */
			/* the timer was last started to the accumulated time        */
			#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
				LARGE_INTEGER tms;
				if (QueryPerformanceCounter(&tms))
				{
					PointerSizeUInt secs_to_add = tms.QuadPart / cpu_speed_;
					elapsed_seconds = current_secs_ + secs_to_add - last_secs_;
					PointerSizeUInt usecs_to_add = (PointerSizeUInt)((double)(tms.QuadPart - secs_to_add * cpu_speed_) /(double)(cpu_speed_) * 1000000.0);
					elapsed_useconds  = current_usecs_ + usecs_to_add - last_usecs_;
				}
			#else
				struct timeval timeval_buffer;
				struct timezone timezone_buffer;

				gettimeofday(&timeval_buffer, &timezone_buffer);

				elapsed_seconds = current_secs_ + timeval_buffer.tv_sec - last_secs_;
				elapsed_useconds = current_usecs_ + timeval_buffer.tv_usec - last_usecs_;
			#endif
		}

		/* Adjust for the fact that the useconds may be negative. */
		/* If they are, take away 1 second and add 1 million      */
		/* microseconds until they are positive.                  */
		while (elapsed_useconds < 0L)
		{
			elapsed_useconds += 1000000L;
			elapsed_seconds--;
		}

		/* convert into floating point number of seconds */
		return (double)((double)elapsed_seconds + (double)elapsed_useconds / 1000000.0);
	}

	/************************************************************************/
	/*																																			*/
	/*   getUserTime reports the current amount of user cpu time						*/
	/*   accumulated by this Timer.  If the timer is currently off,					*/
	/*   this is just the accumulated time.  If the Timer is running, this	*/
	/*   is the accumulated time plust the time since the timer was last    */
	/*   started.																														*/
	/*																																			*/
	/************************************************************************/
	double Timer::getUserTime() const
	{
		double temp_value;

		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			FILETIME kt,ut,ct,et;
		#else
			struct tms tms_buffer;	
		#endif
		if (is_running_ == false)
		{ 
			/* timer is off, just return accumulated time */
			temp_value = (double)current_user_time_;
		}	
		else 
		{
			/* timer is on, add current running time to accumulated time */
			#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
				HANDLE my_id=GetCurrentProcess();
				GetProcessTimes(my_id,&ct,&et,&kt,&ut);
				
				ULARGE_INTEGER kernel_time; 
				kernel_time.HighPart = kt.dwHighDateTime;
				kernel_time.LowPart = kt.dwLowDateTime;
				ULARGE_INTEGER user_time; 
				user_time.HighPart = ut.dwHighDateTime;
				user_time.LowPart = ut.dwLowDateTime;
				
				temp_value = (double)(current_user_time_ + user_time.QuadPart / 10.0 - last_user_time_);
			#else
				times(&tms_buffer);
				temp_value = (double)(current_user_time_ + tms_buffer.tms_utime - last_user_time_);
			#endif
		}

		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			return (double)(temp_value / 1000000.0);
		#else		
			/* convert from clock ticks to seconds using the */
			/* cpu-speed value obtained in the constructor   */
			return (double)(temp_value / (double)cpu_speed_);
		#endif	
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
	double Timer::getSystemTime() const
	{
		double temp_value = 0.0;

		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			//struct tms tms_buffer;
			FILETIME kt,ut,ct,et;
		#endif												
		
		if (is_running_ == false)
		{ 
			/* timer is off, just return accumulated time */
			temp_value = (double)current_system_time_;
		} 
		else 
		{ 
			/* timer is on, return accumulated plus current */
			#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
				//times(&tms_buffer);
				HANDLE my_id=GetCurrentProcess();
				GetProcessTimes(my_id,&ct,&et,&kt,&ut);
			
				ULARGE_INTEGER kernel_time; 
				kernel_time.HighPart = kt.dwHighDateTime;
				kernel_time.LowPart = kt.dwLowDateTime;
				ULARGE_INTEGER user_time; 
				user_time.HighPart = ut.dwHighDateTime;
				user_time.LowPart = ut.dwLowDateTime;
				temp_value = (double)((double)current_system_time_ + kernel_time.QuadPart / 10.0 - (double)last_system_time_);
			#endif
		}

		/* convert from clock ticks to seconds using the */
		/* cpu-speed value obtained by the constructor   */
		#ifndef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			return (double)(temp_value / 1000000.0);
		#else 
			return 0.0;
		#endif
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
		return (last_secs_ == timer.last_secs_
									&& last_usecs_ == timer.last_usecs_
									&& last_user_time_ == timer.last_user_time_
									&& last_system_time_ == timer.last_system_time_
									&& current_secs_ == timer.current_secs_
									&& current_usecs_ == timer.current_usecs_
									&& current_user_time_ == timer.current_user_time_
									&& current_system_time_ == timer.current_system_time_);
	}

	void Timer::dump(ostream& s, Size depth) const
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
