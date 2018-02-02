// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>
#include <BALL/DATATYPE/regularData3D.h>

START_TEST(RegularData3D)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
RegularData3D*	grid;

CHECK([EXTRA]operator >> (instream& os, RegularData3D&))
RESULT

CHECK(BALL_CREATE(TRegularData3D<ValueType>))
  RegularData3D g(Vector3(1.0, 2.0, 3.0), Vector3(4.0, 5.0, 6.0), Vector3(0.1, 0.2, 0.3));
	g[0] = 1.23;
	RegularData3D* p = (RegularData3D*)g.create(false, true);
	RegularData3D empty;
	TEST_EQUAL(*p, empty)
	delete p;
	p = (RegularData3D*)g.create();
	TEST_REAL_EQUAL(p->getOrigin().x, 1.0)
	TEST_REAL_EQUAL(p->getOrigin().y, 2.0)
	TEST_REAL_EQUAL(p->getOrigin().z, 3.0)
	TEST_REAL_EQUAL(p->getDimension().x, 4.0)
	TEST_REAL_EQUAL(p->getDimension().y, 5.0)
	TEST_REAL_EQUAL(p->getDimension().z, 6.0)
	TEST_REAL_EQUAL(p->getSpacing().x, 0.1)
	TEST_REAL_EQUAL(p->getSpacing().y, 0.2)
	TEST_REAL_EQUAL(p->getSpacing().z, 0.3)
	TEST_REAL_EQUAL((*p)[0], 1.23)
	delete p;
RESULT

CHECK(ConstIterator begin() const throw())
	RegularData3D rg;
	RegularData3D::ConstIterator it = rg.begin();
	TEST_EQUAL(it == rg.end(), true)
	rg = RegularData3D(Vector3(0,0,0),Vector3(1,0,0),Vector3(1,1,1));
	rg[0] = 1.23;
	it = rg.begin();
	TEST_REAL_EQUAL(*it, 1.23)
	it++;
	TEST_REAL_EQUAL(*it, 0)
RESULT

CHECK(ConstIterator end() const throw())
	RegularData3D rg;
	RegularData3D::ConstIterator it = rg.end();
	TEST_EQUAL(it == rg.end(), true)
RESULT

CHECK(Iterator begin() throw())
	RegularData3D rg;
	RegularData3D::Iterator it = rg.begin();
	TEST_EQUAL(it == rg.end(), true)
	rg = RegularData3D(Vector3(0,0,0),Vector3(1,0,0),Vector3(1,1,1));
	rg[0] = 1.23;
	it = rg.begin();
	*it = 2.34;
	TEST_REAL_EQUAL(*it, 2.34)
	it++;
	TEST_REAL_EQUAL(*it, 0)
RESULT

CHECK(Iterator end() throw())
	RegularData3D rg;
	RegularData3D::Iterator it = rg.end();
	TEST_EQUAL(it == rg.end(), true)
RESULT


CHECK(IndexType getLowerIndex(const CoordinateType& v) const)
  RegularData3D g(Vector3(5.0, 7.0, 9.0), Vector3(2.0, 3.0, 4.0), Vector3(1.0, 0.5, 0.5));
	TEST_EQUAL(g.getLowerIndex(Vector3(6.6, 8.76, 9.3)).x, 1)
	TEST_EQUAL(g.getLowerIndex(Vector3(6.6, 8.76, 9.3)).y, 3)
	TEST_EQUAL(g.getLowerIndex(Vector3(6.6, 8.76, 9.3)).z, 0)
	RegularData3D empty;
	TEST_EXCEPTION(Exception::OutOfGrid, empty.getLowerIndex(Vector3(6.4, 8.72, 1)))
RESULT

CHECK(IndexType())
	RegularData3D::IndexType* it = new RegularData3D::IndexType();
	TEST_NOT_EQUAL(it, 0)
	TEST_EQUAL(it->x, 0)
	TEST_EQUAL(it->y, 0)
	TEST_EQUAL(it->z, 0)
	delete it;
RESULT

