// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: System_test.C,v 1.13 2003/07/01 15:03:42 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(System, "$Id: System_test.C,v 1.13 2003/07/01 15:03:42 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;
String filename;
NEW_TMP_FILE(filename)

System*	s;
CHECK(System() throw())
s = new System;
TEST_NOT_EQUAL(s, 0);
RESULT

CHECK([EXTRA]isValid)
TEST_EQUAL(s->isValid(), true)
RESULT

CHECK(~System() throw())
delete s;
s = new System;
delete s;
RESULT

CHECK(System(const String& name) throw())
	System* f1 = new System("hello");
	TEST_NOT_EQUAL(f1, 0)
	if (f1 != 0)
	{
		TEST_EQUAL(f1->getName(), "hello")
		delete f1;
	}
RESULT

CHECK(System(const System& system, bool deep = true) throw())
	System* f1 = new System;
	f1->setName("testname");
	Molecule a;
	f1->insert(a);
	System* f2 = new System(*f1, true);
	TEST_NOT_EQUAL(f2, 0)
	if (f2 != 0)
	{
		TEST_EQUAL(f2->getName(), "testname")
		TEST_EQUAL(f2->countMolecules(), 1)
		delete f2;
	}
	f2 = new System(*f1, false);
	TEST_NOT_EQUAL(f2, 0)
	if (f2 != 0)
	{
		TEST_EQUAL(f2->countMolecules(), 0)
		delete f2;
	}
	delete f1;
RESULT

CHECK(void set(const System& system, bool deep = true) throw())
	System s1("name1");
	Molecule m1;
	s1.insert(m1);
	System s3;
	s3.set(s1, false);
	TEST_EQUAL(s3.getName(), "name1");
	TEST_EQUAL(s3.countMolecules(), 0);
	s1.setName("name2");
	s3.set(s1, true);
	TEST_EQUAL(s3.getName(), "name2");
	TEST_EQUAL(s3.countMolecules(), 1);
RESULT

CHECK(void get(System& system, bool deep = true) const throw())
	System s1("name1");
	Molecule m1;
	s1.insert(m1);
	System s3;
	s1.get(s3, false);
	TEST_EQUAL(s3.getName(), "name1");
	TEST_EQUAL(s3.countMolecules(), 0);
	s1.setName("name2");
	s1.get(s3, true);
	TEST_EQUAL(s3.getName(), "name2");
	TEST_EQUAL(s3.countMolecules(), 1);
RESULT

CHECK(Size countProteins() const throw())
	System f1("name1");
	Protein a;
	f1.insert(a);
	TEST_EQUAL(f1.countProteins(), 1);
RESULT

CHECK(Size countResidues() const throw())
	System f1("name1");
	Protein p;
	Chain c;
	Residue r;
	p.insert(c);
	f1.insert(p);
	TEST_EQUAL(f1.countResidues(), 0);
	c.insert(r);
	TEST_EQUAL(f1.countResidues(), 1);
RESULT


CHECK(Molecule* getMolecule(Position position) throw())
	System s1;
	Molecule m;
	Molecule* ptr = s1.getMolecule(0);
	TEST_EQUAL(ptr, 0)
	s1.insert(m);
	s1.getMolecule(0)->setName("m1");
	TEST_EQUAL(m.getName(), "m1")
	s1.remove(m);
	TEST_EQUAL(s1.getMolecule(0), 0)
	TEST_EQUAL(s1.getMolecule(25), 0)
RESULT

CHECK(const Molecule* getMolecule(Position position) const throw())
	System s1;
	Molecule m;
	Molecule* ptr = s1.getMolecule(0);
	TEST_EQUAL(ptr, 0)
	s1.insert(m);
	TEST_EQUAL(s1.getMolecule(0), &m)
	s1.remove(m);
	TEST_EQUAL(s1.getMolecule(0), 0)
	TEST_EQUAL(s1.getMolecule(25), 0)
RESULT

CHECK(Size countMolecules() const throw())
	System s;
	TEST_EQUAL(s.countMolecules(), 0);
	Molecule m;
	s.insert(m);
	TEST_EQUAL(s.countMolecules(), 1);
RESULT

