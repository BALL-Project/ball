// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RegularData3D_test.C,v 1.11 2003/05/04 20:15:49 oliver Exp $

#include <BALL/CONCEPT/classTest.h>
#include <BALL/DATATYPE/regularData3D.h>

START_TEST(RegularData3D, "$Id: RegularData3D_test.C,v 1.11 2003/05/04 20:15:49 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
RegularData3D*	grid;

CHECK(RegularData3D<T>())
	grid = new RegularData3D();
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(~RegularData3D<T>())
	delete grid;
RESULT

CHECK(RegularData3D<T>(const Indextype& size, const CoordinateType& origin, const CoordinateType& dimension))
	grid = new RegularData3D(RegularData3D::IndexType(11, 11, 11), Vector3(0.0, 0.0, 0.0), Vector3(10.0, 10.0, 10.0));
	TEST_NOT_EQUAL(grid, 0)
	TEST_EQUAL(grid->size(), 1331)
	delete grid;
RESULT

using BALL::Vector3;
Vector3	lower(0.0, 0.0, 0.0);
Vector3	upper(10.0, 10.0, 10.0);

CHECK(RegularData3D<T>(const Vector3& lower, const Vector3& upper, float spacing))
	grid = new RegularData3D(lower, upper - lower, Vector3(1.0));
	TEST_NOT_EQUAL(grid, 0)
	TEST_EQUAL(grid->size(), 1331)
	delete grid;
RESULT

CHECK(RegularData3D<T>(const Vector3& lower, const Vector3& upper, Size, Size, Size))
	grid = new RegularData3D(RegularData3D::IndexType(11, 11, 11), lower, upper - lower);
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(size())
	TEST_EQUAL(grid->size(), 1331)
RESULT

RegularData3D g(RegularData3D::IndexType(11, 11, 11), lower, upper - lower);

CHECK(resize())
	Vector3 origin(2.0);
	Vector3 dimension(10.0);

	RegularData3D g1;
	g1.resize(RegularData3D::IndexType(2, 2, 2));
	g1.setDimension(dimension);
	g1.setOrigin(origin);
	RegularData3D::IndexType new_size(3, 4, 5);

	g1.resize(new_size);

	TEST_EQUAL(g1.size(), 60)
	TEST_EQUAL(g1.getOrigin(), origin)
	TEST_EQUAL(g1.getDimension(), Vector3(dimension.x * 3. / 2, dimension.y * 4. / 2., dimension.y * 5. / 2.))
RESULT

CHECK(operator = (const RegularData3D<T>& grid))
	RegularData3D g1;
	g1 = g;
	TEST_EQUAL(g1.size(), 1331)
	TEST_EQUAL((g1 == g), true)
RESULT

CHECK(getSize())
	TEST_EQUAL(grid->getSize().x, 11)
	TEST_EQUAL(grid->getSize().y, 11)
	TEST_EQUAL(grid->getSize().z, 11)
RESULT

CHECK(getSpacing())
	TEST_REAL_EQUAL(grid->getSpacing().x, 1.0)
	TEST_REAL_EQUAL(grid->getSpacing().y, 1.0)
	TEST_REAL_EQUAL(grid->getSpacing().z, 1.0)
RESULT

RegularData3D::IndexType	index;

CHECK(getIndex(const Vector3& vector))
	lower.set(3.49, 3.51, 3.0);
	index = grid->getClosestIndex(lower);
	TEST_EQUAL(index.x, 3)
	TEST_EQUAL(index.y, 4)
	TEST_EQUAL(index.z, 3)
RESULT

CHECK(getData(Position))
	grid->getData(RegularData3D::IndexType(0, 0, 0)) = 5.4321;		
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(grid->getData(0), 5.4321);
	TEST_REAL_EQUAL(grid->getData(0), grid->getData(grid->getClosestIndex(lower)));
RESULT

CHECK(operator[]/1/2)
	(*grid)[3 + 11 * 3 + 11 * 11 * 3] = 1.2345;
	lower.set(3.0, 3.0, 3.0);
	TEST_EQUAL((*grid)[3 + 11 * 3 + 11 * 11 * 3], (*grid)[grid->getClosestIndex(lower)]);
RESULT

CHECK(getCoordinates/1)
	lower = grid->getCoordinates(0);
	TEST_REAL_EQUAL(lower.x, 0.0)
	TEST_REAL_EQUAL(lower.y, 0.0)
	TEST_REAL_EQUAL(lower.z, 0.0)
RESULT

CHECK(getCoordinates/2)
	lower = grid->getCoordinates(2 + 2 * 11 + 2 * 11 * 11);
	TEST_REAL_EQUAL(lower.x, 2.0)
	TEST_REAL_EQUAL(lower.y, 2.0)
	TEST_REAL_EQUAL(lower.z, 2.0)
RESULT

CHECK(getCoordinates/3)
	upper.set(3.999999, 4.0, 3.0001);
	lower = grid->getCoordinates(grid->getClosestIndex(upper));
	TEST_REAL_EQUAL(lower.x, 4.0)
	TEST_REAL_EQUAL(lower.y, 4.0)
	TEST_REAL_EQUAL(lower.z, 3.0)
RESULT

CHECK(getBoxIndices)
	lower.set(2, 2, 2);
	Position p1, p2, p3, p4, p5, p6, p7, p8;
	g.getEnclosingIndices(lower, p1, p2, p3, p4, p5, p6, p7, p8);
	TEST_EQUAL(p1, 266);
	TEST_EQUAL(p2, 267);
	TEST_EQUAL(p3, 277);
	TEST_EQUAL(p4, 278);
	TEST_EQUAL(p5, 387);
	TEST_EQUAL(p6, 388);
	TEST_EQUAL(p7, 398);
	TEST_EQUAL(p8, 399);

	lower.set(2.1, 2.1, 2.1);
	g.getEnclosingIndices(lower, p1, p2, p3, p4, p5, p6, p7, p8);
	TEST_EQUAL(p1, 266);
	TEST_EQUAL(p2, 267);
	TEST_EQUAL(p3, 277);
	TEST_EQUAL(p4, 278);
	TEST_EQUAL(p5, 387);
	TEST_EQUAL(p6, 388);
	TEST_EQUAL(p7, 398);
	TEST_EQUAL(p8, 399);

	lower.set(10.1, 2.1, 2.1);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingIndices(lower, p1, p2, p3, p4, p5, p6, p7, p8))
