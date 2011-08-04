// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/SOLVATION/ooiEnergy.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

START_TEST(OoiEnergy)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(2.0) // it's not THAT precise
CHECK(calculateOoiEnergy() / BPTI)
	System S;
	PDBFile f(BALL_TEST_DATA_PATH(OoiEnergy_test.pdb));
	f >> S;
	f.close();

	FragmentDB frag_db("fragments/Fragments.db");
	S.apply(frag_db.normalize_names);
	S.apply(frag_db.build_bonds);
	
	TEST_EQUAL(S.countAtoms(), 892)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -2602.97)
RESULT											

CHECK(calculateOoiEnergy() / ethanol)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test1.hin));
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
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test2.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 9)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -28.04)
RESULT											

CHECK(calculateOoiEnergy() / butylamine)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test3.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 16)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -18.34)
RESULT											

CHECK(calculateOoiEnergy() / methanethiol)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test4.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 6)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -4.72)
RESULT											

CHECK(calculateOoiEnergy() / acetic acid)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test5.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 8)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -29.15)
RESULT											

CHECK(calculateOoiEnergy() / acetate)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test6.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 7)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -341.942)
RESULT											

CHECK(calculateOoiEnergy() / butylammonium)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test7.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 17)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -289.70)
RESULT											

CHECK(calculateOoiEnergy() / propionate)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test8.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 10)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -315.938)
RESULT											

CHECK(calculateOoiEnergy() / methylimidazolium)
	System S;
	HINFile f(BALL_TEST_DATA_PATH(OoiEnergy_test9.hin));
	f >> S;
	f.close();

	TEST_EQUAL(S.countAtoms(), 13)
	float energy = calculateOoiEnergy(S);
	TEST_REAL_EQUAL(energy, -268.268)
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
