// $Id: Angle_test.C,v 1.14 2000/07/26 16:49:46 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#	include <BALL/MATHS/angle.h>
///////////////////////////

START_TEST(class_name, "$Id: Angle_test.C,v 1.14 2000/07/26 16:49:46 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
using std::ofstream;
using std::ios;
PRECISION(1E-5)

CHECK(TAngle::TAngle())
  Angle* a;
	a = new Angle();
	TEST_NOT_EQUAL(0, a)
RESULT

CHECK(~TAngle();)
  Angle* a;
	a = new Angle();
	delete a;
RESULT		

CHECK(TAngle::TAngle(const T& new_value, bool radian))
	Angle a = Angle(0.5);
	TEST_REAL_EQUAL(a, (float)0.5)  
RESULT

CHECK(TAngle::TAngle(const TAngle& angle, bool /* deep */))
	Angle a = Angle(0.5),b;
	b = Angle(a);
	TEST_REAL_EQUAL(a, b )  
RESULT


Angle a = Angle(0.5), b;


CHECK(TAngle::bool operator == (const TAngle& angle) const )
	b = Angle(a);
	TEST_EQUAL(a == b, true )  
	b = Angle(0.6);
	TEST_EQUAL(a == b, false )  
RESULT

CHECK(TAngle::bool operator == (const T& val) const )
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

CHECK(TAngle::BALL_CREATE(TAngle<T>))
	Angle a(0.5);
	Angle* v_ptr = (Angle*)a.create(false, true);
	TEST_REAL_EQUAL(v_ptr->value, 0)
	delete v_ptr;
	v_ptr = (Angle*)a.create();
	TEST_REAL_EQUAL(v_ptr->value, 0.5)
	delete v_ptr;
RESULT

CHECK(TAngle::swap(TAngle& angle))
	Angle a1, a2;
	a1 = Angle(1.0);
	a2 = Angle(2.0);
	a1.swap(a2);
	TEST_REAL_EQUAL(a1, (float)2.0)  
	TEST_REAL_EQUAL(a2, (float)1.0)  
RESULT

CHECK(TAngle::set(const T& new_value, bool radian = true))
	b.set(0.1);
	TEST_REAL_EQUAL(b, (float)0.1)  
RESULT

CHECK(TAngle::set(const TAngle& angle, bool deep = true))
	b.set(a);
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle::TAngle& operator = (const TAngle& angle))
	b.set(21.0);
	b = a;
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle::TAngle& operator = (const T& new_value))
	b.set(21.0);
	b = 0.2;
	TEST_REAL_EQUAL(0.2, b )  
RESULT

CHECK(TAngle::get(TAngle& angle, bool deep = true) const )
	b.set(21.0);
	a.get(b);
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle::get(T& val, bool radian = true) const )
	float x;
	a.get(x);
	TEST_REAL_EQUAL(x, 0.5 )  
RESULT

CHECK(TAngle:: operator T () const )
		b = 3.1;
		TEST_REAL_EQUAL(3.1, (float) b)
RESULT

CHECK(TAngle::toRadian() const )
	b = 3.1;
	TEST_REAL_EQUAL(b, b.toRadian())
RESULT

CHECK(TAngle::toRadian(const T& degree))
	TEST_REAL_EQUAL((Constants::PI / 180.0 * 3.1), b.toRadian(3.1))
RESULT

CHECK(TAngle::toDegree() const )
	b = 3.1;
	TEST_REAL_EQUAL((180.0 / Constants::PI * 3.1),b.toDegree())
RESULT

CHECK(TAngle::toDegree(const T& radian))
	TEST_REAL_EQUAL((180.0 / Constants::PI * 3.1),b.toDegree(3.1))
RESULT

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

CHECK(TAngle::negate())
	b = 35.4;
	b.negate();
	TEST_REAL_EQUAL(-35.4, b )
RESULT

CHECK(TAngle::TAngle operator + () const )
	b = 35.4;
	b = + b;
	TEST_REAL_EQUAL(35.4, b )
RESULT

CHECK(TAngle::TAngle operator - () const )
	b = 35.4;
	b = - b;
	TEST_REAL_EQUAL(-35.4, b )
RESULT

CHECK(TAngle::TAngle& operator += (const TAngle& angle))
	b = Angle(0);
	b+=a;
	TEST_REAL_EQUAL(a, b )  
RESULT

CHECK(TAngle::TAngle& operator += (const T& value))
	b = Angle(0);
	b+=2;
	TEST_REAL_EQUAL(2, b )  
RESULT

CHECK(TAngle::TAngle operator + (const TAngle& angle))
	b = Angle(0);
	b = 1 + a;
	TEST_REAL_EQUAL(1.5, b)  
