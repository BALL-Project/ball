// $Id: timeStamp.sip,v 1.1 2000/08/30 19:59:12 oliver Exp $

class Time
{
%HeaderCode
	#include <time.h>
	#include <BALL/CONCEPT/timeStamp.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Time)
  Time();
  Time(const Time& /time/);
  ~Time();
  static const Time ZERO;
  void set(long /secs/, long /usecs/) throw();
  void set(const Time& /time/) throw();
  // const Time& operator = (const Time& /time/) throw();
	//  bool operator < (const Time& /time/) const throw();
	//  bool operator > (const Time& /time/) const throw();
 	// bool operator == (const Time& /time/) const throw();
  long getSeconds() const throw();
  long getMicroSeconds() const throw();
  static const Time& now() throw();
  //  void write(PersistenceManager& pm)const;
  //  bool read(PersistenceManager& pm);
  operator const char* ();
%MemberCode
  Time* ptr;
  if ((ptr = (Time*)sipGetCppPtr(sipThis,sipClass_Time)) == NULL)
    return NULL;

	char buf[128];
	long secs = ptr->getSeconds();
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(ptr->getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

  return PyString_FromString(time_str.c_str());
%End
};

class TimeStamp
{
%HeaderCode
	#include <BALL/CONCEPT/timeStamp.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(TimeStamp)
  TimeStamp();
  ~TimeStamp();
  bool isNewerThan(const Time& /time/) const throw ();
  bool isOlderThan(const Time& /time/) const throw ();
  bool isNewerThan(const TimeStamp& /stamp/) const throw ();
  bool isOlderThan(const TimeStamp& /stamp/) const throw ();
  virtual void stamp(const Time& /time/ = Time::ZERO) throw ();
  const Time& getTime() const throw ();
  //  void write(PersistenceManager& pm) const;
  //  bool read(PersistenceManager& pm);

  operator const char* ();
%MemberCode
  TimeStamp* ptr;
  if ((ptr = (TimeStamp*)sipGetCppPtr(sipThis,sipClass_TimeStamp)) == NULL)
    return NULL;

	long secs = ptr->getTime().getSeconds();
	char buf[128];
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(ptr->getTime().getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

  return PyString_FromString(time_str.c_str());
%End
};

 // std::ostream& operator << (std::ostream& os, const Time& time);
 // std::ostream& operator << (std::ostream& os, const TimeStamp& stamp);
