// $Id: StandardPredicates_test.C,v 1.3 2000/05/23 07:56:43 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/FORMAT/HINFile.h>

///////////////////////////

START_TEST(standardPredicates, "$Id: StandardPredicates_test.C,v 1.3 2000/05/23 07:56:43 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class AtomNamePredicate::

CHECK(AtomNamePredicate::()(const Atom& atom) const )
  Atom atom;
	atom.setName("Grmpfl.");
	AtomNamePredicate atomName;
	atomName.setArgument("TEST");
	TEST_EQUAL(atomName(atom), false)
	Atom atom2;
	atom2.setName("TEST");
	TEST_EQUAL(atomName(atom2), true)
	atomName.setArgument("");
	TEST_EQUAL(atomName(atom), false)
	atom2.setName("");
	TEST_EQUAL(atomName(atom2), true)
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

HINFile f("data/L-Tryptophan.hin");
System S;
f >> S;
f.close();
Atom* test_atom = &*(S.beginAtom());

// tests for class inRingPredicate::

CHECK(inRingPredicate::()(const Atom& atom) const )
	InRingPredicate in0Ring;
	in0Ring.setArgument("0");
	InRingPredicate in1Ring;
	in1Ring.setArgument("1");
	InRingPredicate in2Ring;
	in2Ring.setArgument("2");
	InRingPredicate in3Ring;
	in3Ring.setArgument("3");
	InRingPredicate in4Ring;
	in4Ring.setArgument("4");
	InRingPredicate in5Ring;
	in5Ring.setArgument("5");
	InRingPredicate in6Ring;
	in6Ring.setArgument("6");
	InRingPredicate in9Ring;
	in9Ring.setArgument("9");
	
	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		TEST_EQUAL(in0Ring(*it), false)
		TEST_EQUAL(in1Ring(*it), false)
		TEST_EQUAL(in2Ring(*it), false)
		TEST_EQUAL(in3Ring(*it), false)
		TEST_EQUAL(in4Ring(*it), false)
		switch (i) 
		{
			case 10: 
			case 11:
			case 13:
			case 14:
			case 16:
				TEST_EQUAL(in5Ring(*it), true)
				break;
			default:
				TEST_EQUAL(in5Ring(*it), false)
		}
		switch (i) 
		{
			case 13:
			case 16:
			case 17:
			case 19:
			case 21:
			case 23:
				TEST_EQUAL(in6Ring(*it), true)
				break;
			default:
				TEST_EQUAL(in6Ring(*it), false)
		}
		switch (i) 
		{
			case 10: 
			case 11:
			case 13:
			case 14:
			case 16:
			case 17:
			case 19:
			case 21:
			case 23:
				TEST_EQUAL(in9Ring(*it), true)
				break;
			default:
				TEST_EQUAL(in9Ring(*it), false)
		}
	}
  //BAUSTELLE
RESULT


// tests for class doubleBondsPredicate::

CHECK(doubleBondsPredicate::()(const Atom& atom) const )
	/*
	doubleBondsPredicate doubleBonds0;
	doubleBonds0.setArgument("0");
	doubleBondsPredicate doubleBonds1;
	doubleBonds1.setArgument("1");
	doubleBondsPredicate doubleBonds2;
	doubleBonds2.setArgument("2");
	doubleBondsPredicate doubleBonds3;
	doubleBonds3.setArgument("3");
	doubleBondsPredicate doubleBonds4;
	doubleBonds4.setArgument("4");
	doubleBondsPredicate doubleBonds5;
	doubleBonds5.setArgument("5");

	it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		TEST_EQUAL(doubleBonds5, false)
		switch (i) 
		{
			case 
				TEST_EQUAL(doubleBonds0(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds0(*it), false)
		}
		switch (i) 
		{
			case 
				TEST_EQUAL(doubleBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds1(*it), false)
		}
		switch (i) 
		{
			case 
				TEST_EQUAL(doubleBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds2(*it), false)
		}
		switch (i) 
		{
			case 
				TEST_EQUAL(doubleBonds3(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds3(*it), false)
		}
		switch (i) 
		{
			case 
				TEST_EQUAL(doubleBonds4(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds4(*it), false)
		}
	}

	*/
  //BAUSTELLE
	
RESULT


// tests for class tripleBondsPredicate::

