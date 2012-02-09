// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <cmath>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/MATHS/angle.h>
///////////////////////////

START_TEST(TVector3)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector3* vector3_ptr = 0;
CHECK(TVector3() throw())
	vector3_ptr = new Vector3;
	TEST_NOT_EQUAL(vector3_ptr, 0)
RESULT								

CHECK(~TVector3() throw())
	delete vector3_ptr;
RESULT		

Vector3 v;
Vector3 v1;
Vector3 v2;

CHECK(void clear() throw())
	Vector3 v1(1, 2, 3);
	Vector3 v2;
	v2.set(0, 0, 0);
	v1.clear();
	TEST_EQUAL(v1, v2)
RESULT

CHECK(T& operator [] (Position position) throw(Exception::IndexOverflow))
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[12354])
	TEST_EXCEPTION(Exception::IndexOverflow,  v[3])
RESULT

CHECK(TVector3(const T* ptr) throw(Exception::NullPointer))
	float arr[3];
	arr[0] = 1.0;
	arr[1] = 2.0;
	arr[2] = 3.0;
	v = Vector3(arr);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
	TEST_EXCEPTION(Exception::NullPointer, v = Vector3((float*)0))
RESULT

CHECK(TVector3(const T& value) throw())
	v = Vector3(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], 1.0)
RESULT

CHECK(TVector3(const T& vx, const T& vy, const T& vz) throw())
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

CHECK(TVector3(const TVector3& vector) throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(v2);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

CHECK(TVector3(const T& r, const TAngle<T>& phi, const TAngle<T>& theta) throw())
	float r = 0.5;
	Angle phi, theta;
	phi   = Angle(1.5, true);
	theta = Angle(0.9, true);
	v  = Vector3(r, phi, theta);
	TEST_REAL_EQUAL(v[0], r * cos(phi) * sin(theta))
	TEST_REAL_EQUAL(v[1], r * sin(phi) * sin(theta))
	TEST_REAL_EQUAL(v[2], r * cos(theta))
RESULT

String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
TextPersistenceManager pm;
CHECK(void write(PersistenceManager& pm) const throw())
	Vector3 v(1.0, 2.0, 3.0);
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	pm.writeStorableObject(v, "testname");
	ofile.close();	
RESULT

using std::ifstream;
using std::cout;
CHECK(bool read(PersistenceManager& pm) throw())
	ifstream ifile(filename.c_str());
	pm.setIstream(ifile);
	Vector3 v;
	bool result = pm.readStorableObject(v, "testname");
	ifile.close();
	TEST_EQUAL(result, true)
	TEST_REAL_EQUAL(v.x, 1.0)
	TEST_REAL_EQUAL(v.y, 2.0)
	TEST_REAL_EQUAL(v.z, 3.0)
RESULT

CHECK(void set(const T* ptr) throw(Exception::NullPointer))
	float arr[3];
	arr[0] = 1.0;
	arr[1] = 2.0;
	arr[2] = 3.0;
	v = Vector3();
	v.set(arr);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
	TEST_EXCEPTION(Exception::NullPointer, v.set((float*)0))
RESULT

CHECK(void set(const T& value) throw())
	v.set(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], 1.0)
RESULT

CHECK(void set(const T& vx, const T& vy, const T& vz) throw())
	v.set(1.0, 2.0, 3.0);
	v2 = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v2, v)
RESULT

CHECK(void set(const TVector3& vector) throw())
	v = Vector3(1.0, 2.0, 3.0);
	v2.set(v);
	TEST_EQUAL(v2, v)
RESULT

CHECK(void set(const T& r, const TAngle<T>& phi, const TAngle<T>& theta) throw())
	float r = 0.5;
	Angle phi, theta;
	phi   = Angle(1.5, true);
	theta = Angle(0.9, true);
	v  = Vector3(r, phi, theta);
	v2.set(r, phi, theta);
	TEST_REAL_EQUAL(v[0], v2[0])
	TEST_REAL_EQUAL(v[1], v2[1])
	TEST_REAL_EQUAL(v[2], v2[2])
RESULT

CHECK(TVector3& operator = (const T* ptr) throw(Exception::NullPointer))
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3();
	v = arr;
	v2 = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v2, v)
	TEST_EXCEPTION(Exception::NullPointer, v = ((float*)0))
RESULT

CHECK(TVector3& operator = (const TVector3& v) throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector3& operator = (T value) throw())
	v2 = 1.1;
	TEST_REAL_EQUAL(v2.x, (float) 1.1)
	TEST_REAL_EQUAL(v2.y, (float) 1.1)
	TEST_REAL_EQUAL(v2.z, (float) 1.1)
RESULT

CHECK(void get(T* ptr) const throw(Exception::NullPointer))
	float arr[3];
	v = Vector3(1.0, 2.0, 3.0);
	v.get(arr);
	TEST_REAL_EQUAL(arr[0], 1.0)
	TEST_REAL_EQUAL(arr[1], 2.0)
	TEST_REAL_EQUAL(arr[2], 3.0)
	TEST_EXCEPTION(Exception::NullPointer, v.get((float*)0))
