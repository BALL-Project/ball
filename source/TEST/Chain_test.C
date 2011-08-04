// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(AtomContainer)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)


Chain*	c;
CHECK(Chain() throw())
	c = new Chain;
	TEST_NOT_EQUAL(c, 0)
RESULT

CHECK(~Chain() throw())
	delete c;
RESULT

CHECK(Chain(const Chain& chain, bool deep = true) throw())
	Chain* c1 = new Chain;
	c1->setName("testname");
	Residue r;
	r.setName("a");
	c1->insert(r);
	Chain* c2 = new Chain(*c1, true);
	TEST_NOT_EQUAL(c2, 0)
	if (c2 != 0)
	{
		TEST_EQUAL(c2->getName(), "testname")
		TEST_EQUAL(c2->getResidue(0)->getName(), "a")
		delete c2;
	}
	c2 = new Chain(*c1, false);
	TEST_NOT_EQUAL(c2, 0)
	if (c2 != 0)
	{
		TEST_EQUAL(c2->getName(), "testname")
		delete c2;
	}
	delete c1;
RESULT

CHECK(Chain(const String& name) throw())
	Chain* c1 = new Chain("hello");
	TEST_NOT_EQUAL(c1, 0)
	if (c1 != 0)
	{
		TEST_EQUAL(c1->getName(), "hello")
		delete c1;
	}
RESULT

CHECK(void set(const Chain& chain, bool deep = true) throw())
	Chain* c1 = new Chain;
	c1->setName("testname");
	Residue r;
	r.setName("a");
	c1->insert(r);
	Chain* c2 = new Chain;
	c2->set(*c1);
	TEST_NOT_EQUAL(c2, 0)
	if (c2 != 0)
	{
		TEST_EQUAL(c2->getName(), "testname")
		TEST_EQUAL(c2->getResidue(0)->getName(), "a")
		delete c2;
	}
	Chain* c3 = new Chain;
	c3->set(*c1);	
	TEST_EQUAL(c3->getName(), "testname")
	delete c3;
	delete c1;
RESULT

CHECK(void get(Chain& chain, bool deep = true) const throw())
	Chain* c1 = new Chain;
	c1->setName("testname");
	Residue r;
	r.setName("a");
	c1->insert(r);
	Chain* c2 = new Chain;
	c1->get(*c2);
	TEST_NOT_EQUAL(c2, 0)
	if (c2 != 0)
	{
		TEST_EQUAL(c2->getName(), "testname")
		TEST_EQUAL(c2->getResidue(0)->getName(), "a")
	}
	Chain* c3 = new Chain;
	c2->get(*c3, false);	
	TEST_EQUAL(c3->getName(), "testname")
	TEST_EQUAL(c3->countResidues(), 0)
	delete c3;
	delete c2;
	delete c1;
RESULT

CHECK(Chain& operator = (const Chain& chain) throw())
	Chain* c1 = new Chain;
	c1->setName("testname");
	Residue r;
	r.setName("a");
	c1->insert(r);
	Chain* c2 = new Chain;
	*c2 = *c1;
	if (c2 != 0)
	{
		TEST_EQUAL(c2->getName(), "testname")
		TEST_EQUAL(c2->getResidue(0)->getName(), "a")
		delete c2;
	}
	Chain* c3 = new Chain;
	*c3 = *c1;	
	TEST_EQUAL(c3->getName(), "testname")
	delete c3;
	delete c1;
RESULT

CHECK(void swap(Chain& chain) throw())
	Chain c1;
	Chain c2;
	c1.setName("c1");
	c2.setName("c2");
	Residue r1;
	Residue r2;
	c1.insert(r1);
	c2.insert(r2);
	c1.swap(c2);
	TEST_EQUAL(c1.getName(), "c2")
	TEST_EQUAL(c2.getName(), "c1")
	TEST_EQUAL(c1.getResidue(0), &r2)
	TEST_EQUAL(c2.getResidue(0), &r1)
RESULT

CHECK(Protein* getProtein() throw())
	Protein p;
	Chain c;
	p.insert(c);
	c.getProtein()->setName("p");
	TEST_EQUAL(p.getName(), "p")
RESULT

CHECK(const Protein* getProtein() const throw())
	Protein p("p");
	Chain c;
	p.insert(c);
	TEST_EQUAL(c.getProtein()->getName(), "p")
RESULT

CHECK(SecondaryStructure* getSecondaryStructure(Position position) throw())
	Chain c;
	SecondaryStructure s;
	TEST_EQUAL(c.getSecondaryStructure(0), 0)
	c.insert(s);
	c.getSecondaryStructure(0)->setName("s");
	TEST_EQUAL(c.getSecondaryStructure(0)->getName(), "s")
RESULT

