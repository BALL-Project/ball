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
	
// tests for class AtomNamePredicate::

CHECK([EXTRA]AtomNamePredicate::operator () (const Atom& atom) const )
  Atom atom;
	atom.setName("Grmpfl.");
	AtomNamePredicate atom_name;
	atom_name.setArgument("TEST");
	TEST_EQUAL(atom_name(atom), false)
	Atom atom2;
	atom2.setName("TEST");
	TEST_EQUAL(atom_name(atom2), true)
	atom_name.setArgument("");
	TEST_EQUAL(atom_name(atom), false)
	atom2.setName("");
	TEST_EQUAL(atom_name(atom2), true)
RESULT


// tests for class AtomTypePredicate::

CHECK([EXTRA]AtomTypePredicate::operator () (const Atom& atom) const )
  Atom atom;
	atom.setTypeName("CT");
	AtomTypePredicate type_name;
	type_name.setArgument("CT");
	TEST_EQUAL(type_name(atom), true)
	Atom atom2;
	atom2.setTypeName("TEST");
	TEST_EQUAL(type_name(atom2), false)
	type_name.setArgument("");
	TEST_EQUAL(type_name(atom), false)
	atom2.setTypeName("");
	TEST_EQUAL(type_name(atom2), true)
RESULT


// tests for class ElementPredicate::
CHECK([EXTRA]ElementPredicate::operator () (const Atom& atom) const )
	Atom atom;
	ElementPredicate pred;
	pred.setArgument("?");	
	TEST_EQUAL(pred(atom), true)
	pred.setArgument("Zr");
	TEST_EQUAL(pred(atom), false)
	atom.setElement(PTE[Element::Zr]);
	TEST_EQUAL(pred(atom), true)
	pred.setArgument("");
	TEST_EQUAL(pred(atom), false)
RESULT


// tests for class ResiduePredicate::

CHECK([EXTRA]ResiduePredicate::operator () (const Atom& atom) const )
	ResiduePredicate pred;

	Residue res;
	PDBAtom* a1 = new PDBAtom;
	res.insert(*a1);
	res.setName("ARG");
	TEST_EQUAL(pred(*a1), false)
	pred.setArgument("ARG");
	TEST_EQUAL(pred(*a1), true)
	pred.setArgument("");
	TEST_EQUAL(pred(*a1), false)
	res.setName("");
	TEST_EQUAL(pred(*a1), true)
	res.remove(*a1);
	TEST_EQUAL(pred(*a1), false)
	delete a1;
RESULT


// tests for class ResidueIDPredicate::

CHECK([EXTRA]ResidueIDPredicate::operator () (const Atom& atom) const )
	ResidueIDPredicate pred;

	Residue res;
	PDBAtom* a1 = new PDBAtom;
	res.insert(*a1);
	res.setID("ARG");
	TEST_EQUAL(pred(*a1), false)
	pred.setArgument("ARG");
	TEST_EQUAL(pred(*a1), true)
	pred.setArgument("");
	TEST_EQUAL(pred(*a1), false)
	res.setID("");
	TEST_EQUAL(pred(*a1), true)
	res.remove(*a1);
	TEST_EQUAL(pred(*a1), false)
	delete a1;
RESULT


// tests for class ProteinPredicate::

CHECK([EXTRA]ProteinPredicate::operator () (const Atom& atom) const )
	ProteinPredicate pred;

	Protein  p;
	Residue& res = *new Residue;
	PDBAtom* a1 = new PDBAtom;
	p.insert(res);
	res.insert(*a1);
	p.setName("ARG");
	TEST_EQUAL(pred(*a1), false)
	pred.setArgument("ARG");
	TEST_EQUAL(pred(*a1), true)
	pred.setArgument("");
	TEST_EQUAL(pred(*a1), false)
	p.setName("");
	TEST_EQUAL(pred(*a1), true)
	res.remove(*a1);
	TEST_EQUAL(pred(*a1), false)
	delete a1;
RESULT


// tests for class ChainPredicate::