CHECK(Size countFragments() const throw())
	System s;
	TEST_EQUAL(s.countFragments(), 0);
	Molecule m;
	Fragment f;
	s.insert(m);
	m.insert(f);
	TEST_EQUAL(s.countFragments(), 1);
RESULT

CHECK(Size countAtoms() const throw())
	System s;
	TEST_EQUAL(s.countAtoms(), 0);
	Molecule m;
	Atom a;
	s.insert(m);
	m.insert(a);
	TEST_EQUAL(s.countAtoms(), 1);
RESULT

CHECK(Size countSecondaryStructures() const throw())
	System s;
	TEST_EQUAL(s.countSecondaryStructures(), 0);
	Protein p;
	SecondaryStructure f;
	p.insert(f);
	s.insert(p);
	TEST_EQUAL(s.countSecondaryStructures(), 1);
RESULT

CHECK(Size countChains() const throw())
	System s;
	TEST_EQUAL(s.countChains(), 0);
	Protein p;
	Chain f;
	p.insert(f);
	s.insert(p);
	TEST_EQUAL(s.countChains(), 1);
RESULT

CHECK(Size countNucleotides() const throw())
	System s;
	TEST_EQUAL(s.countNucleotides(), 0);
	NucleicAcid n;
	Nucleotide f;
	n.insert(f);
	s.insert(n);
	TEST_EQUAL(s.countNucleotides(), 1);
RESULT

CHECK(Size countNucleicAcids() const throw())
	System s;
	TEST_EQUAL(s.countNucleicAcids(), 0);
	NucleicAcid n;
	s.insert(n);
	TEST_EQUAL(s.countNucleicAcids(), 1);
RESULT

CHECK(void prepend(Molecule& molecule) throw())
	System s;
	Molecule m1;
	Molecule m2;
	s.insert(m1);
	s.prepend(m2);
	MoleculeIterator mol_it = s.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m2)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m1)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT

CHECK(void append(Molecule& molecule) throw())
	System s;
	Molecule m1;
	Molecule m2;
	s.insert(m1);
	s.append(m2);
	MoleculeIterator mol_it = s.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m2)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT

CHECK(void insert(Molecule& molecule) throw())
	System s;
	Molecule m1;
	Molecule m2;
	s.insert(m1);
	s.append(m2);
	MoleculeIterator mol_it = s.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m2)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT

CHECK(void insertBefore(Molecule& molecule, Composite& before) throw())
	System s;
	Molecule m1;
	Molecule m2;
	Molecule m3;
	s.insert(m1);
	s.append(m2);
	s.insertBefore(m3, m2);
	MoleculeIterator mol_it = s.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m3)
				break;
			case 2:
				TEST_EQUAL(&*mol_it, &m2)
				break;

		}
			counter++;
	}
	TEST_EQUAL(counter, 3);
RESULT

CHECK(void insertAfter(Molecule& molecule, Composite& after) throw())
	System s;
	Molecule m1;
	Molecule m2;
	Molecule m3;
	s.append(m1);
	s.insertAfter(m2, m1);
	s.insertAfter(m3, m2);
	MoleculeIterator mol_it = s.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m2)
				break;
			case 2:
				TEST_EQUAL(&*mol_it, &m3)
				break;

		}
			counter++;
	}
	TEST_EQUAL(counter, 3);
RESULT

CHECK(bool remove(Molecule& molecule) throw())
	System s;
	Molecule m1;
	Molecule m2;
	Molecule m3;
	s.append(m1);
	s.append(m2);
	s.append(m3);
	s.remove(m2);
	MoleculeIterator mol_it = s.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m3)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT


CHECK(void spliceBefore(System& system) throw())
	System s1;
	System s2;
	Molecule m1;
	Molecule m2;
	s1.append(m1);
	s2.append(m2);
	s2.spliceBefore(s1);
	MoleculeIterator mol_it = s2.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m2)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT

CHECK(void spliceAfter(System& system) throw())
	System s1;
	System s2;
	Molecule m1;
	Molecule m2;
	s1.append(m1);
	s2.append(m2);
	s2.spliceAfter(s1);
	MoleculeIterator mol_it = s2.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m2)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m1)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT

