// $Id: RegularData1D_test.C,v 1.9 2001/07/14 08:38:25 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/regularData1D.h>

///////////////////////////

START_TEST(class_name, "$Id: RegularData1D_test.C,v 1.9 2001/07/14 08:38:25 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

RegularData1D* rd_ptr;
CHECK(TRegularData1D::TRegularData1D())
	rd_ptr = new RegularData1D;
	TEST_NOT_EQUAL(rd_ptr, 0)
RESULT


CHECK(TRegularData1D::~TRegularData1D())
  delete rd_ptr;
RESULT

CHECK(BALL_CREATE(TRegularData1D<T>))
	vector<float> v;
	v.push_back(1.1);
	RegularData1D a(v, 1.1, 1.2); 
	RegularData1D* v_ptr = (RegularData1D*) a.create(false, true);
	TEST_EQUAL(v_ptr->getSize(), 0)
	TEST_REAL_EQUAL(v_ptr->getLowerBound(), 0.0)
	delete v_ptr;
	v_ptr = (RegularData1D*)a.create();
	TEST_EQUAL(v_ptr->getSize(), 1)
	TEST_REAL_EQUAL(v_ptr->getLowerBound(), 1.1)
	TEST_REAL_EQUAL(v_ptr->getUpperBound(), 1.2)
	delete v_ptr;
RESULT
		

RegularData1D rd;
CHECK(TRegularData1D::TRegularData1D(const TRegularData1D& data))
	rd.setBoundaries(1.1, 2.1);
	rd.resize(1);
	rd[0] = 1.2;
	RegularData1D rd2(rd);
	TEST_REAL_EQUAL(rd2.getLowerBound(), 1.1)
	TEST_REAL_EQUAL(rd2.getUpperBound(), 2.1)
	TEST_REAL_EQUAL(rd2[0], 1.2) 
	TEST_EQUAL(rd2.getSize(), 1)
RESULT


RegularData1D::VectorType v;
v.push_back(1.1);
v.push_back(1.2);
v.push_back(1.3);
v.push_back(1.4);

CHECK(TRegularData1D(const VectorType& data, double lower, double upper))
	RegularData1D rd2(v, 1.0, 1.5);
	TEST_REAL_EQUAL(rd2.getLowerBound(), 1.0)
	TEST_REAL_EQUAL(rd2.getUpperBound(), 1.5)
	TEST_EQUAL(rd2.getSize(), 4)
	TEST_REAL_EQUAL(rd2[0], 1.1)
	TEST_REAL_EQUAL(rd2[3], 1.4)

	RegularData1D rd3(v);
	TEST_REAL_EQUAL(rd3.getLowerBound(), 0.0)
	TEST_REAL_EQUAL(rd3.getUpperBound(), 0.0)
	TEST_EQUAL(rd3.getSize(), 4)

	RegularData1D rd4(v, 2.0, 1.0);
	TEST_REAL_EQUAL(rd4.getLowerBound(), 1.0)
	TEST_REAL_EQUAL(rd4.getUpperBound(), 2.0)
	TEST_EQUAL(rd4.getSize(), 4)
RESULT


CHECK(TRegularData1D::clear())
	rd.clear();
	TEST_REAL_EQUAL(rd.getLowerBound(), 1.1)
	TEST_REAL_EQUAL(rd.getUpperBound(), 2.1)
	TEST_EQUAL(rd.getSize(), 1)
	TEST_REAL_EQUAL(rd[0], 0.0)
RESULT


CHECK(TRegularData1D::destroy())
	rd[0] = 2.3;
	rd.destroy();
	TEST_REAL_EQUAL(rd.getLowerBound(), 0.0)
	TEST_REAL_EQUAL(rd.getUpperBound(), 0.0)
	TEST_EQUAL(rd.getSize(), 0)
RESULT


CHECK(TRegularData1D::TRegularData1D& operator = (const TRegularData1D& data))
	rd.setBoundaries(1.1, 2.1);
	rd.resize(1);
	rd[0] = 1.2;
	RegularData1D rd2 = rd;
	TEST_REAL_EQUAL(rd2.getLowerBound(), 1.1)
	TEST_REAL_EQUAL(rd2.getUpperBound(), 2.1)
	TEST_REAL_EQUAL(rd2[0], 1.2) 
	TEST_EQUAL(rd2.getSize(), 1)
RESULT


CHECK(TRegularData1D::TRegularData1D& operator = (const VectorType& data))
	RegularData1D::VectorType v;
	v.push_back(1.1);
	v.push_back(1.2);
	v.push_back(1.3);
	v.push_back(1.4);
	RegularData1D rd2;
	rd2.setBoundaries(0.0, 1.0);
	rd2 = v;
	TEST_REAL_EQUAL(rd2.getLowerBound(), 0.0)
	TEST_REAL_EQUAL(rd2.getUpperBound(), 1.0)
	TEST_EQUAL(rd2.getSize(), 4)
	TEST_REAL_EQUAL(rd2[0], 1.1)
	TEST_REAL_EQUAL(rd2[1], 1.2)
	TEST_REAL_EQUAL(rd2[2], 1.3)
	TEST_REAL_EQUAL(rd2[3], 1.4)
RESULT


CHECK(TRegularData1D::bool operator == (const TRegularData1D& data) const )
	rd.destroy();
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
	rd.setBoundaries(1.1, 1.4);
	TEST_EQUAL(rd == rd2, false)
	rd2.setBoundaries(1.1, 1.4);
	TEST_EQUAL(rd == rd2, true)
RESULT


CHECK(TRegularData1D::T& operator [] (Position index) const )
  TEST_REAL_EQUAL(rd[3], 1.4)
	TEST_EXCEPTION(Exception::IndexOverflow, rd[4])
RESULT


CHECK(TRegularData1D::T& operator [] (Position index))
  rd[3] = 1.5;
	TEST_REAL_EQUAL(rd[3], 1.5)
	TEST_EXCEPTION(Exception::IndexOverflow, rd[4] = 44.4)
RESULT


RegularData1D rd2;
CHECK(TRegularData1D::getSize() const )
  TEST_EQUAL(rd.getSize(), 4)
	TEST_EQUAL(rd2.getSize(), 0)
RESULT


CHECK(TRegularData1D::getLowerBound() const )
	rd.setBoundaries(1.1, 1.5);
	TEST_REAL_EQUAL(rd.getLowerBound(), 1.1)
	TEST_REAL_EQUAL(rd2.getLowerBound(), 0.0)
RESULT


CHECK(TRegularData1D::getUpperBound() const )
	TEST_REAL_EQUAL(rd.getUpperBound(), 1.5)
	TEST_REAL_EQUAL(rd2.getUpperBound(), 0.0)
RESULT


CHECK(TRegularData1D::setLowerBound())
	rd.setBoundaries(-99.9, 99.9);
	TEST_REAL_EQUAL(rd.getLowerBound(), -99.9)
	TEST_REAL_EQUAL(rd.getUpperBound(), 99.9)
	rd.setBoundaries(99.9, -99.9);
	TEST_REAL_EQUAL(rd.getLowerBound(), -99.9)
	TEST_REAL_EQUAL(rd.getUpperBound(), 99.9)
RESULT


CHECK(TRegularData1D::resize(Size new_size))
	TEST_EQUAL(rd.getSize(), 4)
	rd.resize(99);
	TEST_EQUAL(rd.getSize(), 99)
	TEST_REAL_EQUAL(rd[98], 0.0)
	rd.resize(3);
	TEST_EQUAL(rd.getSize(), 3)
	TEST_REAL_EQUAL(rd[2], 1.3)
	TEST_EXCEPTION(Exception::IndexOverflow, rd[3])
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
	data.setBoundaries(0.0, 1.0);
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
