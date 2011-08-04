// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(Molecule)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String filename;
using std::ofstream;
using std::ios;

Molecule* b;

CHECK(Molecule() throw())
	b = new Molecule;
	TEST_NOT_EQUAL(b, 0)
RESULT											

CHECK(~Molecule() throw())
	delete b;
RESULT

CHECK(Molecule(const Molecule& molecule, bool deep = true) throw())
	Atom a1;
	Molecule m("a"), m2;
	m.append(a1);
	m2 = Molecule(m);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(Molecule(const String& name) throw())
	Molecule m("a");
	TEST_EQUAL(m.getName(), "a")
	Molecule m2("");
	TEST_EQUAL(m2.getName(), "")
RESULT

CHECK([EXTRA] clear())
	System s;
	Molecule m("a");
	Atom a1;
	m.append(a1);
	s.append(m);
	m.clear();
	TEST_EQUAL(m.getName(), "")
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK([EXTRA]destroy())
	System s;
	Molecule m("a");
	Atom a1;
	m.append(a1);
	s.append(m);
	m.destroy();
	TEST_EQUAL(m.getName(), "")
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK(void set(const Molecule& molecule, bool deep = true) throw())
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m2.set(m);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(Molecule& operator = (const Molecule& molecule) throw())
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m2 = m;
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(void get(Molecule& molecule, bool deep = true) const throw())
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m.get(m2);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(void swap(Molecule& molecule) throw())
	Molecule m("a");
	Molecule m2("b");
	Atom a1, a2, a3;
	m.append(a1);
	m.append(a2);
	m2.append(a3);
	m.swap(m2);
	TEST_EQUAL(m.getName(), "b")
	TEST_EQUAL(m.countAtoms(), 1)
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 2)
RESULT

CHECK(System* getSystem() throw())
	System s;
	Molecule m("a");
	s.append(m);
	m.getSystem()->setName("b");
	TEST_EQUAL(s.getName(), "b")
RESULT

CHECK(const System* getSystem() const throw())
	System s1("aya");
	Molecule m("a");
	s1.append(m);
	const System s(s1);
	TEST_EQUAL(m.getSystem()->getName(), "aya")
RESULT

CHECK(void prepend(Atom& atom) throw())
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	TEST_EQUAL(m.getAtom(0), &a1)
	m.prepend(a2);
	TEST_EQUAL(m.getAtom(0), &a2)
	TEST_EQUAL(m.getAtom(1), &a1)
RESULT

CHECK(void append(Atom& atom) throw())
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	m.append(a2);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a2)
RESULT

CHECK(void insert(Atom& atom) throw())
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	m.insert(a2);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a2)
RESULT

CHECK(void insertBefore(Atom& atom, Composite& before) throw())
	Molecule m;
	Atom a1;
	Atom a2;
	Atom a3;
	m.insert(a1);
	m.append(a2);
	m.insertBefore(a3, a2);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a3)
	TEST_EQUAL(m.getAtom(2), &a2)
RESULT

CHECK(void insertAfter(Atom& atom, Composite& after) throw())
	Molecule m;
	Atom a1;
	Atom a2;
	Atom a3;
	m.insert(a1);
	m.append(a2);
	m.insertAfter(a3, a1);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a3)
	TEST_EQUAL(m.getAtom(2), &a2)
RESULT

CHECK(bool remove(Atom& atom) throw())
	Molecule m("a");
	Atom a1;
	Atom a2;
	m.append(a1);
	m.append(a2);
	m.remove(a1);
	TEST_EQUAL(m.countAtoms(), 1)
	TEST_EQUAL(m.getAtom(0), &a2)
RESULT

