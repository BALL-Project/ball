// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/contourLine.h>
///////////////////////////

START_TEST(TContourLine)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ContourLine* cl_ptr = 0;
CHECK(TContourLine(T height = 0))
	cl_ptr = new ContourLine;
RESULT

CHECK(~TContourLine())
	delete cl_ptr;
RESULT

CHECK(TContourLine(const TContourLine& copyTContourLine))
  // ???
RESULT

CHECK(bool getNextPoint(PointType &p))
  // ???
RESULT

CHECK(bool operator == (const TContourLine& compTContourLine) const)
  // ???
RESULT

CHECK(const TContourLine& operator = (const TContourLine& assigTContourLine))
  // ???
RESULT

CHECK(void clear())
  // ???
RESULT

CHECK(void createContourLine(TRegularData2D<T>& from))
  // ???
RESULT

CHECK(void interpol12())
  // ???
RESULT

CHECK(void interpol18())
  // ???
RESULT

CHECK(void interpol24())
  // ???
RESULT

CHECK(void interpol48())
  // ???
RESULT

CHECK(void resetCounter())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
