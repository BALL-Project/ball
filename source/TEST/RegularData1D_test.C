// $Id: RegularData1D_test.C,v 1.2 2001/03/12 11:14:30 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/regularData1D.h>

///////////////////////////

START_TEST(class_name, "$Id: RegularData1D_test.C,v 1.2 2001/03/12 11:14:30 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


RegularData1D* rd;
CHECK(TRegularData1D::TRegularData1D())
	rd = new RegularData1D;
	TEST_NOT_EQUAL(rd, 0)
RESULT


CHECK(TRegularData1D::~TRegularData1D())
  delete rd;
RESULT


CHECK(TRegularData1D::BALL_CREATE(RegularData1D<T>))
	
RESULT


CHECK(TRegularData1D::TRegularData1D(const TRegularData1D& data))
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::clear())
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::destroy())
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::TRegularData1D& operator = (const TRegularData1D& data))
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::TRegularData1D& operator = (const DataType& data))
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::bool operator == (const TRegularData1D& data) const )
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::T& operator [] (Position index) const )
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::T& operator [] (Position index))
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::getSize() const )
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::getLower() const )
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::getUpper() const )
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::setUpperBound())
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::setLowerBound())
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::resize(Size new_size))
  //BAUSTELLE
RESULT

CHECK(TRegularData1D::rescale(Size new_size))
	TRegularData1D<float>	data;
	TEST_EQUAL(data.getSize(), 0)
	data.rescale(1);
	TEST_EQUAL(data.getSize(), 1)
	TEST_EQUAL(data[0], 0.0)
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

CHECK(TRegularData1D::rescale(double lower, double upper, Size new_size))
	TRegularData1D<float>	data;
	data.resize(2);
	data.setLowerBound(0.0);
	data.setUpperBound(1.0);
	data[0] = 1.0;
	data[1] = 2.0;
	data.rescale(0.0, 1.0, 3);
	TEST_EQUAL(data.getSize(), 3)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.5)
	TEST_REAL_EQUAL(data[2], 2.0)
	data.rescale(0.0, 1.0, 4);
	TEST_EQUAL(data.getSize(), 4)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.3333333)
	TEST_REAL_EQUAL(data[2], 1.6666667)
	TEST_REAL_EQUAL(data[3], 2.0)
	data.rescale(0.0, 1.0, 4);
	TEST_EQUAL(data.getSize(), 4)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.3333333)
	TEST_REAL_EQUAL(data[2], 1.6666667)
	TEST_REAL_EQUAL(data[3], 2.0)
	data.rescale(-1.0, 2.0, 10);
	TEST_EQUAL(data.getSize(), 10)
	TEST_REAL_EQUAL(data[0], 0.0)
	TEST_REAL_EQUAL(data[1], 0.0)
	TEST_REAL_EQUAL(data[2], 0.0)
	TEST_REAL_EQUAL(data[3], 1.0)
	TEST_REAL_EQUAL(data[4], 1.3333333)
	TEST_REAL_EQUAL(data[5], 1.6666667)
	TEST_REAL_EQUAL(data[6], 2.0)
	TEST_REAL_EQUAL(data[7], 0.0)
	TEST_REAL_EQUAL(data[8], 0.0)
	TEST_REAL_EQUAL(data[9], 0.0)
	data.rescale(0.0, 1.0, 3);
	TEST_EQUAL(data.getSize(), 3)
	TEST_REAL_EQUAL(data[0], 1.0)
	TEST_REAL_EQUAL(data[1], 1.5)
	TEST_REAL_EQUAL(data[2], 2.0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
