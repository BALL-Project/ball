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
Vector2 v;

CHECK(IndexType getLowerIndex(const CoordinateType& v) const)
  RegularData2D g(Vector2(5.0, 7.0), Vector2(2.0, 3.0), Vector2(1.0, 0.5));
	TEST_EQUAL(g.getLowerIndex(Vector2(6.6, 8.76)).x, 1)
	TEST_EQUAL(g.getLowerIndex(Vector2(6.6, 8.76)).y, 3)
	RegularData2D empty;
	TEST_EXCEPTION(Exception::OutOfGrid, empty.getLowerIndex(Vector2(6.4, 8.72)).x)
RESULT

CHECK(IndexType())
	RegularData2D::IndexType p;
	TEST_REAL_EQUAL(p.x, 0.0)
	TEST_REAL_EQUAL(p.y, 0.0)
RESULT

CHECK(IndexType(Position p))
	RegularData2D::IndexType p(2);
	TEST_REAL_EQUAL(p.x, 2.0)
	TEST_REAL_EQUAL(p.y, 2.0)
RESULT

CHECK(IndexType(Position p, Position q))
	RegularData2D::IndexType p(2,4);
	TEST_REAL_EQUAL(p.x, 2.0)
	TEST_REAL_EQUAL(p.y, 4.0)
RESULT

CHECK(Iterator begin() throw())
  RegularData2D g(Vector2(5.0, 7.0), Vector2(2.0, 3.0), Vector2(1.0, 0.5));
	g[0] = 2.0;
	g[1] = 3.0;

	RegularData2D::Iterator it = g.begin();
	TEST_REAL_EQUAL(*it, 2.0)
	it++;
	TEST_REAL_EQUAL(*it, 3.0)

	RegularData2D empty;
	TEST_EQUAL(empty.begin() == empty.end(), true)
RESULT

CHECK(Iterator end() throw())
	RegularData2D empty;
	TEST_EQUAL(empty.begin() == empty.end(), true)
RESULT

CHECK(TRegularData2D(const IndexType& size, const CoordinateType& origin = CoordinateType(0.0), const CoordinateType& dimension = CoordinateType(1.0)))
	RegularData2D g(3, Vector2(1,2), Vector2(2,4));
	TEST_REAL_EQUAL(g.getOrigin().x, 1.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 2.0)
	TEST_REAL_EQUAL(g.getDimension().x, 2.0)
	TEST_REAL_EQUAL(g.getDimension().y, 4.0)
	TEST_REAL_EQUAL(g.getSpacing().x, 1)
	TEST_REAL_EQUAL(g.getSpacing().y, 2)
RESULT

CHECK(ValueType operator () (const CoordinateType& x) const throw())
	RegularData2D g(Vector2(0.0, 0.0), Vector2(4.0, 9.0), Vector2(2.0, 3.0));
	for (Position i = 0; i < g.size() ; i++)
	{
		g[i] = i;
	}
	v = Vector2(0.0, 0.0);
	TEST_REAL_EQUAL(g(v), 0.0)

	v = Vector2(4.0, 9.0);
	TEST_REAL_EQUAL(g(v), 11.0)

	v = Vector2(2.0, 4.5);
	TEST_REAL_EQUAL(g(v), 5.5)

	v = Vector2(4.0, 2.25);
	TEST_REAL_EQUAL(g(v), 4.25)

	v = Vector2(10.1, 0);
	TEST_REAL_EQUAL(g(v), 5.05)
RESULT


RegularData2D rg(Vector2(-2.5), Vector2(5.0), Vector2(1.0, 1.0));
for (Position y = 0; y < rg.size() ; y++)
{
	rg[y] = (float)y;
}
CHECK(ValueType& getClosestValue(const CoordinateType& x))
	RegularData2D rg2(rg);
	TEST_REAL_EQUAL(rg2.getClosestValue(Vector2(1.6, 1.9)), 28.0)
	rg2.getClosestValue(Vector2(1.6, 1.9)) = -150.0;
	TEST_REAL_EQUAL(rg2.getData(RegularData2D::IndexType((int)(1.6 + 2.5 + 0.5), (Index)(1.9 + 2.5 + 0.5))), -150.0)
	TEST_EXCEPTION(Exception::OutOfGrid, rg2.getClosestValue(Vector2(2.505, 2.000)))
	TEST_EXCEPTION(Exception::OutOfGrid, rg2.getClosestValue(Vector2(2.000, 2.505)))
	TEST_EXCEPTION(Exception::OutOfGrid, rg2.getClosestValue(Vector2(-2.505, -2.000)))
	TEST_EXCEPTION(Exception::OutOfGrid, rg2.getClosestValue(Vector2(-2.000, -2.505)))
