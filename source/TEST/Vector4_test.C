// $Id: Vector4_test.C,v 1.24 2001/07/16 09:39:49 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector4.h>
///////////////////////////

START_TEST(TVector4, "$Id: Vector4_test.C,v 1.24 2001/07/16 09:39:49 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
using std::ofstream;
using std::ios;

CHECK(TVector4();)
  Vector4* v;
	v = new Vector4();
	TEST_NOT_EQUAL(0, v)
RESULT								

CHECK(~TVector4();)
  Vector4* v;
	v = new Vector4();
	delete v;
RESULT		

CHECK(clear())
	Vector4 v1(1, 2, 3, 4);
	Vector4 v2;
	v2.set(0, 0, 0,0 );
	v1.clear();
	TEST_EQUAL(v1, v2)
RESULT

Vector4 v;
Vector4 v1;
Vector4 v2;

CHECK(TVector4::T& operator [] (Index index) const)
	v = Vector4(1.0, 2.0, 3.0, 4.0);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2.0)
	TEST_EQUAL(v[2], 3.0)
	TEST_EQUAL(v[3], 4.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[32000])
	TEST_EXCEPTION(Exception::IndexOverflow,  v[4])
RESULT

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
	TEST_EXCEPTION(Exception::NullPointer, v = Vector4((float*)0))
RESULT

CHECK(TVector4(const T& scalar);)
	v = Vector4(1.23456);
	TEST_REAL_EQUAL(v[0], 1.23456)
	TEST_REAL_EQUAL(v[1], 1.23456)
	TEST_REAL_EQUAL(v[2], 1.23456)
	TEST_REAL_EQUAL(v[3], 1.23456)
RESULT

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

CHECK(TVector4(const TVector4& vector, bool deep = true);)
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(v2);
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

CHECK(TVector4 operator + (const TVector4<T>& a, const TVector4<T>& b))
 	v1 = Vector4(1, 2, 3, 4);
	v2 = Vector4(1, 2, 3, 4);
	v  = v1 + v2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

CHECK(TVector4 operator - (const TVector4<T>& a, const TVector4<T>& b))
 	v1 = Vector4(2, 3, 4, 5);
	v2 = Vector4(1, 2, 3, 4);
	v  = v1 - v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 1)
	TEST_EQUAL(v[2], 1)
	TEST_EQUAL(v[3], 1)
RESULT

CHECK(TVector4 operator * (const TVector4<T>& a, const TVector4<T>& b))
 	v1 = Vector4(2, 2, 2, 2);
	v2 = Vector4(1, 2, 3, 4);
	v  = v1 - v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 0)
	TEST_EQUAL(v[2], -1)
	TEST_EQUAL(v[3], -2)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TVector4<T>& vector))
	std::ifstream instr("data/Vector4_test2.txt");
	Vector4 v(1, 2, 3, 4);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
	TEST_REAL_EQUAL(v.z, 3.4)
	TEST_REAL_EQUAL(v.h, 4.5)
RESULT


NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector4<T>& vector))
	Vector4 v(1.2, 2.3, 3.4, 4.5);
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Vector4_test2.txt", false)
RESULT

CHECK(TVector4::BALL_CREATE(TVector4<T>))
	Vector4 v(1, 2, 3, 4);
	Vector4* v_ptr = (Vector4*)v.create(false, true);
	TEST_REAL_EQUAL(v_ptr->x, 0.0)
	TEST_REAL_EQUAL(v_ptr->y, 0.0)
	TEST_REAL_EQUAL(v_ptr->z, 0.0)
	TEST_REAL_EQUAL(v_ptr->h, 0.0)
	delete v_ptr;
	v_ptr = (Vector4*)v.create();
	TEST_REAL_EQUAL(v_ptr->x, 1.0)
	TEST_REAL_EQUAL(v_ptr->y, 2.0)
	TEST_REAL_EQUAL(v_ptr->z, 3.0)
	TEST_REAL_EQUAL(v_ptr->h, 4.0)
	delete v_ptr;
