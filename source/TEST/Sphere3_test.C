// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#	include <BALL/MATHS/sphere3.h>
#	include <BALL/MATHS/vector3.h>
///////////////////////////

START_TEST(Sphere3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector3 const v = Vector3(1, 2, 3);
Vector3 v2;	
float radius;

CHECK(BALL_CREATE(TSphere3))
	Sphere3 s(v, 4.0);
	Sphere3* s_ptr = (Sphere3*)s.create(false, true);
	TEST_EQUAL(s_ptr->p, v2)
	TEST_REAL_EQUAL(s_ptr->radius, 0)
	delete s_ptr;
	s_ptr = (Sphere3*)s.create();
	TEST_EQUAL(s_ptr->p, v)
	TEST_REAL_EQUAL(s_ptr->radius, 4.0)
	delete s_ptr;
RESULT

Sphere3* sphere_ptr = 0;
CHECK(TSphere3() throw())
	sphere_ptr = new Sphere3;
	TEST_NOT_EQUAL(sphere_ptr, 0)
RESULT			

CHECK(~TSphere3() throw())
	delete sphere_ptr;
RESULT	

Sphere3 s, s1, s2;

CHECK(TSphere3(const TVector3<T>& point, const T& radius) throw())
	s = Sphere3(v, 4.0);
	TEST_EQUAL(s.p, v)
	TEST_REAL_EQUAL(s.radius, 4.0)
RESULT

CHECK(TSphere3(const TSphere3& sphere) throw())
	s = Sphere3(v, 4.0);
	s2 = Sphere3(s);
	TEST_EQUAL(s2.p, s.p)
	TEST_REAL_EQUAL(s2.radius, s.radius)
RESULT

CHECK(void swap(TSphere3& sphere) throw())
	v2  = Vector3(10.0, 20.0, 30.0);
	s2 = Sphere3(v2, 40.0);
	s = Sphere3(v, 4.0);
	Sphere3 s_copy = Sphere3(s);
	Sphere3 s2_copy = Sphere3(s2);
	s.swap(s2);
	TEST_EQUAL(s, s2_copy)
	TEST_EQUAL(s2, s_copy)
RESULT

CHECK(bool operator == (const TSphere3& sphere) const throw())
	s = Sphere3(v, 4.0);
	s2 = Sphere3(s);
	TEST_EQUAL(s == s2, true)
	s = Sphere3(v, 4.1);
	TEST_EQUAL(s == s2, false)
	v2  = Vector3(10.0, 20.0, 30.0);
	s = Sphere3(v2, 4);
	TEST_EQUAL(s == s2, false)
RESULT

CHECK(bool operator != (const TSphere3& sphere) const throw())
	s = Sphere3(v, 4.0);
	s2 = Sphere3(s);
	TEST_EQUAL(s != s2, false)
	s = Sphere3(v, 4.1);
	TEST_EQUAL(s != s2, true)
	v2  = Vector3(10.0, 20.0, 30.0);
	s = Sphere3(v2, 4);
	TEST_EQUAL(s != s2, true)
RESULT

CHECK(void set(const TSphere3& sphere) throw())
	s = Sphere3();
	s2 = Sphere3(v, 4.0);
	s.set(s2);
	TEST_EQUAL(s, s2)
RESULT

CHECK(void set(const TVector3<T>& point, const T& r) throw())
	s = Sphere3();
	s2 = Sphere3(v, 4.0);
	s.set(v, 4.0);
	TEST_EQUAL(s, s2)
RESULT

CHECK(TSphere3& operator = (const TSphere3& sphere) throw())
	s = Sphere3();
	s2 = Sphere3(v, 4.0);
	s = s2;
	TEST_EQUAL(s, s2)
RESULT

CHECK(void get(TSphere3& sphere) const throw())
	s = Sphere3(v, 4.0);
	s2 = Sphere3();
	s.get(s2);
	TEST_EQUAL(s, s2)
RESULT

CHECK(void get(TVector3<T>& point, T& r) const throw())
	s = Sphere3(v, 4.0);
	v2 = Vector3();
	s.get(v2, radius);
	TEST_EQUAL(s, s2)
	TEST_REAL_EQUAL(radius, 4.0)
RESULT

CHECK(bool has(const TVector3<T>& point, bool on_surface = false) const throw())
	s2 = Sphere3(v, 0.0);
	TEST_EQUAL(s2.has(v), true)

	s = Sphere3(v, 4.0);
	v2 = Vector3( 1.0, 2.0, 5.0);
	TEST_EQUAL(s.has(v2), true)

	v2 = Vector3( 1.0, 2.0, 11.0);
	TEST_EQUAL(s.has(v2), false)

	v2 = Vector3( 1, 2.0, 7.0);
	TEST_EQUAL(s.has(v2, true), true)

	v2 = Vector3( 1.0, 2.1, 3.0);
	TEST_EQUAL(s.has(v2, true), false)
RESULT

CHECK(bool isEmpty() const throw())
	s = Sphere3(v, 4.0);
	TEST_EQUAL(s.isEmpty(), false)
	s2 = Sphere3(v, 0);
	TEST_EQUAL(s2.isEmpty(), true)
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(s.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	s = Sphere3(v2, 4.0);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	s.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Sphere3_test.txt))
RESULT

CHECK(std::istream& operator >> (std::istream& s, TSphere3<T>& sphere))
	std::ifstream instr(BALL_TEST_DATA_PATH(Sphere3_test2.txt));
	s2 = Sphere3();
	s = Sphere3(v, 4.0);	
	instr >> s2;
	instr.close();
	TEST_EQUAL(s2, s)
RESULT

String filename;
NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TSphere3<T>& sphere))
	s = Sphere3(v, 4.0);	
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << s;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Sphere3_test2.txt))
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
