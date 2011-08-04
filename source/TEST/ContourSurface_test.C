// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DATATYPE/contourSurface.h>

///////////////////////////

START_TEST(ContourSurface)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ContourSurface* cs_ptr = 0;
CHECK(TContourSurface() throw())
	cs_ptr = new ContourSurface;
	TEST_NOT_EQUAL(cs_ptr, 0)
RESULT

CHECK(~TContourSurface() throw())
	delete cs_ptr;
RESULT

CHECK(TContourSurface<T>::operator << (TRegularData3D<T>& data))
	// Create a grid with just the middle point at 1.0 and the
	// remainder at -1.0.
	RegularData3D data(RegularData3D::IndexType(5, 5, 5), Vector3(-2.0), Vector3(4.0));
	STATUS("grid size: " << data.getSize().x << "/" << data.getSize().y << "/" << data.getSize().z)
	for (Position i = 0; i < data.size(); i++)
	{
		data[i] = -1.0;
	}
	data[2 + 2 * 5 + 2 * 25] = 1.0;
	STATUS("Coordinate: " << data.getCoordinates(62))
	STATUS("min: " << data.getOrigin())
	STATUS("max: " << data.getOrigin() + data.getDimension())

	STATUS("grid constructed")

	ContourSurface cs;
	STATUS("ContourSurface constructed.")
	
	cs << data;

	TEST_EQUAL(cs.vertex.size(), 6)
	TEST_EQUAL(cs.normal.size(), 6)
	TEST_EQUAL(cs.triangle.size(), 8)

	ABORT_IF(cs.vertex.size() != 6)
	ABORT_IF(cs.normal.size() != 6)
	Vector3 ref_pos;
	for (Position i = 0; i < cs.vertex.size(); i++)
	{
		switch (i)
		{
			case 0: ref_pos = Vector3(0.0, -0.5, 0.0); break;
			case 1: ref_pos = Vector3(-0.5, 0.0, 0.0); break;
			case 2: ref_pos = Vector3(0.0, 0.0, -0.5); break;
			case 3: ref_pos = Vector3(0.5, 0.0, 0.0);  break;
			case 4: ref_pos = Vector3(0.0, 0.5, 0.0);  break;
			case 5: ref_pos = Vector3(0.0, 0.0, 0.5);  break;
		}
		TEST_REAL_EQUAL(cs.vertex[i].x, ref_pos.x)
		TEST_REAL_EQUAL(cs.vertex[i].y, ref_pos.y)
		TEST_REAL_EQUAL(cs.vertex[i].z, ref_pos.z)
		STATUS(i << " " << cs.vertex[i] * -2.0 << " " << cs.normal[i])
		
	}

	ABORT_IF(cs.triangle.size() != 8)
	Index t[8][3] = {{0, 1, 2}, {0, 2, 3}, {4, 2, 1}, {4, 3, 2}, {0, 5, 1}, {0, 3, 5}, {4, 1, 5}, {4, 5, 3}};
	for (Position i = 0; i < cs.triangle.size(); i++)
	{
		TEST_EQUAL(cs.triangle[i].v1, t[i][0])
		TEST_EQUAL(cs.triangle[i].v2, t[i][1])
		TEST_EQUAL(cs.triangle[i].v3, t[i][2])
	}

	// Do it again and make sure we get identical results.
	// This shall test whether everything is cleared correctly
	// prior to constructing the new surface.
	cs << data;

	TEST_EQUAL(cs.vertex.size(), 6)
	TEST_EQUAL(cs.normal.size(), 6)
	TEST_EQUAL(cs.triangle.size(), 8)

	ABORT_IF(cs.vertex.size() != 6)
	ABORT_IF(cs.normal.size() != 6)
	for (Position i = 0; i < cs.vertex.size(); i++)
	{
		switch (i)
		{
			case 0: ref_pos = Vector3(0.0, -0.5, 0.0); break;
			case 1: ref_pos = Vector3(-0.5, 0.0, 0.0); break;
			case 2: ref_pos = Vector3(0.0, 0.0, -0.5); break;
			case 3: ref_pos = Vector3(0.5, 0.0, 0.0);  break;
			case 4: ref_pos = Vector3(0.0, 0.5, 0.0);  break;
			case 5: ref_pos = Vector3(0.0, 0.0, 0.5);  break;
		}
		TEST_REAL_EQUAL(cs.vertex[i].x, ref_pos.x)
		TEST_REAL_EQUAL(cs.vertex[i].y, ref_pos.y)
		TEST_REAL_EQUAL(cs.vertex[i].z, ref_pos.z)
		TEST_REAL_EQUAL(cs.normal[i].x, ref_pos.x * -2.0)
		TEST_REAL_EQUAL(cs.normal[i].y, ref_pos.y * -2.0)
		TEST_REAL_EQUAL(cs.normal[i].z, ref_pos.z * -2.0)
	}

	ABORT_IF(cs.triangle.size() != 8)
	for (Position i = 0; i < cs.triangle.size(); i++)
	{
		TEST_EQUAL(cs.triangle[i].v1, t[i][0])
		TEST_EQUAL(cs.triangle[i].v2, t[i][1])
		TEST_EQUAL(cs.triangle[i].v3, t[i][2])
	}
	
RESULT


CHECK([EXTRA]Exceptions)
	RegularData3D rd;
	std::ifstream is("2ptc2.loc");
	is >> rd;
	is.close();
	STATUS("read grid")
	ContourSurface cs(0.02);
	STATUS("cs ctor")
	cs << rd;
	STATUS("done")
	STATUS(cs.vertex.size())
	STATUS(cs.triangle.size())
RESULT

CHECK(Cube(const TRegularData3D<T>& grid) throw())
  // ???
RESULT

CHECK(Position computeTopology(double threshold) throw())
  // ???
RESULT

CHECK(Position getIndex(Position corner) const)
  // ???
RESULT

CHECK(TContourSurface(T threshold))
  // ???
RESULT

CHECK(TContourSurface(const TContourSurface& surface))
  // ???
RESULT

CHECK((TContourSurface(const TRegularData3D<T>& data, T threshold = 0.0)))
  // ???
RESULT

CHECK(Vector3 getCoordinates(Position index) const)
  // ???
RESULT

CHECK(Vector3 getOrigin() const)
  // ???
RESULT

CHECK(bool operator == (const TContourSurface<T>& surface) const)
  // ???
RESULT

CHECK(const TContourSurface& operator = (const TContourSurface<T>& surface))
  // ???
RESULT

CHECK(const TContourSurface<T> & operator << (const TRegularData3D<T>& data))
  // ???
RESULT

CHECK(const Vector3& getSpacing() const)
  // ???
RESULT

CHECK(void clear())
  // ???
RESULT

CHECK(void setTo(Position p))
  // ???
RESULT

CHECK(void shift() throw())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
