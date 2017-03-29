// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(Protein)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

Protein* p1;

CHECK(Protein() throw())
	p1 = new Protein;
	TEST_NOT_EQUAL(p1, 0)
RESULT											

CHECK(~Protein() throw())
	delete p1;
RESULT

CHECK(Protein(const Protein& protein, bool deep = true) throw())
	Residue r1;
	Protein p1("p1"), p2;
	p1.append(r1);
	p2 = Protein(p1);
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(Protein(const String& name, const String& id = BALL_PROTEIN_DEFAULT_ID) throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getName(), "p1")
	Protein p2("");
	TEST_EQUAL(p2.getName(), "")
RESULT

CHECK(void clear() throw())
	System sys1;
	Protein p1("p1");
	Residue r1;
	sys1.append(p1);
	p1.append(r1);
	p1.clear();
	TEST_EQUAL(p1.getName(), "")
	TEST_EQUAL(p1.countResidues(), 0)
RESULT

CHECK(void destroy() throw())
	System sys1;
	Protein p1("p1");
	Residue r1;
	sys1.append(p1);
	p1.append(r1);
	p1.destroy();
	TEST_EQUAL(p1.getName(), "")
	TEST_EQUAL(p1.countResidues(), 0)
RESULT

CHECK(void set(const Protein& protein, bool deep = true) throw())
	Protein p1("p1"), p2;
	Residue r1;
	p1.append(r1);
	p2.set(p1);
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(Protein& operator = (const Protein& protein) throw())
	Protein p1("p1"), p2;
	Residue r1;
	p1.append(r1);
	p2 = p1;
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(void get(Protein& protein, bool deep = true) const throw())
	Protein p1("p1"), p2;
	Residue r1;
	p1.append(r1);
	p1.get(p2);
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(void swap(Protein& protein) throw())
	Protein p1("p1");
	Protein p2("p2");
	Residue r1, r2, r3;
	p1.append(r1);
	p1.append(r2);
	p2.append(r3);
	p1.swap(p2);
	TEST_EQUAL(p1.getName(), "p2")
	TEST_EQUAL(p1.countResidues(), 1)
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 2)
RESULT

CHECK(Chain* getChain(Position position) throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getChain(0), 0)
	Chain c1("X");
	p1.insert(c1);
	p1.getChain(0)->setName("c1");
	TEST_EQUAL(p1.getChain(0), &c1)
RESULT

CHECK(const Chain* getChain(Position position) const throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getChain(0), 0)
	Chain c1("c1");
	p1.insert(c1);
	TEST_EQUAL(p1.getChain(0), &c1)
RESULT

CHECK(SecondaryStructure* getSecondaryStructure(Position position) throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getSecondaryStructure(0), 0)
	SecondaryStructure s1("X");
	p1.insert(s1);
	p1.getSecondaryStructure(0)->setName("s1");
	TEST_EQUAL(p1.getSecondaryStructure(0), &s1)
RESULT

CHECK(const SecondaryStructure* getSecondaryStructure(Position position) const throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getSecondaryStructure(0), 0)
	SecondaryStructure s1("s1");
	p1.insert(s1);
	TEST_EQUAL(p1.getSecondaryStructure(0), &s1)
RESULT

CHECK(Residue* getResidue(Position position) throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getResidue(0), 0)
	Residue r1("X");
	p1.insert(r1);
	p1.getResidue(0)->setName("r1");
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(const Residue* getResidue(Position position) const throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getResidue(0), 0)
	Residue r1("r1");
	p1.insert(r1);
	TEST_EQUAL(p1.getResidue(0), &r1)
RESULT

CHECK(PDBAtom* getPDBAtom(Position position) throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getPDBAtom(0), 0)
	PDBAtom a1("x");
	Residue r1;
	p1.insert(r1);
	r1.insert(a1);
	p1.getPDBAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

CHECK(const PDBAtom* getPDBAtom(Position position) const throw())
	Protein p1("p1");
	TEST_EQUAL(p1.getPDBAtom(0), 0)
	Residue r1;
	p1.insert(r1);
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(p1.getPDBAtom(0), &a1)
RESULT

CHECK(Residue* getNTerminal() throw())
	Protein p1;
	Residue r1("X");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(p1.getNTerminal(), 0)
	p1.insert(r1);
	p1.insert(r2);
	TEST_NOT_EQUAL(p1.getNTerminal(), 0)
	if (p1.getNTerminal() != 0)
	{
		p1.getNTerminal()->setName("r1"); 
	}
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(const Residue* getNTerminal() const throw())
	Protein p1;
	Residue r1("r1");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(p1.getNTerminal(), 0)
	p1.insert(r1);
	p1.insert(r2);
	TEST_NOT_EQUAL(p1.getNTerminal(), 0)
	if (p1.getNTerminal() != 0)
	{
		TEST_EQUAL(p1.getNTerminal()->getName(), "r1")
	}
