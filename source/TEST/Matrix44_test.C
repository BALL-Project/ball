// $Id: Matrix44_test.C,v 1.13 2000/07/12 19:36:46 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/angle.h>
#include <BALL/common.h>
#include <BALL/COMMON/limits.h>
#include <math.h>
///////////////////////////

START_TEST(class_name, "$Id: Matrix44_test.C,v 1.13 2000/07/12 19:36:46 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String filename;
using std::ofstream;
using std::ios;
PRECISION(1E-5)
Constants::EPSILON = TEST::precision;

CHECK(TMatrix4x4::BALL_CREATE(TMatrix4x4))
	Matrix4x4 m(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	Matrix4x4* m_ptr = (Matrix4x4*)m.create(false, true);
	TEST_REAL_EQUAL(m_ptr->m11, 0.0)
	TEST_REAL_EQUAL(m_ptr->m12, 0.0)
	TEST_REAL_EQUAL(m_ptr->m13, 0.0)
	TEST_REAL_EQUAL(m_ptr->m14, 0.0)
	TEST_REAL_EQUAL(m_ptr->m21, 0.0)
	TEST_REAL_EQUAL(m_ptr->m22, 0.0)
	TEST_REAL_EQUAL(m_ptr->m23, 0.0)
	TEST_REAL_EQUAL(m_ptr->m24, 0.0)
	TEST_REAL_EQUAL(m_ptr->m31, 0.0)
	TEST_REAL_EQUAL(m_ptr->m32, 0.0)
	TEST_REAL_EQUAL(m_ptr->m33, 0.0)
	TEST_REAL_EQUAL(m_ptr->m34, 0.0)
	TEST_REAL_EQUAL(m_ptr->m41, 0.0)
	TEST_REAL_EQUAL(m_ptr->m42, 0.0)
	TEST_REAL_EQUAL(m_ptr->m43, 0.0)
	TEST_REAL_EQUAL(m_ptr->m44, 0.0)	
	delete m_ptr;
	m_ptr = (Matrix4x4*)m.create();
	TEST_REAL_EQUAL(m_ptr->m11, 1.0)
	TEST_REAL_EQUAL(m_ptr->m12, 2.0)
	TEST_REAL_EQUAL(m_ptr->m13, 3.0)
	TEST_REAL_EQUAL(m_ptr->m14, 4.0)
	TEST_REAL_EQUAL(m_ptr->m21, 5.0)
	TEST_REAL_EQUAL(m_ptr->m22, 6.0)
	TEST_REAL_EQUAL(m_ptr->m23, 7.0)
	TEST_REAL_EQUAL(m_ptr->m24, 8.0)
	TEST_REAL_EQUAL(m_ptr->m31, 9.0)
	TEST_REAL_EQUAL(m_ptr->m32, 10.0)
	TEST_REAL_EQUAL(m_ptr->m33, 11.0)
	TEST_REAL_EQUAL(m_ptr->m34, 12.0)
	TEST_REAL_EQUAL(m_ptr->m41, 13.0)
	TEST_REAL_EQUAL(m_ptr->m42, 14.0)
	TEST_REAL_EQUAL(m_ptr->m43, 15.0)
	TEST_REAL_EQUAL(m_ptr->m44, 16.0)	
	delete m_ptr;
RESULT

CHECK(TMatrix4x4::TMatrix4x4())
  Matrix4x4* m;
	m = new Matrix4x4();
	TEST_NOT_EQUAL(m, 0)
RESULT

CHECK(~TMatrix4x4();)
  Matrix4x4* m;
	m = new Matrix4x4();
	delete m;
RESULT		

CHECK(TMatrix4x4::TMatrix4x4
			(const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44))
  Matrix4x4 m(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	TEST_REAL_EQUAL(m.m11, 1.0)
	TEST_REAL_EQUAL(m.m12, 2.0)
	TEST_REAL_EQUAL(m.m13, 3.0)
	TEST_REAL_EQUAL(m.m14, 4.0)
	TEST_REAL_EQUAL(m.m21, 5.0)
	TEST_REAL_EQUAL(m.m22, 6.0)
	TEST_REAL_EQUAL(m.m23, 7.0)
	TEST_REAL_EQUAL(m.m24, 8.0)
	TEST_REAL_EQUAL(m.m31, 9.0)
	TEST_REAL_EQUAL(m.m32, 10.0)
	TEST_REAL_EQUAL(m.m33, 11.0)
	TEST_REAL_EQUAL(m.m34, 12.0)
	TEST_REAL_EQUAL(m.m41, 13.0)
	TEST_REAL_EQUAL(m.m42, 14.0)
	TEST_REAL_EQUAL(m.m43, 15.0)
	TEST_REAL_EQUAL(m.m44, 16.0)	
RESULT

const Matrix4x4 m(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
Matrix4x4 m1, m2, m3;
const Vector4 v(1.0, 2.0, 3.0, 4.0), v1(5.0, 6.0, 7.0, 8.0), v2(9.0, 10.0, 11.0, 12.0), v3(13.0, 14.0, 15.0, 16.0);
Vector4 vx, vy;
const Angle a = Angle(1.0, true);

CHECK(TMatrix4x4::bool operator == (const TMatrix4x4& matrix) const )
  Matrix4x4 m1(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	TEST_EQUAL(m == m1 , true)
  m1 = Matrix4x4(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.1);
	TEST_EQUAL(m == m1 , false)
RESULT

CHECK(TMatrix4x4::TMatrix4x4(const TMatrix4x4& m ,bool deep = true))
	m1 = Matrix4x4(m);
	TEST_EQUAL(m == m1 , true)
RESULT

CHECK(TMatrix4x4(const T* ptr);)
	float arr[16];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1.0;
	}
	m1 = Matrix4x4(arr);
	TEST_EQUAL(m == m1 , true)
	TEST_EXCEPTION(Exception::NullPointer, m1 = Matrix4x4((float*)0))
RESULT

CHECK(TMatrix4x4(const T ptr[4][4]))
	float v[4][4];
	int pos = 1;
	for (int i=0; i<4; i++ )
	{
			for (int j=0; j<4; j++  )
			{
				v[i][j] = (float)pos;
				pos++;
			}
	}
	m1 = Matrix4x4(v);
	TEST_EQUAL(m1.isEqual(m), true);
	TEST_EXCEPTION(Exception::NullPointer, m1 = Matrix4x4((float*)0))
RESULT

CHECK(TMatrix4x4(const TVector4<T>& col1, const TVector4<T>& col2,
		 const TVector4<T>& col3,const TVector4<T>& col4))
	m1 = Matrix4x4(v, v1, v2, v3);
	TEST_EQUAL(m == m1 , true)
RESULT

CHECK(set( const T* ptr))
	m1 = Matrix4x4();
	float arr[15];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1;
	}
	m1.set(arr);
	TEST_EQUAL(m1.isEqual(m), true)
	TEST_EXCEPTION(Exception::NullPointer, m1.set((float*)0))
RESULT

CHECK(set( const T ptr[4][4]))
	m1 = Matrix4x4();
	float v[4][4];
	int pos = 1;
	for (int i=0; i<4; i++ )
	{
			for (int j=0; j<4; j++ )
			{
				v[i][j] = (float)pos;
				pos++;
			}
	}
	m1.set(v);
	TEST_EQUAL(m1.isEqual(m), true);
	TEST_EXCEPTION(Exception::NullPointer, m1.set((float*)0))
RESULT

CHECK(set(const TMatrix4x4& m, bool deep = true))
	m1 = Matrix4x4();
	m1.set(m);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(set
			(const TVector4<T>& col1, const TVector4<T>& col2,
			 const TVector4<T>& col3, const TVector4<T>& col4))
	m1 = Matrix4x4();
	m1.set(v, v1, v2, v3);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(set
			(const T& m11, const T& m12, const T& m13, const T& m14, 
			 const T& m21, const T& m22, const T& m23, const T& m24, 
			 const T& m31, const T& m32, const T& m33, const T& m34, 
			 const T& m41, const T& m42, const T& m43, const T& m44))
	m1 = Matrix4x4();
	m1.set(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(operator = ( const T* ptr))
	m1 = Matrix4x4();
	float arr[16];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1.0;
	}
	m1 = arr;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(operator = ( const T ptr[4][4]))
	m1 = Matrix4x4();
	float v[4][4];
	int pos = 1;
	for (int i=0; i<4; i++ )
	{
			for (int j=0; j<4; j++ )
			{
				v[i][j] = (float)pos;
				pos++;
			}
	}
	m1 = v;
	TEST_EQUAL(m1.isEqual(m), true);
	TEST_EXCEPTION(Exception::NullPointer, m1 =((float*)0))
RESULT

CHECK(operator = (const TMatrix4x4& m))
	m1 = m;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(get(T* ptr) const)
	m1 = Matrix4x4();
	float arr[16];
	m.get(arr);
	m1.set(arr);
	TEST_EQUAL(m1.isEqual(m), true)
	TEST_EXCEPTION(Exception::NullPointer, m1.get((float*)0))
RESULT

CHECK(get(T* ptr[4][4]) const)
	m1 = Matrix4x4();
	float arr[4][4];
	m.get(arr);
	m1.set(arr);
	TEST_EQUAL(m1.isEqual(m), true);
	TEST_EXCEPTION(Exception::NullPointer, m1.get((float*)0))
RESULT

CHECK(get(TMatrix4x4& m, bool deep = true) const)
	m1 = Matrix4x4();
	m.get(m1);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(get
			(TVector4<T>& col1, TVector4<T>& col2,
			 TVector4<T>& col3, TVector4<T>& col4) const)
	Vector4 z1, z2, z3, z4;
	m1 = Matrix4x4();
	m.get(z1, z2, z3, z4);
	m1.set(z1, z2, z3, z4);
	TEST_EQUAL(m1, m)
RESULT

CHECK(get
			(T& m11, T& m12, T& m13, T& m14, 
			 T& m21, T& m22, T& m23, T& m24, 
			 T& m31, T& m32, T& m33, T& m34, 
			 T& m41, T& m42, T& m43, T& m44) const)
	m1 = Matrix4x4();
	float a[16];
	m.get(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8],
				a[9], a[10], a[11], a[12], a[13], a[14], a[15]);
	m1.set(a);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(swap(TMatrix4x4& m))
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	m3 = m2;
	m1.swap(m2);
	TEST_EQUAL(m2.isEqual(m), true)
	TEST_EQUAL(m3.isEqual(m1), true)
RESULT

CHECK(TMatrix4x4::getTrace() const )
	TEST_EQUAL(m.getTrace(), 34.0)
RESULT

CHECK(static const TMatrix4x4& getZero())
	const Matrix4x4 c = m1.getZero();
	m1 = Matrix4x4();
	m1.m11 = m1.m22 = m1.m33 = m1.m44 = 0;
	TEST_EQUAL(m1.isEqual(c), true)
RESULT

CHECK(const TMatrix4x4<T>& TMatrix4x4::setIdentity())
	const Matrix4x4 c = m1.getIdentity();
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	TEST_EQUAL(c.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setIdentity())
	m1.setIdentity();
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::set(const T& t = (T)1))
	m1.set(2.0);
	m2 = Matrix4x4(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::transpose())
	m1 = Matrix4x4(m);
	m1.transpose();
	m2 = Matrix4x4(1.0, 5.0, 9.0, 13.0, 2.0, 6.0, 10.0, 14.0, 3.0, 7.0, 11.0, 15.0, 4.0, 8.0, 12.0, 16.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::getRow(Position row))
	m1 = Matrix4x4(m);
	vx = m1.getRow(0);
	TEST_EQUAL(vx , v)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.getRow(4))
