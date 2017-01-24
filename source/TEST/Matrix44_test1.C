// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/angle.h>
#include <BALL/common.h>
#include <cmath>
///////////////////////////

START_TEST(Matrix44)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
using std::ofstream;
using std::ios;
PRECISION(5e-4)
Constants::EPSILON = TEST::precision;

CHECK(BALL_CREATE(TMatrix4x4))
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

Matrix4x4* m_ptr = 0;
CHECK(TMatrix4x4() throw())
	m_ptr = new Matrix4x4();
	TEST_NOT_EQUAL(m_ptr, 0)
RESULT

CHECK(~TMatrix4x4() throw())
	delete m_ptr;
RESULT		

CHECK(TMatrix4x4(const T& m11, const T& m12, const T& m13, const T& m14, const T& m21, const T& m22, const T& m23, const T& m24, const T& m31, const T& m32, const T& m33, const T& m34, const T& m41, const T& m42, const T& m43, const T& m44) throw())
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

CHECK(bool operator == (const TMatrix4x4& m) const throw())
  Matrix4x4 m1(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	TEST_EQUAL(m == m1 , true)
  m1 = Matrix4x4(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.1);
	TEST_EQUAL(m == m1 , false)
RESULT

CHECK(TMatrix4x4(const TMatrix4x4& m) throw())
	m1 = Matrix4x4(m);
	TEST_EQUAL(m == m1 , true)
RESULT

CHECK(TMatrix4x4(const T* ptr) throw(Exception::NullPointer))
	float arr[16];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1.0;
	}
	m1 = Matrix4x4(arr);
	TEST_EQUAL(m == m1 , true)
	TEST_EXCEPTION(Exception::NullPointer, m1 = Matrix4x4((float*)0))
RESULT

CHECK(TMatrix4x4(const T ptr[4][4]) throw(Exception::NullPointer))
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

CHECK(TMatrix4x4(const TVector4<T>& col1, const TVector4<T>& col2, const TVector4<T>& col3, const TVector4<T>& col4) throw())
	m1 = Matrix4x4(v, v1, v2, v3);
	TEST_EQUAL(m == m1 , true)
RESULT

CHECK(void set(const T* ptr) throw(Exception::NullPointer))
	m1 = Matrix4x4();
	
	float arr[16];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1;
	}
	
	m1.set(arr);
	TEST_EQUAL(m1.isEqual(m), true)
	TEST_EXCEPTION(Exception::NullPointer, m1.set((float*)0))
RESULT


CHECK(void set(const T ptr[4][4]) throw(Exception::NullPointer))
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


CHECK(void set(const TMatrix4x4& m) throw())
	m1 = Matrix4x4();
	m1.set(m);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void set(const TVector4<T>& col1, const TVector4<T>& col2, const TVector4<T>& col3, const TVector4<T>& col4) throw())
	m1 = Matrix4x4();
	m1.set(v, v1, v2, v3);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void set(const T& m11, const T& m12, const T& m13, const T& m14, const T& m21, const T& m22, const T& m23, const T& m24, const T& m31, const T& m32, const T& m33, const T& m34, const T& m41, const T& m42, const T& m43, const T& m44) throw())
	m1 = Matrix4x4();
	m1.set(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4& operator = (const T* ptr) throw(Exception::NullPointer))
	m1 = Matrix4x4();
	float arr[16];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1.0;
	}
	m1 = arr;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4& operator = (const T ptr[4][4]) throw(Exception::NullPointer))
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

CHECK(TMatrix4x4& operator = (const TMatrix4x4& m) throw())
	m1 = m;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void get(T* ptr) const throw(Exception::NullPointer))
	m1 = Matrix4x4();
	float arr[16];
	m.get(arr);
	m1.set(arr);
	TEST_EQUAL(m1.isEqual(m), true)
	TEST_EXCEPTION(Exception::NullPointer, m1.get((float*)0))
RESULT

CHECK(void get(T ptr[4][4]) const throw(Exception::NullPointer))
	m1 = Matrix4x4();
	float arr[4][4];
	m.get(arr);
	m1.set(arr);
	TEST_EQUAL(m1.isEqual(m), true);
	TEST_EXCEPTION(Exception::NullPointer, m1.get((float*)0))
RESULT

CHECK(void get(TMatrix4x4& m) const throw())
	m1 = Matrix4x4();
	m.get(m1);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void get(TVector4<T>& col1, TVector4<T>& col2, TVector4<T>& col3, TVector4<T>& col4) const throw())
	Vector4 z1, z2, z3, z4;
	m1 = Matrix4x4();
	m.get(z1, z2, z3, z4);
	m1.set(z1, z2, z3, z4);
	TEST_EQUAL(m1, m)
RESULT

CHECK(void get(T& m11, T& m12, T& m13, T& m14, T& m21, T& m22, T& m23, T& m24, T& m31, T& m32, T& m33, T& m34, T& m41, T& m42, T& m43, T& m44) const throw())
	m1 = Matrix4x4();
	float a[16];
	m.get(a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[8],
				a[9], a[10], a[11], a[12], a[13], a[14], a[15]);
	m1.set(a);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void swap(TMatrix4x4& m) throw())
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	m3 = m2;
	m1.swap(m2);
	TEST_EQUAL(m2.isEqual(m), true)
	TEST_EQUAL(m3.isEqual(m1), true)