CHECK(void prepend(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.prepend(bf2);
	m.prepend(bf3);
	TEST_EQUAL(m.getAtomContainer(1), &bf2)
	TEST_EQUAL(m.getAtomContainer(0), &bf3)
RESULT

CHECK(void append(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.append(bf2);
	m.append(bf3);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf3)
RESULT

CHECK(void insert(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.insert(bf2);
	m.insert(bf3);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf3)
RESULT

CHECK(void insertBefore(AtomContainer& atom_container, Composite& before) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	m.append(bf3);
	m.insertBefore(bf4, bf3);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf4)
	TEST_EQUAL(m.getAtomContainer(2), &bf3)
RESULT

CHECK(void insertAfter(AtomContainer& atom_container, Composite& after) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	m.append(bf3);
	m.insertAfter(bf4, bf2);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf4)
	TEST_EQUAL(m.getAtomContainer(2), &bf3)
RESULT

CHECK(void spliceBefore(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	bf4.append(bf3);
	bf4.spliceBefore(m);
	TEST_EQUAL(m.getAtomContainer(0), 0)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf3)
RESULT

CHECK(void spliceAfter(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	bf4.append(bf3);
	bf4.spliceAfter(m);
	TEST_EQUAL(m.getAtomContainer(0), 0)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf3)
RESULT

CHECK(void splice(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	AtomContainer bf5;
	AtomContainer bfx;
	AtomContainer bfy;
	m.append(bf2);
	m.append(bfx);
	m.append(bf3);
	bfx.append(bfy);
	bf4.append(bf5);
	m.splice(bfx);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bfy)
	TEST_EQUAL(m.getAtomContainer(2), &bf3)
	m.splice(bf4);
	TEST_EQUAL(m.getAtomContainer(0), &bf5)
	TEST_EQUAL(m.getAtomContainer(1), &bf2)
RESULT

CHECK(bool remove(AtomContainer& atom_container) throw())
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.append(bf2);
	m.append(bf3);
	m.remove(bf2);
	TEST_EQUAL(m.getAtomContainer(0), &bf3)
RESULT

CHECK([EXTRA]Molecule::isValid() const )
	Molecule m("a");
	Atom a1;
	m.append(a1);
	TEST_EQUAL(m.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Molecule m("molecule1");
	Atom a1, a2;
	a1.setName("atom1");
	a2.setName("atom2");
	m.append(a1);
	m.append(a2);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	m.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Molecule_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
pm.registerClass(getStreamName<Molecule>(), Molecule::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	Atom* f2= new Atom();
	f2->setName("name2");
	Atom* f3= new Atom();
	f3->setName("name3");
	Molecule* f1 = new Molecule("name1");
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
		TEST_EQUAL(isKindOf<Molecule>(*ptr), true)
		Molecule*	f1 = castTo<Molecule>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countAtoms(), 2)
		TEST_EQUAL(f1->getAtom(0)->getName(), "name2")
		TEST_EQUAL(f1->getAtom(1)->getName(), "name3")
		delete f1;
	} 
	else 
	{
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(bool operator == (const Molecule& molecule) const throw())
	Molecule b1;
	Molecule b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const Molecule& molecule) const throw())
 	Molecule b1;
	Molecule b2;
	TEST_EQUAL(b1 != b2, true)
	b1 = b2;
	TEST_EQUAL(b1 != b1, false)
RESULT

CHECK(BALL_CREATE_DEEP(Molecule))
	Molecule m("molecule1");
	Atom a1;
	a1.setName("atom1");
	m.insert(a1);
	Molecule* mol = (Molecule*) m.create(false, true);
	TEST_EQUAL(mol->getName(), "")
	TEST_EQUAL(mol->countAtoms(), 0)
	delete mol;
	mol = (Molecule*) m.create(true, false);
	TEST_EQUAL(mol->getName(), "molecule1")
	TEST_EQUAL(mol->countAtoms(), 1)
	delete mol;
RESULT

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment))
	Molecule m;
	Fragment f1, f2;
	m.insert(f1);
	m.insert(f2);
	TEST_EQUAL(&*m.beginFragment(), &f1)
	TEST_EQUAL(&*--m.endFragment(), &f2)
	TEST_EQUAL(&*m.rbeginFragment(), &f2)	
RESULT

// ==============================================================
// not to be tested:
// ==============================================================

CHECK(AtomContainer* getSuperAtomContainer() throw())
// not to be tested:
RESULT

CHECK(bool isSubAtomContainerOf(const AtomContainer& atom_container) const throw())
// not to be tested:
RESULT

CHECK(bool remove(Molecule& molecule) throw())
// not to be tested:
RESULT

CHECK(const AtomContainer* getSuperAtomContainer() const throw())
// not to be tested:
RESULT

CHECK(const Molecule* getMolecule() const throw())
// not to be tested:
RESULT

CHECK(void append(Molecule& molecule) throw())
// not to be tested:
RESULT

CHECK(void insert(Molecule& molecule) throw())
// not to be tested:
RESULT

CHECK(void insertAfter(Molecule& molecule, Composite& composite) throw())
// not to be tested:
RESULT

CHECK(void insertBefore(Molecule& molecule, Composite& composite) throw())
// not to be tested:
RESULT

CHECK(void prepend(Molecule& molecule) throw())
// not to be tested:
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
