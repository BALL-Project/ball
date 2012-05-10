// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <iterator>

///////////////////////////

using namespace BALL;

UCK *u1;
UCK *u2;
UCK *u3;

START_TEST(UCK)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

SDFile f(BALL_TEST_DATA_PATH(benzoic_acid.sdf));
Molecule* m = 0;
CHECK([Extra] Structure initialization)
	m = f.read();
	f.close();
	TEST_NOT_EQUAL(m, 0)
RESULT
	

UCK u1(*m, (Size)2);
UCK u2(*m);
UCK u3(u1);

CHECK([Extra] bencoic_acid_custom_depth)
	TEST_EQUAL(u1.getUCK().trim(), "39bf9b334b172e4e71e76b93c830b47e")
	ABORT_IF(u1.getUCK().trim() != "39bf9b334b172e4e71e76b93c830b47e")
RESULT											

CHECK([Extra] bencoic_acid_default_depth)
	TEST_EQUAL(u2.getUCK().trim(), "09bdbf9a8c581a33e5cbd70697eadbdd")
	ABORT_IF(u2.getUCK().trim() != "09bdbf9a8c581a33e5cbd70697eadbdd")
RESULT											

CHECK(cpconstructor)
	TEST_EQUAL(u3.getUCK().trim(), "39bf9b334b172e4e71e76b93c830b47e")
	ABORT_IF(u3.getUCK().trim() != "39bf9b334b172e4e71e76b93c830b47e")
RESULT

CHECK(output functions)
	TEST_EQUAL(u1.getDepth(), 2);
	TEST_EQUAL(u2.getDepth(), 3);
	TEST_EQUAL(u3.getDepth(), 2);
	TEST_EQUAL(u1.getId().trim(), "NSC88938 benzoic acid")
	TEST_EQUAL(u1.getFormula().trim(), "C7H6O2")
	TEST_EQUAL(String(u1.getWeight()), "122.123642")
RESULT
delete m;
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
