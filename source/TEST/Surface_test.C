// $Id: Surface_test.C,v 1.3 2002/01/05 02:57:44 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/surface.h>

///////////////////////////

START_TEST(Surface, "$Id: Surface_test.C,v 1.3 2002/01/05 02:57:44 oliver Exp $")

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
	Surface s;
	TEST_EQUAL(s.getNumberOfNormals(), 0)
	Surface::Vertex v1;
	s.pushBackVertex(v1);
	TEST_EQUAL(s.getNumberOfVertices(), 1)
	Surface::Vertex v2;
	s.pushBackVertex(v2);
	TEST_EQUAL(s.getNumberOfVertices(), 2)
RESULT

CHECK(void pushBackNormal(const Vector3& n) throw())
	Surface s;
	TEST_EQUAL(s.getNumberOfNormals(), 0)
	Surface::Normal v1;
	s.pushBackNormal(v1);
	TEST_EQUAL(s.getNumberOfNormals(), 1)
	Surface::Normal v2;
	s.pushBackNormal(v2);
	TEST_EQUAL(s.getNumberOfNormals(), 2)
RESULT

CHECK(Vector3& getVertex(Position index) throw())
	Surface s;
	Surface::Vertex v1(1.0, 2.0, 3.0);
	s.pushBackVertex(v1);
	Surface::Vertex v2(4.0, 5.0, 6.0);
	s.pushBackVertex(v2);
	TEST_EQUAL(s.getNumberOfVertices(), 2)
	TEST_EQUAL(s.getVertex(0), v1)
	TEST_EQUAL(s.getVertex(1), v2)
RESULT

CHECK(const Vector3& getVertex(Position index) const throw())
	Surface s;
	Surface::Vertex v1(1.0, 2.0, 3.0);
	s.pushBackVertex(v1);
	Surface::Vertex v2(4.0, 5.0, 6.0);
	s.pushBackVertex(v2);
	const Surface& c_s(s);
	TEST_EQUAL(c_s.getNumberOfVertices(), 2)
	TEST_EQUAL(c_s.getVertex(0), v1)
	TEST_EQUAL(c_s.getVertex(1), v2)
RESULT

CHECK(Vector3& getNormal(Position index) throw())
	Surface s;
	Surface::Normal v1(1.0, 2.0, 3.0);
	s.pushBackNormal(v1);
	Surface::Normal v2(4.0, 5.0, 6.0);
	s.pushBackNormal(v2);
	TEST_EQUAL(s.getNumberOfNormals(), 2)
	TEST_EQUAL(s.getNormal(0), v1)
	TEST_EQUAL(s.getNormal(1), v2)
RESULT

CHECK(const Vector3& getNormal(Position index) const throw())
	Surface s;
	Surface::Normal v1(1.0, 2.0, 3.0);
	s.pushBackNormal(v1);
	Surface::Normal v2(4.0, 5.0, 6.0);
	s.pushBackNormal(v2);
	const Surface& c_s(s);
	TEST_EQUAL(c_s.getNumberOfNormals(), 2)
	TEST_EQUAL(c_s.getNormal(0), v1)
	TEST_EQUAL(c_s.getNormal(1), v2)
RESULT

CHECK(void clearTriangles())
	Surface s;
	Surface::Triangle t1;
	s.pushBackTriangle(t1);
	Surface::Triangle t2;
	s.pushBackTriangle(t2);
	TEST_EQUAL(s.getNumberOfTriangles(), 2)
	s.clearTriangles();
	TEST_EQUAL(s.getNumberOfTriangles(), 0)
	s.clearTriangles();
	TEST_EQUAL(s.getNumberOfTriangles(), 0)
RESULT

CHECK(void clearVertices())
	Surface s;
	Surface::Vertex v1;
	s.pushBackVertex(v1);
	Surface::Vertex v2;
	s.pushBackVertex(v2);
	TEST_EQUAL(s.getNumberOfVertices(), 2)
	s.clearVertices();
	TEST_EQUAL(s.getNumberOfVertices(), 0)
	s.clearVertices();
	TEST_EQUAL(s.getNumberOfVertices(), 0)
