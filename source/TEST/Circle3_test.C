// $Id: Circle3_test.C,v 1.3 2000/07/12 19:36:45 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#	include <BALL/MATHS/circle3.h>
#	include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(class_name, "$Id: Circle3_test.C,v 1.3 2000/07/12 19:36:45 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String filename;
using std::ofstream;
using std::ios;

CHECK(TCircle3::BALL_CREATE(TCircle3<T>))
	Vector3 v1(1.0, 2.0, 3.0), v2(1.0, 2.0, 3.0);
	Vector3 v0;
	Circle3 c(v1, v2, 5);
	Circle3* c_ptr = (Circle3*)c.create(false, true);
	TEST_REAL_EQUAL(c_ptr->p == v0, true)
	TEST_REAL_EQUAL(c_ptr->n == v0, true)
	delete c_ptr;
	c_ptr = (Circle3*)c.create();
	TEST_REAL_EQUAL(c_ptr->p == v1, true)
	TEST_REAL_EQUAL(c_ptr->n == v2, true)
	TEST_REAL_EQUAL(c_ptr->radius == 5.0, true)
	delete c_ptr;
RESULT

CHECK(TCircle3::TCircle3())
  Circle3* c;
	c = new Circle3();
	TEST_NOT_EQUAL(c, 0)
RESULT

CHECK(~TCircle();)
  Circle3* c;
	c = new Circle3();
	delete c;
RESULT	

Circle3 c, c2;
Vector3 p = Vector3(1.0, 2.0, 3.0),
				n = Vector3(4.0, 5.0, 6.0),
				v1, v2, v3;
				
float radian = 7.0;

//also test for TCircle3::get(TVector3<T>& point, TVector3<T>& normal, T& radius) const
CHECK(TCircle3::TCircle3(const TVector3<T>& point, const TVector3<T>& normal, const T& radius))
	c = Circle3(p, n, radian);
	float radian2;
	c.get(v1, v2, radian2);
	TEST_EQUAL(v1, p)
	TEST_EQUAL(v2, n)
	TEST_REAL_EQUAL(radian, radian2)
RESULT

CHECK(TCircle3::bool operator == (const TCircle3& circle) const )
	c = Circle3(p, n, radian);
	c2 = Circle3(p, n, radian);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(TCircle3::TCircle3(const TCircle3& circle, bool /* deep */ = true))
	c = Circle3(p, n, radian);
	c2 = Circle3(c);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(TCircle3::void swap(TCircle3& circle))
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 12);
	Circle3 cc = c, cc2 = c2;
	c2.swap(c);
	TEST_EQUAL(c == cc2, true)
	TEST_EQUAL(c2 == cc, true)
RESULT

CHECK(TCircle3::void set(const TCircle3& circle, bool /* deep */ = true))
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 212.0);
	c2.set(c);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(TCircle3::void set(const TVector3<T>& point, const TVector3<T>& normal, const T& radius))
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 212.0);
	c = Circle3(v1, v2, 212.0);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(TCircle3::void get(TCircle3& circle, bool /* deep */ = true) const)
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 212.0);
	c2.get(c);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(TCircle3::isValid() const )
	c = Circle3(v1, v2, 123.0);
	TEST_EQUAL(c.isValid(), true)
RESULT

CHECK(bool operator != (const TCircle3& circle) const )
	c = Circle3(p, n, radian);
	c2 = Circle3(22.2, n, radian);
	TEST_EQUAL(c != c2, true)
	c2 = Circle3(p, n, radian);
	TEST_EQUAL(c != c2, false)
RESULT

CHECK(has(const TVector3<T>& point, bool on_surface = false) const )
	c = Circle3(p, n, radian);
	TEST_EQUAL(c.has(p), true)
	v1 = Vector3( 1.0, 2.0, 11.0);
	TEST_EQUAL(c.has(v1), false)

	v1 = Vector3( 1, 0, 0);
	c = Circle3(p, v1, radian);
	v1 = Vector3( 1.0, 9.0, 3.0);
	TEST_EQUAL(c.has(v1, true), true)

	v1 = Vector3( 1.0, 8.1, 3.0);
	TEST_EQUAL(c.has(v1, true), false)
RESULT

CHECK(TCircle3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	c = Circle3(p, v1, radian);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	c.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Circle3_test.txt", true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TCircle3& circle))
	std::ifstream instr("data/Circle3_test2.txt");
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c = Circle3();

	c2 = Circle3(v1, v2, 7.0);
	instr >> c;
	instr.close();
	TEST_EQUAL(c, c2)
RESULT


NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TCircle3& circle))
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c = Circle3(v1, v2, 7.0);
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << c;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Circle3_test2.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
