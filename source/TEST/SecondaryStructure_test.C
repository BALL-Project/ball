// $Id: SecondaryStructure_test.C,v 1.2 2000/05/31 01:01:48 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/residue.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/protein.h>
///////////////////////////

START_TEST(class_name, "$Id: SecondaryStructure_test.C,v 1.2 2000/05/31 01:01:48 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

SecondaryStructure* s1;

CHECK(SecondaryStructure())
	s1 = new SecondaryStructure;
	TEST_NOT_EQUAL(s1, 0)
RESULT											

CHECK(~SecondaryStructure())
	delete s1;
RESULT

CHECK(SecondaryStructure(const SecondaryStructure& SecondaryStructure, bool deep = true))
	Residue r1;
	SecondaryStructure s1("s1"), s2;
	s1.append(r1);
	s2 = SecondaryStructure(s1);
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(SecondaryStructure(const String& name))
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getName(), "s1")
	SecondaryStructure s2("");
	TEST_EQUAL(s2.getName(), "")
RESULT

CHECK(clear())
	Protein p1;
	SecondaryStructure s1("s1");
	Residue r1;
	p1.append(s1);
	s1.append(r1);
	s1.clear();
	TEST_EQUAL(s1.getName(), "")
	TEST_EQUAL(s1.countResidues(), 0)
RESULT

CHECK(destroy())
	Protein p1;
	SecondaryStructure s1("s1");
	Residue r1;
	p1.append(s1);
	s1.append(r1);
	s1.destroy();
	TEST_EQUAL(s1.getName(), "")
	TEST_EQUAL(s1.countResidues(), 0)
RESULT

CHECK(SecondaryStructure::set(const SecondaryStructure& SecondaryStructure, bool deep = true))
	SecondaryStructure s1("s1"), s2;
	Residue r1;
	s1.append(r1);
	s2.set(s1);
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(SecondaryStructure::SecondaryStructure& operator = (const SecondaryStructure& SecondaryStructure))
	SecondaryStructure s1("s1"), s2;
	Residue r1;
	s1.append(r1);
	s2 = s1;
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(SecondaryStructure::get(SecondaryStructure& SecondaryStructure, bool deep = true) const )
	SecondaryStructure s1("s1"), s2;
	Residue r1;
	s1.append(r1);
	s1.get(s2);
	TEST_EQUAL(s2.getName(), "s1")
	TEST_EQUAL(s2.countResidues(), 1)
RESULT

CHECK(SecondaryStructure::swap(SecondaryStructure& SecondaryStructure))
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

CHECK(SecondaryStructure::getProtein())
	Protein p1;
	SecondaryStructure s1("s1");
	p1.append(s1);
	s1.getProtein()->setName("X");
	TEST_EQUAL(p1.getName(), "X")
RESULT

CHECK(SecondaryStructure::getProtein() const )
	Protein p1("X");
	SecondaryStructure s1("s1");
	p1.append(s1);
	const Protein p2(p1);
	TEST_EQUAL(s1.getProtein()->getName(), "X")
RESULT

CHECK(SecondaryStructure::getChain())
	Chain c1;
	SecondaryStructure s1("s1");
	c1.append(s1);
	s1.getChain()->setName("X");
	TEST_EQUAL(c1.getName(), "X")
RESULT

CHECK(SecondaryStructure::getChain() const )
	Chain c1("X");
	SecondaryStructure s1("s1");
	c1.append(s1);
	const Chain p2(c1);
	TEST_EQUAL(s1.getChain()->getName(), "X")
RESULT

CHECK(Residue::getResidue(Position position))
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getResidue(0), 0)
	Residue r1("X");
	s1.insert(r1);
	s1.getResidue(0)->setName("r1");
	TEST_EQUAL(r1.getName(), "r1")
RESULT

CHECK(Residue::getResidue(Position position) const )
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getResidue(0), 0)
	Residue r1("r1");
	s1.insert(r1);
	TEST_EQUAL(s1.getResidue(0), &r1)
RESULT

CHECK(SecondaryStructure::getPDBAtom(Position position))
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getPDBAtom(0), 0)
	PDBAtom a1("x");
	Residue r1;
	s1.insert(r1);
	r1.insert(a1);
	s1.getPDBAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

