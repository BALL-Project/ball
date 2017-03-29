// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

#include <algorithm>
#include "ItemCollector.h"

START_TEST(AtomContainer)

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

CHECK(void setName(const String& name))
	ac = new AtomContainer;
	ac->setName("testname");
RESULT

CHECK(const String& getName() const)
	TEST_EQUAL(ac->getName(), "testname")
	delete ac;
RESULT

CHECK(AtomContainer(const AtomContainer& atom_container, bool deep = true))
	AtomContainer* ac1 = new AtomContainer;
	ac1->setName("testname");
	Atom a;
	a.setName("a");
	Atom b;
	b.setName("b");
	ac1->insert(a);
	ac1->insert(b);
	AtomIterator it(ac1->beginAtom());
	TEST_EQUAL(it.isValid(), true)
	TEST_EQUAL(&*it, &a)
	TEST_EQUAL(it == ac1->endAtom(), false)
	++it;
	TEST_EQUAL(&*it, &b)
	TEST_EQUAL(it == ac1->endAtom(), false)
	++it;
	TEST_EQUAL(it == ac1->endAtom(), true)
	
	AtomConstIterator atit(ac1->beginAtom());
	for (; atit != ac1->endAtom(); ++atit)
	{
		TEST_EQUAL(atit.isValid(), true)
		TEST_EQUAL(+atit, true)
		TEST_EQUAL(atit.isEnd(), false)
	}
	TEST_EQUAL(atit.isValid(), false)
	TEST_EQUAL(+atit, false)
	TEST_EQUAL(atit.isEnd(), true)

	TEST_EQUAL(ac1->countAtoms(), 2)
	STATUS("cloning container with one atom")
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

CHECK(AtomContainer(const String& name))
	AtomContainer* ac1 = new AtomContainer("hello");
	TEST_NOT_EQUAL(ac1, 0)
	if (ac1 != 0)
	{
		TEST_EQUAL(ac1->getName(), "hello")
		delete ac1;
	}
RESULT

CHECK(void insert(AtomContainer& atom_container))
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.insert(ac2);
RESULT

CHECK(bool remove(AtomContainer& atom_container))
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.insert(ac2);
	ac1.remove(ac2);
RESULT

CHECK(Size countAtomContainers() const)
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

CHECK(AtomContainer* getAtomContainer(Position position))
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

CHECK(const AtomContainer* getAtomContainer(Position position) const)
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

CHECK(void clear())
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

CHECK(void destroy())
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

CHECK([EXTRA] void* clone(bool deep = true))
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
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
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

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
        TEST_EQUAL(isKindOf<AtomContainer>(ptr), true)
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

CHECK(void set(const AtomContainer& atom_container, bool deep = true))
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

CHECK(AtomContainer& operator = (const AtomContainer& atom_container))
	AtomContainer ac1("name1");
	Atom a;
	ac1.insert(a);
	AtomContainer ac2;
	ac2 = ac1;
	TEST_EQUAL(ac2.getName(), "name1");
	TEST_EQUAL(ac2.countAtoms(), 1);
RESULT

CHECK(void get(AtomContainer& atom_container, bool deep = true) const)
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

CHECK(void swap(AtomContainer& atom_container))
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

CHECK(AtomContainer* getSuperAtomContainer())
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

CHECK(const AtomContainer* getSuperAtomContainer() const)
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

CHECK(void insert(Atom& atom))
	AtomContainer ac1;
	Atom a;
	ac1.insert(a);
RESULT

CHECK(bool remove(Atom& atom))
	AtomContainer ac1;
	Atom a;
	ac1.insert(a);
	ac1.remove(a);
RESULT

CHECK(Atom* getAtom(Position position))
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

CHECK(const Atom* getAtom(Position position) const)
	AtomContainer ac1;
	Atom a;
	TEST_EQUAL(ac1.getAtom(0), 0)
	ac1.insert(a);
	TEST_EQUAL(ac1.getAtom(0), &a)
	ac1.remove(a);
	TEST_EQUAL(ac1.getAtom(0), 0)
	TEST_EQUAL(ac1.getAtom(24), 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
