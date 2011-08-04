// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/COMMON/limits.h>
#ifdef BALL_HAS_NUMERIC_LIMITS
# include <limits>
#else
# include <limits.h>
#endif
#ifdef BALL_HAS_VALUES_H
#include <values.h> 
#endif
///////////////////////////

START_TEST(Limits)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(static T max() throw())
	// Dummy tests -- individual tests are below
RESULT

CHECK(static T min() throw())
	// Dummy tests -- individual tests are below
RESULT

CHECK([EXTRA] static float max() throw())
	TEST_EQUAL(Limits<float>::max(), FLT_MAX)
RESULT

CHECK([EXTRA] static float min() throw())
	TEST_EQUAL(Limits<float>::min(), FLT_MIN)
RESULT

CHECK([EXTRA] static double max() throw())
	TEST_EQUAL(Limits<double>::max(), DBL_MAX)
RESULT

CHECK([EXTRA] static double min() throw())
	TEST_EQUAL(Limits<double>::min(), DBL_MIN)
RESULT

CHECK([EXTRA] static bool max() throw())
	TEST_EQUAL(Limits<bool>::max(), true)
RESULT

CHECK([EXTRA] static bool min() throw())
	TEST_EQUAL(Limits<bool>::min(), false)
RESULT

CHECK([EXTRA] static char max() throw())
	TEST_EQUAL(Limits<char>::max(), CHAR_MAX)
RESULT

CHECK([EXTRA] static char min() throw())
	TEST_EQUAL(Limits<char>::min(), CHAR_MIN)
RESULT

CHECK([EXTRA] static signed char max() throw())
	TEST_EQUAL(Limits<signed char>::max(), SCHAR_MAX)
RESULT

CHECK([EXTRA] static signed char min() throw())
	TEST_EQUAL(Limits<signed char>::min(), SCHAR_MIN)
RESULT

CHECK([EXTRA] static unsigned char max() throw())
	TEST_EQUAL(Limits<unsigned char>::max(), UCHAR_MAX)
RESULT

CHECK([EXTRA] static unsigned char min() throw())
	TEST_EQUAL(Limits<unsigned char>::min(), 0)
RESULT

CHECK([EXTRA] static unsigned int max() throw())
	TEST_EQUAL(Limits<unsigned int>::max(), UINT_MAX)
RESULT

CHECK([EXTRA] static unsigned int min() throw())
	TEST_EQUAL(Limits<unsigned int>::min(), 0)
RESULT

CHECK([EXTRA] static int max() throw())
	TEST_EQUAL(Limits<int>::max(), INT_MAX)
RESULT

CHECK([EXTRA] static int min() throw())
	TEST_EQUAL(Limits<int>::min(), INT_MIN)
RESULT

CHECK([EXTRA] static short max() throw())
	TEST_EQUAL(Limits<short>::max(), SHRT_MAX)
RESULT

CHECK([EXTRA] static short min() throw())
	TEST_EQUAL(Limits<short>::min(), SHRT_MIN)
RESULT

CHECK([EXTRA] static unsigned short max() throw())
	TEST_EQUAL(Limits<unsigned short>::max(), USHRT_MAX)
RESULT

CHECK([EXTRA] static unsigned short min() throw())
	TEST_EQUAL(Limits<unsigned short>::min(), 0)
RESULT

CHECK([EXTRA] static long max() throw())
	TEST_EQUAL(Limits<long>::max(), LONG_MAX)
RESULT

CHECK([EXTRA] static long min() throw())
	TEST_EQUAL(Limits<long>::min(), LONG_MIN)
RESULT

CHECK([EXTRA] static unsigned long max() throw())
	TEST_EQUAL(Limits<unsigned long>::max(), ULONG_MAX)
RESULT

CHECK([EXTRA] static unsigned long min() throw())
	TEST_EQUAL(Limits<unsigned long>::min(), 0)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