RESULT

CHECK(TMatrix4x4::getColumn(Position col))
	m1 = Matrix4x4(m);
	vx = m1.getColumn(0);
	vy = Vector4(1.0, 5.0, 9.0, 13.0);
	TEST_EQUAL(vx , vy)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.getColumn(4))
RESULT

CHECK(TMatrix4x4::setColumn(Position row, TVector4<T> row_value))
	Vector4 x(0.1, 0.2, 0.3, 0.4);
	m1 = Matrix4x4(m);
	m1.setRow(0, x);
	vx = m1.getRow(0);
	TEST_EQUAL(x , vx)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.setRow(4, vx))
RESULT


CHECK(TMatrix4x4::setColumn(Position col, TVector4<T> col_value))
	Vector4 x(0.1, 0.2, 0.3, 0.4);
	m1 = Matrix4x4(m);
	m1.setColumn(0, x);
	vx = m1.getColumn(0);
	TEST_EQUAL(x , vx)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.setColumn(4, vx))
RESULT

CHECK(TMatrix4x4::getDiagonal() const )
	vx = m.getDiagonal();
	vy = Vector4(1.0, 6.0, 11.0, 16.0);
	TEST_EQUAL(vx , vy)
RESULT

CHECK(TMatrix4x4::T& operator () (Position row, Position col))
	const Matrix4x4& c_m1 = const_cast<const Matrix4x4&>(m1);
	TEST_REAL_EQUAL(c_m1(0, 0), m1.m11)
	TEST_REAL_EQUAL(c_m1(0, 1), m1.m12)
	TEST_REAL_EQUAL(c_m1(0, 2), m1.m13)
	TEST_REAL_EQUAL(c_m1(0, 3), m1.m14)
	TEST_REAL_EQUAL(c_m1(1, 0), m1.m21)
	TEST_REAL_EQUAL(c_m1(1, 1), m1.m22)
	TEST_REAL_EQUAL(c_m1(1, 2), m1.m23)
	TEST_REAL_EQUAL(c_m1(1, 3), m1.m24)
	TEST_REAL_EQUAL(c_m1(2, 0), m1.m31)
	TEST_REAL_EQUAL(c_m1(2, 1), m1.m32)
	TEST_REAL_EQUAL(c_m1(2, 2), m1.m33)
	TEST_REAL_EQUAL(c_m1(2, 3), m1.m34)
	TEST_REAL_EQUAL(c_m1(3, 0), m1.m41)
	TEST_REAL_EQUAL(c_m1(3, 1), m1.m42)
	TEST_REAL_EQUAL(c_m1(3, 2), m1.m43)
	TEST_REAL_EQUAL(c_m1(3, 3), m1.m44)

	TEST_EXCEPTION(Exception::IndexOverflow, m1.m22 = c_m1(4, 1))
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m22 = c_m1(1, 4))
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m22 = c_m1(Limits<Position>::max(), 1))
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m22 = c_m1(1, Limits<Position>::max()))
RESULT