CHECK(SecondaryStructure::getPDBAtom(Position position) const )
	SecondaryStructure s1("s1");
	TEST_EQUAL(s1.getPDBAtom(0), 0)
	Residue r1;
	s1.insert(r1);
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(s1.getPDBAtom(0), &a1)
RESULT

CHECK(SecondaryStructure::getNTerminal())
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

CHECK(SecondaryStructure::getNTerminal() const )
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

CHECK(SecondaryStructure::getCTerminal())
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

CHECK(SecondaryStructure::getCTerminal() const )
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

CHECK(SecondaryStructure::countResidues() const )
	SecondaryStructure s1;
	Residue r1("r1");
	TEST_EQUAL(s1.countResidues(), 0)
	s1.insert(r1);
	TEST_EQUAL(s1.countResidues(), 1)
RESULT

CHECK(SecondaryStructure::countPDBAtoms() const )
	SecondaryStructure s1;
	Residue r1("r1");
	PDBAtom a1("a1");
	r1.insert(a1);
	TEST_EQUAL(s1.countPDBAtoms(), 0)
	s1.insert(r1);
	TEST_EQUAL(s1.countPDBAtoms(), 1)
RESULT


CHECK(SecondaryStructure::prepend(Residue& Residue))
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	s1.insert(r1);
	TEST_EQUAL(s1.getResidue(0), &r1)
	s1.prepend(r2);
	TEST_EQUAL(s1.getResidue(0), &r2)
	TEST_EQUAL(s1.getResidue(1), &r1)
RESULT

CHECK(SecondaryStructure::append(Residue& Residue))
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	s1.insert(r1);
	s1.append(r2);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r2)
RESULT

CHECK(SecondaryStructure::insert(Residue& Residue))
	SecondaryStructure s1;
	Residue r1;
	Residue r2;
	s1.insert(r1);
	s1.insert(r2);
	TEST_EQUAL(s1.getResidue(0), &r1)
	TEST_EQUAL(s1.getResidue(1), &r2)
RESULT

CHECK(SecondaryStructure::insertBefore(Residue& Residue, Composite& before))
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

CHECK(SecondaryStructure::insertAfter(Residue& Residue, Composite& after))
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

CHECK(SecondaryStructure::remove(Residue& Residue))
	SecondaryStructure s1("s1");
	Residue r1;
	Residue r2;
	s1.append(r1);
	s1.append(r2);
	s1.remove(r1);
	TEST_EQUAL(s1.countResidues(), 1)
	TEST_EQUAL(s1.getResidue(0), &r2)
RESULT

CHECK(SecondaryStructure::spliceBefore(SecondaryStructure& SecondaryStructure))
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

CHECK(SecondaryStructure::spliceAfter(SecondaryStructure& SecondaryStructure))
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

CHECK(SecondaryStructure::splice(SecondaryStructure& SecondaryStructure))
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

CHECK(SecondaryStructure::isValid() const )
	SecondaryStructure s1("s1");
	Residue r1;
	s1.append(r1);
	TEST_EQUAL(s1.isValid(), true)
RESULT

CHECK(SecondaryStructure::dump(std::ostream& s = std::cout, Size depth = 0) const )
	SecondaryStructure s1("SecondaryStructure1");
	Residue r1, r2;
	r1.setName("Residue1");
	r2.setName("Residue2");
	s1.append(r1);
	s1.append(r2);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	s1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/SecondaryStructure_test.txt", true)
RESULT

CHECK(read(istream&)) 
// NotImplemented
RESULT

CHECK(write(ostream&))
// NotImplemented
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<SecondaryStructure>(), SecondaryStructure::createDefault);
pm.registerClass(getStreamName<Residue>(), Residue::createDefault);
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), std::ios::out);
	SecondaryStructure* f1= new SecondaryStructure("name1");
	Residue* f2 = new Residue("name2");
	f1->insert(*f2);
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
		TEST_EQUAL(isKindOf<SecondaryStructure>(*ptr), true)
		SecondaryStructure*	f1 = castTo<SecondaryStructure>(*ptr);
		TEST_EQUAL(f1->getName(), "name1")
		TEST_EQUAL(f1->countResidues(), 1)
		TEST_EQUAL(f1->getResidue(0)->getName(), "name2")
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