RESULT

CHECK(getBoxData)
	lower.set(2, 2, 2);
	float p1, p2, p3, p4, p5, p6, p7, p8;
	g[266] = 1;
	g[267] = 2;
	g[277] = 3;
	g[278] = 4;
	g[387] = 5;
	g[388] = 6;
	g[398] = 7;
	g[399] = 8;
	g.getEnclosingValues(lower, p1, p2, p3, p4, p5, p6, p7, p8);
	TEST_EQUAL(p1, 1);
	TEST_EQUAL(p2, 2);
	TEST_EQUAL(p3, 3);
	TEST_EQUAL(p4, 4);
	TEST_EQUAL(p5, 5);
	TEST_EQUAL(p6, 6);
	TEST_EQUAL(p7, 7);
	TEST_EQUAL(p8, 8);

	lower.set(10.1, 2.1, 2.1);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingValues(lower, p1, p2, p3, p4, p5, p6, p7, p8))
RESULT

CHECK(getOrigin)
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 0.0)
	TEST_REAL_EQUAL(lower.y, 0.0)
	TEST_REAL_EQUAL(lower.z, 0.0)
RESULT

CHECK(setOrigin/1)
	grid->setOrigin(Vector3(1.0, 1.0, 1.0));
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 1.0)
	TEST_REAL_EQUAL(lower.y, 1.0)
	TEST_REAL_EQUAL(lower.z, 1.0)
	index = grid->getClosestIndex(Vector3(3.49, 3.51, 3.0));
	TEST_EQUAL(index.x, 2)
	TEST_EQUAL(index.y, 3)
	TEST_EQUAL(index.z, 2)
RESULT

CHECK(setOrigin/2)
	lower.set(2.0, 2.0, 2.0);
	grid->setOrigin(lower);
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 2.0)
	TEST_REAL_EQUAL(lower.y, 2.0)
	TEST_REAL_EQUAL(lower.z, 2.0)
	index = grid->getClosestIndex(Vector3(3.49, 3.51, 3.0));
	TEST_EQUAL(index.x, 1)
	TEST_EQUAL(index.y, 2)
	TEST_EQUAL(index.z, 1)
RESULT

CHECK(getDimension)
	TEST_REAL_EQUAL(grid->getDimension().x, 10.0)
	TEST_REAL_EQUAL(grid->getDimension().y, 10.0)
	TEST_REAL_EQUAL(grid->getDimension().z, 10.0)
RESULT

