// $Id: Vector4_test.C,v 1.3 2000/02/19 19:56:34 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/vector4.h>

///////////////////////////

START_TEST(TVector4, "$Id: Vector4_test.C,v 1.3 2000/02/19 19:56:34 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

//line
CHECK(TVector4();)
  Vector4* v;
	v = new Vector4();
	TEST_NOT_EQUAL(0, v)
RESULT								

//line
CHECK(~TVector4();)
  Vector4* v;
	v = new Vector4();
	delete v;
RESULT		

Vector4 v;
Vector4 v1;
Vector4 v2;
float x;

CHECK(TVector4::T& operator [] (Index index) const)
	v = Vector4(1.0, 2.0, 3.0, 4.0);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2.0)
	TEST_EQUAL(v[2], 3.0)
	TEST_EQUAL(v[3], 4.0)
	TEST_EXCEPTION(Exception::IndexUnderflow, x = v[-1])
	TEST_EXCEPTION(Exception::IndexOverflow,  x = v[4])
RESULT


//line
CHECK(TVector4(const T* ptr);)
	float arr[4];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	v = Vector4(arr);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
	TEST_EXCEPTION(Exception::NullPointer, v = Vector4(0))
RESULT

//line
CHECK(TVector4(const T& x, const T& y, const T& z, const T& h = (T)1);)
	v = Vector4(1, 2, 3, 4);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
	v = Vector4(1, 2, 3);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 1)
RESULT

//line
CHECK(TVector4(const TVector4& vector, bool deep = true);)
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(v2);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

//line 38: method TVector4 operator + (const TVector4<T>& a, const TVector4<T>& b)
CHECK(TVector4 operator + (const TVector4<T>& a, const TVector4<T>& b))
 	v1 = Vector4(1, 2, 3, 4);
	v2 = Vector4(1, 2, 3, 4);
	v  = v1 + v2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

//line 42: method TVector4 operator - (const TVector4<T>& a, const TVector4<T>& b)
CHECK(TVector4 operator - (const TVector4<T>& a, const TVector4<T>& b))
 	v1 = Vector4(2, 3, 4, 5);
	v2 = Vector4(1, 2, 3, 4);
	v  = v1 - v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 1)
	TEST_EQUAL(v[2], 1)
	TEST_EQUAL(v[3], 1)
RESULT

//line 46: method TVector4 operator * (const TVector4<T>& a, const TVector4<T>& b)
CHECK(TVector4 operator * (const TVector4<T>& a, const TVector4<T>& b))
 	v1 = Vector4(2, 2, 2, 2);
	v2 = Vector4(1, 2, 3, 4);
	v  = v1 - v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 0)
	TEST_EQUAL(v[2], -1)
	TEST_EQUAL(v[3], -2)
RESULT

//line 49: method std::istream& operator >> (std::istream& s, TVector4<T>& vector)
CHECK(std::istream& operator >> (std::istream& s, TVector4<T>& vector))
	//std::ostream ostream;
	//v = Vector4(1, 2, 3, 4);
	//ostream = << v;
	//TEST_EQUAL(ostream, << 1 << 2 << 3 << 4)  //BAUSTELLE
RESULT

//line 52: method std::ostream& operator << (std::ostream& s, const TVector4<T>& vector)
CHECK(std::ostream& operator << (std::ostream& s, const TVector4<T>& vector))
	// std::istream instream;
	//instream >> 1 >> 2 >> 3 >> 4 ;
	//v  = Vector4(instream);  //BAUSTELLE
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

//line 67: method TVector4::BALL_CREATE(TVector4<T>)
CHECK(TVector4::BALL_CREATE(TVector4<T>))
  //BAUSTELLE
RESULT

//line 183: method TVector4::getLength() const 
CHECK(TVector4::getLength() const )
	v = Vector4(4, 9, 16, 25);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4 * 4 + 9 * 9 + 16 * 16 + 25 * 25))
RESULT

//line 190: method TVector4::getSquareLength() const 
CHECK(TVector4::getSquareLength() const )
	v = Vector4(1, 2, 3, 4);
	TEST_REAL_EQUAL(v.getSquareLength(), 30)
