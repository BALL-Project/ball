// $Id: Angle_test.C,v 1.2 2000/02/27 20:36:19 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#	include <BALL/MATHS/angle.h>

///////////////////////////

START_TEST(class_name, "$Id: Angle_test.C,v 1.2 2000/02/27 20:36:19 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String filename;
using std::ofstream;
using std::ios;

//line
CHECK(TAngle::TAngle())
  Angle* a;
	a = new Angle();
	TEST_NOT_EQUAL(0, a)
RESULT

//line
CHECK(~TAngle();)
  Angle* a;
	a = new Angle();
	delete a;
RESULT		

//line 293: method TAngle::TAngle(const T& new_value, bool radian)
CHECK(TAngle::TAngle(const T& new_value, bool radian))
	Angle a = Angle(0.5);
	TEST_REAL_EQUAL(a, 0.5 )  
RESULT

//line 287: method TAngle::TAngle(const TAngle& angle, bool /* deep */)
CHECK(TAngle::TAngle(const TAngle& angle, bool /* deep */))
	Angle a = Angle(0.5),b;
	b = Angle(a);
	TEST_REAL_EQUAL(a, b )  
RESULT


Angle a = Angle(0.5), b;


//line 234: method TAngle::bool operator == (const TAngle& angle) const 
CHECK(TAngle::bool operator == (const TAngle& angle) const )
	b = Angle(a);
	TEST_EQUAL(a == b, true )  
	b = Angle(0.6);
	TEST_EQUAL(a == b, false )  
RESULT


//line 236: method TAngle::bool operator == (const T& val) const 
CHECK(TAngle::bool operator == (const T& val) const )
	b = Angle(a);
	TEST_EQUAL(b == 0.5, true )  
	b = Angle(0.6);
	TEST_EQUAL(b == 0.5, false )
RESULT


//line 27: method TAngle operator * (const T& val, const TAngle<T>& angle)
CHECK(TAngle operator * (const T& val, const TAngle<T>& angle))
	b =	2.0 * a;
	TEST_REAL_EQUAL(1.0, b )  
RESULT


//line 31: method TAngle operator + (const T& val, const TAngle<T>& angle)
CHECK(TAngle operator + (const T& val, const TAngle<T>& angle))
	b =	2.0 + a;
	TEST_REAL_EQUAL(2.5, b )  
RESULT


//line 35: method TAngle operator - (const T& val, const TAngle<T>& angle)
CHECK(TAngle operator - (const T& val, const TAngle<T>& angle))
	b =	2.0 - a;
	TEST_REAL_EQUAL(1.5, b )  
RESULT


//line 54: method TAngle::BALL_CREATE(TAngle<T>)
CHECK(TVector3::BALL_CREATE(TVector3<T>))
	Angle a(0.5);
	Angle* v_ptr = (Angle*)a.create(false, true);
	TEST_REAL_EQUAL(v_ptr->value, 0)
	delete v_ptr;
	v_ptr = (Angle*)a.create();
	TEST_REAL_EQUAL(v_ptr->value, 0.5)
	delete v_ptr;
RESULT


//line 109: method TAngle::swap(TAngle& angle)
CHECK(TAngle::swap(TAngle& angle))
	Angle a1, a2;
	a1 = Angle(1.0);
	a2 = Angle(2.0);
	a1.swap(a2);
	TEST_REAL_EQUAL(a1, 2.0 )  
	TEST_REAL_EQUAL(a2, 1.0 )  
RESULT


//line 117: method TAngle::set(const T& new_value, bool radian = true)
CHECK(TAngle::set(const T& new_value, bool radian = true))
	b.set(0.1);
	TEST_REAL_EQUAL(b, 0.1 )  
RESULT


//line 123: method TAngle::set(const TAngle& angle, bool deep = true)
CHECK(TAngle::set(const TAngle& angle, bool deep = true))
	b.set(a);
	TEST_REAL_EQUAL(a, b )  
RESULT


//line 128: method TAngle::TAngle& operator = (const TAngle& angle)
CHECK(TAngle::TAngle& operator = (const TAngle& angle))
	b.set(21.0);
	b = a;
	TEST_REAL_EQUAL(a, b )  
RESULT


//line 135: method TAngle::TAngle& operator = (const T& new_value)
CHECK(TAngle::TAngle& operator = (const T& new_value))
	b.set(21.0);
	b = 0.2;
	TEST_REAL_EQUAL(0.2, b )  
RESULT


//line 139: method TAngle::get(TAngle& angle, bool deep = true) const 
CHECK(TAngle::get(TAngle& angle, bool deep = true) const )
	b.set(21.0);
	a.get(b);
	TEST_REAL_EQUAL(a, b )  
RESULT


//line 143: method TAngle::get(T& val, bool radian = true) const 
CHECK(TAngle::get(T& val, bool radian = true) const )
	float x;
	a.get(x);
	TEST_REAL_EQUAL(x, 0.5 )  
RESULT


//line 153: method TAngle:: operator T () const 
CHECK(TAngle:: operator T () const )
		b = 3.1;
		TEST_REAL_EQUAL(3.1, (float) b)
RESULT


//line 157: method TAngle::toRadian() const 
CHECK(TAngle::toRadian() const )
	b = 3.1;
	TEST_REAL_EQUAL(3.1, b.toRadian() )
RESULT


//line 161: method TAngle::toRadian(const T& degree)
CHECK(TAngle::toRadian(const T& degree))
	TEST_REAL_EQUAL((Constants::PI / 180.0 * 3.1), b.toRadian(3.1))
RESULT


//line 165: method TAngle::toDegree() const 
CHECK(TAngle::toDegree() const )
	b = 1;
	TEST_REAL_EQUAL(57.2957795130823209, b.toDegree())
RESULT


//line 169: method TAngle::toDegree(const T& radian)
CHECK(TAngle::toDegree(const T& radian))
	TEST_REAL_EQUAL(57.2957795130823209, b.toDegree(1) )
RESULT


//line 176: method TAngle::getTorsionAngle(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz, const T& cx, const T& cy, const T& cz, const T& dx, const T& dy, const T& dz)
CHECK(TAngle::getTorsionAngle(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz, const T& cx, const T& cy, const T& cz, const T& dx, const T& dy, const T& dz))
	b.getTorsionAngle(0, 10, 20,  100, 210, 320,  50, 110, 170,  200, 410, 620);
//b.toDegree()
	TEST_REAL_EQUAL(b, 0)
RESULT


//line 179: method TAngle::normalize(Range range)
CHECK(TAngle::normalize(Range range))
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


//line 182: method TAngle::negate()
CHECK(TAngle::negate())
	b = 35.4;
	b.negate();
	TEST_REAL_EQUAL(-35.4, b )
RESULT


//line 185: method TAngle::TAngle operator + () const 
CHECK(TAngle::TAngle operator + () const )
	b = 35.4;
	b = + b;
	TEST_REAL_EQUAL(35.4, b )
RESULT


//line 188: method TAngle::TAngle operator - () const 
CHECK(TAngle::TAngle operator - () const )
	b = 35.4;
	b = - b;
	TEST_REAL_EQUAL(-35.4, b )
RESULT


//line 191: method TAngle::TAngle& operator += (const TAngle& angle)
CHECK(TAngle::TAngle& operator += (const TAngle& angle))
	b = Angle(0);
	b+=a;
	TEST_REAL_EQUAL(a, b )  
RESULT


//line 194: method TAngle::TAngle& operator += (const T& value)
CHECK(TAngle::TAngle& operator += (const T& value))
	b = Angle(0);
	b+=2;
	TEST_REAL_EQUAL(2, b )  
RESULT


//line 197: method TAngle::TAngle operator +      (const TAngle& angle)
CHECK(TAngle::TAngle operator + (const TAngle& angle))
	b = Angle(0);
	b = 1 + a;
	TEST_REAL_EQUAL(1.5, b )  
RESULT


//line 200: method TAngle::TAngle operator + (const T& val)
CHECK(TAngle::TAngle operator + (const T& val))
	b = Angle(0);
	b = a + 1;
	TEST_REAL_EQUAL(1.5, b )  
RESULT


//line 203: method TAngle::TAngle& operator -= (const TAngle& angle)
CHECK(TAngle::TAngle& operator -= (const TAngle& angle))
	b = Angle(0);
	b -= a;
	TEST_REAL_EQUAL(-0.5, b )  
RESULT


//line 206: method TAngle::TAngle& operator -= (const T& val)
CHECK(TAngle::TAngle& operator -= (const T& val))
	b = Angle(0);
	b -= 1;
	TEST_REAL_EQUAL(-1, b )  
RESULT


//line 209: method TAngle::TAngle operator - (const TAngle& angle)
CHECK(TAngle::TAngle operator - (const TAngle& angle))
	b = Angle(0);
	b = 2 - a;
	TEST_REAL_EQUAL(1.5, b )
RESULT


//line 212: method TAngle::TAngle operator - (const T& val)
CHECK(TAngle::TAngle operator - (const T& val))
	b = Angle(0);
	b = a - 0.5;
	TEST_REAL_EQUAL(0, b )
RESULT


//line 215: method TAngle::TAngle& operator *= (const TAngle& angle)
CHECK(TAngle::TAngle& operator *= (const TAngle& angle))
	b = Angle(1);
	b *= a;
	TEST_REAL_EQUAL(0.5, b )
RESULT


//line 218: method TAngle::TAngle& operator *=    (const T& val)
CHECK(TAngle::TAngle& operator *= (const T& val))
	b = Angle(1);
	b *= 5;
	TEST_REAL_EQUAL(5, b )
RESULT


//line 221: method TAngle::TAngle operator * (const T& value)
CHECK(TAngle::TAngle operator * (const T& value))
	b = Angle(1);
	b = b * 5;
	TEST_REAL_EQUAL(5, b )
RESULT


//line 223: method TAngle::TAngle& operator /= (const TAngle& angle)
CHECK(TAngle::TAngle& operator /= (const TAngle& angle))
	b = Angle(1);
	b /= a;
	TEST_REAL_EQUAL(2, b )
RESULT


//line 225: method TAngle::TAngle& operator /=    (const T& val)
CHECK(TAngle::TAngle& operator /=       (const T& val))
	b = Angle(1);
	b /= 0.5;
	TEST_REAL_EQUAL(2, b )
RESULT


//line 227: method TAngle::TAngle operator /      (const T& val)
CHECK(TAngle::TAngle operator / (const T& val))
	b = Angle(1);
	b = b / 0.5;
	TEST_REAL_EQUAL(2, b )
RESULT


//line 238: method TAngle::bool operator != (const TAngle& angle) const 
CHECK(TAngle::bool operator != (const TAngle& angle) const )
	b = Angle(1);
	TEST_EQUAL(a != b, true )
	b = a;
	TEST_EQUAL(a != b, false )
RESULT


//line 240: method TAngle::bool operator !=       (const T& val) const 
CHECK(TAngle::bool operator !=  (const T& val) const )
	b = Angle(1);
	TEST_EQUAL(b != 1, false )
	b = Angle(2);
	TEST_EQUAL(b != 1, true )
RESULT


//line 242: method TAngle::bool operator <        (const TAngle& angle) const 
CHECK(TAngle::bool operator <   (const TAngle& angle) const )
	b = Angle(1);
	TEST_EQUAL(b<a, false )
	TEST_EQUAL(a<b, true )
RESULT


//line 244: method TAngle::bool operator <        (const T& val) const 
CHECK(TAngle::bool operator <   (const T& val) const )
	b = Angle(1);
	TEST_EQUAL(b<1, false )
	TEST_EQUAL(b<2, true )
RESULT


//line 246: method TAngle::bool operator <= (const TAngle& angle) const 
CHECK(TAngle::bool operator <= (const TAngle& angle) const )
	b = Angle(0);
	TEST_EQUAL(b<=a, true )
	b = Angle(3);
	TEST_EQUAL(b<=a, false )
RESULT


//line 248: method TAngle::bool operator >=       (const TAngle& angle) const 
CHECK(TAngle::bool operator >=  (const TAngle& angle) const )
	b = Angle(4);
	TEST_EQUAL(b>=a, true )
	b = Angle(0);
	TEST_EQUAL(b>=a, false )
RESULT


//line 250: method TAngle::bool operator > (const TAngle& angle) const 
CHECK(TAngle::bool operator > (const TAngle& angle) const )
	b = Angle(1);
	TEST_EQUAL(b>a, true )
	b = Angle(0);
	TEST_EQUAL(b>a, false )
RESULT


//line 252: method TAngle::isEquivalent(TAngle angle) const 
CHECK(TAngle::isEquivalent(TAngle angle) const )
	Angle a1 = 0.12, a2 = 0.12;
	a1.normalize(Angle::RANGE__UNLIMITED);
	a2.normalize(Angle::RANGE__UNLIMITED);
	TEST_EQUAL(a1.isEquivalent(a2), true )
	a2 = 123;
	TEST_EQUAL(a1.isEquivalent(a2), false )
RESULT


//line 262: method TAngle::isValid() const 
CHECK(TAngle::isValid() const )
	TEST_EQUAL(a.isValid(), true )
RESULT


//line 266: method TAngle::dump(std::ostream& s = std::cout, Size depth = 0) const 
CHECK(TVector3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Angle a(0.5);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	a.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Angle_test.txt", true)
RESULT

/*
//line 
CHECK(std::istream& operator >> (std::istream& s, TAngle<T>& angle))
	std::ifstream instr("data/Angle_test2.txt");
	Angle a(0.4);
	instr >> a;
	instr.close();
	TEST_REAL_EQUAL(a.value, 0.5)
RESULT


//line 
NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TAngle<T>& angle))
	Angle a(0.5);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << a;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Angle_test2.txt", false)
RESULT
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST