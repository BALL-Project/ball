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


const Matrix4x4 m(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
Matrix4x4 m1, m2, m3;
const Vector4 v(1.0, 2.0, 3.0, 4.0), v1(5.0, 6.0, 7.0, 8.0), v2(9.0, 10.0, 11.0, 12.0), v3(13.0, 14.0, 15.0, 16.0);
Vector4 vx, vy;
const Angle a = Angle(1.0, true);


CHECK(TMatrix4x4 operator + () const throw())
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isEqual( + m), true)
RESULT

CHECK(TMatrix4x4 operator - () const throw())
	m1 = Matrix4x4(-1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0, -11.0, -12.0, -13.0, -14.0, -15.0, -16.0);
	TEST_EQUAL(m1.isEqual(- m), true)
RESULT

CHECK(TMatrix4x4 operator + (const TMatrix4x4& m) const throw())
	m1 = Matrix4x4() + m;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4& operator += (const TMatrix4x4& m) throw())
	m1 = Matrix4x4();
	m1 += m;
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(TMatrix4x4 operator - (const TMatrix4x4& m) const throw())
	m1 = Matrix4x4() - m;
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4& operator -= (const TMatrix4x4& m) throw())
	m1 = Matrix4x4();
	m1 -= m;
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4 operator * (const T& scalar) const throw())
	m1 = Matrix4x4(m) * (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4& operator *= (const T& scalar) throw())
	m1 = Matrix4x4(m);
	m1 *= (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
RESULT

CHECK(TMatrix4x4 operator / (const T& scalar) const throw(Exception::DivisionByZero))
	m1 = Matrix4x4(m) / (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
	TEST_EXCEPTION(Exception::DivisionByZero, m1 / 0)
RESULT

CHECK(TMatrix4x4& operator /= (const T& scalar) throw(Exception::DivisionByZero))
	m1 = Matrix4x4(m);
	m1 /= (-1.0);
	TEST_EQUAL(m1.isEqual(-m), true)
	TEST_EXCEPTION(Exception::DivisionByZero, m1 /= 0)
RESULT

CHECK(TMatrix4x4 operator * (const TMatrix4x4& m) const throw())
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

CHECK(TMatrix4x4& operator *= (const TMatrix4x4& m) throw())
	m1 = Matrix4x4(11.0, 51.0, 91.0, 131.0, 21.0, 61.0, 101.0, 
								 141.0, 31.0, 71.0, 111.0, 151.0, 41.0, 81.0, 121.0, 161.0);
	m3 = m1 * m;
	m1 *= m;
	TEST_EQUAL(m3.isEqual(m1), true)
RESULT

CHECK(TVector4<T> operator * (const TVector4<T>& vector) const throw())
	Vector4 v(1.0, 2.0, 3.0, 4.0);
	vx = Vector4
			(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * v.h,
			 m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * v.h,
			 m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * v.h,
			 m.m41 * v.x + m.m42 * v.y + m.m43 * v.z + m.m44 * v.h);
	vy = m * v;
	TEST_EQUAL(vy , vx)
RESULT

CHECK(bool invert(TMatrix4x4& inverse) const throw())
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

CHECK(bool invert() throw())
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

CHECK(T getDeterminant() const throw())
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

CHECK(void translate(const T &x, const T &y, const T &z) throw())
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

CHECK(void translate(const TVector3<T>& v) throw())
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

CHECK(void setTranslation(const T& x, const T& y, const T& z) throw())
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

CHECK(void setTranslation(const TVector3<T>& v) throw())
	Vector3 v(2.0, 3.0, 4.0);
	m1.setTranslation(v);
	m2.setTranslation(2.0, 3.0, 4.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void scale(const T& x_scale, const T& y_scale, const T& z_scale) throw())
	m1 = Matrix4x4(m);
	m1.scale(2.0, 4.0, 5.0);
	m1.scale(0.5, 0.25, 0.2);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void scale(const T& scale) throw())
	m1 = Matrix4x4(m);
	m1.scale(2.0);
	m1.scale(0.5);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void scale(const TVector3<T>& v) throw())
	Vector3 v(2.0, 4.0, 5.0);
	m1 = Matrix4x4(m);
	m1.scale(v);
	v = Vector3(0.5, 0.25, 0.2);
	m1.scale(v);
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

CHECK(void setScale(const T& x_scale, const T& y_scale, const T& z_scale) throw())
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

CHECK(void setScale(const T& scale) throw())
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

CHECK(void setScale(const TVector3<T>& v) throw())
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

CHECK(void setRotationX(const TAngle<T>& phi) throw())
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

CHECK(void rotateX(const TAngle<T>& phi) throw())
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m3.setRotationX(a);
	m2 *= m3;
	m1.rotateX(a);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void setRotationY(const TAngle<T>& phi) throw())
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

CHECK(void rotateY(const TAngle<T>& phi) throw())
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m3.setRotationY(a);
	m2 *= m3;
	m1.rotateY(a);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void setRotationZ(const TAngle<T>& phi) throw())
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

