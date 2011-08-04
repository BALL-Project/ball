// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/MATHS/circle3.h>
#include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(Circle3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
using std::ofstream;
using std::ios;

CHECK(BALL_CREATE(TCircle3<T>))
	Vector3 v1(1.0, 2.0, 3.0), v2(1.0, 2.0, 3.0);
	Vector3 v0;
	Circle3 c(v1, v2, 5);
	Circle3* c_ptr = (Circle3*)c.create(false, true);
	TEST_EQUAL(c_ptr->p == v0, true)
	TEST_EQUAL(c_ptr->n == v0, true)
	delete c_ptr;
	c_ptr = (Circle3*)c.create();
	TEST_EQUAL(c_ptr->p == v1, true)
	TEST_EQUAL(c_ptr->n == v2, true)
	TEST_EQUAL(c_ptr->radius == 5.0, true)
	delete c_ptr;
RESULT

Circle3* circle3_ptr = 0;
CHECK(TCircle3() throw())
	circle3_ptr = new Circle3;
	TEST_NOT_EQUAL(circle3_ptr, 0)
RESULT

CHECK(~TCircle3() throw())
	delete circle3_ptr;
RESULT	

Circle3 c, c2;
Vector3 p = Vector3(1.0, 2.0, 3.0),
				n = Vector3(4.0, 5.0, 6.0),
				v1, v2, v3;
				
float radian = 7.0;

CHECK(TCircle3(const TVector3<T>& point, const TVector3<T>& normal, const T& radius) throw())
	c = Circle3(p, n, radian);
	float radian2;
	c.get(v1, v2, radian2);
	TEST_EQUAL(v1, p)
	TEST_EQUAL(v2, n)
	TEST_EQUAL(radian, radian2)
RESULT

CHECK(bool operator == (const TCircle3& circle) const throw())
	c = Circle3(p, n, radian);
	c2 = Circle3(p, n, radian);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(TCircle3(const TCircle3& circle) throw())
	c = Circle3(p, n, radian);
	c2 = Circle3(c);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(void swap(TCircle3& circle) throw())
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 12);
	Circle3 cc = c, cc2 = c2;
	c2.swap(c);
	TEST_EQUAL(c == cc2, true)
	TEST_EQUAL(c2 == cc, true)
RESULT

CHECK(void set(const TCircle3& circle) throw())
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 212.0);
	c2.set(c);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(void set(const TVector3<T>& point, const TVector3<T>& normal, const T& rad) throw())
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 212.0);
	c = Circle3(v1, v2, 212.0);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(void get(TCircle3& circle) const throw())
	c = Circle3(p, n, radian);
	v1 = Vector3(3.0, 4.0, 5.0);
	v2 = Vector3(6.0, 7.0, 8.0);
	c2 = Circle3(v1, v2, 212.0);
	c2.get(c);
	TEST_EQUAL(c == c2, true)
RESULT

CHECK(bool isValid() const throw())
	c = Circle3(v1, v2, 123.0);
	TEST_EQUAL(c.isValid(), true)
RESULT

CHECK(bool operator != (const TCircle3& circle) const throw())
	c = Circle3(p, n, radian);
	c2 = Circle3(Vector3(22.2), n, radian);
	TEST_EQUAL(c != c2, true)
	c2 = Circle3(p, n, radian);
	TEST_EQUAL(c != c2, false)
RESULT

CHECK(bool has(const TVector3<T>& point, bool on_surface = false) const throw())
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

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	c = Circle3(p, v1, radian);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	c.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Circle3_test.txt))
RESULT

CHECK(std::istream& operator >> (std::istream& s, TCircle3& circle))
	std::ifstream instr(BALL_TEST_DATA_PATH(Circle3_test2.txt));
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
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << c;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Circle3_test2.txt))
RESULT

CHECK(TCircle3& operator = (const TCircle3& circle) throw())
	Circle3 c(v1, v2, 5);
	Circle3 test = c;
	TEST_EQUAL(c.p, v1)
	TEST_EQUAL(c.n, v2)
	TEST_EQUAL(c.radius, 5)
RESULT

CHECK(void clear() throw())
	Circle3 c(v1, v2, 5);
	c.clear();
	Circle3 c2;
	TEST_EQUAL(c, c2)
RESULT

CHECK(void get(TVector3<T>& point, TVector3<T>& normal, T& rhs) const throw())
	Circle3 c(v1, v2, 5);
	Vector3 p, n;
	float radius;
	c.get(p, n, radius);
	TEST_EQUAL(p, v1)
	TEST_EQUAL(n, v2)
	TEST_EQUAL(radius, 5)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
