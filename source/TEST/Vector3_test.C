// $Id: Vector3_test.C,v 1.7 2000/02/20 21:55:30 oliver Exp $ #include
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/vector3.h>

///////////////////////////

START_TEST(TVector3, "$Id: Vector3_test.C,v 1.7 2000/02/20 21:55:30 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

//line
CHECK(TVector3();)
  Vector3* v;
	v = new Vector3();
	TEST_NOT_EQUAL(0, v)
RESULT								

//line
CHECK(~TVector3();)
  Vector3* v;
	v = new Vector3();
	delete v;
RESULT		

Vector3 v;
Vector3 v1;
Vector3 v2;
float x;

CHECK(TVector3::T& operator [] (Index index) const)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2.0)
	TEST_EQUAL(v[2], 3.0)
	TEST_EXCEPTION(Exception::IndexUnderflow, x == v[-1])
	TEST_EXCEPTION(Exception::IndexOverflow,  x == v[3])
RESULT


//line
CHECK(TVector3(const T* ptr);)
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3(arr);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EXCEPTION(Exception::NullPointer, v = Vector3((float*)0))
RESULT

//line
CHECK(TVector3(const T& x, const T& y, const T& z);)
	v = Vector3(1, 2, 3);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
RESULT

//line
CHECK(TVector3(const TVector3& vector, bool deep = true);)
	v2 = Vector3(1, 2, 3);
	v  = Vector3(v2);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
RESULT

//line 
CHECK(TVector3 operator + (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(1, 2, 3);
	v2 = Vector3(1, 2, 3);
	v  = v1 + v2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
RESULT

//line 
CHECK(TVector3 operator - (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(2, 3, 4);
	v2 = Vector3(1, 2, 3);
	v  = v1 - v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 1)
	TEST_EQUAL(v[2], 1)
RESULT

//line
CHECK(TVector3 operator * (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(2, 2, 2);
	v2 = Vector3(1, 2, 3);
	v  = v1 - v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 0)
	TEST_EQUAL(v[2], -1)
RESULT

//line 
CHECK(std::istream& operator >> (std::istream& s, TVector3<T>& vector))
	//std::ostream ostream;
	//v = Vector3(1, 2, 3);
	//ostream = << v;
	//TEST_EQUAL(ostream, << 1 << 2 << 3)  //BAUSTELLE
RESULT

//line 
CHECK(std::ostream& operator << (std::ostream& s, const TVector3<T>& vector))
	//std::istream instream;
	//instream >> 1 >> 2 >> 3 ;
	//v  = Vector3(instream);  //BAUSTELLE
	//TEST_EQUAL(v[0], 1)
	//TEST_EQUAL(v[1], 2)
	//TEST_EQUAL(v[2], 3)
RESULT

//line 
CHECK(TVector3::BALL_CREATE(TVector3<T>))
  //BAUSTELLE
RESULT

//line 
CHECK(TVector3::getLength() const )
	v = Vector3(4, 9, 16);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4 * 4 + 9 * 9 + 16 * 16))
RESULT

//line
CHECK(TVector3::getSquareLength() const )
	v = Vector3(1, 2, 3);
	TEST_REAL_EQUAL(v.getSquareLength(), 14)
RESULT

//line 
CHECK(TVector3::normalize())
	v = Vector3(4, 9, 16);
	v.normalize();
	float erg = sqrt (4 *4 + 9 * 9 + 16 * 16);
	TEST_REAL_EQUAL(v[0], 4/erg)
	TEST_REAL_EQUAL(v[1], 9/erg)
	TEST_REAL_EQUAL(v[2], 16/erg)
	v = Vector3(0, 0, 0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT

//line 
CHECK(TVector3::getZero())
	TEST_EQUAL(Vector3::getZero().x, 0)
	TEST_EQUAL(Vector3::getZero().y, 0)
	TEST_EQUAL(Vector3::getZero().z, 0)
RESULT

//line 
CHECK(TVector3::getUnit())
	TEST_EQUAL(Vector3::getUnit().x, 1)
	TEST_EQUAL(Vector3::getUnit().y, 1)
	TEST_EQUAL(Vector3::getUnit().z, 1)
RESULT

//line 
CHECK(TVector3::T& operator [] (Index index) )
	v = Vector3(1, 2, 3);
	v[0]=5;	v[1]=6;	v[2]=7;
	TEST_EQUAL(v[0], 5)
	TEST_EQUAL(v[1], 6)
	TEST_EQUAL(v[2], 7)
	TEST_EXCEPTION(Exception::IndexUnderflow, v[-1] = 5)
	TEST_EXCEPTION(Exception::IndexOverflow, v[3] = 5)
RESULT

//line 
CHECK(TVector3::TVector3 operator + () const )
	v2 = Vector3(1, 2, 3);
	v = + v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
RESULT

//line
CHECK(TVector3::TVector3 operator - () const )
	v2 = Vector3(1, 2, 3);
	v = + v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
RESULT

//line
CHECK(TVector3::TVector3& operator += (const TVector3& vector))
	v2 = Vector3(1, 2 ,3);
	v  = Vector3(1, 2, 3);
	v += v2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
RESULT

//line
CHECK(TVector3::TVector3& operator -= (const TVector3& vector))
	v2 = Vector3(1, 2, 3);
	v  = Vector3(4, 3, 2);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], -1)
RESULT

//line 
CHECK(TVector3::TVector3 operator * (const T& scalar))
	v  = Vector3(1, 2, 3);
	v = v * 2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
RESULT

//line
CHECK(TVector3::TVector3& operator *= (const T& scalar))
	v  = Vector3(1, 2, 3);
	v *= 2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
RESULT

//line
CHECK(TVector3::TVector3 operator / (const T& scalar))
	v  = Vector3(1, 2, 3);
	v = v / 0.5;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

//line
CHECK(TVector3::TVector3& operator /= (const T& scalar))
	v  = Vector3(1, 2, 3);
	v /= 0.5;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

//line
CHECK(TVector3::T operator * (const TVector3& vector) const )
	v2 = Vector3(1, 2, 3);
	v  = Vector3(1, 2, 3);
	TEST_EQUAL(v * v2 , 14)
RESULT

//line
CHECK(TVector3::getDistance(const TVector3& vector) const )
	v2 = Vector3(1, 2, 4);
	v  = Vector3(0, 1, 2);
	TEST_REAL_EQUAL(v.getDistance(v2) , sqrt(6.0))
RESULT

//line
CHECK(TVector3::getSquareDistance(const TVector3& vector) const )
	v2 = Vector3(1, 2, 4);
	v  = Vector3(0, 1, 2);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 6.0)