RESULT

CHECK(const ValueType& getClosestValue(const CoordinateType& x) const)
	const RegularData2D& rg2 = rg;
	TEST_REAL_EQUAL(rg2.getClosestValue(Vector2(-0.3, -0.2)), 14.0)
	TEST_EXCEPTION(Exception::OutOfGrid, rg.getClosestValue(Vector2(2.505, 2.000)))
	TEST_EXCEPTION(Exception::OutOfGrid, rg.getClosestValue(Vector2(2.000, 2.505)))
	TEST_EXCEPTION(Exception::OutOfGrid, rg.getClosestValue(Vector2(-2.505, -2.000)))
	TEST_EXCEPTION(Exception::OutOfGrid, rg.getClosestValue(Vector2(-2.000, -2.505)))
RESULT

CHECK(ValueType& getData(Position index))
	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	for (Position i = 0; i < rg.size() ; i++) { rg[i] = i; }
	TEST_REAL_EQUAL(rg.getData(0), 0.0)
	TEST_REAL_EQUAL(rg.getData(1), 1.0)
	TEST_REAL_EQUAL(rg.getData(25), 25)
	TEST_EXCEPTION(Exception::OutOfGrid, rg.getData(rg.getSize()))
RESULT

CHECK(ValueType& getData(const IndexType& index))
	RegularData2D rg(Vector2(0.0), Vector2(5.0), Vector2(1.0));
	for (Position x = 0; x < rg.getSize().x; x++) 
	{
		for (Position y = 0; y < rg.getSize().y; y++)
		{ 
			rg.getData(RegularData2D::IndexType(x, y)) = (float)(x * y);
		}
	}
	STATUS("Size: " << rg.getSize().x << "x" << rg.getSize().y)
	STATUS("Org : " << rg.getOrigin())
	STATUS("Dim : " << rg.getDimension())

	TEST_REAL_EQUAL(rg.getData(RegularData2D::IndexType(0, 0)), 0.0)
	TEST_REAL_EQUAL(rg.getData(RegularData2D::IndexType(1, 1)), 1.0)
	TEST_REAL_EQUAL(rg.getData(RegularData2D::IndexType(5, 5)), 25.0)
	TEST_EXCEPTION(Exception::OutOfGrid, rg.getData(RegularData2D::IndexType(12, 12)))
RESULT

CHECK(const ValueType& getData(const IndexType& index) const)
	RegularData2D rg(Vector2(0.0), Vector2(5.0), Vector2(1.0));
	for (Position x = 0; x < rg.getSize().x; x++) 
	{
		for (Position y = 0; y < rg.getSize().y; y++)
		{ 
			rg.getData(RegularData2D::IndexType(x, y)) = x * y;
		}
	}
	const RegularData2D& c_rg(rg);
	TEST_REAL_EQUAL(c_rg.getData(RegularData2D::IndexType(0, 0)), 0.0)
	TEST_REAL_EQUAL(c_rg.getData(RegularData2D::IndexType(1, 1)), 1.0)
	TEST_REAL_EQUAL(c_rg.getData(RegularData2D::IndexType(5, 5)), 25.0)
	TEST_EXCEPTION(Exception::OutOfGrid, c_rg.getData(RegularData2D::IndexType(12, 12)))
RESULT

CHECK(bool empty() const throw())
	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	TEST_EQUAL(rg.empty(), false);
	RegularData2D empty;
	TEST_EQUAL(empty.empty(), true);
RESULT

CHECK(bool operator != (const TRegularData2D<ValueType>& data) const throw())
 	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	for (Position x = 0; x < 6; x++) 
		for (Position y = 0; y < 6; y++)
	{ 
		rg[RegularData2D::IndexType(x,y)] = x*y;
	}

	RegularData2D rg2;
	TEST_EQUAL(rg != rg2, true)
	rg2 = rg;
	TEST_EQUAL(rg != rg2, false)
	rg[RegularData2D::IndexType(3,3)] = 121111;
	TEST_EQUAL(rg != rg2, true)
