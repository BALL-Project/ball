// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/protein.h>
///////////////////////////

START_TEST(SecondaryStructure)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

SecondaryStructure* s1;

CHECK(SecondaryStructure() throw())
	s1 = new SecondaryStructure;
	TEST_NOT_EQUAL(s1, 0)
RESULT

CHECK(~SecondaryStructure() throw())
	delete s1;
RESULT

CHECK(SecondaryStructure(const SecondaryStructure& secondary_structure, bool deep = true) throw())
	Residue r1;
	SecondaryStructure s1("s1"), s2;
	s1.append(r1);
	s2 = SecondaryStructure(s1);
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(SecondaryStructure(const String& name) throw())
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getName(), "s1")
	SecondaryStructure s2("");
	TEST_EQUAL(s2.getName(), "")
RESULT

CHECK(void clear() throw())
	Protein p1;
	SecondaryStructure s1("s1");
	Residue r1;
	p1.append(s1);
	s1.append(r1);
	s1.clear();
	TEST_EQUAL(s1.getName(), "")
	TEST_EQUAL(s1.countResidues(), 0)
RESULT

CHECK(void destroy() throw())
	Protein p1;
	SecondaryStructure s1("s1");
	Residue r1;
	p1.append(s1);
	s1.append(r1);
	s1.destroy();
	TEST_EQUAL(s1.getName(), "")
	TEST_EQUAL(s1.countResidues(), 0)
RESULT

CHECK(void set(const SecondaryStructure& secondary_structure, bool deep = true) throw())
	SecondaryStructure s1("s1"), s2;
	Residue r1;
	s1.append(r1);
	s2.set(s1);
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(SecondaryStructure& operator = (const SecondaryStructure& secondary_structure) throw())
	SecondaryStructure s1("s1"), s2;
	Residue r1;
	s1.append(r1);
	s2 = s1;
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(void get(SecondaryStructure& secondary_structure, bool deep = true) const throw())
	SecondaryStructure s1("s1"), s2;
	Residue r1;
	s1.append(r1);
	s1.get(s2);
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(void swap(SecondaryStructure& secondary_structure) throw())
	SecondaryStructure s1("s1");
	SecondaryStructure s2("s2");
	Residue r1, r2, r3;
	s1.append(r1);
	s1.append(r2);
	s2.append(r3);
	s1.swap(s2);
	TEST_EQUAL(s1.getName(), "s2")
	TEST_EQUAL(s1.countResidues(), 1)
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 2)
RESULT

CHECK(Protein* getProtein() throw())
	Protein p1;
	SecondaryStructure s1("s1");
	p1.append(s1);
	s1.getProtein()->setName("X");
	TEST_EQUAL(p1.getName(), "X")
RESULT

CHECK(const Protein* getProtein() const throw())
	Protein p1("X");
	SecondaryStructure s1("s1");
	p1.append(s1);
	const Protein p2(p1);
	TEST_EQUAL(s1.getProtein()->getName(), "X")
RESULT

CHECK(Chain* getChain() throw())
	Chain c1;
	SecondaryStructure s1("s1");
	c1.append(s1);
	s1.getChain()->setName("X");
	TEST_EQUAL(c1.getName(), "X")
RESULT

CHECK(const Chain* getChain() const throw())
	Chain c1("X");
	SecondaryStructure s1("s1");
	c1.append(s1);
	const Chain p2(c1);
	TEST_EQUAL(s1.getChain()->getName(), "X")
RESULT

CHECK(Residue* getResidue(Position position) throw())
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getResidue(0), 0)
	Residue r1("X");
	s1.insert(r1);
	s1.getResidue(0)->setName("r1");
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(const Residue* getResidue(Position position) const throw())
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getResidue(0), 0)
	Residue r1("r1");
	s1.insert(r1);
	TEST_EQUAL(s1.getResidue(0), &r1)
RESULT

CHECK(PDBAtom* getPDBAtom(Position position) throw())
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getPDBAtom(0), 0)
	PDBAtom a1("x");
	Residue r1;
	s1.insert(r1);
	r1.insert(a1);
	s1.getPDBAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

CHECK(const PDBAtom* getPDBAtom(Position position) const throw())
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getPDBAtom(0), 0)
	Residue r1;
	s1.insert(r1);
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(s1.getPDBAtom(0), &a1)
RESULT

