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

CHECK(Size countAtoms() const)
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.insert(ac2);
	Atom a1;
	Atom a2;
	Atom a3;

	STATUS("a1: " << &a1)
	STATUS("a2: " << &a2)
	STATUS("a3: " << &a3)

	TEST_EQUAL(ac1.countAtoms(), 0)
	TEST_EQUAL(ac2.countAtoms(), 0)
	ac2.insert(a1);
	TEST_EQUAL(ac1.countAtoms(), 1)
	TEST_EQUAL(ac2.countAtoms(), 1)
	ac1.insert(a2);
 	TEST_EQUAL(ac1.countAtoms(), 2)
	TEST_EQUAL(ac2.countAtoms(), 1)
	STATUS("mutable iteration over ac1")
	AtomIterator it1(ac1.beginAtom());
	for (; +it1; ++it1)
	{
		STATUS("  - " << &*it1)
	}
	STATUS("const iteration over ac1")
	AtomConstIterator it2(ac1.beginAtom());
	for (; +it2; ++it2)
	{
		STATUS("  - " << &*it2)
	}

	STATUS("mutable iteration over ac2")
	AtomIterator it3(ac2.beginAtom());
	for (; +it3; ++it3)
	{
		STATUS("  - " << &*it3)
	}
	STATUS("const iteration over ac2")
	AtomConstIterator it4(ac1.beginAtom());
	for (; +it4; ++it4)
	{
		STATUS("  - " << &*it4)
	}

	ac2.insert(a3);
	TEST_EQUAL(ac1.countAtoms(), 3)
	TEST_EQUAL(ac2.countAtoms(), 2)
	ac1.remove(ac2);
	TEST_EQUAL(ac1.countAtoms(), 1)
	TEST_EQUAL(ac2.countAtoms(), 2)

RESULT

CHECK(Size countBonds() const)
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

CHECK(Size countInterBonds() const)
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

CHECK(Size countIntraBonds() const)
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

CHECK(void prepend(Atom& atom))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	ac1.insert(a1);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	ac1.prepend(a2);
	TEST_EQUAL(ac1.getAtom(0), &a2)
	TEST_EQUAL(ac1.getAtom(1), &a1)
RESULT

CHECK(void insert(Atom& atom))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	ac1.insert(a1);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	ac1.insert(a2);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	TEST_EQUAL(ac1.getAtom(1), &a2)
RESULT

CHECK(void append(Atom& atom))
	AtomContainer ac1;
	Atom a1;
	Atom a2;
	ac1.insert(a1);
	ac1.append(a2);
	TEST_EQUAL(ac1.getAtom(0), &a1)
	TEST_EQUAL(ac1.getAtom(1), &a2)
RESULT

CHECK(void insertBefore(Atom& atom, Composite& before))
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

CHECK(void insertAfter(Atom& atom, Composite& after))
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

CHECK(void prepend(AtomContainer& atom_container))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.prepend(ac2);
	ac1.prepend(ac3);
	TEST_EQUAL(ac1.getAtomContainer(1), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(0), &ac3)
RESULT

CHECK(void append(AtomContainer& atom_container))
	AtomContainer ac1;
	AtomContainer ac2;
	AtomContainer ac3;
	ac1.append(ac2);
	ac1.append(ac3);
	TEST_EQUAL(ac1.getAtomContainer(0), &ac2)
	TEST_EQUAL(ac1.getAtomContainer(1), &ac3)
RESULT

CHECK(void insertBefore(AtomContainer& atom_container, Composite& before))
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

CHECK(void insertAfter(AtomContainer& atom_container, Composite& after))
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

CHECK(void spliceBefore(AtomContainer& atom_container))
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

CHECK(void spliceAfter(AtomContainer& atom_container))
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

CHECK(void splice(AtomContainer& atom_container))
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

CHECK(void destroyBonds())
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