RESULT

CHECK(TRegularData2D& operator = (const TRegularData2D<ValueType>& data))
 	RegularData2D rg(Vector2(0.0), Vector2(5,5), Vector2(1.0, 1.0));
	for (Position x = 0; x < 6; x++) 
		for (Position y = 0; y < 6; y++)
	{ 
		rg[RegularData2D::IndexType(x,y)] = x*y;
	}

	RegularData2D rg2 = rg;
	TEST_EQUAL(rg == rg2, true)
RESULT

CHECK(size_type max_size() const throw())
	RegularData2D rg;
	TEST_EQUAL(rg.max_size() > 10000, true)
RESULT

CHECK(void binaryWrite(const String& filename) const)
	NEW_TMP_FILE(filename)
 	RegularData2D g(Vector2(1,2), Vector2(8,10), Vector2(1.0, 1.0));
	for (Position x = 0; x < g.getSize().x; x++) 
	{
		for (Position y = 0; y < g.getSize().y; y++)
		{ 
			g[RegularData2D::IndexType(x,y)] = x*y;
		}
	}
	TEST_EQUAL(g.size(), 99)
	TEST_REAL_EQUAL(g.getOrigin().x, 1.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 2.0)
	TEST_REAL_EQUAL(g.getDimension().x, 8.0)
	TEST_REAL_EQUAL(g.getDimension().y, 10.0)
	TEST_REAL_EQUAL(g.getSpacing().x, 1.0)
	TEST_REAL_EQUAL(g.getSpacing().y, 1.0)
	TEST_EQUAL(g.getSize().x, 9)
	TEST_EQUAL(g.getSize().y, 11)

	g.binaryWrite(filename);
RESULT

CHECK(void binaryRead(const String& filename))
	RegularData2D g;
	g.binaryRead(filename);
	for (Position x = 0; x < 7; x++) 
	{
		for (Position y = 0; y < 8; y++)
		{ 
			TEST_REAL_EQUAL(g[RegularData2D::IndexType(x,y)], x*y)
		}
	}

	TEST_EQUAL(g.size(), 99)
	TEST_REAL_EQUAL(g.getOrigin().x, 1.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 2.0)
	TEST_REAL_EQUAL(g.getDimension().x, 8.0)
	TEST_REAL_EQUAL(g.getDimension().y, 10.0)
	TEST_REAL_EQUAL(g.getSpacing().x, 1.0)
	TEST_REAL_EQUAL(g.getSpacing().y, 1.0)
	TEST_EQUAL(g.getSize().x, 9)
	TEST_EQUAL(g.getSize().y, 11)
RESULT

CHECK(void swap(TRegularData2D<ValueType>& data) throw())
 	RegularData2D rg2(Vector2(1,2), Vector2(8,10), Vector2(1.0, 1.0));
	for (Position x = 1; x < 8; x++) 
		for (Position y = 2; y < 10; y++)
	{ 
		rg2[RegularData2D::IndexType(x,y)] = x*y;
	}

 	RegularData2D g(Vector2(0,1), Vector2(7,9), Vector2(0.5, 0.6));
	for (Position x = 0; x < 7; x++)
		for (Position y = 1; y < 9; y++)
	{ 
		g[RegularData2D::IndexType(x,y)] = x*y/2;
	}

	g.swap(rg2); 

	TEST_EQUAL(g.size(), 99)
	TEST_REAL_EQUAL(g.getOrigin().x, 1.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 2.0)
	TEST_REAL_EQUAL(g.getDimension().x, 8.0)
	TEST_REAL_EQUAL(g.getDimension().y, 10.0)
	TEST_REAL_EQUAL(g.getSpacing().x, 1.0)
	TEST_REAL_EQUAL(g.getSpacing().y, 1.0)
	TEST_EQUAL(g.getSize().x, 9)
	TEST_EQUAL(g.getSize().y, 11)

	TEST_EQUAL(rg2.size(), 240)
	TEST_REAL_EQUAL(rg2.getOrigin().x, 0.0)
	TEST_REAL_EQUAL(rg2.getOrigin().y, 1.0)
	TEST_REAL_EQUAL(rg2.getDimension().x, 7.0)
	TEST_REAL_EQUAL(rg2.getDimension().y, 9.0)
	TEST_REAL_EQUAL(rg2.getSpacing().x, 0.5)
	TEST_REAL_EQUAL(rg2.getSpacing().y, 0.6)
	TEST_EQUAL(rg2.getSize().x, 15)
	TEST_EQUAL(rg2.getSize().y, 16)


	for (Position x = 1; x < 8; x++) 
		for (Position y = 2; y < 10; y++)
	{ 
		TEST_REAL_EQUAL(g[RegularData2D::IndexType(x,y)], x*y)
	}

	for (Position x = 0; x < 7; x++) 
		for (Position y = 1; y < 9; y++)
	{ 
		TEST_REAL_EQUAL(rg2[RegularData2D::IndexType(x,y)], x*y/2)
	}
