// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MATHS/matrix44.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/quaternion.h>
///////////////////////////

START_TEST(Quaternion)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector3 const v = Vector3(1.0, 2.0, 3.0);
float const a = 4.0;
String filename;
using std::ofstream;
using std::ios;
PRECISION(1E-4)

float i = 0.24302, j = 2 * i, k = 3 * i, angle = -0.416147; 

CHECK(BALL_CREATE(TQuaternion<T>))
	Quaternion q(v, 4.0);
	Quaternion* q_ptr = (Quaternion*)q.create(false, true);
	TEST_REAL_EQUAL(q_ptr->i(), 0)
	TEST_REAL_EQUAL(q_ptr->j(), 0)
	TEST_REAL_EQUAL(q_ptr->k(), 0)
	TEST_REAL_EQUAL(q_ptr->w(), 1.0)
	delete q_ptr;
	q_ptr = (Quaternion*)q.create();
	TEST_REAL_EQUAL(q_ptr->i(), 0.24302)
	TEST_REAL_EQUAL(q_ptr->j(), 0.48604)
	TEST_REAL_EQUAL(q_ptr->k(), 0.72906)
	TEST_REAL_EQUAL(q_ptr->w(), -0.416147)
	delete q_ptr;
RESULT

CHECK(BALL_CREATE(TQuaternion<T>))
	Quaternion q;
	TEST_REAL_EQUAL(q.w(), 1)
	TEST_REAL_EQUAL(q.i(), 0)
	TEST_REAL_EQUAL(q.j(), 0)
	TEST_REAL_EQUAL(q.k(), 0)
RESULT

Quaternion* q_ptr = 0;
CHECK(TQuaternion())
	q_ptr = new Quaternion();
	TEST_NOT_EQUAL(q_ptr, 0)
RESULT		

CHECK(~TQuaternion())
	delete q_ptr;
RESULT	

CHECK(TQuaternion(const TQuaternion& q))
	Quaternion q;
	q.w() = a; 
	q.i() = v.x;
	q.j() = v.y;
	q.k() = v.z;
	Quaternion q1(q);
	TEST_REAL_EQUAL(q1.w(), q.w())
	TEST_REAL_EQUAL(q1.i(), q.i())
	TEST_REAL_EQUAL(q1.j(), q.j())
	TEST_REAL_EQUAL(q1.k(), q.k())
RESULT	

CHECK(TQuaternion(const boost::math::quaternion<T>& q))
	boost::math::quaternion<float> bq(4.0, 1.0, 2.0, 3.0);
	Quaternion q1(bq);
	TEST_REAL_EQUAL(q1.w(), bq.R_component_1())
	TEST_REAL_EQUAL(q1.i(), bq.R_component_2())
	TEST_REAL_EQUAL(q1.j(), bq.R_component_3())
	TEST_REAL_EQUAL(q1.k(), bq.R_component_4())
RESULT	