CHECK(tripleBondsPredicate::()(const Atom& atom) const )
	/*
	tripleBondsPredicate tripleBonds0;
	tripleBonds0.setArgument("0");
	tripleBondsPredicate tripleBonds1;
	tripleBonds1.setArgument("1");
	tripleBondsPredicate tripleBonds2;
	tripleBonds2.setArgument("2");
	tripleBondsPredicate tripleBonds3;
	tripleBonds3.setArgument("3");

	it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		TEST_EQUAL(tripleBonds3, false)
		switch (i) 
		{
			case 
				TEST_EQUAL(tripleBonds0(*it), true)
				break;
			default:
				TEST_EQUAL(tripleBonds0(*it), false)
		}
		switch (i) 
		{
			case 
				TEST_EQUAL(tripleBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(tripleBonds1(*it), false)
		}
		switch (i) 
		{
			case 
				TEST_EQUAL(tripleBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(tripleBonds2(*it), false)
		}
	}
  */
  //BAUSTELLE
RESULT


// tests for class aromaticBondsPredicate::

CHECK(aromaticBondsPredicate::()(const Atom& atom) const )
	aromaticBondsPredicate aromaticBonds0;
	aromaticBonds0.setArgument("0");
	aromaticBondsPredicate aromaticBonds1;
	aromaticBonds1.setArgument("1");
	aromaticBondsPredicate aromaticBonds2;
	aromaticBonds2.setArgument("2");
	aromaticBondsPredicate aromaticBonds3;
	aromaticBonds3.setArgument("3");
	aromaticBondsPredicate aromaticBonds4;
	aromaticBonds3.setArgument("4");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		TEST_EQUAL(aromaticBonds4(*it), false)
		switch (i) 
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 7:
			case 8:
			case 9:
			case 12:
			case 15:
			case 18:
			case 20:
			case 22:
			case 24:
			case 26:
			case 27:
				TEST_EQUAL(aromaticBonds0(*it), true)
				break;
			default:
				TEST_EQUAL(aromaticBonds0(*it), false)
		}

		switch (i) 
		{
			case 6:
			case 25:
				TEST_EQUAL(aromaticBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(aromaticBonds1(*it), false)
		}

		switch (i) 
		{
			case 5:
			case 10:
			case 17:
			case 21:
			case 23:
			case 19:
			case 14:
			case 11:
				TEST_EQUAL(aromaticBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(aromaticBonds2(*it), false)
		}

		switch (i) 
		{
			case 13:
			case 16:
				TEST_EQUAL(aromaticBonds3(*it), true)
				break;
			default:
				TEST_EQUAL(aromaticBonds3(*it), false)
		}
	}
  //BAUSTELLE
RESULT


// tests for class numberOfBondsPredicate::

CHECK(numberOfBondsPredicate::()(const Atom& atom) const )
	numberOfBondsPredicate numberOfBonds0;
	numberOfBonds0.setArgument("0");
	numberOfBondsPredicate numberOfBonds1;
	numberOfBonds1.setArgument("1");
	numberOfBondsPredicate numberOfBonds2;
	numberOfBonds2.setArgument("2");
	numberOfBondsPredicate numberOfBonds3;
	numberOfBonds3.setArgument("3");
	numberOfBondsPredicate numberOfBonds4;
	numberOfBonds4.setArgument("4");
	numberOfBondsPredicate numberOfBonds5;
	numberOfBonds5.setArgument("5");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		TEST_EQUAL(numberOfBonds0(*it), false)
		TEST_EQUAL(numberOfBonds2(*it), false)
		TEST_EQUAL(numberOfBonds5(*it), false)
		switch (i) 
		{
			case 2:
			case 4:
			case 6:
			case 8:
			case 9:
			case 12:
			case 15:
			case 18:
			case 20:
			case 22:
			case 24:
			case 25:
			case 26:
			case 27:
				TEST_EQUAL(numberOfBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(numberOfBonds1(*it), false)
		}
		switch (i) 
		{
			case 5:
			case 10:
			case 11:
			case 13:
			case 14:
			case 16:
			case 17:
			case 19:
			case 21:
			case 23:
				TEST_EQUAL(numberOfBonds3(*it), true)
				break;
			default:
				TEST_EQUAL(numberOfBonds3(*it), false)
		}
		switch (i) 
		{
			case 1:
			case 3:
			case 7:
				TEST_EQUAL(numberOfBonds4(*it), true)
				break;
			default:
				TEST_EQUAL(numberOfBonds4(*it), false)
		}
	}
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