RESULT

CHECK([EXTRA]STL compatibility - Container requirements)
	// Make sure we have a value_type
	RegularData2D::value_type value(5);
	TEST_EQUAL(value, 5)
	
	// Make sure we have a reference type
	RegularData2D::reference value_ref(value);
	TEST_EQUAL(value_ref, 5)
	value_ref = 6;
	TEST_EQUAL(value_ref, 6)
	TEST_EQUAL(value, 6)

	// Make sure we have a const reference type
	RegularData2D::const_reference const_value_ref(value);
	TEST_EQUAL(const_value_ref, 6)

	// Make sure we have a pointer type
	RegularData2D::pointer value_ptr = &value;
	TEST_NOT_EQUAL(value_ptr, 0)
	ABORT_IF(value_ptr == 0)
	TEST_EQUAL(*value_ptr, 6)
	*value_ptr = 5;
	TEST_EQUAL(*value_ptr, 5)
	TEST_EQUAL(value, 5)
	TEST_EQUAL(value_ref, 5)

	// Check for size_type
	RegularData2D grid(Vector2(1.0, 1.0), Vector2(2.0, 2.0), Vector2(1.0, 0.5));
	RegularData2D::size_type size;
	
	
	// Check for iterator type
	RegularData2D::iterator it1;
	RegularData2D::iterator it2;
	
	// Check for difference_type
	RegularData2D::difference_type diff;

	// Valid expressions:
	it1 = grid.begin();
	it2 = grid.end();
	size = grid.size();
	TEST_EQUAL(size, 15)
	size = grid.max_size();
	STATUS("max_size() = " << size)
	TEST_EQUAL((size > 1e7), true)
	RegularData2D g2;
	grid.swap(g2);
	TEST_EQUAL(grid.size(), 0)
	TEST_EQUAL(g2.size(), 15)

	// Make sure the difference computation works alright.
	diff = it2 - it1;
	TEST_EQUAL(diff, 15)

	TEST_EQUAL(g2.end() - g2.begin(), (RegularData2D::difference_type)g2.size())

	std::fill(g2.begin(), g2.end(), 1.2);
	TEST_REAL_EQUAL(g2[0], 1.2)
	TEST_REAL_EQUAL(g2[1], 1.2)
	TEST_REAL_EQUAL(g2[2], 1.2)
	TEST_REAL_EQUAL(g2[3], 1.2)
	TEST_REAL_EQUAL(g2[4], 1.2)
	TEST_REAL_EQUAL(g2[5], 1.2)
	TEST_REAL_EQUAL(g2[6], 1.2)
	TEST_REAL_EQUAL(g2[7], 1.2)
	TEST_REAL_EQUAL(g2[8], 1.2)
	TEST_REAL_EQUAL(g2[9], 1.2)
	TEST_REAL_EQUAL(g2[10], 1.2)
	TEST_REAL_EQUAL(g2[11], 1.2)
	TEST_REAL_EQUAL(g2[12], 1.2)
	TEST_REAL_EQUAL(g2[13], 1.2)
	TEST_REAL_EQUAL(g2[14], 1.2)
RESULT

CHECK([EXTRA]STL compatibility - Unary Function requirements)
	RegularData2D g3(Vector2(0.0), Vector2(1.0), Vector2(1.0));
	g3[0] = 0.0;
	g3[1] = 1.0;
	g3[2] = 1.0;
	g3[3] = 2.0;
	for (double x = 0.0; x < 1.0; x += 0.12)
	{
		for (double y = 0.02; y < 1.0; y += 0.17)
		{
			TEST_REAL_EQUAL(g3(Vector2(x, y)), (x + y))
		}
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
