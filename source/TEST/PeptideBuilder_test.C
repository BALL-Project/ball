// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PeptideBuilder_test.C,v 1.2 2003/04/17 14:48:05 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/peptideBuilder.h>

///////////////////////////

START_TEST(PeptideBuilder, "$Id: PeptideBuilder_test.C,v 1.2 2003/04/17 14:48:05 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::Peptides;

PeptideBuilder* pb_ptr = 0;
CHECK(PeptideBuilder())
	pb_ptr = new PeptideBuilder;
	TEST_NOT_EQUAL(pb_ptr, 0)
RESULT								

CHECK(~PeptideBuilder())
	delete pb_ptr;
	pb_ptr = 0;
RESULT

// ????
// Real tests still missing

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