CHECK(Residue* getNTerminal() throw())
	SecondaryStructure s1;
	Residue r1("X");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(s1.getNTerminal(), 0)
	s1.insert(r1);
	s1.insert(r2);
	TEST_NOT_EQUAL(s1.getNTerminal(), 0)
	if (s1.getNTerminal() != 0)
	{
		s1.getNTerminal()->setName("r1"); 
	}
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(const Residue* getNTerminal() const throw())
	SecondaryStructure s1;
	Residue r1("r1");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(s1.getNTerminal(), 0)
	s1.insert(r1);
	s1.insert(r2);
	TEST_NOT_EQUAL(s1.getNTerminal(), 0)
	if (s1.getNTerminal() != 0)
	{
		TEST_EQUAL(s1.getNTerminal()->getName(), "r1")
	}
RESULT

CHECK(Residue* getCTerminal() throw())
	SecondaryStructure s1;
	Residue r1("1");
	Residue r2("2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(s1.getCTerminal(), 0)
	s1.insert(r1);
	s1.insert(r2);
	TEST_NOT_EQUAL(s1.getCTerminal(), 0)
	if (s1.getCTerminal() != 0)
	{
		s1.getCTerminal()->setName("r2"); 
	}
	TEST_EQUAL(r2.getName(), "r2")
RESULT

CHECK(const Residue* getCTerminal() const throw())
	SecondaryStructure s1;
	Residue r1("r1");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(s1.getCTerminal(), 0)
	s1.insert(r1);
	s1.insert(r2);
	TEST_NOT_EQUAL(s1.getCTerminal(), 0)
	if (s1.getCTerminal() != 0)
	{
		TEST_EQUAL(s1.getCTerminal()->getName(), "r2")
	}
RESULT

CHECK(Size countResidues() const throw())
	SecondaryStructure s1;
	Residue r1("r1");
	TEST_EQUAL(s1.countResidues(), 0)
	s1.insert(r1);
	TEST_EQUAL(s1.countResidues(), 1)
RESULT

CHECK(Size countPDBAtoms() const throw())
	SecondaryStructure s1;
	Residue r1("r1");
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(s1.countPDBAtoms(), 0)
	s1.insert(r1);
	TEST_EQUAL(s1.countPDBAtoms(), 1)
RESULT


CHECK(void prepend(Residue& residue) throw())
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	s1.insert(r1);
	TEST_EQUAL(s1.getResidue(0), &r1)
	s1.prepend(r2);
	TEST_EQUAL(s1.getResidue(0), &r2)
	TEST_EQUAL(s1.getResidue(1), &r1)
RESULT

CHECK(void append(Residue& residue) throw())
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	s1.insert(r1);
	s1.append(r2);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r2)
RESULT

CHECK(void insert(Residue& residue) throw())
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	s1.insert(r1);
	s1.insert(r2);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r2)
RESULT

CHECK(void insertBefore(Residue& residue, Composite& before) throw())
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	Residue r3;
	s1.insert(r1);
	s1.append(r2);
	s1.insertBefore(r3, r2);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r3)
	TEST_EQUAL(s1.getResidue(2), &r2)
RESULT

CHECK(void insertAfter(Residue& residue, Composite& after) throw())
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	Residue r3;
	s1.insert(r1);
	s1.append(r2);
	s1.insertAfter(r3, r1);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r3)
	TEST_EQUAL(s1.getResidue(2), &r2)
RESULT

CHECK(bool remove(Residue& residue) throw())
	SecondaryStructure s1("s1");
	Residue r1;
	Residue r2;
	s1.append(r1);
	s1.append(r2);
	s1.remove(r1);
	TEST_EQUAL(s1.countResidues(), 1)
	TEST_EQUAL(s1.getResidue(0), &r2)
RESULT

CHECK(void spliceBefore(SecondaryStructure& secondary_structure) throw())
	SecondaryStructure s1('x');
	SecondaryStructure s2('x');
	Residue r1("r1");
	Residue r2("r2");
	s1.insert(r1);
	s2.insert(r2);
	s1.spliceBefore(s2);
	TEST_EQUAL(s1.getResidue(0), &r2)
	TEST_EQUAL(s1.getResidue(1), &r1)
RESULT

CHECK(void spliceAfter(SecondaryStructure& secondary_structure) throw())
	SecondaryStructure s1('x');
	SecondaryStructure s2('x');
	Residue r1("r1");
	Residue r2("r2");
	s1.insert(r1);
	s2.insert(r2);
	s1.spliceAfter(s2);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r2)
RESULT

