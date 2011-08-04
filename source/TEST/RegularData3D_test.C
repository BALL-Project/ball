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

CHECK(TRegularData3D() throw())
	grid = new RegularData3D();
	TEST_NOT_EQUAL(grid, 0)
RESULT

CHECK(~TRegularData3D() throw())
	delete grid;
RESULT

CHECK(TRegularData3D(const IndexType& size, const CoordinateType& origin = CoordinateType(0.0), const CoordinateType& dimension = CoordinateType(1.0)) throw(Exception::OutOfMemory))
	grid = new RegularData3D(RegularData3D::IndexType(11, 11, 11), Vector3(0.0, 0.0, 0.0), Vector3(10.0, 10.0, 10.0));
	TEST_NOT_EQUAL(grid, 0)
	TEST_EQUAL(grid->size(), 1331)
	delete grid;
RESULT

using BALL::Vector3;
Vector3	lower(0.0, 0.0, 0.0);
Vector3	upper(10.0, 10.0, 10.0);

CHECK(TRegularData3D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing) throw(Exception::OutOfMemory))
	grid = new RegularData3D(lower, upper - lower, Vector3(1.0));
	TEST_NOT_EQUAL(grid, 0)
	TEST_EQUAL(grid->size(), 1331)
	delete grid;
	grid = new RegularData3D(lower, upper - lower, Vector3(1.0));
RESULT

CHECK(size_type size() const throw())
	TEST_EQUAL(grid->size(), 1331)
RESULT

RegularData3D g(RegularData3D::IndexType(11, 11, 11), lower, upper - lower);

CHECK(void resize(const IndexType& size) throw(Exception::OutOfMemory))
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

CHECK(TRegularData3D& operator = (const TRegularData3D<ValueType>& data) throw(Exception::OutOfMemory))
	RegularData3D g1;
	g1 = g;
	TEST_EQUAL(g1.size(), 1331)
	TEST_EQUAL((g1 == g), true)
RESULT

CHECK(const IndexType& getSize() const throw())
	TEST_EQUAL(grid->getSize().x, 11)
	TEST_EQUAL(grid->getSize().y, 11)
	TEST_EQUAL(grid->getSize().z, 11)
RESULT

CHECK(const CoordinateType& getSpacing() const throw())
	TEST_REAL_EQUAL(grid->getSpacing().x, 1.0)
	TEST_REAL_EQUAL(grid->getSpacing().y, 1.0)
	TEST_REAL_EQUAL(grid->getSpacing().z, 1.0)
RESULT

RegularData3D::IndexType	index;

CHECK(IndexType getClosestIndex(const CoordinateType& v) const throw(Exception::OutOfGrid))
 	lower.set(3.49, 3.51, 3.0);
	index = grid->getClosestIndex(lower);
	TEST_EQUAL(index.x, 3)
	TEST_EQUAL(index.y, 4)
	TEST_EQUAL(index.z, 3)
	TEST_EXCEPTION(Exception::OutOfGrid, g.getClosestIndex(Vector3(1111, 1,1)))
RESULT

CHECK(ValueType& getClosestValue(const CoordinateType& x) throw(Exception::OutOfGrid))
	g.getClosestValue(Vector3(1.2, 2.3, 4.3)) = 1.232;
	TEST_REAL_EQUAL(g[RegularData3D::IndexType(1,2,4)], 1.232)
	TEST_EXCEPTION(Exception::OutOfGrid, g.getClosestValue(Vector3(1111, 1,1)))
RESULT

CHECK(const ValueType& getClosestValue(const CoordinateType& x) const throw(Exception::OutOfGrid))
	const RegularData3D& rd = g;
	TEST_REAL_EQUAL(rd.getClosestValue(Vector3(1.2, 2.3, 4.3)), 1.232)
	TEST_EXCEPTION(Exception::OutOfGrid, rd.getClosestValue(Vector3(1111, 1,1)))
RESULT

CHECK(ValueType& getData(Position index) throw(Exception::OutOfGrid))
	grid->getData(RegularData3D::IndexType(0, 0, 0)) = 5.4321;		
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(grid->getData(0), 5.4321);
	TEST_REAL_EQUAL(grid->getData(0), grid->getData(grid->getClosestIndex(lower)));
	TEST_EXCEPTION(Exception::OutOfGrid, grid->getData(1331))
RESULT

CHECK(const ValueType& getData(Position index) const throw(Exception::OutOfGrid))
  RegularData3D g(Vector3(0.0, 0.0, 0.0), Vector3(1.0, 1.0, 1.0), Vector3(1, 1, 1));
	g.getData(1) = 1.23;
	const RegularData3D& d = g;
	TEST_REAL_EQUAL(d.getData(1), 1.23)
	TEST_EXCEPTION(Exception::OutOfGrid, d.getData(8))
RESULT


CHECK(ValueType& operator [] (Position index) throw())
	(*grid)[3 + 11 * 3 + 11 * 11 * 3] = 1.2345;
RESULT

CHECK(const ValueType& operator [] (Position index) const throw())
	lower.set(3.0, 3.0, 3.0);
	const RegularData3D& const_grid = *grid;
	TEST_EQUAL(const_grid[3 + 11 * 3 + 11 * 11 * 3], (*grid)[grid->getClosestIndex(lower)]);
RESULT


