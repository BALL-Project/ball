// $Id: RegularData1D_test.C,v 1.1 2000/09/13 06:31:24 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/regularData1D.h>

///////////////////////////

START_TEST(class_name, "$Id: RegularData1D_test.C,v 1.1 2000/09/13 06:31:24 oliver Exp $")

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


CHECK(TRegularData1D::setUpper())
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::setLower())
  //BAUSTELLE
RESULT


CHECK(TRegularData1D::resize(Size new_size))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
