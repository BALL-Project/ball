// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: timeStamp.C,v 1.21 2005/12/23 17:02:33 amoll Exp $

#include <BALL/CONCEPT/timeStamp.h>

#ifdef BALL_HAS_SYS_TIME_H
#	include <sys/time.h>
#endif 
#ifdef BALL_HAS_TIME_H
#	include <time.h>
#endif 

#ifdef BALL_COMPILER_MSVC
 #ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
  #include <windows.h>
 #else
	#include <time.h>
 #endif
 #include <sys/timeb.h>
#endif

using namespace std;

namespace BALL 
{

	PreciseTime::PreciseTime()
		
		: secs_(0),
			usecs_(0)
	{
		#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
			LARGE_INTEGER t;
			QueryPerformanceFrequency(&t);
			ticks_ = (long)t.QuadPart;
		#endif
	}

	PreciseTime::PreciseTime(const PreciseTime& time)
		
		:	secs_(time.secs_),
			usecs_(time.usecs_)
	{
	}

#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
	long PreciseTime::ticks_;
#endif

	TimeStamp::TimeStamp()
		
		:	time_()
	{
	}


	void PreciseTime::write(PersistenceManager& pm) const
		
  {
		Size tmp = (Size)secs_;
		pm.writePrimitive(tmp, "secs_");
		tmp = (Size)usecs_;
		pm.writePrimitive(tmp, "usecs_");
	}

	bool PreciseTime::read(PersistenceManager& pm)
		
	{
	  Size tmp;
		bool result = pm.readPrimitive(tmp, "secs_");
		secs_ = (long)tmp;

		result &= pm.readPrimitive(tmp, "usecs_");
		usecs_ = (long)tmp;

		return result;
	}
	
	PreciseTime PreciseTime::now() 
		
	{
		#ifdef BALL_COMPILER_MSVC
			#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
				LARGE_INTEGER tvl;
				QueryPerformanceCounter(&tvl);
				long sec = tvl.QuadPart / ticks_;
				long usec = (tvl.QuadPart - sec * ticks_) * 1000000 / ticks_;
				return PreciseTime(sec, usec);
			#else
				::_timeb tv;
				_ftime(&tv);
				return PreciseTime(tv.time, tv.millitm * 1000);
			#endif
		#else
			// get the current time via the system call
			// gettimeofday()
			struct timeval tv;
			gettimeofday(&tv, 0);
			return PreciseTime(tv.tv_sec, tv.tv_usec);
		#endif
	}

	const PreciseTime PreciseTime::ZERO;

  ostream& operator << (ostream& os, const PreciseTime& time)
		
	{
		String usecs((double)time.getMicroSeconds() / 1.0e6);
		time_t secs = (time_t)time.getSeconds();
		static char buf[128];
		strftime(buf, 127, "%Y%m%d%H%M%S", localtime(&secs));

		return os << buf << "." << (usecs.has('.') ? usecs.after(".").toString().c_str() : "0");
	}

  ostream& operator << (ostream& os, const TimeStamp& stamp)
		
	{
		return os << stamp.getTime();
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/CONCEPT/timeStamp.iC>
# endif

}