CHECK(const SecondaryStructure* getSecondaryStructure(Position position) const throw())
	Chain c;
	SecondaryStructure s("s");
	TEST_EQUAL(c.getSecondaryStructure(0), 0)
	c.insert(s);
	TEST_EQUAL(c.getSecondaryStructure(0)->getName(), "s")
RESULT

CHECK(Residue* getResidue(Position position) throw())
	Chain c;
	Residue r;
	TEST_EQUAL(c.getResidue(0), 0)
	c.insert(r);
	c.getResidue(0)->setName("r");
	TEST_EQUAL(c.getResidue(0)->getName(), "r")
RESULT

CHECK(const Residue* getResidue(Position position) const throw())
	Chain c;
	Residue r("r");
	TEST_EQUAL(c.getResidue(0), 0)
	c.insert(r);
	TEST_EQUAL(c.getResidue(0)->getName(), "r")
RESULT

CHECK(Residue* getNTerminal() throw())
	Chain c;
	Residue r1("r");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(c.getNTerminal(), 0)
	c.insert(r1);
	c.insert(r2);
	TEST_NOT_EQUAL(c.getNTerminal(), 0)
	if (c.getNTerminal() != 0)
	{
		c.getNTerminal()->setName("r1"); 
	}
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(const Residue* getNTerminal() const throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(c.getNTerminal(), 0)
	c.insert(r1);
	c.insert(r2);
	TEST_NOT_EQUAL(c.getNTerminal(), 0)
	if (c.getNTerminal() != 0)
	{
		TEST_EQUAL(c.getNTerminal()->getName(), "r1")
	}
RESULT

CHECK(Residue* getCTerminal() throw())
	Chain c;
	Residue r1("1");
	Residue r2("2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(c.getCTerminal(), 0)
	c.insert(r1);
	c.insert(r2);
	TEST_EQUAL(&*c.beginResidue(), &r1)
	TEST_EQUAL(&*--c.endResidue(), &r2)
	TEST_EQUAL(&*c.rbeginResidue(), &r2)	
	TEST_NOT_EQUAL(c.getCTerminal(), 0)
	if (c.getCTerminal() != 0)
	{
		c.getCTerminal()->setName("r2"); 
	}
	TEST_EQUAL(r2.getName(), "r2")
RESULT

CHECK(const Residue* getCTerminal() const throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(c.getCTerminal(), 0)
	c.insert(r1);
	c.insert(r2);
	TEST_NOT_EQUAL(c.getCTerminal(), 0)
	if (c.getCTerminal() != 0)
	{
		TEST_EQUAL(c.getCTerminal()->getName(), "r2")
	}
RESULT

CHECK(PDBAtom* getPDBAtom(Position position) throw())
	Chain c;
	Residue r;
	PDBAtom a;
	r.insert(a);
	TEST_EQUAL(c.getPDBAtom(0), 0)
	c.insert(r);
	c.getPDBAtom(0)->setName("a");
	TEST_EQUAL(c.getPDBAtom(0)->getName(), "a")
RESULT

CHECK(const PDBAtom* getPDBAtom(Position position) const throw())
	Chain c;
	Residue r;
	PDBAtom a;
	r.insert(a);
	TEST_EQUAL(c.getPDBAtom(0), 0)
	c.insert(r);
	c.getPDBAtom(0)->setName("a");
	TEST_EQUAL(c.getPDBAtom(0)->getName(), "a")
RESULT

CHECK(Size countSecondaryStructures() const throw())
	Chain c;
	SecondaryStructure s("s");
	TEST_EQUAL(c.countSecondaryStructures(), 0)
	c.insert(s);
	TEST_EQUAL(c.countSecondaryStructures(), 1)
RESULT

CHECK(Size countResidues() const throw())
	Chain c;
	Residue r("r");
	TEST_EQUAL(c.countResidues(), 0)
	c.insert(r);
	TEST_EQUAL(c.countResidues(), 1)
RESULT

CHECK(Size countPDBAtoms() const throw())
	Chain c;
	Residue r("r");
	PDBAtom a("a");
	r.insert(a);
	TEST_EQUAL(c.countPDBAtoms(), 0)
	c.insert(r);
	TEST_EQUAL(c.countPDBAtoms(), 1)
RESULT

CHECK(void prepend(SecondaryStructure& secondary_structure) throw())
	Chain c;
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	TEST_EQUAL(c.getSecondaryStructure(0), 0)
	c.prepend(s1);
	c.prepend(s2);
	TEST_EQUAL(c.getSecondaryStructure(0), &s2)
	TEST_EQUAL(c.getSecondaryStructure(1), &s1)
RESULT

CHECK(void append(SecondaryStructure& secondary_structure) throw())
	Chain c;
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	c.append(s1);
	c.append(s2);
	TEST_EQUAL(c.getSecondaryStructure(0), &s1)
	TEST_EQUAL(c.getSecondaryStructure(1), &s2)
RESULT

CHECK(void insert(SecondaryStructure& secondary_structure) throw())
	Chain c;
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	c.insert(s1);
	c.insert(s2);
	TEST_EQUAL(c.getSecondaryStructure(0), &s1)
	TEST_EQUAL(c.getSecondaryStructure(1), &s2)
RESULT

CHECK(void insertBefore(SecondaryStructure& secondary_structure, Composite& before) throw())
	Chain c;
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	SecondaryStructure s3("s3");
	c.insert(s1);
	c.insert(s3);
	c.insertBefore(s2, s3);
	TEST_EQUAL(c.getSecondaryStructure(0), &s1)
	TEST_EQUAL(c.getSecondaryStructure(1), &s2)
	TEST_EQUAL(c.getSecondaryStructure(2), &s3)
RESULT

CHECK(void insertAfter(SecondaryStructure& secondary_structure, Composite& after) throw())
	Chain c;
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	SecondaryStructure s3("s3");
	c.insert(s1);
	c.insert(s3);
	c.insertAfter(s2, s1);
	TEST_EQUAL(c.getSecondaryStructure(0), &s1)
	TEST_EQUAL(c.getSecondaryStructure(1), &s2)
	TEST_EQUAL(c.getSecondaryStructure(2), &s3)
RESULT

CHECK(bool remove(SecondaryStructure& secondary_structure) throw())
	Chain c;
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	SecondaryStructure s3("s3");
	c.insert(s1);
	c.insert(s2);
	c.insert(s3);
	c.remove(s2);
	TEST_EQUAL(c.getSecondaryStructure(0), &s1)
	TEST_EQUAL(c.getSecondaryStructure(1), &s3)
RESULT

CHECK(void prepend(Residue& residue) throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	TEST_EQUAL(c.getResidue(0), 0)
	c.prepend(r1);
	c.prepend(r2);
	TEST_EQUAL(c.getResidue(0), &r2)
	TEST_EQUAL(c.getResidue(1), &r1)
RESULT

CHECK(void append(Residue& residue) throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	c.append(r1);
	c.append(r2);
	TEST_EQUAL(c.getResidue(0), &r1)
	TEST_EQUAL(c.getResidue(1), &r2)
RESULT

CHECK(void insert(Residue& residue) throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	c.insert(r1);
	c.insert(r2);
	TEST_EQUAL(c.getResidue(0), &r1)
	TEST_EQUAL(c.getResidue(1), &r2)
RESULT

CHECK(void insertBefore(Residue& residue, Composite& before) throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	Residue r3("r3");
	c.insert(r1);
	c.insert(r3);
	c.insertBefore(r2, r3);
	TEST_EQUAL(c.getResidue(0), &r1)
	TEST_EQUAL(c.getResidue(1), &r2)
	TEST_EQUAL(c.getResidue(2), &r3)
RESULT

CHECK(void insertAfter(Residue& residue, Composite& after) throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	Residue r3("r3");
	c.insert(r1);
	c.insert(r3);
	c.insertAfter(r2, r1);
	TEST_EQUAL(c.getResidue(0), &r1)
	TEST_EQUAL(c.getResidue(1), &r2)
	TEST_EQUAL(c.getResidue(2), &r3)
RESULT

CHECK(bool remove(Residue& residue) throw())
	Chain c;
	Residue r1("r1");
	Residue r2("r2");
	Residue r3("r3");
	c.insert(r1);
	c.insert(r2);
	c.insert(r3);
	c.remove(r2);
	TEST_EQUAL(c.getResidue(0), &r1)
	TEST_EQUAL(c.getResidue(1), &r3)
RESULT

CHECK(void spliceBefore(Chain& chain) throw())
	Chain c1;
	Chain c2;
	Residue r1("r1");
	Residue r2("r2");
	c1.insert(r1);
	c2.insert(r2);
	c2.spliceBefore(c1);
	TEST_EQUAL(c2.getResidue(0), &r1)
	TEST_EQUAL(c2.getResidue(1), &r2)
RESULT

CHECK(void spliceAfter(Chain &chain) throw())
	Chain c1;
	Chain c2;
	Residue r1("r1");
	Residue r2("r2");
	c1.insert(r1);
	c2.insert(r2);
	c2.spliceAfter(c1);
	TEST_EQUAL(c2.getResidue(0), &r2)
	TEST_EQUAL(c2.getResidue(1), &r1)
RESULT

CHECK(void splice(Chain &chain) throw())
	Chain c1;
	Chain c2;
	Chain c3;
	Residue r1("r1");
	Residue r2("r2");
	Residue r3("r3");
	c1.insert(r1);
	c2.insert(r2);
	c3.insert(r3);
	c1.splice(c2);
	c1.splice(c3);
	TEST_EQUAL(c1.getResidue(0), &r3)
	TEST_EQUAL(c1.getResidue(1), &r2)
	TEST_EQUAL(c1.getResidue(2), &r1)
	TEST_EQUAL(c2.getResidue(0), 0)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	Chain c1("c1");
	Residue r1("r1");
	c1.insert(r1);
	std::ofstream outfile(filename.c_str(), std::ios::out);
	c1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Chain_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	Chain* f1 = new Chain("name1");
	Residue* f2 = new Residue("name2");
	Residue* f3 = new Residue("name3");
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
		TEST_EQUAL(isKindOf<Chain>(*ptr), true)
		Chain*	f1 = castTo<Chain>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countResidues(), 2)
		TEST_EQUAL(f1->getResidue(0)->getName(), "name2")
		TEST_EQUAL(f1->getResidue(1)->getName(), "name3")
		delete f1;
	} 
	else 
	{
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(bool operator == (const Chain& chain) const throw())
	Chain c1, c2;
	TEST_EQUAL(c1 == c2, false)

	c1 = c2;
	TEST_EQUAL(c1 == c2, false)

	TEST_EQUAL(c2 == c2, true)
RESULT

CHECK(bool operator != (const Chain& chain) const throw())
	Chain c1, c2;
	TEST_EQUAL(c1 != c2, true)

	c1 = c2;
	TEST_EQUAL(c1 != c2, true)

	TEST_EQUAL(c2 != c2, false)
RESULT


CHECK(BALL_CREATE_DEEP(Chain))
	Chain c1("c1");
	Residue r1("r1");
	c1.insert(r1);

	Chain* c2 = (Chain*) c1.create(false, true);
	TEST_EQUAL(c2->countResidues(), 0)
	delete c2;
	c2 = (Chain*) c1.create(true, false);
	TEST_EQUAL(c2->countResidues(), 1)
	delete c2;
RESULT

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(SecondaryStructure) (Residue)(PDBAtom))
	Chain c;
	SecondaryStructure s1,s2;
	Residue r1, r2;
	c.insert(s1);
	c.insert(s2);
	s1.insert(r1);
	s2.insert(r2);
	PDBAtom p1,p2,p3;
	r1.insert(p1);
	r2.insert(p2);
	r2.insert(p3);
	TEST_EQUAL(&*c.beginResidue(), &r1)
	TEST_EQUAL(&*--c.endResidue(), &r2)
	TEST_EQUAL(&*c.rbeginResidue(), &r2)	

	TEST_EQUAL(&*c.beginSecondaryStructure(), &s1)
	TEST_EQUAL(&*--c.endSecondaryStructure(), &s2)
	TEST_EQUAL(&*c.rbeginSecondaryStructure(), &s2)	

	TEST_EQUAL(&*c.beginPDBAtom(), &p1)
	TEST_EQUAL(&*++c.beginPDBAtom(), &p2)
	TEST_EQUAL(&*--c.endPDBAtom(), &p3)
	TEST_EQUAL(&*c.rbeginPDBAtom(), &p3)	
RESULT


// ====================================================================================
// not accessible methods, not to be tested
// ====================================================================================

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer))
	// not to be tested
RESULT

CHECK(bool remove(Atom& atom) throw())
	// not to be tested
RESULT

CHECK(Atom* getAtom(Position position) throw())
	// not to be tested
RESULT

CHECK(bool remove(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

CHECK(const Atom* getAtom(Position position) const throw())
	// not to be tested
RESULT

CHECK(const AtomContainer* getAtomContainer(Position position) const throw())
	// not to be tested
RESULT

CHECK(void append(Atom& atom) throw())
	// not to be tested
RESULT

CHECK(void append(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

CHECK(void insert(Atom& atom) throw())
	// not to be tested
RESULT

CHECK(void insert(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

CHECK(void insertAfter(Atom& atom, Composite& after) throw())
	// not to be tested
RESULT

CHECK(void insertAfter(AtomContainer& atom_container, Composite& after) throw())
	// not to be tested
RESULT

CHECK(void insertBefore(Atom& atom, Composite& before) throw())
	// not to be tested
RESULT

CHECK(void insertBefore(AtomContainer& atom_container, Composite& before) throw())
	// not to be tested
RESULT

CHECK(void prepend(Atom& atom) throw())
	// not to be tested
RESULT

CHECK(void prepend(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

CHECK(void splice(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

CHECK(void spliceAfter(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

CHECK(void spliceBefore(AtomContainer& atom_container) throw())
	// not to be tested
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
