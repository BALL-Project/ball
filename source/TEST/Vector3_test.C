// $Id: Vector3_test.C,v 1.24 2000/05/26 19:25:05 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <math.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/MATHS/angle.h>
///////////////////////////

START_TEST(TVector3, "$Id: Vector3_test.C,v 1.24 2000/05/26 19:25:05 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(TVector3::BALL_CREATE(TVector3<T>))
	Vector3 v(1.0, 2.0, 3.0);
	Vector3* v_ptr = (Vector3*)v.create(false, true);
	TEST_REAL_EQUAL(v_ptr->x, 0.0)
	TEST_REAL_EQUAL(v_ptr->y, 0.0)
	TEST_REAL_EQUAL(v_ptr->z, 0.0)
	delete v_ptr;
	v_ptr = (Vector3*)v.create();
	TEST_REAL_EQUAL(v_ptr->x, 1.0)
	TEST_REAL_EQUAL(v_ptr->y, 2.0)
	TEST_REAL_EQUAL(v_ptr->z, 3.0)
	delete v_ptr;
RESULT

CHECK(TVector3();)
  Vector3* v;
	v = new Vector3();
	TEST_NOT_EQUAL(v, 0)
RESULT								

CHECK(~TVector3();)
  Vector3* v;
	v = new Vector3();
	delete v;
RESULT		

Vector3 v;
Vector3 v1;
Vector3 v2;

CHECK(TVector3::T& operator [] (Index index) const)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
	TEST_EXCEPTION(Exception::IndexUnderflow, v[-1])
	TEST_EXCEPTION(Exception::IndexOverflow,  v[3])
RESULT

CHECK(TVector3(const T* ptr);)
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

CHECK(TVector3(const T& value);)
	v = Vector3(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], 1.0)
RESULT

CHECK(TVector3(const T& x, const T& y, const T& z);)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

CHECK(TVector3(const TVector3& vector);)
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(v2);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

CHECK(TVector3(const T& r, const TAngle<T>& phi, const TAngle<T>& theta);)
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
CHECK(virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;)
	Vector3 v(1.0, 2.0, 3.0);
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), ios::out);
	pm.setOstream(ofile);
	pm.registerClass(getStreamName<Vector3>(), Vector3::createDefault);
	v >> pm;
	ofile.close();	
RESULT

using std::ifstream;
using std::cout;
CHECK(virtual void persistentRead(PersistenceManager& pm);)
	ifstream  ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject* ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Vector3>(*ptr), true)
		if (isKindOf<Vector3>(*ptr))
		{
			Vector3* v_ptr = castTo<Vector3>(*ptr);
			TEST_REAL_EQUAL(v_ptr->x, 1.0)
			TEST_REAL_EQUAL(v_ptr->y, 2.0)
			TEST_REAL_EQUAL(v_ptr->z, 3.0)
		}
	}
RESULT

CHECK(TVector3<T>::set(const T* ptr))
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

CHECK(TVector3<T>::set(const T& value))
	v.set(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], 1.0)
RESULT

CHECK(TVector3::set(const T& x, const T& y, const T& z))
	v.set(1.0, 2.0, 3.0);
	v2 = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector3::set(const TVector3& vector);)
	v = Vector3(1.0, 2.0, 3.0);
	v2.set(v);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector3::set(const T& r, const TAngle<T>& phi, const TAngle<T> &theta))
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

CHECK(TVector3::operator = (const T* ptr))
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

CHECK(TVector3<T>& TVector3<T>::operator = (const TVector3<T>& v))
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector3::get(T* ptr) const )
	float arr[3];
	v = Vector3(1.0, 2.0, 3.0);
	v.get(arr);
	TEST_REAL_EQUAL(arr[0], 1.0)
	TEST_REAL_EQUAL(arr[1], 2.0)
	TEST_REAL_EQUAL(arr[2], 3.0)
	TEST_EXCEPTION(Exception::NullPointer, v.get((float*)0))
RESULT

