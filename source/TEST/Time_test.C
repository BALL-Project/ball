// $Id: Time_test.C,v 1.2 2000/08/28 18:03:50 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>

///////////////////////////

START_TEST(Time, "$Id: Time_test.C,v 1.2 2000/08/28 18:03:50 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class Time::

Time* t_ptr;
CHECK(Time::Time())
	t_ptr = new Time;
	TEST_NOT_EQUAL(t_ptr, 0)
RESULT

CHECK(Time::~Time())
	delete t_ptr;
RESULT

CHECK(Time::getSeconds() const )
	Time t;
	TEST_EQUAL(t.getSeconds(), 0)
RESULT


CHECK(Time::getMicroSeconds() const )
	Time t;
	TEST_EQUAL(t.getMicroSeconds(), 0)
RESULT


CHECK(Time::set(long secs, long usecs) throw())
	Time t;
	TEST_EQUAL(t.getSeconds(), 0)
	TEST_EQUAL(t.getMicroSeconds(), 0)
	t.set(1,1);
	TEST_EQUAL(t.getSeconds(), 1)
	TEST_EQUAL(t.getMicroSeconds(), 1)
	t.set(9999,12345);
	TEST_EQUAL(t.getSeconds(), 9999)
	TEST_EQUAL(t.getMicroSeconds(), 12345)
RESULT


CHECK(Time::Time(const Time& time))
	Time t1;
	t1.set(12345678, 23456789);
	Time t2(t1);
	TEST_EQUAL(t2, t1)
	TEST_EQUAL(t2.getSeconds(), 12345678)
	TEST_EQUAL(t2.getMicroSeconds(), 23456789)
RESULT

CHECK(Time::set(const Time& time) throw())	
	Time t1, t2;
	t1.set(12345678, 23456789);
	t2.set(t1);
	TEST_EQUAL(t2, t1)
	TEST_EQUAL(t2.getSeconds(), 12345678)
	TEST_EQUAL(t2.getMicroSeconds(), 23456789)
RESULT


CHECK(Time::Time& operator = (const Time& time) throw())
	Time t1, t2;
	t1.set(12345678, 23456789);
	t2 = t1;
	TEST_EQUAL(t2, t1)
	TEST_EQUAL(t2.getSeconds(), 12345678)
	TEST_EQUAL(t2.getMicroSeconds(), 23456789)
RESULT


CHECK(Time::bool operator < (const Time& time) const  throw())
	Time t1, t2;
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


CHECK(Time::bool operator > (const Time& time) const  throw())
	Time t1, t2;
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


CHECK(Time::bool operator == (const Time& time) const  throw())
	Time t1, t2;
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


CHECK(Time::now())
	Time t1(Time::now());
	TEST_NOT_EQUAL(t1.getSeconds(), 0)
	TEST_NOT_EQUAL(t1.getMicroSeconds(), 0)
	Time t2(Time::now());
	TEST_NOT_EQUAL(t2.getSeconds(), 0)
	TEST_NOT_EQUAL(t2.getMicroSeconds(), 0)
	TEST_EQUAL((t1 < t2), true)
	TEST_EQUAL((t1 == t2), false)
RESULT


CHECK(Time::write(PersistenceManager& pm) const )
  //BAUSTELLE
RESULT


CHECK(Time::read(PersistenceManager& pm))
  //BAUSTELLE
RESULT

CHECK(ostream& operator << (ostream& os, const Time& time))
	// BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
