// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK_test.C,v 1.1 2004/06/15 09:13:10 bender Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecule.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

///////////////////////////

using namespace BALL;
UCK *u;


START_TEST(UCK, "$Id: UCK_test.C,v 1.1 2004/06/15 09:13:10 bender Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

SDFile f("bencoic_acid.sdf");
Molecule *m = f.read();
f.close();

CHECK(bencoic_acid)
	u = new UCK(m, "benzoic_acid.sdf", 2, 1);
	TEST_EQUAL(u->getUck().trim(), "39bf9b334b172e4e71e76b93c830b47e")
	ABORT_IF(u->getUck().trim() != "39bf9b334b172e4e71e76b93c830b47e")
RESULT											

CHECK(output functions)
	TEST_EQUAL(u->getId().trim(), "bencoic_acid.sdf:1:NSC88938 benzoic acid")
	TEST_EQUAL(u->getFormula().trim(), "C7H6O2")
	TEST_EQUAL(String(u->getWeight()), "122.123642")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
