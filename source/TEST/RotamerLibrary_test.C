// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RotamerLibrary_test.C,v 1.1 2003/04/01 21:10:54 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(RotamerLibrary, "$Id: RotamerLibrary_test.C,v 1.1 2003/04/01 21:10:54 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Rotamer* rot_ptr = 0;
CHECK(Rotamer::Rotamer())
	rot_ptr = new Rotamer;
	TEST_NOT_EQUAL(rot_ptr, 0)
RESULT

CHECK(Rotamer::~Rotamer())
	delete rot_ptr;
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
