// $Id: ForEach_test.C,v 1.2 2001/06/29 18:49:04 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(class_name, "$Id: ForEach_test.C,v 1.2 2001/06/29 18:49:04 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

System s1;

Molecule m1, m2;
m1.setName("m1");
m2.setName("m2");
s1.insert(m1);
s1.insert(m2);

Protein p1, p2;
p1.setName("p1");
p2.setName("p2");
s1.insert(p1);
s1.insert(p2);

Chain c1, c2;
c1.setName("c1");
c2.setName("c2");
p1.insert(c1);
p2.insert(c2);

SecondaryStructure ss1, ss2;
ss1.setName("ss1");
ss2.setName("ss2");
c1.insert(ss1);
c2.insert(ss2);

Fragment f1, f2;
f1.setName("f1");
f2.setName("f2");
p1.insert(f1);
p2.insert(f2);

Residue r1, r2;
r1.setName("r1");
r2.setName("r2");
ss1.insert(r1);
ss2.insert(r2);

Atom a1, a2, a3;
a1.setName("a1");
a2.setName("a2");
f1.insert(a1);
f2.insert(a2);

PDBAtom pa1, pa2, pa3;
pa1.setName("pa1");
pa2.setName("pa2");
r1.insert(pa1);
r2.insert(pa2);


a1.createBond(a2);
a1.getBond(0)->setName("[a1a2]");
a1.createBond(a3);
a1.getBond(1)->setName("[a1a3]");
a1.createBond(pa1);
a1.getBond(2)->setName("[a1pa1]");

pa1.createBond(pa2);
pa1.getBond(1)->setName("[pa1pa2]");
pa1.createBond(pa3);
pa1.getBond(2)->setName("[pa1pa3]");
pa2.createBond(pa3);
pa2.getBond(1)->setName("[pa2pa3]");
a3.createBond(pa3);
a3.getBond(1)->setName("[a3b0]");

AtomIterator ai1, ai2;
AtomBondIterator bi;

String test;

CHECK(BALL_FOREACH_MOLECULE)
	MoleculeIterator mi;
	BALL_FOREACH_MOLECULE(s1, mi) test += mi->getName();
	TEST_EQUAL(test, "m1m2p1p2")
RESULT


CHECK(BALL_FOREACH_FRAGMENT)
	FragmentIterator fi;
	test.clear();
	BALL_FOREACH_FRAGMENT(s1, fi) test += fi->getName();	
	TEST_EQUAL(test, "r1f1r2f2")
RESULT


CHECK(BALL_FOREACH_ATOMCONTAINER)
	AtomContainerIterator aci;
	test.clear();
	BALL_FOREACH_ATOMCONTAINER(s1, aci) test += aci->getName();
	TEST_EQUAL(test, "m1m2p1c1ss1r1f1p2c2ss2r2f2")
RESULT


CHECK(BALL_FOREACH_ATOM)
	test.clear();
	BALL_FOREACH_ATOM(s1, ai1) test += ai1->getName();
	TEST_EQUAL(test, "pa1a1pa2a2")
RESULT

CHECK(BALL_FOREACH_ATOM_BOND)
	test.clear();
	BALL_FOREACH_ATOM_BOND(a1, bi) test += bi->getName();
	TEST_EQUAL(test, "[a1a2][a1a3][a1pa1]")
RESULT


CHECK(BALL_FOREACH_ATOM_PAIR)
	test.clear();
	BALL_FOREACH_ATOM_PAIR(s1, ai1, ai2) { test += ai1->getName(); test += ai2->getName(); }
	TEST_EQUAL(test, "pa1pa2a1pa1a1pa2a1a2a2pa1a2pa2")
RESULT


CHECK(BALL_FOREACH_BOND)
	test.clear();
	BALL_FOREACH_BOND(s1, ai1, bi) 
	{ 
		test += ai1->getName(); 
		test += bi->getName(); 
	}
	TEST_EQUAL(test, "pa1[pa1pa2]pa1[pa1pa3]a1[a1a2]a1[a1a3]a1[a1pa1]pa2[pa2pa3]")
RESULT


CHECK(BALL_FOREACH_INTERBOND)
	test.clear();
	BALL_FOREACH_INTERBOND(s1, ai1, bi) { test += ai1->getName(); test += bi->getName(); }
	TEST_EQUAL(test, "pa1[pa1pa3]a1[a1a3]pa2[pa2pa3]")
RESULT


CHECK(BALL_FOREACH_INTRABOND)
	test.clear();
	BALL_FOREACH_INTRABOND(s1, ai1, bi) { test += ai1->getName(); test += bi->getName(); }
	TEST_EQUAL(test, "pa1[pa1pa2]a1[a1a2]a1[a1pa1]")
RESULT


CHECK(BALL_FOREACH_PROTEIN)
	ProteinIterator pi;
	test.clear();
	BALL_FOREACH_PROTEIN(s1, pi) test += pi->getName();
	TEST_EQUAL(test, "p1p2")
RESULT


CHECK(BALL_FOREACH_CHAIN)
	ChainIterator ci;
	test.clear();
	BALL_FOREACH_CHAIN(s1, ci) test += ci->getName();
	TEST_EQUAL(test, "c1c2")
RESULT


CHECK(BALL_FOREACH_SECONDARYSTRUCTURE)
	SecondaryStructureIterator ssi;
	test.clear();
	BALL_FOREACH_SECONDARYSTRUCTURE(s1, ssi) test += ssi->getName();
	TEST_EQUAL(test, "ss1ss2")
RESULT


CHECK(BALL_FOREACH_RESIDUE)
	ResidueIterator ri;
	test.clear();
	BALL_FOREACH_RESIDUE(s1, ri) test += ri->getName();
	TEST_EQUAL(test, "r1r2")
RESULT


CHECK(BALL_FOREACH_PDBATOM)
	PDBAtomIterator pai;
	test.clear();
	BALL_FOREACH_PDBATOM(p1, pai) test += pai->getName();
	TEST_EQUAL(test, "pa1")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
