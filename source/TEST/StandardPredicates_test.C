// $Id: StandardPredicates_test.C,v 1.13 2000/05/26 17:15:31 anker Exp $
#include <BALL/CONCEPT/classTest.h>

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

START_TEST(standardPredicates, "$Id: StandardPredicates_test.C,v 1.13 2000/05/26 17:15:31 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class AtomNamePredicate::

CHECK(AtomNamePredicate::operator () (const Atom& atom) const )
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

CHECK(AtomTypePredicate::operator () (const Atom& atom) const )
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
CHECK(ElementPredicate::operator () (const Atom& atom) const )
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

CHECK(ResiduePredicate::operator () (const Atom& atom) const )
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

CHECK(ResidueIDPredicate::operator () (const Atom& atom) const )
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

CHECK(ProteinPredicate::operator () (const Atom& atom) const )
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

CHECK(ChainPredicate::operator () (const Atom& atom) const )
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

CHECK(SecondaryStructurePredicate::operator () (const Atom& atom) const )
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

CHECK(SolventPredicate::operator () (const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class MoleculePredicate::

CHECK(MoleculePredicate::operator () (const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class NucleicAcidPredicate::

CHECK(NucleicAcidPredicate::operator () (const Atom& atom) const )
  //BAUSTELLE
RESULT


// tests for class NucleotidePredicate::

CHECK(NucleotidePredicate::operator () (const Atom& atom) const )
  //BAUSTELLE
RESULT

HINFile f("data/L-Tryptophan.hin");
System S;
f >> S;
f.close();

// tests for class inRingPredicate::

// tests for class BackBonePredicate::

CHECK(BackBonePredicate::operator () (const Atom& atom) const )
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


CHECK(InRingPredicate::operator () (const Atom& atom) const )
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
RESULT

// tests for class ConnectedToPredicate::

CHECK(ConnectedToPredicate::operator () (const Atom& atom) const )
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
	STATUS("(C(H))");
	connectedTo.setArgument("(C(H))");
	TEST_EQUAL(connectedTo(*it), true)
	STATUS("(H)(H)(H)(C(H)(C(O)(=O)))");
	connectedTo.setArgument("(H)(H)(H)(C(H)(C(O)(=O)))");
	TEST_EQUAL(connectedTo(*it), false)
	STATUS("(H)(H)(H)(C(H)(C(-O)(-O)))");
	connectedTo.setArgument("(H)(H)(H)(C(H)(C(-O)(-O)))");
	TEST_EQUAL(connectedTo(*it), false)
	STATUS("(H)(H)(H)(C(H)(C(~O)(~O)))");
	connectedTo.setArgument("(H)(H)(H)(C(H)(C(~O)(~O)))");
	TEST_EQUAL(connectedTo(*it), true)
	STATUS("(-H)(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H)(~C)))))")
	connectedTo.setArgument("(-H)(-H)(-H)(-C(-H)(-C(~O)(~O))(-C(-H)(-H)(-C(~C(-H))(~C))))");
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
RESULT

// tests for class AromaticBondsPredicate::

CHECK(AromaticBondsPredicate::operator () (const Atom& atom) const )
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

CHECK(NumberOfBondsPredicate::operator () (const Atom& atom) const )
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
HINFile g("data/Predicate_test.hin");
g >> S;
g.close();

// tests for class DoubleBondsPredicate::

CHECK(DoubleBondsPredicate::operator () (const Atom& atom) const )
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

CHECK(SingleBondsPredicate::operator () (const Atom& atom) const )
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

CHECK(TripleBondsPredicate::operator () (const Atom& atom) const )
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

CHECK(SpHybridizedPredicate::operator () (const Atom& atom) const )
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

CHECK(Sp2HybridizedPredicate::operator () (const Atom& atom) const )
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
CHECK(Sp3HybridizedPredicate::operator () (const Atom& atom) const )
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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