CHECK(IndexType(Position p))
	RegularData3D::IndexType* it = new RegularData3D::IndexType(5);
	TEST_NOT_EQUAL(it, 0)
	TEST_EQUAL(it->x, 5)
	TEST_EQUAL(it->y, 5)
	TEST_EQUAL(it->z, 5)
  delete it;
RESULT

CHECK(IndexType(Position p, Position q, Position r))
	RegularData3D::IndexType* it = new RegularData3D::IndexType(1,2,3);
	TEST_NOT_EQUAL(it, 0)
	TEST_EQUAL(it->x, 1)
	TEST_EQUAL(it->y, 2)
	TEST_EQUAL(it->z, 3)
	delete it;
RESULT

CHECK(TRegularData3D(const TRegularData3D<ValueType>& grid))
  RegularData3D g(Vector3(1.0, 2.0, 3.0), Vector3(4.0, 5.0, 6.0), Vector3(0.1, 0.2, 0.3));
	g[0] = 1.23;
	RegularData3D g2(g);
	TEST_REAL_EQUAL(g2.getOrigin().x, 1.0)
	TEST_REAL_EQUAL(g2.getOrigin().y, 2.0)
	TEST_REAL_EQUAL(g2.getOrigin().z, 3.0)
	TEST_REAL_EQUAL(g2.getDimension().x, 4.0)
	TEST_REAL_EQUAL(g2.getDimension().y, 5.0)
	TEST_REAL_EQUAL(g2.getDimension().z, 6.0)
	TEST_REAL_EQUAL(g2.getSpacing().x, 0.1)
	TEST_REAL_EQUAL(g2.getSpacing().y, 0.2)
	TEST_REAL_EQUAL(g2.getSpacing().z, 0.3)
	TEST_REAL_EQUAL(g2[0], 1.23)
RESULT

CHECK(ValueType& getData(const IndexType& index))
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	TEST_REAL_EQUAL(g.getSize().x, 2.0)
	TEST_REAL_EQUAL(g.getSize().y, 2.0)
	TEST_REAL_EQUAL(g.getSize().z, 2.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0,0,0)), 0)
	g.getData(RegularData3D::IndexType(1,1,1)) = 1.23;
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1,1,1)), 1.23)
	TEST_REAL_EQUAL(g[7], 1.23)
	TEST_EXCEPTION(Exception::OutOfGrid, g.getData(RegularData3D::IndexType(2,1,1)));
RESULT

CHECK(const ValueType& getData(const IndexType& index) const)
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	g.getData(RegularData3D::IndexType(1,1,1)) = 1.23;
	const RegularData3D& rd = g;
	TEST_REAL_EQUAL(rd.getData(RegularData3D::IndexType(1,1,1)), 1.23)
	TEST_EXCEPTION(Exception::OutOfGrid, rd.getData(RegularData3D::IndexType(2,1,1)));
RESULT

CHECK(ValueType& operator [] (const IndexType& index) throw())
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	TEST_REAL_EQUAL(g[RegularData3D::IndexType(0,0,0)], 0)
	g[RegularData3D::IndexType(1,1,1)] = 1.23;
	TEST_REAL_EQUAL(g[RegularData3D::IndexType(1,1,1)], 1.23)
	TEST_REAL_EQUAL(g[7], 1.23)
	TEST_REAL_EQUAL(g.getSize().x, 2.0)
	TEST_REAL_EQUAL(g.getSize().y, 2.0)
	TEST_REAL_EQUAL(g.getSize().z, 2.0)
RESULT

CHECK(const ValueType& operator [] (const IndexType& index) const throw())
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	TEST_REAL_EQUAL(g[RegularData3D::IndexType(0,0,0)], 0)
	g[RegularData3D::IndexType(1,1,1)] = 1.23;
	const RegularData3D& d = g;
	TEST_REAL_EQUAL(d[RegularData3D::IndexType(1,1,1)], 1.23)
RESULT

CHECK(bool empty() const throw())
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	TEST_EQUAL(g.empty(), false)
	g[RegularData3D::IndexType(1,1,1)] = 1.23;
	TEST_EQUAL(g.empty(), false)
	RegularData3D empty;
	TEST_EQUAL(empty.empty(), true)