RESULT

CHECK(TVector4::getLength() const )
	v = Vector4(4, 9, 16, 25);
	float result = sqrt(4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0 + 25.0 * 25.0);
	TEST_REAL_EQUAL(v.getLength(), result)
	v = Vector4(0, 0, 0, 0);
	TEST_REAL_EQUAL(v.getLength(), 0)
RESULT

CHECK(TVector4::getSquareLength() const )
	v = Vector4(1, 2, 3, 4);
	TEST_REAL_EQUAL(v.getSquareLength(), 30)
	v = Vector4(0, 0, 0, 0);
	TEST_REAL_EQUAL(v.getSquareLength(), 0)
RESULT

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

CHECK(TVector4::getZero())
	TEST_EQUAL(Vector4::getZero().x, 0)
	TEST_EQUAL(Vector4::getZero().y, 0)
	TEST_EQUAL(Vector4::getZero().z, 0)
	TEST_EQUAL(Vector4::getZero().h, 0)
RESULT

CHECK(TVector4::getUnit())
	TEST_EQUAL(Vector4::getUnit().x, 1)
	TEST_EQUAL(Vector4::getUnit().y, 1)
	TEST_EQUAL(Vector4::getUnit().z, 1)
	TEST_EQUAL(Vector4::getUnit().h, 1)
RESULT

CHECK(TVector4::set(const T& value = (T)1))
	v = Vector4(2,2,2,2);
	v.set();
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 1)
	TEST_EQUAL(v[2], 1)
	TEST_EQUAL(v[3], 1)
	v = Vector4(2,2,2,2);
	v.set(3);
	TEST_EQUAL(v[0], 3)
	TEST_EQUAL(v[1], 3)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 3)
RESULT

CHECK(operator = (T value))
	v = 1.1;
	TEST_REAL_EQUAL(v.x, (float) 1.1)
	TEST_REAL_EQUAL(v.y, (float) 1.1)
	TEST_REAL_EQUAL(v.z, (float) 1.1)
	TEST_REAL_EQUAL(v.h, (float) 1.1)
RESULT

CHECK(TVector4::T& operator [] (Index index) )
	v = Vector4(1, 2, 3, 4);
	v[0]=5;	v[1]=6;
	v[2]=7;	v[3]=8;
	TEST_EQUAL(v[0], 5)
	TEST_EQUAL(v[1], 6)
	TEST_EQUAL(v[2], 7)
	TEST_EQUAL(v[3], 8)
	TEST_EXCEPTION(Exception::IndexOverflow, v[32000] = 5)
	TEST_EXCEPTION(Exception::IndexOverflow, v[4] = 5)
RESULT

CHECK(TVector4::TVector4 operator + () const )
	v2 = Vector4(1, 2, 3, 4);
	v = + v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

CHECK(TVector4::TVector4 operator - () const )
	v2 = Vector4(1, 2, 3, 4);
	v = + v2;
	TEST_EQUAL(v[0], 1)
	TEST_EQUAL(v[1], 2)
	TEST_EQUAL(v[2], 3)
	TEST_EQUAL(v[3], 4)
RESULT

CHECK(TVector4::TVector4& operator += (const TVector4& vector))
	v2 = Vector4(1, 2 ,3, 4);
	v  = Vector4(1, 2, 3, 4);
	v += v2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

CHECK(TVector4::TVector4& operator -= (const TVector4& vector))
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(4, 3, 2, 1);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], -1)
	TEST_REAL_EQUAL(v[3], -3)
RESULT

