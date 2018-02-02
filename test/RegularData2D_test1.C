// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/DATATYPE/regularData2D.h>

START_TEST(RegularData2D)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;

RegularData2D*	grid;
CHECK(TRegularData2D() throw())
	grid = new RegularData2D();
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(~TRegularData2D() throw())
	delete grid;
RESULT

CHECK(size_type size() const throw())
	RegularData2D g;
	TEST_EQUAL(g.size(), 0)
RESULT

CHECK(const CoordinateType& getOrigin() const throw())
	RegularData2D g;
	TEST_REAL_EQUAL(g.getOrigin().x, 0.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 0.0)
RESULT

CHECK(void setOrigin(const CoordinateType& origin) throw())
	RegularData2D g;
	TEST_REAL_EQUAL(g.getOrigin().x, 0.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 0.0)
	g.setOrigin(Vector2(1.23, 4.56));
	TEST_REAL_EQUAL(g.getOrigin().x, 1.23)
	TEST_REAL_EQUAL(g.getOrigin().y, 4.56)
RESULT

CHECK(const CoordinateType& getDimension() const throw())
	RegularData2D g;
	TEST_REAL_EQUAL(g.getDimension().x, 0.0)
	TEST_REAL_EQUAL(g.getDimension().y, 0.0)
RESULT

CHECK(void setDimension(const CoordinateType& dimension) throw())
	RegularData2D g;
	TEST_REAL_EQUAL(g.getDimension().x, 0.0)
	TEST_REAL_EQUAL(g.getDimension().y, 0.0)
	g.setDimension(Vector2(1.23, 4.56));
	TEST_REAL_EQUAL(g.getDimension().x, 1.23)
	TEST_REAL_EQUAL(g.getDimension().y, 4.56)
RESULT

CHECK(const CoordinateType& getSpacing() const throw())
	RegularData2D g;
	TEST_REAL_EQUAL(g.getSpacing().x, 1.0)
	TEST_REAL_EQUAL(g.getSpacing().y, 1.0)
RESULT

CHECK(TRegularData2D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing))
	RegularData2D g(Vector2(5.0, 7.0), Vector2(2.0, 3.0), Vector2(1.0, 0.5));
	TEST_EQUAL(g.size(), 21)
	TEST_REAL_EQUAL(g.getOrigin().x, 5.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 7.0)
	TEST_REAL_EQUAL(g.getDimension().x, 2.0)
	TEST_REAL_EQUAL(g.getDimension().y, 3.0)
	TEST_REAL_EQUAL(g.getSpacing().x, 1.0)
	TEST_REAL_EQUAL(g.getSpacing().y, 0.5)
	TEST_EQUAL(g.getSize().x, 3)
	TEST_EQUAL(g.getSize().y, 7)
RESULT

RegularData2D g(Vector2(1.0, 2.0), Vector2(3.0, 4.0), Vector2(0.25, 0.5));
CHECK(TRegularData2D(const TRegularData2D<ValueType>& data))
	RegularData2D g0;
	RegularData2D g1(g0);
	TEST_EQUAL(g1.size(), 0)
	g[0] = 1.2;
	g[1] = 2.2;
	g[g.size() - 1] = 5.7;
	TEST_REAL_EQUAL(g[0], 1.2)
	TEST_REAL_EQUAL(g[1], 2.2)
	TEST_REAL_EQUAL(g[g.size() - 1], 5.7)

	RegularData2D g2(g);
	TEST_EQUAL(g2.size(), g.size())
	TEST_EQUAL(g2.getOrigin(), g.getOrigin())
	TEST_EQUAL(g2.getSpacing(), g.getSpacing())
	TEST_EQUAL(g2.getSize().x, g.getSize().x)
	TEST_EQUAL(g2.getSize().y, g.getSize().y)

	TEST_REAL_EQUAL(g2[0], 1.2)
	TEST_REAL_EQUAL(g2[1], 2.2)
	TEST_REAL_EQUAL(g2[g.size() - 1], 5.7)
RESULT