CHECK([EXTRA]ChainPredicate::operator () (const Atom& atom) const )
	ChainPredicate pred;

	Chain chain;
	Residue& res = *new Residue;
	PDBAtom* a1 = new PDBAtom;
	chain.insert(res);
	res.insert(*a1);
	chain.setName("ARG");
	TEST_EQUAL(pred(*a1), false)
	pred.setArgument("ARG");
	TEST_EQUAL(pred(*a1), true)
	pred.setArgument("");
	TEST_EQUAL(pred(*a1), false)
	chain.setName("");
	TEST_EQUAL(pred(*a1), true)
	res.remove(*a1);
	TEST_EQUAL(pred(*a1), false)
	delete a1;
RESULT


// tests for class SecondaryStructurePredicate::

CHECK([EXTRA]SecondaryStructurePredicate::operator () (const Atom& atom) const )
	SecondaryStructurePredicate pred;

	SecondaryStructure sec_struct;
	Residue& res = *new Residue;
	PDBAtom* a1 = new PDBAtom;
	sec_struct.insert(res);
	res.insert(*a1);
	sec_struct.setName("ARG");
	TEST_EQUAL(pred(*a1), false)
	pred.setArgument("ARG");
	TEST_EQUAL(pred(*a1), true)
	pred.setArgument("");
	TEST_EQUAL(pred(*a1), false)
	sec_struct.setName("");
	TEST_EQUAL(pred(*a1), true)
	res.remove(*a1);
	TEST_EQUAL(pred(*a1), false)
	delete a1;
RESULT


// tests for class SolventPredicate::

CHECK([EXTRA]SolventPredicate::operator () (const Atom& atom) const )
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	m.insert(a2);
	SolventPredicate sp;
	TEST_EQUAL(sp(a1), false)
	TEST_EQUAL(sp(a2), false)
	m.setProperty(Molecule::IS_SOLVENT);
	TEST_EQUAL(sp(a1), true)
	TEST_EQUAL(sp(a2), true)
RESULT

// tests for class MoleculePredicate::

CHECK([EXTRA]MoleculePredicate::operator () (const Atom& atom) const )
	Atom atom;
	Molecule mol;
	mol.insert(atom);
	MoleculePredicate pred;
	pred.setArgument("");
	TEST_EQUAL(pred(atom), true)
	mol.setName("TESTMOL");
	TEST_NOT_EQUAL(pred(atom), true)
	pred.setArgument("TESTMOL");
	TEST_EQUAL(pred(atom), true)
RESULT


// tests for class NucleicAcidPredicate::

CHECK([EXTRA]NucleicAcidPredicate::operator () (const Atom& atom) const )
	Atom atom;
	NucleicAcid n_a;
	n_a.insert(atom);
	NucleicAcidPredicate pred;
	pred.setArgument("");
	TEST_EQUAL(pred(atom), true)
	n_a.setName("TESTMOL");
	TEST_NOT_EQUAL(pred(atom), true)
	pred.setArgument("TESTMOL");
	TEST_EQUAL(pred(atom), true)
RESULT


// tests for class NucleotidePredicate::

CHECK([EXTRA]NucleotidePredicate::operator () (const Atom& atom) const )
	ResiduePredicate pred;

	Residue res;
	PDBAtom* a1 = new PDBAtom;
	res.insert(*a1);
	res.setName("ARG");
	TEST_EQUAL(pred(*a1), false)
	pred.setArgument("ARG");
	TEST_EQUAL(pred(*a1), true)
	pred.setArgument("");
	TEST_EQUAL(pred(*a1), false)
	res.setName("");
	TEST_EQUAL(pred(*a1), true)
	res.remove(*a1);
	TEST_EQUAL(pred(*a1), false)
	delete a1;
RESULT

HINFile f(BALL_TEST_DATA_PATH(L-Tryptophan.hin));
System S;
f >> S;
f.close();

// tests for class BackBonePredicate::

CHECK([EXTRA]BackBonePredicate::operator () (const Atom& atom) const )
	BackBonePredicate pred;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{	
		STATUS("testing atom" << it->getFullName())
		switch (i)
		{
			case 1:
			case 3:
			case 5:
			case 6:
				TEST_EQUAL(pred(*it), true)
				break;
			default:
				TEST_EQUAL(pred(*it), false)
		}
	}
RESULT

// tests for InRingPredicate