CHECK(CoordinateType getCoordinates(Position index) const throw(Exception::OutOfGrid))
	lower = grid->getCoordinates(0);
	TEST_REAL_EQUAL(lower.x, 0.0)
	TEST_REAL_EQUAL(lower.y, 0.0)
	TEST_REAL_EQUAL(lower.z, 0.0)

	lower = grid->getCoordinates(2 + 2 * 11 + 2 * 11 * 11);
	TEST_REAL_EQUAL(lower.x, 2.0)
	TEST_REAL_EQUAL(lower.y, 2.0)
	TEST_REAL_EQUAL(lower.z, 2.0)
RESULT

CHECK(CoordinateType getCoordinates(const IndexType& index) const throw(Exception::OutOfGrid))
	upper.set(3.999999, 4.0, 3.0001);
	lower = grid->getCoordinates(grid->getClosestIndex(upper));
	TEST_REAL_EQUAL(lower.x, 4.0)
	TEST_REAL_EQUAL(lower.y, 4.0)
	TEST_REAL_EQUAL(lower.z, 3.0)
RESULT

CHECK(void getEnclosingIndices(const CoordinateType& r, Position& llf, Position& rlf, Position& luf, Position& ruf, Position& llb, Position& rlb, Position& lub, Position& rub) const throw(Exception::OutOfGrid))
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


CHECK(void getEnclosingValues(const CoordinateType& r, ValueType& llf, ValueType& rlf, ValueType& luf, ValueType& ruf, ValueType& llb, ValueType& rlb, ValueType& lub, ValueType& rub) const throw(Exception::OutOfGrid))
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

CHECK(const CoordinateType& getOrigin() const throw())
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 0.0)
	TEST_REAL_EQUAL(lower.y, 0.0)
	TEST_REAL_EQUAL(lower.z, 0.0)
RESULT

CHECK(void setOrigin(const CoordinateType& origin) throw())
	grid->setOrigin(Vector3(1.0, 1.0, 1.0));
	lower = grid->getOrigin();
	TEST_REAL_EQUAL(lower.x, 1.0)
	TEST_REAL_EQUAL(lower.y, 1.0)
	TEST_REAL_EQUAL(lower.z, 1.0)
	index = grid->getClosestIndex(Vector3(3.49, 3.51, 3.0));
	TEST_EQUAL(index.x, 2)
	TEST_EQUAL(index.y, 3)
	TEST_EQUAL(index.z, 2)

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

CHECK(const CoordinateType& getDimension() const throw())
	TEST_REAL_EQUAL(grid->getDimension().x, 10.0)
	TEST_REAL_EQUAL(grid->getDimension().y, 10.0)
	TEST_REAL_EQUAL(grid->getDimension().z, 10.0)
RESULT

CHECK(ValueType getInterpolatedValue(const CoordinateType& x) const throw(Exception::OutOfGrid))
	lower.set(2, 2, 2);
	TEST_EQUAL(g.getInterpolatedValue(lower), 1)
	lower.set(0, 0, 0);
	TEST_EQUAL(g.getInterpolatedValue(lower), 0)
	lower.set(3, 3, 3);
	TEST_EQUAL(g.getInterpolatedValue(lower), 8)
	lower.set(10.1, 0, 0);
	TEST_EXCEPTION(Exception::OutOfGrid, g.getInterpolatedValue(lower))
RESULT

CHECK(ValueType operator () (const CoordinateType& x) const throw())
	lower.set(2, 2, 2);
	TEST_EQUAL(g(lower), 1)
	lower.set(0, 0, 0);
	TEST_EQUAL(g(lower), 0)
	lower.set(3, 3, 3);
	TEST_EQUAL(g(lower), 8)
	lower.set(10.1, 0, 0);
RESULT

RegularData3D grid2 = *grid;

CHECK(void clear() throw())
	grid2.clear();
	TEST_EQUAL(grid2.getSize().x, 0)
	TEST_EQUAL(grid2.getSize().y, 0)
	TEST_EQUAL(grid2.getSize().z, 0)
RESULT

CHECK(bool operator == (const TRegularData3D<ValueType>& grid) const throw())
	grid2 = *grid;
	TEST_EQUAL(*grid == grid2, true)
	grid2[5] = -99.9;
	TEST_EQUAL(*grid == grid2, false)
RESULT

CHECK(bool operator != (const TRegularData3D<ValueType>& grid) const throw())
	TEST_EQUAL(*grid != grid2, true)
	grid2[5] = (*grid)[5];
	TEST_EQUAL(*grid != grid2, false)
RESULT
delete grid;



CHECK(bool isInside(const CoordinateType& r) const throw())
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

CHECK([EXTRA]operator << (ostream& os, const RegularData3D&))
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
		TEST_REAL_EQUAL(data[i], in_data[i])
	}
RESULT

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


CHECK(IndexType getLowerIndex(const CoordinateType& v) const throw(Exception::OutOfGrid))
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

CHECK(TRegularData3D(const TRegularData3D<ValueType>& grid) throw(Exception::OutOfMemory))
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

CHECK(ValueType& getData(const IndexType& index) throw(Exception::OutOfGrid))
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

CHECK(const ValueType& getData(const IndexType& index) const throw(Exception::OutOfGrid))
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

CHECK(void binaryWrite(const String& filename) const throw(Exception::FileNotFound))
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

CHECK(void binaryRead(const String& filename) throw(Exception::FileNotFound))
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

CHECK(void rescale(const IndexType& new_size) throw(Exception::OutOfMemory))
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