CHECK(void clear() throw())
	RegularData2D g2(g);
	g2[1] = 1.234;
	g2.clear();
	TEST_EQUAL(g2.size(), 0)
RESULT

CHECK(bool operator == (const TRegularData2D<ValueType>& data) const throw())
	g[1] = 1.2345;
	RegularData2D g1;
	g1 = g;
	TEST_EQUAL(g1.size(), 117)
	TEST_REAL_EQUAL(g1[1], 1.2345)
RESULT

CHECK(const IndexType& getSize() const throw())
	TEST_EQUAL(g.getSize().x, 13)
	TEST_EQUAL(g.getSize().y, 9)
RESULT

Vector2 v;
CHECK(const ValueType& getData(Position index) const)
	g[0] = 5.4321;		
	g[1] = 1.2345;		
	TEST_REAL_EQUAL(g.getData(0), 5.4321);
	TEST_REAL_EQUAL(g.getData(1), 1.2345);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getData(117)) 
RESULT

CHECK(ValueType& operator [] (Position index) throw())
	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	rg[0] = 12.4;
	RegularData2D::Iterator it = rg.begin();
	TEST_REAL_EQUAL(*it, 12.4)
	rg[0] = 1.4;
	TEST_REAL_EQUAL(*it, 1.4)
RESULT

CHECK(ValueType& operator [] (const IndexType& index) throw())
	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	rg[RegularData2D::IndexType(0,0)] = 2;
	RegularData2D::Iterator it = rg.begin();
	TEST_REAL_EQUAL(*it, 2)
	rg[RegularData2D::IndexType(1,0)] = 3;
	it++;
	TEST_REAL_EQUAL(*it, 3)
	rg[RegularData2D::IndexType(10,0)] = 3;
RESULT

CHECK(const ValueType& operator [] (Position index) const throw())
	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	g[5] = 1.2345;
	TEST_REAL_EQUAL(g[5], 1.2345);
	TEST_REAL_EQUAL(g[4], 0.0);
RESULT

CHECK(const ValueType& operator [] (const IndexType& index) const throw())
	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	rg[RegularData2D::IndexType(0,0)] = 2;
	TEST_REAL_EQUAL(rg[RegularData2D::IndexType(0,0)], 2)
	rg[RegularData2D::IndexType(1,0)] = 3;
	TEST_REAL_EQUAL(rg[RegularData2D::IndexType(1,0)], 3)
	TEST_REAL_EQUAL(rg[RegularData2D::IndexType(10,0)], 0)
RESULT

CHECK(CoordinateType getCoordinates(Position index) const)
	v = g.getCoordinates(0);
	TEST_REAL_EQUAL(v.x, g.getOrigin().x)
	TEST_REAL_EQUAL(v.y, g.getOrigin().y)
	v = g.getCoordinates(1);
	TEST_REAL_EQUAL(v.x, g.getOrigin().x + g.getSpacing().x)
	TEST_REAL_EQUAL(v.y, g.getOrigin().y)
	v = g.getCoordinates(g.getSize().x);
	TEST_REAL_EQUAL(v.x, g.getOrigin().x)
	TEST_REAL_EQUAL(v.y, g.getOrigin().y + g.getSpacing().y)

	TEST_EXCEPTION(Exception::OutOfGrid, g.getCoordinates(g.size()))
	TEST_EXCEPTION(Exception::OutOfGrid, g.getCoordinates(g.size() + 1))
RESULT

CHECK(CoordinateType getCoordinates(const IndexType& index) const)
	v = g.getCoordinates(RegularData2D::IndexType(0, 0));
	TEST_REAL_EQUAL(v.x, g.getOrigin().x)
	TEST_REAL_EQUAL(v.y, g.getOrigin().y)
	v = g.getCoordinates(RegularData2D::IndexType(1, 0));
	TEST_REAL_EQUAL(v.x, g.getOrigin().x + g.getSpacing().x)
	TEST_REAL_EQUAL(v.y, g.getOrigin().y)
	v = g.getCoordinates(RegularData2D::IndexType(0, 1));
	TEST_REAL_EQUAL(v.x, g.getOrigin().x)
	TEST_REAL_EQUAL(v.y, g.getOrigin().y + g.getSpacing().y)

	TEST_EXCEPTION(Exception::OutOfGrid, g.getCoordinates(RegularData2D::IndexType(0, 10000)))
	TEST_EXCEPTION(Exception::OutOfGrid, g.getCoordinates(RegularData2D::IndexType(100000, 0)))
