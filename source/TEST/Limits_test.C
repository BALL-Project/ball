// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Limits_test.C,v 1.6 2003/06/09 22:40:52 oliver Exp $

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

START_TEST(Limits, "$Id: Limits_test.C,v 1.6 2003/06/09 22:40:52 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(static float max() throw())
	TEST_EQUAL(Limits<float>::max(), FLT_MAX)
RESULT

CHECK(static float min() throw())
	TEST_EQUAL(Limits<float>::min(), FLT_MIN)
RESULT

CHECK(static double max() throw())
	TEST_EQUAL(Limits<double>::max(), DBL_MAX)
RESULT

CHECK(static double min() throw())
	TEST_EQUAL(Limits<double>::min(), DBL_MIN)
RESULT

CHECK(static bool max() throw())
	TEST_EQUAL(Limits<bool>::max(), true)
RESULT

CHECK(static bool min() throw())
	TEST_EQUAL(Limits<bool>::min(), false)
RESULT

CHECK(static char max() throw())
	TEST_EQUAL(Limits<char>::max(), CHAR_MAX)
RESULT

CHECK(static char min() throw())
	TEST_EQUAL(Limits<char>::min(), CHAR_MIN)
RESULT

CHECK(static signed char max() throw())
	TEST_EQUAL(Limits<signed char>::max(), SCHAR_MAX)
RESULT

CHECK(static signed char min() throw())
	TEST_EQUAL(Limits<signed char>::min(), SCHAR_MIN)
RESULT

CHECK(static unsigned char max() throw())
	TEST_EQUAL(Limits<unsigned char>::max(), UCHAR_MAX)
RESULT

CHECK(static unsigned char min() throw())
	TEST_EQUAL(Limits<unsigned char>::min(), 0)
RESULT

CHECK(static unsigned int max() throw())
	TEST_EQUAL(Limits<unsigned int>::max(), UINT_MAX)
RESULT

CHECK(static unsigned int min() throw())
	TEST_EQUAL(Limits<unsigned int>::min(), 0)
RESULT

CHECK(static int max() throw())
	TEST_EQUAL(Limits<int>::max(), INT_MAX)
RESULT

CHECK(static int min() throw())
	TEST_EQUAL(Limits<int>::min(), INT_MIN)
RESULT

CHECK(static short max() throw())
	TEST_EQUAL(Limits<short>::max(), SHRT_MAX)
RESULT

CHECK(static short min() throw())
	TEST_EQUAL(Limits<short>::min(), SHRT_MIN)
RESULT

CHECK(static unsigned short max() throw())
	TEST_EQUAL(Limits<unsigned short>::max(), USHRT_MAX)
RESULT

CHECK(static unsigned short min() throw())
	TEST_EQUAL(Limits<unsigned short>::min(), 0)
RESULT

CHECK(static long max() throw())
	TEST_EQUAL(Limits<long>::max(), LONG_MAX)
RESULT

CHECK(static long min() throw())
	TEST_EQUAL(Limits<long>::min(), LONG_MIN)
RESULT

CHECK(static unsigned long max() throw())
	TEST_EQUAL(Limits<unsigned long>::max(), ULONG_MAX)
RESULT

CHECK(static unsigned long min() throw())
	TEST_EQUAL(Limits<unsigned long>::min(), 0)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
