// $Id: System_test.C,v 1.3 2000/05/16 15:16:00 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/atom.h>
START_TEST(System, "$Id: System_test.C,v 1.3 2000/05/16 15:16:00 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
using namespace BALL;
String filename;
NEW_TMP_FILE(filename)

System*	s;
CHECK(default constructor)
s = new System;
TEST_NOT_EQUAL(s, 0);
RESULT

CHECK(isValid)
TEST_EQUAL(s->isValid(), true)
RESULT

CHECK(destructor)
delete s;
s = new System;
delete s;
RESULT

CHECK(System(String&))
	System* f1 = new System("hello");
	TEST_NOT_EQUAL(f1, 0)
	if (f1 != 0)
	{
		TEST_EQUAL(f1->getName(), "hello")
		delete f1;
	}
RESULT

CHECK(System(System&, bool))
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

CHECK(set(System&, bool))
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

CHECK(get(System&, bool))
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

CHECK(operator = (System&))
	System f1("name1");
	Molecule a;
	f1.insert(a);
	System f2;
	f2 = f1;
	TEST_EQUAL(f2.getName(), "name1");
	TEST_EQUAL(f2.countMolecules(), 1);
RESULT

CHECK(getMolecule(Position))
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

CHECK(getMolecule(Position) const)
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

CHECK(countMolecules())
	System s;
	TEST_EQUAL(s.countMolecules(), 0);
	Molecule m;
	s.insert(m);
	TEST_EQUAL(s.countMolecules(), 1);
RESULT

CHECK(countFragments())
	System s;
	TEST_EQUAL(s.countFragments(), 0);
	Molecule m;
	Fragment f;
	s.insert(m);
	m.insert(f);
	TEST_EQUAL(s.countFragments(), 1);
RESULT

CHECK(countAtoms())
	System s;
	TEST_EQUAL(s.countAtoms(), 0);
	Molecule m;
	Atom a;
	s.insert(m);
	m.insert(a);
	TEST_EQUAL(s.countAtoms(), 1);
RESULT

CHECK(countSecondaryStructures())
	System s;
	TEST_EQUAL(s.countSecondaryStructures(), 0);
	Protein p;
	SecondaryStructure f;
	p.insert(f);
	s.insert(p);
	TEST_EQUAL(s.countSecondaryStructures(), 1);
RESULT

CHECK(countChains())
	System s;
	TEST_EQUAL(s.countChains(), 0);
	Protein p;
	Chain f;
	p.insert(f);
	s.insert(p);
	TEST_EQUAL(s.countChains(), 1);
RESULT

CHECK(countNucleotides())
	System s;
	TEST_EQUAL(s.countNucleotides(), 0);
	NucleicAcid n;
	Nucleotide f;
	n.insert(f);
	s.insert(n);
	TEST_EQUAL(s.countNucleotides(), 1);
RESULT

CHECK(countNucleicAcids())
	System s;
	TEST_EQUAL(s.countNucleicAcids(), 0);
	NucleicAcid n;
	s.insert(n);
	TEST_EQUAL(s.countNucleicAcids(), 1);
RESULT

CHECK(prepend(Molecule& molecule))
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

CHECK(append(Molecule& molecule))
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

CHECK(insert(Molecule& molecule))
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

CHECK(insertBefore(Molecule& molecule, Composite& before))
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

CHECK(insertAfter(Molecule& molecule, Composite& after))
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

CHECK(remove(Molecule& molecule))
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


CHECK(spliceBefore(System& system))
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

CHECK(spliceAfter(System& system))
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

CHECK(splice(System& system))
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

CHECK(destroyBonds())
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

/*
TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Composite>(), Composite::createDefault);
pm.registerClass(getStreamName<System>(), System::createDefault);
pm.registerClass(getStreamName<Molecule>(), Molecule::createDefault);
String filename;
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	System* f1 = new System("name1");
	System* f2 = new System("name2");
	f1->insert(*f2);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT
*/
/*
CHECK(persistentRead(PersistenceManager&))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<System>(*ptr), true)
		System*	f1 = castTo<System>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1-> ystems(), 1)
		TEST_EQUAL(f1->getSystem(0)->getName(), "name2")
		delete f1;
	} else {
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT
*/


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