CHECK([EXTRA]InRingPredicate::operator () (const Atom& atom) const )
	InRingPredicate inRing;
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

	RingFinder find_5_ring(5);
	RingFinder find_6_ring(6);
	RingFinder find_9_ring(9);
	
	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		STATUS(i)
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
				TEST_EQUAL(find_5_ring(*it), true)
				TEST_EQUAL(find_5_ring.getRingAtoms().size(), 5);
				break;
			default:
				TEST_EQUAL(in5Ring(*it), false)
				TEST_EQUAL(find_5_ring(*it), false)
				TEST_NOT_EQUAL(find_5_ring.getRingAtoms().size(), 5);
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
				TEST_EQUAL(find_6_ring(*it), true)
				TEST_EQUAL(find_6_ring.getRingAtoms().size(), 6);
				break;
			default:
				TEST_EQUAL(in6Ring(*it), false)
				TEST_EQUAL(find_6_ring(*it), false)
				TEST_NOT_EQUAL(find_6_ring.getRingAtoms().size(), 6);
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
				TEST_EQUAL(inRing(*it), true)
				TEST_EQUAL(in9Ring(*it), true)
				TEST_EQUAL(find_9_ring(*it), true)
				TEST_EQUAL(find_9_ring.getRingAtoms().size(), 9);
				break;
			default:
				TEST_EQUAL(inRing(*it), false)
				TEST_EQUAL(in9Ring(*it), false)
				TEST_EQUAL(find_9_ring(*it), false)
				TEST_NOT_EQUAL(find_9_ring.getRingAtoms().size(), 9);
		}
	}
RESULT

// tests for class ConnectedToPredicate::

