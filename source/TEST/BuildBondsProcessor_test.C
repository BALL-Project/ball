// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: BuildBondsProcessor_test.C,v 1.2 2005/02/18 12:08:12 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/buildBondsProcessor.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(Fragment, "$Id: BuildBondsProcessor_test.C,v 1.2 2005/02/18 12:08:12 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;
using namespace std;

// TODO testing of all methods and constructors, and operators


BuildBondsProcessor* bbp = 0;
CHECK(BuildBondsProcessor() )
	bbp = new BuildBondsProcessor();
	TEST_NOT_EQUAL(bbp, 0);
RESULT

CHECK( ~BuildBondsProcessor() )
	delete bbp;
RESULT

CHECK(operator() (AtomContainer& ac))
	BuildBondsProcessor bbp;
	PDBFile infileA("data/ACE_test_A.pdb");
	System sysA;
	infileA >> sysA;
	sysA.apply(bbp);
	TEST_EQUAL(sysA.countBonds(), 1666)

	PDBFile infileB("data/ACE_test_B.pdb");
	System sysB;
	infileB >> sysB;
	sysB.apply(bbp);
	TEST_EQUAL(sysB.countBonds(), 468)

	SDFile infileC("data/buildBondsProcessor_test.sdf");
	System sysC;
	infileC >> sysC;
	Size results[] = {9, 9, 9, 11, 9, 8, 9, 20, 6, 18, 12, 24, 21, 22};
	Size i(0);
	for (MoleculeIterator mit = sysC.beginMolecule(); +mit; ++mit, i++)
	{
		mit->apply(bbp);
		TEST_EQUAL(mit->countBonds(), results[i]);
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