CHECK(bool isSubAtomContainerOf(const AtomContainer& atom_container) const)
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

CHECK(bool isSuperAtomContainerOf(const AtomContainer& atom_container) const)
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

CHECK(bool isValid() const)
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

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const)
	AtomContainer ac1;
	AtomContainer ac2;
	ac1.setName("BF1");
	ac2.setName("BF2");
	ac1.append(ac2);
	Atom a1;
	a1.setName("A1");
	ac2.append(a1);
	std::ofstream outfile(filename.c_str(), std::ios::out);
	ac1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(AtomContainer_test.txt))
RESULT

CHECK(bool operator == (const AtomContainer& atom_container) const)
	AtomContainer c1, c2;
	TEST_EQUAL(c1 == c2, false)

	c1 = c2;
	TEST_EQUAL(c1 == c2, false)

	TEST_EQUAL(c2 == c2, true)
RESULT

CHECK(bool operator != (const AtomContainer& atom_container) const)
	AtomContainer c1, c2;
	TEST_EQUAL(c1 != c2, true)

	c1 = c2;
	TEST_EQUAL(c1 != c2, true)

	TEST_EQUAL(c2 != c2, false)
RESULT

AtomContainer ac1;
ac1.setName("ac1");
Atom a1;
a1.setName("A1");
Atom a2;
a2.setName("A2");
ac1.append(a1);
ac1.append(a2);
Atom a3;
Bond b1("b1", a1, a2, 1, 1);
Bond b2("b2", a1, a3, 1, 1);

CHECK([EXTRA] beginAtom()/endAtom())
	vector<Atom> vec(20);
	std::copy(ac1.beginAtom(), ac1.endAtom(), vec.begin());
	TEST_EQUAL(vec[0].getName(), "A1")
	TEST_EQUAL(vec[1].getName(), "A2")
	TEST_EQUAL(vec[3].getName(), "")
RESULT

CHECK(BALL_CREATE_DEEP(AtomContainer))
	AtomContainer* a_ptr = (AtomContainer*)ac1.create(false, false);
	AtomContainer a = *a_ptr;
	delete a_ptr;
	AtomContainer empty;
	TEST_EQUAL(a.countAtoms(), 0)
	TEST_EQUAL(a.getName(), "ac1")
	a_ptr = (AtomContainer*) ac1.create();
	a = *a_ptr;
	delete a_ptr;
	TEST_EQUAL(a.countAtoms(), 2)
	TEST_EQUAL(a.getName(), "ac1")
RESULT

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS (Atom)(AtomContainer))
  // ???
RESULT

CHECK(bool applyInterBond(UnaryProcessor<Bond>& processor))
	ItemCollector<Bond> myproc;
	ac1.applyInterBond(myproc);
	TEST_EQUAL(myproc.getList().size(), 1)
	TEST_EQUAL(*myproc.getList().begin(), &b2)
RESULT

CHECK(bool applyIntraBond(UnaryProcessor<Bond>& processor))
	ItemCollector<Bond> myproc;
	ac1.applyIntraBond(myproc);
	TEST_EQUAL(myproc.getList().size(), 1)
	TEST_EQUAL(*myproc.getList().begin(), &b1)
RESULT

CHECK(const Atom* getAtom(const String& name) const)
	const AtomContainer& ac2 = ac1;
	TEST_EQUAL(ac2.getAtom("A1"), &a1)
	TEST_EQUAL(ac2.getAtom("A2"), &a2)
	TEST_EQUAL(ac2.getAtom(""), 0)
	Atom a2_2;
	a2_2.setName("A2");
	ac1.append(a2_2);
	TEST_EQUAL(ac2.getAtom("A2"), &a2)
RESULT

CHECK(Atom* getAtom(const String& name))
	ac1.getAtom("A1")->setPosition(Vector3(1,2,4));
	TEST_EQUAL(a1.getPosition(), Vector3(1,2,4))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
