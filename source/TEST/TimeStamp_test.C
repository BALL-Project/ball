// $Id: TimeStamp_test.C,v 1.6 2001/05/10 23:32:08 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>

///////////////////////////

START_TEST(TimeStamp, "$Id: TimeStamp_test.C,v 1.6 2001/05/10 23:32:08 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#define BUSY_WAIT { double x = 0.0; for (int i = 0; i < 200000; i++, x += 0.1); } 

using namespace BALL;

TimeStamp* ts = 0;
CHECK(TimeStamp::TimeStamp())
	ts = new TimeStamp;
	TEST_NOT_EQUAL(ts, 0)
RESULT


CHECK(TimeStamp::~TimeStamp())
	delete ts;
RESULT


CHECK(TimeStamp::getTime() const  throw())
  TimeStamp* t1 = new TimeStamp;	
	t1->stamp();
	STATUS(*t1)
	BUSY_WAIT
  TimeStamp* t2 = new TimeStamp;
	t2->stamp();
	STATUS(*t2)
	TEST_NOT_EQUAL(t1->getTime(), t2->getTime())
	TEST_EQUAL((t1->getTime() < t2->getTime()), true)
	TEST_EQUAL((t1->getTime() > t2->getTime()), false)
	delete t1;
	delete t2;
RESULT

CHECK(TimeStamp::isNewerThan(const Time& time) const  throw())
	TimeStamp* ts1 = new TimeStamp;
	ts1->stamp();
	STATUS(*ts1)
	BUSY_WAIT
	TimeStamp* ts2 = new TimeStamp;
	ts2->stamp();
	STATUS(*ts2)
	TEST_EQUAL(ts1->isNewerThan(ts1->getTime()), false)
	TEST_EQUAL(ts1->isNewerThan(ts2->getTime()), false)
	TEST_EQUAL(ts2->isNewerThan(ts1->getTime()), true)
	TEST_EQUAL(ts2->isNewerThan(ts2->getTime()), false)
	delete ts1;
	delete ts2;
RESULT


CHECK(TimeStamp::isOlderThan(const Time& time) const  throw())
	TimeStamp* ts1 = new TimeStamp;
	ts1->stamp();
	STATUS(*ts1)
	BUSY_WAIT
	TimeStamp* ts2 = new TimeStamp;
	ts2->stamp();
	STATUS(*ts2)
	TEST_EQUAL(ts1->isOlderThan(ts1->getTime()), false)
	TEST_EQUAL(ts1->isOlderThan(ts2->getTime()), true)
	TEST_EQUAL(ts2->isOlderThan(ts1->getTime()), false)
	TEST_EQUAL(ts2->isOlderThan(ts2->getTime()), false)
	delete ts1;
	delete ts2;
RESULT


CHECK(TimeStamp::isNewerThan(const TimeStamp& stamp) const  throw())
	TimeStamp* ts1 = new TimeStamp;
	ts1->stamp();
	STATUS(*ts1)
	BUSY_WAIT
	TimeStamp* ts2 = new TimeStamp;
	ts2->stamp();
	STATUS(*ts2)
	TEST_EQUAL(ts1->isNewerThan(*ts1), false)
	TEST_EQUAL(ts1->isNewerThan(*ts2), false)
	TEST_EQUAL(ts2->isNewerThan(*ts1), true)
	TEST_EQUAL(ts2->isNewerThan(*ts2), false)
	delete ts1;
	delete ts2;
RESULT


CHECK(TimeStamp::isOlderThan(const TimeStamp& stamp) const  throw())
	TimeStamp* ts1 = new TimeStamp;
	ts1->stamp();
	STATUS(*ts1)
	BUSY_WAIT
	TimeStamp* ts2 = new TimeStamp;
	ts2->stamp();
	STATUS(*ts2)
	TEST_EQUAL(ts1->isOlderThan(*ts1), false)
	TEST_EQUAL(ts1->isOlderThan(*ts2), true)
	TEST_EQUAL(ts2->isOlderThan(*ts1), false)
	TEST_EQUAL(ts2->isOlderThan(*ts2), false)
	delete ts1;
	delete ts2;
RESULT


CHECK(TimeStamp::stamp(const Time& time = ZERO) throw())
  TimeStamp* ts1 = new TimeStamp;
	ts1->stamp();
	STATUS(*ts1)
	BUSY_WAIT
  TimeStamp* ts2 = new TimeStamp;
	ts2->stamp();
	STATUS(*ts2)
	TEST_EQUAL(ts1->isNewerThan(*ts1), false)
	TEST_EQUAL(ts1->isNewerThan(*ts2), false)
	TEST_EQUAL(ts2->isNewerThan(*ts1), true)
	TEST_EQUAL(ts2->isNewerThan(*ts2), false)
	BUSY_WAIT
	ts1->stamp();
	TEST_EQUAL(ts1->isNewerThan(*ts1), false)
	TEST_EQUAL(ts1->isNewerThan(*ts2), true)
	TEST_EQUAL(ts2->isNewerThan(*ts1), false)
	TEST_EQUAL(ts2->isNewerThan(*ts2), false)
	delete ts1;
	delete ts2;
RESULT


CHECK(TimeStamp::write(PersistenceManager& pm) const )
  //BAUSTELLE
RESULT


CHECK(TimeStamp::read(PersistenceManager& pm))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
