// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#	include <BALL/MATHS/simpleBox3.h>
#	include <BALL/MATHS/vector3.h>
///////////////////////////

START_TEST(SimpleBox3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

CHECK(BALL_CREATE(TSimpleBox3<T>))
	Vector3 v1(1.0, 2.0, 3.0), v2(1.0, 2.0, 3.0);
	Vector3 v0;
	SimpleBox3 v(v1, v2);
	SimpleBox3* v_ptr = (SimpleBox3*)v.create(false, true);
	TEST_EQUAL(v_ptr->a == v0, true)
	TEST_EQUAL(v_ptr->b == v0, true)
	delete v_ptr;
	v_ptr = (SimpleBox3*)v.create();
	TEST_EQUAL(v_ptr->a == v1, true)
	TEST_EQUAL(v_ptr->b == v2, true)
	delete v_ptr;
RESULT

SimpleBox3* simpleBox3_ptr = 0;
CHECK(TSimpleBox3() throw())
	simpleBox3_ptr = new SimpleBox3;
	TEST_NOT_EQUAL(simpleBox3_ptr, 0)
RESULT

CHECK(~TSimpleBox3() throw())
	delete simpleBox3_ptr;
RESULT

SimpleBox3 box, box2;
Vector3 v1, v2, v3, v4;
float a = 1.0, b = 2.0, c = 3.0,
			d = 6.0, e = 5.0, f = 4.0;
float a1, b1, c1, d1, e1, f1;

v1 = Vector3(1.0, 2.0, 3.0);
v2 = Vector3(6.0, 5.0, 4.0);

String filename;
using std::ofstream;
using std::ios;

CHECK(TSimpleBox3(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz) throw())
	box = SimpleBox3(a, b, c, d, e, f);
	float a1, b1, c1, d1, e1, f1;
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT

CHECK(void get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) throw())
	box = SimpleBox3(a, b, c, d, e, f);
	float a1, b1, c1, d1, e1, f1;
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT

CHECK(TSimpleBox3(const TSimpleBox3& box) throw())
	box = SimpleBox3(v1, v2);
	box2 = SimpleBox3(box);
	float a1, b1, c1, d1, e1, f1;
	box2.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT

CHECK(TSimpleBox3(const TVector3<T>& a, const TVector3<T>& b) throw())
	box = SimpleBox3(v1, v2);
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT

CHECK(void clear() throw())
	box = SimpleBox3(v1, v2);
	box.clear();
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a1, 0)
	TEST_REAL_EQUAL(b1, 0)
	TEST_REAL_EQUAL(c1, 0)
	TEST_REAL_EQUAL(d1, 0)
	TEST_REAL_EQUAL(e1, 0)
	TEST_REAL_EQUAL(f1, 0)
RESULT

CHECK(T getSurface() const throw())
	box = SimpleBox3(v1, v2);
	TEST_REAL_EQUAL(box.getSurface(), 46.0)
RESULT

CHECK(T getVolume() const throw())
	box = SimpleBox3(v1, v2);
	TEST_REAL_EQUAL(box.getVolume(), 15.0)
RESULT

CHECK(T getWidth() const throw())
	box = SimpleBox3(v1, v2);
	TEST_REAL_EQUAL(box.getWidth(), 5.0)
RESULT

CHECK(T getHeight() const throw())
	box = SimpleBox3(v1, v2);
	TEST_REAL_EQUAL(box.getHeight(), 3.0)
RESULT

CHECK(T getDepth() const throw())
	box = SimpleBox3(v1, v2);
	TEST_REAL_EQUAL(box.getDepth(), 1.0)
RESULT

CHECK(void join(const TSimpleBox3& box) throw())
	box = SimpleBox3(v1, v2);
	v3 = Vector3(101.0, 102.0, 103.0);
	v4 = Vector3(104.0, 105.0, 106.0);
	box2 = SimpleBox3(v3, v4);
	box.join(box2);
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(1, a1)
	TEST_REAL_EQUAL(2, b1)
	TEST_REAL_EQUAL(3, c1)
	TEST_REAL_EQUAL(104, d1)
	TEST_REAL_EQUAL(105, e1)
	TEST_REAL_EQUAL(106, f1)
RESULT

CHECK(bool operator == (const TSimpleBox3& box) const throw())
	box = SimpleBox3(v1, v2);
	box2 = SimpleBox3(v3, v4);
	TEST_EQUAL(box == box2, false)
	box2 = SimpleBox3(v1, v2);
	TEST_EQUAL(box == box2, true)
RESULT

CHECK(bool operator != (const TSimpleBox3& box) const throw())
	box = SimpleBox3(v1, v2);
	box2 = SimpleBox3(v3, v4);
	TEST_EQUAL(box != box2, true)
	box2 = SimpleBox3(v1, v2);
	TEST_EQUAL(box != box2, false)
RESULT

CHECK(bool isValid() const throw())
	box = SimpleBox3(v1, v2);
	TEST_EQUAL(box.isValid(), true)
RESULT

