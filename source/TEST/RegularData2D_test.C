// $Id: RegularData2D_test.C,v 1.1 2001/05/31 21:51:33 amoll Exp $

#include <BALL/CONCEPT/classTest.h>
#include <BALL/DATATYPE/regularData2D.h>

START_TEST(RegularData2D, "$Id: RegularData2D_test.C,v 1.1 2001/05/31 21:51:33 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
using BALL::RegularData2D;
RegularData2D*	grid;

CHECK(RegularData2D())
	grid = new RegularData2D();
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(~RegularData2D())
	delete grid;
RESULT

RegularData2D g;

CHECK(RegularData2D(/2))
	g = RegularData2D(0.0, 0.0, 4.0, 9.0, 2.0, 3.0);
	TEST_EQUAL(g.getSize(), 6)

	TEST_REAL_EQUAL(g.getMinX(), 0.0)
	TEST_REAL_EQUAL(g.getMinY(), 0.0)

	TEST_REAL_EQUAL(g.getMaxX(), 4.0)
	TEST_REAL_EQUAL(g.getMaxY(), 9.0)

	TEST_EQUAL(g.getMaxXIndex(), 1)
	TEST_EQUAL(g.getMaxYIndex(), 2)

	TEST_REAL_EQUAL(g.getXSpacing(), 4.0)
	TEST_REAL_EQUAL(g.getYSpacing(), 4.5)

	RegularData2D::Vector2 p(0, 0);
	TEST_EQUAL(g.getOrigin()== p, true)
RESULT

CHECK(RegularData2D(const TRegularData2D<GridDataType>& grid, bool deep = true))
	RegularData2D g2(g);
	TEST_EQUAL(g.getSize(), 6)
RESULT

CHECK(clear())
	RegularData2D g2(g);
	g2[1] = 1.234;
	g2.clear();
	TEST_EQUAL(g2.isValid(), false)
	TEST_EQUAL(g2.getSize(), 0)
RESULT

CHECK(getSize())
	TEST_EQUAL(g.getSize(), 6)
	RegularData2D g2;
	TEST_EQUAL(g2.getSize(), 0)
RESULT

CHECK(set(const RegularData2D& grid))
	g[1] = 1.2345;
	RegularData2D g1;
	g1.set(g);
	TEST_EQUAL(g1.getSize(), 6)
	TEST_REAL_EQUAL(g1[1], 1.2345)
RESULT

CHECK(operator = (const RegularData2D& grid))
	g[1] = 1.2345;
	RegularData2D g1;
	g1 = g;
	TEST_EQUAL(g1.getSize(), 6)
	TEST_REAL_EQUAL(g1[1], 1.2345)
RESULT

CHECK(dump())
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	for (Position k = 0; k < g.getSize(); k++)
	{
		g[k] = k;
	}
	g.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/RegularData2D_test.txt", false)
RESULT


CHECK(isValid())
	TEST_EQUAL(g.isValid(), true)
RESULT

CHECK(getMaxX())
	TEST_REAL_EQUAL(g.getMaxX(), 4.0)
RESULT

CHECK(getMaxY())
	TEST_REAL_EQUAL(g.getMaxY(), 9.0)
RESULT

CHECK(getMinX())
	TEST_REAL_EQUAL(g.getMinX(), 0.0)
RESULT

CHECK(getMinY())
	TEST_REAL_EQUAL(g.getMinY(), 0.0)
RESULT

CHECK(getMaxXIndex())
	TEST_EQUAL(g.getMaxXIndex(), 1)
RESULT

CHECK(getMaxYIndex())
	TEST_EQUAL(g.getMaxYIndex(), 2)
RESULT

CHECK(getXSpacing())
	TEST_REAL_EQUAL(g.getXSpacing(), 4.0)
RESULT

CHECK(getYSpacing())
	TEST_REAL_EQUAL(g.getYSpacing(), 4.5)
RESULT

BALL::RegularData2D::GridIndex	pp;
RegularData2D::Vector2 v;

CHECK(getIndex(const Vector2& vector))
	v = RegularData2D::Vector2(2.11, 4.51);
	pp = g.getIndex(v);
	TEST_EQUAL(pp.x, 1)
	TEST_EQUAL(pp.y, 1)
RESULT

CHECK(getIndex(/2))
	pp = g.getIndex(2.11, 4.51);
	TEST_EQUAL(pp.x, 1)
	TEST_EQUAL(pp.y, 1)
RESULT

CHECK(getData(Position))
	*(g.getData(0, 0)) = 5.4321;		
	v = g.getOrigin();
	TEST_REAL_EQUAL(*(g.getData(0)), 5.4321);
RESULT

CHECK(operator[]/1/2)
	g[5] = 1.2345;
	v = RegularData2D::Vector2(3.0, 8.2);
	TEST_EQUAL(g[5], g[v]);
RESULT

CHECK(getGridCoordinates/1)
	v = g.getGridCoordinates(1, 1);
	TEST_REAL_EQUAL(v.first, 4.0)
	TEST_REAL_EQUAL(v.second, 4.5)
RESULT

CHECK(getGridCoordinates/2)
	v = g.getGridCoordinates(5);
	TEST_REAL_EQUAL(v.first, 4.0)
	TEST_REAL_EQUAL(v.second, 9.0)

	TEST_EXCEPTION(Exception::OutOfGrid, v = g.getGridCoordinates(6))
	TEST_EXCEPTION(Exception::OutOfGrid, g.getGridCoordinates(7))
RESULT

CHECK(getRectangleIndices)
	v = RegularData2D::Vector2(3.9, 3.9);
	Position p1, p2, p3, p4;
	g.getRectangleIndices(v, p1, p2, p3, p4);
	TEST_EQUAL(p1, 0);
	TEST_EQUAL(p2, 1);
	TEST_EQUAL(p3, 2);
	TEST_EQUAL(p4, 3);

	v = RegularData2D::Vector2(3.1, 4.6);
	g.getRectangleIndices(v, p1, p2, p3, p4);
	TEST_EQUAL(p1, 2);
	TEST_EQUAL(p2, 3);
	TEST_EQUAL(p3, 4);
	TEST_EQUAL(p4, 5);

	v = RegularData2D::Vector2(10.1, 2.1);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getRectangleIndices(v, p1, p2, p3, p4))
