// $Id: AtomContainer_test.C,v 1.4 2001/06/26 19:34:56 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

#include <algorithm>

START_TEST(AtomContainer, "$Id: AtomContainer_test.C,v 1.4 2001/06/26 19:34:56 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)


AtomContainer*	ac;
CHECK(AtomContainer())
	ac = new AtomContainer;
	TEST_NOT_EQUAL(ac, 0)
RESULT

CHECK(~AtomContainer())
	delete ac;
RESULT

CHECK(setName(String&))
	ac = new AtomContainer;
	ac->setName("testname");
RESULT

CHECK(getName())
	TEST_EQUAL(ac->getName(), "testname")
	delete ac;
RESULT

CHECK(AtomContainer(AtomContainer&, bool))
	AtomContainer* ac1 = new AtomContainer;
	ac1->setName("testname");
	Atom a;
	a.setName("a");
	ac1->insert(a);
	AtomContainer* ac2 = new AtomContainer(*ac1, true);
	TEST_NOT_EQUAL(ac2, 0)
	if (ac2 != 0)
	{
		TEST_EQUAL(ac2->getName(), "testname")
		TEST_EQUAL(ac2->getAtom(0)->getName(), "a")
		delete ac2;
	}
	ac2 = new AtomContainer(*ac1, false);
	TEST_NOT_EQUAL(ac2, 0)
	if (ac2 != 0)
	{
		TEST_EQUAL(ac2->getName(), "testname")
		delete ac2;
	}
	delete ac1;
RESULT

CHECK(AtomContainer(String&))
	AtomContainer* ac1 = new AtomContainer("hello");
	TEST_NOT_EQUAL(ac1, 0)
	if (ac1 != 0)
	{
		TEST_EQUAL(ac1->getName(), "hello")
		delete ac1;
	}
RESULT

CHECK(insert(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.insert(ac2);
RESULT

CHECK(remove(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.insert(ac2);
	ac1.remove(ac2);
RESULT

CHECK(countAtomContainers())
	AtomContainer ac1;
	AtomContainer ac2;
	TEST_EQUAL(ac1.countAtomContainers(), 0)
	ac1.insert(ac2);
	TEST_EQUAL(ac1.countAtomContainers(), 1)
	ac1.remove(ac2);
	TEST_EQUAL(ac1.countAtomContainers(), 0)
	ac1.insert(ac1);
	TEST_EQUAL(ac1.countAtomContainers(), 0)
	ac1.insert(ac2);
	TEST_EQUAL(ac1.countAtomContainers(), 1)
	ac2.insert(ac1);
	TEST_EQUAL(ac1.countAtomContainers(), 1)
	TEST_EQUAL(ac2.countAtomContainers(), 0)
	ac1.remove(ac2);
	TEST_EQUAL(ac1.countAtomContainers(), 0)
	TEST_EQUAL(ac2.countAtomContainers(), 0)
	ac2.remove(ac1);
	TEST_EQUAL(ac1.countAtomContainers(), 0)
	TEST_EQUAL(ac2.countAtomContainers(), 0)
RESULT

CHECK(getAtomContainer(Position))
	AtomContainer ac1("ac1");
	AtomContainer ac2("ac2");
	AtomContainer* ac3;
	ac3 = ac1.getAtomContainer(0);
	TEST_EQUAL(ac3, 0)
	ac1.insert(ac2);
	ac3 = ac1.getAtomContainer(0);
	TEST_NOT_EQUAL(ac3, 0)
	if (ac3 != 0)
	{
		TEST_EQUAL(ac3->getName(), "ac2")
	}
	ac3 = ac1.getAtomContainer(1);
	TEST_EQUAL(ac3, 0)
RESULT

CHECK(getAtomContainer(Position) const)
	AtomContainer ac1("ac1");
	AtomContainer ac2("ac2");
	const AtomContainer* ac3;
	ac3 = ac1.getAtomContainer(0);
	TEST_EQUAL(ac3, 0)
	ac1.insert(ac2);
	ac3 = ac1.getAtomContainer(0);
	TEST_NOT_EQUAL(ac3, 0)
	if (ac3 != 0)
	{
		TEST_EQUAL(ac3->getName(), "ac2")
	}
	ac3 = ac1.getAtomContainer(1);
	TEST_EQUAL(ac3, 0)
RESULT

CHECK(clear())
	AtomContainer ac1("ac1");
	AtomContainer ac2("ac2");
	AtomContainer ac3("ac2");
	ac1.insert(ac2);
	ac2.insert(ac3);
	ac2.clear();
	TEST_EQUAL(ac1.getName(), "ac1")
	TEST_EQUAL(ac2.getName(), "")
	TEST_EQUAL(ac3.getName(), "")
	TEST_EQUAL(ac1.countAtomContainers(), 1)
	TEST_EQUAL(ac2.countAtomContainers(), 0)
RESULT

CHECK(destroy())
	AtomContainer ac1("ac1");
	AtomContainer ac2("ac2");
	AtomContainer ac3("ac2");
	ac1.insert(ac2);
	ac2.insert(ac3);
	ac2.destroy();
	TEST_EQUAL(ac1.getName(), "ac1")
	TEST_EQUAL(ac2.getName(), "")
	TEST_EQUAL(ac3.getName(), "")
	TEST_EQUAL(ac1.countAtomContainers(), 0)
	TEST_EQUAL(ac2.countAtomContainers(), 0)
RESULT

CHECK(clone(bool))
	AtomContainer* ac1 = new AtomContainer;
	Composite*	composite = (Composite*)ac1;
	ac1->setName("hello");

	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);

	ac1->insert(a1);
	ac1->insert(a2);
	ac1->insert(a3);
	ac1->insert(a4);

	TEST_EQUAL(ac1->countBonds(), 3)
	TEST_EQUAL(ac1->countAtoms(), 4)

	AtomContainer* ac2 = (AtomContainer*)composite->create(true);
	TEST_NOT_EQUAL(ac2, 0)
	if (ac2 != 0)
	{
		TEST_EQUAL(ac2->getName(), "hello")
		TEST_EQUAL(ac2->countBonds(), 3)
		TEST_EQUAL(ac2->countAtoms(), 4)
		delete ac2;
	}
	ac2 = (AtomContainer*)composite->create(false);
	TEST_NOT_EQUAL(ac2, 0)
	if (ac2 != 0)
	{
		TEST_EQUAL(ac2->getName(), "hello")
		TEST_EQUAL(ac2->countBonds(), 0)
		TEST_EQUAL(ac2->countAtoms(), 0)
		delete ac2;
	}
	delete ac1;
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
	AtomContainer ac1("name1");
	AtomContainer ac2;
	ac1.insert(ac2);
	AtomContainer ac3;
	ac3.set(ac1, false);
	TEST_EQUAL(ac3.getName(), "name1");
	TEST_EQUAL(ac3.countAtomContainers(), 0);
	ac1.setName("name2");
	ac3.set(ac1, true);
	TEST_EQUAL(ac3.getName(), "name2");
	TEST_EQUAL(ac3.countAtomContainers(), 1);
RESULT

CHECK(operator = (AtomContainer&))
	AtomContainer ac1("name1");
	Atom a;
	ac1.insert(a);
	AtomContainer ac2;
	ac2 = ac1;
	TEST_EQUAL(ac2.getName(), "name1");
	TEST_EQUAL(ac2.countAtoms(), 1);
RESULT

CHECK(get(AtomContainer&, bool))
	AtomContainer ac1("name1");
	AtomContainer ac2;
	ac1.insert(ac2);
	AtomContainer ac3;
	ac1.get(ac3, false);
	TEST_EQUAL(ac3.getName(), "name1");
	TEST_EQUAL(ac3.countAtomContainers(), 0);
	ac1.setName("name2");
	ac1.get(ac3, true);
	TEST_EQUAL(ac3.getName(), "name2");
	TEST_EQUAL(ac3.countAtomContainers(), 1);
RESULT

CHECK(swap(AtomContainer&))
	AtomContainer ac1("ac1");
	AtomContainer ac2("ac2");
	AtomContainer ac3("ac3");
	AtomContainer ac4("ac4");
	ac1.insert(ac2);
	ac3.insert(ac4);
	TEST_EQUAL(ac1.getName(), "ac1")
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac3.getName(), "ac3")
	TEST_EQUAL(ac3.getAtomContainer(0), &ac4)
	ac1.swap(ac3);
	TEST_EQUAL(ac1.getName(), "ac3")
	TEST_EQUAL(ac1.getAtomContainer(0), &ac4)
	TEST_EQUAL(ac3.getName(), "ac1")
	TEST_EQUAL(ac3.getAtomContainer(0), &ac2)
	ac1.swap(ac3);
	TEST_EQUAL(ac1.getName(), "ac1")
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac3.getName(), "ac3")
	TEST_EQUAL(ac3.getAtomContainer(0), &ac4)
RESULT

CHECK(getMolecule())
	AtomContainer ac1("ac1");
	Molecule* ptr;
	ptr = ac1.getMolecule();
	TEST_EQUAL(ptr, 0)
	Molecule m;
	m.insert(ac1);
	ptr = ac1.getMolecule();
	TEST_EQUAL(ptr, &m)
	m.remove(ac1);
	AtomContainer ac2;
	ac2.insert(ac1);
	m.insert(ac2);
	ptr = ac1.getMolecule();
	TEST_EQUAL(ptr, &m)
	ptr = ac2.getMolecule();
	TEST_EQUAL(ptr, &m)
RESULT

CHECK(getMolecule() const)
	AtomContainer ac1("ac1");
	TEST_EQUAL(ac1.getMolecule(), 0)
	Molecule m;
	m.insert(ac1);
	TEST_EQUAL(ac1.getMolecule(), &m)
	m.remove(ac1);
	AtomContainer ac2;
	ac2.insert(ac1);
	m.insert(ac2);
	TEST_EQUAL(ac2.getMolecule(), &m)
	TEST_EQUAL(ac1.getMolecule(), &m)
RESULT

CHECK(getSuperAtomContainer())
	AtomContainer ac1("ac1");
	AtomContainer* ptr = ac1.getSuperAtomContainer();
	TEST_EQUAL(ptr, 0)
	AtomContainer ac2;
	AtomContainer ac3;
	ac2.insert(ac3);
	ac1.insert(ac2);
	ptr = ac3.getSuperAtomContainer();
	TEST_EQUAL(ptr, &ac2)
	ptr = ac2.getSuperAtomContainer();
	TEST_EQUAL(ptr, &ac1)
RESULT

CHECK(getSuperAtomContainer() const)
	AtomContainer ac1("ac1");
	const AtomContainer* ptr = ac1.getSuperAtomContainer();
	TEST_EQUAL(ptr, 0)
	AtomContainer ac2;
	AtomContainer ac3;
	ac2.insert(ac3);
	ac1.insert(ac2);
	ptr = ac3.getSuperAtomContainer();
	TEST_EQUAL(ptr, &ac2)
	ptr = ac2.getSuperAtomContainer();
	TEST_EQUAL(ptr, &ac1)
RESULT

CHECK(insert(Atom&))
	AtomContainer ac1;
	Atom a;
	ac1.insert(a);
RESULT

CHECK(remove(Atom&))
	AtomContainer ac1;
	Atom a;
	ac1.insert(a);
	ac1.remove(a);
RESULT

CHECK(getAtom(Position))
	AtomContainer ac1;
	Atom a;
	Atom* ptr = ac1.getAtom(0);
	TEST_EQUAL(ptr, 0)
	ac1.insert(a);
	ptr = ac1.getAtom(0);
	TEST_EQUAL(ptr, &a)
	ac1.remove(a);
	ptr = ac1.getAtom(0);
	TEST_EQUAL(ptr, 0)
	ptr = ac1.getAtom(25);
	TEST_EQUAL(ptr, 0)
RESULT

CHECK(getAtom(Position) const)
	AtomContainer ac1;
	Atom a;
	TEST_EQUAL(ac1.getAtom(0), 0)
	ac1.insert(a);
	TEST_EQUAL(ac1.getAtom(0), &a)
	ac1.remove(a);
	TEST_EQUAL(ac1.getAtom(0), 0)
	TEST_EQUAL(ac1.getAtom(24), 0)
RESULT

CHECK(countAtoms())
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.insert(ac2);
	Atom a1;
	Atom a2;
	Atom a3;
	TEST_EQUAL(ac1.countAtoms(), 0)
	TEST_EQUAL(ac2.countAtoms(), 0)
	ac2.insert(a1);
	TEST_EQUAL(ac1.countAtoms(), 1)
	TEST_EQUAL(ac2.countAtoms(), 1)
	ac1.insert(a2);
	TEST_EQUAL(ac1.countAtoms(), 2)
	TEST_EQUAL(ac2.countAtoms(), 1)
	ac2.insert(a3);
	TEST_EQUAL(ac1.countAtoms(), 3)
	TEST_EQUAL(ac2.countAtoms(), 2)
	ac1.remove(ac2);
	TEST_EQUAL(ac1.countAtoms(), 1)
	TEST_EQUAL(ac2.countAtoms(), 2)
RESULT

CHECK(countBonds())
	AtomContainer ac1;
	AtomContainer ac2;
	Atom a1;
	Atom a2;
	Atom a3;
	Atom a4;
	ac1.insert(a1);
	ac1.insert(a2);
	ac2.insert(a3);
	ac2.insert(a4);
	TEST_EQUAL(ac1.countBonds(), 0);
	TEST_EQUAL(ac2.countBonds(), 0);
	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	a1.createBond(a4);
	TEST_EQUAL(ac1.countBonds(), 3);
	TEST_EQUAL(ac2.countBonds(), 3);
RESULT

CHECK(countInterBonds())
	AtomContainer ac1;
	AtomContainer ac2;
	Atom a1;
	Atom a2;
	Atom a3;
	Atom a4;
	ac1.insert(a1);
	ac1.insert(a2);
	ac2.insert(a3);
	ac2.insert(a4);
	TEST_EQUAL(ac1.countInterBonds(), 0);
	TEST_EQUAL(ac2.countInterBonds(), 0);
	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	TEST_EQUAL(ac1.countInterBonds(), 1);
	TEST_EQUAL(ac2.countInterBonds(), 1);
RESULT

CHECK(countIntraBonds())
	AtomContainer ac1;
	AtomContainer ac2;
	Atom a1;
	Atom a2;
	Atom a3;
	Atom a4;
	ac1.insert(a1);
	ac1.insert(a2);
	ac2.insert(a3);
	ac2.insert(a4);
	TEST_EQUAL(ac1.countIntraBonds(), 0);
	TEST_EQUAL(ac2.countIntraBonds(), 0);
	a1.createBond(a2);
	a2.createBond(a3);
	a3.createBond(a4);
	TEST_EQUAL(ac1.countIntraBonds(), 1);
	TEST_EQUAL(ac2.countIntraBonds(), 1);
RESULT

CHECK(prepend(Atom&))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	ac1.insert(a1);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	ac1.prepend(a2);
	TEST_EQUAL(ac1.getAtom(0), &a2)
	TEST_EQUAL(ac1.getAtom(1), &a1)
RESULT

CHECK(insert(Atom&))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	ac1.insert(a1);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	ac1.insert(a2);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	TEST_EQUAL(ac1.getAtom(1), &a2)
RESULT

CHECK(append(Atom&))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	ac1.insert(a1);
	ac1.append(a2);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	TEST_EQUAL(ac1.getAtom(1), &a2)
RESULT

CHECK(insertBefore(Atom&, Composite&))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	Atom a3;
	ac1.insert(a1);
	ac1.append(a2);
	ac1.insertBefore(a3, a2);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	TEST_EQUAL(ac1.getAtom(1), &a3)
	TEST_EQUAL(ac1.getAtom(2), &a2)
RESULT

CHECK(insertAfter(Atom&, Composite&))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	Atom a3;
	ac1.insert(a1);
	ac1.append(a2);
	ac1.insertAfter(a3, a1);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	TEST_EQUAL(ac1.getAtom(1), &a3)
	TEST_EQUAL(ac1.getAtom(2), &a2)
RESULT

CHECK(prepend(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.prepend(ac2);
	ac1.prepend(ac3);
	TEST_EQUAL(ac1.getAtomContainer(1), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(0), &ac3)
RESULT

CHECK(append(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.append(ac2);
	ac1.append(ac3);
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(1), &ac3)
RESULT

CHECK(insertBefore(AtomContainer&, Composite&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	AtomContainer ac4;
	ac1.append(ac2);
	ac1.append(ac3);
	ac1.insertBefore(ac4, ac3);
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(1), &ac4)
	TEST_EQUAL(ac1.getAtomContainer(2), &ac3)
RESULT

CHECK(insertAfter(AtomContainer&, Composite&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	AtomContainer ac4;
	ac1.append(ac2);
	ac1.append(ac3);
	ac1.insertAfter(ac4, ac2);
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(1), &ac4)
	TEST_EQUAL(ac1.getAtomContainer(2), &ac3)
RESULT

CHECK(spliceBefore(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	AtomContainer ac4;
	ac1.append(ac2);
	ac4.append(ac3);
	ac4.spliceBefore(ac1);
	TEST_EQUAL(ac1.getAtomContainer(0), 0)
	TEST_EQUAL(ac4.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac4.getAtomContainer(1), &ac3)
	ac4.spliceBefore(ac4);
	TEST_EQUAL(ac4.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac4.getAtomContainer(1), &ac3)
RESULT

CHECK(spliceAfter(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	AtomContainer ac4;
	ac1.append(ac2);
	ac4.append(ac3);
	ac4.spliceAfter(ac1);
	TEST_EQUAL(ac1.getAtomContainer(0), 0)
	TEST_EQUAL(ac4.getAtomContainer(1), &ac2)
	TEST_EQUAL(ac4.getAtomContainer(0), &ac3)
	ac4.spliceBefore(ac4);
	TEST_EQUAL(ac4.getAtomContainer(1), &ac2)
	TEST_EQUAL(ac4.getAtomContainer(0), &ac3)
RESULT

CHECK(splice(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	AtomContainer ac4;
	AtomContainer ac5;
	AtomContainer acx;
	AtomContainer acy;
	ac1.append(ac2);
	ac1.append(acx);
	ac1.append(ac3);
	acx.append(acy);
	ac4.append(ac5);
	ac1.splice(acx);
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(1), &acy)
	TEST_EQUAL(ac1.getAtomContainer(2), &ac3)
	ac1.splice(ac4);
	TEST_EQUAL(ac1.getAtomContainer(0), &ac5)
	TEST_EQUAL(ac1.getAtomContainer(1), &ac2)
RESULT

CHECK(destroyBonds())
	AtomContainer ac1;
	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a4);
	ac1.append(a1);
	ac1.append(a2);
	ac1.destroyBonds();
	TEST_EQUAL(a1.countBonds(), 0)	
	TEST_EQUAL(a2.countBonds(), 0)	
	TEST_EQUAL(a3.countBonds(), 1)	
RESULT

CHECK(isSubAtomContainerOf(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.append(ac2);
	ac2.append(ac3);
	TEST_EQUAL(ac1.isSubAtomContainerOf(ac2), false)	
	TEST_EQUAL(ac2.isSubAtomContainerOf(ac2), false)	
	TEST_EQUAL(ac2.isSubAtomContainerOf(ac1), true)	
	TEST_EQUAL(ac3.isSubAtomContainerOf(ac1), true)	
RESULT

CHECK(isSuperAtomContainerOf(AtomContainer&))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.append(ac2);
	ac2.append(ac3);
	TEST_EQUAL(ac1.isSuperAtomContainerOf(ac2), true)	
	TEST_EQUAL(ac1.isSuperAtomContainerOf(ac3), true)	
	TEST_EQUAL(ac2.isSuperAtomContainerOf(ac2), false)	
	TEST_EQUAL(ac2.isSuperAtomContainerOf(ac1), false)	
	TEST_EQUAL(ac3.isSuperAtomContainerOf(ac1), false)	
RESULT

CHECK(isValid())
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.append(ac2);
	ac2.append(ac3);
	Atom a1, a2, a3, a4;
	a1.createBond(a2);
	a1.createBond(a3);
	a1.createBond(a4);
	a3.createBond(a3);
	ac1.append(a1);
	ac2.append(a2);
	ac3.append(a3);
	TEST_EQUAL(ac1.isValid(), true)	
	TEST_EQUAL(ac2.isValid(), true)	
	TEST_EQUAL(ac3.isValid(), true)	
RESULT

CHECK(dump(ostream&, Size))
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.setName("BF1");
	ac2.setName("BF2");
	ac1.append(ac2);
	Atom a1;
	a1.setName("A1");
	ac2.append(a1);
	std::ofstream outfile(filename.c_str(), File::OUT);
	ac1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/AtomContainer_test.txt", true)
RESULT

CHECK(operator ==)
	AtomContainer c1, c2;
	TEST_EQUAL(c1 == c2, false)

	c1 = c2;
	TEST_EQUAL(c1 == c2, false)

	TEST_EQUAL(c2 == c2, true)
RESULT

CHECK(operator !=)
	AtomContainer c1, c2;
	TEST_EQUAL(c1 != c2, true)

	c1 = c2;
	TEST_EQUAL(c1 != c2, true)

	TEST_EQUAL(c2 != c2, false)
RESULT

CHECK(beginAtom()/endAtom())
	AtomContainer ac1;
	Atom a1;
	a1.setName("A1");
	Atom a2;
	a2.setName("A2");
	ac1.append(a1);
	ac1.append(a2);
	vector<Atom> vec(20);

	std::copy(ac1.beginAtom(), ac1.endAtom(), vec.begin());
	TEST_EQUAL(vec[0].getName(), "A1")
	TEST_EQUAL(vec[1].getName(), "A2")
	TEST_EQUAL(vec[3].getName(), "")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
