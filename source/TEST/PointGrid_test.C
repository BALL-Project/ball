// $Id: PointGrid_test.C,v 1.4 2000/07/02 01:23:14 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/DATATYPE/pointGrid.h>

START_TEST(PointGrid, "$Id: PointGrid_test.C,v 1.4 2000/07/02 01:23:14 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;

String filename;
using BALL::PointGrid;
PointGrid<float>*	grid;

CHECK(constructor/0)
	grid = new PointGrid<float>();
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(destructor)
	delete grid;
RESULT

CHECK(constructor/1)
	grid = new PointGrid<float>(0.0, 0.0, 0.0,
															10.0, 10.0, 10.0,
															11, 11, 11);
	TEST_NOT_EQUAL(grid, 0)
RESULT

using BALL::Vector3;
Vector3	lower(0.0, 0.0, 0.0);
Vector3	upper(10.0, 10.0, 10.0);

CHECK(constructor/2)
	grid = new PointGrid<float>(lower, upper, 1.0);
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(constructor/3)
	delete grid;
	grid = new PointGrid<float>(lower, upper, 11, 11, 11);
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(getSize)
	TEST_EQUAL(grid->getSize(), 1331)
RESULT

PointGrid<float> g(0.0, 0.0, 0.0, 10.0, 10.0, 10.0,	11, 11, 11);

CHECK(set)
	PointGrid<float> g1;
	g1.set(g);
	TEST_EQUAL(g1.getSize(), 1331)
RESULT

CHECK(operator =)
	PointGrid<float> g1;
	g1 = g;
	TEST_EQUAL(g1.getSize(), 1331)
RESULT

CHECK(dump)
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	g.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/PointGrid_test.txt", true)
RESULT

CHECK(isValid)
	TEST_EQUAL(g.isValid(), true)
RESULT

CHECK(getMaxX)
	TEST_REAL_EQUAL(grid->getMaxX(), 10.0)
RESULT

CHECK(getMaxY)
	TEST_REAL_EQUAL(grid->getMaxY(), 10.0)
RESULT

CHECK(getMaxZ)
	TEST_REAL_EQUAL(grid->getMaxZ(), 10.0)
RESULT

CHECK(getMinX)
	TEST_REAL_EQUAL(grid->getMinX(), 0.0)
RESULT

CHECK(getMinY)
	TEST_REAL_EQUAL(grid->getMinY(), 0.0)
RESULT

CHECK(getMinZ)
	TEST_REAL_EQUAL(grid->getMinZ(), 0.0)
RESULT

CHECK(getMaxXIndex)
	TEST_EQUAL(grid->getMaxXIndex(), 10)
RESULT

CHECK(getMaxYIndex)
	TEST_EQUAL(grid->getMaxYIndex(), 10)
RESULT

CHECK(getMaxZIndex)
	TEST_EQUAL(grid->getMaxZIndex(), 10)
RESULT

CHECK(getXSpacing)
	TEST_REAL_EQUAL(grid->getXSpacing(), 1.0)
RESULT

CHECK(getYSpacing)
	TEST_REAL_EQUAL(grid->getYSpacing(), 1.0)
RESULT

CHECK(getZSpacing)
	TEST_REAL_EQUAL(grid->getZSpacing(), 1.0)
RESULT

BALL::PointGrid<float>::GridIndex	index;

CHECK(getIndex/1)
	lower.set(3.49, 3.51, 3.0);
	index = grid->getIndex(lower);
	TEST_EQUAL(index.x, 3)
	TEST_EQUAL(index.y, 4)
	TEST_EQUAL(index.z, 3)
RESULT

CHECK(getIndex/2)
	index = grid->getIndex(3.49, 3.51, 3.0);
	TEST_EQUAL(index.x, 3)
	TEST_EQUAL(index.y, 4)
	TEST_EQUAL(index.z, 3)
RESULT

CHECK(getData/1/2)
	*(grid->getData(0, 0, 0)) = 5.4321;		
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(*(grid->getData(0)), 5.4321);
	TEST_REAL_EQUAL(*(grid->getData(0)), *(grid->getData(lower)));
RESULT

CHECK(operator[]/1/2)
	(*grid)[3 + 11 * 3 + 11 * 11 * 3] = 1.2345;
	lower.set(3.0, 3.0, 3.0);
	TEST_EQUAL((*grid)[3 + 11 * 3 + 11 * 11 * 3], (*grid)[lower]);
RESULT

CHECK(getGridCoordinates/1)
	lower = grid->getGridCoordinates(0, 0, 0);
	TEST_REAL_EQUAL(lower.x, 0.0)
	TEST_REAL_EQUAL(lower.y, 0.0)
	TEST_REAL_EQUAL(lower.z, 0.0)
RESULT

CHECK(getGridCoordinates/2)
	lower = grid->getGridCoordinates(2 + 2 * 11 + 2 * 11 * 11);
	TEST_REAL_EQUAL(lower.x, 2.0)
	TEST_REAL_EQUAL(lower.y, 2.0)
	TEST_REAL_EQUAL(lower.z, 2.0)
RESULT

CHECK(getGridCoordinates/3)
	upper.set(3.999999, 4.0, 3.0001);
	lower = grid->getGridCoordinates(upper);
	TEST_REAL_EQUAL(lower.x, 3.0)
	TEST_REAL_EQUAL(lower.y, 4.0)
	TEST_REAL_EQUAL(lower.z, 3.0)
RESULT

CHECK(getBoxIndices)
	lower.set(2, 2, 2);
	Position p1, p2, p3, p4, p5, p6, p7, p8;
	TEST_EQUAL(g.getBoxIndices(lower, p1, p2, p3, p4, p5, p6, p7, p8), true)
	TEST_EQUAL(p1, 266);
	TEST_EQUAL(p2, 267);
	TEST_EQUAL(p3, 277);
	TEST_EQUAL(p4, 278);
	TEST_EQUAL(p5, 387);
	TEST_EQUAL(p6, 388);
	TEST_EQUAL(p7, 398);
	TEST_EQUAL(p8, 399);

	lower.set(2.1, 2.1, 2.1);
	TEST_EQUAL(g.getBoxIndices(lower, p1, p2, p3, p4, p5, p6, p7, p8), true)
	TEST_EQUAL(p1, 266);
	TEST_EQUAL(p2, 267);
	TEST_EQUAL(p3, 277);
	TEST_EQUAL(p4, 278);
	TEST_EQUAL(p5, 387);
	TEST_EQUAL(p6, 388);
	TEST_EQUAL(p7, 398);
	TEST_EQUAL(p8, 399);

	lower.set(10.1, 2.1, 2.1);
	TEST_EQUAL(g.getBoxIndices(lower, p1, p2, p3, p4, p5, p6, p7, p8), false)
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
	TEST_EQUAL(g.getBoxData(lower, p1, p2, p3, p4, p5, p6, p7, p8), true)
	TEST_EQUAL(p1, 1);
	TEST_EQUAL(p2, 2);
	TEST_EQUAL(p3, 3);
	TEST_EQUAL(p4, 4);
	TEST_EQUAL(p5, 5);
	TEST_EQUAL(p6, 6);
	TEST_EQUAL(p7, 7);
	TEST_EQUAL(p8, 8);

	lower.set(10.1, 2.1, 2.1);
	TEST_EQUAL(g.getBoxData(lower, p1, p2, p3, p4, p5, p6, p7, p8), false)
RESULT

CHECK(getOrigin)
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 0.0)
	TEST_REAL_EQUAL(lower.y, 0.0)
	TEST_REAL_EQUAL(lower.z, 0.0)
RESULT

CHECK(setOrigin/1)
	grid->setOrigin(1.0, 1.0, 1.0);
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 1.0)
	TEST_REAL_EQUAL(lower.y, 1.0)
	TEST_REAL_EQUAL(lower.z, 1.0)
	index = grid->getIndex(3.49, 3.51, 3.0);
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
	index = grid->getIndex(3.49, 3.51, 3.0);
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
///
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
