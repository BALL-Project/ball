// $Id: Sphere3_test.C,v 1.2 2000/03/03 11:54:50 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#	include <BALL/MATHS/sphere3.h>
#	include <BALL/MATHS/vector3.h>
///////////////////////////

START_TEST(class_name, "$Id: Sphere3_test.C,v 1.2 2000/03/03 11:54:50 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

Vector3 const v = Vector3(1, 2, 3);
Vector3 v2;	
float radius;

//line 41: method TSphere3::BALL_CREATE(TSphere3)
CHECK(TSphere3::BALL_CREATE(TSphere3))
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


CHECK(TSphere3();)
  Sphere3* s;
	s = new Sphere3();
	TEST_NOT_EQUAL(0, s)
RESULT			


CHECK(~TSphere3();)
  Sphere3* s;
	s = new Sphere3();
	delete s;
RESULT	

Sphere3 s, s1, s2;

//line
CHECK(TSphere3(const TVector3<T>& point, const T& radius))
	s = Sphere3(v, 4.0);
	TEST_EQUAL(s.p, v)
	TEST_REAL_EQUAL(s.radius, 4.0)
RESULT

//line
CHECK(TSphere3(const TSphere3& sphere3,bool /* deep */ = true))
	s = Sphere3(v, 4.0);
	s2 = Sphere3(s);
	TEST_EQUAL(s2.p, s.p)
	TEST_REAL_EQUAL(s2.radius, s.radius)
RESULT

CHECK(void swap(TSphere3& sphere3))
	v2  = Vector3(10.0, 20.0, 30.0);
	s2 = Sphere3(v2, 40.0);
	s = Sphere3(v, 4.0);
	Sphere3 s_copy = Sphere3(s);
	Sphere3 s2_copy = Sphere3(s2);
	s.swap(s2);
	TEST_EQUAL(s, s2_copy)
	TEST_EQUAL(s2, s_copy)
RESULT

//line 166: method TSphere3::bool operator == (const TSphere3& sphere3) const 
CHECK(TSphere3::bool operator == (const TSphere3& sphere3) const )
	s = Sphere3(v, 4.0);
	s2 = Sphere3(s);
	TEST_EQUAL(s == s2, true)
	s = Sphere3(v, 4.1);
	TEST_EQUAL(s == s2, false)
	v2  = Vector3(10.0, 20.0, 30.0);
	s = Sphere3(v2, 4);
	TEST_EQUAL(s == s2, false)
RESULT


//line 174: method bool operator != (const TSphere3& sphere3) const 
CHECK(bool operator != (const TSphere3& sphere3) const )
	s = Sphere3(v, 4.0);
	s2 = Sphere3(s);
	TEST_EQUAL(s != s2, false)
	s = Sphere3(v, 4.1);
	TEST_EQUAL(s != s2, true)
	v2  = Vector3(10.0, 20.0, 30.0);
	s = Sphere3(v2, 4);
	TEST_EQUAL(s != s2, true)
RESULT


CHECK(void set(const TSphere3& sphere3, bool /* deep */ = true))
	s = Sphere3();
	s2 = Sphere3(v, 4.0);
	s.set(s2);
	TEST_EQUAL(s, s2)
RESULT

CHECK(void set(const TVector3<T>& point, const T& radius))
	s = Sphere3();
	s2 = Sphere3(v, 4.0);
	s.set(v, 4.0);
	TEST_EQUAL(s, s2)
RESULT

CHECK(TSphere3 &operator =(const TSphere3& sphere3))
	s = Sphere3();
	s2 = Sphere3(v, 4.0);
	s = s2;
	TEST_EQUAL(s, s2)
RESULT

CHECK(void get(TSphere3& sphere3, bool /* deep */ = true) const)
	s = Sphere3(v, 4.0);
	s2 = Sphere3();
	s.get(s2);
	TEST_EQUAL(s, s2)
RESULT

CHECK(void get(TSphere3& sphere3, bool /* deep */ = true) const)
	s = Sphere3(v, 4.0);
	v2 = Vector3();
	s.get(v2, radius);
	TEST_EQUAL(s, s2)
	TEST_REAL_EQUAL(radius, 4.0)
RESULT

//line 185: method has(const TVector3<T>& point, bool on_surface = false) const 
CHECK(has(const TVector3<T>& point, bool on_surface = false) const )
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


//line 198: method isEmpty() const 
CHECK(isEmpty() const )
	s = Sphere3(v, 4.0);
	TEST_EQUAL(s.isEmpty(), false)
	s2 = Sphere3(v, 0);
	TEST_EQUAL(s2.isEmpty(), true)
RESULT


CHECK(isValid() const )
	TEST_EQUAL(s.isValid(), true)
RESULT

//line
CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const)
	s = Sphere3(v2, 4.0);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	s.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Sphere3_test.txt", true)
RESULT


//line 260: method std::istream& operator >> (std::istream& s, TSphere3<T>& sphere3)
CHECK(std::istream& operator >> (std::istream& s, TSphere3<T>& sphere3))
	std::ifstream instr("data/Sphere3_test2.txt");
	s2 = Sphere3();
	s = Sphere3(v, 4.0);	
	instr >> s2;
	instr.close();
	TEST_EQUAL(s2, s)
RESULT

String filename;
NEW_TMP_FILE(filename)
//line 275: method std::ostream& operator << (std::ostream& s, const TSphere3<T>& sphere3)
CHECK(std::ostream& operator << (std::ostream& s, const TSphere3<T>& sphere3))
	s = Sphere3(v, 4.0);	
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << s;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Sphere3_test2.txt", false)
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
