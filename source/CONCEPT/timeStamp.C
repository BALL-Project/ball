// $Id: timeStamp.C,v 1.17.4.3 2002/12/01 21:45:23 oliver Exp $

#include <BALL/CONCEPT/timeStamp.h>

#ifdef BALL_HAS_SYS_TIME_H
#	include <sys/time.h>
#endif 
#ifdef BALL_HAS_TIME_H
#	include <time.h>
#endif 

#ifdef BALL_HAS_WINDOWS_PERFORMANCE_COUNTER
#	include <windows.h>
#	include <sys/timeb.h>
#endif

using namespace std;

namespace BALL 
{

	PreciseTime::PreciseTime()
		throw()
		: secs_(0),
			usecs_(0)
	{
	}

	PreciseTime::PreciseTime(const PreciseTime& time)
		throw()
		:	secs_(time.secs_),
			usecs_(time.usecs_)
	{
	}

	TimeStamp::TimeStamp()
		throw()
		:	time_()
	{
	}


	void PreciseTime::write(PersistenceManager& pm) const
		throw()
  {
		Size tmp = (Size)secs_;
		pm.writePrimitive(tmp, "secs_");
		tmp = (Size)usecs_;
		pm.writePrimitive(tmp, "usecs_");
	}

	bool PreciseTime::read(PersistenceManager& pm)
		throw()
	{
	  Size tmp;
		bool result = pm.readPrimitive(tmp, "secs_");
		secs_ = (long)tmp;

		result &= pm.readPrimitive(tmp, "usecs_");
		usecs_ = (long)tmp;

		return result;
	}
	
	PreciseTime PreciseTime::now() 
		throw()
	{
#ifdef BALL_COMPILER_MSVC
		struct _timeb tv;
		_ftime(&tv);
		return PreciseTime(tv.time, tv.millitm * 1000);
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
		throw()
	{
		String usecs((double)time.getMicroSeconds() / 1.0e6);
		time_t secs = (time_t)time.getSeconds();
		static char buf[128];
		strftime(buf, 127, "%Y%m%d%H%M%S", localtime(&secs));

		return os << buf << "." << (usecs.has('.') ? usecs.after(".").toString().c_str() : "0");
	}

  ostream& operator << (ostream& os, const TimeStamp& stamp)
		throw()
	{
		return os << stamp.getTime();
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/CONCEPT/timeStamp.iC>
# endif

}
