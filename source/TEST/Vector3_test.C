// $Id: Vector3_test.C,v 1.12 2000/02/25 21:22:52 amoll Exp $ #include
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector3.h>
#include <math.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/MATHS/angle.h>
///////////////////////////

START_TEST(TVector3, "$Id: Vector3_test.C,v 1.12 2000/02/25 21:22:52 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

//line 64
CHECK(TVector3::BALL_CREATE(TVector3<T>))
	Vector3 v(1, 2, 3);
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

//line 74
CHECK(TVector3();)
  Vector3* v;
	v = new Vector3();
	TEST_NOT_EQUAL(0, v)
RESULT								

//line 160
CHECK(~TVector3();)
  Vector3* v;
	v = new Vector3();
	delete v;
RESULT		

Vector3 v;
Vector3 v1;
Vector3 v2;

//line 321
CHECK(TVector3::T& operator [] (Index index) const)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
	TEST_EXCEPTION(Exception::IndexUnderflow, v[-1])
	TEST_EXCEPTION(Exception::IndexOverflow,  v[3])
RESULT


//line 88
CHECK(TVector3(const T* ptr);)
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3(arr);
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 2)
	TEST_REAL_EQUAL(v[2], 3)
	TEST_EXCEPTION(Exception::NullPointer, v = Vector3((float*)0))
RESULT

//line 104
CHECK(TVector3(const T& value);)
	v = Vector3(1);
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], 1)
RESULT

//line 118
CHECK(TVector3(const T& x, const T& y, const T& z);)
	v = Vector3(1.0, 2.0, 3.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_REAL_EQUAL(v[2], 3.0)
RESULT

//line 131
CHECK(TVector3(const TVector3& vector, bool deep = true);)
	v2 = Vector3(1, 2, 3);
	v  = Vector3(v2);
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 2)
	TEST_REAL_EQUAL(v[2], 3)
RESULT

//line 148
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

//line 160 s.o.

//line 174
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
	pm.registerClass(getStreamName<Vector3>(), getNew<Vector3>);
	v >> pm;
	ofile.close();	
RESULT

//line 180
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

//line 194
CHECK(TVector3<T>::set(const T* ptr))
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3();
	v.set(arr);
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 2)
	TEST_REAL_EQUAL(v[2], 3)
	TEST_EXCEPTION(Exception::NullPointer, v.set((float*)0))
RESULT

//line 200
CHECK(TVector3<T>::set(const T& value))
	v.set(1);
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], 1)
RESULT

//line 207
CHECK(TVector3::set(const T& x, const T& y, const T& z))
	v.set(1, 2, 3);
	v2 = Vector3(1, 2, 3);
	TEST_EQUAL(v2, v)
RESULT

//line 213
CHECK(TVector3::set(const TVector3& vector, bool deep = true);)
	v = Vector3(1, 2, 3);
	v2.set(v, true);
	TEST_EQUAL(v2, v)
RESULT

//line 230
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

//line 236
CHECK(TVector3::operator = (const T* ptr))
	float arr[3];
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	v = Vector3();
	v = arr;
	v2 = Vector3(1,2,3);
	TEST_EQUAL(v2, v)
	TEST_EXCEPTION(Exception::NullPointer, v = ((float*)0))
RESULT

// line 243
CHECK(TVector3<T>& TVector3<T>::operator = (const TVector3<T>& v))
	v2 = Vector3(1,2,3);
	v  = Vector3();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT


//line 251
CHECK(TVector3::get(T* ptr) const )
	float arr[3];
	v = Vector3(1,2,3);
	v.get(arr);
	TEST_REAL_EQUAL(arr[0], 1)
	TEST_REAL_EQUAL(arr[1], 2)
	TEST_REAL_EQUAL(arr[2], 3)
	TEST_EXCEPTION(Exception::NullPointer, v.get((float*)0))

RESULT

//line 258
CHECK(TVector3::get(T& rx, T& ry, T& rz) const)
	float a,b,c;
	v = Vector3(1,2,3);
	v.get(a,b,c);
	TEST_REAL_EQUAL(a, 1)
	TEST_REAL_EQUAL(b, 2)
	TEST_REAL_EQUAL(c, 3)
RESULT

//line 265
CHECK(TVector3::get(TVector3<T>& v, bool deep) const )
	v2 = Vector3(1,2,3);
	v  = Vector3();
	v.get(v2, false);
	TEST_EQUAL(v2, v)
RESULT

//line 274
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

//line 279
CHECK(TVector3<T>::swap(TVector3<T>& v))
	v  = Vector3(1, 2, 3);
	Vector3 u2 = v;

	Vector3 u(4, 3, 2);
	v2 = u;

	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

