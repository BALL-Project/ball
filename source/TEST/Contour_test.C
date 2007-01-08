// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Contour_test.C,v 1.1 2003/06/11 14:33:57 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/contour.h>
///////////////////////////

START_TEST("Contour", "$Id: Contour_test.C,v 1.1 2003/06/11 14:33:57 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BALL_CREATE(TContour<T>))
  // ???
RESULT

CHECK(TContour(Size num_lines=0, double start=0, double end=0))
  // ???
RESULT

CHECK(TContour(const TContour& contour))
  // ???
RESULT

CHECK(bool getNextContourLine(TContourLine<T>& contour))
  // ???
RESULT

CHECK(bool operator == (const TContour& contour) const)
  // ???
RESULT

CHECK(const TContour& operator = (const TContour& assigTContour))
  // ???
RESULT

CHECK(void apply(TRegularData2D<T>& data))
  // ???
RESULT

CHECK(void clear())
  // ???
RESULT

CHECK(void resetCounter())
  // ???
RESULT

CHECK(~TContour())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
