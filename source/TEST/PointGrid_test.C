// $Id: PointGrid_test.C,v 1.2 1999/12/29 08:52:52 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/DATATYPE/pointGrid.h>

START_TEST(PointGrid, "$Id: PointGrid_test.C,v 1.2 1999/12/29 08:52:52 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
	
using BALL::PointGrid;
PointGrid<float>*	grid;
CHECK(constructor/1)
grid = new PointGrid<float>(0.0, 0.0, 0.0,
														10.0, 10.0, 10.0,
														11, 11, 11);
TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(destructor)
delete grid;
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

CHECK(getDimension)
TEST_REAL_EQUAL(grid->getDimension().x, 10.0)
TEST_REAL_EQUAL(grid->getDimension().y, 10.0)
TEST_REAL_EQUAL(grid->getDimension().z, 10.0)
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
RESULT

CHECK(setOrigin/2)
lower.set(2.0, 2.0, 2.0);
grid->setOrigin(lower);
lower = grid->getOrigin();
TEST_REAL_EQUAL(lower.x, 2.0)
TEST_REAL_EQUAL(lower.y, 2.0)
TEST_REAL_EQUAL(lower.z, 2.0)
RESULT

CHECK(getMaxX)
TEST_REAL_EQUAL(grid->getMaxX(), 12.0)
RESULT

CHECK(getMaxY)
TEST_REAL_EQUAL(grid->getMaxY(), 12.0)
RESULT

CHECK(getMaxZ)
TEST_REAL_EQUAL(grid->getMaxZ(), 12.0)
RESULT

CHECK(getMinX)
TEST_REAL_EQUAL(grid->getMinX(), 2.0)
RESULT

CHECK(getMinY)
TEST_REAL_EQUAL(grid->getMinY(), 2.0)
RESULT

CHECK(getMinZ)
TEST_REAL_EQUAL(grid->getMinZ(), 2.0)
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

CHECK(getMaxXIndex)
TEST_EQUAL(grid->getMaxXIndex(), 10)
RESULT

CHECK(getMaxYIndex)
TEST_EQUAL(grid->getMaxYIndex(), 10)
RESULT

CHECK(getMaxZIndex)
TEST_EQUAL(grid->getMaxZIndex(), 10)
RESULT

CHECK(operator[])
(*grid)[3 + 11 * 3 + 11 * 11 * 3] = 1.2345;
lower.set(5.0, 5.0, 5.0);
TEST_EQUAL((*grid)[3 + 11 * 3 + 11 * 11 * 3], (*grid)[lower]);
RESULT

BALL::PointGrid<float>::GridIndex	index;
CHECK(getIndex/1)
lower.set(3.49, 3.51, 3.0);
index = grid->getIndex(lower);
TEST_EQUAL(index.x, 1)
TEST_EQUAL(index.y, 2)
TEST_EQUAL(index.z, 1)
RESULT

CHECK(getIndex/2)
index = grid->getIndex(3.49, 3.51, 3.0);
TEST_EQUAL(index.x, 1)
TEST_EQUAL(index.y, 2)
TEST_EQUAL(index.z, 1)
RESULT

CHECK(getData)
*(grid->getData(0, 0, 0)) = 5.4321;		
lower = grid->getOrigin();
TEST_REAL_EQUAL(*(grid->getData(0)), 5.4321);
TEST_REAL_EQUAL(*(grid->getData(0)), *(grid->getData(lower)));
RESULT

CHECK(getGridCoordinates/1)
lower = grid->getGridCoordinates(0, 0, 0);
TEST_REAL_EQUAL(lower.x, 2.0)
TEST_REAL_EQUAL(lower.y, 2.0)
TEST_REAL_EQUAL(lower.z, 2.0)
RESULT

CHECK(getGridCoordinates/2)
lower = grid->getGridCoordinates(2 + 2 * 11 + 2 * 11 * 11);
TEST_REAL_EQUAL(lower.x, 4.0)
TEST_REAL_EQUAL(lower.y, 4.0)
TEST_REAL_EQUAL(lower.z, 4.0)
RESULT

CHECK(getGridCoordinates/3)
upper.set(3.999999, 4.0, 3.0001);
lower = grid->getGridCoordinates(upper);
TEST_REAL_EQUAL(lower.x, 3.0)
TEST_REAL_EQUAL(lower.y, 4.0)
TEST_REAL_EQUAL(lower.z, 3.0)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
