// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/partialChargeBase.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////
START_TEST(PartialChargeBase)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(PartialChargeBase)	

	PartialChargeBase pcb;
	S.apply(pcb);

	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);
		TEST_EQUAL(molecule->hasProperty("TotalPositivePartialCharge"), true);
		TEST_EQUAL(molecule->hasProperty("TotalNegativePartialCharge"), true);
		TEST_EQUAL(molecule->hasProperty("RelPositivePartialCharge"), true);
		TEST_EQUAL(molecule->hasProperty("RelNegativePartialCharge"), true);
	}
		
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
