// $Id: Box3_test.C,v 1.7 2000/04/30 16:46:40 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#	include <BALL/MATHS/box3.h>
#	include <BALL/MATHS/vector3.h>
///////////////////////////

START_TEST(class_name, "$Id: Box3_test.C,v 1.7 2000/04/30 16:46:40 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


//line 33: method TBox3::BALL_CREATE(TBox3<T>)
CHECK(TVector3::BALL_CREATE(TBox3<T>))
	Vector3 v1(1.0, 2.0, 3.0), v2(1.0, 2.0, 3.0);
	Vector3 v0;
	Box3 v(v1, v2);
	Box3* v_ptr = (Box3*)v.create(false, true);
	TEST_EQUAL(v_ptr->a == v0, true)
	TEST_EQUAL(v_ptr->b == v0, true)
	delete v_ptr;
	v_ptr = (Box3*)v.create();
	TEST_EQUAL(v_ptr->a == v1, true)
	TEST_EQUAL(v_ptr->b == v2, true)
	delete v_ptr;
RESULT

//line 39
CHECK(TBox3::TBox3())
  Box3* box;
	box = new Box3();
	TEST_NOT_EQUAL(box, 0)
RESULT

Box3 box, box2;
Vector3 v1, v2, v3, v4;
float a = 1.0, b = 2.0, c = 3.0,
			d = 6.0, e = 5.0, f = 4.0;
float a1, b1, c1, d1, e1, f1;

v1 = Vector3(1.0, 2.0, 3.0);
v2 = Vector3(6.0, 5.0, 4.0);

String filename;
using std::ofstream;
using std::ios;

//line 246: method TBox3::TBox3(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz)
//also test for TBox3::get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const 
CHECK(TBox3::TBox3(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz))
	box = Box3(a, b, c, d, e, f);
	float a1, b1, c1, d1, e1, f1;
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT

//line 230: method TBox3::TBox3(const TBox3<T>& box, bool /* deep */)
CHECK(TBox3::TBox3(const TBox3<T>& box, bool /* deep */))
	box = Box3(v1, v2);
	box2 = Box3(box);
	float a1, b1, c1, d1, e1, f1;
	box2.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT

//line 237: method TBox3::TBox3(const TVector3<T>& a, const TVector3<T>& b)
CHECK(TBox3::TBox3(const TVector3<T>& a, const TVector3<T>& b))
	box = Box3(v1, v2);
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(a, a1)
	TEST_REAL_EQUAL(b, b1)
	TEST_REAL_EQUAL(c, c1)
	TEST_REAL_EQUAL(d, d1)
	TEST_REAL_EQUAL(e, e1)
	TEST_REAL_EQUAL(f, f1)
RESULT


//line 147: method TBox3::getSurface() const 
CHECK(TBox3::getSurface() const )
	box = Box3(v1, v2);
	TEST_REAL_EQUAL(box.getSurface(), 46.0)
RESULT


//line 152: method TBox3::getVolume() const 
CHECK(TBox3::getVolume() const )
	box = Box3(v1, v2);
	TEST_REAL_EQUAL(box.getVolume(), 15.0)
RESULT


//line 157: method TBox3::getWidth() const 
CHECK(TBox3::getWidth() const )
	box = Box3(v1, v2);
	TEST_REAL_EQUAL(box.getWidth(), 5.0)
RESULT


//line 162: method TBox3::getHeight() const 
CHECK(TBox3::getHeight() const )
	box = Box3(v1, v2);
	TEST_REAL_EQUAL(box.getHeight(), 3.0)
RESULT


//line 167: method TBox3::getDepth() const 
CHECK(TBox3::getDepth() const )
	box = Box3(v1, v2);
	TEST_REAL_EQUAL(box.getDepth(), 1.0)
RESULT


//line 174: method TBox3::join(const TBox3& box)
CHECK(TBox3::join(const TBox3& box))
	box = Box3(v1, v2);
	v3 = Vector3(101.0, 102.0, 103.0);
	v4 = Vector3(104.0, 105.0, 106.0);
	box2 = Box3(v3, v4);
	box.join(box2);
	box.get(a1, b1, c1, d1, e1, f1);
	TEST_REAL_EQUAL(1, a1)
	TEST_REAL_EQUAL(2, b1)
	TEST_REAL_EQUAL(3, c1)
	TEST_REAL_EQUAL(104, d1)
	TEST_REAL_EQUAL(105, e1)
	TEST_REAL_EQUAL(106, f1)
RESULT


//line 184: method TBox3::bool operator == (const TBox3& box) const 
CHECK(TBox3::bool operator == (const TBox3& box) const )
	box = Box3(v1, v2);
	box2 = Box3(v3, v4);
	TEST_EQUAL(box == box2, false)
	box2 = Box3(v1, v2);
	TEST_EQUAL(box == box2, true)
RESULT


//line 189: method TBox3::bool operator != (const TBox3& box) const 
CHECK(TBox3::bool operator != (const TBox3& box) const )
	box = Box3(v1, v2);
	box2 = Box3(v3, v4);
	TEST_EQUAL(box != box2, true)
	box2 = Box3(v1, v2);
	TEST_EQUAL(box != box2, false)
RESULT


//line 200: method TBox3::isValid() const 
CHECK(TBox3::isValid() const )
	box = Box3(v1, v2);
	TEST_EQUAL(box.isValid(), true)
RESULT


//line 203: method TBox3::dump(std::ostream& s = std::cout, Size depth = 0) const 
CHECK(TBox3::dump(std::ostream& s = std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT


//line 254: method TBox3::set(const TBox3<T>& box, bool /* deep */)
CHECK(TBox3::set(const TBox3<T>& box, bool /* deep */))
	box = Box3(v1, v2);
	box2.set(box);
	TEST_EQUAL(box == box2, true)
RESULT


//line 272: method TBox3::set(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz)
CHECK(TBox3::set(const T& ax, const T& ay, const T& az, const T& bx, const T& by, const T& bz))
	box = Box3(v1, v2);
	box2.set(1.0, 2.0, 3.0, 6.0, 5.0, 4.0);
	TEST_EQUAL(box == box2, true)
RESULT


//line 289: method TBox3::get(TBox3<T>& box, bool deep) const 
CHECK(TBox3::get(TBox3<T>& box, bool deep) const )
	box = Box3(v1, v2);
	box.get(box2);
	TEST_EQUAL(box == box2, true)
RESULT


//line 289: method TBox3::get(TVector3&, TVector3&) const
CHECK(TBox3::get(TVector3& lower, TVector3& upper) const)
	Box3 box;
	box2.get(box);
	TEST_EQUAL(box == box2, true)
RESULT


//line 304: method TBox3::get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const 
CHECK(TBox3::get(T& ax, T& ay, T& az, T& bx, T& by, T& bz) const )
	box = Box3(v1, v2);
	box.get(a1, b1, c1, d1, e1, f1);
	box2.set(a1, b1, c1, d1, e1, f1);
	TEST_EQUAL(box == box2, true)
RESULT


//line 424: method TBox3<T>::dump(std::ostream& s, Size depth) const
CHECK(TVector3::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Box3 v(1.0, 2.0, 3.0, 4.0, 5.0, 6.0);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	v.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Box3_test.txt", true)
RESULT

//line 
CHECK(std::istream& operator >> (std::istream& s, TBox3<T>& Box3))
	std::ifstream instr("data/Box3_test2.txt");
	Box3 b(10, 20, 30, 40, 50, 60);
	instr >> b;
	instr.close();
	Box3 b1(1, 2, 3, 4, 5, 6);
	TEST_EQUAL(b, b1)
RESULT


//line 
NEW_TMP_FILE(filename)
CHECK(std::ostream& operator << (std::ostream& s, const TBox3<T>& Box3))
	Box3 v(1, 2, 3, 4, 5, 6);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << v;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Box3_test2.txt", false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
