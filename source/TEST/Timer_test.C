// $Id: Timer_test.C,v 1.11 2001/07/17 02:36:38 oliver Exp $
#include <BALL/CONCEPT/classTest.h>
#include <unistd.h>
///////////////////////////
#include <BALL/SYSTEM/timer.h>
#include <BALL/SYSTEM/file.h>
///////////////////////////

START_TEST(Timer, "$Id: Timer_test.C,v 1.11 2001/07/17 02:36:38 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

#define BUSY_WAIT \
	STATUS("WAITING")\
	{ double x = 0.0; for (int i = 0; i < 500000; i++, x += rand()); }

CHECK(Timer::Timer())
	Timer* t1 = new Timer();
	TEST_NOT_EQUAL(t1, 0)
	TEST_EQUAL(t1->isRunning(), false)
RESULT

CHECK(Timer::~Timer())
	Timer* t1 = new Timer();
	delete t1;
RESULT

CHECK(Timer::Timer(Timer& timer))
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2(t1);
	TEST_EQUAL(t2.isRunning(), false)
	TEST_REAL_EQUAL(t1.getClockTime(), t2.getClockTime())	
	TEST_REAL_EQUAL(t1.getUserTime(), t2.getUserTime())	
	TEST_REAL_EQUAL(t1.getSystemTime(), t2.getSystemTime())	
	TEST_REAL_EQUAL(t1.getCPUTime(), t2.getCPUTime())	
RESULT

CHECK(Timer::clear())
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

CHECK(Timer::start())
	Timer t1;
	TEST_EQUAL(t1.start(), true)
	TEST_EQUAL(t1.start(), false)
	TEST_EQUAL(t1.isRunning(), true)
	BUSY_WAIT
	TEST_EQUAL(t1.getClockTime() >= 0, true)	
	TEST_EQUAL(t1.getUserTime() >= 0, true)	
	TEST_EQUAL(t1.getCPUTime() >= 0, true)	
RESULT

CHECK(Timer::stop())
	Timer t1;
	t1.start();
	TEST_EQUAL(t1.isRunning(), true)
	TEST_EQUAL(t1.stop(), true)
	TEST_EQUAL(t1.stop(), false)
	TEST_EQUAL(t1.isRunning(), false)	
RESULT

CHECK(Timer::reset())
	Timer t1;
	t1.start();
	// busy waiting
	t1.reset();
	// this is somewhat dangerous, but the best we could come up
	// with
	TEST_EQUAL(t1.isRunning(), true)
	TEST_EQUAL(t1.getClockTime() < 0.1, true)	
	TEST_EQUAL(t1.getUserTime() < 0.1, true)	
	TEST_EQUAL(t1.getSystemTime() < 0.1, true)	
	TEST_EQUAL(t1.getCPUTime() < 0.1, true)	
RESULT

CHECK(Timer::getClockTime() const )
	Timer t1;
	TEST_EQUAL(t1.getClockTime(), 0)	
	t1.start();
	sleep(2);
	t1.stop();
	TEST_EQUAL(t1.getClockTime() > 1, true)
	TEST_EQUAL(t1.getClockTime() < 3, true)	
RESULT

CHECK(Timer::getUserTime() const )
	Timer t1;
	TEST_EQUAL(t1.getUserTime(), 0)	
	t1.start();
	BUSY_WAIT
	t1.stop();
	TEST_EQUAL(t1.getUserTime() >= 0, true)	
RESULT

CHECK(Timer::getSystemTime() const )
	Timer t1;
	TEST_EQUAL(t1.getSystemTime(), 0)	
	t1.start();
	BUSY_WAIT
	t1.stop();
	TEST_EQUAL(t1.getSystemTime() > 0, false)	
	t1.start();
	File f("data/Timer_test1.txt");
	for (int i = 0; i < 200 ; i++)
	{
		TEST_EQUAL(f.copyTo("data/Timer_test1.bak"), true)
	}
	f.remove("data/Timer_test1.bak");
	TEST_EQUAL(t1.getSystemTime() >= 0, true)	
RESULT

CHECK(Timer::getCPUTime() const )
	Timer t1;
	TEST_EQUAL(t1.getCPUTime(), 0)	
	t1.start();
	sleep(2);
	t1.stop();
	TEST_EQUAL(t1.getCPUTime() <= 1, true)	
	t1.reset();
	t1.start();
	BUSY_WAIT
	t1.stop();
	TEST_EQUAL(t1.getCPUTime() > 0, true)	
	TEST_REAL_EQUAL(t1.getCPUTime(), t1.getSystemTime() + t1.getUserTime())	
RESULT

CHECK(Timer::Timer& operator = (const Timer& timer))
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	t2 = t1;
	TEST_EQUAL(t2.isRunning(), false)
	TEST_EQUAL(t1.getClockTime(), t2.getClockTime())	
	TEST_REAL_EQUAL(t1.getUserTime(), t2.getUserTime())	
	TEST_REAL_EQUAL(t1.getSystemTime(), t2.getSystemTime())	
	TEST_REAL_EQUAL(t1.getCPUTime(), t2.getCPUTime())	
RESULT

CHECK(Timer::isRunning() const )
	Timer t1;
	t1.start();
	TEST_EQUAL(t1.isRunning(), true)
	t1.stop();
	TEST_EQUAL(t1.isRunning(), false)
RESULT

CHECK(Timer::bool operator == (const Timer& timer) const )
	Timer t1;
	t1.start();
	sleep(1);
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 == t2, false);
	t2 = t1;
	TEST_EQUAL(t1 == t2, true);
RESULT

CHECK(Timer::bool operator != (const Timer& timer) const )
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 != t2, true);
	t2 = t1;
	TEST_EQUAL(t1 != t2, false);
RESULT

CHECK(Timer::bool operator < (const Timer& timer) const )
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 < t2, false);
	TEST_EQUAL(t2 < t1, true);
	t2 = t1;
	TEST_EQUAL(t1 < t2, false);
RESULT

CHECK(Timer::bool operator <= (const Timer& timer) const )
	Timer& t1 = *new Timer;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer& t2 = *new Timer;
	TEST_EQUAL(t1 <= t2, false);
	TEST_EQUAL(t2 <= t1, true);
	t2 = t1;
	TEST_EQUAL(t1 <= t2, true);
	delete &t1;
	delete &t2;
RESULT

CHECK(Timer::bool operator >= (const Timer& timer) const )
	Timer& t1 = *new Timer;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer& t2 = *new Timer;
	TEST_EQUAL(t1 >= t2, true);
	TEST_EQUAL(t2 > t1, false);
	t2 = t1;
	TEST_EQUAL(t1 >= t2, true);
	delete &t1;
	delete &t2;
RESULT

CHECK(Timer::bool operator > (const Timer& timer) const )
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
	Timer t2;
	TEST_EQUAL(t1 > t2, true);
	TEST_EQUAL(t2 > t1, false);
	t2 = t1;
	TEST_EQUAL(t1 > t2, false);
RESULT

CHECK(Timer::isValid() const )
	Timer t1;
	TEST_EQUAL(t1.isValid(), true);
RESULT

CHECK(Timer::dump(::std::ostream& s = ::std::cout, Size depth = 0L) const )
	Timer t1;
	t1.start();
	BUSY_WAIT
	t1.stop();
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	t1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Timer_test.txt", true)
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
