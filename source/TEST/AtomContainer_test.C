// $Id: AtomContainer_test.C,v 1.1 2000/08/30 19:59:14 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(AtomContainer, "$Id: AtomContainer_test.C,v 1.1 2000/08/30 19:59:14 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)


AtomContainer*	bf;
CHECK(AtomContainer())
	bf = new AtomContainer;
	TEST_NOT_EQUAL(bf, 0)
RESULT

CHECK(~AtomContainer())
	delete bf;
RESULT

CHECK(setName(String&))
	bf = new AtomContainer;
	bf->setName("testname");
RESULT

CHECK(getName())
	TEST_EQUAL(bf->getName(), "testname")
	delete bf;
RESULT

CHECK(AtomContainer(AtomContainer&, bool))
	AtomContainer* bf1 = new AtomContainer;
	bf1->setName("testname");
	Atom a;
	a.setName("a");
	bf1->insert(a);
	AtomContainer* bf2 = new AtomContainer(*bf1, true);
	TEST_NOT_EQUAL(bf2, 0)
	if (bf2 != 0)
	{
		TEST_EQUAL(bf2->getName(), "testname")
		TEST_EQUAL(bf2->getAtom(0)->getName(), "a")
		delete bf2;
	}
	bf2 = new AtomContainer(*bf1, false);
	TEST_NOT_EQUAL(bf2, 0)
	if (bf2 != 0)
	{
		TEST_EQUAL(bf2->getName(), "testname")
		delete bf2;
	}
	delete bf1;
RESULT

CHECK(AtomContainer(String&))
	AtomContainer* bf1 = new AtomContainer("hello");
	TEST_NOT_EQUAL(bf1, 0)
	if (bf1 != 0)
	{
		TEST_EQUAL(bf1->getName(), "hello")
		delete bf1;
	}
RESULT

CHECK(insert(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	bf1.insert(bf2);
RESULT

CHECK(remove(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	bf1.insert(bf2);
	bf1.remove(bf2);
RESULT

CHECK(countAtomContainers())
	AtomContainer bf1;
	AtomContainer bf2;
	TEST_EQUAL(bf1.countAtomContainers(), 0)
	bf1.insert(bf2);
	TEST_EQUAL(bf1.countAtomContainers(), 1)
	bf1.remove(bf2);
	TEST_EQUAL(bf1.countAtomContainers(), 0)
	bf1.insert(bf1);
	TEST_EQUAL(bf1.countAtomContainers(), 0)
	bf1.insert(bf2);
	TEST_EQUAL(bf1.countAtomContainers(), 1)
	bf2.insert(bf1);
	TEST_EQUAL(bf1.countAtomContainers(), 1)
	TEST_EQUAL(bf2.countAtomContainers(), 0)
	bf1.remove(bf2);
	TEST_EQUAL(bf1.countAtomContainers(), 0)
	TEST_EQUAL(bf2.countAtomContainers(), 0)
	bf2.remove(bf1);
	TEST_EQUAL(bf1.countAtomContainers(), 0)
	TEST_EQUAL(bf2.countAtomContainers(), 0)
RESULT

CHECK(getAtomContainer(Position))
	AtomContainer bf1("bf1");
	AtomContainer bf2("bf2");
	AtomContainer* bf3;
	bf3 = bf1.getAtomContainer(0);
	TEST_EQUAL(bf3, 0)
	bf1.insert(bf2);
	bf3 = bf1.getAtomContainer(0);
	TEST_NOT_EQUAL(bf3, 0)
	if (bf3 != 0)
	{
		TEST_EQUAL(bf3->getName(), "bf2")
	}
	bf3 = bf1.getAtomContainer(1);
	TEST_EQUAL(bf3, 0)
RESULT

CHECK(getAtomContainer(Position) const)
	AtomContainer bf1("bf1");
	AtomContainer bf2("bf2");
	const AtomContainer* bf3;
	bf3 = bf1.getAtomContainer(0);
	TEST_EQUAL(bf3, 0)
	bf1.insert(bf2);
	bf3 = bf1.getAtomContainer(0);
	TEST_NOT_EQUAL(bf3, 0)
	if (bf3 != 0)
	{
		TEST_EQUAL(bf3->getName(), "bf2")
	}
	bf3 = bf1.getAtomContainer(1);
	TEST_EQUAL(bf3, 0)
RESULT

CHECK(clear())
	AtomContainer bf1("bf1");
	AtomContainer bf2("bf2");
	AtomContainer bf3("bf2");
	bf1.insert(bf2);
	bf2.insert(bf3);
	bf2.clear();
	TEST_EQUAL(bf1.getName(), "bf1")
	TEST_EQUAL(bf2.getName(), "")
	TEST_EQUAL(bf3.getName(), "")
	TEST_EQUAL(bf1.countAtomContainers(), 1)
	TEST_EQUAL(bf2.countAtomContainers(), 0)
RESULT

CHECK(destroy())
	AtomContainer bf1("bf1");
	AtomContainer bf2("bf2");
	AtomContainer bf3("bf2");
	bf1.insert(bf2);
	bf2.insert(bf3);
	bf2.destroy();
	TEST_EQUAL(bf1.getName(), "bf1")
	TEST_EQUAL(bf2.getName(), "")
	TEST_EQUAL(bf3.getName(), "")
	TEST_EQUAL(bf1.countAtomContainers(), 0)
	TEST_EQUAL(bf2.countAtomContainers(), 0)
RESULT

CHECK(clone(bool))
	AtomContainer* bf1 = new AtomContainer;
	Composite*	composite = (Composite*)bf1;
	bf1->setName("hello");

	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);

	bf1->insert(a1);
	bf1->insert(a2);
	bf1->insert(a3);
	bf1->insert(a4);

	TEST_EQUAL(bf1->countBonds(), 3)
	TEST_EQUAL(bf1->countAtoms(), 4)

	AtomContainer* bf2 = (AtomContainer*)composite->create(true);
	TEST_NOT_EQUAL(bf2, 0)
	if (bf2 != 0)
	{
		TEST_EQUAL(bf2->getName(), "hello")
		TEST_EQUAL(bf2->countBonds(), 3)
		TEST_EQUAL(bf2->countAtoms(), 4)
		delete bf2;
	}
	bf2 = (AtomContainer*)composite->create(false);
	TEST_NOT_EQUAL(bf2, 0)
	if (bf2 != 0)
	{
		TEST_EQUAL(bf2->getName(), "hello")
		TEST_EQUAL(bf2->countBonds(), 0)
		TEST_EQUAL(bf2->countAtoms(), 0)
		delete bf2;
	}
	delete bf1;
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<AtomContainer>(), AtomContainer::createDefault);
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), File::OUT);
	AtomContainer* f1 = new AtomContainer("name1");
	AtomContainer* f2 = new AtomContainer("name2");
	AtomContainer* f3 = new AtomContainer("name3");
	f1->insert(*f2);
	f1->insert(*f3);
	pm.setOstream(ofile);
	*f1 >> pm;
	ofile.close();
	delete f1;
