// $Id: PeakList_test.C,v 1.2 2001/07/09 17:10:21 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here

///////////////////////////

START_TEST(class_name, "$Id: PeakList_test.C,v 1.2 2001/07/09 17:10:21 oliver Exp $")

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
