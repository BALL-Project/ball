// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ContourLine_test.C,v 1.1 2003/06/23 12:26:35 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/contourLine.h>
///////////////////////////

START_TEST(class_name, "$Id: ContourLine_test.C,v 1.1 2003/06/23 12:26:35 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(TContourLine(T height = 0))
	ContourLine* line = new ContourLine;
RESULT

CHECK(~TContourLine())
  // ???
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
