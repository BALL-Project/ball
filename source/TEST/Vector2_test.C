// $Id: Vector2_test.C,v 1.1 2001/05/31 21:51:51 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/vector2.h>
#include <math.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/MATHS/angle.h>
///////////////////////////

START_TEST(TVector2, "$Id: Vector2_test.C,v 1.1 2001/05/31 21:51:51 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

CHECK(TVector2::BALL_CREATE(TVector2<T>))
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

CHECK(TVector2();)
  Vector2* v;
	v = new Vector2();
	TEST_NOT_EQUAL(v, 0)
RESULT								

CHECK(~TVector2();)
  Vector2* v;
	v = new Vector2();
	delete v;
RESULT		

Vector2 v;
Vector2 v1;
Vector2 v2;

CHECK(clear())
	Vector2 v1(1, 2);
	Vector2 v2;
	v2.set(0, 0);
	v1.clear();
	TEST_EQUAL(v1, v2)
RESULT

CHECK(TVector2::T& operator [] (Index index) const)
	v = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[12354])
	TEST_EXCEPTION(Exception::IndexOverflow,  v[2])
RESULT

CHECK(TVector2(const T& value);)
	v = Vector2(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
RESULT

CHECK(TVector2(const T& x, const T& y);)
	v = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
RESULT

CHECK(TVector2(const TVector2& vector);)
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
CHECK(virtual void persistentWrite(PersistenceManager& pm, const char* name = 0) const;)
	Vector2 v(1.0, 2.0);
	NEW_TMP_FILE(filename)
	ofstream  ofile(filename.c_str(), File::OUT);
	pm.setOstream(ofile);
	pm.registerClass(getStreamName<Vector2>(), Vector2::createDefault);
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
		TEST_EQUAL(isKindOf<Vector2>(*ptr), true)
		if (isKindOf<Vector2>(*ptr))
		{
			Vector2* v_ptr = castTo<Vector2>(*ptr);
			TEST_REAL_EQUAL(v_ptr->x, 1.0)
			TEST_REAL_EQUAL(v_ptr->y, 2.0)
		}
	}
RESULT

CHECK(TVector2<T>::set(const T& value))
	v.set(1.0);
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 1.0)
RESULT

CHECK(TVector2::set(const T& x, const T& y))
	v.set(1.0, 2.0);
	v2 = Vector2(1.0, 2.0);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector2::set(const TVector2& vector);)
	v = Vector2(1.0, 2.0);
	v2.set(v);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector2<T>& TVector2<T>::operator = (const TVector2<T>& v))
	v2 = Vector2(1.0, 2.0);
	v  = Vector2();
	v = v2;
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector2::get(T& rx, T& ry) const)
	float a, b;
	v = Vector2(1.0, 2.0);
	v.get(a, b);
	TEST_REAL_EQUAL(a, 1.0)
	TEST_REAL_EQUAL(b, 2.0)
RESULT

CHECK(TVector2::get(TVector2<T>& v) const )
	v2 = Vector2(1.0, 2.0);
	v  = Vector2();
	v.get(v2);
	TEST_EQUAL(v2, v)
RESULT

CHECK(TVector2<T>::swap(TVector2<T>& v))
	v  = Vector2(1.0, 2.0);
	Vector2 u2 = v;

	Vector2 u(4.0, 2.0);
	v2 = u;

	v.swap(u);
	TEST_EQUAL(v, v2)
	TEST_EQUAL(u, u2)
RESULT

CHECK(TVector2::getLength() const )
	v = Vector2(4.0, 9.0);
	TEST_REAL_EQUAL(v.getLength(), sqrt(4.0 * 4.0 + 9.0 * 9.0))
	v = Vector2(0.0, 0.0);
	TEST_REAL_EQUAL(v.getLength(), 0.0)
RESULT

CHECK(TVector2::getSquareLength() const )
	v = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 5.0)
	v = Vector2(0.0, 0.0);
	TEST_REAL_EQUAL(v.getSquareLength(), 0.0)
RESULT

CHECK(TVector2::normalize())
	v = Vector2(4.0, 9.0);
	v.normalize();
	float erg = ::sqrt (4.0 * 4.0 + 9.0 * 9.0);
	TEST_REAL_EQUAL(v[0], 4.0 / erg)
	TEST_REAL_EQUAL(v[1], 9.0 / erg)
	v = Vector2(0.0, 0.0);
	TEST_EXCEPTION(Exception::DivisionByZero, v.normalize())
RESULT

CHECK(TVector2::getZero())
	TEST_REAL_EQUAL(Vector2::getZero().x, 0.0)
	TEST_REAL_EQUAL(Vector2::getZero().y, 0.0)
RESULT

CHECK(TVector2::getUnit())
	TEST_REAL_EQUAL(Vector2::getUnit().x, 1.0)
	TEST_REAL_EQUAL(Vector2::getUnit().y, 1.0)
RESULT

CHECK(TVector2::T& operator [] (Index index) )
	v = Vector2(1.0, 2.0);
	v[0]=5.0;	v[1]=6.0;	
	TEST_REAL_EQUAL(v[0], 5.0)
	TEST_REAL_EQUAL(v[1], 6.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[32472] = 5.0)
	TEST_EXCEPTION(Exception::IndexOverflow, v[2] = 5.0)
