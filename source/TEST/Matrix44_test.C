// $Id: Matrix44_test.C,v 1.1 2000/03/08 16:17:08 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

///////////////////////////

START_TEST(class_name, "$Id: Matrix44_test.C,v 1.1 2000/03/08 16:17:08 amoll Exp $")

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

Matrix4x4 m(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
Matrix4x4 m1, m2, m3;
const Vector4 v(1.0, 2.0, 3.0, 4.0), v1(5.0, 6.0, 7.0, 8.0), v2(9.0, 10.0, 11.0, 12.0), v3(13.0, 14.0, 15.0, 16.0);
Vector4 vx, vy;

CHECK(TMatrix4x4::bool operator == (const TMatrix4x4& matrix) const )
  Matrix4x4 m1(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0);
	TEST_EQUAL(m == m1 , true)
  m1 = Matrix4x4(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.1);
	TEST_EQUAL(m == m1 , false)
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
	vx = m.getRow(0);
	TEST_EQUAL(vx , v)
RESULT


//line 278: method TMatrix4x4::getColumn(Index col)
CHECK(TMatrix4x4::getColumn(Index col))
	vx = m.getColumn(0);
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
  //BAUSTELLE
RESULT


//line 309: method TMatrix4x4::TMatrix4x4 operator - () const 
CHECK(TMatrix4x4::TMatrix4x4 operator - () const )
  //BAUSTELLE
RESULT


//line 316: method TMatrix4x4::TMatrix4x4 operator + (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::TMatrix4x4 operator + (const TMatrix4x4& m) const )
  //BAUSTELLE
RESULT


//line 323: method TMatrix4x4::TMatrix4x4& operator += (const TMatrix4x4& m)
CHECK(TMatrix4x4::TMatrix4x4& operator += (const TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 330: method TMatrix4x4::TMatrix4x4 operator - (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::TMatrix4x4 operator - (const TMatrix4x4& m) const )
  //BAUSTELLE
RESULT


//line 337: method TMatrix4x4::TMatrix4x4& operator -= (const TMatrix4x4& m)
CHECK(TMatrix4x4::TMatrix4x4& operator -= (const TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 342: method TMatrix4x4::TMatrix4x4 operator * (const T& scalar) const 
CHECK(TMatrix4x4::TMatrix4x4 operator * (const T& scalar) const )
  //BAUSTELLE
RESULT


//line 347: method TMatrix4x4::TMatrix4x4& operator *= (const T& scalar)
CHECK(TMatrix4x4::TMatrix4x4& operator *= (const T& scalar))
  //BAUSTELLE
RESULT


//line 353: method TMatrix4x4::TMatrix4x4 operator / (const T& scalar) const 
CHECK(TMatrix4x4::TMatrix4x4 operator / (const T& scalar) const )
  //BAUSTELLE
RESULT


//line 359: method TMatrix4x4::TMatrix4x4& operator /= (const T& scalar)
CHECK(TMatrix4x4::TMatrix4x4& operator /= (const T& scalar))
  //BAUSTELLE
RESULT


//line 364: method TMatrix4x4::TMatrix4x4 operator * (const TMatrix4x4& m) const 
CHECK(TMatrix4x4::TMatrix4x4 operator * (const TMatrix4x4& m) const )
  //BAUSTELLE
RESULT


//line 369: method TMatrix4x4::TMatrix4x4& operator *= (const TMatrix4x4& m)
CHECK(TMatrix4x4::TMatrix4x4& operator *= (const TMatrix4x4& m))
  //BAUSTELLE
RESULT


//line 372: method TMatrix4x4::TVector4 operator * (const TVector4<T> &) const 
CHECK(TMatrix4x4::TVector4 operator * (const TVector4<T> &) const )
  //BAUSTELLE
RESULT


//line 381: method TMatrix4x4::invert(TMatrix4x4& inverse) const 
CHECK(TMatrix4x4::invert(TMatrix4x4& inverse) const )
  //BAUSTELLE
RESULT


//line 388: method TMatrix4x4::invert()
CHECK(TMatrix4x4::invert())
  //BAUSTELLE
RESULT


//line 393: method TMatrix4x4::getDeterminant() const 
CHECK(TMatrix4x4::getDeterminant() const )
  //BAUSTELLE
RESULT


//line 396: method TMatrix4x4::translate(const T &x, const T &y, const T &z)
CHECK(TMatrix4x4::translate(const T &x, const T &y, const T &z))
  //BAUSTELLE
RESULT


//line 399: method TMatrix4x4::translate(const TVector3<T>& v)
CHECK(TMatrix4x4::translate(const TVector3<T>& v))
  //BAUSTELLE
RESULT


//line 402: method TMatrix4x4::setTranslation(const T& x, const T& y, const T& z)
CHECK(TMatrix4x4::setTranslation(const T& x, const T& y, const T& z))
  //BAUSTELLE
RESULT


//line 405: method TMatrix4x4::setTranslation(const TVector3<T>& v)
CHECK(TMatrix4x4::setTranslation(const TVector3<T>& v))
  //BAUSTELLE
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
  //BAUSTELLE
RESULT


//line 417: method TMatrix4x4::scale(const T& scale)
CHECK(TMatrix4x4::scale(const T& scale))
  //BAUSTELLE
RESULT


//line 420: method TMatrix4x4::scale(const TVector3<T>& v)
CHECK(TMatrix4x4::scale(const TVector3<T>& v))
  //BAUSTELLE
RESULT


//line 423: method TMatrix4x4::setScale(const T& x_scale, const T& y_scale, const T& z_scale)
CHECK(TMatrix4x4::setScale(const T& x_scale, const T& y_scale, const T& z_scale))
  //BAUSTELLE
RESULT


//line 426: method TMatrix4x4::setScale(const T& scale)
CHECK(TMatrix4x4::setScale(const T& scale))
  //BAUSTELLE
RESULT


//line 429: method TMatrix4x4::setScale(const TVector3<T> &)
CHECK(TMatrix4x4::setScale(const TVector3<T> &))
  //BAUSTELLE
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