RESULT

CHECK(persistentRead(PersistenceManager&))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<AtomContainer>(*ptr), true)
		AtomContainer*	f1 = castTo<AtomContainer>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countAtomContainers(), 2)
		TEST_EQUAL(f1->getAtomContainer(0)->getName(), "name2")
		TEST_EQUAL(f1->getAtomContainer(1)->getName(), "name3")
		delete f1;
	} 
	else 
	{
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(set(AtomContainer&, bool))
	AtomContainer bf1("name1");
	AtomContainer bf2;
	bf1.insert(bf2);
	AtomContainer bf3;
	bf3.set(bf1, false);
	TEST_EQUAL(bf3.getName(), "name1");
	TEST_EQUAL(bf3.countAtomContainers(), 0);
	bf1.setName("name2");
	bf3.set(bf1, true);
	TEST_EQUAL(bf3.getName(), "name2");
	TEST_EQUAL(bf3.countAtomContainers(), 1);
RESULT

CHECK(operator = (AtomContainer&))
	AtomContainer bf1("name1");
	Atom a;
	bf1.insert(a);
	AtomContainer bf2;
	bf2 = bf1;
	TEST_EQUAL(bf2.getName(), "name1");
	TEST_EQUAL(bf2.countAtoms(), 1);
RESULT

CHECK(get(AtomContainer&, bool))
	AtomContainer bf1("name1");
	AtomContainer bf2;
	bf1.insert(bf2);
	AtomContainer bf3;
	bf1.get(bf3, false);
	TEST_EQUAL(bf3.getName(), "name1");
	TEST_EQUAL(bf3.countAtomContainers(), 0);
	bf1.setName("name2");
	bf1.get(bf3, true);
	TEST_EQUAL(bf3.getName(), "name2");
	TEST_EQUAL(bf3.countAtomContainers(), 1);
RESULT

CHECK(swap(AtomContainer&))
	AtomContainer bf1("bf1");
	AtomContainer bf2("bf2");
	AtomContainer bf3("bf3");
	AtomContainer bf4("bf4");
	bf1.insert(bf2);
	bf3.insert(bf4);
	TEST_EQUAL(bf1.getName(), "bf1")
	TEST_EQUAL(bf1.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf3.getName(), "bf3")
	TEST_EQUAL(bf3.getAtomContainer(0), &bf4)
	bf1.swap(bf3);
	TEST_EQUAL(bf1.getName(), "bf3")
	TEST_EQUAL(bf1.getAtomContainer(0), &bf4)
	TEST_EQUAL(bf3.getName(), "bf1")
	TEST_EQUAL(bf3.getAtomContainer(0), &bf2)
	bf1.swap(bf3);
	TEST_EQUAL(bf1.getName(), "bf1")
	TEST_EQUAL(bf1.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf3.getName(), "bf3")
	TEST_EQUAL(bf3.getAtomContainer(0), &bf4)
RESULT

CHECK(getMolecule())
	AtomContainer bf1("bf1");
	Molecule* ptr;
	ptr = bf1.getMolecule();
	TEST_EQUAL(ptr, 0)
	Molecule m;
	m.insert(bf1);
	ptr = bf1.getMolecule();
	TEST_EQUAL(ptr, &m)
	m.remove(bf1);
	AtomContainer bf2;
	bf2.insert(bf1);
	m.insert(bf2);
	ptr = bf1.getMolecule();
	TEST_EQUAL(ptr, &m)
	ptr = bf2.getMolecule();
	TEST_EQUAL(ptr, &m)
RESULT

CHECK(getMolecule() const)
	AtomContainer bf1("bf1");
	TEST_EQUAL(bf1.getMolecule(), 0)
	Molecule m;
	m.insert(bf1);
	TEST_EQUAL(bf1.getMolecule(), &m)
	m.remove(bf1);
	AtomContainer bf2;
	bf2.insert(bf1);
	m.insert(bf2);
	TEST_EQUAL(bf2.getMolecule(), &m)
	TEST_EQUAL(bf1.getMolecule(), &m)
RESULT

CHECK(getSuperAtomContainer())
	AtomContainer bf1("bf1");
	AtomContainer* ptr = bf1.getSuperAtomContainer();
	TEST_EQUAL(ptr, 0)
	AtomContainer bf2;
	AtomContainer bf3;
	bf2.insert(bf3);
	bf1.insert(bf2);
	ptr = bf3.getSuperAtomContainer();
	TEST_EQUAL(ptr, &bf2)
	ptr = bf2.getSuperAtomContainer();
	TEST_EQUAL(ptr, &bf1)
RESULT

CHECK(getSuperAtomContainer() const)
	AtomContainer bf1("bf1");
	const AtomContainer* ptr = bf1.getSuperAtomContainer();
	TEST_EQUAL(ptr, 0)
	AtomContainer bf2;
	AtomContainer bf3;
	bf2.insert(bf3);
	bf1.insert(bf2);
	ptr = bf3.getSuperAtomContainer();
	TEST_EQUAL(ptr, &bf2)
	ptr = bf2.getSuperAtomContainer();
	TEST_EQUAL(ptr, &bf1)
RESULT

CHECK(insert(Atom&))
	AtomContainer bf1;
	Atom a;
	bf1.insert(a);
RESULT

CHECK(remove(Atom&))
	AtomContainer bf1;
	Atom a;
	bf1.insert(a);
	bf1.remove(a);
RESULT

CHECK(getAtom(Position))
	AtomContainer bf1;
	Atom a;
	Atom* ptr = bf1.getAtom(0);
	TEST_EQUAL(ptr, 0)
	bf1.insert(a);
	ptr = bf1.getAtom(0);
	TEST_EQUAL(ptr, &a)
	bf1.remove(a);
	ptr = bf1.getAtom(0);
	TEST_EQUAL(ptr, 0)
	ptr = bf1.getAtom(25);
	TEST_EQUAL(ptr, 0)
RESULT

CHECK(getAtom(Position) const)
	AtomContainer bf1;
	Atom a;
	TEST_EQUAL(bf1.getAtom(0), 0)
	bf1.insert(a);
	TEST_EQUAL(bf1.getAtom(0), &a)
	bf1.remove(a);
	TEST_EQUAL(bf1.getAtom(0), 0)
	TEST_EQUAL(bf1.getAtom(24), 0)
RESULT

CHECK(countAtoms())
	AtomContainer bf1;
	AtomContainer bf2;
	bf1.insert(bf2);
	Atom a1;
	Atom a2;
	Atom a3;
	TEST_EQUAL(bf1.countAtoms(), 0)
	TEST_EQUAL(bf2.countAtoms(), 0)
	bf2.insert(a1);
	TEST_EQUAL(bf1.countAtoms(), 1)
	TEST_EQUAL(bf2.countAtoms(), 1)
	bf1.insert(a2);
	TEST_EQUAL(bf1.countAtoms(), 2)
	TEST_EQUAL(bf2.countAtoms(), 1)
	bf2.insert(a3);
	TEST_EQUAL(bf1.countAtoms(), 3)
	TEST_EQUAL(bf2.countAtoms(), 2)
	bf1.remove(bf2);
	TEST_EQUAL(bf1.countAtoms(), 1)
	TEST_EQUAL(bf2.countAtoms(), 2)
RESULT

CHECK(countBonds())
	AtomContainer bf1;
	AtomContainer bf2;
	Atom a1;
	Atom a2;
	Atom a3;
	Atom a4;
	bf1.insert(a1);
	bf1.insert(a2);
	bf2.insert(a3);
	bf2.insert(a4);
	TEST_EQUAL(bf1.countBonds(), 0);
	TEST_EQUAL(bf2.countBonds(), 0);
	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	a1.createBond(a4);
	TEST_EQUAL(bf1.countBonds(), 3);
	TEST_EQUAL(bf2.countBonds(), 3);
RESULT

CHECK(countInterBonds())
	AtomContainer bf1;
	AtomContainer bf2;
	Atom a1;
	Atom a2;
	Atom a3;
	Atom a4;
	bf1.insert(a1);
	bf1.insert(a2);
	bf2.insert(a3);
	bf2.insert(a4);
	TEST_EQUAL(bf1.countInterBonds(), 0);
	TEST_EQUAL(bf2.countInterBonds(), 0);
	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	TEST_EQUAL(bf1.countInterBonds(), 1);
	TEST_EQUAL(bf2.countInterBonds(), 1);
RESULT

CHECK(countIntraBonds())
	AtomContainer bf1;
	AtomContainer bf2;
	Atom a1;
	Atom a2;
	Atom a3;
	Atom a4;
	bf1.insert(a1);
	bf1.insert(a2);
	bf2.insert(a3);
	bf2.insert(a4);
	TEST_EQUAL(bf1.countIntraBonds(), 0);
	TEST_EQUAL(bf2.countIntraBonds(), 0);
	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	TEST_EQUAL(bf1.countIntraBonds(), 1);
	TEST_EQUAL(bf2.countIntraBonds(), 1);
RESULT

CHECK(prepend(Atom&))
	AtomContainer bf1;
	Atom a1;
	Atom a2;
	bf1.insert(a1);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	bf1.prepend(a2);
	TEST_EQUAL(bf1.getAtom(0), &a2)
	TEST_EQUAL(bf1.getAtom(1), &a1)
RESULT

CHECK(insert(Atom&))
	AtomContainer bf1;
	Atom a1;
	Atom a2;
	bf1.insert(a1);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	bf1.insert(a2);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a2)