RESULT

CHECK(void getEnclosingIndices(const CoordinateType& r, Position& ll, Position& lr, Position& ul, Position& ur) const)
	v = g.getOrigin() + g.getSpacing() * 0.5;
	Position p1, p2, p3, p4;
	g.getEnclosingIndices(v, p1, p2, p3, p4);
	TEST_EQUAL(p1, 0);
	TEST_EQUAL(p2, 1);
	TEST_EQUAL(p3, g.getSize().x);
	TEST_EQUAL(p4, g.getSize().x + 1);

	v = g.getOrigin() + g.getSpacing() * 1.03;
	g.getEnclosingIndices(v, p1, p2, p3, p4);
	TEST_EQUAL(p1, g.getSize().x + 1);
	TEST_EQUAL(p2, g.getSize().x + 2);
	TEST_EQUAL(p3, 2 * g.getSize().x + 1);
	TEST_EQUAL(p4, 2 * g.getSize().x + 2);

	v = g.getOrigin() + Vector2(-0.001, 0.0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingIndices(v, p1, p2, p3, p4))
	v = g.getOrigin() + g.getDimension() + Vector2(0.0, 0.001);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingIndices(v, p1, p2, p3, p4))
	v = g.getOrigin() + Vector2(0.1, -0.0001);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingIndices(v, p1, p2, p3, p4))
	v = g.getOrigin() + g.getDimension() + Vector2(0.001, 0.0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingIndices(v, p1, p2, p3, p4))
RESULT

CHECK(void getEnclosingValues(const CoordinateType& r, ValueType& ll, ValueType& lr, ValueType& ul, ValueType& ur) const)
	v = g.getOrigin() + g.getSpacing() * 0.999;
	float p1, p2, p3, p4;
	g[0] = 1.0;
	g[1] = 2.0;
	g[g.getSize().x] = 3.0;
	g[g.getSize().x + 1] = 4.0;
	g.getEnclosingValues(v, p1, p2, p3, p4);
	TEST_REAL_EQUAL(p1, 1.0);
	TEST_REAL_EQUAL(p2, 2.0);
	TEST_REAL_EQUAL(p3, 3.0);
	TEST_REAL_EQUAL(p4, 4.0);

	v = g.getOrigin() + Vector2(-0.001, 0.0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingValues(v, p1, p2, p3, p4))
	v = g.getOrigin() + g.getDimension() + Vector2(0.0, 0.001);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingValues(v, p1, p2, p3, p4))
	v = g.getOrigin() + Vector2(0.1, -0.0001);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingValues(v, p1, p2, p3, p4))
	v = g.getOrigin() + g.getDimension() + Vector2(0.001, 0.0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getEnclosingValues(v, p1, p2, p3, p4))
RESULT

g = RegularData2D(Vector2(0.0, 0.0), Vector2(4.0, 9.0), Vector2(2.0, 3.0));

CHECK(ValueType getInterpolatedValue(const CoordinateType& x) const)
for (Position i = 0; i < g.size() ; i++)
{
	g[i] = i;
}
v = Vector2(0.0, 0.0);
TEST_EQUAL(g.getInterpolatedValue(v), 0.0)

v = Vector2(4.0, 9.0);
TEST_EQUAL(g.getInterpolatedValue(v), 11.0)

v = Vector2(2.0, 4.5);
TEST_EQUAL(g.getInterpolatedValue(v), 5.5)

v = Vector2(4.0, 2.25);
TEST_EQUAL(g.getInterpolatedValue(v), 4.25)

