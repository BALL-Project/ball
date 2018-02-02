// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MATHS/common.h>
///////////////////////////

START_TEST(Residue)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

PRECISION(1E-5)

String filename;
NEW_TMP_FILE(filename)

CHECK(const PDBAtom* getPDBAtom(Position position) const throw())
	Residue r1("r1");
	TEST_EQUAL(r1.getPDBAtom(0), 0)
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
RESULT

CHECK(void setID(const String& id) throw())
	Residue r1("x");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("r1");
	TEST_EQUAL(r1.getID(), "r1")
RESULT

CHECK(const String& getID() const throw())
	Residue r1("x");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("r1");
	TEST_EQUAL(r1.getID(), "r1")
RESULT

CHECK(void setInsertionCode(char insertion_code) throw())
	Residue r1('x');
	TEST_EQUAL(r1.getInsertionCode(), ' ')
	r1.setInsertionCode('r');
	TEST_EQUAL(r1.getInsertionCode(), 'r')
RESULT

CHECK(setInsertionCode invalid)
	Residue r1('x');

	TEST_EQUAL(r1.getInsertionCode(), ' ');
	TEST_EXCEPTION(Exception::InvalidArgument, r1.setInsertionCode('\0'));
	TEST_EQUAL(r1.getInsertionCode(), ' ');
RESULT

CHECK(void unsetInsertionCode())
	Residue r1('x');
	TEST_EQUAL(r1.getInsertionCode(), ' ');
	r1.setInsertionCode('A');
	TEST_EQUAL(r1.getInsertionCode(), 'A');
	r1.unsetInsertionCode();
	TEST_EQUAL(r1.getInsertionCode(), ' ');
RESULT

CHECK(char getInsertionCode() const throw())
	Residue r1('x');
	TEST_EQUAL(r1.getInsertionCode(), ' ')
	r1.setInsertionCode('r');
	TEST_EQUAL(r1.getInsertionCode(), 'r')
RESULT

CHECK(Size countPDBAtoms() const throw())
	Residue r1('x');
	PDBAtom a("a");
	TEST_EQUAL(r1.countPDBAtoms(), 0)
	r1.insert(a);
	TEST_EQUAL(r1.countPDBAtoms(), 1)
RESULT

CHECK(void prepend(PDBAtom& atom) throw())
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.prepend(a1);
	r1.prepend(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a2)
	TEST_EQUAL(r1.getPDBAtom(1), &a1)
RESULT

CHECK(void append(PDBAtom& atom) throw())
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.append(a1);
	r1.append(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a2)
RESULT

CHECK(void insert(PDBAtom& atom) throw())
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.insert(a1);
	r1.insert(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a2)
RESULT

CHECK(void insertBefore(PDBAtom& atom, Composite& before) throw())
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	PDBAtom a3("a3");
	r1.insert(a1);
	r1.insert(a2);
	r1.insertBefore(a3, a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a3)
	TEST_EQUAL(r1.getPDBAtom(2), &a2)
RESULT

CHECK(void insertAfter(PDBAtom& atom, Composite& after) throw())
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	PDBAtom a3("a3");
	r1.insert(a1);
	r1.insert(a2);
	r1.insertAfter(a3, a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a2)
	TEST_EQUAL(r1.getPDBAtom(2), &a3)
RESULT

CHECK(bool remove(PDBAtom& atom) throw())
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	PDBAtom a3("a3");
	r1.insert(a1);
	r1.insert(a2);
	r1.insertAfter(a3, a2);
	r1.remove(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a3)
RESULT

CHECK(void spliceBefore(Residue& residue) throw())
	Residue r1('x');
	Residue r2('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.insert(a1);
	r2.insert(a2);
	r1.spliceBefore(r2);
	TEST_EQUAL(r1.getPDBAtom(0), &a2)
	TEST_EQUAL(r1.getPDBAtom(1), &a1)
RESULT

CHECK(void spliceAfter(Residue& residue) throw())
	Residue r1('x');
	Residue r2('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.insert(a1);
	r2.insert(a2);
	r1.spliceAfter(r2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a2)
RESULT

CHECK(void splice(Residue& residue) throw())
	Residue r1('x');
	Residue r2('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.insert(a1);
	r2.insert(a2);
	r1.splice(r2);
	TEST_EQUAL(r1.getPDBAtom(0), &a2)
	TEST_EQUAL(r1.getPDBAtom(1), &a1)
RESULT

CHECK(bool isAminoAcid() const throw())
	Residue r1('x');
	TEST_EQUAL(r1.isAminoAcid(), false)
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r1.isAminoAcid(), true)
RESULT

CHECK(bool isTerminal() const throw())
	Residue r1('x');
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r1.isTerminal(), false)
	Chain c1;
	c1.insert(r1);
	TEST_EQUAL(r1.isTerminal(), true)
	Residue r2('x');
	Residue r3('x');
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	r3.setProperty(Residue::PROPERTY__AMINO_ACID);
	c1.append(r2);
	TEST_EQUAL(r1.isTerminal(), true)
	c1.prepend(r3);
	TEST_EQUAL(r1.isTerminal(), false)
RESULT

CHECK(bool isNTerminal() const throw())
	Residue r1('x');
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r1.isNTerminal(), false)
	Chain c1;
	c1.insert(r1);
	TEST_EQUAL(r1.isNTerminal(), true)
	Residue r2('x');
	Residue r3('x');
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	r3.setProperty(Residue::PROPERTY__AMINO_ACID);
	c1.append(r2);
	TEST_EQUAL(r1.isNTerminal(), true)
	c1.prepend(r3);
	TEST_EQUAL(r1.isNTerminal(), false)