CHECK(TMatrix4x4::T& operator () (Position row, Position col) const )
	TEST_REAL_EQUAL(m1(0, 0), m1.m11)
	TEST_REAL_EQUAL(m1(0, 1), m1.m12)
	TEST_REAL_EQUAL(m1(0, 2), m1.m13)
	TEST_REAL_EQUAL(m1(0, 3), m1.m14)
	TEST_REAL_EQUAL(m1(1, 0), m1.m21)
	TEST_REAL_EQUAL(m1(1, 1), m1.m22)
	TEST_REAL_EQUAL(m1(1, 2), m1.m23)
	TEST_REAL_EQUAL(m1(1, 3), m1.m24)
	TEST_REAL_EQUAL(m1(2, 0), m1.m31)
	TEST_REAL_EQUAL(m1(2, 1), m1.m32)
	TEST_REAL_EQUAL(m1(2, 2), m1.m33)
	TEST_REAL_EQUAL(m1(2, 3), m1.m34)
	TEST_REAL_EQUAL(m1(3, 0), m1.m41)
	TEST_REAL_EQUAL(m1(3, 1), m1.m42)
	TEST_REAL_EQUAL(m1(3, 2), m1.m43)
	TEST_REAL_EQUAL(m1(3, 3), m1.m44)

	TEST_EXCEPTION(Exception::IndexOverflow, m1(4, 1))
	TEST_EXCEPTION(Exception::IndexOverflow, m1(1, 4))
	TEST_EXCEPTION(Exception::IndexOverflow, m1(Limits<Position>::max(), 1))
	TEST_EXCEPTION(Exception::IndexOverflow, m1(1, Limits<Position>::max()))
