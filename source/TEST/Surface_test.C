// $Id: Surface_test.C,v 1.2 2001/12/28 02:33:13 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/surface.h>

///////////////////////////

START_TEST(Surface, "$Id: Surface_test.C,v 1.2 2001/12/28 02:33:13 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Surface* surf_ptr = 0;
CHECK(Surface()	throw())
	surf_ptr = new Surface;
	TEST_NOT_EQUAL(surf_ptr, 0)
RESULT

CHECK(~Surface() throw())
	delete surf_ptr;
RESULT

CHECK(Size getNumberOfTriangles() const throw())
	Surface s;
	TEST_EQUAL(s.getNumberOfTriangles(), 0)
RESULT

CHECK(Size getNumberOfVertices() const throw())
	Surface s;
	TEST_EQUAL(s.getNumberOfVertices(), 0)
RESULT

CHECK(Size getNumberOfNormals() const throw())
	Surface s;
	TEST_EQUAL(s.getNumberOfNormals(), 0)
RESULT

CHECK(void pushBackTriangle(const Triangle& t) throw())
	Surface s;
	TEST_EQUAL(s.getNumberOfTriangles(), 0)
	Surface::Triangle t1;
	s.pushBackTriangle(t1);
	TEST_EQUAL(s.getNumberOfTriangles(), 1)
	Surface::Triangle t2;
	s.pushBackTriangle(t2);
	TEST_EQUAL(s.getNumberOfTriangles(), 2)
RESULT

CHECK(const Triangle& getTriangle(Position index) const throw())
	Surface s;
	Surface::Triangle t1;
	t1.v1 = 1;
	t1.v2 = 2;
	t1.v3 = 3;
	s.pushBackTriangle(t1);
	TEST_EQUAL(s.getNumberOfTriangles(), 1)
	TEST_EQUAL(s.getTriangle(0).v1, 1)
	TEST_EQUAL(s.getTriangle(0).v2, 2)
	TEST_EQUAL(s.getTriangle(0).v3, 3)
	Surface::Triangle t2;
	t2.v1 = 11;
	t2.v2 = 21;
	t2.v3 = 31;
	s.pushBackTriangle(t2);
	TEST_EQUAL(s.getNumberOfTriangles(), 2)
	TEST_EQUAL(s.getTriangle(0).v1, 1)
	TEST_EQUAL(s.getTriangle(0).v2, 2)
	TEST_EQUAL(s.getTriangle(0).v3, 3)
	TEST_EQUAL(s.getTriangle(1).v1, 11)
	TEST_EQUAL(s.getTriangle(1).v2, 21)
	TEST_EQUAL(s.getTriangle(1).v3, 31)
RESULT

CHECK(Triangle& getTriangle(Position index) throw())
	Surface s;
	Surface::Triangle t1;
	t1.v1 = 1;
	t1.v2 = 2;
	t1.v3 = 3;
	s.pushBackTriangle(t1);
	TEST_EQUAL(s.getNumberOfTriangles(), 1)
	TEST_EQUAL(s.getTriangle(0).v1, 1)
	TEST_EQUAL(s.getTriangle(0).v2, 2)
	TEST_EQUAL(s.getTriangle(0).v3, 3)
	Surface::Triangle t2;
	t2.v1 = 11;
	t2.v2 = 21;
	t2.v3 = 31;
	s.pushBackTriangle(t2);
	TEST_EQUAL(s.getNumberOfTriangles(), 2)
	TEST_EQUAL(s.getTriangle(0).v1, 1)
	TEST_EQUAL(s.getTriangle(0).v2, 2)
	TEST_EQUAL(s.getTriangle(0).v3, 3)
	TEST_EQUAL(s.getTriangle(1).v1, 11)
	TEST_EQUAL(s.getTriangle(1).v2, 21)
	TEST_EQUAL(s.getTriangle(1).v3, 31)
	s.getTriangle(0).v1 = 21;
	s.getTriangle(0).v2 = 22;
	s.getTriangle(0).v3 = 23;
	TEST_EQUAL(s.getNumberOfTriangles(), 2)
	TEST_EQUAL(s.getTriangle(0).v1, 21)
	TEST_EQUAL(s.getTriangle(0).v2, 22)
	TEST_EQUAL(s.getTriangle(0).v3, 23)
	TEST_EQUAL(s.getTriangle(1).v1, 11)
	TEST_EQUAL(s.getTriangle(1).v2, 21)
	TEST_EQUAL(s.getTriangle(1).v3, 31)
RESULT

CHECK(void pushBackVertex(const Vector3& v) throw())
	// ???
RESULT

CHECK(void pushBackNormal(const Vector3& n) throw())
	// ???
RESULT

CHECK(Surface(const Surface& surface)	throw())
	// ???
RESULT

CHECK(void set(const Surface& box) throw())
	// ???
RESULT

CHECK(const Surface& operator = (const Surface& box) throw())
	// ???
RESULT

CHECK(void get(Surface& box) const throw())
	// ???
RESULT

CHECK(void clear() throw())
	// ???
RESULT

CHECK(void readMSMSFile(const String& vert_filename, const String& face_filename) throw(Exception::FileNotFound))
	// ???
RESULT

CHECK(float getArea() const throw())
	// ???
RESULT

CHECK(Vector3& getVertex(Position index) throw())
	// ???
RESULT

CHECK(const Vector3& getVertex(Position index) const throw())
	// ???
RESULT

CHECK(Vector3& getNormal(Position index) throw())
	// ???
RESULT

CHECK(const Vector3& getNormal(Position index) const throw())
	// ???
RESULT

CHECK(void clearTriangles())
	// ???
RESULT

CHECK(void clearVertices())
	// ???
RESULT

CHECK(void clearNormals())
	// ???
RESULT

CHECK(void resizeTriangles(Size size))
	// ???
RESULT

CHECK(void resizeVertices(Size size))
	// ???
RESULT

CHECK(void resizeNormals(Size size))
	// ???
RESULT

CHECK(bool operator == (const Surface& surface) const throw())
	// ???
RESULT

CHECK(bool operator != (const Surface& surface) const throw())
	// ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