//line 286
CHECK(TVector3::getLength() const )
	v = Vector3(4, 9, 16);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4. * 4. + 9. * 9. + 16. * 16.))
RESULT

//line 293
CHECK(TVector3::getSquareLength() const )
	v = Vector3(1, 2, 3);
	TEST_REAL_EQUAL(v.getSquareLength(), 14)
RESULT

//line 301
CHECK(TVector3::normalize())
	v = Vector3(4, 9, 16);
	v.normalize();
	float erg = sqrt (4 *4 + 9 * 9 + 16 * 16);
	TEST_REAL_EQUAL(v[0], 4 / erg)
	TEST_REAL_EQUAL(v[1], 9 / erg)
	TEST_REAL_EQUAL(v[2], 16 / erg)
	v = Vector3(0, 0, 0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT


//line 305
CHECK(TVector3::getZero())
	TEST_REAL_EQUAL(Vector3::getZero().x, 0.0)
	TEST_REAL_EQUAL(Vector3::getZero().y, 0.0)
	TEST_REAL_EQUAL(Vector3::getZero().z, 0.0)
RESULT

//line 309
CHECK(TVector3::getUnit())
	TEST_REAL_EQUAL(Vector3::getUnit().x, 1.0)
	TEST_REAL_EQUAL(Vector3::getUnit().y, 1.0)
	TEST_REAL_EQUAL(Vector3::getUnit().z, 1.0)
RESULT

//line 315
CHECK(TVector3::T& operator [] (Index index) )
	v = Vector3(1, 2, 3);
	v[0]=5;	v[1]=6;	v[2]=7;
	TEST_REAL_EQUAL(v[0], 5)
	TEST_REAL_EQUAL(v[1], 6)
	TEST_REAL_EQUAL(v[2], 7)
	TEST_EXCEPTION(Exception::IndexUnderflow, v[-1] = 5)
	TEST_EXCEPTION(Exception::IndexOverflow, v[3] = 5)
RESULT

//line 321 s.o.

//line 330
CHECK(TVector3::TVector3 operator + () const )
	v2 = Vector3(1, 2, 3);
	v = + v2;
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 2)
	TEST_REAL_EQUAL(v[2], 3)
RESULT

//line 334
CHECK(TVector3::TVector3 operator - () const )
	v2 = Vector3(1, 2, 3);
	v = - v2;
	TEST_REAL_EQUAL(v[0], -1)
	TEST_REAL_EQUAL(v[1], -2)
	TEST_REAL_EQUAL(v[2], -3)
RESULT

//line 341
CHECK(TVector3::TVector3& operator += (const TVector3& vector))
	v2 = Vector3(1, 2 ,3);
	v  = Vector3(1, 2, 3);
	v += v2;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
RESULT

//line 348
CHECK(TVector3::TVector3& operator -= (const TVector3& vector))
	v2 = Vector3(1, 2, 3);
	v  = Vector3(4, 3, 2);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], -1)
RESULT

//line 355
CHECK(TVector3::TVector3 operator * (const T& scalar))
	v  = Vector3(1, 2, 3);
	v = v * 2;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
RESULT

//line 362
CHECK(TVector3::TVector3& operator *= (const T& scalar))
	v  = Vector3(1, 2, 3);
	v *= 2;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
RESULT

//line 370
CHECK(TVector3::TVector3 operator / (const T& scalar))
	v  = Vector3(1, 2, 3);
	v = v / 0.5;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

//line 377
CHECK(TVector3::TVector3& operator /= (const T& scalar))
	v  = Vector3(1, 2, 3);
	v /= 0.5;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

//line 382
CHECK(TVector3::T operator * (const TVector3& vector) const )
	v2 = Vector3(1, 2, 3);
	v  = Vector3(1, 2, 3);
	TEST_REAL_EQUAL(v * v2 , 14)
RESULT

//line 387
CHECK(TVector3 operator % (const TVector3& vector) const)
 	v1 = Vector3(1, 2, 3);
	v2 = Vector3(4, 5, 6);
	v  = v1 % v2;
	TEST_REAL_EQUAL(v[0], -3)
	TEST_REAL_EQUAL(v[1], 6)
	TEST_REAL_EQUAL(v[2], -3)
RESULT

//line 392
CHECK(TVector3 operator %= (const TVector3& vector))
 	v  = Vector3(1, 2, 3);
	v2 = Vector3(4, 5, 6);
	v  %= v2;
	TEST_REAL_EQUAL(v[0], -3)
	TEST_REAL_EQUAL(v[1], 6)
	TEST_REAL_EQUAL(v[2], -3)
RESULT


//line 402
CHECK(TVector3::getDistance(const TVector3& vector) const )
	v2 = Vector3(1, 2, 4);
	v  = Vector3(0, 1, 2);
	TEST_REAL_EQUAL(v.getDistance(v2) , sqrt(6.0))