RESULT

CHECK(const T& TMatrxi44<T>::operator [] (Position index) const)
	const Matrix4x4& c_m1 = const_cast<const Matrix4x4&>(m1);
	TEST_REAL_EQUAL(c_m1[0] , m1.m11)
	TEST_REAL_EQUAL(c_m1[1] , m1.m12)
	TEST_REAL_EQUAL(c_m1[2] , m1.m13)
	TEST_REAL_EQUAL(c_m1[3] , m1.m14)
	TEST_REAL_EQUAL(c_m1[4] , m1.m21)
	TEST_REAL_EQUAL(c_m1[5] , m1.m22)
	TEST_REAL_EQUAL(c_m1[6] , m1.m23)
	TEST_REAL_EQUAL(c_m1[7] , m1.m24)
	TEST_REAL_EQUAL(c_m1[8] , m1.m31)
	TEST_REAL_EQUAL(c_m1[9] , m1.m32)
	TEST_REAL_EQUAL(c_m1[10] , m1.m33)
	TEST_REAL_EQUAL(c_m1[11] , m1.m34)
	TEST_REAL_EQUAL(c_m1[12] , m1.m41)
	TEST_REAL_EQUAL(c_m1[13] , m1.m42)
	TEST_REAL_EQUAL(c_m1[14] , m1.m43)
	TEST_REAL_EQUAL(c_m1[15] , m1.m44)

	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = c_m1[16])
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = c_m1[Limits<Position>::max()])
RESULT


