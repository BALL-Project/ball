// $Id: timeStamp.sip,v 1.4 2001/08/01 01:28:39 oliver Exp $

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
  ~PreciseTime() throw();
  static const PreciseTime ZERO;
  void set(long /secs/, long /usecs/) throw();
  void set(const PreciseTime& /time/) throw();
  // const PreciseTime& operator = (const PreciseTime& /time/) throw();
	//  bool operator < (const PreciseTime& /time/) const throw();
	//  bool operator > (const PreciseTime& /time/) const throw();
 	// bool operator == (const PreciseTime& /time/) const throw();
  long getSeconds() const throw();
  long getMicroSeconds() const throw();
  static PreciseTime now() throw();
  //  void write(PersistenceManager& pm)const;
  //  bool read(PersistenceManager& pm);
	PyMethods:
	__str__
%MemberCode
  PreciseTime* ptr;
  if ((ptr = (PreciseTime*)sipGetCppPtr((sipThisType*)a0,sipClass_PreciseTime)) == NULL)
    return NULL;

	char buf[128];
	time_t secs = ptr->getSeconds();
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
  TimeStamp() throw();
  ~TimeStamp() throw();
  bool isNewerThan(const PreciseTime& /time/) const throw ();
  bool isOlderThan(const PreciseTime& /time/) const throw ();
  bool isNewerThan(const TimeStamp& /stamp/) const throw ();
  bool isOlderThan(const TimeStamp& /stamp/) const throw ();
  virtual void stamp(const PreciseTime& /time/ = PreciseTime::ZERO) throw ();
  const PreciseTime& getTime() const throw ();
  //  void write(PersistenceManager& pm) const;
  //  bool read(PersistenceManager& pm);

	PyMethods:
	__str__
%MemberCode
  TimeStamp* ptr;
  if ((ptr = (TimeStamp*)sipGetCppPtr((sipThisType*)a0,sipClass_TimeStamp)) == NULL)
    return NULL;

	time_t secs = ptr->getTime().getSeconds();
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
