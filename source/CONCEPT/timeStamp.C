// $Id: timeStamp.C,v 1.5 2000/10/16 20:00:50 oliver Exp $

#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/CONCEPT/persistenceManager.h>

#include <sys/time.h>

using namespace std;

namespace BALL 
{
	Time::Time()
		: secs_(0),
			usecs_(0)
	{
	}

	Time::Time(const Time& time)
		:	secs_(time.secs_),
		 	usecs_(time.usecs_)
	{
	}

	Time::~Time()
	{
	}

	void Time::set(const Time& time) throw()
	{
		secs_ = time.secs_;
		usecs_ = time.usecs_;
	}

	void Time::set(long secs, long usecs) throw()
	{
		secs_ = secs;
		usecs_ = usecs;
	}

	const Time& Time::operator = (const Time& time) throw()
	{
		set(time);
		return *this;
	}

	void Time::clear() 
		throw()
	{
		secs_ = 0;
		usecs_ = 0;
	}

	bool Time::operator < (const Time& time) const throw()
	{
		return ((secs_ < time.secs_) || ((secs_ == time.secs_) && (usecs_ < time.usecs_)));
	}

	bool Time::operator > (const Time& time) const throw()
	{
		return ((secs_ > time.secs_) || ((secs_ == time.secs_) && (usecs_ > time.usecs_)));
	}

	bool Time::operator == (const Time& time) const throw()
	{
		return ((secs_ == time.secs_) && (usecs_ == time.usecs_));
	}


	void Time::write(PersistenceManager& pm) const
  {
		pm.writePrimitive(secs_, "secs_");
		pm.writePrimitive(usecs_, "usecs_");
	}

	bool Time::read(PersistenceManager& pm)
	{
		return (pm.readPrimitive(secs_, "secs_") && pm.readPrimitive(usecs_, "usecs_"));
	}
	
	const Time& Time::now() throw()
	{
		static Time t;

		// get the current time via the system call
		// gettimeofday()
		struct timeval tv;
		gettimeofday(&tv, 0);

		// copy the struct to the members of time
		t.secs_ = tv.tv_sec;
		t.usecs_ = tv.tv_usec;

		return t;
	}

	const Time Time::ZERO;

 
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

	void TimeStamp::stamp(const Time& time) 
		throw ()
	{
		// in the default case, stamp with the current 
		// time
		if (time == Time::ZERO)
		{
			time_ = Time::now();
		}
		else 
		{
			time_ = time;
		}
	}

	

	const Time& TimeStamp::getTime() const throw()
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



  ostream& operator << (ostream& os, const Time& time)
	{
		String usecs(time.getMicroSeconds() / 1.0e6);
		time_t secs = time.getSeconds();
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
