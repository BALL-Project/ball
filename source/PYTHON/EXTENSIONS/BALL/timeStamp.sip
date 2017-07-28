class PreciseTime
{
%TypeHeaderCode
	#include <time.h>
	#include <BALL/CONCEPT/timeStamp.h>
%End
	public:

	PreciseTime();
	PreciseTime(const PreciseTime&);
	~PreciseTime() throw();

	static const PreciseTime ZERO;
	void set(long, long) throw();
	void set(const PreciseTime&) throw();
	bool operator < (const PreciseTime&) const throw();
	bool operator > (const PreciseTime&) const throw();
	bool operator == (const PreciseTime&) const throw();
	long getSeconds() const throw();
	long getMicroSeconds() const throw();
	static PreciseTime now() throw();

	SIP_PYOBJECT __str__();
%MethodCode
	static char buf[128];
	time_t secs = sipCpp->getSeconds();
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(sipCpp->getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

	sipRes = PyString_FromString(time_str.c_str());
%End
};

class TimeStamp
{
%TypeHeaderCode
	#include <BALL/CONCEPT/timeStamp.h>
%End
	public:

	TimeStamp() throw();
	~TimeStamp() throw();

	bool isNewerThan(const PreciseTime&) const throw ();
	bool isOlderThan(const PreciseTime&) const throw ();
	bool isNewerThan(const TimeStamp&) const throw ();
	bool isOlderThan(const TimeStamp&) const throw ();
	virtual void stamp(const PreciseTime& time = PreciseTime::ZERO) throw ();
	const PreciseTime& getTime() const throw ();

	SIP_PYOBJECT __str__();
%MethodCode
	time_t secs = sipCpp->getTime().getSeconds();
	static char buf[128];
	strftime(buf, 127, "\045Y\045m\045d\045H\045M\045S", localtime(&secs));
	String time_str(buf);
	time_str.append(".");
	String usec_str(sipCpp->getTime().getMicroSeconds() / 1.0e6);
	time_str.append(usec_str.after("."));

	sipRes = PyString_FromString(time_str.c_str());
%End
};
