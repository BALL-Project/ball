// $Id: Protein_test.C,v 1.3 2000/05/31 14:54:57 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(class_name, "$Id: Protein_test.C,v 1.3 2000/05/31 14:54:57 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

Protein* p1;

CHECK(Protein())
	p1 = new Protein;
	TEST_NOT_EQUAL(p1, 0)
RESULT											

CHECK(~Protein())
	delete p1;
RESULT

CHECK(Protein(const Protein& Protein, bool deep = true))
	Residue r1;
	Protein p1("p1"), p2;
	p1.append(r1);
	p2 = Protein(p1);
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(Protein(const String& name))
	Protein p1("p1");
	TEST_EQUAL(p1.getName(), "p1")
	Protein p2("");
	TEST_EQUAL(p2.getName(), "")
RESULT

CHECK(clear())
	System sys1;
	Protein p1("p1");
	Residue r1;
	sys1.append(p1);
	p1.append(r1);
	p1.clear();
	TEST_EQUAL(p1.getName(), "")
	TEST_EQUAL(p1.countResidues(), 0)
RESULT

CHECK(destroy())
	System sys1;
	Protein p1("p1");
	Residue r1;
	sys1.append(p1);
	p1.append(r1);
	p1.destroy();
	TEST_EQUAL(p1.getName(), "")
	TEST_EQUAL(p1.countResidues(), 0)
RESULT

CHECK(Protein::set(const Protein& Protein, bool deep = true))
	Protein p1("p1"), p2;
	Residue r1;
	p1.append(r1);
	p2.set(p1);
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(Protein::Protein& operator = (const Protein& Protein))
	Protein p1("p1"), p2;
	Residue r1;
	p1.append(r1);
	p2 = p1;
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(Protein::get(Protein& Protein, bool deep = true) const )
	Protein p1("p1"), p2;
	Residue r1;
	p1.append(r1);
	p1.get(p2);
	TEST_EQUAL(p2.getName(), "p1")
	TEST_EQUAL(p2.countResidues(), 1)
RESULT

CHECK(Protein::swap(Protein& Protein))
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

CHECK(Protein::getChain(Position position))
	Protein p1("p1");
	TEST_EQUAL(p1.getChain(0), 0)
	Chain c1("X");
	p1.insert(c1);
	p1.getChain(0)->setName("c1");
	TEST_EQUAL(p1.getChain(0), &c1)
RESULT

CHECK(Protein::getChain(Position position) const )
	Protein p1("p1");
	TEST_EQUAL(p1.getChain(0), 0)
	Chain c1("c1");
	p1.insert(c1);
	TEST_EQUAL(p1.getChain(0), &c1)
RESULT

CHECK(Protein::getSecondaryStructure(Position position))
	Protein p1("p1");
	TEST_EQUAL(p1.getSecondaryStructure(0), 0)
	SecondaryStructure s1("X");
	p1.insert(s1);
	p1.getSecondaryStructure(0)->setName("s1");
	TEST_EQUAL(p1.getSecondaryStructure(0), &s1)
RESULT

CHECK(Protein::getSecondaryStructure(Position position) const )
	Protein p1("p1");
	TEST_EQUAL(p1.getSecondaryStructure(0), 0)
	SecondaryStructure s1("s1");
	p1.insert(s1);
	TEST_EQUAL(p1.getSecondaryStructure(0), &s1)
RESULT

CHECK(Protein::getResidue(Position position))
	Protein p1("p1");
	TEST_EQUAL(p1.getResidue(0), 0)
	Residue r1("X");
	p1.insert(r1);
	p1.getResidue(0)->setName("r1");
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(Protein::getResidue(Position position) const )
	Protein p1("p1");
	TEST_EQUAL(p1.getResidue(0), 0)
	Residue r1("r1");
	p1.insert(r1);
	TEST_EQUAL(p1.getResidue(0), &r1)
RESULT

