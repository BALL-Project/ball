// $Id: PeakList_test.C,v 1.3 2001/07/15 17:32:40 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here

///////////////////////////

START_TEST(PeakList, "$Id: PeakList_test.C,v 1.3 2001/07/15 17:32:40 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

// tests for class PeakList::

CHECK(PeakList::BALL_CREATE(PeakList<PT>))
  //BAUSTELLE
RESULT


CHECK(PeakList::scale(float x))
  //BAUSTELLE
RESULT


CHECK(PeakList::getMaxIntensity() const )
  //BAUSTELLE
RESULT


CHECK(PeakList::getMinIntensity() const )
  //BAUSTELLE
RESULT


CHECK(PeakList::getMinPosition() const )
  //BAUSTELLE
RESULT


CHECK(PeakList::getMaxPosition() const )
  //BAUSTELLE
RESULT


CHECK(PeakList::PeakList::PeakList())
  //BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
