// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RegularData1D_test.C,v 1.15 2003/06/12 14:12:32 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/regularData1D.h>
#include <BALL/COMMON/limits.h>

///////////////////////////

START_TEST(RegularData1D, "$Id: RegularData1D_test.C,v 1.15 2003/06/12 14:12:32 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

RegularData1D* rd_ptr;
CHECK(TRegularData1D() throw())
	rd_ptr = new RegularData1D;
	TEST_NOT_EQUAL(rd_ptr, 0)
RESULT


CHECK(~TRegularData1D() throw())
  delete rd_ptr;
RESULT

CHECK(BALL_CREATE(TRegularData1D<ValueType>))
	vector<float> v;
	v.push_back(1.1);
	RegularData1D a(v, 1.1, 1.2); 
	RegularData1D* v_ptr = (RegularData1D*)a.create(false, true);
	TEST_EQUAL(v_ptr->getSize(), 0)
	TEST_REAL_EQUAL(v_ptr->getOrigin(), 0.0)
	delete v_ptr;
	v_ptr = (RegularData1D*)a.create();
	TEST_EQUAL(v_ptr->getSize(), 1)
	TEST_REAL_EQUAL(v_ptr->getOrigin(), 1.1)
	TEST_REAL_EQUAL(v_ptr->getDimension(), 1.2)
	delete v_ptr;
RESULT
		

RegularData1D rd;
CHECK(void setOrigin(const CoordinateType& origin) throw())
	rd.resize(2);
	rd.setOrigin(1.1);
	rd.setDimension(1.0);
	TEST_REAL_EQUAL(rd.getOrigin(), 1.1)
	TEST_REAL_EQUAL(rd.getDimension(), 1.0)
RESULT


CHECK(void setDimension(const CoordinateType& dimension) throw())
	rd.setDimension(2.2);
	TEST_REAL_EQUAL(rd.getOrigin(), 1.1)
	TEST_REAL_EQUAL(rd.getDimension(), 2.2)
	rd.setDimension(0.0);
	TEST_REAL_EQUAL(rd.getDimension(), 0.0)
RESULT


RegularData1D::VectorType v;
v.push_back(1.1);
v.push_back(1.2);
v.push_back(1.3);
v.push_back(1.4);

CHECK(TRegularData1D(const VectorType& data, const CoordinateType& origin = 0.0, const CoordinateType& dimension = 1.0) throw(Exception::OutOfMemory))
	RegularData1D rd2(v, 1.0, 0.5);
	TEST_REAL_EQUAL(rd2.getOrigin(), 1.0)
	TEST_REAL_EQUAL(rd2.getDimension(), 0.5)
	TEST_EQUAL(rd2.getSize(), 4)
	TEST_REAL_EQUAL(rd2[0], 1.1)
	TEST_REAL_EQUAL(rd2[3], 1.4)

	RegularData1D rd3(v);
	TEST_REAL_EQUAL(rd3.getOrigin(), 0.0)
	TEST_REAL_EQUAL(rd3.getDimension(), 1.0)
	TEST_EQUAL(rd3.getSize(), 4)

	RegularData1D rd4(v, 2.0, 1.0);
	TEST_REAL_EQUAL(rd4.getOrigin(), 2.0)
	TEST_REAL_EQUAL(rd4.getDimension(), 1.0)
	TEST_EQUAL(rd4.getSize(), 4)
RESULT


CHECK(void clear() throw())
	rd.resize(10);
	rd.setOrigin(0.0);
	rd.setDimension(2.0);
	rd[0] = 1.0;
	rd[9] = 9.0;
	TEST_REAL_EQUAL(rd[0], 1.0)
	TEST_REAL_EQUAL(rd[9], 9.0)
	TEST_REAL_EQUAL(rd.getOrigin(), 0.0)
	TEST_REAL_EQUAL(rd.getDimension(), 2.0)
	TEST_EQUAL(rd.getSize(), 10)
	rd.clear();
	TEST_REAL_EQUAL(rd[0], 0.0)
	TEST_REAL_EQUAL(rd[9], 0.0)
	TEST_REAL_EQUAL(rd.getOrigin(), 0.0)
	TEST_REAL_EQUAL(rd.getDimension(), 2.0)
	TEST_EQUAL(rd.getSize(), 10)
RESULT


CHECK(const TRegularData1D& operator = (const TRegularData1D<ValueType>& data) throw(Exception::OutOfMemory))
	rd.resize(2);
	rd.setOrigin(1.1);
	rd.setDimension(2.1);
	rd[0] = 1.2;
	rd[1] = 2.3;
	RegularData1D rd2 = rd;
	TEST_REAL_EQUAL(rd2.getOrigin(), 1.1)
	TEST_REAL_EQUAL(rd2.getDimension(), 2.1)
	TEST_REAL_EQUAL(rd2[0], 1.2) 
	TEST_REAL_EQUAL(rd2[1], 2.3) 
	TEST_EQUAL(rd2.getSize(), 2)
RESULT


CHECK(const TRegularData1D& operator = (const VectorType& data) throw(Exception::OutOfMemory))
	RegularData1D::VectorType v;
	v.push_back(1.1);
	v.push_back(1.2);
	v.push_back(1.3);
	v.push_back(1.4);
	RegularData1D rd2;
	rd2.setOrigin(0.0);
	rd2.setDimension(1.0);
	rd2 = v;
	TEST_REAL_EQUAL(rd2.getOrigin(), 0.0)
	TEST_REAL_EQUAL(rd2.getDimension(), 1.0)
	TEST_EQUAL(rd2.getSize(), 4)
	TEST_REAL_EQUAL(rd2[0], 1.1)
	TEST_REAL_EQUAL(rd2[1], 1.2)
	TEST_REAL_EQUAL(rd2[2], 1.3)
	TEST_REAL_EQUAL(rd2[3], 1.4)
RESULT


CHECK(bool operator == (const TRegularData1D& data) const throw())
	rd.clear();
	rd.resize(4);
	rd[0] = 1.1;
	rd[1] = 1.2;
	rd[2] = 1.3;
	rd[3] = 1.4;
	RegularData1D rd2 = rd;
	TEST_EQUAL(rd == rd2, true)
	rd2[3] = 1.41;
	TEST_EQUAL(rd == rd2, false)
	rd2[3] = 1.4;
	TEST_EQUAL(rd == rd2, true)
	rd.setOrigin(1.1);
	rd.setDimension(1.4);
	TEST_EQUAL(rd == rd2, false)
	rd2.setOrigin(1.1);
	rd2.setDimension(1.4);
	TEST_EQUAL(rd == rd2, true)
RESULT


CHECK(const ValueType& operator [] (const IndexType& index) const throw())
  TEST_REAL_EQUAL(rd[0], 1.1)
  TEST_REAL_EQUAL(rd[1], 1.2)
  TEST_REAL_EQUAL(rd[2], 1.3)
  TEST_REAL_EQUAL(rd[3], 1.4)
RESULT

CHECK(const ValueType& getData(const IndexType& index) const throw(Exception::OutOfGrid))
	TEST_REAL_EQUAL(rd.getData(0), 1.1)
	TEST_REAL_EQUAL(rd.getData(1), 1.2)
	TEST_REAL_EQUAL(rd.getData(2), 1.3)
	TEST_REAL_EQUAL(rd.getData(3), 1.4)
	TEST_EXCEPTION(Exception::OutOfGrid, rd.getData(4))
RESULT


CHECK(ValueType& operator [] (const IndexType& index) throw())
  rd[3] = 2.1;
	TEST_REAL_EQUAL(rd[3], 2.1)
  rd[3] = 2.2;
	TEST_REAL_EQUAL(rd[3], 2.2)
  rd[3] = 2.3;
	TEST_REAL_EQUAL(rd[3], 2.3)
  rd[3] = 2.4;
	TEST_REAL_EQUAL(rd[3], 2.4)
RESULT


CHECK(ValueType& getData(const IndexType& index) throw(Exception::OutOfGrid))
	rd.getData(0) = 3.1;
	TEST_REAL_EQUAL(rd.getData(0), 3.1)
	rd.getData(1) = 3.2;
	TEST_REAL_EQUAL(rd.getData(1), 3.2)
	rd.getData(2) = 3.3;
	TEST_REAL_EQUAL(rd.getData(2), 3.3)
	rd.getData(3) = 3.4;
	TEST_REAL_EQUAL(rd.getData(3), 3.4)
	TEST_EXCEPTION(Exception::OutOfGrid, rd.getData(4) = 1.0)
RESULT


RegularData1D rd2;
CHECK(IndexType getSize() const throw())
  TEST_EQUAL(rd.getSize(), 4)
	TEST_EQUAL(rd2.getSize(), 0)
RESULT


CHECK(const CoordinateType& getOrigin() const throw())
	rd.setOrigin(1.1);
	rd.setDimension(1.5);
	TEST_REAL_EQUAL(rd.getOrigin(), 1.1)
	TEST_REAL_EQUAL(rd2.getOrigin(), 0.0)
RESULT


CHECK(const CoordinateType& getDimension() const throw())
	TEST_REAL_EQUAL(rd.getDimension(), 1.5)
	TEST_REAL_EQUAL(rd2.getDimension(), 0.0)
RESULT


CHECK(TRegularData1D::setOrigin())
	rd.setOrigin(-99.9);
	rd.setDimension(99.9);
	TEST_REAL_EQUAL(rd.getOrigin(), -99.9)
	TEST_REAL_EQUAL(rd.getDimension(), 99.9)
	rd.setOrigin(99.9);
	rd.setDimension(-99.9);
	TEST_REAL_EQUAL(rd.getOrigin(), 99.9)
	TEST_REAL_EQUAL(rd.getDimension(), -99.9)
RESULT


CHECK(void resize(const IndexType& size) throw(Exception::OutOfMemory))
	TEST_EQUAL(rd.getSize(), 4)
	rd.resize(99);
	TEST_EQUAL(rd.getSize(), 99)
	TEST_REAL_EQUAL(rd[98], 0.0)
	rd.resize(3);
	TEST_EQUAL(rd.getSize(), 3)
	TEST_REAL_EQUAL(rd[2], 3.3)
RESULT

CHECK(void rescale(const IndexType& new_size) throw(Exception::OutOfMemory))
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
RESULT

CHECK(TRegularData1D::rescale(double origin, double dimension, Size new_size))
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
RESULT


CHECK(ConstIterator begin() const throw())
  // ???
RESULT

CHECK(ConstIterator end() const throw())
  // ???
RESULT

CHECK(CoordinateType getCoordinates(const IndexType& index) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(IndexType getClosestIndex(const CoordinateType& x) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(IndexType getLowerIndex(const CoordinateType& x) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(Iterator begin() throw())
  // ???
RESULT

CHECK(Iterator end() throw())
  // ???
RESULT

CHECK(TRegularData1D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing) throw(Exception::OutOfMemory))
  // ???
RESULT

CHECK(TRegularData1D(const IndexType& size, const CoordinateType& origin = CoordinateType(0.0), const CoordinateType& dimension = CoordinateType(1.0)) throw(Exception::OutOfMemory))
  // ???
RESULT

CHECK(TRegularData1D(const TRegularData1D& data) throw(Exception::OutOfMemory))
  // ???
RESULT

CHECK(ValueType getInterpolatedValue(const CoordinateType& x) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(ValueType operator () (const CoordinateType& x) const throw())
  // ???
RESULT

CHECK(ValueType& getClosestValue(const CoordinateType& x) throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(bool empty() const throw())
  // ???
RESULT

CHECK(bool isInside(const CoordinateType& x) const throw())
  // ???
RESULT

CHECK(bool operator != (const TRegularData1D& data) const throw())
  // ???
RESULT

CHECK(const CoordinateType& getSpacing() const throw())
  // ???
RESULT

CHECK(const ValueType& getClosestValue(const CoordinateType& x) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(size_type max_size() const throw())
  // ???
RESULT

CHECK(size_type size() const throw())
  // ???
RESULT

CHECK(void binaryRead(const String& filename) throw())
  // ???
RESULT

CHECK(void binaryWrite(const String& filename) const throw())
  // ???
RESULT

CHECK(void getEnclosingIndices(const CoordinateType& x, Position& lower, Position& upper) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(void getEnclosingValues(const CoordinateType& x, ValueType& lower, ValueType& upper) const throw(Exception::OutOfGrid))
  // ???
RESULT

CHECK(void swap(TRegularData1D<ValueType>& data) throw())
  // ???
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
