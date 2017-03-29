// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/MATHS/vector2.h>
#include <cmath>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(TVector2)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

Vector2* vector2_ptr = 0;
CHECK(TVector2() throw())
	vector2_ptr = new Vector2;
	TEST_NOT_EQUAL(vector2_ptr, 0)
RESULT								

CHECK(~TVector2() throw())
	delete vector2_ptr;
RESULT		

CHECK(BALL_CREATE(TVector2<T>))
	Vector2 v(1.0, 2.0);
	Vector2* v_ptr = (Vector2*)v.create(false, true);
	TEST_REAL_EQUAL(v_ptr->x, 0.0)
	TEST_REAL_EQUAL(v_ptr->y, 0.0)
	delete v_ptr;
	v_ptr = (Vector2*)v.create();
	TEST_REAL_EQUAL(v_ptr->x, 1.0)
	TEST_REAL_EQUAL(v_ptr->y, 2.0)
	delete v_ptr;
RESULT

Vector2 v;
Vector2 v1;
Vector2 v2;

CHECK(void clear() throw())
	Vector2 v1(1, 2);
	Vector2 v2;
	v2.set(0, 0);
	v1.clear();
	TEST_EQUAL(v1, v2)
RESULT

CHECK(const T& operator [] (Position position) const throw(Exception::IndexOverflow))
	v = Vector2(1.0, 2.0);
	const Vector2& c_v(v);
	TEST_REAL_EQUAL(c_v[0], 1.0)
	TEST_REAL_EQUAL(c_v[1], 2.0)
	TEST_EXCEPTION(Exception::IndexOverflow, c_v[12354])
	TEST_EXCEPTION(Exception::IndexOverflow,  c_v[2])
RESULT

CHECK(TVector2(const T& value) throw())
	v = Vector2(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
RESULT

CHECK(TVector2(const T& vx, const T& vy) throw())
	v = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
RESULT

CHECK(TVector2(const TVector2& vector) throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(v2);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
RESULT

String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
TextPersistenceManager pm;
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	Vector2 v(1.0, 2.0);
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	pm.registerClass(getStreamName<Vector2>(), Vector2::createDefault);
	v >> pm;
	ofile.close();	
RESULT

using std::ifstream;
using std::cout;
CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	ifstream  ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject* ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
        TEST_EQUAL(isKindOf<Vector2>(ptr), true)
        if (isKindOf<Vector2>(ptr))
		{
			Vector2* v_ptr = castTo<Vector2>(*ptr);
			TEST_REAL_EQUAL(v_ptr->x, 1.0)
			TEST_REAL_EQUAL(v_ptr->y, 2.0)
		}
		delete ptr;
	}
RESULT

CHECK(void set(const T& value) throw())
	v.set(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
RESULT

CHECK(void set(const T& vx, const T& vy) throw())
	v.set(1.0, 2.0);
	v2 = Vector2(1.0, 2.0);
	TEST_EQUAL(v2, v)
RESULT

CHECK(void set(const TVector2& vector) throw())
	v = Vector2(1.0, 2.0);
	v2.set(v);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector2& operator = (const TVector2& v) throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector2& operator = (const T* ptr) throw(Exception::NullPointer))
	v2 = Vector2(1.0, 2.0);
	float x[2] = {1.0, 2.0};
	v  = Vector2();
	v = x;
	TEST_EQUAL(v2, v)
RESULT


CHECK(TVector2& operator = (const T& value) throw())
	v2 = 1.1;
	TEST_REAL_EQUAL(v2.x, (float) 1.1)
	TEST_REAL_EQUAL(v2.y, (float) 1.1)
RESULT


CHECK(T getLength() const throw())
	v = Vector2(4.0, 9.0);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4.0 * 4.0 + 9.0 * 9.0))
	v = Vector2(0.0, 0.0);
	TEST_REAL_EQUAL(v.getLength(), 0.0)
RESULT

CHECK(T getSquareLength() const throw())
	v = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 5.0)
	v = Vector2(0.0, 0.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 0.0)
RESULT