CHECK([EXTRA]ConnectedToPredicate::operator () (const Atom& atom) const )
	ConnectedToPredicate connectedTo;

	// we may not walk back again!
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::N]);
	Bond* bond = a1->createBond(*a2);
	bond->setOrder(Bond::ORDER__SINGLE);
	STATUS("(-N(-C(-N(-C(-N(-C(-N(-C(-N(-C(-N)))))))))))")
	connectedTo.setArgument("(-N(-C(-N(-C(-N(-C(-N(-C(-N(-C(-N)))))))))))");
	TEST_EQUAL(connectedTo.getArgument(), "(-N(-C(-N(-C(-N(-C(-N(-C(-N(-C(-N)))))))))))")
	TEST_EQUAL(connectedTo(*a1), false)
	STATUS("NCNCNCNCNCN")
	connectedTo.setArgument("NCNCNCNCNCN");
	TEST_EQUAL(connectedTo.getArgument(), "NCNCNCNCNCN")
	TEST_EQUAL(connectedTo(*a1), false)
	STATUS("(-N)")
	connectedTo.setArgument("(-N)");
	TEST_EQUAL(connectedTo(*a1), true)
	STATUS("(-N)(-N)")
	connectedTo.setArgument("(-N)(-N)");
	TEST_EQUAL(connectedTo(*a1), false)
	delete a1;
	delete a2;

	AtomIterator it = S.beginAtom();

	STATUS("(H)");
	connectedTo.setArgument("(H)");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(C)");
	connectedTo.setArgument("(C)");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(O)");
	connectedTo.setArgument("(O)");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("(H)(H)(H)");
	connectedTo.setArgument("(H)(H)(H)");
	TEST_EQUAL(connectedTo(*it), true)

	// The following is not the same as (H)(H)(H)! Actually it means (H(H(H)))
	STATUS("HHH");
	connectedTo.setArgument("HHH");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("H3");
	connectedTo.setArgument("H3");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("H4");
	connectedTo.setArgument("H4");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("H1");
	connectedTo.setArgument("H1");
	TEST_EQUAL(connectedTo(*it), true)

	// This is another special case: whenever a number indicates that
	// elements have to be multiplied, the parser treats it as a subgroup.
	// Always.
	STATUS("H1H1");
	connectedTo.setArgument("H1H1");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(H1)(H1)");
	connectedTo.setArgument("(H1)(H1)");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(C(H))");
	connectedTo.setArgument("(C(H))");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("CH");
	connectedTo.setArgument("CH");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("CH)");
	CAPTURE_OUTPUT_LEVEL(2000)
	connectedTo.setArgument("CH)");
	COMPARE_OUTPUT("ConnectedToPredicate::parse_():\n")
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("(CH");
	CAPTURE_OUTPUT_LEVEL(2000)
	connectedTo.setArgument("(CH");
	COMPARE_OUTPUT("ConnectedToPredicate::parse_():\n")
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("C(H");
	CAPTURE_OUTPUT_LEVEL(2000)
	connectedTo.setArgument("C(H");
	COMPARE_OUTPUT("ConnectedToPredicate::parse_():\n")
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("(H)(H)(H)(C(H)(C(~O)(~O)))")
	connectedTo.setArgument("(H)(H)(H)(C(H)(C(~O)(~O)))");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(H3)(C(H)(CO2)CH2)")
	connectedTo.setArgument("(H3)(C(H)(CO2)CH2)");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(H3)C(H)(CO2)CH2")
	connectedTo.setArgument("(H3)C(H)(CO2)CH2");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(H3)C(C(H2)C)C")
	connectedTo.setArgument("(H3)C(C(H2)C)C");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(H)(H)(H)(C(H)(C(O)(=O)))");
	connectedTo.setArgument("(H)(H)(H)(C(H)(C(O)(=O)))");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("(H)(H)(H)(C(H)(C(-O)(-O)))");
	connectedTo.setArgument("(H)(H)(H)(C(H)(C(-O)(-O)))");
	TEST_EQUAL(connectedTo(*it), false)

 	STATUS("(-H)(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H)(~C)))))")
	connectedTo.setArgument("(-H)(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C))))");
	TEST_EQUAL(connectedTo(*it), true)

 	STATUS("(-H)(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H)(~C)))))")
	connectedTo.setArgument("(-H)(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C))))");
	STATUS("predicate set")
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(-H)(-*)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H)(~C)))))")
	connectedTo.setArgument("(-H)(-*)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C))))");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(-H(-N(-H)))")
	connectedTo.setArgument("(-H(-N(-H)))");
	TEST_EQUAL(connectedTo(*it), false);

	STATUS("(-C(-*)(-*)(-*)(-*))")
	connectedTo.setArgument("(-C(-*)(-*)(-*)(-*))");
	TEST_EQUAL(connectedTo(*it), false);

	STATUS("H3C(CO2)(H)CH2@1CCNCC@1")
	connectedTo.setArgument("H3C(CO2)(H)CH2@1CCNCC@1");
	TEST_EQUAL(connectedTo(*it), true);

	STATUS("H3C(CO2)(H)CH2C@1CNCC@1")
	connectedTo.setArgument("H3C(CO2)(H)CH2C@1CNCC@1");
	TEST_EQUAL(connectedTo(*it), false);

	STATUS("H3C(CFe2)(H)CH2@1CCNCC@1")
	connectedTo.setArgument("H3C(CFe2)(H)CH2@1CCNCC@1");
	TEST_EQUAL(connectedTo(*it), false);

	++it;

	STATUS("(-N(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H)(~C))))))")
	connectedTo.setArgument("(-N(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C)))))");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(-N(-C(-*)(-*)(-*)(-*)))")
	connectedTo.setArgument("(-C(-*)(-*)(-*)(-*))");
	TEST_EQUAL(connectedTo(*it), false);
	
	++it;
	STATUS("(-N(-H)(-H)(-H))(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C)))")
	connectedTo.setArgument("(-N(-H)(-H)(-H))(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C)))");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(-N(-H)(-H)(-H))(-H)(-C(~O)(~O))(-C(=H)(-H)(-C(~C(-H))(~C)))")
	connectedTo.setArgument("(-N(-H)(-H)(-H))(-H)(-C(~O)(~O))(-C(=H)(-H)(-C(~C(-H))(~C)))");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("(-N(-H)(-O)(-H))(-H)(-C(~O)(~O))(-C(=H)(-H)(-C(~C(-H))(~C)))");
	connectedTo.setArgument("(-N(-H)(-O)(-H))(-H)(-C(~O)(~O))(-C(=H)(-H)(-C(~C(-H))(~C)))");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("(C(~O)(~*))")
	connectedTo.setArgument("(C(~O)(~*))");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(-H)(-C)(-C)")
	connectedTo.setArgument("(-H)(-C)(-C)");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("(-H)(-C(-H))(-C(-H))")
	connectedTo.setArgument("(-H)(-C(-H))(-C(-H))");
	TEST_EQUAL(connectedTo(*it), false)

	STATUS("CH2R")
	connectedTo.setArgument("CH2R");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("*R")
	connectedTo.setArgument("*R");
	TEST_EQUAL(connectedTo(*it), true)

	STATUS("R")
	connectedTo.setArgument("R");
	TEST_EQUAL(connectedTo(*it), false)