CHECK( T& TMatrxi44<T>::operator [] (Position index))
	TEST_REAL_EQUAL(m1[0] , m1.m11)
	TEST_REAL_EQUAL(m1[1] , m1.m12)
	TEST_REAL_EQUAL(m1[2] , m1.m13)
	TEST_REAL_EQUAL(m1[3] , m1.m14)
	TEST_REAL_EQUAL(m1[4] , m1.m21)
	TEST_REAL_EQUAL(m1[5] , m1.m22)
	TEST_REAL_EQUAL(m1[6] , m1.m23)
	TEST_REAL_EQUAL(m1[7] , m1.m24)
	TEST_REAL_EQUAL(m1[8] , m1.m31)
	TEST_REAL_EQUAL(m1[9] , m1.m32)
	TEST_REAL_EQUAL(m1[10] , m1.m33)
	TEST_REAL_EQUAL(m1[11] , m1.m34)
	TEST_REAL_EQUAL(m1[12] , m1.m41)
	TEST_REAL_EQUAL(m1[13] , m1.m42)
	TEST_REAL_EQUAL(m1[14] , m1.m43)
	TEST_REAL_EQUAL(m1[15] , m1.m44)

	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = m1[16])
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = m1[Limits<Position>::max()])
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator + () const )
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isEqual( + m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator - () const )
	m1 = Matrix4x4(-1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0, -11.0, -12.0, -13.0, -14.0, -15.0, -16.0);
	TEST_EQUAL(m1.isEqual(- m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator + (const TMatrix4x4& m) const )
	m1 = Matrix4x4() + m;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4& operator += (const TMatrix4x4& m))
	m1 = Matrix4x4();
	m1 += m;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator - (const TMatrix4x4& m) const )
	m1 = Matrix4x4() - m;
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4& operator -= (const TMatrix4x4& m))
	m1 = Matrix4x4();
	m1 -= m;
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator * (const T& scalar) const )
	m1 = Matrix4x4(m) * (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4& operator *= (const T& scalar))
	m1 = Matrix4x4(m);
	m1 *= (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator / (const T& scalar) const )
	m1 = Matrix4x4(m) / (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
	TEST_EXCEPTION(Exception::DivisionByZero, m1 / 0)
RESULT

CHECK(TMatrix4x4::TMatrix4x4& operator /= (const T& scalar))
	m1 = Matrix4x4(m);
	m1 /= (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
	TEST_EXCEPTION(Exception::DivisionByZero, m1 /= 0)
RESULT

CHECK(TMatrix4x4::TMatrix4x4 operator * (const TMatrix4x4& m) const )
	m1 = Matrix4x4(11.0, 51.0, 91.0, 131.0, 21.0, 61.0, 101.0, 
								 141.0, 31.0, 71.0, 111.0, 151.0, 41.0, 81.0, 121.0, 161.0);
	m2 = Matrix4x4(
				 m1.m11 * m.m11 + m1.m12 * m.m21 + m1.m13 * m.m31 + m1.m14 * m.m41,
				 m1.m11 * m.m12 + m1.m12 * m.m22 + m1.m13 * m.m32 + m1.m14 * m.m42,
				 m1.m11 * m.m13 + m1.m12 * m.m23 + m1.m13 * m.m33 + m1.m14 * m.m43,
				 m1.m11 * m.m14 + m1.m12 * m.m24 + m1.m13 * m.m34 + m1.m14 * m.m44,

				 m1.m21 * m.m11 + m1.m22 * m.m21 + m1.m23 * m.m31 + m1.m24 * m.m41,
				 m1.m21 * m.m12 + m1.m22 * m.m22 + m1.m23 * m.m32 + m1.m24 * m.m42,
				 m1.m21 * m.m13 + m1.m22 * m.m23 + m1.m23 * m.m33 + m1.m24 * m.m43,
				 m1.m21 * m.m14 + m1.m22 * m.m24 + m1.m23 * m.m34 + m1.m24 * m.m44,
		 
				 m1.m31 * m.m11 + m1.m32 * m.m21 + m1.m33 * m.m31 + m1.m34 * m.m41,
		 		 m1.m31 * m.m12 + m1.m32 * m.m22 + m1.m33 * m.m32 + m1.m34 * m.m42,
		 		 m1.m31 * m.m13 + m1.m32 * m.m23 + m1.m33 * m.m33 + m1.m34 * m.m43,
		 		 m1.m31 * m.m14 + m1.m32 * m.m24 + m1.m33 * m.m34 + m1.m34 * m.m44,
		 
				 m1.m41 * m.m11 + m1.m42 * m.m21 + m1.m43 * m.m31 + m1.m44 * m.m41,
				 m1.m41 * m.m12 + m1.m42 * m.m22 + m1.m43 * m.m32 + m1.m44 * m.m42,
				 m1.m41 * m.m13 + m1.m42 * m.m23 + m1.m43 * m.m33 + m1.m44 * m.m43,
				 m1.m41 * m.m14 + m1.m42 * m.m24 + m1.m43 * m.m34 + m1.m44 * m.m44);
	m3 = m1 * m;
	TEST_EQUAL(m3.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::TMatrix4x4& operator *= (const TMatrix4x4& m))
	m1 = Matrix4x4(11.0, 51.0, 91.0, 131.0, 21.0, 61.0, 101.0, 
								 141.0, 31.0, 71.0, 111.0, 151.0, 41.0, 81.0, 121.0, 161.0);
	m3 = m1 * m;
	m1 *= m;
	TEST_EQUAL(m3.isEqual(m1), true)
RESULT

CHECK(TMatrix4x4::TVector4 operator * (const TVector4<T> &) const )
	Vector4 v(1.0, 2.0, 3.0, 4.0);
	vx = Vector4
			(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * v.h,
			 m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * v.h,
			 m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * v.h,
			 m.m41 * v.x + m.m42 * v.y + m.m43 * v.z + m.m44 * v.h);
	vy = m * v;
	TEST_EQUAL(vy , vx)
RESULT

CHECK(TMatrix4x4::invert(TMatrix4x4& inverse) const )
	bool bool1;
	Matrix4x4 e;
	e.setIdentity();
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	bool1 = m1.invert(m2);
	TEST_EQUAL(bool1 , true)
	m3 = m1 * m2;
	TEST_EQUAL(m3.isEqual(e), true)
RESULT

CHECK(TMatrix4x4::invert())
	bool bool1;
	Matrix4x4 e;
	e.setIdentity();
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	m2 = Matrix4x4(m1);
	bool1 = m1.invert();
	m3 = m1 * m2;
	TEST_EQUAL(m3.isEqual(e), true)
	TEST_EQUAL(bool1 , true)
RESULT

CHECK(TMatrix4x4::getDeterminant() const )
	float d1, d2, d3;
	Vector4 v1, v2, v3, v4;
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	d1 = m1.getDeterminant();//-169.68
	m2 = Matrix4x4(m1);
	v1 = m2.getRow(0);
	v2 = m2.getRow(1);
	v3 = m2.getRow(2);
	v4 = m2.getRow(3);
	m2.setColumn(0, v1);
	m2.setColumn(1, v2);
	m2.setColumn(2, v3);
	m2.setColumn(3, v4);
	d2 = m2.getDeterminant();
	TEST_EQUAL(Maths::isNear(d1, d2, d2 / 100), true)

	m2 = Matrix4x4(m1);
	v1 = m2.getRow(1);
	v2 = m2.getRow(2);
	m2.setRow(1, v2);
	m2.setRow(2, v1);
	d2 = m2.getDeterminant();
	TEST_REAL_EQUAL(-d2, d1)

	m2 = Matrix4x4(m1);
	v1 = m2.getRow(1);
	m2.setRow(2, v1);
	d2 = m2.getDeterminant();
	TEST_REAL_EQUAL(d2, 0)

	m2 = Matrix4x4();
	d2 = m2.getDeterminant();
	TEST_REAL_EQUAL(d2, 0)
	
	m2 = Matrix4x4(m1);
	v1 = m2.getRow(1);
	v1 *= 5;
	m2.setRow(1, v1);
	d2 = m2.getDeterminant();
	TEST_EQUAL(Maths::isNear(d1 * 5, d2, d2 / 100), true)

	m2 = Matrix4x4(m1);
	d2 = m2.getDeterminant();
	m3 = m1 * m2;
	d3 = m3.getDeterminant();
	TEST_EQUAL(Maths::isNear(d1 * d2, d3, d3 / 100), true)
RESULT

CHECK(TMatrix4x4::translate(const T &x, const T &y, const T &z))
	float x = 2.0, y = 3.0, z = 4.0;
	m1 = Matrix4x4(m);
	m1.m14 += m1.m11 * x + m1.m12 * y + m1.m13 * z;
	m1.m24 += m1.m21 * x + m1.m22 * y + m1.m23 * z;
	m1.m34 += m1.m31 * x + m1.m32 * y + m1.m33 * z;
	m1.m44 += m1.m41 * x + m1.m42 * y + m1.m43 * z;
	m2 = Matrix4x4(m);
	m2.translate(x, y, z);
	TEST_EQUAL(m1.isEqual(m2), true)
	x = -2; y = -3; z =-4;
	m2.translate(x, y, z);
	TEST_EQUAL(m.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::translate(const TVector3<T>& v))
	Vector3 v(2.0, 3.0, 4.0);
	m1 = Matrix4x4(m);
	m1.m14 += m1.m11 * v.x + m1.m12 * v.y + m1.m13 * v.z;
	m1.m24 += m1.m21 * v.x + m1.m22 * v.y + m1.m23 * v.z;
	m1.m34 += m1.m31 * v.x + m1.m32 * v.y + m1.m33 * v.z;
	m1.m44 += m1.m41 * v.x + m1.m42 * v.y + m1.m43 * v.z;
	m2 = Matrix4x4(m);
	m2.translate(v);
	TEST_EQUAL(m1.isEqual(m2), true)
	v = -v;
	m2.translate(v);
	TEST_EQUAL(m.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setTranslation(const T& x, const T& y, const T& z))
	float x = 2.0, y = 3.0, z = 4.0;
	m1.set(9.0);
	m1.setTranslation(x, y, z);
	x = m1.m12 * m1.m13 *	m1.m21 * m1.m23 *	m1.m31 * m1.m32 *	m1.m41 * m1.m42 * m1.m43;
	TEST_REAL_EQUAL(x , 0)

	x = m1.m11 * m1.m22 * m1.m33 * m1.m44;
	TEST_REAL_EQUAL(x , 1)

	x = 2.0;
	TEST_REAL_EQUAL(m1.m14 , x)
	TEST_REAL_EQUAL(m1.m24 , y)
	TEST_REAL_EQUAL(m1.m34 , z)
RESULT

CHECK(TMatrix4x4::setTranslation(const TVector3<T>& v))
	Vector3 v(2.0, 3.0, 4.0);
	m1.setTranslation(v);
	m2.setTranslation(2.0, 3.0, 4.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::scale(const T& x_scale, const T& y_scale, const T& z_scale))
	m1 = Matrix4x4(m);
	m1.scale(2.0, 4.0, 5.0);
	m1.scale(0.5, 0.25, 0.2);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4::scale(const T& scale))
	m1 = Matrix4x4(m);
	m1.scale(2.0);
	m1.scale(0.5);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4::scale(const TVector3<T>& v))
	Vector3 v(2.0, 4.0, 5.0);
	m1 = Matrix4x4(m);
	m1.scale(v);
	v = Vector3(0.5, 0.25, 0.2);
	m1.scale(v);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4::setScale(const T& x_scale, const T& y_scale, const T& z_scale))
	m1 = Matrix4x4(m);
	m1.setScale(2.0, 3.0, 4.0);
	float x = m1.m12 * m1.m13 * m1.m14 * m1.m21 * m1.m23 * m1.m24 *
						m1.m31 * m1.m32 * m1.m34 * m1.m41 * m1.m42 * m1.m43;
	TEST_REAL_EQUAL(x , 0)
	TEST_REAL_EQUAL(m1.m11, 2.0)
	TEST_REAL_EQUAL(m1.m22, 3.0)
	TEST_REAL_EQUAL(m1.m33, 4.0)
	TEST_REAL_EQUAL(m1.m44, 1.0)