CHECK(TVector2& normalize() throw(Exception::DivisionByZero))
	v = Vector2(4.0, 9.0);
	v.normalize();
	float erg = ::sqrt (4.0 * 4.0 + 9.0 * 9.0);
	TEST_REAL_EQUAL(v[0], 4.0 / erg)
	TEST_REAL_EQUAL(v[1], 9.0 / erg)
	v = Vector2(0.0, 0.0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT

CHECK(static const TVector2& getZero() throw())
	TEST_REAL_EQUAL(Vector2::getZero().x, 0.0)
	TEST_REAL_EQUAL(Vector2::getZero().y, 0.0)
RESULT

CHECK(static const TVector2& getUnit() throw())
	TEST_REAL_EQUAL(Vector2::getUnit().x, 1.0)
	TEST_REAL_EQUAL(Vector2::getUnit().y, 1.0)
RESULT

CHECK(T& operator [] (Position position) throw(Exception::IndexOverflow))
	v = Vector2(1.0, 2.0);
	v[0]=5.0;	v[1]=6.0;	
	TEST_REAL_EQUAL(v[0], 5.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[32472] = 5.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[2] = 5.0)
RESULT

CHECK(const TVector2& operator + () const throw())
	v2 = Vector2(1.0, 2.0);
	v = + v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
RESULT

CHECK(TVector2 operator - () const throw())
	v2 = Vector2(1.0, 2.0);
	v = - v2;
	TEST_REAL_EQUAL(v[0], -1.0)
	TEST_REAL_EQUAL(v[1], -2.0)
RESULT

CHECK(TVector2& negate() throw())
	v2 = Vector2(1.0, 2.0);
	v2.negate();
	TEST_REAL_EQUAL(v[0], -1.0)
	TEST_REAL_EQUAL(v[1], -2.0)
RESULT

CHECK(TVector2& operator += (const TVector2& vector) throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.0);
	v += v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2& operator -= (const TVector2& vector) throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(4.0, 3.0);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3.0)
	TEST_REAL_EQUAL(v[1], 1.0)
RESULT

CHECK(TVector2 operator * (const T& scalar) const throw())
	v  = Vector2(1.0, 2.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2& operator *= (const T& scalar) throw())
	v  = Vector2(1.0, 2.0);
	v *= 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2 operator / (const T& lambda) const throw(Exception::DivisionByZero))
	v  = Vector2(1.0, 2.0);
	v = v / 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

CHECK(TVector2& operator /= (const T& lambda) throw(Exception::DivisionByZero))
	v  = Vector2(1.0, 2.0);
	v /= 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

CHECK(T operator * (const TVector2& vector) const throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v * v2 , 5.0)
RESULT

CHECK(T getDistance(const TVector2& vector) const throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(0, 1.0);
	TEST_REAL_EQUAL(v.getDistance(v2) , sqrt(2.0))
RESULT

CHECK(T getSquareDistance(const TVector2& vector) const throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(0, 1.0);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 2.0)
RESULT

CHECK(bool operator == (const TVector2& vector) const throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.1);
	TEST_EQUAL(v == v2 , false)
	v  = Vector2(1.0, 2.0);
	TEST_EQUAL(v == v2 , true)
RESULT

CHECK(bool operator != (const TVector2& vector) const throw())
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.1);
	TEST_EQUAL(v != v2 , true)
	v = Vector2(1.0, 2.0);
	TEST_EQUAL(v != v2 , false)
RESULT

CHECK(bool isZero() const throw())
	v = Vector2(1.0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector2(0, 1.0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector2(0, 0);
	TEST_EQUAL(v.isZero() , true)
RESULT

CHECK(bool isOrthogonalTo(TVector2& vector) const throw())
	v2 = Vector2(1.0, 0);
	v  = Vector2(0, 2.0);
	TEST_EQUAL(v.isOrthogonalTo(v2), true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2), false)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Vector2 v(1.2, 2.3);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	v.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Vector2_test.txt))
RESULT

CHECK(bool isValid() const throw())
	v  = Vector2(0, 2.0);
	TEST_EQUAL(v.isValid(), true)
RESULT

CHECK(TVector2 operator + (const TVector2& b) const throw())
 	v1 = Vector2(1.0, 2.0);
	v2 = Vector2(1.0, 2.0);
	v  = v1 + v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2 operator - (const TVector2& b) const throw())
 	v1 = Vector2(2.0, 4.0);
	v2 = Vector2(1.0, 2.0);
	v  = v1 - v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
RESULT

CHECK(TVector2 operator * (const T& scalar, const TVector2<T>& vector))
 	v = Vector2(1.0, 2.0);
	v = (float) 2.0 * v;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2 operator * (const TVector2<T>& vector, const T& scalar))
 	v = Vector2(1.0, 2.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(std::istream& operator >> (std::istream& s, TVector2<T>& vector))
	std::ifstream instr(BALL_TEST_DATA_PATH(Vector2_test2.txt));
	Vector2 v(1, 2);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector2<T>& vector))
	Vector2 v(1.2, 2.3);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Vector2_test2.txt))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