v = Vector2(10.1, 0);
TEST_EXCEPTION(Exception::OutOfGrid, g.getInterpolatedValue(v))
RESULT

RegularData2D grid2 = g;

CHECK(bool isInside(const CoordinateType& x) const throw())
	RegularData2D g(Vector2(0.0, 0.0), Vector2(10.0, 10.0), Vector2(1.0, 1.0));
	TEST_EQUAL(g.isInside(Vector2(0.0, 0.0)), true)
	TEST_EQUAL(g.isInside(Vector2(10.0, 10.0)), true)
	TEST_EQUAL(g.isInside(Vector2(10.1, 10.0)), false)

	RegularData2D h;
	TEST_EQUAL(h.isInside(Vector2(0.0, 0.0)), true)
RESULT

CHECK(void resize(const IndexType& new_size))
	RegularData2D g(Vector2(0.0), Vector2(9.0), Vector2(9.0));
	g[0] = 0.0;
	g[1] = 1.0;
	g[2] = 1.0;
	g[3] = 2.0;
	g.resize(RegularData2D::IndexType(3, 3));
	TEST_EQUAL(g[0], 0)
	TEST_EQUAL(g[1], 1.0)
	TEST_EQUAL(g[2], 0.0)
	TEST_EQUAL(g[3], 1.0)
	TEST_EQUAL(g[4], 2.0)
	TEST_EQUAL(g[5], 0.0)
	TEST_EQUAL(g[6], 0.0)
	TEST_EQUAL(g[7], 0.0)
	TEST_EQUAL(g[8], 0.0)
RESULT

CHECK(void rescale(const IndexType& new_size))
	RegularData2D g(Vector2(0.0), Vector2(9.0), Vector2(9.0));
	g[0] = 0.0;
	g[1] = 1.0;
	g[2] = 1.0;
	g[3] = 2.0;
	g.rescale(RegularData2D::IndexType(3, 3));
	TEST_EQUAL(g[0], 0)
	TEST_EQUAL(g[1], 0.5)
	TEST_EQUAL(g[2], 1)
	TEST_EQUAL(g[3], 0.5)
	TEST_EQUAL(g[4], 1.0)
	TEST_EQUAL(g[5], 1.5)
	TEST_EQUAL(g[6], 1.0)
	TEST_EQUAL(g[7], 1.5)
	TEST_EQUAL(g[8], 2.0)
RESULT

CHECK(BALL_CREATE(TRegularData2D<ValueType>))
  RegularData2D g(Vector2(5.0, 7.0), Vector2(2.0, 3.0), Vector2(1.0, 0.5));
	RegularData2D* v_ptr = (RegularData2D*)g.create(false, true);
	RegularData2D empty;
	TEST_EQUAL(empty == *v_ptr, true)
	delete v_ptr;
	v_ptr = (RegularData2D*)g.create();
	TEST_EQUAL(g == *v_ptr, true)
	delete v_ptr;
RESULT

CHECK(ConstIterator begin() const throw())
  RegularData2D g(Vector2(5.0, 7.0), Vector2(2.0, 3.0), Vector2(1.0, 0.5));
	g[0] = 2;
	RegularData2D::Iterator it = g.begin();
	TEST_REAL_EQUAL(*it, 2)
RESULT

CHECK(ConstIterator end() const throw())
	RegularData2D g;
	RegularData2D::Iterator it = g.begin();
	TEST_EQUAL(it == g.end(), true)
RESULT

CHECK(IndexType getClosestIndex(const CoordinateType& v) const)
  RegularData2D g(Vector2(5.0, 7.0), Vector2(2.0, 3.0), Vector2(1.0, 0.5));
	TEST_EQUAL(g.getClosestIndex(Vector2(6.4, 8.72)).x, 1)
	TEST_EQUAL(g.getClosestIndex(Vector2(6.4, 8.72)).y, 3)
	RegularData2D empty;
	TEST_EXCEPTION(Exception::OutOfGrid, empty.getClosestIndex(Vector2(6.4, 8.72)).x)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
