// $Id: Plane3_test.C,v 1.9 2000/07/26 16:49:47 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/plane3.h>
#include <BALL/MATHS/line3.h>
///////////////////////////

START_TEST(class_name, "$Id: Plane3_test.C,v 1.9 2000/07/26 16:49:47 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector3 v0, v1, v2, v3, v4;
Plane3 p, p2;

String filename;
using std::ofstream;
using std::ios;

CHECK(TPlane3::BALL_CREATE(TPlane3<T>))
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

CHECK(TPlane3();)
  Plane3* p;
	p = new Plane3();
	TEST_NOT_EQUAL(0, p)
RESULT		

CHECK(~TPlane3();)
  Plane3* p;
	p = new Plane3();
	delete p;
RESULT		

CHECK(TPlane3::bool operator == (const TPlane3& plane) const )
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

CHECK(TPlane3::::bool operator != (const TPlane3& plane) const )
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

CHECK(TPlane3::void get(TPlane3 &plane, bool /* deep */ = true) const)
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	p.get(p2);
	TEST_EQUAL(p, p2)
RESULT

CHECK(TPlane3::void set(TPlane3 &plane, bool /* deep */ = true) const)
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = Plane3(v3, v4);
	p.set(p2);
	TEST_EQUAL(p, p2)
RESULT

CHECK(TPlane3::void set(const TPlane3<T>& point, const TPlane3<T>& normal))
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3();
	p.set(v3, v4);
	p2 = Plane3(v3, v4);
	TEST_EQUAL(p, p2)
RESULT

CHECK(TPlane3::TPlane3& operator = (const TPlane3 &plane))
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p2 = p;
	TEST_EQUAL(p, p2)
RESULT

CHECK(TPlane3::void get(TVector3<T>& point, TVector3<T>& normal) const)
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(4.0, 5.0, 5.0);
	p = Plane3(v1, v2);
	p.get(v3, v4);
	TEST_EQUAL(v1, v3)
	TEST_EQUAL(v2, v4)
RESULT

CHECK(TPlane3::bool has(const TVector3<T>& point) const)
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	v3 = Vector3(1.0, 2.0, 3.0);
	v4 = Vector3(111.0, 211.0, 113.0);
	TEST_EQUAL(p.has(v3), true)
	TEST_EQUAL(p.has(v4), false)
RESULT

CHECK(TPlane3::bool has(const TLine3<T>& line) const)
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

CHECK(TPlane3<T>::swap(TPlane3<T>& plane))
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

CHECK(TPlane3::normalize())
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

CHECK(hessify())
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

CHECK(isValid() const )
	TEST_EQUAL(p.isValid(), true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TPlane3& plane))
	std::ifstream instr("data/Plane3_test2.txt");
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
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << p;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Plane3_test2.txt", false)
RESULT

CHECK(TPlane3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	p = Plane3(v1, v2);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	p.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Plane3_test.txt", true)
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