CHECK(void rotateZ(const TAngle<T>& phi) throw())
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m3.setRotationZ(a);
	m2 *= m3;
	m1.rotateZ(a);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void rotate(const TAngle<T>& phi, const T& axis_x, const T& axis_y, const T& axis_z) throw())
	m2 = m1 = Matrix4x4(m);
	Angle b = Angle(Constants::PI * 2);
	Angle a = Angle(0);
	m1.rotate(b, 1.0, 2.0, 3.0);
	m2.rotate(a, 1.0, 2.0, 3.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void rotate(const TAngle<T>& phi, const TVector3<T>& axis) throw())
	Vector3 v(2.0, 3.0, 4.0);
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m1.rotate(a, v.x, v.y, v.z);
	m2.rotate(a, v);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void rotate(const TAngle<T>& phi, const TVector4<T>& axis) throw())
	Vector4 v(2.0, 3.0, 4.0, 5.0);
	m1 = Matrix4x4(m);
	m2 = Matrix4x4(m);
	m1.rotate(a, v.x, v.y, v.z);
	m2.rotate(a, v);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void setRotation(const TAngle<T>& phi, const T& axis_x, const T& axis_y, const T& axis_z) throw())
	m1.m12 = m1.m13 = m1.m14 = m1.m21 = m1.m23 = m1.m24 = 
	m1.m31 = m1.m32 = m1.m34 = m1.m41 = m1.m42 = m1.m43 = 0.0;
	m1.m11 = m1.m22 = m1.m33 = m1.m44 = 1.0;
	m1.rotate(a, 2.0, 3.0, 4.0);
	m2.setRotation(a, 2.0, 3.0, 4.0);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void setRotation(const TAngle<T>& phi, const TVector3<T>& axis) throw())
	Vector3 v(2.0, 3.0, 4.0);
	m1.m12 = m1.m13 = m1.m14 = m1.m21 = m1.m23 = m1.m24 = 
	m1.m31 = m1.m32 = m1.m34 = m1.m41 = m1.m42 = m1.m43 = 0.0;
	m1.m11 = m1.m22 = m1.m33 = m1.m44 = 1.0;
	m1.rotate(a, 2.0, 3.0, 4.0);
	m2.setRotation(a, v);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(void setRotation(const TAngle<T>& phi, const TVector4<T>& axis) throw())
	Vector4 v4(2.0, 3.0, 4.0, 1.1);
	m2.setRotation(a, v4);
	Vector3 v3(2.0, 3.0, 4.0);
	m1.setRotation(a, v3);
	TEST_EQUAL(m1.isEqual(m2), true)
RESULT

CHECK(bool operator != (const TMatrix4x4& m) const throw())
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1 != m, false)
	m1.m11 = 12.12;
	TEST_EQUAL(m1 != m, true)
RESULT

CHECK(bool isIdentity() const throw())
	m1.setIdentity();
	TEST_EQUAL(m1.isIdentity(), true)
	m1.m12 = 12.12;
	TEST_EQUAL(m1.isIdentity(), false)
RESULT

CHECK(bool isRegular() const throw())
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isRegular(), false)
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	TEST_EQUAL(m1.isRegular(), true)
RESULT

CHECK(bool isSingular() const throw())
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isSingular(), true)
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	TEST_EQUAL(m1.isSingular(), false)
RESULT

CHECK(bool isSymmetric() const throw())
	m1 = Matrix4x4(0.0, 2.0, 3.0, 4.0, 2.0, 0.0, 5.0, 6.0, 3.0, 5.0, 0.0, 7.0, 4.0, 6.0, 7.0, 0.0);
	TEST_EQUAL(m1.isSymmetric(), true)
	m1.m12 = 12.12;
	TEST_EQUAL(m1.isSymmetric(), false)
RESULT

CHECK(bool isLowerTriangular() const throw())
	m1 = Matrix4x4(1.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0);
	TEST_EQUAL(m1.isLowerTriangular(), true)
	m1.m12 = 1;
	TEST_EQUAL(m1.isLowerTriangular(), false)
RESULT

CHECK(bool isUpperTriangular() const throw())
	m1 = Matrix4x4(1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	TEST_EQUAL(m1.isUpperTriangular(), true)
	m1.m32 = 1;
	TEST_EQUAL(m1.isUpperTriangular(), false)
RESULT

CHECK(bool isDiagonal() const throw())
	m1 = Matrix4x4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	TEST_EQUAL(m1.isDiagonal(), true)
	m1.m32 = 1;
	TEST_EQUAL(m1.isDiagonal(), false)
RESULT

CHECK(bool isEqual(const TMatrix4x4& m) const throw())
	m1 = Matrix4x4(m);
	TEST_EQUAL(m1.isEqual(m), true)
		m1.m41 = 12.12;
	TEST_EQUAL(m1.isEqual(m), false)
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(m1.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	m.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Matrix4x4_test.txt))
RESULT

CHECK(std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m))
	std::ifstream instr(BALL_TEST_DATA_PATH(Matrix4x4_test2.txt));
	instr >> m1;
	instr.close();
	TEST_EQUAL(m1.isEqual(m), true)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m))
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << m;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Matrix4x4_test2.txt))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
