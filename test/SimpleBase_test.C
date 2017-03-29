// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/QSAR/simpleBase.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////
START_TEST(SimpleBase)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SDFile infile(BALL_TEST_DATA_PATH(descriptors_test.sdf));
System S;
infile >> S;
infile.close();

Molecule * molecule;
Size limit = S.countMolecules();

CHECK(SimpleBase)	

	SimpleBase sb;
	S.apply(sb);

	for (Size i=0;i!=limit;++i)
	{
		molecule = S.getMolecule(i);

		// atom descripors
		TEST_EQUAL(molecule->hasProperty("NumberOfAtoms"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfHydrogen"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfBoron"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfCarbon"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfNitrogen"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfOxygen"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfFlourine"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfPhosphorus"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfSulfur"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfChlorine"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfBromine"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfIodine"), true);
    TEST_EQUAL(molecule->hasProperty("FormalCharge"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfHydrogenBondAcceptors"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfHydrogenBondDonors"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfHydrophobicAtoms"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfHeavyAtoms"), true);
    TEST_EQUAL(molecule->hasProperty("MeanAtomInformationContent"), true);
    TEST_EQUAL(molecule->hasProperty("MolecularWeight"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfAromaticAtoms"), true);

		// bond descriptors
    TEST_EQUAL(molecule->hasProperty("NumberOfSingleBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfDoubleBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfTripleBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfAromaticBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfHeavyBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfRotatableBonds"), true);
    TEST_EQUAL(molecule->hasProperty("NumberOfRotatableSingleBonds"), true);
    TEST_EQUAL(molecule->hasProperty("AtomicPolarizabilities"), true);
    TEST_EQUAL(molecule->hasProperty("BondPolarizabilities"), true);
    TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertia"), true);
    TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertiaX"), true);
    TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertiaY"), true);
    TEST_EQUAL(molecule->hasProperty("PrincipalMomentOfInertiaZ"), true);		
		TEST_EQUAL(molecule->hasProperty("SizeOfSSSR"), true);
	}
		
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