CHECK(TVector3::get(T& rx, T& ry, T& rz) const)
	float a, b, c;
	v = Vector3(1.0, 2.0, 3.0);
	v.get(a,b,c);
	TEST_REAL_EQUAL(a, 1.0)
	TEST_REAL_EQUAL(b, 2.0)
	TEST_REAL_EQUAL(c, 3.0)
RESULT

CHECK(TVector3::get(TVector3<T>& v) const )
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3();
	v.get(v2);
	TEST_EQUAL(v2, v)
RESULT

CHECK(get(T& r, TAngle<T>& phi, TAngle<T>& theta) const)
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

CHECK(TVector3<T>::swap(TVector3<T>& v))
	v  = Vector3(1.0, 2.0, 3.0);
	Vector3 u2 = v;

	Vector3 u(4.0, 3.0, 2.0);
	v2 = u;

	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

CHECK(TVector3::getLength() const )
	v = Vector3(4.0, 9.0, 16.0);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0))
RESULT

CHECK(TVector3::getSquareLength() const )
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 14.0)
RESULT

CHECK(TVector3::normalize())
	v = Vector3(4.0, 9.0, 16.0);
	v.normalize();
	float erg = ::sqrt (4.0 * 4.0 + 9.0 * 9.0 + 16.0 * 16.0);
	TEST_REAL_EQUAL(v[0], 4.0 / erg)
	TEST_REAL_EQUAL(v[1], 9.0 / erg)
	TEST_REAL_EQUAL(v[2], 16.0 / erg)
	v = Vector3(0.0, 0.0, 0.0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT

CHECK(TVector3::getZero())
	TEST_REAL_EQUAL(Vector3::getZero().x, 0.0)
	TEST_REAL_EQUAL(Vector3::getZero().y, 0.0)
	TEST_REAL_EQUAL(Vector3::getZero().z, 0.0)
RESULT

CHECK(TVector3::getUnit())
	TEST_REAL_EQUAL(Vector3::getUnit().x, 1.0)
	TEST_REAL_EQUAL(Vector3::getUnit().y, 1.0)
	TEST_REAL_EQUAL(Vector3::getUnit().z, 1.0)
RESULT

CHECK(TVector3::T& operator [] (Index index) )
	v = Vector3(1.0, 2.0, 3.0);
	v[0]=5.0;	v[1]=6.0;	v[2]=7.0;
	TEST_REAL_EQUAL(v[0], 5.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_REAL_EQUAL(v[2], 7.0)
	TEST_EXCEPTION(Exception::IndexUnderflow, v[-1] = 5.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[3] = 5.0)
RESULT

CHECK(TVector3::TVector3 operator + () const )
	v2 = Vector3(1.0, 2.0, 3.0);
	v = + v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

CHECK(TVector3::TVector3 operator - () const )
	v2 = Vector3(1.0, 2.0, 3.0);
	v = - v2;
	TEST_REAL_EQUAL(v[0], -1.0)
	TEST_REAL_EQUAL(v[1], -2.0)
	TEST_REAL_EQUAL(v[2], -3.0)
RESULT

CHECK(TVector3::TVector3& operator += (const TVector3& vector))
	v2 = Vector3(1.0, 2.0 ,3.0);
	v  = Vector3(1.0, 2.0, 3.0);
	v += v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3::TVector3& operator -= (const TVector3& vector))
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(4.0, 3.0, 2.0);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], -1.0)
RESULT

CHECK(TVector3::TVector3 operator * (const T& scalar))
	v  = Vector3(1.0, 2.0, 3.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3::TVector3& operator *= (const T& scalar))
	v  = Vector3(1.0, 2.0, 3.0);
	v *= 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3::TVector3 operator / (const T& scalar))
	v  = Vector3(1.0, 2.0, 3.0);
	v = v / 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

CHECK(TVector3::TVector3& operator /= (const T& scalar))
	v  = Vector3(1.0, 2.0, 3.0);
	v /= 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

CHECK(TVector3::T operator * (const TVector3& vector) const )
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v * v2 , 14.0)
RESULT

