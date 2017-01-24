// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(standardPredicates)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

ExpressionPredicate* ptr = 0;
ExpressionPredicate* test = 0;
Atom atom;
atom.setName("Grmpfl.");
STATUS(10)
CHECK(BALL_CREATE(AromaticBondsPredicate))
STATUS(11)
	ptr = new AromaticBondsPredicate;
STATUS(12)
	test = (ExpressionPredicate*) ptr->create(true, false);
STATUS(13)
	TEST_NOT_EQUAL(test, 0)
STATUS(14)
	(*test)(atom);
STATUS(15)
	delete test;
STATUS(16)
	delete ptr;
STATUS(17)
	test = ptr = 0;
STATUS(18)
RESULT
STATUS(19)

CHECK(BALL_CREATE(AtomNamePredicate))
	ptr = new AtomNamePredicate;
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT

STATUS(1)
CHECK(BALL_CREATE(AtomTypePredicate))
STATUS(2)
	ptr = new AtomTypePredicate();
STATUS(3)
	test = (ExpressionPredicate*) ptr->create(true, false);
STATUS(4)
	TEST_NOT_EQUAL(test, 0)
STATUS(5)
	(*test)(atom);
STATUS(6)
	delete test;
STATUS(7)
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(AxialPredicate))
	ptr = new AxialPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(BackBonePredicate))
	ptr = new BackBonePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ChainPredicate))
	ptr = new ChainPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ChargePredicate))
	ptr = new ChargePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(Conformation4C1Predicate))
	ptr = new Conformation4C1Predicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ConnectedToPredicate))
	ptr = new ConnectedToPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(DoubleBondsPredicate))
	ptr = new DoubleBondsPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ElementPredicate))
	ptr = new ElementPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(FalsePredicate))
	ptr = new FalsePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(InRingPredicate))
	ptr = new InRingPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(MoleculePredicate))
	ptr = new MoleculePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(NucleicAcidPredicate))
	ptr = new NucleicAcidPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(NucleotidePredicate))
	ptr = new NucleotidePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(NumberOfBondsPredicate))
	ptr = new NumberOfBondsPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ProteinPredicate))
	ptr = new ProteinPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ResidueIDPredicate))
	ptr = new ResidueIDPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(ResiduePredicate))
	ptr = new ResiduePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(SecondaryStructurePredicate))
	ptr = new SecondaryStructurePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(SelectedPredicate))
	ptr = new SelectedPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(SingleBondsPredicate))
	ptr = new SingleBondsPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(SolventPredicate))
	ptr = new SolventPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(Sp2HybridizedPredicate))
	ptr = new Sp2HybridizedPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(Sp3HybridizedPredicate))
	ptr = new Sp3HybridizedPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(SpHybridizedPredicate))
	ptr = new SpHybridizedPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(TripleBondsPredicate))
	ptr = new TripleBondsPredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT


CHECK(BALL_CREATE(TruePredicate))
	ptr = new TruePredicate();
	test = (ExpressionPredicate*) ptr->create(true, false);
	TEST_NOT_EQUAL(test, 0)
	(*test)(atom);
	delete test;
	delete ptr;
	test = ptr = 0;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
