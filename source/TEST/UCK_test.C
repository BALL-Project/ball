// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK_test.C,v 1.3 2004/06/24 16:03:26 bender Exp $
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

UCK *u1;
UCK *u2;
UCK *u3;

START_TEST(UCK, "$Id: UCK_test.C,v 1.3 2004/06/24 16:03:26 bender Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

SDFile f("benzoic_acid.sdf");
Molecule *m = f.read();
f.close();
UCK *u1 = new UCK(*m, 2);
UCK *u2 = new UCK(*m);
UCK *u3 = new UCK(*u1);

CHECK(bencoic_acid_custom_depth)
	TEST_EQUAL(u1->getUCK().trim(), "39bf9b334b172e4e71e76b93c830b47e")
	ABORT_IF(u1->getUCK().trim() != "39bf9b334b172e4e71e76b93c830b47e")
RESULT											

CHECK(bencoic_acid_default_depth)
	TEST_EQUAL(u2->getUCK().trim(), "09bdbf9a8c581a33e5cbd70697eadbdd")
	ABORT_IF(u2->getUCK().trim() != "09bdbf9a8c581a33e5cbd70697eadbdd")
RESULT											

CHECK(cpconstructor)
	TEST_EQUAL(u3->getUCK().trim(), "39bf9b334b172e4e71e76b93c830b47e")
	ABORT_IF(u3->getUCK().trim() != "39bf9b334b172e4e71e76b93c830b47e")
RESULT

CHECK(output functions)
	TEST_EQUAL(u1->getDepth(), 2);
	TEST_EQUAL(u2->getDepth(), 3);
	TEST_EQUAL(u3->getDepth(), 2);
	TEST_EQUAL(u1->getId().trim(), "NSC88938 benzoic acid")
	TEST_EQUAL(u1->getFormula().trim(), "C7H6O2")
	TEST_EQUAL(String(u1->getWeight()), "122.123642")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
