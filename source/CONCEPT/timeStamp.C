// $Id: timeStamp.C,v 1.10 2000/12/10 21:19:51 amoll Exp $

#include <BALL/CONCEPT/timeStamp.h>

#include <sys/time.h>

using namespace std;

namespace BALL 
{

	void PreciseTime::write(PersistenceManager& pm) const
		throw()
  {
		pm.writePrimitive(secs_, "secs_");
		pm.writePrimitive(usecs_, "usecs_");
	}

	bool PreciseTime::read(PersistenceManager& pm)
		throw()
	{
		return (pm.readPrimitive(secs_, "secs_") && pm.readPrimitive(usecs_, "usecs_"));
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
		
		return os << buf << "." << usecs.after(".");
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
