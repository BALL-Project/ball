// $Id: LogStream_test.C,v 1.1 2000/05/24 00:02:57 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/COMMON/logStream.h>
#include <sys/time.h>
#	include <BALL/MATHS/common.h>

///////////////////////////

START_TEST(class_name, "$Id: LogStream_test.C,v 1.1 2000/05/24 00:02:57 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

LogStream* l1 = new LogStream();
LogStream* l2 = new LogStream(true);

CHECK(LogStream(bool associate_stdio = false))
	TEST_NOT_EQUAL(l1, 0)
	TEST_NOT_EQUAL(l2, 0)
RESULT

CHECK(~LogStream())
	delete l1;
	delete l2;
RESULT

CHECK(LogStream(LogStreamBuf* buf))
	LogStream l1;
	l1.error() << "TEST" <<endl;
	//LogStream* l2 = new LogStream(l1);
	TEST_NOT_EQUAL(l2, 0)
RESULT

CHECK(rdbuf())
// BAUSTELLE
RESULT

CHECK(operator -> ())
	//TEST_EQUAL(l1->*this, l1.rdbuf())
RESULT

CHECK(setLevel(int level))
	LogStream l1;
	l1 << "TEST1" <<endl;
	l1.setLevel(99);
	l1 << "TEST2" <<endl;
	TEST_EQUAL(l1.getLineLevel(0), 0)
	TEST_EQUAL(l1.getLineLevel(1), 99)
RESULT

CHECK(l1.getLevel())
	LogStream l1;
	TEST_EQUAL(l1.getLevel(), 0)
	l1.setLevel(99);
	TEST_EQUAL(l1.getLevel(), 99)
RESULT

CHECK(level(int n))
	LogStream l1;
	l1.level(99) << "TEST" <<endl;
	TEST_EQUAL(l1.getNumberOfLines(), 1)
	TEST_EQUAL(l1.getLineText(0), "TEST")
	TEST_EQUAL(l1.getLineLevel(0), 99)
RESULT

CHECK(info(int n = 0))
	LogStream l1;
	l1.warn() << "TEST" <<endl;
	TEST_EQUAL(l1.getNumberOfLines(), 1)
	TEST_EQUAL(l1.getLineText(0), "TEST")
	TEST_EQUAL(l1.getLineLevel(0), LogStream::INFORMATION)
	l1.warn(1) << "TEST2" <<endl;
	TEST_EQUAL(l1.getLineLevel(1), LogStream::INFORMATION + 1)
RESULT

CHECK(error(int n = 0))
	LogStream l1;
	l1.error() << "TEST" <<endl;
	TEST_EQUAL(l1.getNumberOfLines(), 1)
	TEST_EQUAL(l1.getLineText(0), "TEST")
	TEST_EQUAL(l1.getLineLevel(0), LogStream::ERROR)
RESULT

CHECK(warn(int n = 0))
	LogStream l1;
	l1.warn() << "TEST" <<endl;
	TEST_EQUAL(l1.getNumberOfLines(), 1)
	TEST_EQUAL(l1.getLineText(0), "TEST")
	TEST_EQUAL(l1.getLineLevel(0), LogStream::WARNING)
RESULT

CHECK(insert(std::ostream& s, int min_level = INT_MIN, int max_level = INT_MAX))
	ostream s;
	LogStream l1;
	l1.insert(s);
// BAUSTELLE
RESULT

CHECK(remove(std::ostream& s))
// BAUSTELLE
RESULT

CHECK(insertNotification(const std::ostream& s, const Target& target))
// BAUSTELLE
RESULT

CHECK(removeNotification(const std::ostream& s))
// BAUSTELLE
RESULT

CHECK(setMinLevel(const std::ostream& s, int min_level))
	LogStream l1;
//	l1.setMinLevel(
RESULT

CHECK(setMaxLevel(const std::ostream& s, int max_level))
// BAUSTELLE
RESULT

CHECK(setPrefix(const std::ostream& s, const string& prefix))
// BAUSTELLE
RESULT

CHECK(clear())
	LogStream l1;
	l1.error() << "TEST" <<endl;
	TEST_EQUAL(l1.getNumberOfLines(), 0)
RESULT

CHECK(l1.getNumberOfLines(int min_level = INT_MIN, int max_level = INT_MAX))
	LogStream l1;
	TEST_EQUAL(l1.getNumberOfLines(), 0)
	l1.error() << "TEST" <<endl;
	TEST_EQUAL(l1.getNumberOfLines(), 1)
RESULT

CHECK(l1.getLineText(Index index))
	LogStream l1;
	l1.error() << "TEST" <<endl;
	TEST_EQUAL(l1.getLineText(0), "TEST")
RESULT

CHECK(l1.getLineTime(Index index))
	LogStream l1;
	time_t timer;
  timer = time(NULL);
	l1.error() << "TEST" <<endl;
	TEST_EQUAL(timer, l1.getLineTime(0))
	TEST_EQUAL(Maths::isNear(timer, l1.getLineTime(0), (long)1), true)
RESULT

CHECK(l1.getLineLevel(Index index))
	LogStream l1;
	l1.level(99) << "TEST" <<endl;
	TEST_EQUAL(l1.getLineLevel(0), 99)
RESULT

CHECK(l1.getLineIndices(const int min_level = INT_MIN, const int max_level = INT_MAX, const time_t earliest = 0))
// BAUSTELLE
RESULT

CHECK(filterLinesAfterLevel(const list<int>, const int min_level = INT_MIN, int max_level = INT_MAX))
// BAUSTELLE
RESULT

CHECK(filterLinesAfterTime(const list<int>, const time_t earliest, const time_t latest))
// BAUSTELLE
RESULT

CHECK(filterLinesAfterContent(const list<int>, const string& s))
// BAUSTELLE
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