RESULT

// tests for class AromaticBondsPredicate::

CHECK([EXTRA]AromaticBondsPredicate::operator () (const Atom& atom) const )
	AromaticBondsPredicate aromaticBonds0;
	aromaticBonds0.setArgument("0");
	AromaticBondsPredicate aromaticBonds1;
	aromaticBonds1.setArgument("1");
	AromaticBondsPredicate aromaticBonds2;
	aromaticBonds2.setArgument("2");
	AromaticBondsPredicate aromaticBonds3;
	aromaticBonds3.setArgument("3");
	AromaticBondsPredicate aromaticBonds4;
	aromaticBonds4.setArgument("4");

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
RESULT

// tests for class NumberOfBondsPredicate::

CHECK([EXTRA]NumberOfBondsPredicate::operator () (const Atom& atom) const )
	NumberOfBondsPredicate numberOfBonds0;
	numberOfBonds0.setArgument("0");
	NumberOfBondsPredicate numberOfBonds1;
	numberOfBonds1.setArgument("1");
	NumberOfBondsPredicate numberOfBonds2;
	numberOfBonds2.setArgument("2");
	NumberOfBondsPredicate numberOfBonds3;
	numberOfBonds3.setArgument("3");
	NumberOfBondsPredicate numberOfBonds4;
	numberOfBonds4.setArgument("4");
	NumberOfBondsPredicate numberOfBonds5;
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
RESULT

S.destroy();
HINFile g(BALL_TEST_DATA_PATH(Predicate_test.hin));
g >> S;
g.close();

// tests for class DoubleBondsPredicate::

CHECK([EXTRA]DoubleBondsPredicate::operator () (const Atom& atom) const )
	DoubleBondsPredicate doubleBonds0;
	doubleBonds0.setArgument("0");
	DoubleBondsPredicate doubleBonds1;
	doubleBonds1.setArgument("1");
	DoubleBondsPredicate doubleBonds2;
	doubleBonds2.setArgument("2");
	DoubleBondsPredicate doubleBonds3;
	doubleBonds3.setArgument("3");
	DoubleBondsPredicate doubleBonds4;
	doubleBonds4.setArgument("4");
	DoubleBondsPredicate doubleBonds5;
	doubleBonds5.setArgument("5");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(it->getName())
		STATUS(i)
		switch (i) 
		{
			case 4:
			case 5:
			case 6:
				TEST_EQUAL(doubleBonds0(*it), false)
				break;
			default:
				TEST_EQUAL(doubleBonds0(*it), true)
		}
		switch (i) 
		{
			case 4:
			case 6:
				TEST_EQUAL(doubleBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds1(*it), false)
		}
		switch (i) 
		{
			case 5:
				TEST_EQUAL(doubleBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(doubleBonds2(*it), false)
		}
		TEST_EQUAL(doubleBonds3(*it), false)
		TEST_EQUAL(doubleBonds4(*it), false)
		TEST_EQUAL(doubleBonds5(*it), false)
	}
RESULT

// tests for class SingleBondsPredicate::

CHECK([EXTRA]SingleBondsPredicate::operator () (const Atom& atom) const )
	SingleBondsPredicate singleBonds0;
	singleBonds0.setArgument("0");
	SingleBondsPredicate singleBondslt1;
	singleBondslt1.setArgument("<1");
	SingleBondsPredicate singleBonds1;
	singleBonds1.setArgument("1");
	SingleBondsPredicate singleBonds2;
	singleBonds2.setArgument("2");
	SingleBondsPredicate singleBonds3;
	singleBonds3.setArgument("3");
	SingleBondsPredicate singleBonds4;
	singleBonds4.setArgument("4");
	SingleBondsPredicate singleBonds5;
	singleBonds5.setArgument("5");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i);
		switch (i) 
		{
			case 5:
			case 8:
			case 9:
				TEST_EQUAL(singleBonds0(*it), true)
				break;
			default:
				TEST_EQUAL(singleBonds0(*it), false)
		}
		switch (i) 
		{
			case 5:
			case 8:
			case 9:
				TEST_EQUAL(singleBondslt1(*it), true)
				break;
			default:
				TEST_EQUAL(singleBondslt1(*it), false)
		}
		switch (i) 
		{
			case 3:
			case 4:
			case 5:
			case 6:
			case 8:
			case 9:
			case 10:
			case 11:
				TEST_EQUAL(singleBonds1(*it), false)
				break;
			default:
				TEST_EQUAL(singleBonds1(*it), true)
		}
		switch (i) 
		{
			case 4:
			case 6:
				TEST_EQUAL(singleBonds2(*it), true)
				break;
			default:
				TEST_EQUAL(singleBonds2(*it), false)
		}
		TEST_EQUAL(singleBonds3(*it), false)
		switch (i) 
		{
			case 3:
			case 10:
			case 11:
				TEST_EQUAL(singleBonds4(*it), true)
				break;
			default:
				TEST_EQUAL(singleBonds4(*it), false)
		}
		TEST_EQUAL(singleBonds5(*it), false)
	}
