// $Id: timeStamp.C,v 1.17 2002/01/04 19:52:08 oliver Exp $

#include <BALL/CONCEPT/timeStamp.h>

#include <sys/time.h>

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
		// get the current time via the system call
		// gettimeofday()
		struct timeval tv;
		gettimeofday(&tv, 0);

		return PreciseTime(tv.tv_sec, tv.tv_usec);
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