RESULT

CHECK(bool isCTerminal() const throw())
	Residue r1('x');
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r1.isCTerminal(), false)
	Chain c1;
	c1.insert(r1);
	TEST_EQUAL(r1.isCTerminal(), true)
	Residue r2('x');
	Residue r3('x');
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	r3.setProperty(Residue::PROPERTY__AMINO_ACID);
	c1.prepend(r3);
	TEST_EQUAL(r1.isCTerminal(), true)
	c1.append(r2);
	TEST_EQUAL(r1.isCTerminal(), false)
RESULT

CHECK(bool isValid() const throw())
	Residue r("r1", "id", 'i');
	TEST_EQUAL(r.isValid(), true)
	r.setName("");
	TEST_EQUAL(r.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Residue r1("r1", "test", 'X');
	PDBAtom a1("a1");
	r1.insert(a1);
	std::ofstream outfile(filename.c_str(), std::ios::out);
	r1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Residue_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
pm.registerClass(getStreamName<Molecule>(), Molecule::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const)
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

CHECK(void persistentRead(PersistenceManager& pm))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Molecule>(ptr), true)
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

CHECK(bool operator == (const Residue& residue) const throw())
	Residue b1;
	Residue b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const Residue& residue) const throw())
	Residue b1;
	Residue b2;
	TEST_EQUAL(b1 != b2, true)
	b1 = b2;
	TEST_EQUAL(b1 != b1, false)
RESULT

CHECK(BALL_CREATE_DEEP(Residue))
	Residue r1('x');
	PDBAtom a1("a1");
	r1.insert(a1);
	Residue* test = (Residue*) r1.create(false, true);
	TEST_EQUAL(test->getName(), "")
	TEST_EQUAL(test->countPDBAtoms(), 0)
	delete test;
	test = (Residue*) r1.create(true, false);
	TEST_EQUAL(test->getName(), "x")
	TEST_EQUAL(test->countPDBAtoms(), 1)
	delete test;
RESULT

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(PDBAtom))
	Residue c;
	PDBAtom p1,p2,p3;
	c.insert(p1);
	c.insert(p2);
	c.insert(p3);

	TEST_EQUAL(&*c.beginPDBAtom(), &p1)
	TEST_EQUAL(&*++c.beginPDBAtom(), &p2)
	TEST_EQUAL(&*--c.endPDBAtom(), &p3)
	TEST_EQUAL(&*c.rbeginPDBAtom(), &p3)
RESULT

// ============================================================
// not to be tested:
// ============================================================

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer))
// not to be tested
RESULT

CHECK(Size countAtomContainers() const throw())
// not to be tested
RESULT

CHECK(bool isSuperAtomContainerOf(const AtomContainer& atom_container) const throw())
// not to be tested
RESULT

CHECK(bool remove(AtomContainer& AtomContainer) throw())
// not to be tested
RESULT

CHECK(const AtomContainer* getAtomContainer(Position position) const throw())
// not to be tested
RESULT

CHECK(void append(AtomContainer& atom_container) throw())
// not to be tested
RESULT

CHECK(void insert(AtomContainer& atom_container) throw())
// not to be tested
RESULT

CHECK(void insertAfter(AtomContainer& atom_container, Composite& composite) throw())
// not to be tested
RESULT

CHECK(void insertBefore(AtomContainer& atom_container, Composite& composite) throw())
// not to be tested
RESULT

CHECK(void prepend(AtomContainer& atom_container) throw())
// not to be tested
RESULT

CHECK(void splice(AtomContainer& AtomContainer) throw())
// not to be tested
RESULT

CHECK(void spliceAfter(AtomContainer& base_ragment) throw())
// not to be tested
RESULT

CHECK(void spliceBefore(AtomContainer& atom_container) throw())
// not to be tested
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