RESULT

// tests for class TripleBondsPredicate::

CHECK([EXTRA]TripleBondsPredicate::operator () (const Atom& atom) const )
	TripleBondsPredicate tripleBonds0;
	tripleBonds0.setArgument("0");
	TripleBondsPredicate tripleBonds1;
	tripleBonds1.setArgument("1");
	TripleBondsPredicate tripleBonds2;
	tripleBonds2.setArgument("2");
	TripleBondsPredicate tripleBonds3;
	tripleBonds3.setArgument("3");

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 1:
			case 2:
				TEST_EQUAL(tripleBonds0(*it), false)
				break;
			default:
				TEST_EQUAL(tripleBonds0(*it), true)
		}
		switch (i) 
		{
			case 1:
			case 2:
				TEST_EQUAL(tripleBonds1(*it), true)
				break;
			default:
				TEST_EQUAL(tripleBonds1(*it), false)
		}
		TEST_EQUAL(tripleBonds2(*it), false)
		TEST_EQUAL(tripleBonds3(*it), false)
	}
RESULT

// tests for class SpHybridizedPredicate::

CHECK([EXTRA]SpHybridizedPredicate::operator () (const Atom& atom) const )
	SpHybridizedPredicate isSp;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 1:
			case 2:
			case 5:
				TEST_EQUAL(isSp(*it), true)
				break;
			default:
				TEST_EQUAL(isSp(*it), false)
		}
	}
RESULT


// tests for class sp2HybridizedPredicate::

CHECK([EXTRA]Sp2HybridizedPredicate::operator () (const Atom& atom) const )
	Sp2HybridizedPredicate isSp2;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 4:
			case 6:
			case 7:
				TEST_EQUAL(isSp2(*it), true)
				break;
			default:
				TEST_EQUAL(isSp2(*it), false)
		}
	}
RESULT


// tests for class sp3HybridizedPredicate::
CHECK([EXTRA]Sp3HybridizedPredicate::operator () (const Atom& atom) const )
	Sp3HybridizedPredicate isSp3;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		switch (i) 
		{
			case 3:
			case 10:
			case 11:
				TEST_EQUAL(isSp3(*it), true)
				break;
			default:
				TEST_EQUAL(isSp3(*it), false)
		}
	}
RESULT


CHECK([EXTRA]ChargePredicate::operator ())
  // ???
RESULT

S.clear();
f.open(BALL_TEST_DATA_PATH(LacNAc.hin));
f >> S;
f.close();

CHECK([EXTRA]AxialPredicate::operator () (const Atom& atom) )
	AxialPredicate isAxial;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		STATUS(it->getFullName())
		switch (i) 
		{
			case 27:
				TEST_EQUAL(isAxial(*it), true)
				break;
			default:
				TEST_EQUAL(isAxial(*it), false)
		}
	}
RESULT

CHECK([EXTRA]Conformation4C1Predicate::operator () (const Atom& atom) )
	Conformation4C1Predicate is_4C1;

	AtomIterator it = S.beginAtom();
	for (Size i=1; +it; ++it, ++i)
	{
		STATUS(i)
		STATUS(it->getFullName())
		switch (i) 
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 11:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 33:
				TEST_EQUAL(is_4C1(*it), true)
				break;
			default:
				TEST_EQUAL(is_4C1(*it), false)
		}
	}
RESULT

CHECK(~ConnectedToPredicate() throw())
	ConnectedToPredicate* ptr = new ConnectedToPredicate;
	delete ptr;
RESULT