RESULT

CHECK(void clearNormals())
	Surface s;
	Surface::Normal v1;
	s.pushBackNormal(v1);
	Surface::Normal v2;
	s.pushBackNormal(v2);
	TEST_EQUAL(s.getNumberOfNormals(), 2)
	s.clearNormals();
	TEST_EQUAL(s.getNumberOfNormals(), 0)
	s.clearNormals();
	TEST_EQUAL(s.getNumberOfNormals(), 0)
RESULT

CHECK(void resizeTriangles(Size size))
	Surface s;
	s.resizeTriangles(10);
	TEST_EQUAL(s.getNumberOfTriangles(), 10)
	Surface::Triangle t1;
	t1.v1 = 1;
	t1.v2 = 2;
	t1.v3 = 3;
	Surface::Triangle t2;
	t1.v1 = 4;
	t1.v2 = 5;
	t1.v3 = 6;
	Surface::Triangle t3;
	t1.v1 = 7;
	t1.v2 = 8;
	t1.v3 = 9;
	s.getTriangle(0) = t1;
	s.getTriangle(1) = t2;
	s.getTriangle(2) = t3;
	s.resizeTriangles(2);
	TEST_EQUAL(s.getNumberOfTriangles(), 2)
	TEST_EQUAL(s.getTriangle(0).v1, t1.v1)
	TEST_EQUAL(s.getTriangle(0).v2, t1.v2)
	TEST_EQUAL(s.getTriangle(0).v3, t1.v3)
	TEST_EQUAL(s.getTriangle(1).v1, t2.v1)
	TEST_EQUAL(s.getTriangle(1).v2, t2.v2)
	TEST_EQUAL(s.getTriangle(1).v3, t2.v3)
RESULT

CHECK(void resizeVertices(Size size))
	Surface s;
	s.resizeVertices(10);
	TEST_EQUAL(s.getNumberOfVertices(), 10)
	Surface::Vertex v1(1.0, 2.0, 3.0);
	Surface::Vertex v2(4.0, 5.0, 6.0);
	Surface::Vertex v3(7.0, 8.0, 9.0);
	s.getVertex(0) = v1;
	s.getVertex(1) = v2;
	s.getVertex(2) = v3;
	s.resizeVertices(2);
	TEST_EQUAL(s.getNumberOfVertices(), 2)
	TEST_EQUAL(s.getVertex(0), v1)
	TEST_EQUAL(s.getVertex(1), v2)
RESULT

CHECK(void resizeNormals(Size size))
	Surface s;
	s.resizeNormals(10);
	TEST_EQUAL(s.getNumberOfNormals(), 10)
	Surface::Normal v1(1.0, 2.0, 3.0);
	Surface::Normal v2(4.0, 5.0, 6.0);
	Surface::Normal v3(7.0, 8.0, 9.0);
	s.getNormal(0) = v1;
	s.getNormal(1) = v2;
	s.getNormal(2) = v3;
	s.resizeNormals(2);
	TEST_EQUAL(s.getNumberOfNormals(), 2)
	TEST_EQUAL(s.getNormal(0), v1)
	TEST_EQUAL(s.getNormal(1), v2)
RESULT

Surface s;
s.pushBackVertex(Surface::Vertex(0.0, 0.0, 0.0));
s.pushBackVertex(Surface::Vertex(1.0, 0.0, 0.0));
s.pushBackVertex(Surface::Vertex(0.0, 1.0, 0.0));
s.pushBackVertex(Surface::Vertex(0.0, 0.0, 1.0));
s.pushBackNormal(Surface::Normal(0.0, 0.0, 0.0));
s.pushBackNormal(Surface::Normal(1.0, 0.0, 0.0));
s.pushBackNormal(Surface::Normal(0.0, 1.0, 0.0));
s.pushBackNormal(Surface::Normal(0.0, 0.0, 1.0));
Surface::Triangle t;
t.v1 = 0;
t.v2 = 1;
t.v3 = 2;
s.pushBackTriangle(t);
t.v3 = 3;
s.pushBackTriangle(t);
t.v2 = 2;
s.pushBackTriangle(t);
t.v1 = 1;
s.pushBackTriangle(t);