CHECK(TQuaternion(const T& w, const T& i, const T& j, const T& k))
	Quaternion q1(4.0, 1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(q1.w(), 4.0)
	TEST_REAL_EQUAL(q1.i(), 1.0)
	TEST_REAL_EQUAL(q1.j(), 2.0)
	TEST_REAL_EQUAL(q1.k(), 3.0)
RESULT	

CHECK(TQuaternion(const TVector3<T>& axis, const T& new_angle) )
	Quaternion q(v, 4.0);
	TEST_REAL_EQUAL(q.w(), angle)
	TEST_REAL_EQUAL(q.i(), i)
	TEST_REAL_EQUAL(q.j(), j)
	TEST_REAL_EQUAL(q.k(), k)
RESULT	

CHECK(void clear())
	Quaternion q;
	Quaternion q1(4.0, 1.0, 2.0, 3.0);
	q1.clear();
	TEST_EQUAL(q1 == q, true)
RESULT

CHECK(void set(const TQuaternion& q) )
	Quaternion q(4.0, 1.0, 2.0, 3.0);
	Quaternion q1;
	q1.set(q);
	TEST_EQUAL(q1, q)
RESULT	

CHECK(void set(const boost::math::quaternion<T>& q))
	boost::math::quaternion<float> bq(4.0, 1.0, 2.0, 3.0);
	Quaternion q1;
	q1.set(bq);
	TEST_REAL_EQUAL(q1.w(), bq.R_component_1())
	TEST_REAL_EQUAL(q1.i(), bq.R_component_2())
	TEST_REAL_EQUAL(q1.j(), bq.R_component_3())
	TEST_REAL_EQUAL(q1.k(), bq.R_component_4())
RESULT	

CHECK(void set(const TVector3<T>& axis, const T& angle))
	Quaternion q(v,angle);
	Quaternion q1;
	q1.fromAxisAngle(v,angle);
	TEST_EQUAL(q1, q)
RESULT	

CHECK(void set(const T& w, const T& i, const T& j, const T& k))
	Quaternion q(4.0, 1.0, 2.0, 3.0);
	Quaternion q1;
	q1.set(4.0, 1.0, 2.0, 3.0);
 TEST_EQUAL(q1 == q, true)
RESULT

CHECK(TQuaternion& operator = (const TQuaternion& q) )
	Quaternion q(4.0, 1.0, 2.0, 3.0);
	Quaternion q1;
	q1 = q;
	TEST_EQUAL(q1, q)
RESULT	

CHECK(void set(const boost::math::quaternion<T>& q))
	boost::math::quaternion<float> bq(4.0, 1.0, 2.0, 3.0);
	Quaternion q1;
	q1 = bq;
	TEST_REAL_EQUAL(q1.w(), bq.R_component_1())
	TEST_REAL_EQUAL(q1.i(), bq.R_component_2())
	TEST_REAL_EQUAL(q1.j(), bq.R_component_3())
	TEST_REAL_EQUAL(q1.k(), bq.R_component_4())
RESULT	

CHECK(void setIdentity())
	Quaternion q(1.0, 1.0, 1.0, 1.0);
  q.setIdentity();
  TEST_EQUAL(q.w(), 1)
  TEST_EQUAL(q.i(), 0)
  TEST_EQUAL(q.j(), 0)
  TEST_EQUAL(q.k(), 0)
RESULT

CHECK(TQuaternion<T>& normalize())
	Quaternion q(4.0, 1.0, 2.0, 3.0);
	Quaternion q1 = q.normalize();
  TEST_REAL_EQUAL(q1.w(), 0.133333)
  TEST_REAL_EQUAL(q1.i(), 0.0333333)
  TEST_REAL_EQUAL(q1.j(), 0.0666667)
  TEST_REAL_EQUAL(q1.k(), 0.1)
RESULT

CHECK(void swap(TQuaternion& q))
	Quaternion q(10, 20, 30, 40);
	Quaternion q1(1.0, 2.0, 3.0, 4.0);
	Quaternion q12(q1);
	Quaternion q2(q);
	q.swap(q1);
	TEST_EQUAL(q, q12)
	TEST_EQUAL(q1, q2)
RESULT

CHECK(void fromAxisAngle(const TVector3<T>& axis, const T& angle))
	Quaternion q1;
	q1.fromAxisAngle(v, a);
	TEST_REAL_EQUAL(q1.w(), angle)
	TEST_REAL_EQUAL(q1.i(), i)
	TEST_REAL_EQUAL(q1.j(), j)
	TEST_REAL_EQUAL(q1.k(), k)
RESULT

CHECK(void fromEulerAngles(const T& yaw, const T& pitch, const T& roll));
	Quaternion q;
	q.fromEulerAngles(0.4, 0.2, 0.1);
	TEST_REAL_EQUAL(q.w(), 0.974943)
	TEST_REAL_EQUAL(q.i(), 0.0289292)
	TEST_REAL_EQUAL(q.j(), 0.107601)
	TEST_REAL_EQUAL(q.k(), 0.19254)
RESULT


CHECK(void toAxisAngle(TVector3<T>& axis, T& angle))
	Vector3 v1(v);
	Vector3 v2;
	float ang;
	Quaternion q(v1, a);	
	q.toAxisAngle(v2, ang);
	v1.normalize();
	TEST_REAL_EQUAL(ang, a)
	TEST_REAL_EQUAL(v2.x, v1.x)
	TEST_REAL_EQUAL(v2.y, v1.y)
	TEST_REAL_EQUAL(v2.z, v1.z)
RESULT

CHECK(void toEulerAngles(T& yaw, T& pitch, T& roll))
	Quaternion q(0.974943, 0.0289292, 0.107601, 0.19254);
	float yaw, pitch, roll;
	q.toEulerAngles(yaw, pitch, roll);
	TEST_REAL_EQUAL(yaw, 0.4)
	TEST_REAL_EQUAL(pitch, 0.2)
	TEST_REAL_EQUAL(roll, 0.1)
RESULT

CHECK(void get(TQuaternion& q) const)
	Quaternion q(4.0, 1.0, 2.0, 3.0);
	Quaternion q1;
	q.get(q1);
	TEST_EQUAL(q1, q)
RESULT	

CHECK(T getAngle() const)
	Quaternion q(v,a);
	TEST_REAL_EQUAL(q.getAngle(),a) 
RESULT

CHECK(TVector3<T> getAxis())
	Quaternion q(v,a);
	Vector3 v1(v);
	v1.normalize();
	Vector3 v2 = q.getAxis();
	TEST_REAL_EQUAL(v2.x, v1.x)
	TEST_REAL_EQUAL(v2.y, v1.y)
	TEST_REAL_EQUAL(v2.z, v1.z)
RESULT

CHECK(TMatrix4x4<T>& getRotationMatrix(TMatrix4x4<T>& m) const throw())
	Quaternion q(angle, i,j,k);
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

CHECK(TQuaternion getInverse() const throw())
	Quaternion q(0.5, 0.34, 0.69, 0.17);
	Quaternion q1, q2;
	q1 = q.getInverse();
	q2 = q * q1;
	TEST_REAL_EQUAL(q2.w(), 1)
	TEST_REAL_EQUAL(q2.i(), 0)
	TEST_REAL_EQUAL(q2.j(), 0)
	TEST_REAL_EQUAL(q2.k(), 0)
RESULT

CHECK(TQuaternion getConjugate() const throw())
	Quaternion q(0.92, 0.31, 0.27, 0.74);
	Quaternion q1;
	q1 = q.getConjugate();
	TEST_REAL_EQUAL(q1.w(), q.w())
	TEST_REAL_EQUAL(q1.i(), -q.i())
	TEST_REAL_EQUAL(q1.j(), -q.j())
	TEST_REAL_EQUAL(q1.k(), -q.k())
RESULT

Quaternion q1;
CHECK(std::istream& operator >>(std::istream& s, TQuaternion<T>& q))
	std::ifstream instr(BALL_TEST_DATA_PATH(Quaternion_test2.txt));
	instr >> q1;
	instr.close();
	TEST_REAL_EQUAL(q1.w(), -0.416147)
	TEST_REAL_EQUAL(q1.i(), 0.24302)
	TEST_REAL_EQUAL(q1.j(), 0.48604)
	TEST_REAL_EQUAL(q1.k(), 0.72906)
RESULT

CHECK(std::ostream& operator << (std::ostream& s, const TQuaternion<T>& q))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << q1;
	outstr.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Quaternion_test2.txt))
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	q1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Quaternion_test.txt))
RESULT		

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
