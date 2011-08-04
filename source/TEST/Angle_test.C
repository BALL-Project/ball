// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#	include <BALL/MATHS/angle.h>
///////////////////////////

START_TEST(Angle)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
using std::ofstream;
using std::ios;
PRECISION(1E-5)

Angle* angle_ptr = 0;
CHECK(TAngle() throw())
	angle_ptr = new Angle;
	TEST_NOT_EQUAL(angle_ptr, 0)
RESULT

CHECK(~TAngle() throw())
	delete angle_ptr;
RESULT		

CHECK(TAngle(const T& new_value, bool radian = true) throw())
	Angle a = Angle(0.5);
	TEST_REAL_EQUAL(a, (float)0.5)  
	Angle b = Angle(180.0, false);
	TEST_REAL_EQUAL(b, (float)3.141592)  
RESULT

CHECK(TAngle(const TAngle& angle) throw())
	Angle a = Angle(0.5),b;
	b = Angle(a);
	TEST_REAL_EQUAL(a, b )  
RESULT


Angle a = Angle(0.5), b;

CHECK(void clear() throw())
	Angle a = Angle(0.5);
	a.clear();
	TEST_REAL_EQUAL(a, 0)  
RESULT

CHECK(bool operator == (const TAngle& angle) const throw())
	b = Angle(a);
	TEST_EQUAL(a == b, true )  
	b = Angle(0.6);
	TEST_EQUAL(a == b, false )  
RESULT

CHECK(bool operator == (const T& val) const throw())
	b = Angle(a);
	TEST_EQUAL(b == (float)0.5, true)  
	b = Angle(0.6);
	TEST_EQUAL(b == (float)0.5, false)
RESULT

CHECK(TAngle operator * (const T& val, const TAngle<T>& angle))
	b =	2.0 * a;
	TEST_REAL_EQUAL(1.0, b )  
RESULT

CHECK(TAngle operator + (const T& val, const TAngle<T>& angle))
	b =	2.0 + a;
	TEST_REAL_EQUAL(2.5, b )  
RESULT

CHECK(TAngle operator - (const T& val, const TAngle<T>& angle))
	b =	2.0 - a;
	TEST_REAL_EQUAL(1.5, b )  
RESULT

CHECK(BALL_CREATE(TAngle<T>))
	Angle a(0.5);
	Angle* v_ptr = (Angle*)a.create(false, true);
	TEST_REAL_EQUAL(v_ptr->value, 0)
	delete v_ptr;
	v_ptr = (Angle*)a.create();
	TEST_REAL_EQUAL(v_ptr->value, 0.5)
	delete v_ptr;
RESULT

CHECK(void swap(TAngle& angle) throw())
	Angle a1, a2;
	a1 = Angle(1.0);
	a2 = Angle(2.0);
	a1.swap(a2);
	TEST_REAL_EQUAL(a1, (float)2.0)  
	TEST_REAL_EQUAL(a2, (float)1.0)  
RESULT

CHECK(void set(const T& new_value, bool radian = true) throw())
	b.set(0.1);
	TEST_REAL_EQUAL(b, (float)0.1)  
RESULT

CHECK(void set(const TAngle& angle) throw())
	b.set(a);
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle& operator = (const TAngle& angle) throw())
	b.set(21.0);
	b = a;
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle& operator = (const T& new_value) throw())
	b.set(21.0);
	b = 0.2;
	TEST_REAL_EQUAL(0.2, b )  
RESULT

CHECK(void get(TAngle& angle) const throw())
	b.set(21.0);
	a.get(b);
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(void get(T& val, bool radian = true) const throw())
	float x;
	a.get(x);
	TEST_REAL_EQUAL(x, 0.5 )  
RESULT

CHECK(operator T () const throw())
	b = 3.1;
	TEST_REAL_EQUAL(3.1, (float)b)
RESULT

CHECK(T toRadian() const throw())
	b = 3.1;
	TEST_REAL_EQUAL(b, b.toRadian())
RESULT

CHECK(static T toRadian(const T& degree) throw())
	TEST_REAL_EQUAL((Constants::PI / 180.0 * 3.1), b.toRadian(3.1))
RESULT

CHECK(T toDegree() const throw())
	b = 3.1;
	TEST_REAL_EQUAL((180.0 / Constants::PI * 3.1),b.toDegree())
RESULT

CHECK(static T toDegree(const T& radian) throw())
	TEST_REAL_EQUAL((180.0 / Constants::PI * 3.1),b.toDegree(3.1))
RESULT

CHECK(void normalize(Range range) throw())
	b = 35.4;
	b.normalize(Angle::RANGE__UNLIMITED);
	TEST_REAL_EQUAL(35.4, b )  
	b = 35.4;
	b.normalize(Angle::RANGE__UNSIGNED);
	TEST_REAL_EQUAL(35.4 - 5 * (Constants::PI * 2), b )
	b = 35.4;
	b.normalize(Angle::RANGE__SIGNED);
	TEST_REAL_EQUAL(35.4 - 6 * (Constants::PI * 2), b )
RESULT

CHECK(void negate() throw())
	b = 35.4;
	b.negate();
	TEST_REAL_EQUAL(-35.4, b )
RESULT

CHECK(TAngle operator + () const throw())
	b = 35.4;
	b = + b;
	TEST_REAL_EQUAL(35.4, b )
