// $Id: TimeStamp_test.C,v 1.2 2000/08/28 20:27:59 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>

///////////////////////////

START_TEST(TimeStamp, "$Id: TimeStamp_test.C,v 1.2 2000/08/28 20:27:59 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

TimeStamp* ts = 0;
CHECK(Time::TimeStamp::TimeStamp())
	ts = new TimeStamp;
	TEST_NOT_EQUAL(ts, 0)
RESULT


CHECK(Time::TimeStamp::~TimeStamp())
	delete ts;
RESULT


CHECK(Time::TimeStamp::getTime() const  throw())
  TimeStamp t1;
  TimeStamp t2;
	TEST_NOT_EQUAL(t1.getTime(), t2.getTime())
	TEST_EQUAL((t1.getTime() < t2.getTime()), true)
	TEST_EQUAL((t1.getTime() > t2.getTime()), false)
RESULT

CHECK(Time::TimeStamp::isNewerThan(const Time& time) const  throw())
	TimeStamp ts1;
	STATUS(ts1)
	TimeStamp ts2;
	STATUS(ts2)
	TEST_EQUAL(ts1.isNewerThan(ts1.getTime()), false)
	TEST_EQUAL(ts1.isNewerThan(ts2.getTime()), false)
	TEST_EQUAL(ts2.isNewerThan(ts1.getTime()), true)
	TEST_EQUAL(ts2.isNewerThan(ts2.getTime()), false)
RESULT


CHECK(Time::TimeStamp::isOlderThan(const Time& time) const  throw())
	TimeStamp ts1;
	STATUS(ts1)
	TimeStamp ts2;
	STATUS(ts2)
	TEST_EQUAL(ts1.isOlderThan(ts1.getTime()), false)
	TEST_EQUAL(ts1.isOlderThan(ts2.getTime()), true)
	TEST_EQUAL(ts2.isOlderThan(ts1.getTime()), false)
	TEST_EQUAL(ts2.isOlderThan(ts2.getTime()), false)
RESULT


CHECK(Time::TimeStamp::isNewerThan(const TimeStamp& stamp) const  throw())
	TimeStamp ts1;
	STATUS(ts1)
	TimeStamp ts2;
	STATUS(ts2)
	TEST_EQUAL(ts1.isNewerThan(ts1), false)
	TEST_EQUAL(ts1.isNewerThan(ts2), false)
	TEST_EQUAL(ts2.isNewerThan(ts1), true)
	TEST_EQUAL(ts2.isNewerThan(ts2), false)
RESULT


CHECK(Time::TimeStamp::isOlderThan(const TimeStamp& stamp) const  throw())
	TimeStamp ts1;
	STATUS(ts1)
	TimeStamp ts2;
	STATUS(ts2)
	TEST_EQUAL(ts1.isOlderThan(ts1), false)
	TEST_EQUAL(ts1.isOlderThan(ts2), true)
	TEST_EQUAL(ts2.isOlderThan(ts1), false)
	TEST_EQUAL(ts2.isOlderThan(ts2), false)
RESULT


CHECK(Time::TimeStamp::stamp(const Time& time = Time::ZERO) throw())
	TimeStamp ts1;
	TimeStamp ts2;
	TEST_EQUAL(ts1.isNewerThan(ts1), false)
	TEST_EQUAL(ts1.isNewerThan(ts2), false)
	TEST_EQUAL(ts2.isNewerThan(ts1), true)
	TEST_EQUAL(ts2.isNewerThan(ts2), false)
	ts1.stamp();
	TEST_EQUAL(ts1.isNewerThan(ts1), false)
	TEST_EQUAL(ts1.isNewerThan(ts2), true)
	TEST_EQUAL(ts2.isNewerThan(ts1), false)
	TEST_EQUAL(ts2.isNewerThan(ts2), false)
RESULT


CHECK(Time::TimeStamp::write(PersistenceManager& pm) const )
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::read(PersistenceManager& pm))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
