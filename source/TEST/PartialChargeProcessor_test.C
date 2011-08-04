// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/partialChargeProcessor.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////
START_TEST(PartialChargeProcessor)

/////////////////////////////////////////////////////////////
// tests only if the partial charge processor
// creates the PEOEPartialCharge proeprty
// no numerical correctness is tested
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(PartialChargeProcessor)	

	PartialChargeProcessor pcp;
	S.apply(pcp);

	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		AtomConstIterator a_it = molecule->beginAtom();
		for (;a_it!=molecule->endAtom();++a_it)
		{
			TEST_EQUAL(a_it->hasProperty("PEOEPartialCharge"), true)
		}
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