RESULT

CHECK(TMatrix4x4::setScale(const T& scale))
	m1 = Matrix4x4(m);
	m1.setScale(2.0);
	float x = m1.m12 * m1.m13 * m1.m14 * m1.m21 * m1.m23 * m1.m24 *
						m1.m31 * m1.m32 * m1.m34 * m1.m41 * m1.m42 * m1.m43;
	TEST_REAL_EQUAL(x , 0)
	TEST_REAL_EQUAL(m1.m11, 2.0)
	TEST_REAL_EQUAL(m1.m22, 2.0)
	TEST_REAL_EQUAL(m1.m33, 2.0)
	TEST_REAL_EQUAL(m1.m44, 1.0)
RESULT

CHECK(TMatrix4x4::setScale(const TVector3<T> &))
	m1 = Matrix4x4(m);
	Vector3 v(2.0, 3.0, 4.0);
	m1.setScale(v);
	float x = m1.m12 * m1.m13 * m1.m14 * m1.m21 * m1.m23 * m1.m24 *
						m1.m31 * m1.m32 * m1.m34 * m1.m41 * m1.m42 * m1.m43;
	TEST_REAL_EQUAL(x , 0)
	TEST_REAL_EQUAL(m1.m11, 2.0)
	TEST_REAL_EQUAL(m1.m22, 3.0)
	TEST_REAL_EQUAL(m1.m33, 4.0)
	TEST_REAL_EQUAL(m1.m44, 1.0)