RESULT

//line 406
CHECK(TVector3::getSquareDistance(const TVector3& vector) const )
	v2 = Vector3(1, 2, 4);
	v  = Vector3(0, 1, 2);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 6.0)
RESULT

//line 412
CHECK(TAngle<T> getAngle(const TVector3& vector) const)
	float r, e;
	v2 = Vector3(1, 2, 4);
	v  = Vector3(0, 1, 2);
	r = v.getSquareLength() * v2.getSquareLength();
  e = acos((v * v2) / sqrt(r));
	TEST_REAL_EQUAL(v.getAngle(v2) , e)
RESULT

//line 417
CHECK(TVector3<T> TVector3<T>::getOrthogonalProjection(const TVector3<T>& direction) const)
	v  = Vector3(0, 1, 2);
	v1 = Vector3(1, 2, 4);
	v2 = v.getOrthogonalProjection(v1);
	TEST_EQUAL(v2 == (v1 * v / (v1 * v1) * v1), true)
RESULT

//line 436
CHECK(TAngle<T> TVector3<T>::getTorsionTAngle())
	Vector3 a, b, c, d, e;
	Angle angle;
	a = Vector3(0, 1, 2);
	b = Vector3(100, 201, 302);
	c = Vector3(50, 101, 202);
	d = Vector3(200, 401, 602);
	angle = a.getTorsionTAngle(a, b, c, d);
  TEST_EQUAL(angle == 0, true )
	d = Vector3(50, 101, 602);
	angle = a.getTorsionTAngle(a, b, c, d);
  TEST_EQUAL(angle == 0, true )
RESULT

//line 449
CHECK(TVector3::bool operator == (const TVector3& vector) const )
	v2 = Vector3(1, 2, 3);
	v  = Vector3(1, 2, 2);
	TEST_EQUAL(v == v2 , false)
	v  = Vector3(1, 2, 3);
	TEST_EQUAL(v == v2 , true)
RESULT

//line 454
CHECK(TVector3::bool operator != (const TVector3& vector) const )
	v2 = Vector3(1, 2, 3);
	v  = Vector3(1, 2, 2);
	TEST_EQUAL(v != v2 , true)
	v = Vector3(1,2,3);
	TEST_EQUAL(v != v2 , false)
RESULT

//line 458
CHECK(TVector3::isZero() const )
	v = Vector3(1, 0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 1, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 0, 1);
	TEST_EQUAL(v.isZero() , false)
	v = Vector3(0, 0, 0);
	TEST_EQUAL(v.isZero() , true)
RESULT

//line 462
CHECK(TVector3::isOrthogonalTo(TVector3& vector) const )
	v2 = Vector3(1, 0, 3);
	v  = Vector3(0, 2, 0);
	TEST_EQUAL(v.isOrthogonalTo(v2), true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2), false)
RESULT

//line 471
CHECK(TVector3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Vector3 v(1.2, 2.3, 3.4);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	v.dump(outfile);
	outfile.close();
	
	TEST_FILE(filename.c_str(), "data/Vector3_test.txt", true)
RESULT

//line 477
CHECK(TVector3::isValid() const )
	v  = Vector3(0,2,0);
	TEST_EQUAL(v.isValid(), true)
RESULT

//line 42
CHECK(TVector3 operator + (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(1, 2, 3);
	v2 = Vector3(1, 2, 3);
	v  = v1 + v2;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
RESULT

//line 46
CHECK(TVector3 operator - (const TVector3<T>& a, const TVector3<T>& b))
 	v1 = Vector3(2, 3, 4);
	v2 = Vector3(1, 2, 3);
	v  = v1 - v2;
	TEST_REAL_EQUAL(v[0], 1)
	TEST_REAL_EQUAL(v[1], 1)
	TEST_REAL_EQUAL(v[2], 1)
RESULT

//line 1020
CHECK(TVector3 operator * (const T& scalar, const TVector3<T>& vector))
 	v = Vector3(1, 2, 3);
	v = (float) 2.0 * v;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
RESULT

//line 1028
CHECK(TVector3 operator * (const TVector3<T>& vector, const T& scalar))
 	v = Vector3(1, 2, 3);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2)
	TEST_REAL_EQUAL(v[1], 4)
	TEST_REAL_EQUAL(v[2], 6)
RESULT

//line 
CHECK(std::istream& operator >> (std::istream& s, TVector3<T>& vector))
	std::ifstream instr("data/Vector3_test2.txt");
	Vector3 v(1, 2, 3);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
	TEST_REAL_EQUAL(v.z, 3.4)
RESULT


//line 
NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector3<T>& vector))
	Vector3 v(1.2, 2.3, 3.4);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Vector3_test2.txt", false)
RESULT


/////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