RESULT

CHECK(append(Atom&))
	AtomContainer bf1;
	Atom a1;
	Atom a2;
	bf1.insert(a1);
	bf1.append(a2);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a2)
RESULT

CHECK(insertBefore(Atom&, Composite&))
	AtomContainer bf1;
	Atom a1;
	Atom a2;
	Atom a3;
	bf1.insert(a1);
	bf1.append(a2);
	bf1.insertBefore(a3, a2);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a3)
	TEST_EQUAL(bf1.getAtom(2), &a2)
RESULT

CHECK(insertAfter(Atom&, Composite&))
	AtomContainer bf1;
	Atom a1;
	Atom a2;
	Atom a3;
	bf1.insert(a1);
	bf1.append(a2);
	bf1.insertAfter(a3, a1);
	TEST_EQUAL(bf1.getAtom(0), &a1)
	TEST_EQUAL(bf1.getAtom(1), &a3)
	TEST_EQUAL(bf1.getAtom(2), &a2)
RESULT

CHECK(prepend(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	bf1.prepend(bf2);
	bf1.prepend(bf3);
	TEST_EQUAL(bf1.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf1.getAtomContainer(0), &bf3)
RESULT

CHECK(append(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	bf1.append(bf2);
	bf1.append(bf3);
	TEST_EQUAL(bf1.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf1.getAtomContainer(1), &bf3)
RESULT

CHECK(insertBefore(AtomContainer&, Composite&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	bf1.append(bf2);
	bf1.append(bf3);
	bf1.insertBefore(bf4, bf3);
	TEST_EQUAL(bf1.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf1.getAtomContainer(1), &bf4)
	TEST_EQUAL(bf1.getAtomContainer(2), &bf3)
RESULT

CHECK(insertAfter(AtomContainer&, Composite&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	bf1.append(bf2);
	bf1.append(bf3);
	bf1.insertAfter(bf4, bf2);
	TEST_EQUAL(bf1.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf1.getAtomContainer(1), &bf4)
	TEST_EQUAL(bf1.getAtomContainer(2), &bf3)
RESULT

CHECK(spliceBefore(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	bf1.append(bf2);
	bf4.append(bf3);
	bf4.spliceBefore(bf1);
	TEST_EQUAL(bf1.getAtomContainer(0), 0)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf3)
RESULT

CHECK(spliceAfter(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	bf1.append(bf2);
	bf4.append(bf3);
	bf4.spliceAfter(bf1);
	TEST_EQUAL(bf1.getAtomContainer(0), 0)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf3)
RESULT

CHECK(splice(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	AtomContainer bf5;
	AtomContainer bfx;
	AtomContainer bfy;
	bf1.append(bf2);
	bf1.append(bfx);
	bf1.append(bf3);
	bfx.append(bfy);
	bf4.append(bf5);
	bf1.splice(bfx);
	TEST_EQUAL(bf1.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf1.getAtomContainer(1), &bfy)
	TEST_EQUAL(bf1.getAtomContainer(2), &bf3)
	bf1.splice(bf4);
	TEST_EQUAL(bf1.getAtomContainer(0), &bf5)
	TEST_EQUAL(bf1.getAtomContainer(1), &bf2)
RESULT

CHECK(destroyBonds())
	AtomContainer bf1;
	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a4);
	bf1.append(a1);
	bf1.append(a2);
	bf1.destroyBonds();
	TEST_EQUAL(a1.countBonds(), 0)	
	TEST_EQUAL(a2.countBonds(), 0)	
	TEST_EQUAL(a3.countBonds(), 1)	
RESULT

CHECK(isSubAtomContainerOf(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	bf1.append(bf2);
	bf2.append(bf3);
	TEST_EQUAL(bf1.isSubAtomContainerOf(bf2), false)	
	TEST_EQUAL(bf2.isSubAtomContainerOf(bf2), false)	
	TEST_EQUAL(bf2.isSubAtomContainerOf(bf1), true)	
	TEST_EQUAL(bf3.isSubAtomContainerOf(bf1), true)	
RESULT

CHECK(isSuperAtomContainerOf(AtomContainer&))
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	bf1.append(bf2);
	bf2.append(bf3);
	TEST_EQUAL(bf1.isSuperAtomContainerOf(bf2), true)	
	TEST_EQUAL(bf1.isSuperAtomContainerOf(bf3), true)	
	TEST_EQUAL(bf2.isSuperAtomContainerOf(bf2), false)	
	TEST_EQUAL(bf2.isSuperAtomContainerOf(bf1), false)	
	TEST_EQUAL(bf3.isSuperAtomContainerOf(bf1), false)	
RESULT

CHECK(isValid())
	AtomContainer bf1;
	AtomContainer bf2;
	AtomContainer bf3;
	bf1.append(bf2);
	bf2.append(bf3);
	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a3);
	bf1.append(a1);
	bf2.append(a2);
	bf3.append(a3);
	TEST_EQUAL(bf1.isValid(), true)	
	TEST_EQUAL(bf2.isValid(), true)	
	TEST_EQUAL(bf3.isValid(), true)	
RESULT

CHECK(dump(ostream&, Size))
	AtomContainer bf1;
	AtomContainer bf2;
	bf1.setName("BF1");
	bf2.setName("BF2");
	bf1.append(bf2);
	Atom a1;
	a1.setName("A1");
	bf2.append(a1);
	std::ofstream outfile(filename.c_str(), File::OUT);
	bf1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/AtomContainer_test.txt", true)
RESULT

CHECK(read(istream&)) 
// NotImplemented
RESULT

CHECK(write(ostream&))
// NotImplemented
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
