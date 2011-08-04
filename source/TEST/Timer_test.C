// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <unistd.h>
///////////////////////////
#include <BALL/SYSTEM/timer.h>
#include <BALL/SYSTEM/file.h>

#ifdef BALL_COMPILER_MSVC
#	include<windows.h>
#	define sleep(a) Sleep(1000 * a)
#endif
///////////////////////////

START_TEST(Timer)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

#define BUSY_WAIT \
	STATUS("WAITING")\
	{ double x = 0.0; for (int i = 0; i < 5e6; i++, x += rand()) {}; }

Timer* timer_ptr = 0;
CHECK(Timer())
	timer_ptr = new Timer;
	TEST_NOT_EQUAL(timer_ptr, 0)
	TEST_EQUAL(timer_ptr->isRunning(), false)
RESULT

CHECK(~Timer())
	delete timer_ptr;
RESULT

CHECK(Timer(Timer& timer))
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2(t1);
	TEST_EQUAL(t2.isRunning(), false)

	STATUS(t1.getClockTime())
	STATUS(t2.getClockTime())
	STATUS(t1.getClockTime())
	STATUS(t2.getClockTime())
	STATUS(t1.getClockTime())
	STATUS(t2.getClockTime())

	double diff = t1.getClockTime() - t2.getClockTime();
	STATUS("t1.getClockTime() = " << t1.getClockTime() << " / diff = " << diff)
	PRECISION(1e-5)
	TEST_REAL_EQUAL(diff, 0.0)	

	diff = t1.getUserTime() - t2.getUserTime();
	STATUS("t1.getUserTime() = " << t1.getUserTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	

	diff = t1.getSystemTime() - t2.getSystemTime();
	STATUS("t1.getSystemTime() = " << t1.getSystemTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	

	diff = t1.getCPUTime() - t2.getCPUTime();
	STATUS("t1.getCPUTime() = " << t1.getCPUTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	
RESULT

CHECK(void clear())
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	t1.clear();
	TEST_EQUAL(t1.isRunning(), false)
	TEST_EQUAL(t1.getClockTime(), 0)	
	TEST_EQUAL(t1.getUserTime(), 0)	
	TEST_EQUAL(t1.getSystemTime(), 0)	
	TEST_EQUAL(t1.getCPUTime(), 0)	
RESULT

CHECK(bool start())
	Timer t1;
	TEST_EQUAL(t1.start(), true)
	TEST_EQUAL(t1.start(), false)
	TEST_EQUAL(t1.isRunning(), true)
	BUSY_WAIT
	TEST_EQUAL(t1.getClockTime() >= 0, true)	
	TEST_EQUAL(t1.getUserTime() >= 0, true)	
	TEST_EQUAL(t1.getCPUTime() >= 0, true)	
RESULT

CHECK(bool stop())
	Timer t1;
	t1.start();
	TEST_EQUAL(t1.isRunning(), true)
	TEST_EQUAL(t1.stop(), true)
	TEST_EQUAL(t1.stop(), false)
	TEST_EQUAL(t1.isRunning(), false)	
RESULT

CHECK(void reset())
	Timer t1;
	t1.start();
	// some waiting
	t1.reset();
	// this is somewhat dangerous, but the best we could come up
	// with
	TEST_EQUAL(t1.isRunning(), true)
	t1.stop();
	STATUS(t1.getClockTime())
	STATUS(t1.getUserTime())
	STATUS(t1.getSystemTime())
	STATUS(t1.getCPUTime())
	TEST_EQUAL(t1.getClockTime() < 0.1, true)	
	TEST_EQUAL(t1.getUserTime() < 0.1, true)	
	TEST_EQUAL(t1.getSystemTime() < 0.1, true)	
	TEST_EQUAL(t1.getCPUTime() < 0.1, true)	
RESULT

CHECK(double getClockTime() const)
	Timer t1;
	TEST_EQUAL(t1.getClockTime(), 0.0)	
	STATUS("t1.getClockTime() = " << t1.getClockTime())
	t1.start();
	sleep(1);
	t1.stop();
	STATUS("t1.getClockTime() = " << t1.getClockTime())
	t1.start();
	sleep(1);
	t1.stop();
	STATUS("t1.getClockTime() = " << t1.getClockTime())
	TEST_EQUAL(t1.getClockTime() > 1.0, true)
	TEST_EQUAL(t1.getClockTime() < 3.0, true)	
RESULT


CHECK(double getUserTime() const)
	Timer t1;
	TEST_EQUAL(t1.getUserTime(), 0)	
	t1.start();
	BUSY_WAIT
	t1.stop();
	TEST_EQUAL(t1.getUserTime() >= 0, true)	
RESULT

CHECK(double getSystemTime() const)
	Timer t1;
	TEST_EQUAL(t1.getSystemTime(), 0)	
	t1.start();
	BUSY_WAIT
	t1.stop();
	TEST_EQUAL(t1.getSystemTime() >= 0, true)	
	t1.start();
	File f(BALL_TEST_DATA_PATH(Timer_test1.txt));
	for (int i = 0; i < 200 ; i++)
	{
		TEST_EQUAL(f.copyTo(BALL_TEST_DATA_PATH(Timer_test1.bak)), true)
	}
	f.remove(BALL_TEST_DATA_PATH(Timer_test1.bak));
	t1.stop();
	TEST_EQUAL(t1.getSystemTime() >= 0, true)	
RESULT

CHECK(double getCPUTime() const)
	Timer t0;
	t0.start();
	Timer t1;
	STATUS(t1.getCPUTime())
	TEST_EQUAL(t1.getCPUTime(), 0)	
	t1.start();
	sleep(2);
	t1.stop();
	STATUS(t1.getCPUTime())
	TEST_EQUAL(t1.getCPUTime() <= 1, true)	
	t1.reset();
	t1.start();
	BUSY_WAIT
	BUSY_WAIT
	t1.stop();
	STATUS(t1.getCPUTime())
	TEST_EQUAL(t1.getCPUTime() > 0, true)	
	TEST_REAL_EQUAL(t1.getCPUTime(), t1.getSystemTime() + t1.getUserTime())	
	STATUS(t0.getCPUTime())
	t0.stop();
	STATUS(t0.getCPUTime())
RESULT

CHECK(Timer& operator = (const Timer& timer))
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	t2 = t1;
	TEST_EQUAL(t2.isRunning(), false)

	STATUS(t1.getClockTime())
	STATUS(t2.getClockTime())
	BUSY_WAIT
	STATUS(t1.getClockTime())
	STATUS(t2.getClockTime())
	STATUS(t1.getClockTime())
	STATUS(t2.getClockTime())
	STATUS(t1.getClockTime())
	BUSY_WAIT
	STATUS(t2.getClockTime())

	double diff = t1.getClockTime() - t2.getClockTime();
	STATUS("t1.getClockTime() = " << t1.getClockTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	

	diff = t1.getUserTime() - t2.getUserTime();
	STATUS("t1.getUserTime() = " << t1.getUserTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	

	diff = t1.getSystemTime() - t2.getSystemTime();
	STATUS("t1.getSystemTime() = " << t1.getSystemTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	

	diff = t1.getCPUTime() - t2.getCPUTime();
	STATUS("t1.getCPUTime() = " << t1.getCPUTime() << " / diff = " << diff)
	TEST_REAL_EQUAL(diff, 0.0)	
RESULT

CHECK(bool isRunning() const)
	Timer t1;
	t1.start();
	TEST_EQUAL(t1.isRunning(), true)
	t1.stop();
	TEST_EQUAL(t1.isRunning(), false)
RESULT

CHECK(bool operator == (const Timer& timer) const)
	Timer t1;
	t1.start();
	sleep(1);
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 == t2, false);
	t2 = t1;
	TEST_EQUAL(t1 == t2, true);
RESULT

CHECK(bool operator != (const Timer& timer) const)
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 != t2, true);
	t2 = t1;
	TEST_EQUAL(t1 != t2, false);
RESULT

CHECK(bool operator < (const Timer& timer) const)
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 < t2, false);
	TEST_EQUAL(t2 < t1, true);
	TEST_EQUAL(t1 < t2, false);
RESULT

CHECK(bool operator <= (const Timer& timer) const)
	Timer& t1 = *new Timer;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer& t2 = *new Timer;
	TEST_EQUAL(t1 <= t2, false);
	TEST_EQUAL(t2 <= t1, true);
	delete &t1;
	delete &t2;
RESULT

CHECK(bool operator >= (const Timer& timer) const)
	Timer& t1 = *new Timer;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer& t2 = *new Timer;
	TEST_EQUAL(t1 >= t2, true);
	TEST_EQUAL(t2 > t1, false);
	delete &t1;
	delete &t2;
RESULT

CHECK(bool operator > (const Timer& timer) const)
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 > t2, true);
	TEST_EQUAL(t2 > t1, false);
RESULT

CHECK(bool isValid() const)
	Timer t1;
	TEST_EQUAL(t1.isValid(), true);
RESULT

CHECK(void dump(::std::ostream& s = ::std::cout, Size depth = 0L) const)
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
  String filename;
	NEW_TMP_FILE(filename)

	std::ofstream outfile(filename.c_str(), std::ios::out);

	t1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Timer_test.txt))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