RESULT

CHECK(T getTrace() const throw())
	TEST_EQUAL(m.getTrace(), 34.0)
RESULT

CHECK(static const TMatrix4x4& getZero() throw())
	const Matrix4x4 c = m1.getZero();
	m1 = Matrix4x4();
	m1.m11 = m1.m22 = m1.m33 = m1.m44 = 0;
	TEST_EQUAL(m1.isEqual(c), true)
RESULT

CHECK(static const TMatrix4x4& getIdentity() throw())
	const Matrix4x4 c = m1.getIdentity(); 
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	TEST_EQUAL(c.isEqual(m2), true)
RESULT

CHECK(void setIdentity() throw())
	m1.setIdentity();
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void clear() throw())
	m1.clear();
	m2 = Matrix4x4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void set(const T& t = (T)1) throw())
	m1.set(2.0);
	m2 = Matrix4x4(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void transpose() throw())
	m1 = Matrix4x4(m);
	m1.transpose();
	m2 = Matrix4x4(1.0, 5.0, 9.0, 13.0, 2.0, 6.0, 10.0, 14.0, 3.0, 7.0, 11.0, 15.0, 4.0, 8.0, 12.0, 16.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(TVector4<T> getRow(Position row) const throw(Exception::IndexOverflow))
	m1 = Matrix4x4(m);
	vx = m1.getRow(0);
	TEST_EQUAL(vx , v)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.getRow(4))
RESULT

CHECK(TVector4<T> getColumn(Position col) const throw(Exception::IndexOverflow))
	m1 = Matrix4x4(m);
	vx = m1.getColumn(0);
	vy = Vector4(1.0, 5.0, 9.0, 13.0);
	TEST_EQUAL(vx , vy)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.getColumn(4))
RESULT

CHECK(void setRow(Position row, const TVector4<T>& row_value) throw(Exception::IndexOverflow))
	Vector4 x(0.1, 0.2, 0.3, 0.4);
	m1 = Matrix4x4(m);
	m1.setRow(0, x);
	vx = m1.getRow(0);
	TEST_EQUAL(x , vx)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.setRow(4, vx))
RESULT


CHECK(void setColumn(Position col, const TVector4<T>& col_value) throw(Exception::IndexOverflow))
	Vector4 x(0.1, 0.2, 0.3, 0.4);
	m1 = Matrix4x4(m);
	m1.setColumn(0, x);
	vx = m1.getColumn(0);
	TEST_EQUAL(x , vx)
	TEST_EXCEPTION(Exception::IndexOverflow, m1.setColumn(4, vx))
RESULT

CHECK(TVector4<T> getDiagonal() const throw())
	vx = m.getDiagonal();
	vy = Vector4(1.0, 6.0, 11.0, 16.0);
	TEST_EQUAL(vx , vy)
RESULT

CHECK(const T& operator () (Position row, Position col) const throw(Exception::IndexOverflow))
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
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m22 = c_m1(std::numeric_limits<Position>::max(), 1))
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m22 = c_m1(1, std::numeric_limits<Position>::max()))
RESULT

CHECK(T& operator () (Position row, Position col) throw(Exception::IndexOverflow))
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
	TEST_EXCEPTION(Exception::IndexOverflow, m1(std::numeric_limits<Position>::max(), 1))
	TEST_EXCEPTION(Exception::IndexOverflow, m1(1, std::numeric_limits<Position>::max()))
RESULT

CHECK(const T& operator [] (Position position) const throw(Exception::IndexOverflow))
	const Matrix4x4& c_m1 = const_cast<const Matrix4x4&>(m1);
	TEST_REAL_EQUAL(c_m1[0], m1.m11)
	TEST_REAL_EQUAL(c_m1[1], m1.m12)
	TEST_REAL_EQUAL(c_m1[2], m1.m13)
	TEST_REAL_EQUAL(c_m1[3], m1.m14)
	TEST_REAL_EQUAL(c_m1[4], m1.m21)
	TEST_REAL_EQUAL(c_m1[5], m1.m22)
	TEST_REAL_EQUAL(c_m1[6], m1.m23)
	TEST_REAL_EQUAL(c_m1[7], m1.m24)
	TEST_REAL_EQUAL(c_m1[8], m1.m31)
	TEST_REAL_EQUAL(c_m1[9], m1.m32)
	TEST_REAL_EQUAL(c_m1[10], m1.m33)
	TEST_REAL_EQUAL(c_m1[11], m1.m34)
	TEST_REAL_EQUAL(c_m1[12], m1.m41)
	TEST_REAL_EQUAL(c_m1[13], m1.m42)
	TEST_REAL_EQUAL(c_m1[14], m1.m43)
	TEST_REAL_EQUAL(c_m1[15], m1.m44)

	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = c_m1[16])
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = c_m1[std::numeric_limits<Position>::max()])
RESULT


CHECK(T& operator [] (Position position) throw(Exception::IndexOverflow))
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
	TEST_EXCEPTION(Exception::IndexOverflow, m1.m44 = m1[std::numeric_limits<Position>::max()])
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
