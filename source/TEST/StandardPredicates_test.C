// $Id: StandardPredicates_test.C,v 1.1 2000/05/19 13:20:17 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/standardPredicates.h>

///////////////////////////

START_TEST(standardPredicates, "$Id: StandardPredicates_test.C,v 1.1 2000/05/19 13:20:17 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class AtomNamePredicate::

CHECK(AtomNamePredicate::()(const Atom& atom) const )
  Atom atom;
	atom.setName("Grmpfl.");
	AtomNamePredicate predicate;
	predicate.setArgument("TEST");
	TEST_EQUAL(predicate(atom), false)
	Atom atom2;
	atom2.setName("TEST");
	TEST_EQUAL(predicate(atom2), true)
	predicate.setArgument("");
	TEST_EQUAL(predicate(atom), false)
	atom2.setName("");
	TEST_EQUAL(predicate(atom2), true)
RESULT


// tests for class AtomTypePredicate::

CHECK(AtomTypePredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class ElementPredicate::

CHECK(ElementPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class ResiduePredicate::

CHECK(ResiduePredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class ResidueIDPredicate::

CHECK(ResidueIDPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class ProteinPredicate::

CHECK(ProteinPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class ChainPredicate::

CHECK(ChainPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class SecondaryStructurePredicate::

CHECK(SecondaryStructurePredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class SolventPredicate::

CHECK(SolventPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class MoleculePredicate::

CHECK(MoleculePredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class BackBonePredicate::

CHECK(BackBonePredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class NucleicAcidPredicate::

CHECK(NucleicAcidPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class NucleotidePredicate::

CHECK(NucleotidePredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class inRingPredicate::

CHECK(inRingPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class doubleBondsPredicate::

CHECK(doubleBondsPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class tripleBondsPredicate::

CHECK(tripleBondsPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class aromaticBondsPredicate::

CHECK(aromaticBondsPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class numberOfBondsPredicate::

CHECK(numberOfBondsPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class connectedToPredicate::

CHECK(connectedToPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class spHybridizedPredicate::

CHECK(spHybridizedPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class sp2HybridizedPredicate::

CHECK(sp2HybridizedPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class sp3HybridizedPredicate::
CHECK(sp3HybridizedPredicate::()(const Atom& atom) const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
