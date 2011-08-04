// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/STRUCTURE/buildBondsProcessor.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
///////////////////////////

START_TEST(Fragment)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;
using namespace std;

BuildBondsProcessor* bbp = 0;
CHECK(BuildBondsProcessor())
	bbp = new BuildBondsProcessor();
	TEST_NOT_EQUAL(bbp, 0);
RESULT

CHECK(~BuildBondsProcessor())
	delete bbp;
RESULT

CHECK(BuildBondsProcessor(const String& filename))
	bbp = 0;
	bbp = new BuildBondsProcessor("bond_lengths/bond_lengths.db");
	TEST_NOT_EQUAL(bbp,0)
	TEST_EXCEPTION(Exception::FileNotFound, BuildBondsProcessor("file_does_not_exist"))
	delete bbp;
RESULT

CHECK(operator() (AtomContainer& ac))

	BuildBondsProcessor bbp2;
	bbp2.options[BuildBondsProcessor::Option::DELETE_EXISTING_BONDS] = "false";
	bbp2.options[BuildBondsProcessor::Option::REESTIMATE_BONDORDERS_RINGS] = "false";
	bbp2.options[BuildBondsProcessor::Option::DELETE_OVERESTIMATED_BONDS] = "false";
	
	PDBFile infileA(BALL_TEST_DATA_PATH(ACE_test_A.pdb));
	System sysA;
	infileA >> sysA;
	sysA.apply(bbp2);
	TEST_EQUAL(sysA.countBonds(), 1666)

	PDBFile infileB(BALL_TEST_DATA_PATH(ACE_test_B.pdb));
	System sysB;
	infileB >> sysB;
	sysB.apply(bbp2);
	TEST_EQUAL(sysB.countBonds(), 468)
		
	SDFile infileC(BALL_TEST_DATA_PATH(buildBondsProcessor_test.sdf));
	System sysC;
	infileC >> sysC;
	Size results[] = {9, 9, 9, 9, 9, 8, 9, 20, 6, 18, 12, 24, 21, 22};
	Size i(0);
	for (MoleculeIterator mit = sysC.beginMolecule(); +mit; ++mit, i++)
	{
		mit->apply(bbp2);
		TEST_EQUAL(mit->countBonds(), results[i]);
	}

	bbp2.options[BuildBondsProcessor::Option::DELETE_EXISTING_BONDS] = "true";
	i=0;
	for (MoleculeIterator mit = sysC.beginMolecule(); +mit; ++mit, i++)
	{
		mit->apply(bbp2);
		TEST_EQUAL(mit->countBonds(), results[i]);
	}

	bbp2.options[BuildBondsProcessor::Option::REESTIMATE_BONDORDERS_RINGS] = "false";
	bbp2.options[BuildBondsProcessor::Option::DELETE_OVERESTIMATED_BONDS] = "false";

	i=0;
	Size double_results[] = {3, 3, 0, 0, 0, 0, 0, 0, 3, 0, 3, 0, 3, 4};
	for (MoleculeIterator mit = sysC.beginMolecule(); +mit; ++mit, i++)
	{
		mit->apply(bbp2);
		TEST_EQUAL(mit->countBonds(), results[i])

		Size num_double(0);
		AtomIterator ait;
		Atom::BondIterator bit;
		BALL_FOREACH_BOND(*mit, ait, bit)
		{
			if (bit->getOrder() == Bond::ORDER__DOUBLE)
			{
				++num_double;
			}
		}
		TEST_EQUAL(num_double, double_results[i])
	}

RESULT

CHECK(setBondLengths(const String& filename))
	BuildBondsProcessor bbp2;
	bbp2.setBondLengths("bond_lengths/bond_lengths.db");
	PDBFile infileA(BALL_TEST_DATA_PATH(ACE_test_A.pdb));
	System sysA;
	infileA >> sysA;
	sysA.apply(bbp2);
	TEST_EQUAL(sysA.countBonds(), 1666)
	TEST_EXCEPTION(Exception::FileNotFound, bbp2.setBondLengths("file_does_not_exist"));
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