CHECK(Surface(const Surface& surface)	throw())
	Surface s2(s);
	TEST_EQUAL(s2.getNumberOfTriangles(), s.getNumberOfTriangles())
	TEST_EQUAL(s2.getNumberOfVertices(), s.getNumberOfVertices())
	TEST_EQUAL(s2.getNumberOfNormals(), s.getNumberOfNormals())

	ABORT_IF(s2.getNumberOfTriangles() != s.getNumberOfTriangles())
	for (Position i = 0; i < s2.getNumberOfTriangles(); i++)
	{
		TEST_EQUAL(s2.getTriangle(i).v1, s.getTriangle(i).v1)
		TEST_EQUAL(s2.getTriangle(i).v2, s.getTriangle(i).v2)
		TEST_EQUAL(s2.getTriangle(i).v3, s.getTriangle(i).v3)
	}
	ABORT_IF(s2.getNumberOfVertices() != s.getNumberOfVertices())
	for (Position i = 0; i < s2.getNumberOfVertices(); i++)
	{
		TEST_EQUAL(s2.getVertex(i), s.getVertex(i))
	}
	ABORT_IF(s2.getNumberOfNormals() != s.getNumberOfNormals())
	for (Position i = 0; i < s2.getNumberOfNormals(); i++)
	{
		TEST_EQUAL(s2.getNormal(i), s.getNormal(i))
	}
RESULT

CHECK(void set(const Surface& surface) throw())
	Surface s2;
	s2.set(s);

	TEST_EQUAL(s2.getNumberOfTriangles(), s.getNumberOfTriangles())
	TEST_EQUAL(s2.getNumberOfVertices(), s.getNumberOfVertices())
	TEST_EQUAL(s2.getNumberOfNormals(), s.getNumberOfNormals())

	ABORT_IF(s2.getNumberOfTriangles() != s.getNumberOfTriangles())
	for (Position i = 0; i < s2.getNumberOfTriangles(); i++)
	{
		TEST_EQUAL(s2.getTriangle(i).v1, s.getTriangle(i).v1)
		TEST_EQUAL(s2.getTriangle(i).v2, s.getTriangle(i).v2)
		TEST_EQUAL(s2.getTriangle(i).v3, s.getTriangle(i).v3)
	}
	ABORT_IF(s2.getNumberOfVertices() != s.getNumberOfVertices())
	for (Position i = 0; i < s2.getNumberOfVertices(); i++)
	{
		TEST_EQUAL(s2.getVertex(i), s.getVertex(i))
	}
	ABORT_IF(s2.getNumberOfNormals() != s.getNumberOfNormals())
	for (Position i = 0; i < s2.getNumberOfNormals(); i++)
	{
		TEST_EQUAL(s2.getNormal(i), s.getNormal(i))
	}
RESULT