CHECK(Protein::getPDBAtom(Position position))
	Protein p1("p1");
	TEST_EQUAL(p1.getPDBAtom(0), 0)
	PDBAtom a1("x");
	Residue r1;
	p1.insert(r1);
	r1.insert(a1);
	p1.getPDBAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

CHECK(Protein::getPDBAtom(Position position) const )
	Protein p1("p1");
	TEST_EQUAL(p1.getPDBAtom(0), 0)
	Residue r1;
	p1.insert(r1);
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(p1.getPDBAtom(0), &a1)
RESULT

CHECK(Protein::getNTerminal())
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

CHECK(Protein::getNTerminal() const )
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

CHECK(Protein::getCTerminal())
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

CHECK(Protein::getCTerminal() const )
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

CHECK(Protein::countChains() const )
	Protein p1;
	Chain c1;
	TEST_EQUAL(p1.countChains(), 0)
	p1.insert(c1);
	TEST_EQUAL(p1.countChains(), 1)
RESULT

CHECK(Protein::countSecondaryStructures() const )
	Protein p1;
	SecondaryStructure s1("s1");
	TEST_EQUAL(p1.countSecondaryStructures(), 0)
	p1.insert(s1);
	TEST_EQUAL(p1.countSecondaryStructures(), 1)
RESULT

CHECK(Protein::countResidues() const )
	Protein p1;
	Residue r1("r1");
	TEST_EQUAL(p1.countResidues(), 0)
	p1.insert(r1);
	TEST_EQUAL(p1.countResidues(), 1)
RESULT

CHECK(Protein::countPDBAtoms() const )
	Protein p1;
	Residue r1("r1");
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(p1.countPDBAtoms(), 0)
	p1.insert(r1);
	TEST_EQUAL(p1.countPDBAtoms(), 1)
RESULT


CHECK(Protein::prepend(Residue& Residue))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	TEST_EQUAL(p1.getResidue(0), &r1)
	p1.prepend(r2);
	TEST_EQUAL(p1.getResidue(0), &r2)
	TEST_EQUAL(p1.getResidue(1), &r1)
RESULT

CHECK(Protein::append(Residue& Residue))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	p1.append(r2);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r2)
RESULT

CHECK(Protein::insert(Residue& Residue))
	Protein p1;
	Residue r1;
	Residue r2;
	p1.insert(r1);
	p1.insert(r2);
	TEST_EQUAL(p1.getResidue(0), &r1)
	TEST_EQUAL(p1.getResidue(1), &r2)
RESULT

CHECK(Protein::insertBefore(Residue& Residue, Composite& before))
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

CHECK(Protein::insertAfter(Residue& Residue, Composite& after))
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

CHECK(Protein::remove(Residue& Residue))
	Protein p1("p1");
	Residue r1;
	Residue r2;
	p1.append(r1);
	p1.append(r2);
	p1.remove(r1);
	TEST_EQUAL(p1.countResidues(), 1)
	TEST_EQUAL(p1.getResidue(0), &r2)
RESULT

CHECK(Protein::spliceBefore(Protein& Protein))
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

CHECK(Protein::spliceAfter(Protein& Protein))
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

CHECK(Protein::splice(Protein& Protein))
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

CHECK(Protein::isValid() const )
	Protein p1("p1");
	Residue r1;
	p1.append(r1);
	TEST_EQUAL(p1.isValid(), true)
RESULT

CHECK(Protein::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Protein p1("Protein1");
	Residue r1, r2;
	r1.setName("Residue1");
	r2.setName("Residue2");
	p1.append(r1);
	p1.append(r2);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	p1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Protein_test.txt", true)
RESULT

CHECK(read(istream&)) 
// NotImplemented
RESULT

CHECK(write(ostream&))
// NotImplemented
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Protein>(), Protein::createDefault);
pm.registerClass(getStreamName<Chain>(), Chain::createDefault);
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
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

CHECK(persistentRead(PersistenceManager&))
	std::ifstream	ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject*	ptr = pm.readObject();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Protein>(*ptr), true)
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
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
