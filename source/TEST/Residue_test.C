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

Residue*	r;
CHECK(Residue() throw())
	r = new Residue;
	TEST_NOT_EQUAL(r, 0)
RESULT

CHECK(~Residue() throw())
	delete r;
RESULT

CHECK(Residue(const Residue& residue, bool deep = true) throw())
	Residue* r1 = new Residue;
	r1->setName("testname");
	PDBAtom a("a");
	r1->insert(a);
	Residue* r2 = new Residue(*r1, true);
	TEST_NOT_EQUAL(r2, 0)
	if (r2 != 0)
	{
		TEST_EQUAL(r2->getName(), "testname")
		TEST_EQUAL(r2->getPDBAtom(0)->getName(), "a")
		delete r2;
	}
	r2 = new Residue(*r1, false);
	TEST_NOT_EQUAL(r2, 0)
	if (r2 != 0)
	{
		TEST_EQUAL(r2->getName(), "testname")
		delete r2;
	}
	delete r1;
RESULT

CHECK(Residue(const String& name, const String& id = BALL_RESIDUE_DEFAULT_ID, char insertion_code = BALL_RESIDUE_DEFAULT_INSERTION_CODE) throw())

	Residue* r1 = new Residue("r1", "id", 'i');
	TEST_NOT_EQUAL(r1, 0)
	if (r1 != 0)
	{
		TEST_EQUAL(r1->getName(), "r1")
		TEST_EQUAL(r1->getID(), "id")
		TEST_EQUAL(r1->getInsertionCode(), 'i')
		delete r1;
	}
	Residue* r2 = new Residue("r1");
	TEST_NOT_EQUAL(r2, 0)
	if (r2 != 0)
	{
		TEST_EQUAL(r2->getName(), "r1")
		TEST_EQUAL(r2->getID(), "")
		TEST_EQUAL(r2->getInsertionCode(), ' ')
		delete r2;
	}
RESULT


CHECK(void clear() throw())
	Residue r("r1", "id", 'c');
	PDBAtom a("a");
	r.insert(a);
	Chain c;
	c.insert(r);
	r.clear();
	TEST_EQUAL(r.countPDBAtoms(), 0)
	TEST_EQUAL(r.getID(), "")
	TEST_EQUAL(r.getInsertionCode(), ' ')
	TEST_EQUAL(c.getResidue(0), &r)
RESULT

CHECK(void destroy() throw())
	Residue r("r1", "id", 'c');
	PDBAtom a("a");
	r.insert(a);
	Chain c;
	c.insert(r);
	r.destroy();
	TEST_EQUAL(r.countPDBAtoms(), 0)
	TEST_EQUAL(r.getID(), "")
	TEST_EQUAL(r.getInsertionCode(), ' ')
	TEST_EQUAL(c.getResidue(0), 0)
RESULT

CHECK(void set(const Residue& residue, bool deep = true) throw())
	Residue r1("r1");
	PDBAtom a("a");
	r1.insert(a);
	Residue r2("r2");
	r2.set(r1, false);
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.countPDBAtoms(), 0)
	r2.setName("a");
	r2.set(r1);
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.countPDBAtoms(), 1)
RESULT

CHECK(Residue& operator = (const Residue& residue) throw())
	Residue r1("r1");
	PDBAtom a("a");
	r1.insert(a);
	Residue r2("r2");
	r2 = r1;
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.countPDBAtoms(), 1)
RESULT

CHECK(void get(Residue& residue, bool deep = true) const throw())
	Residue r1("r1");
	PDBAtom a("a");
	r1.insert(a);
	Residue r2("r2");
	r1.get(r2, false);
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.countPDBAtoms(), 0)
	r2.setName("a");
	r1.get(r2);
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.countPDBAtoms(), 1)
RESULT

CHECK(void swap(Residue& residue) throw())
	Residue r1("r1");
	Residue r2("r2");
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.insert(a1);
	r2.insert(a2);
	r1.swap(r2);
	TEST_EQUAL(r1.getName(), "r2")
	TEST_EQUAL(r1.getPDBAtom(0), &a2)
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.getPDBAtom(0), &a1)
RESULT

