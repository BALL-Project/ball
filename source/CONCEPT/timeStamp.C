// $Id: timeStamp.C,v 1.8 2000/11/02 18:22:36 oliver Exp $

#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/CONCEPT/persistenceManager.h>

#include <sys/time.h>

using namespace std;

namespace BALL 
{
	PreciseTime::PreciseTime()
		: secs_(0),
			usecs_(0)
	{
	}

	PreciseTime::PreciseTime(const PreciseTime& time)
		:	secs_(time.secs_),
		 	usecs_(time.usecs_)
	{
	}

	PreciseTime::PreciseTime(long secs, long usecs)
		:	secs_(secs),
		 	usecs_(usecs)
	{
	}

	PreciseTime::~PreciseTime()
	{
	}

	void PreciseTime::set(const PreciseTime& time) throw()
	{
		secs_ = time.secs_;
		usecs_ = time.usecs_;
	}

	void PreciseTime::set(long secs, long usecs) throw()
	{
		secs_ = secs;
		usecs_ = usecs;
	}

	const PreciseTime& PreciseTime::operator = (const PreciseTime& time) throw()
	{
		set(time);
		return *this;
	}

	void PreciseTime::clear() 
		throw()
	{
		secs_ = 0;
		usecs_ = 0;
	}

	bool PreciseTime::operator < (const PreciseTime& time) const throw()
	{
		return ((secs_ < time.secs_) || ((secs_ == time.secs_) && (usecs_ < time.usecs_)));
	}

	bool PreciseTime::operator > (const PreciseTime& time) const throw()
	{
		return ((secs_ > time.secs_) || ((secs_ == time.secs_) && (usecs_ > time.usecs_)));
	}

	bool PreciseTime::operator == (const PreciseTime& time) const throw()
	{
		return ((secs_ == time.secs_) && (usecs_ == time.usecs_));
	}


	void PreciseTime::write(PersistenceManager& pm) const
  {
		pm.writePrimitive(secs_, "secs_");
		pm.writePrimitive(usecs_, "usecs_");
	}

	bool PreciseTime::read(PersistenceManager& pm)
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

 
	TimeStamp::TimeStamp()
		:	time_()
	{
	}

	TimeStamp::~TimeStamp()
	{
		clear();
	}

	void TimeStamp::clear()
		throw()
	{
		time_.clear();
	}

	void TimeStamp::stamp(const PreciseTime& time) 
		throw ()
	{
		// in the default case, stamp with the current 
		// time
		if (time == PreciseTime::ZERO)
		{
			time_ = PreciseTime::now();
		}
		else 
		{
			time_ = time;
		}
	}

	

	const PreciseTime& TimeStamp::getTime() const throw()
	{
		return time_;
	}


	void TimeStamp::write(PersistenceManager& pm) const
  {
		pm.writeStorableObject(time_, "time_");
	}

	bool TimeStamp::read(PersistenceManager& pm)
	{
		return pm.readStorableObject(time_, "time_");
	}



  ostream& operator << (ostream& os, const PreciseTime& time)
	{
		String usecs((double)time.getMicroSeconds() / 1.0e6);
		time_t secs = (time_t)time.getSeconds();
		static char buf[128];
		strftime(buf, 127, "%Y%m%d%H%M%S", localtime(&secs));
		
		return os << buf << "." << usecs.after(".");
	}

  ostream& operator << (ostream& os, const TimeStamp& stamp)
	{
		return os << stamp.getTime();
	}

# ifdef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/CONCEPT/timeStamp.iC>
# endif

}
