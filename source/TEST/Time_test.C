// $Id: Time_test.C,v 1.1 2000/08/28 16:10:26 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>

///////////////////////////

START_TEST(Time, "$Id: Time_test.C,v 1.1 2000/08/28 16:10:26 oliver Exp $")

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

CHECK(Time::Time(const Time& time))
	Time t1;
	Time t2(t1);
	TEST_EQUAL(t2, t1)
RESULT

CHECK(Time::set(long secs, long usecs) throw())
  //BAUSTELLE
RESULT


CHECK(Time::set(const Time& time) throw())
  //BAUSTELLE
RESULT


CHECK(Time::Time& operator = (const Time& time) throw())
  //BAUSTELLE
RESULT


CHECK(Time::bool operator < (const Time& time) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::bool operator > (const Time& time) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::bool operator == (const Time& time) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::getSeconds() const )
  //BAUSTELLE
RESULT


CHECK(Time::getMicroSeconds() const )
  //BAUSTELLE
RESULT


CHECK(Time::now())
  //BAUSTELLE
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
