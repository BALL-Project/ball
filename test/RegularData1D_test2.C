// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DATATYPE/regularData1D.h>

///////////////////////////

START_TEST(RegularData1D)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

CHECK(void rescale(const IndexType& new_size))
	TRegularData1D<float>	data;
	TEST_EQUAL(data.getSize(), 0)
	data.rescale(1);
	TEST_EQUAL(data.getSize(), 1)
	// the contents if data[0] are now undetermined!
	data[0] = 2.0;
	data.rescale(2);
	TEST_EQUAL(data.getSize(), 2)
	TEST_EQUAL(data[0], 2.0)
	TEST_EQUAL(data[0], 2.0)
	data[0] = 1.0;
	data.rescale(3);
	TEST_EQUAL(data.getSize(), 3)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.5)
	TEST_REAL_EQUAL(data[2], 2.0)
	data.rescale(4);
	TEST_EQUAL(data.getSize(), 4)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.3333333)
	TEST_REAL_EQUAL(data[2], 1.6666667)
	TEST_REAL_EQUAL(data[3], 2.0)

{
	TRegularData1D<float>	data;
	data.resize(2);
	data.setOrigin(0.0);
	data.setDimension(1.0);
	data[0] = 1.0;
	data[1] = 2.0;
	data.rescale(3);
	TEST_EQUAL(data.getSize(), 3)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.5)
	TEST_REAL_EQUAL(data[2], 2.0)
	data.rescale(4);
	TEST_EQUAL(data.getSize(), 4)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.3333333)
	TEST_REAL_EQUAL(data[2], 1.6666667)
	TEST_REAL_EQUAL(data[3], 2.0)
	data.rescale(4);
	TEST_EQUAL(data.getSize(), 4)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.3333333)
	TEST_REAL_EQUAL(data[2], 1.6666667)
	TEST_REAL_EQUAL(data[3], 2.0)
	data.rescale(10);
	TEST_EQUAL(data.getSize(), 10)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.1111111)
	TEST_REAL_EQUAL(data[2], 1.2222222)
	TEST_REAL_EQUAL(data[3], 1.3333333)
	TEST_REAL_EQUAL(data[4], 1.4444444)
	TEST_REAL_EQUAL(data[5], 1.5555556)
	TEST_REAL_EQUAL(data[6], 1.6666667)
	TEST_REAL_EQUAL(data[7], 1.7777778)
	TEST_REAL_EQUAL(data[8], 1.8888889)
	TEST_REAL_EQUAL(data[9], 2.0)
	data.rescale(3);
	TEST_EQUAL(data.getSize(), 3)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.5)
	TEST_REAL_EQUAL(data[2], 2.0)
}
RESULT

RegularData1D::VectorType v;
v.push_back(1.1);
v.push_back(1.2);
v.push_back(1.3);
v.push_back(1.4);

RegularData1D rd_it(v);

CHECK(ConstIterator begin() const throw())
	TEST_REAL_EQUAL(*(rd_it.begin()), 1.1);
RESULT

CHECK(ConstIterator end() const throw())
	RegularData1D::ConstIterator it(rd_it.end());
	--it;
	TEST_REAL_EQUAL(*it, 1.4);
RESULT

CHECK(Iterator begin() throw())
	TEST_REAL_EQUAL(*(rd_it.begin()), 1.1);
RESULT

CHECK(Iterator end() throw())
	RegularData1D::Iterator it(rd_it.end());
	--it;
	TEST_REAL_EQUAL(*it, 1.4);
RESULT

CHECK(CoordinateType getCoordinates(const IndexType& index) const)
	RegularData1D rd(0.0, 2.0, 0.5);

	TEST_REAL_EQUAL(rd.getCoordinates(0), 0.0);
	TEST_REAL_EQUAL(rd.getCoordinates(1), 0.5);
	TEST_REAL_EQUAL(rd.getCoordinates(4), 2.0);
RESULT

CHECK(IndexType getClosestIndex(const CoordinateType& x) const)
 	RegularData1D rd(0.0, 2.0, 0.5);

	TEST_EQUAL(rd.getClosestIndex(0.0), 0);
	TEST_EQUAL(rd.getClosestIndex(0.1), 0);
	TEST_EQUAL(rd.getClosestIndex(0.26), 1);
	TEST_EQUAL(rd.getClosestIndex(0.6), 1);
RESULT

CHECK(IndexType getLowerIndex(const CoordinateType& x) const)
 	RegularData1D rd(0.0, 2.0, 0.5);

	TEST_EQUAL(rd.getLowerIndex(0.0), 0);
	TEST_EQUAL(rd.getLowerIndex(0.1), 0);
	TEST_EQUAL(rd.getLowerIndex(0.26), 0);
	TEST_EQUAL(rd.getLowerIndex(0.6), 1);