RESULT

CHECK(TAngle::TAngle operator + (const T& val))
	b = Angle(0);
	b = a + (float)1.0;
	TEST_REAL_EQUAL((float)1.5, b)  
RESULT

CHECK(TAngle::TAngle& operator -= (const TAngle& angle))
	b = Angle(0);
	b -= a;
	TEST_REAL_EQUAL(-0.5, b )  
RESULT

CHECK(TAngle::TAngle& operator -= (const T& val))
	b = Angle(0);
	b -= 1;
	TEST_REAL_EQUAL(-1, b )  
RESULT

CHECK(TAngle::TAngle operator - (const TAngle& angle))
	b = Angle(0);
	b = 2 - a;
	TEST_REAL_EQUAL(1.5, b )
RESULT

CHECK(TAngle::TAngle operator - (const T& val))
	b = Angle(0);
	b = a - (float)0.5;
	TEST_REAL_EQUAL(0, b)
RESULT

CHECK(TAngle::TAngle& operator *= (const TAngle& angle))
	b = Angle(1);
	b *= a;
	TEST_REAL_EQUAL(0.5, b )
RESULT

CHECK(TAngle::TAngle& operator *= (const T& val))
	b = Angle(1);
	b *= 5;
	TEST_REAL_EQUAL(5, b )
RESULT

CHECK(TAngle::TAngle operator * (const T& value))
	b = Angle(1);
	b = b * (float)5.0;
	TEST_REAL_EQUAL((float)5.0, b)
RESULT

CHECK(TAngle::TAngle& operator /= (const TAngle& angle))
	b = Angle(1);
	b /= a;
	TEST_REAL_EQUAL(2, b )
RESULT

CHECK(TAngle::TAngle& operator /=       (const T& val))
	b = Angle(1);
	b /= 0.5;
	TEST_REAL_EQUAL(2, b )
RESULT

CHECK(TAngle::TAngle operator / (const T& val))
	b = Angle(1);
	b = b / (float)0.5;
	TEST_REAL_EQUAL(2, b)
RESULT

CHECK(TAngle::bool operator != (const TAngle& angle) const )
	b = Angle(1);
	TEST_EQUAL(a != b, true)
	b = a;
	TEST_EQUAL(a != b, false )
RESULT

CHECK(TAngle::bool operator !=  (const T& val) const )
	b = Angle(1);
	TEST_EQUAL(b != (float)1.0, false)
	b = Angle(2);
	TEST_EQUAL(b != (float)1.0, true)
RESULT

CHECK(TAngle::bool operator <   (const TAngle& angle) const )
	b = Angle(1);
	TEST_EQUAL(b < a, false)
	TEST_EQUAL(a < b, true)
RESULT

CHECK(TAngle::bool operator <   (const T& val) const )
	b = Angle(1);
	TEST_EQUAL(b < (float)1.0, false)
	TEST_EQUAL(b < (float)2.0, true)
RESULT

CHECK(TAngle::bool operator <= (const TAngle& angle) const )
	b = Angle(0);
	TEST_EQUAL(b <= a, true)
	b = Angle(3);
	TEST_EQUAL(b <= a, false)
RESULT

CHECK(TAngle::bool operator >=  (const TAngle& angle) const )
	b = Angle(4);
	TEST_EQUAL(b >= a, true)
	b = Angle(0);
	TEST_EQUAL(b >= a, false)
RESULT

CHECK(TAngle::bool operator > (const TAngle& angle) const )
	b = Angle(1);
	TEST_EQUAL(b > a, true)
	b = Angle(0);
	TEST_EQUAL(b > a, false)
RESULT

CHECK(TAngle::isEquivalent(TAngle angle) const )
	Angle a1(0.12);
	Angle a2(0.12);
	a1.normalize(Angle::RANGE__UNLIMITED);
	a2.normalize(Angle::RANGE__UNLIMITED);
	TEST_EQUAL(a1.isEquivalent(a2), true )
	a2 = 123;
	TEST_EQUAL(a1.isEquivalent(a2), false )
RESULT

CHECK(TAngle::isValid() const )
	TEST_EQUAL(a.isValid(), true )
RESULT

CHECK(TAngle::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Angle a(0.5);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	a.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Angle_test.txt", true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TAngle<T>& angle))
	std::ifstream instr("data/Angle_test2.txt");
	Angle a = Angle();
	instr >> a;
	instr.close();
	TEST_REAL_EQUAL(a.value, 1.0)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TAngle<T>& angle))
	Angle a(1.0);
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << a;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Angle_test2.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