RESULT

CHECK(Residue* getCTerminal() throw())
	Protein p1;
	Residue r1("1");
	Residue r2("2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(p1.getCTerminal(), 0)
	p1.insert(r1);
	p1.insert(r2);
	TEST_NOT_EQUAL(p1.getCTerminal(), 0)
	if (p1.getCTerminal() != 0)
	{
		p1.getCTerminal()->setName("r2"); 
	}
	TEST_EQUAL(r2.getName(), "r2")
RESULT

CHECK(const Residue* getCTerminal() const throw())
	Protein p1;
	Residue r1("r1");
	Residue r2("r2");
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	r2.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(p1.getCTerminal(), 0)
	p1.insert(r1);
	p1.insert(r2);
	TEST_NOT_EQUAL(p1.getCTerminal(), 0)
	if (p1.getCTerminal() != 0)
	{
		TEST_EQUAL(p1.getCTerminal()->getName(), "r2")
	}
RESULT

CHECK(Size countChains() const throw())
	Protein p1;
	Chain c1;
	TEST_EQUAL(p1.countChains(), 0)
	p1.insert(c1);
	TEST_EQUAL(p1.countChains(), 1)
RESULT

CHECK(Size countSecondaryStructures() const throw())
	Protein p1;
	SecondaryStructure s1("s1");
	TEST_EQUAL(p1.countSecondaryStructures(), 0)
	p1.insert(s1);
	TEST_EQUAL(p1.countSecondaryStructures(), 1)
RESULT

CHECK(Size countResidues() const throw())
	Protein p1;
	Residue r1("r1");
	TEST_EQUAL(p1.countResidues(), 0)
	p1.insert(r1);
	TEST_EQUAL(p1.countResidues(), 1)
RESULT

CHECK(Size countPDBAtoms() const throw())
	Protein p1;
	Residue r1("r1");
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(p1.countPDBAtoms(), 0)
	p1.insert(r1);
	TEST_EQUAL(p1.countPDBAtoms(), 1)
RESULT


CHECK([EXTRA]Protein::prepend(Residue& Residue))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	TEST_EQUAL(p1.getResidue(0), &r1)
	p1.prepend(r2);
	TEST_EQUAL(p1.getResidue(0), &r2)
	TEST_EQUAL(p1.getResidue(1), &r1)
RESULT

CHECK([EXTRA]Protein::append(Residue& Residue))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	p1.append(r2);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r2)
RESULT

CHECK([EXTRA]Protein::insert(Residue& Residue))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	p1.insert(r2);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r2)
RESULT

CHECK([EXTRA]Protein::insertBefore(Residue& Residue, Composite& before))
	Protein p1;
	Residue r1;
	Residue r2;
	Residue r3;
	p1.insert(r1);
	p1.append(r2);
	p1.insertBefore(r3, r2);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r3)
	TEST_EQUAL(p1.getResidue(2), &r2)
RESULT

CHECK([EXTRA]Protein::insertAfter(Residue& Residue, Composite& after))
	Protein p1;
	Residue r1;
	Residue r2;
	Residue r3;
	p1.insert(r1);
	p1.append(r2);
	p1.insertAfter(r3, r1);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r3)
	TEST_EQUAL(p1.getResidue(2), &r2)
RESULT

CHECK([EXTRA]Protein::remove(Residue& Residue))
	Protein p1("p1");
	Residue r1;
	Residue r2;
	p1.append(r1);
	p1.append(r2);
	p1.remove(r1);
	TEST_EQUAL(p1.countResidues(), 1)
	TEST_EQUAL(p1.getResidue(0), &r2)
RESULT

CHECK([EXTRA]Protein::spliceBefore(Protein& Protein))
	Protein p1('x');
	Protein p2('x');
	Residue r1("r1");
	Residue r2("r2");
	p1.insert(r1);
	p2.insert(r2);
	p1.spliceBefore(p2);
	TEST_EQUAL(p1.getResidue(0), &r2)
	TEST_EQUAL(p1.getResidue(1), &r1)
RESULT

CHECK([EXTRA]Protein::spliceAfter(Protein& Protein))
	Protein p1('x');
	Protein p2('x');
	Residue r1("r1");
	Residue r2("r2");
	p1.insert(r1);
	p2.insert(r2);
	p1.spliceAfter(p2);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r2)