CHECK(TVector3 operator % (const TVector3& vector) const)
 	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v  = v1 % v2;
	TEST_REAL_EQUAL(v[0], -3.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_REAL_EQUAL(v[2], -3.0)
RESULT

CHECK(TVector3 operator %= (const TVector3& vector))
 	v  = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(4.0, 5.0, 6.0);
	v  %= v2;
	TEST_REAL_EQUAL(v[0], -3.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_REAL_EQUAL(v[2], -3.0)
RESULT

CHECK(TVector3::getDistance(const TVector3& vector) const )
	v2 = Vector3(1.0, 2.0, 4.0);
	v  = Vector3(0, 1.0, 2.0);
	TEST_REAL_EQUAL(v.getDistance(v2) , sqrt(6.0))
RESULT

CHECK(TVector3::getSquareDistance(const TVector3& vector) const )
	v2 = Vector3(1.0, 2.0, 4.0);
	v  = Vector3(0, 1.0, 2.0);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 6.0)
RESULT

CHECK(TAngle<T> getAngle(const TVector3& vector) const)
	float r, e;
	v2 = Vector3(1.0, 2.0, 4.0);
	v  = Vector3(0, 1.0, 2.0);
	r = v.getSquareLength() * v2.getSquareLength();
  e = acos((v * v2) / sqrt(r));
	TEST_REAL_EQUAL(v.getAngle(v2) , e)
RESULT

CHECK(TVector3<T> TVector3<T>::getOrthogonalProjection(const TVector3<T>& direction) const)
	v  = Vector3(0, 1.0, 2.0);
	v1 = Vector3(1.0, 2.0, 4.0);
	v2 = v.getOrthogonalProjection(v1);
	TEST_EQUAL(v2 == (v1 * v / (v1 * v1) * v1), true)
RESULT

CHECK(TVector3::bool operator == (const TVector3& vector) const )
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(1.0, 2.0, 2.0);
	TEST_EQUAL(v == v2 , false)
	v  = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v == v2 , true)
RESULT

CHECK(TVector3::bool operator != (const TVector3& vector) const )
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = Vector3(1.0, 2.0, 2.0);
	TEST_EQUAL(v != v2 , true)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_EQUAL(v != v2 , false)
RESULT

CHECK(TVector3::isZero() const )
	v = Vector3(1.0, 0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 1.0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 0, 1.0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 0, 0);
	TEST_EQUAL(v.isZero() , true)
RESULT

CHECK(TVector3::isOrthogonalTo(TVector3& vector) const )
	v2 = Vector3(1.0, 0, 3.0);
	v  = Vector3(0, 2.0, 0);
	TEST_EQUAL(v.isOrthogonalTo(v2), true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2), false)
RESULT

CHECK(TVector3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Vector3 v(1.2, 2.3, 3.4);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	v.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Vector3_test.txt", true)
RESULT

CHECK(TVector3::isValid() const )
	v  = Vector3(0, 2.0, 0);
	TEST_EQUAL(v.isValid(), true)
RESULT

CHECK(TVector3 operator + (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(1.0, 2.0, 3.0);
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = v1 + v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3 operator - (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(2.0, 3.0, 4.0);
	v2 = Vector3(1.0, 2.0, 3.0);
	v  = v1 - v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
	TEST_REAL_EQUAL(v[2], 1.0)
RESULT

CHECK(TVector3 operator * (const T& scalar, const TVector3<T>& vector))
 	v = Vector3(1.0, 2.0, 3.0);
	v = (float) 2.0 * v;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(TVector3 operator * (const TVector3<T>& vector, const T& scalar))
 	v = Vector3(1.0, 2.0, 3.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_REAL_EQUAL(v[2], 6.0)
RESULT

CHECK(T getTripleProduct (const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c))
 	v = Vector3(1.0, 2.0, 3.0);
 	v1 = Vector3(2.0, 4.0, 6.0);
 	v2 = Vector3(3.0, 6.0, 9.0);
	TEST_REAL_EQUAL(v.getTripleProduct(v, v1, v2), 0)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TVector3<T>& vector))
	std::ifstream instr("data/Vector3_test2.txt");
	Vector3 v(1, 2, 3);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
	TEST_REAL_EQUAL(v.z, 3.4)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector3<T>& vector))
	Vector3 v(1.2, 2.3, 3.4);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Vector3_test2.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