RESULT

//line 
CHECK(TVector3::bool operator == (const TVector3& vector) const )
	v2 = Vector3(1, 2, 3);
	v  = Vector3(1, 2, 2);
	TEST_EQUAL(v == v2 , false)
	v  = Vector3(1, 2, 3);
	TEST_EQUAL(v == v2 , true)
RESULT

//line 
CHECK(TVector3::bool operator != (const TVector3& vector) const )
	v2 = Vector3(1, 2, 3);
	v  = Vector3(1, 2, 2);
	TEST_EQUAL(v != v2 , true)
	v = Vector3(1,2,3);
	TEST_EQUAL(v != v2 , false)
RESULT

//line
CHECK(TVector3::isOrthogonalTo(TVector3& vector) const )
	v2 = Vector3(1, 0, 3);
	v  = Vector3(0, 2, 0);
	TEST_EQUAL(v.isOrthogonalTo(v2) , true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2) , false)
RESULT

//line
CHECK(TVector3::isValid() const )
	v  = Vector3(0,2,0);
	TEST_EQUAL(v.isValid(), true)
RESULT

//line
CHECK(TVector3::dump(std::ostream& s = std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT

//line
CHECK(TVector3<T>::set(const T* ptr))
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3();
	v.set(arr);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
RESULT

//line
CHECK(TVector3::set(const T& x, const T& y, const T& z))
	v = Vector3();
	v.set(1,2,3);
	v2 = Vector3(1,2,3);
	TEST_EQUAL(v2, v)
RESULT

//line 
CHECK(TVector3::operator = (const T* ptr))
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3();
	v = arr;
	v2 = Vector3(1,2,3);
	TEST_EQUAL(v2, v)
RESULT

// line
CHECK(TVector3<T>& TVector3<T>::operator = (const TVector3<T>& v))
	v2 = Vector3(1,2,3);
	v  = Vector3();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

//line
CHECK(TVector3::get(T* ptr) const )
	float arr[4];
	v = Vector3(1,2,3);
	v.get(arr);
	TEST_EQUAL(arr[0], 1)
	TEST_EQUAL(arr[1], 2)
	TEST_EQUAL(arr[2], 3)
RESULT

//line 
CHECK(TVector3::get(T& rx, T& ry, T& rz) const)
	float a,b,c;
	v = Vector3(1,2,3);
	v.get(a,b,c);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
RESULT

//line
CHECK(TVector3::get(TVector3<T>& v, bool deep) const )
	v2 = Vector3(1,2,3);
	v  = Vector3();
	v.get(v2);
	TEST_EQUAL(v2, v)
RESULT

//line
CHECK(TVector3<T>::swap(TVector3<T>& v))
	v  = Vector3(1, 2, 3);
	Vector3 u(4, 3, 2);
	v2 = u;
	Vector3 u2 = v;
	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
