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

Residue* r;
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
