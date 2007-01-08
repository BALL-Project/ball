// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Bruker1DFile_test.C,v 1.2 2003/06/02 14:34:29 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/bruker1DFile.h>

///////////////////////////

START_TEST(Bruker1DFile, "$Id: Bruker1DFile_test.C,v 1.2 2003/06/02 14:34:29 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Bruker1DFile* ptr = 0;
CHECK(Bruker1DFile::Bruker1DFile() throw())
	ptr = new Bruker1DFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT											

CHECK(Bruker1DFile::~Bruker1DFile() throw())
	delete ptr;
RESULT											

/// ????

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