CHECK(void splice(System& system) throw())
	System s1;
	System s2;
	Molecule m1;
	Molecule m2;
	s1.append(m1);
	s2.append(m2);
	s2.splice(s1);
	MoleculeIterator mol_it = s2.beginMolecule();
	Size counter = 0;
	for (; +mol_it; ++mol_it)
	{
		switch (counter)
		{
			case 0:
				TEST_EQUAL(&*mol_it, &m1)
				break;
			case 1:
				TEST_EQUAL(&*mol_it, &m2)
				break;
		}
			counter++;
	}
	TEST_EQUAL(counter, 2);
RESULT

CHECK([EXTRA]destroyBonds())
	System s1;
	Molecule m1;
	Atom a1, a2, a3, a4;
	s1.append(m1);
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a4);
	m1.append(a1);
	m1.append(a2);
	s1.destroyBonds();
	TEST_EQUAL(a1.countBonds(), 0)	
	TEST_EQUAL(a2.countBonds(), 0)	
	TEST_EQUAL(a3.countBonds(), 1)	
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Composite>(), Composite::createDefault);
pm.registerClass(getStreamName<System>(), System::createDefault);
pm.registerClass(getStreamName<Molecule>(), Molecule::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	System* f1 = new System("name1");
	Molecule* f2 = new Molecule("name2");
	f1->insert(*f2);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<System>(*ptr), true)
		System*	f1 = castTo<System>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countMolecules(), 1)
		TEST_EQUAL(f1->getMolecule(0)->getName(), "name2")
		delete f1;
	} 
	else 
	{
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(bool operator == (const System& system) const throw())
	System c1, c2;
	TEST_EQUAL(c1 == c2, false)

	c1 = c2;
	TEST_EQUAL(c1 == c2, false)

	TEST_EQUAL(c2 == c2, true)
RESULT

CHECK(bool operator != (const System& system) const throw())
	System c1, c2;
	TEST_EQUAL(c1 != c2, true)

	c1 = c2;
	TEST_EQUAL(c1 != c2, true)

	TEST_EQUAL(c2 != c2, false)
RESULT

CHECK([EXTRA]Chain iteration)
	System S;
	Protein p1;
	Protein p2;
	Protein p3;
	Chain c1;
	Chain c2;
	Chain c3;
	Chain c4;
	c1.setName("C1");
	c2.setName("C2");
	c3.setName("C3");
	c4.setName("C4");
	p2.insert(c1);
	p2.insert(c2);
	p2.insert(c3);
	p3.insert(c4);
	S.insert(p1);
	S.insert(p2);
	S.insert(p3);

	ChainIterator ci = S.beginChain();
	TEST_EQUAL(+ci, true)
	ABORT_IF(!+ci)
	TEST_EQUAL(ci->getName(), "C1")
	ci++;
	ABORT_IF(!+ci)
	TEST_EQUAL(ci->getName(), "C2")
	ci++;
	ABORT_IF(!+ci)
	TEST_EQUAL(ci->getName(), "C3")
	ci++;
	ABORT_IF(!+ci)
	TEST_EQUAL(ci->getName(), "C4")
	ci++;
	TEST_EQUAL(+ci, false)
RESULT

CHECK(BALL_CREATE_DEEP(System))
	System S("system1");
	Protein p1;
	S.insert(p1);
	System* test = (System*) S.create(false, true);
	TEST_EQUAL(test->getName(), "")
	TEST_EQUAL(test->countProteins(), 0)
	delete test;
	test = (System*) S.create(true, false);
	TEST_EQUAL(test->getName(), "system1")
	TEST_EQUAL(test->countProteins(), 1)
	delete test;
RESULT


CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Atom)(AtomContainer)(Fragment)(Molecule)(Protein)(Residue)(Chain)(SecondaryStructure) (Nucleotide)(NucleicAcid))
	// ???
RESULT


CHECK(System& operator = (const System& system) throw())
	System S("system1");
	Protein p1;
	S.insert(p1);
	System test;
	test = S;
	TEST_EQUAL(test.getName(), "system1")
	TEST_EQUAL(test.countProteins(), 1)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
