class Timer
{
%TypeHeaderCode
	#include <BALL/SYSTEM/timer.h>
%End
	public:

	Timer();
	Timer(Timer&);
	~Timer();

	bool isRunning() const;
	bool isValid() const;
	bool operator != (const Timer&) const;
	bool operator < (const Timer&) const;
	bool operator <= (const Timer&) const;
	bool operator == (const Timer&) const;
	bool operator > (const Timer&) const;
	bool operator >= (const Timer&) const;
	bool start();
	bool stop();
	double getCPUTime() const;
	double getClockTime() const;
	double getSystemTime() const;
	double getUserTime() const;
	void clear();
	void reset();
};
