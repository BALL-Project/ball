// $Id: PeakList_test.C,v 1.4 2001/12/30 13:29:00 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here

///////////////////////////

START_TEST(PeakList, "$Id: PeakList_test.C,v 1.4 2001/12/30 13:29:00 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class PeakList::

CHECK(PeakList::BALL_CREATE(PeakList<PT>))
  //?????
RESULT


CHECK(PeakList::scale(float x))
  //?????
RESULT


CHECK(PeakList::getMaxIntensity() const )
  //?????
RESULT


CHECK(PeakList::getMinIntensity() const )
  //?????
RESULT


CHECK(PeakList::getMinPosition() const )
  //?????
RESULT


CHECK(PeakList::getMaxPosition() const )
  //?????
RESULT


CHECK(PeakList::PeakList::PeakList())
  //?????
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