CHECK(TVector4::TVector4 operator * (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v = v * 2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

CHECK(TVector4::TVector4& operator *= (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v *= 2;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
RESULT

CHECK(TVector4::TVector4 operator / (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v = v / 0.5;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

CHECK(TVector4::TVector4& operator /= (const T& scalar))
	v  = Vector4(1, 2, 3, 4);
	v /= 0.5;
	TEST_EQUAL(v[0], 2)
	TEST_EQUAL(v[1], 4)
	TEST_EQUAL(v[2], 6)
	TEST_EQUAL(v[3], 8)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

CHECK(TVector4::T operator * (const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(1, 2, 3, 4);
	TEST_REAL_EQUAL(v * v2 , 30.0)
RESULT

CHECK(TVector4::getDistance(const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(0, 1, 2, 3);
	TEST_REAL_EQUAL(v.getDistance(v2) , 2)
RESULT

CHECK(TVector4::getSquareDistance(const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(0, 1, 2, 3);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 4)
RESULT

CHECK(TVector4::bool operator == (const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(1, 2, 3, 3);
	TEST_EQUAL(v == v2 , false)
	v  = Vector4(1, 2, 3, 4);
	TEST_EQUAL(v == v2 , true)
RESULT

CHECK(TVector4::bool operator != (const TVector4& vector) const )
	v2 = Vector4(1, 2, 3, 4);
	v  = Vector4(1, 2, 3, 3);
	TEST_EQUAL(v != v2 , true)
	v = Vector4(1,2,3,4);
	TEST_EQUAL(v != v2 , false)
RESULT

CHECK(TVector4::isOrthogonalTo(TVector4& vector) const )
	v2 = Vector4(1, 0, 3, 0);
	v  = Vector4(0, 2, 0, 3);
	TEST_EQUAL(v.isOrthogonalTo(v2) , true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2) , false)
RESULT

CHECK(TVector4::isValid() const )
	v  = Vector4(0,2,0,3);
	TEST_EQUAL(v.isValid(), true)
RESULT

CHECK(TVector3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Vector4 v(1.2, 2.3, 3.4, 4.5);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	v.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Vector4_test.txt", true)
RESULT

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

CHECK(TVector4::set(const T& x, const T& y, const T& z, const T& h))
	v = Vector4();
	v.set(1,2,3,4);
	v2 = Vector4(1,2,3,4);
	TEST_EQUAL(v2, v)
RESULT

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

CHECK(TVector4<T>& TVector4<T>::operator = (const TVector4<T>& v))
	v2 = Vector4(1,2,3,4);
	v  = Vector4();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector4::get(T* ptr) const )
	float arr[4];
	v = Vector4(1,2,3,4);
	v.get(arr);
	TEST_EQUAL(arr[0], 1)
	TEST_EQUAL(arr[1], 2)
	TEST_EQUAL(arr[2], 3)
	TEST_EQUAL(arr[3], 4)
RESULT

CHECK(TVector4::get(T& rx, T& ry, T& rz, T& rh) const)
	float a,b,c,d;
	v = Vector4(1,2,3,4);
	v.get(a,b,c,d);
	TEST_EQUAL(a, 1)
	TEST_EQUAL(b, 2)
	TEST_EQUAL(c, 3)
	TEST_EQUAL(d, 4)
RESULT

CHECK(TVector4::get(TVector4<T>& v, bool deep) const )
	v2 = Vector4(1,2,3,4);
	v  = Vector4();
	v.get(v2);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector4<T>::swap(TVector4<T>& v))
	v  = Vector4(1, 2, 3, 4);
	Vector4 u(4, 3, 2, 1);
	v2 = u;
	Vector4 u2 = v;
	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

CHECK(TVector4::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Vector4 v(1.2, 2.3, 3.4, 4.5);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	v.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Vector4_test.txt", true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TVector4<T>& vector))
	std::ifstream instr("data/Vector4_test2.txt");
	Vector4 v(1, 2, 3, 4);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
	TEST_REAL_EQUAL(v.z, 3.4)
	TEST_REAL_EQUAL(v.h, 4.5)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector4<T>& vector))
	Vector4 v(1.2, 2.3, 3.4, 4.5);
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Vector4_test2.txt", false)
RESULT
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
