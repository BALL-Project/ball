// $Id: Residue_test.C,v 1.9 2000/05/22 20:26:48 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/MATHS/common.h>
///////////////////////////

START_TEST(Residue, "$Id: Residue_test.C,v 1.9 2000/05/22 20:26:48 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

#undef PRECISION
#define PRECISION 1E-5


String filename;
NEW_TMP_FILE(filename)

Residue*	r;
CHECK(Residue())
	r = new Residue;
	TEST_NOT_EQUAL(r, 0)
RESULT

CHECK(~Residue())
	delete r;
RESULT

CHECK(Residue(Residue&, bool))
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

CHECK(Residue(const String& name,
			  			const String& id = BALL_RESIDUE_DEFAULT_ID,
							char insertion_code = BALL_RESIDUE_DEFAULT_INSERTION_CODE))
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


CHECK(Residue::clear())
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

CHECK(Residue::destroy())
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

CHECK(Residue::set(const Residue& residue, bool deep = true))
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

CHECK(Residue::Residue& operator = (const Residue& residue))
	Residue r1("r1");
	PDBAtom a("a");
	r1.insert(a);
	Residue r2("r2");
	r2 = r1;
	TEST_EQUAL(r2.getName(), "r1")
	TEST_EQUAL(r2.countPDBAtoms(), 1)
RESULT

CHECK(Residue::get(Residue& residue, bool deep = true) const )
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

CHECK(Residue::swap(Residue& residue))
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

CHECK(Residue::getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const )
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
RESULT

CHECK(Residue::hasTorsionPhi() const )
	Residue r;
	TEST_EQUAL(r.hasTorsionPhi(), false)	
	Chain c;
	c.insert(r);
	TEST_EQUAL(r.hasTorsionPhi(), false)	
	Residue r2;
	c.insert(r2);
	TEST_EQUAL(r.hasTorsionPhi(), true)
	Residue r3;
	c.append(r3);
	TEST_EQUAL(r.hasTorsionPhi(), true)
	Residue r4;
	c.prepend(r4);
	TEST_EQUAL(r.hasTorsionPhi(), true)
RESULT

CHECK(Residue::getTorsionPhi() const )
	HINFile infile("data/AlaGlySer.hin");
	System s;
	infile >> s;
	TEST_EQUAL(s.countResidues(), 3)
	ResidueIterator res_it = s.beginResidue();
	TEST_EQUAL(res_it->getTorsionPhi(), 0)
	res_it++;
	TEST_REAL_EQUAL(res_it->getTorsionPhi().value, 3.14159)
	res_it++;
	TEST_REAL_EQUAL(res_it->getTorsionPhi().value, -3.14111)
RESULT

CHECK(Residue::hasTorsionPsi() const )
	Residue r;
	TEST_EQUAL(r.hasTorsionPsi(), false)	
	Chain c;
	c.insert(r);
	TEST_EQUAL(r.hasTorsionPsi(), false)	
	Residue r2;
	c.insert(r2);
	TEST_EQUAL(r.hasTorsionPsi(), true)
	Residue r3;
	c.prepend(r3);
	TEST_EQUAL(r.hasTorsionPsi(), true)
	Residue r4;
	c.append(r4);
	TEST_EQUAL(r.hasTorsionPsi(), true)
RESULT

CHECK(Residue::getTorsionPsi() const )
	HINFile infile("data/AlaGlySer.hin");
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

CHECK(Residue::getProtein())
	Residue r1("r1");
	TEST_EQUAL(r1.getProtein(), 0)
	Chain c;
	c.insert(r1);
	Protein p;
	p.insert(c);
	r1.getProtein()->setName("p");
	TEST_EQUAL(p.getName(), "p")
RESULT

CHECK(Residue::getProtein() const )
	Residue r1("r1");
	Chain c;
	c.insert(r1);
	Protein p;
	p.insert(c);
	TEST_EQUAL(r1.getProtein(), &p)
RESULT

CHECK(Residue::getChain())
	Residue r1("r1");
	TEST_EQUAL(r1.getChain(), 0)
	Chain c;
	c.insert(r1);
	r1.getChain()->setName("c");
	TEST_EQUAL(c.getName(), "c")
RESULT

CHECK(Residue::getChain() const )
	Residue r1("r1");
	Chain c;
	c.insert(r1);
	TEST_EQUAL(r1.getChain(), &c)
RESULT

CHECK(Residue::getPDBAtom(Position position))
	Residue r1("r1");
	TEST_EQUAL(r1.getPDBAtom(0), 0)
	PDBAtom a1("x");
	r1.insert(a1);
	r1.getPDBAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

CHECK(Residue::getPDBAtom(Position position) const )
	Residue r1("r1");
	TEST_EQUAL(r1.getPDBAtom(0), 0)
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
RESULT

CHECK(Residue::setID(const String& id))
	Residue r1("x");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("r1");
	TEST_EQUAL(r1.getID(), "r1")
RESULT

CHECK(Residue::getID() const )
	Residue r1("x");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("");
	TEST_EQUAL(r1.getID(), "")
	r1.setID("r1");
	TEST_EQUAL(r1.getID(), "r1")
RESULT

CHECK(Residue::setInsertionCode(char insertion_code))
	Residue r1('x');
	TEST_EQUAL(r1.getInsertionCode(), ' ')
	r1.setInsertionCode('r');
	TEST_EQUAL(r1.getInsertionCode(), 'r')
RESULT

CHECK(Residue::getInsertionCode() const )
	Residue r1('x');
	TEST_EQUAL(r1.getInsertionCode(), ' ')
	r1.setInsertionCode('r');
	TEST_EQUAL(r1.getInsertionCode(), 'r')
RESULT

CHECK(Residue::countPDBAtoms() const )
	Residue r1('x');
	PDBAtom a("a");
	TEST_EQUAL(r1.countPDBAtoms(), 0)
	r1.insert(a);
	TEST_EQUAL(r1.countPDBAtoms(), 1)
RESULT

CHECK(Residue::prepend(PDBAtom& atom))
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.prepend(a1);
	r1.prepend(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a2)
	TEST_EQUAL(r1.getPDBAtom(1), &a1)
RESULT

CHECK(Residue::append(PDBAtom& atom))
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.append(a1);
	r1.append(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a2)
RESULT

CHECK(Residue::insert(PDBAtom& atom))
	Residue r1('x');
	PDBAtom a1("a1");
	PDBAtom a2("a2");
	r1.insert(a1);
	r1.insert(a2);
	TEST_EQUAL(r1.getPDBAtom(0), &a1)
	TEST_EQUAL(r1.getPDBAtom(1), &a2)
RESULT

CHECK(Residue::insertBefore(PDBAtom& atom, Composite& before))
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

CHECK(Residue::insertAfter(PDBAtom& atom, Composite& after))
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

CHECK(Residue::remove(PDBAtom& atom))
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

CHECK(Residue::spliceBefore(Residue& residue))
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

CHECK(Residue::spliceAfter(Residue& residue))
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

CHECK(Residue::splice(Residue& residue))
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

CHECK(Residue::isAminoAcid() const )
	Residue r1('x');
	TEST_EQUAL(r1.isAminoAcid(), false)
	r1.setProperty(Residue::PROPERTY__AMINO_ACID);
	TEST_EQUAL(r1.isAminoAcid(), true)
RESULT

CHECK(Residue::isTerminal() const )
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

CHECK(Residue::isNTerminal() const )
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

CHECK(Residue::isCTerminal() const )
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

CHECK(Residue::isValid() const )
	Residue r("r1", "id", 'i');
	TEST_EQUAL(r.isValid(), true)
	r.setName("");
	TEST_EQUAL(r.isValid(), true)
RESULT

CHECK(Residue::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Residue r1("r1", "test", 'X');
	PDBAtom a1("a1");
	r1.insert(a1);
	std::ofstream outfile(filename.c_str(), ios::out);
	r1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Residue_test.txt", true)
RESULT

CHECK(Residue::read(std::istream& s))
  //BAUSTELLE
RESULT

CHECK(Residue::write(std::ostream& s) const )
  //BAUSTELLE
RESULT

CHECK(Residue::persistentWrite(PersistenceManager& pm, const char* name = 0) const )
  //BAUSTELLE
RESULT

CHECK(Residue::persistentRead(PersistenceManager& pm))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
