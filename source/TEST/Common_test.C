// $Id: Common_test.C,v 1.1 2000/04/02 15:48:04 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/common.h>

///////////////////////////

START_TEST(class_name, "$Id: Common_test.C,v 1.1 2000/04/02 15:48:04 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
	
CHECK(abs(const T& t))
	TEST_REAL_EQUAL(Maths::abs(1.0), 1.0)
	TEST_REAL_EQUAL(Maths::abs(-1.0), 1.0)
	TEST_REAL_EQUAL(Maths::abs(0.0), 0.0)
RESULT


CHECK(frac(const T& t))
	TEST_REAL_EQUAL(Maths::frac(1.0), 0.0)
	TEST_REAL_EQUAL(Maths::frac(-1.0), 0.0)
	TEST_REAL_EQUAL(Maths::frac(-1.1), -0.1)
	TEST_REAL_EQUAL(Maths::frac(1.1), 0.1)
	TEST_REAL_EQUAL(Maths::frac(1.1), 0.1)
RESULT


CHECK(max(const T& a, const T& b))
	TEST_REAL_EQUAL(Maths::max(1.0, 1.0), 1.0)
	TEST_REAL_EQUAL(Maths::max(1.0, 1.1), 1.1)
	TEST_REAL_EQUAL(Maths::max(-1.0, 1.0), 1.0)
	TEST_REAL_EQUAL(Maths::max(-1.0, -1.0), -1.0)
	TEST_REAL_EQUAL(Maths::max(-1.0, -1.1), -1.0)
	TEST_REAL_EQUAL(Maths::max(0.0, 0.0), 0.0)
RESULT


CHECK(max(const T& a, const T& b, const T& c))
	TEST_REAL_EQUAL(Maths::max(1.0, 1.0, 1.0), 1.0)
	TEST_REAL_EQUAL(Maths::max(1.0, 1.1, 0.0), 1.1)
	TEST_REAL_EQUAL(Maths::max(-1.0, 1.0, 1.1), 1.1)
	TEST_REAL_EQUAL(Maths::max(-1.0, -1.0, -2.0), -1.0)
	TEST_REAL_EQUAL(Maths::max(0.0, 0.0, 0.0), 0.0)
RESULT


CHECK(min(const T& a, const T& b))
	TEST_REAL_EQUAL(Maths::min(1.0, 1.0), 1.0)
	TEST_REAL_EQUAL(Maths::min(1.0, 1.1), 1.0)
	TEST_REAL_EQUAL(Maths::min(-1.0, 1.0), -1.0)
	TEST_REAL_EQUAL(Maths::min(-1.0, -1.0), -1.0)
	TEST_REAL_EQUAL(Maths::min(-1.0, -1.1), -1.1)
	TEST_REAL_EQUAL(Maths::min(0.0, 0.0), 0.0)
RESULT


CHECK(min(const T& a, const T& b, const T& c))
	TEST_REAL_EQUAL(Maths::min(1.0, 1.0, 1.0), 1.0)
	TEST_REAL_EQUAL(Maths::min(1.0, 1.1, 0.0), 0.0)
	TEST_REAL_EQUAL(Maths::min(-1.0, 1.0, 1.1), -1.0)
	TEST_REAL_EQUAL(Maths::min(-1.0, -1.0, -2.0), -2.0)
	TEST_REAL_EQUAL(Maths::min(0.0, 0.0, 0.0), 0.0)
RESULT


CHECK(round(const T& t))
	TEST_REAL_EQUAL(Maths::round(1.0), 1.0)
	TEST_REAL_EQUAL(Maths::round(1.1), 1.0)
	TEST_REAL_EQUAL(Maths::round(0.0), 0.0)
	TEST_REAL_EQUAL(Maths::round(-1.9), -2.0)
	TEST_REAL_EQUAL(Maths::round(1.5), 2.0)
RESULT


CHECK(sgn(const T& t))
	TEST_REAL_EQUAL(Maths::sgn(1.0), 1.0)
	TEST_REAL_EQUAL(Maths::sgn(0.0), 0.0)
	TEST_REAL_EQUAL(Maths::sgn(-1.9), -1.0)
RESULT


CHECK(isFinite(const T& t))
	TEST_EQUAL(Maths::isFinite(1.0), true)
RESULT


CHECK(isInfinite(const T& t))
	TEST_EQUAL(Maths::isInfinite(1.0), false)
RESULT


CHECK(isNan(const T& t))
	TEST_EQUAL(Maths::isNan(1.0), false)
RESULT

CHECK(isZero(const T& t))
	TEST_EQUAL(Maths::isZero(0.0), true)
	TEST_EQUAL(Maths::isZero(0.1), false)
	TEST_EQUAL(Maths::isZero(-0.1), false)
RESULT

CHECK(isNotZero(const T& t))
	TEST_EQUAL(Maths::isNotZero(0.0), false)
	TEST_EQUAL(Maths::isNotZero(0.1), true)
	TEST_EQUAL(Maths::isNotZero(-0.1), true)
RESULT


CHECK(isEqual(const T1& a, const T2& b))
	TEST_EQUAL(Maths::isEqual(-0.1, -0.1), true)
	TEST_EQUAL(Maths::isEqual(-0.1, 0.1), false)
	TEST_EQUAL(Maths::isEqual(-0.1, -0.11), false)
RESULT


CHECK(isNotEqual(const T1& a, const T2& b))
	TEST_EQUAL(Maths::isNotEqual(-0.1, -0.1), false)
	TEST_EQUAL(Maths::isNotEqual(-0.1, 0.1), true)
	TEST_EQUAL(Maths::isNotEqual(-0.1, -0.11), true)
RESULT


CHECK(isLess(const T1& a, const T2& b))
	TEST_EQUAL(Maths::isLess(-0.1, -0.1), false)
	TEST_EQUAL(Maths::isLess(-0.1, 0.1), true)
	TEST_EQUAL(Maths::isLess(-0.1, -0.11), false)
RESULT


CHECK(isLessOrEqual(const T1& a, const T2& b))
	TEST_EQUAL(Maths::isLessOrEqual(-0.1, -0.1), true)
	TEST_EQUAL(Maths::isLessOrEqual(-0.1, 0.1), true)
	TEST_EQUAL(Maths::isLessOrEqual(-0.1, -0.11), false)
RESULT


CHECK(isGreater(const T1& a, const T2& b))
	TEST_EQUAL(Maths::isGreater(-0.1, -0.1), false)
	TEST_EQUAL(Maths::isGreater(-0.1, 0.1), false)
	TEST_EQUAL(Maths::isGreater(-0.1, -0.11), true)
RESULT


CHECK(isGreaterOrEqual(const T1& a, const T2& b))
	TEST_EQUAL(Maths::isGreaterOrEqual(-0.1, -0.1), true)
	TEST_EQUAL(Maths::isGreaterOrEqual(-0.1, 0.1), false)
	TEST_EQUAL(Maths::isGreaterOrEqual(-0.1, -0.11), true)
RESULT


CHECK(floor(const T1& t))
	TEST_REAL_EQUAL(Maths::floor(-0.1), -1.0)
	TEST_REAL_EQUAL(Maths::floor(0.0), 0.0)
	TEST_REAL_EQUAL(Maths::floor(0.1), 0.0)
RESULT


CHECK(ceiling(const T& t))
	TEST_REAL_EQUAL(Maths::ceiling(-0.1), -0.0)
	TEST_REAL_EQUAL(Maths::ceiling(0.0), 0.0)
	TEST_REAL_EQUAL(Maths::ceiling(0.1), 1.0)
RESULT


CHECK(compare(const T1& a, const T2& b))
	TEST_REAL_EQUAL(Maths::compare(-0.1, -0.1), 0)
	TEST_REAL_EQUAL(Maths::compare(-0.11, -0.1), -1)
	TEST_REAL_EQUAL(Maths::compare(-0.11, -0.12), 1)
RESULT


CHECK(isNear(const T1& a, const T2& b, const T3& maxDiff))
	TEST_EQUAL(Maths::isNear(-0.1, -0.1, 0), true)
	TEST_EQUAL(Maths::isNear(-0.1, -0.11, 0.01), true)
	TEST_EQUAL(Maths::isNear(-0.1, -0.12, 0.01), false)
RESULT						

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