RESULT

CHECK(size_type max_size() const throw())
	RegularData3D g;
	TEST_EQUAL(g.max_size() > 1000, true)
RESULT

CHECK(void binaryWrite(const String& filename) const)
	NEW_TMP_FILE(filename)
  RegularData3D g(Vector3(1.0, 2.0, 3.0), Vector3(4.0, 5.0, 6.0), Vector3(0.1, 0.2, 0.3));
	for (Position x = 0; x < g.getSize().x; x++)
		for (Position y = 0; y < g.getSize().y; y++)
			for (Position z = 0; z < g.getSize().z; z++)
	{
		g[RegularData3D::IndexType(x,y,z)] = x * y * z;
	}

	g.binaryWrite(filename);
	TEST_EXCEPTION(Exception::FileNotFound, g.binaryWrite("/not/there/strange_file!"))
RESULT

CHECK(void binaryRead(const String& filename))
	RegularData3D g;
	RegularData3D empty;
	TEST_EQUAL(g == empty, true)
	TEST_EXCEPTION(Exception::FileNotFound, g.binaryRead("/not/there/strange_file!"))
	g.binaryRead(filename);

	for (Position x = 0; x < g.getSize().x; x++)
		for (Position y = 0; y < g.getSize().y; y++)
			for (Position z = 0; z < g.getSize().z; z++)
	{
		TEST_REAL_EQUAL(g[RegularData3D::IndexType(x,y,z)], x*y*z)
	}

	TEST_REAL_EQUAL(g.getOrigin().x, 1.0)
	TEST_REAL_EQUAL(g.getOrigin().y, 2.0)
	TEST_REAL_EQUAL(g.getOrigin().z, 3.0)
	TEST_REAL_EQUAL(g.getDimension().x, 4.0)
	TEST_REAL_EQUAL(g.getDimension().y, 5.0)
	TEST_REAL_EQUAL(g.getDimension().z, 6.0)
	TEST_REAL_EQUAL(g.getSpacing().x, 0.1)
	TEST_REAL_EQUAL(g.getSpacing().y, 0.2)
	TEST_REAL_EQUAL(g.getSpacing().z, 0.3)
RESULT

