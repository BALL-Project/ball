// $Id: TimeStamp_test.C,v 1.10 2002/01/04 01:22:00 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <fstream>

///////////////////////////

START_TEST(TimeStamp, "$Id: TimeStamp_test.C,v 1.10 2002/01/04 01:22:00 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

#define BUSY_WAIT { double x = 0.0; for (int i = 0; i < 2000000; i++, x += rand()); } 

using namespace BALL;
using namespace std;

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

TextPersistenceManager pm;
CHECK(TimeStamp::write(PersistenceManager& pm) const )
	TimeStamp t;
	// a very nasty way to break the encapsulation, but simplifies
	// things a great deal....!
	PreciseTime& t_ref = const_cast<PreciseTime&>(t.getTime());
	t_ref.set(12345678, 23456789);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream of(filename.c_str(), std::ios::out);
	pm.setOstream(of);
	t.write(pm);
	of.close();
	TEST_FILE(filename.c_str(), "data/TimeStamp_test2.txt", false)
RESULT


CHECK(TimeStamp::read(PersistenceManager& pm))
	TimeStamp t;
	std::ifstream inf("data/TimeStamp_test2.txt");
	pm.setIstream(inf);
	t.read(pm);
	inf.close();
	TEST_EQUAL(t.getTime().getSeconds(), 12345678)
	TEST_EQUAL(t.getTime().getMicroSeconds(), 23456789)
RESULT

CHECK(TimeStamp::operator << (std::ostream& os, const TimeStamp& ts))
	TimeStamp t;
	// a very nasty way to break the encapsulation, but simplifies
	// things a great deal....!
	PreciseTime& t_ref = const_cast<PreciseTime&>(t.getTime());
	t_ref.set(12345678, 23456789);
	String filename;
	NEW_TMP_FILE(filename);
	ofstream of(filename.c_str(), std::ios::out);
	of << t << std::endl;
	of.close();
	TEST_FILE(filename.c_str(), "data/TimeStamp_test.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
