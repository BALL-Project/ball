// $Id: Vector3_test.C,v 1.1 2000/02/16 19:23:04 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
///////////////////////////

START_TEST(class_name, "$Id: Vector3_test.C,v 1.1 2000/02/16 19:23:04 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Vector3* v;
CHECK(Vector3::Vector3())
v = new Vector3();
TEST_NOT_EQUAL(v, 0)
TEST_EQUAL(v->x, 0)
TEST_EQUAL(v->y, 0)
TEST_EQUAL(v->z, 0)
RESULT											

CHECK(Vector3::~Vector3())
delete v;
RESULT

CHECK(Vector3(const float*))
float arr[3] = {0.1, 0.2, 0.3};
Vector3* v = new Vector3(arr);
#undef PRECISION
#define PRECISION 1e-6
TEST_REAL_EQUAL(v->x, 0.1)
TEST_REAL_EQUAL(v->y, 0.2)
TEST_REAL_EQUAL(v->z, 0.3)
RESULT

CHECK(operator + (const Vector3&, const Vector3&))
Vector3 a(1.1, 2.2, 3.3);
Vector3 b(5.5, 6.6, 7.7);
Vector3 c;
c = a + b;
TEST_REAL_EQUAL(c.x, 6.6)
TEST_REAL_EQUAL(c.y, 8.8)
TEST_REAL_EQUAL(c.z, 11.0)
Vector3 d(a + b);
TEST_REAL_EQUAL(d.x, 6.6)
TEST_REAL_EQUAL(d.y, 8.8)
TEST_REAL_EQUAL(d.z, 11.0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