RESULT

CHECK(TMatrix4x4::setRotationX(const Angle& phi))
	m1.setRotationX(a);
	float x = m1.m12 * m1.m13 * m1.m14 * m1.m21 * m1.m24 * m1.m31 * m1.m34 * m1.m41 * m1.m42 * m1.m43;
	TEST_REAL_EQUAL(x, 0)
	TEST_REAL_EQUAL(m1.m11, 1)
	TEST_REAL_EQUAL(m1.m44, 1)
	TEST_REAL_EQUAL(m1.m22, cos(a))
	TEST_REAL_EQUAL(m1.m33, cos(a))
	TEST_REAL_EQUAL(m1.m32, sin(a))
	TEST_REAL_EQUAL(m1.m23, -m1.m32)
RESULT

CHECK(TMatrix4x4::rotateX(const Angle& phi))
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m3.setRotationX(a);
	m2 *= m3;
	m1.rotateX(a);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setRotationY(const Angle& phi))
	m1.setRotationY(a);
	float x =	m1.m12 * m1.m14 * m1.m21 * m1.m23 * m1.m24 * m1.m32 * m1.m34 * m1.m41 * m1.m42 * m1.m43; 
	TEST_REAL_EQUAL(x, 0)
	TEST_REAL_EQUAL(m1.m22, 1)
	TEST_REAL_EQUAL(m1.m44, 1)
	TEST_REAL_EQUAL(m1.m11, cos(a))
	TEST_REAL_EQUAL(m1.m33, cos(a))
	TEST_REAL_EQUAL(m1.m13, sin(a))
	TEST_REAL_EQUAL(m1.m31, -m1.m13)
RESULT

CHECK(TMatrix4x4::rotateY(const Angle &phi))
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m3.setRotationY(a);
	m2 *= m3;
	m1.rotateY(a);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setRotationZ(const Angle &phi))
	m1.setRotationZ(a);
	float x =	m1.m13 * m1.m14 * m1.m23 * m1.m24 * m1.m31 * m1.m32 * m1.m34 * m1.m41 * m1.m42 * m1.m43 ; 
	TEST_REAL_EQUAL(x, 0)
	TEST_REAL_EQUAL(m1.m33, 1)
	TEST_REAL_EQUAL(m1.m44, 1)
	TEST_REAL_EQUAL(m1.m11, cos(a))
	TEST_REAL_EQUAL(m1.m22, cos(a))
	TEST_REAL_EQUAL(m1.m21, sin(a))
	TEST_REAL_EQUAL(m1.m12, -m1.m21)
RESULT