RESULT

CHECK(void get(T& x, T& y, T& z) const throw())
	float a, b, c;
	v = Vector3(1.0, 2.0, 3.0);
	v.get(a,b,c);
	TEST_REAL_EQUAL(a, 1.0)
	TEST_REAL_EQUAL(b, 2.0)
	TEST_REAL_EQUAL(c, 3.0)
RESULT

CHECK(void get(TVector3& vector) const throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3();
	v.get(v2);
	TEST_EQUAL(v2, v)
RESULT

CHECK(void get(T& r, TAngle<T>& phi, TAngle<T>& theta) const throw())
	float r = 0.5, r2;
	Angle phi, theta, phi2, theta2;
	phi   = Angle(1.5, true);
	theta = Angle(0.9, true);
	v  = Vector3(r, phi, theta);
	v.get(r2, phi2, theta2);
	TEST_REAL_EQUAL(r, r2)
	TEST_REAL_EQUAL(phi, phi2)
	TEST_REAL_EQUAL(theta, theta2)
RESULT

CHECK(void swap(TVector3& vector) throw())
	v  = Vector3(1.0, 2.0, 3.0);
	Vector3 u2 = v;

	Vector3 u(4.0, 3.0, 2.0);
	v2 = u;

	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

CHECK(T getLength() const throw())
	v = Vector3(4.0, 9.0, 16.0);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0))
	v = Vector3(0.0, 0.0, 0.0);
	TEST_REAL_EQUAL(v.getLength(), 0.0)
RESULT

CHECK(T getSquareLength() const throw())
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 14.0)
	v = Vector3(0.0, 0.0, 0.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 0.0)
RESULT

CHECK(TVector3& normalize() throw(Exception::DivisionByZero))
	v = Vector3(4.0, 9.0, 16.0);
	v.normalize();
	float erg = ::sqrt (4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0);
	TEST_REAL_EQUAL(v[0], 4.0 / erg)
	TEST_REAL_EQUAL(v[1], 9.0 / erg)
	TEST_REAL_EQUAL(v[2], 16.0 / erg)
	v = Vector3(0.0, 0.0, 0.0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT

CHECK(static const TVector3& getZero() throw())
	TEST_REAL_EQUAL(Vector3::getZero().x, 0.0)
	TEST_REAL_EQUAL(Vector3::getZero().y, 0.0)
	TEST_REAL_EQUAL(Vector3::getZero().z, 0.0)
RESULT

CHECK(static const TVector3& getUnit() throw())
	TEST_REAL_EQUAL(Vector3::getUnit().x, 1.0)
	TEST_REAL_EQUAL(Vector3::getUnit().y, 1.0)
	TEST_REAL_EQUAL(Vector3::getUnit().z, 1.0)
RESULT

CHECK(const T& operator [] (Position position) const throw(Exception::IndexOverflow))
	v = Vector3(1.0, 2.0, 3.0);
	v[0]=5.0;	v[1]=6.0;	v[2]=7.0;
	const Vector3& c_v(v);
	TEST_REAL_EQUAL(c_v[0], 5.0)
	TEST_REAL_EQUAL(c_v[1], 6.0)
	TEST_REAL_EQUAL(c_v[2], 7.0)
	float x = 5;
	TEST_EXCEPTION(Exception::IndexOverflow, x = c_v[32472])
	TEST_EXCEPTION(Exception::IndexOverflow, x = c_v[3])
RESULT

CHECK(const TVector3& operator + () const throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v = + v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

CHECK(TVector3 operator - () const throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v = - v2;
	TEST_REAL_EQUAL(v[0], -1.0)
	TEST_REAL_EQUAL(v[1], -2.0)
	TEST_REAL_EQUAL(v[2], -3.0)
RESULT

CHECK(TVector3& operator += (const TVector3& vector) throw())
	v2 = Vector3(1.0, 2.0 ,3.0);
	v  = Vector3(1.0, 2.0, 3.0);
	v += v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3& operator -= (const TVector3& vector) throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(4.0, 3.0, 2.0);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], -1.0)
RESULT

CHECK(TVector3 operator * (const T& scalar) const throw())
	v  = Vector3(1.0, 2.0, 3.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3& operator *= (const T& scalar) throw())
	v  = Vector3(1.0, 2.0, 3.0);
	v *= 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3 operator / (const T& lambda) const throw(Exception::DivisionByZero))
	v  = Vector3(1.0, 2.0, 3.0);
	v = v / 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

CHECK(TVector3& operator /= (const T& lambda) throw(Exception::DivisionByZero))
	v  = Vector3(1.0, 2.0, 3.0);
	v /= 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

CHECK(T operator * (const TVector3& vector) const throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v * v2 , 14.0)
RESULT