RESULT

CHECK([EXTRA]Protein::splice(Protein& Protein))
	Protein p1('x');
	Protein p2('x');
	Residue r1("r1");
	Residue r2("r2");
	p1.insert(r1);
	p2.insert(r2);
	p1.splice(p2);
	TEST_EQUAL(p1.getResidue(0), &r2)
	TEST_EQUAL(p1.getResidue(1), &r1)
RESULT

CHECK([EXTRA]Protein::isValid() const )
	Protein p1("p1");
	Residue r1;
	p1.append(r1);
	TEST_EQUAL(p1.isValid(), true)
RESULT

CHECK([EXTRA]Protein::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Protein p1("Protein1");
	Residue r1, r2;
	r1.setName("Residue1");
	r2.setName("Residue2");
	p1.append(r1);
	p1.append(r2);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	p1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Protein_test.txt))
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Protein>(), Protein::createDefault);
pm.registerClass(getStreamName<Chain>(), Chain::createDefault);
NEW_TMP_FILE(filename)
CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	Protein* f1= new Protein("name1");
	Chain* f2 = new Chain("name2");
	f1->insert(*f2);
	Chain* f3 = new Chain("name3");
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
        TEST_EQUAL(isKindOf<Protein>(ptr), true)
		Protein*	f1 = castTo<Protein>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countChains(), 2)
		TEST_EQUAL(f1->getChain(0)->getName(), "name2")
		TEST_EQUAL(f1->getChain(1)->getName(), "name3")
		delete f1;
	} 
	else 
	{
		throw Exception::NullPointer(__FILE__, __LINE__);
	}
RESULT

CHECK(bool operator == (const Protein& protein) const throw())
	Protein b1;
	Protein b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

CHECK(bool operator != (const Protein& protein) const throw())
	Protein b1;
	Protein b2;
	TEST_EQUAL(b1 != b2, true)
	b1 = b2;
	TEST_EQUAL(b1 != b1, false)
RESULT

CHECK(const String& getID() const throw())
	Protein p;
	TEST_EQUAL(p.getID(), "")
RESULT

CHECK(void setID(const String& id) throw())
	Protein p;
	p.setID("asddd");
	TEST_EQUAL(p.getID(), "asddd")
RESULT

CHECK(BALL_CREATE_DEEP(Protein))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	p1.insert(r2);
	p1.setName("protein1");
	r1.setName("r1");
	p1.setID("p1");
	
	Protein* test = (Protein*) p1.create(false, true);
	TEST_EQUAL(test->getName(), "")
	TEST_EQUAL(test->getID(), "")
	TEST_EQUAL(test->countResidues(), 0)
	delete test;
	test = (Protein*) p1.create(true, false);
	TEST_EQUAL(test->getName(), "protein1")
	TEST_EQUAL(test->getID(), "p1")
	TEST_EQUAL(test->countResidues(), 2)
	TEST_EQUAL(test->getResidue(0)->getName(), "r1")
  delete test;
RESULT

CHECK([EXTRA] iterators)
	Protein s;
	Chain c1,c2;
	SecondaryStructure s1,s2;
	Residue r1, r2;
	s.insert(c1);
	s.insert(c2);
	c1.insert(s1);
	c2.insert(s2);
	s1.insert(r1);
	s2.insert(r2);
	PDBAtom pa1,pa2,pa3;
	r1.insert(pa1);
	r2.insert(pa2);
	r2.insert(pa3);

	TEST_EQUAL(&*s.beginResidue(), &r1)
	TEST_EQUAL(&*--s.endResidue(), &r2)
	TEST_EQUAL(&*s.rbeginResidue(), &r2)	

	TEST_EQUAL(&*s.beginFragment(), &r1)
	TEST_EQUAL(&*--s.endFragment(), &r2)
	TEST_EQUAL(&*s.rbeginFragment(), &r2)	

	TEST_EQUAL(&*s.beginSecondaryStructure(), &s1)
	TEST_EQUAL(&*--s.endSecondaryStructure(), &s2)
	TEST_EQUAL(&*s.rbeginSecondaryStructure(), &s2)	

	TEST_EQUAL(&*s.beginChain(), &c1)
	TEST_EQUAL(&*--s.endChain(), &c2)
	TEST_EQUAL(&*s.rbeginChain(), &c2)	

	TEST_EQUAL(&*s.beginAtom(), &pa1)
	TEST_EQUAL(&*++s.beginAtom(), &pa2)
	TEST_EQUAL(&*--s.endAtom(), &pa3)
	TEST_EQUAL(&*s.rbeginAtom(), &pa3)	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
