// $Id: timeStamp.sip,v 1.2 2000/10/24 20:29:18 oliver Exp $

class PreciseTime
{
%HeaderCode
	#include <time.h>
	#include <BALL/CONCEPT/timeStamp.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(PreciseTime)
  PreciseTime();
  PreciseTime(const PreciseTime& /time/);
  ~PreciseTime();
  static const PreciseTime ZERO;
  void set(long /secs/, long /usecs/) throw();
  void set(const PreciseTime& /time/) throw();
  // const PreciseTime& operator = (const PreciseTime& /time/) throw();
	//  bool operator < (const PreciseTime& /time/) const throw();
	//  bool operator > (const PreciseTime& /time/) const throw();
 	// bool operator == (const PreciseTime& /time/) const throw();
  long getSeconds() const throw();
  long getMicroSeconds() const throw();
  static const PreciseTime& now() throw();
  //  void write(PersistenceManager& pm)const;
  //  bool read(PersistenceManager& pm);
  operator const char* ();
%MemberCode
  PreciseTime* ptr;
  if ((ptr = (PreciseTime*)sipGetCppPtr(sipThis,sipClass_PreciseTime)) == NULL)
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
  bool isNewerThan(const PreciseTime& /time/) const throw ();
  bool isOlderThan(const PreciseTime& /time/) const throw ();
  bool isNewerThan(const TimeStamp& /stamp/) const throw ();
  bool isOlderThan(const TimeStamp& /stamp/) const throw ();
  virtual void stamp(const PreciseTime& /time/ = PreciseTime::ZERO) throw ();
  const PreciseTime& getTime() const throw ();
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

 // std::ostream& operator << (std::ostream& os, const PreciseTime& time);
 // std::ostream& operator << (std::ostream& os, const TimeStamp& stamp);