RESULT

//line 198: method TVector4::normalize()
CHECK(TVector4::normalize())
	v = Vector4(4, 9, 16, 25);
	v.normalize();
	TEST_REAL_EQUAL(v[0], 4/31.273)
	TEST_REAL_EQUAL(v[1], 9/31.273)
	TEST_REAL_EQUAL(v[2], 16/31.273)
	TEST_REAL_EQUAL(v[3], 25/31.273)
	v = Vector4(0, 0, 0, 0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT

//line 202: method TVector4::getZero()
CHECK(TVector4::getZero())
	TEST_EQUAL(Vector4::getZero().x, 0)
	TEST_EQUAL(Vector4::getZero().y, 0)
	TEST_EQUAL(Vector4::getZero().z, 0)
	TEST_EQUAL(Vector4::getZero().h, 0)
RESULT

//line 206: method TVector4::getUnit()
CHECK(TVector4::getUnit())
	TEST_EQUAL(Vector4::getUnit().x, 1)
	TEST_EQUAL(Vector4::getUnit().y, 1)
	TEST_EQUAL(Vector4::getUnit().z, 1)
	TEST_EQUAL(Vector4::getUnit().h, 1)
RESULT

//line 211: method TVector4::fill(const T& value = (T)1)
CHECK(TVector4::fill(const T& value = (T)1))
	v = Vector4(2,2,2,2);
	v.fill();
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 1)
	TEST_EQUAL(v[2], 1)
	TEST_EQUAL(v[3], 1)
	v = Vector4(2,2,2,2);
	v.fill(3);
	TEST_EQUAL(v[0], 3)
	TEST_EQUAL(v[1], 3)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 3)
RESULT

//line 217: method TVector4::T& operator [] (Index index)
// s.o.

//line 223: method TVector4::T& operator [] (Index index) const 
CHECK(TVector4::T& operator [] (Index index) )
	v = Vector4(1, 2, 3, 4);
	v[0]=5;	v[1]=6;
	v[2]=7;	v[3]=8;
	TEST_EQUAL(v[0], 5)
	TEST_EQUAL(v[1], 6)
	TEST_EQUAL(v[2], 7)
	TEST_EQUAL(v[3], 8)
	TEST_EXCEPTION(Exception::IndexUnderflow, v[-1] = 5)
	TEST_EXCEPTION(Exception::IndexOverflow, v[4] = 5)
RESULT

//line 231: method TVector4::TVector4 operator + () const 
CHECK(TVector4::TVector4 operator + () const )
	v2 = Vector4(1, 2, 3, 4);
	v = + v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

//line 235: method TVector4::TVector4 operator - () const 
CHECK(TVector4::TVector4 operator - () const )
	v2 = Vector4(1, 2, 3, 4);
	v = + v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

//line 242: method TVector4::TVector4& operator += (const TVector4& vector)
CHECK(TVector4::TVector4& operator += (const TVector4& vector))
	v2 = Vector4(1, 2 ,3, 4);
	v  = Vector4(1, 2, 3, 4);
	v += v2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

//line 249: method TVector4::TVector4& operator -= (const TVector4& vector)
CHECK(TVector4::TVector4& operator -= (const TVector4& vector))
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(4, 3, 2, 1);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], -1)
	TEST_REAL_EQUAL(v[3], -3)
RESULT