CHECK(void splice(SecondaryStructure& secondary_structure) throw())
	SecondaryStructure s1('x');
	SecondaryStructure s2('x');
	Residue r1("r1");
	Residue r2("r2");
	s1.insert(r1);
	s2.insert(r2);
	s1.splice(s2);
	TEST_EQUAL(s1.getResidue(0), &r2)
	TEST_EQUAL(s1.getResidue(1), &r1)
RESULT

CHECK(bool isValid() const throw())
	SecondaryStructure s1("s1");
	Residue r1;
	s1.append(r1);
	TEST_EQUAL(s1.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	SecondaryStructure s1("SecondaryStructure1");
	Residue r1, r2;
	r1.setName("Residue1");
	r2.setName("Residue2");
	s1.append(r1);
	s1.append(r2);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	s1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(SecondaryStructure_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<SecondaryStructure>(), SecondaryStructure::createDefault);
pm.registerClass(getStreamName<Residue>(), Residue::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	SecondaryStructure* f1= new SecondaryStructure("name1");
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
		TEST_EQUAL(isKindOf<SecondaryStructure>(*ptr), true)
		SecondaryStructure*	f1 = castTo<SecondaryStructure>(*ptr);
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

CHECK(bool operator == (const SecondaryStructure& secondary_structure) const throw())
	SecondaryStructure b1;
	SecondaryStructure b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const SecondaryStructure& secondary_structure) const throw())
	SecondaryStructure b1;
	SecondaryStructure b2;
	TEST_EQUAL(b1 != b2, true) 
	b1 = b2;
	TEST_EQUAL(b1 != b1, false)
RESULT

CHECK(BALL_CREATE_DEEP(SecondaryStructure))
	SecondaryStructure s1("s1");
	Residue r1('x');
	s1.insert(r1);
	SecondaryStructure* test = (SecondaryStructure*) s1.create(false, true);
	TEST_EQUAL(test->getName(), "")
	TEST_EQUAL(test->countResidues(), 0)
	delete test;
	test = (SecondaryStructure*) s1.create(true, false);
	TEST_EQUAL(test->getName(), "s1")
	TEST_EQUAL(test->countResidues(), 1)
	delete test;
RESULT

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS (Residue)(PDBAtom))
	SecondaryStructure c;
	Residue r1, r2;
	c.insert(r1);
	c.insert(r2);
	PDBAtom p1,p2,p3;
	r1.insert(p1);
	r2.insert(p2);
	r2.insert(p3);
	TEST_EQUAL(&*c.beginResidue(), &r1)
	TEST_EQUAL(&*--c.endResidue(), &r2)
	TEST_EQUAL(&*c.rbeginResidue(), &r2)	

	TEST_EQUAL(&*c.beginPDBAtom(), &p1)
	TEST_EQUAL(&*++c.beginPDBAtom(), &p2)
	TEST_EQUAL(&*--c.endPDBAtom(), &p3)
	TEST_EQUAL(&*c.rbeginPDBAtom(), &p3)	
RESULT

// =======================================================================
// not to be tested
// =======================================================================

CHECK(BALL_KERNEL_DEFINE_ITERATOR_CREATORS(AtomContainer))
  // not to be tested
RESULT

CHECK(Atom* getAtom(Position position) throw())
  // not to be tested
RESULT

CHECK(bool remove(Atom &atom) throw())
  // not to be tested
RESULT

CHECK(bool remove(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

CHECK(const Atom* getAtom(Position position) const throw())
  // not to be tested
RESULT

CHECK(const Fragment* getFragment(Position position) const throw())
  // not to be tested
RESULT

CHECK(void append(Atom &atom) throw())
  // not to be tested
RESULT

CHECK(void append(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

CHECK(void insert(Atom &atom) throw())
  // not to be tested
RESULT

CHECK(void insert(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

CHECK(void insertAfter(Atom &atom, Composite &after) throw())
  // not to be tested
RESULT

CHECK(void insertAfter(AtomContainer &atom_container, Composite &after) throw())
  // not to be tested
RESULT

CHECK(void insertBefore(Atom &atom, Composite &before) throw())
  // not to be tested
RESULT

CHECK(void insertBefore(AtomContainer &atom_container, Composite &before) throw())
  // not to be tested
RESULT

CHECK(void prepend(Atom &atom) throw())
  // not to be tested
RESULT

CHECK(void prepend(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

CHECK(void splice(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

CHECK(void spliceAfter(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

CHECK(void spliceBefore(AtomContainer &atom_container) throw())
  // not to be tested
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
