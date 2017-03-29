// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/plane3.h>
#include <BALL/MATHS/line3.h>
///////////////////////////

START_TEST(Plane3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector3 v0, v1, v2, v3, v4;
Plane3 p, p2;

String filename;
using std::ofstream;
using std::ios;

CHECK(BALL_CREATE(TPlane3<T>))
	v0 = Vector3();
	v1 = Vector3(1, 2, 3);
	v2 = Vector3(4, 5, 6);
	Plane3 p(v1, v2);
	Plane3* p_ptr = (Plane3*)p.create(false, true);
	TEST_EQUAL(p_ptr->p, v0)
	TEST_EQUAL(p_ptr->n, v0)
	delete p_ptr;
	p_ptr = (Plane3*)p.create();
	TEST_EQUAL(p_ptr->p, v1)
	TEST_EQUAL(p_ptr->n, v2)
	delete p_ptr;
RESULT

Plane3* plane3_ptr = 0;
CHECK(TPlane3() throw())
	plane3_ptr = new Plane3;
	TEST_NOT_EQUAL(plane3_ptr, 0)
RESULT		

CHECK(~TPlane3() throw())
	delete plane3_ptr;
RESULT		

CHECK(bool operator == (const TPlane3& plane) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	TEST_EQUAL(p == p2 , false)
	v4 = Vector3(4.0, 5.0, 6.0);
	p2 = Plane3(v3, v4);
	TEST_EQUAL(p == p2 , true)
RESULT

CHECK(bool operator != (const TPlane3& plane) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	TEST_EQUAL(p != p2 , true)
	v4 = Vector3(4.0, 5.0, 6.0);
	p2 = Plane3(v3, v4);
	TEST_EQUAL(p != p2 , false)
RESULT

CHECK(void get(TPlane3& plane) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	p.get(p2);
	TEST_EQUAL(p, p2)
RESULT

CHECK(void set(const TPlane3& plane) throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	p.set(p2);
	TEST_EQUAL(p, p2)
RESULT

CHECK(void set(const TVector3<T>& point, const TVector3<T>& normal) throw())
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3();
	p.set(v3, v4);
	p2 = Plane3(v3, v4);
	TEST_EQUAL(p, p2)
RESULT

CHECK(TPlane3& operator = (const TPlane3& plane) throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = p;
	TEST_EQUAL(p, p2)
RESULT

CHECK(void get(TVector3<T>& point, TVector3<T>& normal) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p.get(v3, v4);
	TEST_EQUAL(v1, v3)
	TEST_EQUAL(v2, v4)
RESULT

CHECK(bool has(const TVector3<T>& point) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(111.0, 211.0, 113.0);
	TEST_EQUAL(p.has(v3), true)
	TEST_EQUAL(p.has(v4), false)
RESULT

CHECK(bool has(const TLine3<T>& line) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(0, 0, 0);
	Line3 l = Line3(v1, v2);
	v2 = Vector3(10.0, 11.0, 12.0);
	p = Plane3(v1, v2);
	TEST_EQUAL(p.has(l), true)

	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(111.0, 211.0, 113.0);
	l = Line3(v3, v4);
	TEST_EQUAL(p.has(l), false)
RESULT

CHECK(void swap(TPlane3& plane) throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(10.0, 20.0, 30.0);
	v4 = Vector3(111.0, 211.0, 113.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	p.swap(p2);
	TEST_EQUAL(p.p, v3)
	TEST_EQUAL(p.n, v4)
	TEST_EQUAL(p2.p, v1)
	TEST_EQUAL(p2.n, v2)
RESULT

CHECK(void normalize() throw(Exception::DivisionByZero))
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 9.0, 16.0);
	p = Plane3(v1, v2);
	p.normalize();
	float erg = ::sqrt(4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0);
	TEST_REAL_EQUAL(p.n[0], 4.0 / erg)
	TEST_REAL_EQUAL(p.n[1], 9.0 / erg)
	TEST_REAL_EQUAL(p.n[2], 16.0 / erg)
	v2 = Vector3(0, 0, 0);
	p = Plane3(v1, v2);
	TEST_EXCEPTION(Exception::DivisionByZero, p.normalize())
RESULT

CHECK(void hessify() throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 9.0, 16.0);
	p = Plane3(v1, v2);
	p.normalize();
	float erg = ::sqrt (4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0);
	TEST_REAL_EQUAL(p.n[0], 4 / erg)
	TEST_REAL_EQUAL(p.n[1], 9 / erg)
	TEST_REAL_EQUAL(p.n[2], 16 / erg)
	TEST_EQUAL(p.p, v1)
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(p.isValid(), true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TPlane3& plane))
	std::ifstream instr(BALL_TEST_DATA_PATH(Plane3_test2.txt));
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3();
	instr >> p;
	instr.close();
	TEST_EQUAL(p.p, v1)
	TEST_EQUAL(p.n, v2)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TPlane3<T>& plane))
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << p;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Plane3_test2.txt))
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	p.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Plane3_test.txt))
RESULT

CHECK(TPlane3(const T& a, const T& b, const T& c, const T& d) throw(Exception::DivisionByZero))
	Plane3 p(0, 1, 2, 3);
	TEST_EQUAL(p.n, Vector3(0,1,2))
	TEST_EQUAL(p.p, Vector3(0,-3,0))
	TEST_EXCEPTION(Exception::DivisionByZero, Plane3(0,0,0,0))
RESULT

CHECK(TPlane3(const TPlane3& plane) throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	Plane3 p2(p);
	TEST_EQUAL(p2, p)
RESULT

CHECK(TPlane3(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c) throw())
	v1 = Vector3(0, 0, 0);
	v2 = Vector3(1, 0, 0);
	Vector3 v3 = Vector3(0, 1, 0);
	p = Plane3(v1, v2, v3);
	TEST_EQUAL(p.n, Vector3(0,0,1))
	TEST_EQUAL(p.p, v1)
RESULT

CHECK(TPlane3(const TVector3<T>& point, const TVector3<T>& normal) throw())
	Plane3 p2(Vector3(0,0,0), Vector3(0,0,1));
	TEST_EQUAL(p2, p)
RESULT

CHECK(void clear() throw())
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	p.clear();
	Plane3 p2;
	TEST_EQUAL(p, p2)
RESULT

CHECK(void set(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c) throw())
	v1 = Vector3(0, 0, 0);
	v2 = Vector3(1, 0, 0);
	Vector3 v3 = Vector3(0, 1, 0);
	p.set(v1, v2, v3);
	TEST_EQUAL(p.n, Vector3(0,0,1))
	TEST_EQUAL(p.p, v1)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