CHECK(TVector3 operator % (const TVector3& vector) const throw())
 	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v  = v1 % v2;
	TEST_REAL_EQUAL(v[0], -3.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_REAL_EQUAL(v[2], -3.0)
RESULT

CHECK(TVector3& operator %= (const TVector3& vector) throw())
 	v  = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v  %= v2;
	TEST_REAL_EQUAL(v[0], -3.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_REAL_EQUAL(v[2], -3.0)
RESULT

CHECK(T getDistance(const TVector3& vector) const throw())
	v2 = Vector3(1.0, 2.0, 4.0);
	v  = Vector3(0, 1.0, 2.0);
	TEST_REAL_EQUAL(v.getDistance(v2) , sqrt(6.0))
RESULT

CHECK(T getSquareDistance(const TVector3& vector) const throw())
	v2 = Vector3(1.0, 2.0, 4.0);
	v  = Vector3(0, 1.0, 2.0);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 6.0)
RESULT

CHECK(TAngle<T> getAngle(const TVector3& vector) const throw(Exception::DivisionByZero))
	float r, e;
	v2 = Vector3(1.0, 2.0, 4.0);
	v  = Vector3(0, 1.0, 2.0);
	r = v.getSquareLength() * v2.getSquareLength();
  e = acos((v * v2) / sqrt(r));
	TEST_REAL_EQUAL(v.getAngle(v2) , e)
	v2.clear();
	TEST_EXCEPTION(Exception::DivisionByZero, v2.getAngle(v))
RESULT

CHECK(TVector3 getOrthogonalProjection(const TVector3& direction) const throw())
	v  = Vector3(0, 1.0, 2.0);
	v1 = Vector3(1.0, 2.0, 4.0);
	v2 = v.getOrthogonalProjection(v1);
	TEST_EQUAL(v2 == (v1 * v / (v1 * v1) * v1), true)
RESULT

CHECK(bool operator == (const TVector3& vector) const throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(1.0, 2.0, 2.0);
	TEST_EQUAL(v == v2 , false)
	v  = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v == v2 , true)
RESULT

CHECK(bool operator != (const TVector3& vector) const throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(1.0, 2.0, 2.0);
	TEST_EQUAL(v != v2 , true)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v != v2 , false)
RESULT

CHECK(bool isZero() const throw())
	v = Vector3(1.0, 0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 1.0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 0, 1.0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 0, 0);
	TEST_EQUAL(v.isZero() , true)
RESULT

CHECK(bool isOrthogonalTo(TVector3& vector) const throw())
	v2 = Vector3(1.0, 0, 3.0);
	v  = Vector3(0, 2.0, 0);
	TEST_EQUAL(v.isOrthogonalTo(v2), true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2), false)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Vector3 v(1.2, 2.3, 3.4);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	v.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Vector3_test.txt))
RESULT

CHECK(bool isValid() const throw())
	v  = Vector3(0, 2.0, 0);
	TEST_EQUAL(v.isValid(), true)
RESULT

CHECK(TVector3 operator + (const TVector3& b) const throw())
 	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = v1 + v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3 operator - (const TVector3& b) const throw())
 	v1 = Vector3(2.0, 3.0, 4.0);
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = v1 - v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], 1.0)
RESULT

CHECK(TVector3 operator * (const T& scalar, const TVector3& vector))
 	v = Vector3(1.0, 2.0, 3.0);
	v = (float) 2.0 * v;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3 operator * (const TVector3& vector, const T& scalar))
 	v = Vector3(1.0, 2.0, 3.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(static T getTripleProduct(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c) throw())
 	v = Vector3(1.0, 2.0, 3.0);
 	v1 = Vector3(2.0, 4.0, 6.0);
 	v2 = Vector3(3.0, 6.0, 9.0);
	TEST_REAL_EQUAL(v.getTripleProduct(v, v1, v2), 0)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TVector3& vector))
	std::ifstream instr(BALL_TEST_DATA_PATH(Vector3_test2.txt));
	Vector3 v(1, 2, 3);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
	TEST_REAL_EQUAL(v.z, 3.4)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector3& vector))
	Vector3 v(1.2, 2.3, 3.4);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Vector3_test2.txt))
RESULT

CHECK(TVector3& negate() throw())
	v2 = Vector3(1.0, 2.0, 3.0);
	v2.negate();
	TEST_REAL_EQUAL(v2[0], -1.0)
	TEST_REAL_EQUAL(v2[1], -2.0)
	TEST_REAL_EQUAL(v2[2], -3.0)
RESULT

CHECK(static TVector3 getPerpendicularNormalization(const TVector3& a, const TVector3& b, const TVector3& c) throw())
	Vector3 v1(0,0,0);
	Vector3 v2(1,0,0);
	Vector3 v3(0,1,0);
	TEST_EQUAL(Vector3::getPerpendicularNormalization(v1, v2, v3), Vector3(0, 0, -1))
	TEST_EQUAL(Vector3::getPerpendicularNormalization(v1, v1, v1), Vector3(0, 0, 0))
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
