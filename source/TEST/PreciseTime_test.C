// $Id: PreciseTime_test.C,v 1.5 2002/01/04 01:10:20 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <fstream>

///////////////////////////

START_TEST(PreciseTime, "$Id: PreciseTime_test.C,v 1.5 2002/01/04 01:10:20 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

// tests for class PreciseTime::

#define BUSY_WAIT \
	double x = 0.0;  for (int i = 0; i < 200000; i++, x += rand());

PreciseTime* t_ptr;
CHECK(PreciseTime::PreciseTime())
	t_ptr = new PreciseTime;
	TEST_NOT_EQUAL(t_ptr, 0)
RESULT

CHECK(PreciseTime::~PreciseTime())
	delete t_ptr;
RESULT

CHECK(PreciseTime::getSeconds() const )
	PreciseTime t;
	TEST_EQUAL(t.getSeconds(), 0)
RESULT


CHECK(PreciseTime::getMicroSeconds() const )
	PreciseTime t;
	TEST_EQUAL(t.getMicroSeconds(), 0)
RESULT


CHECK(PreciseTime::set(long secs, long usecs) throw())
	PreciseTime t;
	TEST_EQUAL(t.getSeconds(), 0)
	TEST_EQUAL(t.getMicroSeconds(), 0)
	t.set(1,1);
	TEST_EQUAL(t.getSeconds(), 1)
	TEST_EQUAL(t.getMicroSeconds(), 1)
	t.set(9999,12345);
	TEST_EQUAL(t.getSeconds(), 9999)
	TEST_EQUAL(t.getMicroSeconds(), 12345)
RESULT


CHECK(PreciseTime::PreciseTime(const PreciseTime& time))
	PreciseTime t1;
	t1.set(12345678, 23456789);
	PreciseTime t2(t1);
	TEST_EQUAL(t2, t1)
	TEST_EQUAL(t2.getSeconds(), 12345678)
	TEST_EQUAL(t2.getMicroSeconds(), 23456789)
RESULT

CHECK(PreciseTime::set(const PreciseTime& time) throw())	
	PreciseTime t1, t2;
	t1.set(12345678, 23456789);
	t2.set(t1);
	TEST_EQUAL(t2, t1)
	TEST_EQUAL(t2.getSeconds(), 12345678)
	TEST_EQUAL(t2.getMicroSeconds(), 23456789)
RESULT


CHECK(PreciseTime::PreciseTime& operator = (const PreciseTime& time) throw())
	PreciseTime t1, t2;
	t1.set(12345678, 23456789);
	t2 = t1;
	TEST_EQUAL(t2, t1)
	TEST_EQUAL(t2.getSeconds(), 12345678)
	TEST_EQUAL(t2.getMicroSeconds(), 23456789)
RESULT

CHECK(void PreciseTime::clear() throw())
	PreciseTime t1;
	PreciseTime t2;
	TEST_EQUAL(t1, t2)
	TEST_EQUAL(t1.getSeconds(), 0)
	TEST_EQUAL(t1.getMicroSeconds(), 0)
	t1.set(12345, 23456);
	TEST_EQUAL(t1.getSeconds(), 12345)
	TEST_EQUAL(t1.getMicroSeconds(), 23456)
	t1.clear();
	TEST_EQUAL(t1, t2)
RESULT


CHECK(PreciseTime::bool operator < (const PreciseTime& time) const  throw())
	PreciseTime t1, t2;
	t1.set(12345678, 23456789);
	t2.set(12345679, 23456789);
	TEST_EQUAL((t2 < t1), false)
	TEST_EQUAL((t1 < t2), true)
	t2.set(12345678, 23456789);
	TEST_EQUAL((t2 < t1), false)
	TEST_EQUAL((t1 < t2), false)
	t2.set(12345678, 2345);
	TEST_EQUAL((t2 < t1), true)
	TEST_EQUAL((t1 < t2), false)
RESULT


CHECK(PreciseTime::bool operator > (const PreciseTime& time) const  throw())
	PreciseTime t1, t2;
	t1.set(12345678, 23456789);
	t2.set(12345679, 23456789);
	TEST_EQUAL((t2 > t1), true)
	TEST_EQUAL((t1 > t2), false)
	t2.set(12345678, 23456789);
	TEST_EQUAL((t2 > t1), false)
	TEST_EQUAL((t1 > t2), false)
	t2.set(12345678, 2345);
	TEST_EQUAL((t2 > t1), false)
	TEST_EQUAL((t1 > t2), true)
RESULT


CHECK(PreciseTime::bool operator == (const PreciseTime& time) const  throw())
	PreciseTime t1, t2;
	t1.set(12345678, 23456789);
	t2.set(12345679, 23456789);
	TEST_EQUAL((t2 == t1), false)
	TEST_EQUAL((t1 == t2), false)
	t2.set(12345678, 23456789);
	TEST_EQUAL((t2 == t1), true)
	TEST_EQUAL((t1 == t2), true)
	t2.set(12345678, 2345);
	TEST_EQUAL((t2 == t1), false)
	TEST_EQUAL((t1 == t2), false)
RESULT


CHECK(PreciseTime::now())
	PreciseTime t1(PreciseTime::now());
	TEST_NOT_EQUAL(t1.getSeconds(), 0)
	TEST_NOT_EQUAL(t1.getMicroSeconds(), 0)
	BUSY_WAIT
	PreciseTime t2(PreciseTime::now());
	TEST_NOT_EQUAL(t2.getSeconds(), 0)
	TEST_NOT_EQUAL(t2.getMicroSeconds(), 0)
	TEST_EQUAL((t1 < t2), true)
	TEST_EQUAL((t1 == t2), false)
RESULT

TextPersistenceManager pm;
CHECK(PreciseTime::write(PersistenceManager& pm) const )
	PreciseTime t(12345678, 23456789);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream of(filename.c_str(), std::ios::out);
	pm.setOstream(of);
	t.write(pm);
	of.close();
	TEST_FILE(filename.c_str(), "data/PreciseTime_test2.txt", false)
RESULT


CHECK(PreciseTime::read(PersistenceManager& pm))
	PreciseTime t;
	std::ifstream inf("data/PreciseTime_test2.txt");
	pm.setIstream(inf);
	t.read(pm);
	inf.close();
	TEST_EQUAL(t.getSeconds(), 12345678)
	TEST_EQUAL(t.getMicroSeconds(), 23456789)
RESULT

CHECK(ostream& operator << (ostream& os, const PreciseTime& time))
	PreciseTime t(12345678, 23456789);
	String filename;
	NEW_TMP_FILE(filename);
	ofstream of(filename.c_str(), std::ios::out);
	of << t << std::endl;
	of.close();
	TEST_FILE(filename.c_str(), "data/PreciseTime_test.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