CHECK(getInterpolatedValue)
	lower.set(2, 2, 2);
	TEST_EQUAL(g.getInterpolatedValue(lower), 1)
	lower.set(0, 0, 0);
	TEST_EQUAL(g.getInterpolatedValue(lower), 0)
	lower.set(3, 3, 3);
	TEST_EQUAL(g.getInterpolatedValue(lower), 8)
	lower.set(10.1, 0, 0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getInterpolatedValue(lower))
RESULT

RegularData3D grid2 = *grid;

CHECK(clear())
	grid2.clear();
	TEST_EQUAL(grid2.getSize().x, 0)
	TEST_EQUAL(grid2.getSize().y, 0)
	TEST_EQUAL(grid2.getSize().z, 0)
RESULT

CHECK(operator ==)
	grid2 = *grid;
	TEST_EQUAL(*grid == grid2, true)
	grid2[5] = -99.9;
	TEST_EQUAL(*grid == grid2, false)
RESULT

CHECK(operator !=)
	TEST_EQUAL(*grid != grid2, true)
	grid2[5] = (*grid)[5];
	TEST_EQUAL(*grid != grid2, false)
RESULT

CHECK(isInside()1/1)
	RegularData3D g(RegularData3D::IndexType(11, 11, 11), Vector3(0.0, 0.0, 0.0), Vector3(10.0, 10.0, 10.0));
	TEST_EQUAL(g.isInside(Vector3(0.0, 0.0, 0.0)), true)
	Vector3 v(0.0, 0.0, 0.0);
	TEST_EQUAL(g.isInside(v), true)

	TEST_EQUAL(g.isInside(Vector3(10.0, 10.0, 10.0)), true)
	v = Vector3(10.0, 10.0, 10.0);
	TEST_EQUAL(g.isInside(v), true)

	TEST_EQUAL(g.isInside(Vector3(10.1, 10.0, 10.0)), false)
	v = Vector3(10.0, 10.0, 10.1);
	TEST_EQUAL(g.isInside(v), false)

	TEST_EQUAL(g.isInside(Vector3(0.0, 0.0, -0.1)), false)
	v = Vector3(0.0, 0.0, -0.1);
	TEST_EQUAL(g.isInside(v), false)

	RegularData3D h;
	TEST_EQUAL(h.isInside(Vector3(0.0, 0.0, 0.0)), true)
	v = Vector3(0.0, 0.0, 0.0);
	TEST_EQUAL(h.isInside(v), true)
RESULT

CHECK(operator << (ostream& os, const RegularData3D&))
	String filename;
	NEW_TMP_FILE(filename)
	
	STATUS(1)
	Vector3 lower(-1.0, -2.0, -3.0);
	Vector3 upper(3.0, 2.0, 1.0);
	TRegularData3D<float>	data(lower, upper - lower, Vector3(0.5));
	
	STATUS(2)
	// fill the grid with something meaningful
	for (Position i = 0; i < data.size(); 
			 data[i] = (float)((float)i / data.size()), i++);
	
	STATUS(3)
	std::ofstream os(filename.c_str(), std::ios::out);
	os << data;
	os.close();

	STATUS(4)
	std::ifstream is(filename.c_str());
	STATUS(5)
	TRegularData3D<float> in_data;
	STATUS(5.1)
	is >> in_data;
	STATUS(5.2)
	is.close();
	STATUS(5.3)

	TEST_EQUAL(in_data.size(), data.size())
	ABORT_IF(in_data.size() != data.size())
	
	STATUS(6)

	TEST_REAL_EQUAL(data.getSpacing().x, in_data.getSpacing().x)
	TEST_REAL_EQUAL(data.getSpacing().y, in_data.getSpacing().y)
	TEST_REAL_EQUAL(data.getSpacing().z, in_data.getSpacing().z)

	TEST_REAL_EQUAL(data.getOrigin().x, in_data.getOrigin().x)
	TEST_REAL_EQUAL(data.getOrigin().y, in_data.getOrigin().y)
	TEST_REAL_EQUAL(data.getOrigin().z, in_data.getOrigin().z)

	TEST_REAL_EQUAL(data.getDimension().x, in_data.getDimension().x)
	TEST_REAL_EQUAL(data.getDimension().y, in_data.getDimension().y)
	TEST_REAL_EQUAL(data.getDimension().z, in_data.getDimension().z)

	TEST_EQUAL(data.getSize().x, in_data.getSize().x)
	TEST_EQUAL(data.getSize().y, in_data.getSize().y)
	TEST_EQUAL(data.getSize().z, in_data.getSize().z)

	STATUS(7)
	for (Position i = 0; i < data.size(); i++)
	{
		//STATUS(i)
		//TEST_REAL_EQUAL(data[i], in_data[i])
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