CHECK(TMatrix4x4::rotateZ(const Angle& phi))
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m3.setRotationZ(a);
	m2 *= m3;
	m1.rotateZ(a);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::rotate(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z))
	m2 = m1 = Matrix4x4(m);
	Angle b = Angle(Constants::PI * 2);
	Angle a = Angle(0);
	m1.rotate(b, 1.0, 2.0, 3.0);
	m2.rotate(a, 1.0, 2.0, 3.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::rotate(const Angle& phi, const TVector3<T>& axis))
	Vector3 v(2.0, 3.0, 4.0);
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m1.rotate(a, v.x, v.y, v.z);
	m2.rotate(a, v);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::rotate(const Angle& phi, const TVector4<T>& axis))
	Vector4 v(2.0, 3.0, 4.0, 5.0);
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m1.rotate(a, v.x, v.y, v.z);
	m2.rotate(a, v);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z))
	m1.m12 = m1.m13 = m1.m14 = m1.m21 = m1.m23 = m1.m24 = 
	m1.m31 = m1.m32 = m1.m34 = m1.m41 = m1.m42 = m1.m43 = 0.0;
	m1.m11 = m1.m22 = m1.m33 = m1.m44 = 1.0;
	m1.rotate(a, 2.0, 3.0, 4.0);
	m2.setRotation(a, 2.0, 3.0, 4.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setRotation(const Angle& phi, const TVector3<T>& axis))
	Vector3 v(2.0, 3.0, 4.0);
	m1.m12 = m1.m13 = m1.m14 = m1.m21 = m1.m23 = m1.m24 = 
	m1.m31 = m1.m32 = m1.m34 = m1.m41 = m1.m42 = m1.m43 = 0.0;
	m1.m11 = m1.m22 = m1.m33 = m1.m44 = 1.0;
	m1.rotate(a, 2.0, 3.0, 4.0);
	m2.setRotation(a, v);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::setRotation(const Angle& phi, const TVector4<T>& axis))
	Vector4 v4(2.0, 3.0, 4.0, 1.1);
	m2.setRotation(a, v4);
	Vector3 v3(2.0, 3.0, 4.0);
	m1.setRotation(a, v3);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TMatrix4x4::bool operator != (const TMatrix4x4& m) const )
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1 != m, false)
	m1.m11 = 12.12;
	TEST_EQUAL(m1 != m, true)
RESULT

CHECK(TMatrix4x4::isIdentity() const )
	m1.setIdentity();
	TEST_EQUAL(m1.isIdentity(), true)
	m1.m12 = 12.12;
	TEST_EQUAL(m1.isIdentity(), false)
RESULT

CHECK(TMatrix4x4::isRegular() const )
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isRegular(), false)
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	TEST_EQUAL(m1.isRegular(), true)
RESULT

CHECK(TMatrix4x4::isSingular() const )
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isSingular(), true)
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	TEST_EQUAL(m1.isSingular(), false)
RESULT

CHECK(TMatrix4x4::isSymmetric() const )
	m1 = Matrix4x4(0.0, 2.0, 3.0, 4.0, 2.0, 0.0, 5.0, 6.0, 3.0, 5.0, 0.0, 7.0, 4.0, 6.0, 7.0, 0.0);
	TEST_EQUAL(m1.isSymmetric(), true)
	m1.m12 = 12.12;
	TEST_EQUAL(m1.isSymmetric(), false)
RESULT

CHECK(TMatrix4x4::isLowerTriangular() const )
	m1 = Matrix4x4(1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0);
	TEST_EQUAL(m1.isLowerTriangular(), true)
	m1.m12 = 1;
	TEST_EQUAL(m1.isLowerTriangular(), false)
RESULT

CHECK(TMatrix4x4::isUpperTriangular() const )
	m1 = Matrix4x4(1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	TEST_EQUAL(m1.isUpperTriangular(), true)
	m1.m32 = 1;
	TEST_EQUAL(m1.isUpperTriangular(), false)
RESULT

CHECK(TMatrix4x4::isDiagonal() const )
	m1 = Matrix4x4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	TEST_EQUAL(m1.isDiagonal(), true)
	m1.m32 = 1;
	TEST_EQUAL(m1.isDiagonal(), false)
RESULT

CHECK(TMatrix4x4::isEqual(TMatrix4x4<T>& m, T maxDiff) const )
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isEqual(m), true)
		m1.m41 = 12.12;
	TEST_EQUAL(m1.isEqual(m), false)
RESULT

CHECK(TMatrix4x4::isValid() const )
	TEST_EQUAL(m1.isValid(), true)
RESULT

CHECK(TMatrix4x4::dump(std::ostream& s = std::cout, Size depth = 0) const )
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	m.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Matrix4x4_test.txt", true)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m))
	std::ifstream instr("data/Matrix4x4_test2.txt");
	instr >> m1;
	instr.close();
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m))
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << m;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Matrix4x4_test2.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
