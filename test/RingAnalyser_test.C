// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/ringAnalyser.h>

///////////////////////////

START_TEST(RingAnalyser)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

RingAnalyser* ra;

CHECK(RingAnalyser())
	ra = new RingAnalyser;
	TEST_NOT_EQUAL(ra, 0)
RESULT

CHECK(~RingAnalyser())
	delete ra;
RESULT

END_TEST