CHECK(void rescale(const IndexType& new_size))
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(2, 2, 2));
	g[RegularData3D::IndexType(0, 0, 0)] = 0.0;
	g[RegularData3D::IndexType(1, 0, 0)] = 0.0;
	g[RegularData3D::IndexType(0, 1, 0)] = 0.0;
	g[RegularData3D::IndexType(1, 1, 0)] = 0.0;
	g[RegularData3D::IndexType(0, 0, 1)] = 1.0;
	g[RegularData3D::IndexType(1, 0, 1)] = 1.0;
	g[RegularData3D::IndexType(0, 1, 1)] = 1.0;
	g[RegularData3D::IndexType(1, 1, 1)] = 1.0;
	STATUS(g.getSize().x << " x " << g.getSize().y << " x " << g.getSize().z)
	STATUS(g.getData(0))
	STATUS(g.getData(1))
	STATUS(g.getData(2))
	STATUS(g.getData(3))
	STATUS(g.getData(4))
	STATUS(g.getData(5))
	STATUS(g.getData(6))
	STATUS(g.getData(7))
	STATUS(g.getData(RegularData3D::IndexType(0, 0, 0)))
	STATUS(g.getData(RegularData3D::IndexType(1, 0, 0)))
	STATUS(g.getData(RegularData3D::IndexType(0, 1, 0)))
	STATUS(g.getData(RegularData3D::IndexType(1, 1, 0)))
	STATUS(g.getData(RegularData3D::IndexType(0, 0, 1)))
	STATUS(g.getData(RegularData3D::IndexType(1, 0, 1)))
	STATUS(g.getData(RegularData3D::IndexType(0, 1, 1)))
	STATUS(g.getData(RegularData3D::IndexType(1, 1, 1)))
	g.rescale(RegularData3D::IndexType(3, 3, 3));
	STATUS(g.getSize().x << " x " << g.getSize().y << " x " << g.getSize().z)
	STATUS(g.getData(RegularData3D::IndexType(0, 0, 0)))
	STATUS(g.getData(RegularData3D::IndexType(1, 0, 0)))
	STATUS(g.getData(RegularData3D::IndexType(2, 0, 0)))
	STATUS(g.getData(RegularData3D::IndexType(0, 1, 0)))
	STATUS(g.getData(RegularData3D::IndexType(1, 1, 0)))
	STATUS(g.getData(RegularData3D::IndexType(2, 1, 0)))
	STATUS(g.getData(RegularData3D::IndexType(0, 2, 0)))
	STATUS(g.getData(RegularData3D::IndexType(1, 2, 0)))
	STATUS(g.getData(RegularData3D::IndexType(2, 2, 0)))

	STATUS(g.getData(RegularData3D::IndexType(0, 0, 1)))
	STATUS(g.getData(RegularData3D::IndexType(1, 0, 1)))
	STATUS(g.getData(RegularData3D::IndexType(2, 0, 1)))
	STATUS(g.getData(RegularData3D::IndexType(0, 1, 1)))
	STATUS(g.getData(RegularData3D::IndexType(1, 1, 1)))
	STATUS(g.getData(RegularData3D::IndexType(2, 1, 1)))
	STATUS(g.getData(RegularData3D::IndexType(0, 2, 1)))
	STATUS(g.getData(RegularData3D::IndexType(1, 2, 1)))
	STATUS(g.getData(RegularData3D::IndexType(2, 2, 1)))

	STATUS(g.getData(RegularData3D::IndexType(0, 0, 2)))
	STATUS(g.getData(RegularData3D::IndexType(1, 0, 2)))
	STATUS(g.getData(RegularData3D::IndexType(2, 0, 2)))
	STATUS(g.getData(RegularData3D::IndexType(0, 1, 2)))
	STATUS(g.getData(RegularData3D::IndexType(1, 1, 2)))
	STATUS(g.getData(RegularData3D::IndexType(2, 1, 2)))
	STATUS(g.getData(RegularData3D::IndexType(0, 2, 2)))
	STATUS(g.getData(RegularData3D::IndexType(1, 2, 2)))
	STATUS(g.getData(RegularData3D::IndexType(2, 2, 2)))

	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 0, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 0, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 0, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 1, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 1, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 1, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 2, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 2, 0)), 0.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 2, 0)), 0.0)

	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 0, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 0, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 0, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 1, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 1, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 1, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 2, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 2, 1)), 0.5)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 2, 1)), 0.5)

	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 0, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 0, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 0, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 1, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 1, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 1, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(0, 2, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(1, 2, 2)), 1.0)
	TEST_REAL_EQUAL(g.getData(RegularData3D::IndexType(2, 2, 2)), 1.0)
RESULT

CHECK(void setDimension(const CoordinateType& dimension) throw())
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	g.setDimension(Vector3(2.0,3.0,4.0));
	TEST_EQUAL(g.getDimension(), Vector3(2.0, 3.0, 4.0))
	TEST_REAL_EQUAL(g.getSpacing().x, 2.0)
	TEST_REAL_EQUAL(g.getSpacing().y, 3.0)
	TEST_REAL_EQUAL(g.getSpacing().z, 4.0)
RESULT

CHECK(void swap(TRegularData3D<ValueType>& grid))
  RegularData3D h(Vector3(1.0, 2.0, 3.0), Vector3(4.0, 5.0, 6.0), Vector3(0.1, 0.2, 0.3));
	h[RegularData3D::IndexType(0,0,0)] = 1.0;
	RegularData3D h2(h);
  RegularData3D g(Vector3(1.1, 2.1, 3.1), Vector3(4.1, 5.1, 6.1), Vector3(1.1, 1.2, 1.3));
	g[RegularData3D::IndexType(0,0,0)] = 1.1;
	RegularData3D g2(g);

	g.swap(h);
	TEST_EQUAL(g2 == h, true)
	TEST_EQUAL(h2 == g, true)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