RESULT

CHECK(TAngle operator - () const throw())
	b = 35.4;
	b = - b;
	TEST_REAL_EQUAL(-35.4, b )
RESULT

CHECK(TAngle& operator += (const TAngle& angle) throw())
	b = Angle(0);
	b+=a;
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle& operator += (const T& val) throw())
	b = Angle(0);
	b+=2;
	TEST_REAL_EQUAL(2, b )  
RESULT

CHECK(TAngle operator + (const TAngle& angle) throw())
	b = Angle(0);
	b = 1 + a;
	TEST_REAL_EQUAL(1.5, b)  
RESULT

CHECK(TAngle operator + (const T& val) throw())
	b = Angle(0);
	b = a + (float)1.0;
	TEST_REAL_EQUAL((float)1.5, b)  
RESULT

CHECK(TAngle& operator -= (const TAngle& angle) throw())
	b = Angle(0);
	b -= a;
	TEST_REAL_EQUAL(-0.5, b )  
RESULT

CHECK(TAngle& operator -= (const T& val) throw())
	b = Angle(0);
	b -= 1;
	TEST_REAL_EQUAL(-1, b )  
RESULT

CHECK(TAngle operator - (const TAngle& angle) throw())
	b = Angle(0);
	b = 2 - a;
	TEST_REAL_EQUAL(1.5, b )
RESULT

CHECK(TAngle operator - (const T& val) throw())
	b = Angle(0);
	b = a - (float)0.5;
	TEST_REAL_EQUAL(0, b)
RESULT

CHECK(TAngle& operator *= (const TAngle& angle) throw())
	b = Angle(1);
	b *= a;
	TEST_REAL_EQUAL(0.5, b )
RESULT

CHECK(TAngle& operator *= (const T& val) throw())
	b = Angle(1);
	b *= 5;
	TEST_REAL_EQUAL(5, b )
RESULT

CHECK(TAngle operator * (const T& value) throw())
	b = Angle(1);
	b = b * (float)5.0;
	TEST_REAL_EQUAL((float)5.0, b)
RESULT

CHECK(TAngle& operator /= (const TAngle& angle) throw(Exception::DivisionByZero))
	b = Angle(1);
	b /= a;
	TEST_REAL_EQUAL(2, b )
RESULT

CHECK(TAngle& operator /= (const T& val) throw(Exception::DivisionByZero))
	b = Angle(1);
	b /= 0.5;
	TEST_REAL_EQUAL(2, b )
RESULT

CHECK(TAngle operator / (const TAngle& val) throw(Exception::DivisionByZero))
	b = Angle(1);
	Angle denom (0.5);
	b = b / denom;
	TEST_REAL_EQUAL(2, b)
	denom = 0.0;
	TEST_EXCEPTION(Exception::DivisionByZero, b / denom) 
RESULT

CHECK(bool operator != (const TAngle& angle) const throw())
	b = Angle(1);
	TEST_EQUAL(a != b, true)
	b = a;
	TEST_EQUAL(a != b, false )
RESULT

CHECK(bool operator != (const T& val) const throw())
	b = Angle(1);
	TEST_EQUAL(b != (float)1.0, false)
	b = Angle(2);
	TEST_EQUAL(b != (float)1.0, true)
RESULT

CHECK(bool operator < (const TAngle& angle) const throw())
	b = Angle(1);
	TEST_EQUAL(b < a, false)
	TEST_EQUAL(a < b, true)
RESULT

CHECK(bool operator < (const T& val) const throw())
	b = Angle(1);
	TEST_EQUAL(b < (float)1.0, false)
	TEST_EQUAL(b < (float)2.0, true)
RESULT

CHECK(bool operator <= (const TAngle& angle) const throw())
	b = Angle(0);
	TEST_EQUAL(b <= a, true)
	b = Angle(3);
	TEST_EQUAL(b <= a, false)
RESULT

CHECK(bool operator >= (const TAngle& angle) const throw())
	b = Angle(4);
	TEST_EQUAL(b >= a, true)
	b = Angle(0);
	TEST_EQUAL(b >= a, false)
RESULT

CHECK(bool operator > (const TAngle& angle) const throw())
	b = Angle(1);
	TEST_EQUAL(b > a, true)
	b = Angle(0);
	TEST_EQUAL(b > a, false)
RESULT

CHECK(bool isEquivalent(TAngle angle) const throw())
	Angle a1(0.12);
	Angle a2(0.12);
	a1.normalize(Angle::RANGE__UNLIMITED);
	a2.normalize(Angle::RANGE__UNLIMITED);
	TEST_EQUAL(a1.isEquivalent(a2), true )
	a2 = 123;
	TEST_EQUAL(a1.isEquivalent(a2), false )
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(a.isValid(), true )
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Angle a(0.5);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	a.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Angle_test.txt))
RESULT

CHECK(std::istream& operator >> (std::istream& s, TAngle<T>& angle) throw())
	std::ifstream instr(BALL_TEST_DATA_PATH(Angle_test2.txt));
	Angle a = Angle();
	instr >> a;
	instr.close();
	TEST_REAL_EQUAL(a.value, 1.0)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TAngle<T>& angle) throw())
	Angle a(1.0);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << a;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Angle_test2.txt))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