RESULT

CHECK(TVector2::TVector2 operator + () const )
	v2 = Vector2(1.0, 2.0);
	v = + v2;
	TEST_REAL_EQUAL(v[0], 1.0)
	TEST_REAL_EQUAL(v[1], 2.0)
RESULT

CHECK(TVector2::TVector2 operator - () const )
	v2 = Vector2(1.0, 2.0);
	v = - v2;
	TEST_REAL_EQUAL(v[0], -1.0)
	TEST_REAL_EQUAL(v[1], -2.0)
RESULT

CHECK(TVector2::TVector2& operator += (const TVector2& vector))
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.0);
	v += v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2::TVector2& operator -= (const TVector2& vector))
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(4.0, 3.0);
	v -= v2;
	TEST_REAL_EQUAL(v[0], 3.0)
	TEST_REAL_EQUAL(v[1], 1.0)
RESULT

CHECK(TVector2::TVector2 operator * (const T& scalar))
	v  = Vector2(1.0, 2.0);
	v = v * 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2::TVector2& operator *= (const T& scalar))
	v  = Vector2(1.0, 2.0);
	v *= 2.0;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2::TVector2 operator / (const T& scalar))
	v  = Vector2(1.0, 2.0);
	v = v / 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v = v / 0)
RESULT

CHECK(TVector2::TVector2& operator /= (const T& scalar))
	v  = Vector2(1.0, 2.0);
	v /= 0.5;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
	TEST_EXCEPTION(Exception::DivisionByZero, v /= 0)
RESULT

CHECK(TVector2::T operator * (const TVector2& vector) const )
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.0);
	TEST_REAL_EQUAL(v * v2 , 5.0)
RESULT

CHECK(TVector2 operator % (const TVector2& vector) const)
 	v1 = Vector2(1.0, 2.0);
	v2 = Vector2(4.0, 5.0);
	v  = v1 % v2;
	TEST_REAL_EQUAL(v[0], -3.0)
	TEST_REAL_EQUAL(v[1], 6.0)
RESULT

CHECK(TVector2 operator %= (const TVector2& vector))
 	v  = Vector2(1.0, 2.0);
	v2 = Vector2(4.0, 5.0);
	v  %= v2;
	TEST_REAL_EQUAL(v[0], -3.0)
	TEST_REAL_EQUAL(v[1], 6.0)
RESULT

CHECK(TVector2::getDistance(const TVector2& vector) const )
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(0, 1.0);
	TEST_REAL_EQUAL(v.getDistance(v2) , sqrt(2.0))
RESULT

CHECK(TVector2::getSquareDistance(const TVector2& vector) const )
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(0, 1.0);
	TEST_REAL_EQUAL(v.getSquareDistance(v2) , 2.0)
RESULT

CHECK(TVector2::bool operator == (const TVector2& vector) const )
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.1);
	TEST_EQUAL(v == v2 , false)
	v  = Vector2(1.0, 2.0);
	TEST_EQUAL(v == v2 , true)
RESULT

CHECK(TVector2::bool operator != (const TVector2& vector) const )
	v2 = Vector2(1.0, 2.0);
	v  = Vector2(1.0, 2.1);
	TEST_EQUAL(v != v2 , true)
	v = Vector2(1.0, 2.0);
	TEST_EQUAL(v != v2 , false)
RESULT

CHECK(TVector2::isZero() const )
	v = Vector2(1.0, 0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector2(0, 1.0);
	TEST_EQUAL(v.isZero() , false)
	v = Vector2(0, 0);
	TEST_EQUAL(v.isZero() , true)
RESULT

CHECK(TVector2::isOrthogonalTo(TVector2& vector) const )
	v2 = Vector2(1.0, 0);
	v  = Vector2(0, 2.0);
	TEST_EQUAL(v.isOrthogonalTo(v2), true)
	v[0]=1;
	TEST_EQUAL(v.isOrthogonalTo(v2), false)
RESULT

CHECK(TVector2::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Vector2 v(1.2, 2.3);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	v.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Vector2_test.txt", true)
RESULT

CHECK(TVector2::isValid() const )
	v  = Vector2(0, 2.0);
	TEST_EQUAL(v.isValid(), true)
RESULT

CHECK(TVector2 operator + (const TVector2<T>& a, const TVector2<T>& b))
 	v1 = Vector2(1.0, 2.0);
	v2 = Vector2(1.0, 2.0);
	v  = v1 + v2;
	TEST_REAL_EQUAL(v[0], 2.0)
	TEST_REAL_EQUAL(v[1], 4.0)
RESULT

CHECK(TVector2 operator - (const TVector2<T>& a, const TVector2<T>& b))
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
	std::ifstream instr("data/Vector2_test2.txt");
	Vector2 v(1, 2);
	instr >> v;
	instr.close();
	TEST_REAL_EQUAL(v.x, 1.2)
	TEST_REAL_EQUAL(v.y, 2.3)
RESULT

NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TVector2<T>& vector))
	Vector2 v(1.2, 2.3);
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Vector2_test2.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
