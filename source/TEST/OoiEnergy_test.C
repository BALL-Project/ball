// $Id: OoiEnergy_test.C,v 1.1 2000/05/30 10:28:50 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SOLVATION/ooiEnergy.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

START_TEST(OOiEnergy, "$Id: OoiEnergy_test.C,v 1.1 2000/05/30 10:28:50 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

PRECISION(1.0) // it's not THAT precise
CHECK(calculateOoiEnergy() / BPTI)
	System S;
	PDBFile f("data/OoiEnergy_test.pdb");
	f >> S;
	f.close();

	FragmentDB frag_db;
	S.apply(frag_db.normalize_names);
	S.apply(frag_db.build_bonds);
	
	TEST_EQUAL(S.countAtoms(), 892)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -2589.7)
RESULT											

CHECK(calculateOoiEnergy() / ethanol)
	System S;
	HINFile f("data/OoiEnergy_test1.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 9)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -21.09)
	TEST_EQUAL(S.countAtoms(), 9)
	energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -21.09)
RESULT											

CHECK(calculateOoiEnergy() / acetamide)
	System S;
	HINFile f("data/OoiEnergy_test2.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 9)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -28.04)
RESULT											

CHECK(calculateOoiEnergy() / butylamine)
	System S;
	HINFile f("data/OoiEnergy_test3.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 16)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -18.34)
RESULT											

CHECK(calculateOoiEnergy() / methanethiol)
	System S;
	HINFile f("data/OoiEnergy_test4.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 6)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -4.72)
RESULT											

CHECK(calculateOoiEnergy() / acetic acid)
	System S;
	HINFile f("data/OoiEnergy_test5.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 8)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -29.15)
RESULT											

CHECK(calculateOoiEnergy() / acetate)
	System S;
	HINFile f("data/OoiEnergy_test6.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 7)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -337.44)
RESULT											

CHECK(calculateOoiEnergy() / butylammonium)
	System S;
	HINFile f("data/OoiEnergy_test7.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 17)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -289.70)
RESULT											

CHECK(calculateOoiEnergy() / propionate)
	System S;
	HINFile f("data/OoiEnergy_test8.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 10)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -311.8)
RESULT											

CHECK(calculateOoiEnergy() / methylimidazolium)
	System S;
	HINFile f("data/OoiEnergy_test9.hin");
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 13)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -26.4)
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
