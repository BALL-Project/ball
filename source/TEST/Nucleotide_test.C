// $Id: Nucleotide_test.C,v 1.9 2001/06/26 19:34:56 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/PTE.h>
///////////////////////////

START_TEST(Nucleotide, "$Id: Nucleotide_test.C,v 1.9 2001/06/26 19:34:56 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

String filename;
NEW_TMP_FILE(filename)

Nucleotide*	n;
CHECK(Nucleotide())
	n = new Nucleotide;
	TEST_NOT_EQUAL(n, 0)
RESULT

CHECK(~Nucleotide())
	delete n;
RESULT

CHECK(Nucleotide(Nucleotide&, bool))
	Nucleotide* n1 = new Nucleotide;
	n1->setName("testname");
	Atom a(PTE[Element::HELIUM],"a");
	n1->insert(a);
	Nucleotide* n2 = new Nucleotide(*n1, true);
	TEST_NOT_EQUAL(n2, 0)
	if (n2 != 0)
	{
		TEST_EQUAL(n2->getName(), "testname")
		TEST_EQUAL(n2->getAtom(0)->getName(), "a")
		delete n2;
	}
	n2 = new Nucleotide(*n1, false);
	TEST_NOT_EQUAL(n2, 0)
	if (n2 != 0)
	{
		TEST_EQUAL(n2->getName(), "testname")
		delete n2;
	}
	delete n1;
RESULT

CHECK(Nucleotide(const String& name,
			  			const String& id = BALL_Nucleotide_DEFAULT_ID,
							char insertion_code = BALL_Nucleotide_DEFAULT_INSERTION_CODE))
	Nucleotide* n1 = new Nucleotide("n1", "id", 'i');
	TEST_NOT_EQUAL(n1, 0)
	if (n1 != 0)
	{
		TEST_EQUAL(n1->getName(), "n1")
		TEST_EQUAL(n1->getID(), "id")
		TEST_EQUAL(n1->getInsertionCode(), 'i')
		delete n1;
	}
	Nucleotide* n2 = new Nucleotide("n1");
	TEST_NOT_EQUAL(n2, 0)
	if (n2 != 0)
	{
		TEST_EQUAL(n2->getName(), "n1")
		TEST_EQUAL(n2->getID(), "")
		TEST_EQUAL(n2->getInsertionCode(), ' ')
		delete n2;
	}
RESULT

CHECK(Nucleotide::clear())
	Nucleotide n("n1", "id", 'c');
	Atom a(PTE[Element::HELIUM],"a");
	n.insert(a);
	NucleicAcid c;
	c.insert(n);
	n.clear();
	TEST_EQUAL(n.countAtoms(), 0)
	TEST_EQUAL(n.getID(), "")
	TEST_EQUAL(n.getInsertionCode(), ' ')
	TEST_EQUAL(c.getNucleotide(0), &n)
RESULT

CHECK(Nucleotide::destroy())
	Nucleotide n("n1", "id", 'c');
	Atom a(PTE[Element::HELIUM],"a");
	n.insert(a);
	NucleicAcid c;
	c.insert(n);
	n.destroy();
	TEST_EQUAL(n.countAtoms(), 0)
	TEST_EQUAL(n.getID(), "")
	TEST_EQUAL(n.getInsertionCode(), ' ')
	TEST_EQUAL(c.getNucleotide(0), 0)
RESULT

CHECK(Nucleotide::set(const Nucleotide& Nucleotide, bool deep = true))
	Nucleotide n1("n1");
	Atom a(PTE[Element::HELIUM],"a");
	n1.insert(a);
	Nucleotide n2("n2");
	n2.set(n1, false);
	TEST_EQUAL(n2.getName(), "n1")
	TEST_EQUAL(n2.countAtoms(), 0)
	n2.setName("a");
	n2.set(n1);
	TEST_EQUAL(n2.getName(), "n1")
	TEST_EQUAL(n2.countAtoms(), 1)
RESULT

CHECK(Nucleotide::Nucleotide& operator = (const Nucleotide& Nucleotide))
	Nucleotide n1("n1");
	Atom a(PTE[Element::HELIUM],"a");
	n1.insert(a);
	Nucleotide n2("n2");
	n2 = n1;
	TEST_EQUAL(n2.getName(), "n1")
	TEST_EQUAL(n2.countAtoms(), 1)
RESULT

CHECK(Nucleotide::get(Nucleotide& Nucleotide, bool deep = true) const )
	Nucleotide n1("n1");
	Atom a(PTE[Element::HELIUM],"a");
	n1.insert(a);
	Nucleotide n2("n2");
	n1.get(n2, false);
	TEST_EQUAL(n2.getName(), "n1")
	TEST_EQUAL(n2.countAtoms(), 0)
	n2.setName("a");
	n1.get(n2);
	TEST_EQUAL(n2.getName(), "n1")
	TEST_EQUAL(n2.countAtoms(), 1)
RESULT

CHECK(Nucleotide::swap(Nucleotide& Nucleotide))
	Nucleotide n1("n1");
	Nucleotide n2("n2");
	Atom a1(PTE[Element::HELIUM],"a");
	Atom a2(PTE[Element::HELIUM],"a");
	n1.insert(a1);
	n2.insert(a2);
	n1.swap(n2);
	TEST_EQUAL(n1.getName(), "n2")
	TEST_EQUAL(n1.getAtom(0), &a2)
	TEST_EQUAL(n2.getName(), "n1")
	TEST_EQUAL(n2.getAtom(0), &a1)
RESULT

CHECK(Nucleotide::getNucleicAcid())
	Nucleotide n1("n1");
	TEST_EQUAL(n1.getNucleicAcid(), 0)
	NucleicAcid c;
	c.insert(n1);
	n1.getNucleicAcid()->setName("c");
	TEST_EQUAL(c.getName(), "c")
RESULT

CHECK(Nucleotide::getNucleicAcid() const )
	Nucleotide n1("n1");
	NucleicAcid c;
	c.insert(n1);
	TEST_EQUAL(n1.getNucleicAcid(), &c)
RESULT

CHECK(Nucleotide::getAtom(Position position))
	Nucleotide n1("n1");
	TEST_EQUAL(n1.getAtom(0), 0)
	Atom a1(PTE[Element::HELIUM],"a");
	n1.insert(a1);
	n1.getAtom(0)->setName("a1");
	TEST_EQUAL(a1.getName(), "a1")
RESULT

CHECK(Nucleotide::getAtom(Position position) const )
	Nucleotide n1("n1");
	TEST_EQUAL(n1.getAtom(0), 0)
	Atom a1(PTE[Element::HELIUM],"a1");
	n1.insert(a1);
	TEST_EQUAL(n1.getAtom(0), &a1)
RESULT

CHECK(Nucleotide::setID(const String& id))
	Nucleotide n1("x");
	TEST_EQUAL(n1.getID(), "")
	n1.setID("");
	TEST_EQUAL(n1.getID(), "")
	n1.setID("n1");
	TEST_EQUAL(n1.getID(), "n1")
RESULT

CHECK(Nucleotide::getID() const )
	Nucleotide n1("x");
	TEST_EQUAL(n1.getID(), "")
	n1.setID("");
	TEST_EQUAL(n1.getID(), "")
	n1.setID("n1");
	TEST_EQUAL(n1.getID(), "n1")
RESULT

CHECK(Nucleotide::setInsertionCode(char insertion_code))
	Nucleotide n1('x');
	TEST_EQUAL(n1.getInsertionCode(), ' ')
	n1.setInsertionCode('r');
	TEST_EQUAL(n1.getInsertionCode(), 'r')
RESULT

CHECK(Nucleotide::getInsertionCode() const )
	Nucleotide n1('x');
	TEST_EQUAL(n1.getInsertionCode(), ' ')
	n1.setInsertionCode('r');
	TEST_EQUAL(n1.getInsertionCode(), 'r')
RESULT

CHECK(Nucleotide::countAtoms() const )
	Nucleotide n1('x');
	Atom a(PTE[Element::HELIUM],"a");
	TEST_EQUAL(n1.countAtoms(), 0)
	n1.insert(a);
	TEST_EQUAL(n1.countAtoms(), 1)
RESULT

CHECK(Nucleotide::prepend(Atom& atom))
	Nucleotide n1('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	n1.prepend(a1);
	n1.prepend(a2);
	TEST_EQUAL(n1.getAtom(0), &a2)
	TEST_EQUAL(n1.getAtom(1), &a1)
RESULT

CHECK(Nucleotide::append(Atom& atom))
	Nucleotide n1('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	n1.append(a1);
	n1.append(a2);
	TEST_EQUAL(n1.getAtom(0), &a1)
	TEST_EQUAL(n1.getAtom(1), &a2)
RESULT

CHECK(Nucleotide::insert(Atom& atom))
	Nucleotide n1('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	n1.insert(a1);
	n1.insert(a2);
	TEST_EQUAL(n1.getAtom(0), &a1)
	TEST_EQUAL(n1.getAtom(1), &a2)
RESULT

CHECK(Nucleotide::insertBefore(Atom& atom, Composite& before))
	Nucleotide n1('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	Atom a3(PTE[Element::HELIUM],"a3");
	n1.insert(a1);
	n1.insert(a2);
	n1.insertBefore(a3, a2);
	TEST_EQUAL(n1.getAtom(0), &a1)
	TEST_EQUAL(n1.getAtom(1), &a3)
	TEST_EQUAL(n1.getAtom(2), &a2)
RESULT

CHECK(Nucleotide::insertAfter(Atom& atom, Composite& after))
	Nucleotide n1('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	Atom a3(PTE[Element::HELIUM],"a3");
	n1.insert(a1);
	n1.insert(a2);
	n1.insertAfter(a3, a2);
	TEST_EQUAL(n1.getAtom(0), &a1)
	TEST_EQUAL(n1.getAtom(1), &a2)
	TEST_EQUAL(n1.getAtom(2), &a3)
RESULT

CHECK(Nucleotide::remove(Atom& atom))
	Nucleotide n1('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	Atom a3(PTE[Element::HELIUM],"a3");
	n1.insert(a1);
	n1.insert(a2);
	n1.insertAfter(a3, a2);
	n1.remove(a2);
	TEST_EQUAL(n1.getAtom(0), &a1)
	TEST_EQUAL(n1.getAtom(1), &a3)
RESULT

CHECK(Nucleotide::spliceBefore(Nucleotide& Nucleotide))
	Nucleotide n1('x');
	Nucleotide n2('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	n1.insert(a1);
	n2.insert(a2);
	n1.spliceBefore(n2);
	TEST_EQUAL(n1.getAtom(0), &a2)
	TEST_EQUAL(n1.getAtom(1), &a1)
RESULT

CHECK(Nucleotide::spliceAfter(Nucleotide& Nucleotide))
	Nucleotide n1('x');
	Nucleotide n2('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	n1.insert(a1);
	n2.insert(a2);
	n1.spliceAfter(n2);
	TEST_EQUAL(n1.getAtom(0), &a1)
	TEST_EQUAL(n1.getAtom(1), &a2)
RESULT

CHECK(Nucleotide::splice(Nucleotide& Nucleotide))
	Nucleotide n1('x');
	Nucleotide n2('x');
	Atom a1(PTE[Element::HELIUM],"a1");
	Atom a2(PTE[Element::HELIUM],"a2");
	n1.insert(a1);
	n2.insert(a2);
	n1.splice(n2);
	TEST_EQUAL(n1.getAtom(0), &a2)
	TEST_EQUAL(n1.getAtom(1), &a1)
RESULT

CHECK(Nucleotide::isTerminal() const )
	Nucleotide n1('x');
	TEST_EQUAL(n1.isTerminal(), false)
	NucleicAcid c1;
	c1.insert(n1);
	TEST_EQUAL(n1.isTerminal(), true)
	Nucleotide n2('x');
	Nucleotide n3('x');
	c1.append(n2);
	TEST_EQUAL(n1.isTerminal(), true)
	c1.prepend(n3);
	TEST_EQUAL(n1.isTerminal(), false)
RESULT

CHECK(Nucleotide::is3Prime() const )
	Nucleotide n1('x');
	TEST_EQUAL(n1.is3Prime(), false)
	NucleicAcid c1;
	c1.insert(n1);
	TEST_EQUAL(n1.is3Prime(), true)
	Nucleotide n2('x');
	Nucleotide n3('x');
	c1.append(n2);
	TEST_EQUAL(n1.is3Prime(), true)
	c1.prepend(n3);
	TEST_EQUAL(n1.is3Prime(), false)
RESULT

CHECK(Nucleotide::is5Prime() const )
	Nucleotide n1('x');
	TEST_EQUAL(n1.is5Prime(), false)
	NucleicAcid c1;
	c1.insert(n1);
	TEST_EQUAL(n1.is5Prime(), true)
	Nucleotide n2('x');
	Nucleotide n3('x');
	c1.prepend(n3);
	TEST_EQUAL(n1.is5Prime(), true)
	c1.append(n2);
	TEST_EQUAL(n1.is5Prime(), false)
RESULT

CHECK(Nucleotide::isValid() const )
	Nucleotide n("n1", "id", 'i');
	TEST_EQUAL(n.isValid(), true)
	n.setName("");
	TEST_EQUAL(n.isValid(), true)
RESULT

CHECK(Nucleotide::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Nucleotide n1("n1", "test", 'X');
	Atom a1(PTE[Element::HELIUM],"a1");
	n1.insert(a1);
	std::ofstream outfile(filename.c_str(), File::OUT);
	n1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Nucleotide_test.txt", true)
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Nucleotide>(), Nucleotide::createDefault);
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), File::OUT);
	Nucleotide* f1= new Nucleotide("name1");
	Atom* f2 = new Atom();
	f2->setName("name2");
	f1->insert(*f2);
	Atom* f3 = new Atom();
	f3->setName("name3");
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
		TEST_EQUAL(isKindOf<Nucleotide>(*ptr), true)
		Nucleotide*	f1 = castTo<Nucleotide>(*ptr);
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

CHECK(operator ==)
	Nucleotide b1;
	Nucleotide b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