RESULT

CHECK(getRectangleData)
	v = RegularData2D::Vector2(0, 4.2);
	float p1, p2, p3, p4;
	g[0] = 1;
	g[1] = 2;
	g[2] = 3;
	g[3] = 4;
	g.getRectangleData(v, p1, p2, p3, p4);
	TEST_EQUAL(p1, 1);
	TEST_EQUAL(p2, 2);
	TEST_EQUAL(p3, 3);
	TEST_EQUAL(p4, 4);

	v = RegularData2D::Vector2(4.1, 2.1);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getRectangleData(v, p1, p2, p3, p4))
RESULT

CHECK(getOrigin)
	v = g.getOrigin();
	TEST_REAL_EQUAL(v.first, 0.0)
	TEST_REAL_EQUAL(v.second, 0.0)
RESULT

CHECK(setOrigin/1)
	g.setOrigin(1.0, 1.0);
	v = g.getOrigin();
	TEST_REAL_EQUAL(v.first, 1.0)
	TEST_REAL_EQUAL(v.second, 1.0)
	pp = g.getIndex(2.49, 5.1);
	TEST_EQUAL(pp.x, 0)
	TEST_EQUAL(pp.y, 1)
RESULT

CHECK(setOrigin/2)
	v = RegularData2D::Vector2(2.0, 2.0);
	g.setOrigin(v);
	v = g.getOrigin();
	TEST_REAL_EQUAL(v.first, 2.0)
	TEST_REAL_EQUAL(v.second, 2.0)
	pp = g.getIndex(3.94, 6.41);
	TEST_EQUAL(pp.x, 0)
	TEST_EQUAL(pp.y, 1)
RESULT

CHECK(getDimension)
	TEST_REAL_EQUAL(g.getDimension().first, 4.0)
	TEST_REAL_EQUAL(g.getDimension().second, 9.0)
RESULT

g = RegularData2D(0.0, 0.0, 4.0, 9.0, 2.0, 3.0);

CHECK(getInterpolatedValue)
	for (Position i = 0; i < 5 ; i++)
	{
		g[i] = i;
	}
	v = RegularData2D::Vector2(0.0, 0.0);
	TEST_EQUAL(g.getInterpolatedValue(v), 0)

	v = RegularData2D::Vector2(4.0, 9.0);
	TEST_EQUAL(g.getInterpolatedValue(v), 0)

	v = RegularData2D::Vector2(2.0, 4.5);
	TEST_EQUAL(g.getInterpolatedValue(v), 2.5)

	v = RegularData2D::Vector2(4.0, 2.25);
	TEST_EQUAL(g.getInterpolatedValue(v), 2.0)

	v = RegularData2D::Vector2(10.1, 0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getInterpolatedValue(v))
RESULT

RegularData2D grid2 = g;

CHECK(operator ==)
	grid2 = g;
	TEST_EQUAL(g.isValid(), true)
	TEST_EQUAL(g == g, true)
	grid2[5] = -99.9;
	TEST_EQUAL(g == grid2, false)
RESULT

CHECK(operator !=)
	TEST_EQUAL(g != grid2, true)
	grid2[5] = (g)[5];
	TEST_EQUAL(g != grid2, false)
RESULT

CHECK(has()1/1)
	RegularData2D g(0.0, 0.0, 10.0, 10.0, 10, 10);
	TEST_EQUAL(g.has(0.0, 0.0), true)
	TEST_EQUAL(g.has(10.0, 10.0), true)
	TEST_EQUAL(g.has(10.1, 10.0), false)

	RegularData2D h;
	TEST_EQUAL(h.isValid(), false)
	TEST_EQUAL(h.has(0.0, 0.0), false)
RESULT

CHECK(asd)
		   
RESULT
			
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