CHECK(const Surface& operator = (const Surface& box) throw())
	Surface s2;
	s2 = s;

	TEST_EQUAL(s2.getNumberOfTriangles(), s.getNumberOfTriangles())
	TEST_EQUAL(s2.getNumberOfVertices(), s.getNumberOfVertices())
	TEST_EQUAL(s2.getNumberOfNormals(), s.getNumberOfNormals())

	ABORT_IF(s2.getNumberOfTriangles() != s.getNumberOfTriangles())
	for (Position i = 0; i < s2.getNumberOfTriangles(); i++)
	{
		TEST_EQUAL(s2.getTriangle(i).v1, s.getTriangle(i).v1)
		TEST_EQUAL(s2.getTriangle(i).v2, s.getTriangle(i).v2)
		TEST_EQUAL(s2.getTriangle(i).v3, s.getTriangle(i).v3)
	}
	ABORT_IF(s2.getNumberOfVertices() != s.getNumberOfVertices())
	for (Position i = 0; i < s2.getNumberOfVertices(); i++)
	{
		TEST_EQUAL(s2.getVertex(i), s.getVertex(i))
	}
	ABORT_IF(s2.getNumberOfNormals() != s.getNumberOfNormals())
	for (Position i = 0; i < s2.getNumberOfNormals(); i++)
	{
		TEST_EQUAL(s2.getNormal(i), s.getNormal(i))
	}
RESULT

CHECK(void get(Surface& surface) const throw())
	Surface s2;
	s.get(s2);

	TEST_EQUAL(s2.getNumberOfTriangles(), s.getNumberOfTriangles())
	TEST_EQUAL(s2.getNumberOfVertices(), s.getNumberOfVertices())
	TEST_EQUAL(s2.getNumberOfNormals(), s.getNumberOfNormals())

	ABORT_IF(s2.getNumberOfTriangles() != s.getNumberOfTriangles())
	for (Position i = 0; i < s2.getNumberOfTriangles(); i++)
	{
		TEST_EQUAL(s2.getTriangle(i).v1, s.getTriangle(i).v1)
		TEST_EQUAL(s2.getTriangle(i).v2, s.getTriangle(i).v2)
		TEST_EQUAL(s2.getTriangle(i).v3, s.getTriangle(i).v3)
	}
	ABORT_IF(s2.getNumberOfVertices() != s.getNumberOfVertices())
	for (Position i = 0; i < s2.getNumberOfVertices(); i++)
	{
		TEST_EQUAL(s2.getVertex(i), s.getVertex(i))
	}
	ABORT_IF(s2.getNumberOfNormals() != s.getNumberOfNormals())
	for (Position i = 0; i < s2.getNumberOfNormals(); i++)
	{
		TEST_EQUAL(s2.getNormal(i), s.getNormal(i))
	}
RESULT

CHECK(void clear() throw())
	Surface s2(s);
	s2.clear();
	TEST_EQUAL(s2.getNumberOfTriangles(), 0)
	TEST_EQUAL(s2.getNumberOfVertices(), 0)
	TEST_EQUAL(s2.getNumberOfNormals(), 0)
RESULT

CHECK(void readMSMSFile(const String& vert_filename, const String& face_filename) throw(Exception::FileNotFound))
	Surface s;
	s.readMSMSFile("data/Surface_test.vert.dat", "data/Surface_test.face.dat");
	TEST_EQUAL(s.getNumberOfTriangles(), 170)
	TEST_EQUAL(s.getNumberOfVertices(), 87)
	TEST_EQUAL(s.getNumberOfNormals(), 87)
	
	ABORT_IF(s.getNumberOfTriangles() != 170)
	ABORT_IF(s.getNumberOfVertices() != 87)
	ABORT_IF(s.getNumberOfNormals() != 87)
	
	TEST_EQUAL(s.getTriangle(0).v1, 0)
	TEST_EQUAL(s.getTriangle(0).v2, 6)
	TEST_EQUAL(s.getTriangle(0).v3, 1)

	TEST_EQUAL(s.getTriangle(169).v1, 86)
	TEST_EQUAL(s.getTriangle(169).v2, 23)
	TEST_EQUAL(s.getTriangle(169).v3, 84)

	TEST_REAL_EQUAL(s.getVertex(86).x, 3.000)
	TEST_REAL_EQUAL(s.getVertex(86).y, 3.149)
	TEST_REAL_EQUAL(s.getVertex(86).z, -0.964)
	
	TEST_REAL_EQUAL(s.getNormal(86).x, 0.000)
	TEST_REAL_EQUAL(s.getNormal(86).y, 0.766)
	TEST_REAL_EQUAL(s.getNormal(86).z, -0.643)
	
	TEST_REAL_EQUAL(s.getVertex(0).x, 0.500)
	TEST_REAL_EQUAL(s.getVertex(0).y, 1.571)
	TEST_REAL_EQUAL(s.getVertex(0).z, 1.132)
	
	TEST_REAL_EQUAL(s.getNormal(0).x, 0.250)
	TEST_REAL_EQUAL(s.getNormal(0).y, 0.786)
	TEST_REAL_EQUAL(s.getNormal(0).z, 0.566)
	
	TEST_EXCEPTION(Exception::FileNotFound, s.readMSMSFile("data/Surface_test.vert.dat", "data/Surface_test.face.dot"))
	TEST_EXCEPTION(Exception::FileNotFound, s.readMSMSFile("data/Surface_test.vert.dot", "data/Surface_test.face.dat"))
	TEST_EXCEPTION(Exception::FileNotFound, s.readMSMSFile("data/Surface_test.vert.dot", "data/Surface_test.face.dot"))
