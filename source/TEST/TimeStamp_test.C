// $Id: TimeStamp_test.C,v 1.1 2000/08/28 16:10:26 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/timeStamp.h>

///////////////////////////

START_TEST(TimeStamp, "$Id: TimeStamp_test.C,v 1.1 2000/08/28 16:10:26 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Time::TimeStamp::TimeStamp())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::~TimeStamp())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::isNewerThan(const Time& time) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::isOlderThan(const Time& time) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::isNewerThan(const TimeStamp& stamp) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::isOlderThan(const TimeStamp& stamp) const  throw())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::stamp(const Time& time = Time::ZERO) throw())
  //BAUSTELLE
RESULT


CHECK(Time::TimeStamp::getTime() const  throw())
  //BAUSTELLE
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