CHECK(~InRingPredicate() throw())
	InRingPredicate* ptr = new InRingPredicate;
	delete ptr;
RESULT

CHECK(ConnectedToPredicate() throw())
	ConnectedToPredicate c;
RESULT

CHECK(InRingPredicate() throw())
	InRingPredicate i;
RESULT

CHECK(InRingPredicate(Size n) throw())
	InRingPredicate i(1);
RESULT


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

// =====================================================
// Ringfinder
// =====================================================

RingFinder* finder = 0;
CHECK(RingFinder() throw())
	finder = new RingFinder;
	TEST_NOT_EQUAL(finder, 0)
RESULT

CHECK(~RingFinder() throw())
	delete finder;
RESULT

CHECK(RingFinder(Size n) throw())
	RingFinder f(6);
RESULT

CHECK(bool operator () (const Atom& atom) throw())
	RingFinder f;
	f(atom);
RESULT

CHECK(const HashSet<const Bond*>& getVisitedBonds() const throw())
	RingFinder f(6);
	TEST_EQUAL(f.getVisitedBonds().size(), 0)
RESULT

CHECK(const std::vector<const Atom*>& getRingAtoms() const throw())
	RingFinder f(6);
	TEST_EQUAL(f.getRingAtoms().size(), 0)
RESULT

CHECK(bool dfs(const Atom& atom, const Size limit) throw())
	RingFinder f(5);
	TEST_EQUAL(f.dfs(atom, 1), false)
RESULT

CHECK(void setRingSize(Size n) throw())
	RingFinder f;
	f.setRingSize(1);
RESULT

// =====================================================
// not to be tested
// =====================================================
CHECK(::std::list<CTPNode*>& getChildren() throw())
// Not to be tested
RESULT

CHECK(CTPNode() throw())
// Not to be tested
RESULT

CHECK(CTPNode(const CTPNode& node) throw())
// Not to be tested
RESULT

CHECK(CTPNode* getParent() const throw())
// Not to be tested
RESULT

CHECK(ConstIterator begin() const throw())
// Not to be tested
RESULT

CHECK(ConstIterator end() const throw())
// Not to be tested
RESULT

CHECK(Iterator begin() throw())
// Not to be tested
RESULT

CHECK(Iterator end() throw())
// Not to be tested
RESULT

CHECK(Size getBondType() const throw())
// Not to be tested
RESULT

CHECK(Size getNumberOfChildren() const throw())
// Not to be tested
RESULT

CHECK(String getSymbol() const throw())
// Not to be tested
RESULT

CHECK(bool isFinished() const throw())
// Not to be tested
RESULT

CHECK(bool isLinked() const throw())
// Not to be tested
RESULT

CHECK(bool operator () (const Atom& atom) const throw())
// Not to be tested
RESULT

CHECK(char getBondTypeChar() const throw())
// Not to be tested
RESULT

CHECK(const HashSet<const CTPNode*>& getLinkSet() const throw())
// Not to be tested
RESULT

CHECK(void addChild(CTPNode* child) throw())
// Not to be tested
RESULT

CHECK(void clear() throw())
// Not to be tested
RESULT

CHECK(void destroy() throw())
// Not to be tested
RESULT

CHECK(void dump() const throw())
// Not to be tested
RESULT

CHECK(void dump(const CTPNode* current) const throw())
// Not to be tested
RESULT

CHECK(void linkWith(CTPNode* partner) throw())
// Not to be tested
RESULT

CHECK(void removeChild(CTPNode* child) throw())
// Not to be tested
RESULT

CHECK(void setArgument(const String& argument) throw())
// Not to be tested
RESULT

CHECK(void setBondType(Size type) throw())
// Not to be tested
RESULT

CHECK(void setBondType(char type) throw())
// Not to be tested
RESULT

CHECK(void setFinished() throw())
// Not to be tested
RESULT

CHECK(void setLinked() throw())
// Not to be tested
RESULT

CHECK(void setParent(CTPNode* parent) throw())
// Not to be tested
RESULT

CHECK(void setSymbol(const String& symbol) throw())
// Not to be tested
RESULT

CHECK(void unsetFinished() throw())
// Not to be tested
RESULT

CHECK(void unsetLinked() throw())
// Not to be tested
RESULT

CHECK(~CTPNode() throw())
// Not to be tested
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