RESULT

CHECK(float getArea() const throw())
	// the area of the tetrahedron: 0.5 for each axis-parallel face
	// and 1/2 sqrt(3) for the fourth side.
	float area = 3.0 * 0.5 + sqrt(3.0) * 0.5;
	TEST_REAL_EQUAL(s.getArea(), area)
RESULT

CHECK(bool operator == (const Surface& surface) const throw())
	Surface s2(s);
	bool result = (s2 == s);
	TEST_EQUAL(result, true)
	result = (s == s2);
	TEST_EQUAL(result, true)
	// check whether differences in the vertex coordinates
	// are recognized
	s2.getVertex(0).x = 17.0;
	result = (s2 == s);
	TEST_EQUAL(result, false)
	result = (s == s2);
	TEST_EQUAL(result, false)
	s2 = s;
	
	// check whether differences in the normal coordinates
	// are recognized
	s2.getNormal(3).z = 17.0;
	result = (s2 == s);
	TEST_EQUAL(result, false)
	result = (s == s2);
	TEST_EQUAL(result, false)
	s2 = s;
	
	// check whether differences in the triangle indices
	// are recognized
	s2.getTriangle(0).v1 = 12;
	result = (s2 == s);
	TEST_EQUAL(result, false)
	result = (s == s2);
	TEST_EQUAL(result, false)
	s2 = s;
	
	// test for equality of empty surfaces
	Surface s3;
	Surface s4;
	result = (s3 == s4);
	TEST_EQUAL(result, true)
	result = (s4 == s3);
	TEST_EQUAL(result, true)
RESULT

CHECK(bool operator != (const Surface& surface) const throw())
	Surface s2(s);
	bool result = (s2 != s);
	TEST_EQUAL(result, false)
	result = (s != s2);
	TEST_EQUAL(result, false)
	// check whether differences in the vertex coordinates
	// are recognized
	s2.getVertex(0).x = 17.0;
	result = (s2 != s);
	TEST_EQUAL(result, true)
	result = (s != s2);
	TEST_EQUAL(result, true)
	s2 = s;
	
	// check whether differences in the normal coordinates
	// are recognized
	s2.getNormal(3).z = 17.0;
	result = (s2 != s);
	TEST_EQUAL(result, true)
	result = (s != s2);
	TEST_EQUAL(result, true)
	s2 = s;
	
	// check whether differences in the triangle indices
	// are recognized
	s2.getTriangle(0).v1 = 12;
	result = (s2 != s);
	TEST_EQUAL(result, true)
	result = (s != s2);
	TEST_EQUAL(result, true)
	s2 = s;
	
	// test for equality of empty surfaces
	Surface s3;
	Surface s4;
	result = (s3 != s4);
	TEST_EQUAL(result, false)
	result = (s4 != s3);
	TEST_EQUAL(result, false)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