//line 256: method TVector4::TVector4 operator * (const T& scalar)
CHECK(TVector4::TVector4 operator * (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v = v * 2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

//line 263: method TVector4::TVector4& operator *= (const T& scalar)
CHECK(TVector4::TVector4& operator *= (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v *= 2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

//line 271: method TVector4::TVector4 operator / (const T& scalar)
CHECK(TVector4::TVector4 operator / (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v = v / 0.5;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

//line 278: method TVector4::TVector4& operator /= (const T& scalar)
CHECK(TVector4::TVector4& operator /= (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v /= 0.5;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

//line 283: method TVector4::T operator * (const TVector4& vector) const 
CHECK(TVector4::T operator * (const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(1, 2, 3, 4);
	TEST_EQUAL(v * v2 , 30)
RESULT

//line 289: method TVector4::getDistance(const TVector4& vector) const 
CHECK(TVector4::getDistance(const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(0, 1, 2, 3);
	TEST_REAL_EQUAL(v.getDistance(v2) , 2)
RESULT

//line 297: method TVector4::getSquareDistance(const TVector4& vector) const 
CHECK(TVector4::getSquareDistance(const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(0, 1, 2, 3);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 4)
RESULT

//line 307: method TVector4::bool operator == (const TVector4& vector) const 
CHECK(TVector4::bool operator == (const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(1, 2, 3, 3);
	TEST_EQUAL(v == v2 , false)
	v  = Vector4(1, 2, 3, 4);
	TEST_EQUAL(v == v2 , true)
RESULT

//line 312: method TVector4::bool operator != (const TVector4& vector) const 
CHECK(TVector4::bool operator != (const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(1, 2, 3, 3);
	TEST_EQUAL(v != v2 , true)
	v = Vector4(1,2,3,4);
	TEST_EQUAL(v != v2 , false)
RESULT

//line 316: method TVector4::isOrthogonalTo(TVector4& vector) const 
CHECK(TVector4::isOrthogonalTo(TVector4& vector) const )
	v2 = Vector4(1, 0, 3, 0);
	v  = Vector4(0, 2, 0, 3);
	TEST_EQUAL(v.isOrthogonalTo(v2) , true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2) , false)
RESULT

//line 323: method TVector4::isValid() const 
CHECK(TVector4::isValid() const )
	v  = Vector4(0,2,0,3);
	TEST_EQUAL(v.isValid(), true)
RESULT

//line 326: method TVector4::dump(std::ostream& s = std::cout, Size depth = 0) const 
CHECK(TVector4::dump(std::ostream& s = std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT

//line 395 method TVector4::TVector4<T>::set(const T* ptr)
CHECK(TVector4<T>::set(const T* ptr))
	float arr[4];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	v = Vector4();
	v.set(arr);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

//line 408: method TVector4::set(const T& x, const T& y, const T& z, const T& h)
CHECK(TVector4::set(const T& x, const T& y, const T& z, const T& h))
	v = Vector4();
	v.set(1,2,3,4);
	v2 = Vector4(1,2,3,4);
	TEST_EQUAL(v2, v)
RESULT

//line 428: method TVector4::operator = (const T* ptr)
CHECK(TVector4::operator = (const T* ptr))
	float arr[4];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	arr[3] = 4;
	v = Vector4();
	v = arr;
	v2 = Vector4(1,2,3,4);
	TEST_EQUAL(v2, v)
RESULT

// line 442
CHECK(TVector4<T>& TVector4<T>::operator = (const TVector4<T>& v))
	v2 = Vector4(1,2,3,4);
	v  = Vector4();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

//line 455: method TVector4::get(T* ptr) const 
CHECK(TVector4::get(T* ptr) const )
	float arr[4];
	v = Vector4(1,2,3,4);
	v.get(arr);
	TEST_EQUAL(arr[0], 1)
	TEST_EQUAL(arr[1], 2)
	TEST_EQUAL(arr[2], 3)
	TEST_EQUAL(arr[3], 4)
RESULT

//line 467: method TVector4::get(T& rx, T& ry, T& rz, T& rh) const
CHECK(TVector4::get(T& rx, T& ry, T& rz, T& rh) const)
	float a,b,c,d;
	v = Vector4(1,2,3,4);
	v.get(a,b,c,d);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
	TEST_EQUAL(d, 4)
RESULT

//line 478: method TVector4::get(TVector4<T>& v, bool deep) const 
CHECK(TVector4::get(TVector4<T>& v, bool deep) const )
	v2 = Vector4(1,2,3,4);
	v  = Vector4();
	v.get(v2);
	TEST_EQUAL(v2, v)
RESULT

//line 487 method TVector4::TVector4<T>::swap(TVector4<T>& v)
CHECK(TVector4<T>::swap(TVector4<T>& v))
	v  = Vector4(1, 2, 3, 4);
	Vector4 u(4, 3, 2, 1);
	v2 = u;
	Vector4 u2 = v;
	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
