// $Id: Matrix44_test.C,v 1.2 2000/03/10 15:28:03 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

///////////////////////////

START_TEST(class_name, "$Id: Matrix44_test.C,v 1.2 2000/03/10 15:28:03 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String filename;
using std::ofstream;
using std::ios;


//line 60: method TMatrix4x4::BALL_CREATE(TMatrix4x4)
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
	float arr[15];
	for (int i = 0; i <16; i++ )
	{
		arr[i] = (float)i + 1;
	}
	m1 = Matrix4x4(arr);
	TEST_EQUAL(m == m1 , true)
	TEST_EXCEPTION(Exception::NullPointer, m1 = Matrix4x4((float*)0))
RESULT

/*
CHECK(TMatrix4x4(const T* ptr[4]);)
RESULT
*/

CHECK(TMatrix4x4(const TVector4<T>& col1, const TVector4<T>& col2,
		 const TVector4<T>& col3,const TVector4<T>& col4))
	m1 = Matrix4x4(v, v1, v2, v3);
	TEST_EQUAL(m == m1 , true)
RESULT


//line 239: method TMatrix4x4::getTrace() const 
CHECK(TMatrix4x4::getTrace() const )
	TEST_EQUAL(m.getTrace(), 34.0)
RESULT


//line 251: method TMatrix4x4::setIdentity()
CHECK(TMatrix4x4::setIdentity())
	m1.setIdentity();
	m2 = Matrix4x4(1.0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	TEST_EQUAL(m1 , m2)
RESULT


//line 256: method TMatrix4x4::fill(const T& t = (T)1)
CHECK(TMatrix4x4::fill(const T& t = (T)1))
	m1.fill(2.0);
	m2 = Matrix4x4(2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0);
	TEST_EQUAL(m1 , m2)
RESULT


//line 262: method TMatrix4x4::transpose()
CHECK(TMatrix4x4::transpose())
	m1 = Matrix4x4(m);
	m1.transpose();
	m2 = Matrix4x4(1.0, 5.0, 9.0, 13.0, 2.0, 6.0, 10.0, 14.0, 3.0, 7.0, 11.0, 15.0, 4.0, 8.0, 12.0, 16.0);
	TEST_EQUAL(m1 , m2)
RESULT


//line 270: method TMatrix4x4::getRow(Index row)
CHECK(TMatrix4x4::getRow(Index row))
	m1 = Matrix4x4(m);
	vx = m1.getRow(0);
	TEST_EQUAL(vx , v)
RESULT


//line 278: method TMatrix4x4::getColumn(Index col)
CHECK(TMatrix4x4::getColumn(Index col))
	m1 = Matrix4x4(m);
	vx = m1.getColumn(0);
	vy = Vector4(1.0, 5.0, 9.0, 13.0);
	TEST_EQUAL(vx , vy)
RESULT


//line 283: method TMatrix4x4::getDiagonal() const 
CHECK(TMatrix4x4::getDiagonal() const )
	vx = m.getDiagonal();
	vy = Vector4(1.0, 6.0, 11.0, 16.0);
	TEST_EQUAL(vx , vy)
RESULT


//line 292: method TMatrix4x4::T& operator ()(Index row, Index col)
CHECK(TMatrix4x4::T& operator ()(Index row, Index col))
	m1(3,3) = 1.1;
	TEST_REAL_EQUAL(m1.m44, 1.1);
RESULT


//line 301: method TMatrix4x4::T& operator ()(Index row, Index col) const 
CHECK(TMatrix4x4::T& operator ()(Index row, Index col) const )
	TEST_REAL_EQUAL(m(3,3) , 16)
RESULT


//line 305: method TMatrix4x4::TMatrix4x4 operator + () const 
CHECK(TMatrix4x4::TMatrix4x4 operator + () const )
	m1 = Matrix4x4(m);
	TEST_EQUAL(+ m1 , m)
RESULT


//line 309: method TMatrix4x4::TMatrix4x4 operator - () const 
CHECK(TMatrix4x4::TMatrix4x4 operator - () const )
	m1 = Matrix4x4(-1.0, -2.0, -3.0, -4.0, -5.0, -6.0, -7.0, -8.0, -9.0, -10.0, -11.0, -12.0, -13.0, -14.0, -15.0, -16.0);
	TEST_EQUAL(- m1 , m)
RESULT


//line 316: method TMatrix4x4::TMatrix4x4 operator + (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::TMatrix4x4 operator + (const TMatrix4x4& m) const )
	m1 = Matrix4x4() + m;
	TEST_EQUAL(m1 , m)
RESULT


//line 323: method TMatrix4x4::TMatrix4x4& operator += (const TMatrix4x4& m)
CHECK(TMatrix4x4::TMatrix4x4& operator += (const TMatrix4x4& m))
	m1 = Matrix4x4();
	m1 += m;
	TEST_EQUAL(m1 , m)
RESULT


//line 330: method TMatrix4x4::TMatrix4x4 operator - (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::TMatrix4x4 operator - (const TMatrix4x4& m) const )
	m1 = Matrix4x4() - m;
	TEST_EQUAL(m1 , - m)
RESULT


//line 337: method TMatrix4x4::TMatrix4x4& operator -= (const TMatrix4x4& m)
CHECK(TMatrix4x4::TMatrix4x4& operator -= (const TMatrix4x4& m))
	m1 = Matrix4x4();
	m1 -= m;
	TEST_EQUAL(m1 , - m)
RESULT


//line 342: method TMatrix4x4::TMatrix4x4 operator * (const T& scalar) const 
CHECK(TMatrix4x4::TMatrix4x4 operator * (const T& scalar) const )
	m1 = Matrix4x4(m) * (-1.0);
	TEST_EQUAL(m1 , -m)
RESULT


//line 347: method TMatrix4x4::TMatrix4x4& operator *= (const T& scalar)
CHECK(TMatrix4x4::TMatrix4x4& operator *= (const T& scalar))
	m1 = Matrix4x4(m);
	m1 *= (-1.0);
	TEST_EQUAL(m1 , -m)
RESULT


//line 353: method TMatrix4x4::TMatrix4x4 operator / (const T& scalar) const 
CHECK(TMatrix4x4::TMatrix4x4 operator / (const T& scalar) const )
	m1 = Matrix4x4(m) / (-1.0);
	TEST_EQUAL(m1 , -m)
RESULT


//line 359: method TMatrix4x4::TMatrix4x4& operator /= (const T& scalar)
CHECK(TMatrix4x4::TMatrix4x4& operator /= (const T& scalar))
	m1 = Matrix4x4(m);
	m1 /= (-1.0);
	TEST_EQUAL(m1 , -m)
RESULT


//line 364: method TMatrix4x4::TMatrix4x4 operator * (const TMatrix4x4& m) const 
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
	TEST_EQUAL(m3 , m2)
RESULT


//line 369: method TMatrix4x4::TMatrix4x4& operator *= (const TMatrix4x4& m)
CHECK(TMatrix4x4::TMatrix4x4& operator *= (const TMatrix4x4& m))
	m1 = Matrix4x4(11.0, 51.0, 91.0, 131.0, 21.0, 61.0, 101.0, 
								 141.0, 31.0, 71.0, 111.0, 151.0, 41.0, 81.0, 121.0, 161.0);
	m3 = m1 * m;
	m1 *= m;
	TEST_EQUAL(m3 , m1)
RESULT


//line 372: method TMatrix4x4::TVector4 operator * (const TVector4<T> &) const 
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


//line 381: method TMatrix4x4::invert(TMatrix4x4& inverse) const 
CHECK(TMatrix4x4::invert(TMatrix4x4& inverse) const )
	bool bool1;
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	bool1 = m1.invert(m2);
	TEST_EQUAL(bool1 , true)
	m3 = m1 * m2;
	TEST_EQUAL(m3.isIdentity(), true)
RESULT


//line 388: method TMatrix4x4::invert()
CHECK(TMatrix4x4::invert())
	bool bool1;
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	m2 = Matrix4x4(m1);
	bool1 = m1.invert();
	m3 = m1 * m2;
	TEST_EQUAL(m3.isIdentity(), true)
	TEST_EQUAL(bool1 , true)
RESULT


//line 393: method TMatrix4x4::getDeterminant() const 
CHECK(TMatrix4x4::getDeterminant() const )
	float d1, d2;
	Vector4 v1, v2;
	m1 = Matrix4x4(1.0, 2.1, 3.1, 0.2, 5.0, 6.0 ,0.0, 
				8.0 ,9.0 ,0.1 ,11.0 ,12.2 ,13.0 ,14.0 ,15.0 ,16.0 );
	d1 = m1.getDeterminant();//-169.68
	m2 = Matrix4x4(m1);
	v1 = m2.getRow(1);
	v2 = m2.getColumn(1);
	v1.swap(v2);
	TEST_EQUAL(m2, m1)
RESULT


//line 396: method TMatrix4x4::translate(const T &x, const T &y, const T &z)
CHECK(TMatrix4x4::translate(const T &x, const T &y, const T &z))
	float x = 2.0, y = 3.0, z = 4.0;
	m1 = Matrix4x4(m);
	m1.m14 += m1.m11 * x + m1.m12 * y + m1.m13 * z;
	m1.m24 += m1.m21 * x + m1.m22 * y + m1.m23 * z;
	m1.m34 += m1.m31 * x + m1.m32 * y + m1.m33 * z;
	m1.m44 += m1.m41 * x + m1.m42 * y + m1.m43 * z;
	m2 = Matrix4x4(m);
	m2.translate(x, y, z);
	TEST_EQUAL(m2 , m1)
	x = -2; y = -3; z =-4;
	m2.translate(x, y, z);
	TEST_EQUAL(m2 , m)
RESULT


//line 399: method TMatrix4x4::translate(const TVector3<T>& v)
CHECK(TMatrix4x4::translate(const TVector3<T>& v))
	Vector3 v(2.0, 3.0, 4.0);
	m1 = Matrix4x4(m);
	m1.m14 += m1.m11 * v.x + m1.m12 * v.y + m1.m13 * v.z;
	m1.m24 += m1.m21 * v.x + m1.m22 * v.y + m1.m23 * v.z;
	m1.m34 += m1.m31 * v.x + m1.m32 * v.y + m1.m33 * v.z;
	m1.m44 += m1.m41 * v.x + m1.m42 * v.y + m1.m43 * v.z;
	m2 = Matrix4x4(m);
	m2.translate(v);
	TEST_EQUAL(m2 , m1)
	v = -v;
	m2.translate(v);
	TEST_EQUAL(m2 , m)
RESULT


//line 402: method TMatrix4x4::setTranslation(const T& x, const T& y, const T& z)
CHECK(TMatrix4x4::setTranslation(const T& x, const T& y, const T& z))
	float x = 2.0, y = 3.0, z = 4.0;
	m1.fill(9.0);
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


//line 405: method TMatrix4x4::setTranslation(const TVector3<T>& v)
CHECK(TMatrix4x4::setTranslation(const TVector3<T>& v))
	Vector3 v(2.0, 3.0, 4.0);
	m1.setTranslation(v);
	m2.setTranslation(2.0, 3.0, 4.0);
	TEST_EQUAL(m1, m2)
RESULT


//line 408: method TMatrix4x4::getTranslation(const T& x, const T& y, const T& z, TMatrix4x4& m)
CHECK(TMatrix4x4::getTranslation(const T& x, const T& y, const T& z, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 411: method TMatrix4x4::getTranslation(const TVector3<T>& v, TMatrix4x4& m)
CHECK(TMatrix4x4::getTranslation(const TVector3<T>& v, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 414: method TMatrix4x4::scale(const T& x_scale, const T& y_scale, const T& z_scale)
CHECK(TMatrix4x4::scale(const T& x_scale, const T& y_scale, const T& z_scale))
	m1 = Matrix4x4(m);
	m1.scale(2.0, 4.0, 5.0);
	m1.scale(0.5, 0.25, 0.2);
	TEST_EQUAL(m1, m)
RESULT


//line 417: method TMatrix4x4::scale(const T& scale)
CHECK(TMatrix4x4::scale(const T& scale))
	m1 = Matrix4x4(m);
	m1.scale(2.0);
	m1.scale(0.5);
	TEST_EQUAL(m1, m)
RESULT


//line 420: method TMatrix4x4::scale(const TVector3<T>& v)
CHECK(TMatrix4x4::scale(const TVector3<T>& v))
	Vector3 v(2.0, 4.0, 5.0);
	m1 = Matrix4x4(m);
	m1.scale(v);
	v = Vector3(0.5, 0.25, 0.2);
	m1.scale(v);
	TEST_EQUAL(m1, m)
RESULT


//line 423: method TMatrix4x4::setScale(const T& x_scale, const T& y_scale, const T& z_scale)
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


//line 426: method TMatrix4x4::setScale(const T& scale)
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


//line 429: method TMatrix4x4::setScale(const TVector3<T> &)
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


//line 432: method TMatrix4x4::getScale(const T& x_scale, const T& y_scale, const T& z_scale, TMatrix4x4& m)
CHECK(TMatrix4x4::getScale(const T& x_scale, const T& y_scale, const T& z_scale, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 435: method TMatrix4x4::getScale(const T& scale, TMatrix4x4& m)
CHECK(TMatrix4x4::getScale(const T& scale, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 438: method TMatrix4x4::getScale(const TVector3<T>& v, TMatrix4x4& m)
CHECK(TMatrix4x4::getScale(const TVector3<T>& v, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 441: method TMatrix4x4::getScale(const TVector4<T>& v, TMatrix4x4& m)
CHECK(TMatrix4x4::getScale(const TVector4<T>& v, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 444: method TMatrix4x4::rotateX(const Angle& phi)
CHECK(TMatrix4x4::rotateX(const Angle& phi))
  //BAUSTELLE
RESULT


//line 447: method TMatrix4x4::setRotationX(const Angle& phi)
CHECK(TMatrix4x4::setRotationX(const Angle& phi))
  //BAUSTELLE
RESULT


//line 450: method TMatrix4x4::getRotationX(const Angle& phi, TMatrix4x4& m)
CHECK(TMatrix4x4::getRotationX(const Angle& phi, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 453: method TMatrix4x4::rotateY(const Angle& phi)
CHECK(TMatrix4x4::rotateY(const Angle& phi))
  //BAUSTELLE
RESULT


//line 456: method TMatrix4x4::setRotationY(const Angle& phi)
CHECK(TMatrix4x4::setRotationY(const Angle& phi))
  //BAUSTELLE
RESULT


//line 459: method TMatrix4x4::getRotationY(const Angle& phi, TMatrix4x4& m)
CHECK(TMatrix4x4::getRotationY(const Angle& phi, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 462: method TMatrix4x4::rotateZ(const Angle &phi)
CHECK(TMatrix4x4::rotateZ(const Angle &phi))
  //BAUSTELLE
RESULT


//line 465: method TMatrix4x4::setRotationZ(const Angle &phi)
CHECK(TMatrix4x4::setRotationZ(const Angle &phi))
  //BAUSTELLE
RESULT


//line 468: method TMatrix4x4::getRotationZ(const Angle& phi, TMatrix4x4& m)
CHECK(TMatrix4x4::getRotationZ(const Angle& phi, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 471: method TMatrix4x4::rotate(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z)
CHECK(TMatrix4x4::rotate(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z))
  //BAUSTELLE
RESULT


//line 474: method TMatrix4x4::rotate(const Angle& phi, const TVector3<T>& axis)
CHECK(TMatrix4x4::rotate(const Angle& phi, const TVector3<T>& axis))
  //BAUSTELLE
RESULT


//line 477: method TMatrix4x4::rotate(const Angle& phi, const TVector4<T>& axis)
CHECK(TMatrix4x4::rotate(const Angle& phi, const TVector4<T>& axis))
  //BAUSTELLE
RESULT


//line 480: method TMatrix4x4::setRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z)
CHECK(TMatrix4x4::setRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z))
  //BAUSTELLE
RESULT


//line 483: method TMatrix4x4::setRotation(const Angle& phi, const TVector3<T>& axis)
CHECK(TMatrix4x4::setRotation(const Angle& phi, const TVector3<T>& axis))
  //BAUSTELLE
RESULT


//line 486: method TMatrix4x4::setRotation(const Angle& phi, const TVector4<T>& axis)
CHECK(TMatrix4x4::setRotation(const Angle& phi, const TVector4<T>& axis))
  //BAUSTELLE
RESULT


//line 492: method TMatrix4x4::getRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z, TMatrix4x4& m)
CHECK(TMatrix4x4::getRotation(const Angle& phi, const T& axis_x, const T& axis_y, const T& axis_z, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 496: method TMatrix4x4::getRotation(const Angle& phi, const TVector3<T>& axis, TMatrix4x4& m)
CHECK(TMatrix4x4::getRotation(const Angle& phi, const TVector3<T>& axis, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 500: method TMatrix4x4::getRotation(const Angle& phi, const TVector4<T>& axis, TMatrix4x4& m)
CHECK(TMatrix4x4::getRotation(const Angle& phi, const TVector4<T>& axis, TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 510: method TMatrix4x4::bool operator == (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::bool operator == (const TMatrix4x4& m) const )
  //BAUSTELLE
RESULT


//line 515: method TMatrix4x4::bool operator != (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::bool operator != (const TMatrix4x4& m) const )
  //BAUSTELLE
RESULT


//line 522: method TMatrix4x4::isIdentity() const 
CHECK(TMatrix4x4::isIdentity() const )
  //BAUSTELLE
RESULT


//line 527: method TMatrix4x4::isRegular() const 
CHECK(TMatrix4x4::isRegular() const )
  //BAUSTELLE
RESULT


//line 532: method TMatrix4x4::isSingular() const 
CHECK(TMatrix4x4::isSingular() const )
  //BAUSTELLE
RESULT


//line 538: method TMatrix4x4::isSymmetric() const 
CHECK(TMatrix4x4::isSymmetric() const )
  //BAUSTELLE
RESULT


//line 544: method TMatrix4x4::isLowerTriangular() const 
CHECK(TMatrix4x4::isLowerTriangular() const )
  //BAUSTELLE
RESULT


//line 550: method TMatrix4x4::isUpperTriangular() const 
CHECK(TMatrix4x4::isUpperTriangular() const )
  //BAUSTELLE
RESULT


//line 555: method TMatrix4x4::isDiagonal() const 
CHECK(TMatrix4x4::isDiagonal() const )
  //BAUSTELLE
RESULT


//line 566: method TMatrix4x4::isValid() const 
CHECK(TMatrix4x4::isValid() const )
  //BAUSTELLE
RESULT


//line 569: method TMatrix4x4::dump(std::ostream& s = std::cout, Size depth = 0) const 
CHECK(TMatrix4x4::dump(std::ostream& s = std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT


//line 39: method std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m)
CHECK(std::istream& operator >> (std::istream& s, TMatrix4x4<T>& m))
  //BAUSTELLE
RESULT


//line 45: method std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m)
CHECK(std::ostream& operator << (std::ostream& s, const TMatrix4x4<T>& m))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