RESULT

CHECK(ValueType getInterpolatedValue(const CoordinateType& x) const)
	RegularData1D::VectorType v;
	v.push_back(0.0);
	v.push_back(0.5);
	v.push_back(1.0);
	RegularData1D rd(v, 0.0, 1.0);

	TEST_REAL_EQUAL(rd.getInterpolatedValue(0.0), 0.0);
	TEST_REAL_EQUAL(rd.getInterpolatedValue(0.5), 0.5);
	TEST_REAL_EQUAL(rd.getInterpolatedValue(1.0), 1.0);
	TEST_REAL_EQUAL(rd.getInterpolatedValue(0.2), 0.2);
	TEST_REAL_EQUAL(rd.getInterpolatedValue(0.6), 0.6);
	TEST_REAL_EQUAL(rd.getInterpolatedValue(0.9), 0.9);
RESULT

CHECK(ValueType operator () (const CoordinateType& x) const throw())
	RegularData1D::VectorType v;
	v.push_back(0.0);
	v.push_back(0.5);
	v.push_back(1.0);
	RegularData1D rd(v, 0.0, 1.0);

	TEST_REAL_EQUAL(rd(0.0), 0.0)
	TEST_REAL_EQUAL(rd(0.5), 0.5)
	TEST_REAL_EQUAL(rd(1.0), 1.0)
	TEST_REAL_EQUAL(rd(0.2), 0.2)
	TEST_REAL_EQUAL(rd(0.6), 0.6)
	TEST_REAL_EQUAL(rd(0.9), 0.9)
RESULT

CHECK(ValueType& getClosestValue(const CoordinateType& x))
	RegularData1D::VectorType v;
	v.push_back(0.0);
	v.push_back(0.5);
	v.push_back(1.0);
	RegularData1D rd(v, 0.0, 1.0);

	TEST_REAL_EQUAL(rd.getClosestValue(0.0), 0.0)
	TEST_REAL_EQUAL(rd.getClosestValue(0.4), 0.5)
	TEST_REAL_EQUAL(rd.getClosestValue(0.6), 0.5)
	TEST_REAL_EQUAL(rd.getClosestValue(0.9), 1.0)
RESULT

CHECK(const ValueType& getClosestValue(const CoordinateType& x) const)
 	RegularData1D::VectorType v;
	v.push_back(0.0);
	v.push_back(0.5);
	v.push_back(1.0);
	const RegularData1D rd(v, 0.0, 1.0);

	TEST_REAL_EQUAL(rd.getClosestValue(0.0), 0.0)
	TEST_REAL_EQUAL(rd.getClosestValue(0.4), 0.5)
	TEST_REAL_EQUAL(rd.getClosestValue(0.6), 0.5)
	TEST_REAL_EQUAL(rd.getClosestValue(0.9), 1.0)
RESULT

CHECK(bool empty() const throw())
	RegularData1D rd(0);

	TEST_EQUAL(rd.empty(), true);
RESULT

CHECK(bool isInside(const CoordinateType& x) const throw())
	RegularData1D rd(0.0, 1.0, 0.5);

	TEST_EQUAL(rd.isInside(-0.01), false);
	TEST_EQUAL(rd.isInside(0.2), true);
	TEST_EQUAL(rd.isInside(1.1), false);
RESULT

CHECK(bool operator != (const TRegularData1D& data) const throw())
	RegularData1D rd(0.0, 1.0, 0.5);
	RegularData1D rd2(0.2, 1.0, 0.5);

	TEST_EQUAL(rd != rd2, true);
	TEST_EQUAL(rd != rd, false);
RESULT

CHECK(const CoordinateType& getSpacing() const throw())
 	RegularData1D rd(0.0, 1.0, 0.5);

	TEST_REAL_EQUAL(rd.getSpacing(), 0.5);	
RESULT

CHECK(size_type max_size() const throw())
  RegularData1D::VectorType v;
	RegularData1D rd(v);

	TEST_EQUAL(rd.max_size(), v.max_size());
RESULT

CHECK(size_type size() const throw())
  RegularData1D::VectorType v(5);
	RegularData1D rd(v);

	TEST_EQUAL(rd.size(), v.size());
RESULT

CHECK(void binaryRead(const String& filename) throw())
  // ???
RESULT

CHECK(void binaryWrite(const String& filename) const throw())
  // ???
RESULT

CHECK(void getEnclosingIndices(const CoordinateType& x, Position& lower, Position& upper) const)
  // ???
RESULT

CHECK(void getEnclosingValues(const CoordinateType& x, ValueType& lower, ValueType& upper) const)
  // ???
RESULT

CHECK(void swap(TRegularData1D<ValueType>& data) throw())
  // ???
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