CHECK(bool has(const TVector3<T>& point, bool on_surface = false) const throw())
	v1 = Vector3(0.0, 0.0, 0.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(4.0, 5.1, 6.0);
	box = SimpleBox3(v1, v2);
	TEST_EQUAL(box.has(v3, true), false)
	v3 = Vector3(4.0, 5.0, 6.0);
	TEST_EQUAL(box.has(v3, true), true)
	TEST_EQUAL(box.has(v3, false), true)
	v3 = Vector3(4.0, 4.0, 6.0);
	TEST_EQUAL(box.has(v3, false), true)

	v2 = Vector3(0.0, 0.0, 0.0);
	v1 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(4.0, 5.1, 6.0);
	box = SimpleBox3(v1, v2);
	TEST_EQUAL(box.has(v3, true), false)
	v3 = Vector3(4.0, 5.0, 6.0);
	TEST_EQUAL(box.has(v3, true), true)
	TEST_EQUAL(box.has(v3, false), true)
	v3 = Vector3(4.0, 4.0, 6.0);
	TEST_EQUAL(box.has(v3, false), true)
RESULT

CHECK(bool isIntersecting(const TSimpleBox3& box) const throw())
	v1 = Vector3(0.0, 0.0, 0.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	box = SimpleBox3(v1, v2);

	v1 = Vector3(0.0, 0.0, 0.0);
	v2 = Vector3(-4.0, -5.0, -6.0);
	box2 = SimpleBox3(v1, v2);
	TEST_EQUAL(box.isIntersecting(box2) , true)

	v1 = Vector3(-0.1, 0.0, 0.0);
	v2 = Vector3(-4.0, -5.0, -6.0);
	box2 = SimpleBox3(v1, v2);
	TEST_EQUAL(box.isIntersecting(box2) , false)

	v2 = Vector3(0.0, 0.0, 0.0);
	v1 = Vector3(-4.0, -5.0, -6.0);
	box2 = SimpleBox3(v1, v2);
	TEST_EQUAL(box2.isIntersecting(box) , true)

	TEST_EQUAL(box2.isIntersecting(box2) , true)
RESULT

CHECK(void set(const TSimpleBox3<T>& box) throw())
	box = SimpleBox3(v1, v2);
	box2.set(box);
	TEST_EQUAL(box == box2, true)
RESULT

CHECK(void set(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz) throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(6.0, 5.0, 4.0);
	box = SimpleBox3(v1, v2);
	box2.set(1.0, 2.0, 3.0, 6.0, 5.0, 4.0);
	TEST_EQUAL(box == box2, true)
RESULT

CHECK(void get(TSimpleBox3& box) const throw())
	box = SimpleBox3(v1, v2);
	box.get(box2);
	TEST_EQUAL(box == box2, true)
RESULT

CHECK(void get(TVector3& lower, TVector3& upper) const throw())
	SimpleBox3 box;
	box2.get(box);
	TEST_EQUAL(box == box2, true)
RESULT

CHECK(void get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const throw())
	box = SimpleBox3(v1, v2);
	box.get(a1, b1, c1, d1, e1, f1);
	box2.set(a1, b1, c1, d1, e1, f1);
	TEST_EQUAL(box == box2, true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	SimpleBox3 v(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	v.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(SimpleBox3_test.txt))
RESULT

CHECK(std::istream& operator >> (std::istream& s, TSimpleBox3<T>& SimpleBox3))
	std::ifstream instr(BALL_TEST_DATA_PATH(SimpleBox3_test2.txt));
	SimpleBox3 b(10, 20, 30, 40, 50, 60);
	instr >> b;
	instr.close();
	SimpleBox3 b1(1, 2, 3, 4, 5, 6);
	TEST_EQUAL(b, b1)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TSimpleBox3<T>& SimpleBox3))
	SimpleBox3 v(1, 2, 3, 4, 5, 6);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(SimpleBox3_test2.txt))
RESULT

CHECK(TSimpleBox3& operator = (const TSimpleBox3& box) throw())
	SimpleBox3 v(1, 2, 3, 4, 5, 6);
	SimpleBox3 v2 = v;
	TEST_EQUAL(v2, v)
RESULT

CHECK(void get(TVector3<T>& lower, TVector3<T>& upper) const throw())
	SimpleBox3 v(1, 2, 3, 4, 5, 6);
	Vector3 a, b;
	v.get(a, b);
	TEST_EQUAL(a, Vector3(1,2,3))
	TEST_EQUAL(b, Vector3(4,5,6))
RESULT

CHECK(void set(const TSimpleBox3& box) throw())
	SimpleBox3 v(1, 2, 3, 4, 5, 6);
	SimpleBox3 v2;
	v2.set(v);
	TEST_EQUAL(v2, v)
RESULT

CHECK(void set(const TVector3<T>& lower, const TVector3<T>& upper) throw())
	SimpleBox3 v2;
	v2.set(Vector3(1,2,3), Vector3(4,5,6));
	SimpleBox3 v(1, 2, 3, 4, 5, 6);
	TEST_EQUAL(v2, v)	
RESULT

CHECK(void swap(TSimpleBox3& box) throw())
	SimpleBox3 v(1, 2, 3, 4, 5, 6);
	SimpleBox3 v2(v);
	SimpleBox3 y(10, 20, 30, 40, 50, 60);
	SimpleBox3 y2(y);
	y2.swap(v2);
	TEST_EQUAL(v, y2)
	TEST_EQUAL(y, v2)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
