// $Id: Quaternion_test.C,v 1.7 2000/06/27 23:29:00 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/quaternion.h>
///////////////////////////

START_TEST(class_name, "$Id: Quaternion_test.C,v 1.7 2000/06/27 23:29:00 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Vector3 const v = Vector3(1.0, 2.0, 3.0);
String filename;
using std::ofstream;
using std::ios;
PRECISION(1E-4)

CHECK(TQuaternion::BALL_CREATE(TQuaternion<T>))
	Quaternion q(v, 4.0);
	Quaternion* q_ptr = (Quaternion*)q.create(false, true);
	TEST_REAL_EQUAL(q_ptr->i, 0)
	TEST_REAL_EQUAL(q_ptr->j, 0)
	TEST_REAL_EQUAL(q_ptr->k, 0)
	TEST_REAL_EQUAL(q_ptr->angle, 1.0)
	delete q_ptr;
	q_ptr = (Quaternion*)q.create();
	TEST_REAL_EQUAL(q_ptr->i, 0.24302)
	TEST_REAL_EQUAL(q_ptr->j, 0.48604)
	TEST_REAL_EQUAL(q_ptr->k, 0.72906)
	TEST_REAL_EQUAL(q_ptr->angle, -0.416147)
	delete q_ptr;
RESULT

CHECK(TQuaternion();)
  Quaternion* p;
	p = new Quaternion();
	TEST_NOT_EQUAL(p, 0)
RESULT		

CHECK(~TQuaternion();)
  Quaternion* p;
	p = new Quaternion();
	delete p;
RESULT	

Quaternion q, q1, q2;
float i = 0.24302, j = 2 * i, k = 3 * i, angle = -0.416147; 

CHECK(TQuaternion(const TVector3<T>& axis, const T &angle))
	q = Quaternion(v, 4.0);
	TEST_REAL_EQUAL(q.i, i)
	TEST_REAL_EQUAL(q.j, j)
	TEST_REAL_EQUAL(q.k, k)
	TEST_REAL_EQUAL(q.angle, angle)
RESULT	

CHECK(TQuaternion(const TQuaternion& q, bool deep = true))
	q = Quaternion(v, 4.0);
	q1 = Quaternion(q);
	TEST_REAL_EQUAL(q1.i, i)
	TEST_REAL_EQUAL(q1.j, j)
	TEST_REAL_EQUAL(q1.k, k)
	TEST_REAL_EQUAL(q1.angle, angle)
RESULT	

CHECK(TQuaternion(const T& x, const T& y, const T& z, const T& angle))
	q1 = Quaternion(1.0, 2.0, 3.0, 4.0);
	TEST_REAL_EQUAL(q1.i, i)
	TEST_REAL_EQUAL(q1.j, j)
	TEST_REAL_EQUAL(q1.k, k)
	TEST_REAL_EQUAL(q1.angle, angle)
RESULT	

CHECK(TQuaternion::getAngle() const )
	TEST_REAL_EQUAL(q.getAngle(), (2.0 * atan2(sqrt(i * i + j * j + k * k), angle)) )
RESULT

CHECK(TQuaternion::getAxis())
	Vector3 v2 = Vector3(i, j, k), v3;
	v2.normalize();
	v3 = q.getAxis();
	TEST_EQUAL(v3, v2)
	v3 = Vector3();
	q1 = Quaternion(v3, 4);
	TEST_EXCEPTION(Exception::DivisionByZero, q1.getAxis())
RESULT

CHECK(TQuaternion::getRotationMatrix(TMatrix4x4<T>& m) const )
	Matrix4x4	m, m2;
	m.set((1.0 - 2.0 * (j * j + k * k)), 
			 (2.0 * (i * j - k * angle)), 
			 (2.0 * (k * i + j * angle)), 
			 0, 
			 (2.0 * (i * j + k * angle)), 
			 (1.0 - 2.0 * (k * k + i * i)), 
			 (2.0 * (j * k - i * angle)), 
			 0, 			 
			 (2.0 * (k * i - j * angle)),
			 (2.0 * (j * k + i * angle)),
			 (1.0 - 2.0 * (j * j + i * i)), 
			 0,
			 0, 0, 0, 1);
	q.getRotationMatrix(m2);
	TEST_REAL_EQUAL(m2.m11, m.m11);
	TEST_REAL_EQUAL(m2.m12, m.m12);
	TEST_REAL_EQUAL(m2.m13, m.m13);
	TEST_REAL_EQUAL(m2.m14, m.m14);
	TEST_REAL_EQUAL(m2.m21, m.m21);
	TEST_REAL_EQUAL(m2.m22, m.m22);
	TEST_REAL_EQUAL(m2.m23, m.m23);
	TEST_REAL_EQUAL(m2.m24, m.m24);
	TEST_REAL_EQUAL(m2.m31, m.m31);
	TEST_REAL_EQUAL(m2.m32, m.m32);
	TEST_REAL_EQUAL(m2.m33, m.m33);
	TEST_REAL_EQUAL(m2.m34, m.m34);
	TEST_REAL_EQUAL(m2.m41, m.m41);
	TEST_REAL_EQUAL(m2.m42, m.m42);
	TEST_REAL_EQUAL(m2.m43, m.m43);
	TEST_REAL_EQUAL(m2.m44, m.m44);
RESULT

CHECK(TQuaternion::TQuaternion operator - () const )
	float tmp = 1 / ::sqrt(angle * angle + i * i + j * j + k * k);
	q2 = -q;
	q1 = Quaternion(-i * tmp, -j * tmp, -k * tmp,	angle * tmp);
	TEST_REAL_EQUAL(q2.i, q1.i)
	TEST_REAL_EQUAL(q2.j, q1.j)
	TEST_REAL_EQUAL(q2.k, q1.k)
	TEST_REAL_EQUAL(q2.angle, q1.angle)
	q1 = Quaternion();
	q1.angle = 0;
	q2 = Quaternion();
	TEST_EQUAL(-q1, q2);
RESULT	

CHECK(TQuaternion::getInverse() const )
	q1 = -Quaternion(q);
	q2 = q.getInverse();
	TEST_EQUAL(q2, q1)
RESULT

CHECK(TQuaternion::getConjugate() const )
	q1 = q.getConjugate();
	TEST_REAL_EQUAL(q1.i, -i)
	TEST_REAL_EQUAL(q1.j, -j)
	TEST_REAL_EQUAL(q1.k, -k)
	TEST_REAL_EQUAL(q1.angle, angle)
RESULT

CHECK(TQuaternion::bool operator == (const TQuaternion& q) const )
	q1 = Quaternion(q);
	TEST_EQUAL(q1 == q, true)
	q1 = Quaternion(v, 4.1);
	TEST_EQUAL(q1 == q, false)
RESULT	

CHECK(TQuaternion::TQuaternion& operator += (const TQuaternion& q))
  q1 = Quaternion();
	q1 += q;
	TEST_REAL_EQUAL(q1.i, i)
	TEST_REAL_EQUAL(q1.j, j)
	TEST_REAL_EQUAL(q1.k, k)
	TEST_REAL_EQUAL(q1.angle, angle)
RESULT	

CHECK(TQuaternion::TQuaternion& operator -= (const TQuaternion& q))
	q1 = q2 = Quaternion();
  q1 += - q;
	q2 -= q;
	TEST_REAL_EQUAL(q1.i, q2.i)
	TEST_REAL_EQUAL(q1.j, q2.j)
	TEST_REAL_EQUAL(q1.k, q2.k)
	TEST_REAL_EQUAL(q1.angle, q2.angle)
RESULT	

CHECK(TQuaternion::bool operator != (const TQuaternion& q) const )
	q1 = Quaternion(q);
	TEST_EQUAL(q1 != q, false)
	q1 = Quaternion(v, 4.1);
	TEST_EQUAL(q1 != q, true)
RESULT	

CHECK(TQuaternion::set(const TVector3<T>& axis, const T& angle))
	q1 = Quaternion();
	q1.set(v, 4.0);
	TEST_EQUAL(q1, q)
RESULT	

CHECK(TQuaternion::set(const TQuaternion<T>& q, bool /* deep */))
	q1 = Quaternion();
	q1.set(q);
	TEST_EQUAL(q1, q)
RESULT	

CHECK(TQuaternion::set(const T& x, const T& y, const T& z, const T& phi))
	q1.set(1.0, 2.0, 3.0, 4.0);
 TEST_EQUAL(q1 == q, true)
RESULT

CHECK(TQuaternion::get(TQuaternion<T>& q, bool deep))
	q.get(q1);
	TEST_EQUAL(q1, q)
RESULT	

CHECK(TQuaternion::operator = (const TQuaternion<T>& q))
	q1 = Quaternion();
	q1 = q;
	TEST_EQUAL(q1 == q, true)
RESULT	

CHECK(TQuaternion::setIdentity())
  q1.setIdentity();
  TEST_EQUAL(q1.i, 0)
  TEST_EQUAL(q1.j, 0)
  TEST_EQUAL(q1.k, 0)
  TEST_EQUAL(q1.angle, 1)
RESULT

CHECK(TQuaternion operator + (const TQuaternion<T>& a, const TQuaternion<T>& b))
	q1 = Quaternion(q);
	q2 = q + q1;
	q1 += q;
  TEST_EQUAL(q2, q1)
RESULT

CHECK(TQuaternion operator - (const TQuaternion<T>& a, const TQuaternion<T>& b))
	q1 = Quaternion(q);
	q2 = q - q1;
	q1 -= q;
  TEST_EQUAL(q2, q1)
RESULT

NEW_TMP_FILE(filename)

CHECK(std::istream& operator >>(std::istream& s, TQuaternion<T>& q))
	std::ifstream instr("data/Quaternion_test2.txt");
	q1 = Quaternion();
	instr >> q1;
	instr.close();
	TEST_REAL_EQUAL(q1.i, i)
	TEST_REAL_EQUAL(q1.j, j)
	TEST_REAL_EQUAL(q1.k, k)
	TEST_REAL_EQUAL(q1.angle, angle)
RESULT

CHECK(std::ostream& operator << (std::ostream& s, const TQuaternion<T>& q))
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << q;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Quaternion_test3.txt", true)
RESULT

CHECK(TQuaternion::dump(std::ostream& s, Size depth) const )
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	q.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Quaternion_test.txt", true)
RESULT		

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