CHECK(String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const throw())
	Residue r1("r1");
	Chain c;
	c.insert(r1);
	TEST_EQUAL(r1.getFullName(Residue::NO_VARIANT_EXTENSIONS), "r1")
	TEST_EQUAL(r1.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r1")
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r1.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r1-M")
	Residue r2("r2");
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	c.insert(r2);
	TEST_EQUAL(r1.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r1-N")
	c.remove(r2);
	c.prepend(r2);
	TEST_EQUAL(r1.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r1-C")
	r1.setProperty(Residue::PROPERTY__HAS_SSBOND);
	TEST_EQUAL(r1.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r1-CS")
	c.remove(r2);
	TEST_EQUAL(r2.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r2")	
	r2.setID("112");
	TEST_EQUAL(r2.getFullName(Residue::ADD_VARIANT_EXTENSIONS), "r2")	
RESULT

CHECK(bool hasTorsionPhi() const throw())
	Residue r;
	r.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r.hasTorsionPhi(), false)	
	Chain c;
	c.insert(r);
	TEST_EQUAL(r.hasTorsionPhi(), false)	
	Residue r2;
	c.insert(r2);
	TEST_EQUAL(r.hasTorsionPhi(), false)
	r.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r.hasTorsionPhi(), false)
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r.hasTorsionPhi(), false)
	Residue r3;
	r3.setProperty(Residue::PROPERTY__AMINO_ACID);
	c.append(r3);
	TEST_EQUAL(r.hasTorsionPhi(), false)
	Residue r4;
	r4.setProperty(Residue::PROPERTY__AMINO_ACID);
	c.prepend(r4);
	TEST_EQUAL(r.hasTorsionPhi(), true)
RESULT

CHECK(Angle getTorsionPhi() const throw())
	HINFile infile(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	System s;
	infile >> s;
	TEST_EQUAL(s.countResidues(), 3)
	ResidueIterator res_it = s.beginResidue();
	PRECISION(0.0005)
	TEST_EQUAL(res_it->getTorsionPhi(), 0)
	res_it++;
	TEST_REAL_EQUAL(res_it->getTorsionPhi().value, 3.14159)
	res_it++;
	TEST_REAL_EQUAL(res_it->getTorsionPhi().value, -3.14111)
RESULT

CHECK(bool hasTorsionPsi() const throw())
	Residue r;
	r.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r.hasTorsionPsi(), false)	
	Chain c;
	c.insert(r);
	TEST_EQUAL(r.hasTorsionPsi(), false)	
	Residue r2;
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	c.insert(r2);
	TEST_EQUAL(r.hasTorsionPsi(), true)
	Residue r3;
	r3.setProperty(Residue::PROPERTY__AMINO_ACID);
	c.prepend(r3);
	TEST_EQUAL(r.hasTorsionPsi(), true)
	Residue r4;
	r4.setProperty(Residue::PROPERTY__AMINO_ACID);
	c.append(r4);
	TEST_EQUAL(r.hasTorsionPsi(), true)
RESULT

CHECK(Angle getTorsionPsi() const throw())
	HINFile infile(BALL_TEST_DATA_PATH(AlaGlySer.hin));
	System s;
	infile >> s;
	TEST_EQUAL(s.countResidues(), 3)
	ResidueIterator res_it = s.beginResidue();
  TEST_REAL_EQUAL(res_it->getTorsionPsi().value, -3.12846)
	res_it++;
	TEST_REAL_EQUAL(res_it->getTorsionPsi().value, 3.14099)
	res_it++;
	TEST_EQUAL(res_it->getTorsionPsi(), 0)
RESULT

CHECK(Protein* getProtein() throw())
	Residue r1("r1");
	TEST_EQUAL(r1.getProtein(), 0)
	Chain c;
	c.insert(r1);
	Protein p;
	p.insert(c);
	r1.getProtein()->setName("p");
	TEST_EQUAL(p.getName(), "p")
RESULT

CHECK(const Protein* getProtein() const throw())
	Residue r1("r1");
	Chain c;
	c.insert(r1);
	Protein p;
	p.insert(c);
	TEST_EQUAL(r1.getProtein(), &p)
RESULT

CHECK(Chain* getChain() throw())
	Residue r1("r1");
	TEST_EQUAL(r1.getChain(), 0)
	Chain c;
	c.insert(r1);
	r1.getChain()->setName("c");
	TEST_EQUAL(c.getName(), "c")
RESULT

CHECK(const Chain* getChain() const throw())
	Residue r1("r1");
	Chain c;
	c.insert(r1);
	TEST_EQUAL(r1.getChain(), &c)
RESULT

CHECK(SecondaryStructure* getSecondaryStructure() throw())
	Residue r1("r1");
	TEST_EQUAL(r1.getSecondaryStructure(), 0)
	SecondaryStructure s;
	Chain c;
	c.insert(s);
	s.insert(r1);
	r1.getSecondaryStructure()->setName("c");
	TEST_EQUAL(s.getName(), "c")
RESULT

CHECK(const SecondaryStructure* getSecondaryStructure() const throw())
	Residue r1("r1");
	SecondaryStructure s;
	Chain c;
	c.insert(s);
	s.insert(r1);
	TEST_EQUAL(r1.getSecondaryStructure(), &s)
RESULT

CHECK(PDBAtom* getPDBAtom(Position position) throw())
	Residue r1("r1");
	TEST_EQUAL(r1.getPDBAtom(0), 0)
	PDBAtom a1("x");
	r1.insert(a1);
	r1.getPDBAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

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
