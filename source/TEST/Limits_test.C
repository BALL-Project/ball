// $Id: Limits_test.C,v 1.2 2001/07/15 17:32:39 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/COMMON/limits.h>
#ifdef BALL_HAS_NUMERIC_LIMITS
# include <limits>
#else
# include <limits.h>
#endif
///////////////////////////

START_TEST(Limits, "$Id: Limits_test.C,v 1.2 2001/07/15 17:32:39 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Limits<float>::max())
TEST_EQUAL(Limits<float>::max(), FLT_MAX)
RESULT

CHECK(Limits<float>::min())
TEST_EQUAL(Limits<float>::min(), FLT_MIN)
RESULT

CHECK(Limits<double>::max())
TEST_EQUAL(Limits<double>::max(), DBL_MAX)
RESULT

CHECK(Limits<double>::min())
TEST_EQUAL(Limits<double>::min(), DBL_MIN)
RESULT

CHECK(Limits<bool>::max())
TEST_EQUAL(Limits<bool>::max(), true)
RESULT

CHECK(Limits<bool>::min())
TEST_EQUAL(Limits<bool>::min(), false)
RESULT

CHECK(Limits<char>::max())
TEST_EQUAL(Limits<char>::max(), CHAR_MAX)
RESULT

CHECK(Limits<char>::min())
TEST_EQUAL(Limits<char>::min(), CHAR_MIN)
RESULT

CHECK(Limits<signed char>::max())
TEST_EQUAL(Limits<signed char>::max(), SCHAR_MAX)
RESULT

CHECK(Limits<signed char>::min())
TEST_EQUAL(Limits<signed char>::min(), SCHAR_MIN)
RESULT

CHECK(Limits<unsigned char>::max())
TEST_EQUAL(Limits<unsigned char>::max(), UCHAR_MAX)
RESULT

CHECK(Limits<unsigned char>::min())
TEST_EQUAL(Limits<unsigned char>::min(), 0)
RESULT

CHECK(Limits<int>::max())
TEST_EQUAL(Limits<int>::max(), INT_MAX)
RESULT

CHECK(Limits<int>::min())
TEST_EQUAL(Limits<int>::min(), INT_MIN)
RESULT

CHECK(Limits<short>::max())
TEST_EQUAL(Limits<short>::max(), SHRT_MAX)
RESULT

CHECK(Limits<short>::min())
TEST_EQUAL(Limits<short>::min(), SHRT_MIN)
RESULT

CHECK(Limits<unsigned short>::max())
TEST_EQUAL(Limits<unsigned short>::max(), USHRT_MAX)
RESULT

CHECK(Limits<unsigned short>::min())
TEST_EQUAL(Limits<unsigned short>::min(), 0)
RESULT

CHECK(Limits<long>::max())
TEST_EQUAL(Limits<long>::max(), LONG_MAX)
RESULT

CHECK(Limits<long>::min())
TEST_EQUAL(Limits<long>::min(), LONG_MIN)
RESULT

CHECK(Limits<unsigned long>::max())
TEST_EQUAL(Limits<unsigned long>::max(), ULONG_MAX)
RESULT

CHECK(Limits<unsigned long>::min())
TEST_EQUAL(Limits<unsigned long>::min(), 0)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
