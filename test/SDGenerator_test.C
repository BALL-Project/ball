// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/sdGenerator.h>

///////////////////////////

START_TEST(SDGenerator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.001)

SDGenerator* sdg;

CHECK(SDGenerator())
	sdg = new SDGenerator;
	TEST_NOT_EQUAL(sdg, 0)
RESULT

CHECK(~SDGenerator())
	delete sdg;
RESULT

END_TEST
