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

RegularData1D* rd_ptr;
CHECK(TRegularData1D() throw())
	rd_ptr = new RegularData1D;
	TEST_NOT_EQUAL(rd_ptr, 0)
RESULT

CHECK(~TRegularData1D() throw())
  delete rd_ptr;
RESULT

CHECK(TRegularData1D(const CoordinateType& origin, const CoordinateType& dimension, const CoordinateType& spacing))
	RegularData1D rd(2.0, 4.0, 0.5);
	TEST_REAL_EQUAL(rd.getOrigin(), 2.0);
	TEST_REAL_EQUAL(rd.getDimension(), 4.0);
	TEST_REAL_EQUAL(rd.getSpacing(), 0.5);
	TEST_EQUAL(rd.getSize(), 9);
RESULT

CHECK(TRegularData1D(const TRegularData1D& data))
	RegularData1D rd(2.0, 4.0, 0.5);
	RegularData1D rd2(rd);

	TEST_REAL_EQUAL(rd2.getOrigin(), 2.0);
	TEST_REAL_EQUAL(rd2.getDimension(), 4.0);
	TEST_REAL_EQUAL(rd2.getSpacing(), 0.5);
	TEST_EQUAL(rd2.getSize(), 9);
RESULT

CHECK(TRegularData1D(const IndexType& size))
	RegularData1D rd(5);
	TEST_EQUAL(rd.getSize(), 5);
	TEST_REAL_EQUAL(rd.getOrigin(), 0.0);
	TEST_REAL_EQUAL(rd.getDimension(), 1.0);
	TEST_REAL_EQUAL(rd.getSpacing(), 0.25);
RESULT

CHECK(TRegularData1D(const VectorType& data, const CoordinateType& origin = 0.0, const CoordinateType& dimension = 1.0))
	RegularData1D::VectorType v;
	v.push_back(1.1);
	v.push_back(1.2);
	v.push_back(1.3);
	v.push_back(1.4);

	RegularData1D rd2(v, 1.0, 0.5);
	TEST_REAL_EQUAL(rd2.getOrigin(), 1.0)
	TEST_REAL_EQUAL(rd2.getDimension(), 0.5)
	TEST_REAL_EQUAL(rd2.getSpacing(), 0.5/3.)
	TEST_EQUAL(rd2.getSize(), 4)
	TEST_REAL_EQUAL(rd2[0], 1.1)
	TEST_REAL_EQUAL(rd2[3], 1.4)

	RegularData1D rd3(v); // The default arguments
	TEST_REAL_EQUAL(rd3.getOrigin(), 0.0)
	TEST_REAL_EQUAL(rd3.getDimension(), 1.0)
	TEST_EQUAL(rd3.getSize(), 4)

	RegularData1D rd4(v, 2.0, 1.0);
	TEST_REAL_EQUAL(rd4.getOrigin(), 2.0)
	TEST_REAL_EQUAL(rd4.getDimension(), 1.0)
	TEST_EQUAL(rd4.getSize(), 4)
RESULT

CHECK(BALL_CREATE(TRegularData1D<ValueType>))
	vector<float> v2;
	v2.push_back(1.1);
	RegularData1D a(v2, 1.1, 1.2); // This is the vector type constructor
	RegularData1D* v_ptr = (RegularData1D*)a.create(false, true);
	TEST_EQUAL(v_ptr->getSize(), 0)
	TEST_REAL_EQUAL(v_ptr->getOrigin(), 0.0)
	delete v_ptr;
	v_ptr = (RegularData1D*)a.create();
	TEST_EQUAL(v_ptr->getSize(), 1)
	TEST_REAL_EQUAL(v_ptr->getOrigin(), 1.1)
	TEST_REAL_EQUAL(v_ptr->getDimension(), 1.2)
	TEST_REAL_EQUAL((*v_ptr)[0], 1.1)
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
	//////// SPACING TESTS!!!
	rd.setDimension(2.2);
	TEST_REAL_EQUAL(rd.getOrigin(), 1.1)
	TEST_REAL_EQUAL(rd.getDimension(), 2.2)
	rd.setDimension(0.0);
	TEST_REAL_EQUAL(rd.getDimension(), 0.0)
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


CHECK(const TRegularData1D& operator = (const TRegularData1D<ValueType>& data))
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


CHECK(const TRegularData1D& operator = (const VectorType& data))
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

CHECK(const ValueType& getData(const IndexType& index) const)
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


CHECK(ValueType& getData(const IndexType& index))
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

CHECK(void resize(const IndexType& size))
	TEST_EQUAL(rd.getSize(), 4)
	rd.resize(99);
	TEST_EQUAL(rd.getSize(), 99)
	TEST_REAL_EQUAL(rd[98], 0.0)
	rd.resize(3);
	TEST_EQUAL(rd.getSize(), 3)
	TEST_REAL_EQUAL(rd[2], 3.3)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
