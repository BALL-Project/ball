// $Id: Molecule_test.C,v 1.10 2001/06/27 10:46:00 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(class_name, "$Id: Molecule_test.C,v 1.10 2001/06/27 10:46:00 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

Molecule* b;

CHECK(Molecule())
	b = new Molecule;
	TEST_NOT_EQUAL(b, 0)
RESULT											

CHECK(~Molecule())
	delete b;
RESULT

CHECK(Molecule(const Molecule& molecule, bool deep = true))
	Atom a1;
	Molecule m("a"), m2;
	m.append(a1);
	m2 = Molecule(m);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(Molecule(const String& name))
	Molecule m("a");
	TEST_EQUAL(m.getName(), "a")
	Molecule m2("");
	TEST_EQUAL(m2.getName(), "")
RESULT

CHECK(clear())
	System s;
	Molecule m("a");
	Atom a1;
	m.append(a1);
	s.append(m);
	m.clear();
	TEST_EQUAL(m.getName(), "")
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK(destroy())
	System s;
	Molecule m("a");
	Atom a1;
	m.append(a1);
	s.append(m);
	m.destroy();
	TEST_EQUAL(m.getName(), "")
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK(Molecule::set(const Molecule& molecule, bool deep = true))
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m2.set(m);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(Molecule::Molecule& operator = (const Molecule& molecule))
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m2 = m;
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(Molecule::get(Molecule& molecule, bool deep = true) const )
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m.get(m2);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT

CHECK(Molecule::swap(Molecule& molecule))
	Molecule m("a");
	Molecule m2("b");
	Atom a1, a2, a3;
	m.append(a1);
	m.append(a2);
	m2.append(a3);
	m.swap(m2);
	TEST_EQUAL(m.getName(), "b")
	TEST_EQUAL(m.countAtoms(), 1)
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 2)
RESULT

CHECK(Molecule::getSystem())
	System s;
	Molecule m("a");
	s.append(m);
	m.getSystem()->setName("b");
	TEST_EQUAL(s.getName(), "b")
RESULT

CHECK(Molecule::getSystem() const )
	System s1("aya");
	Molecule m("a");
	s1.append(m);
	const System s(s1);
	TEST_EQUAL(m.getSystem()->getName(), "aya")
RESULT

CHECK(Molecule::prepend(Atom& atom))
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	TEST_EQUAL(m.getAtom(0), &a1)
	m.prepend(a2);
	TEST_EQUAL(m.getAtom(0), &a2)
	TEST_EQUAL(m.getAtom(1), &a1)
RESULT

CHECK(Molecule::append(Atom& atom))
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	m.append(a2);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a2)
RESULT

CHECK(Molecule::insert(Atom& atom))
	Molecule m;
	Atom a1;
	Atom a2;
	m.insert(a1);
	m.insert(a2);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a2)
RESULT

CHECK(Molecule::insertBefore(Atom& atom, Composite& before))
	Molecule m;
	Atom a1;
	Atom a2;
	Atom a3;
	m.insert(a1);
	m.append(a2);
	m.insertBefore(a3, a2);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a3)
	TEST_EQUAL(m.getAtom(2), &a2)
RESULT

CHECK(Molecule::insertAfter(Atom& atom, Composite& after))
	Molecule m;
	Atom a1;
	Atom a2;
	Atom a3;
	m.insert(a1);
	m.append(a2);
	m.insertAfter(a3, a1);
	TEST_EQUAL(m.getAtom(0), &a1)
	TEST_EQUAL(m.getAtom(1), &a3)
	TEST_EQUAL(m.getAtom(2), &a2)
RESULT

CHECK(Molecule::remove(Atom& atom))
	Molecule m("a");
	Atom a1;
	Atom a2;
	m.append(a1);
	m.append(a2);
	m.remove(a1);
	TEST_EQUAL(m.countAtoms(), 1)
	TEST_EQUAL(m.getAtom(0), &a2)
RESULT

CHECK(Molecule::prepend(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.prepend(bf2);
	m.prepend(bf3);
	TEST_EQUAL(m.getAtomContainer(1), &bf2)
	TEST_EQUAL(m.getAtomContainer(0), &bf3)
RESULT

CHECK(Molecule::append(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.append(bf2);
	m.append(bf3);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf3)
RESULT

CHECK(Molecule::insert(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.insert(bf2);
	m.insert(bf3);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf3)
RESULT

CHECK(Molecule::insertBefore(AtomContainer& atom_container, Composite& before))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	m.append(bf3);
	m.insertBefore(bf4, bf3);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf4)
	TEST_EQUAL(m.getAtomContainer(2), &bf3)
RESULT

CHECK(Molecule::insertAfter(AtomContainer& atom_container, Composite& after))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	m.append(bf3);
	m.insertAfter(bf4, bf2);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bf4)
	TEST_EQUAL(m.getAtomContainer(2), &bf3)
RESULT

CHECK(Molecule::spliceBefore(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	bf4.append(bf3);
	bf4.spliceBefore(m);
	TEST_EQUAL(m.getAtomContainer(0), 0)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getAtomContainer(0), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf3)
RESULT

CHECK(Molecule::spliceAfter(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	m.append(bf2);
	bf4.append(bf3);
	bf4.spliceAfter(m);
	TEST_EQUAL(m.getAtomContainer(0), 0)
	TEST_EQUAL(bf4.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getAtomContainer(1), &bf2)
	TEST_EQUAL(bf4.getAtomContainer(0), &bf3)
RESULT

CHECK(Molecule::splice(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	AtomContainer bf4;
	AtomContainer bf5;
	AtomContainer bfx;
	AtomContainer bfy;
	m.append(bf2);
	m.append(bfx);
	m.append(bf3);
	bfx.append(bfy);
	bf4.append(bf5);
	m.splice(bfx);
	TEST_EQUAL(m.getAtomContainer(0), &bf2)
	TEST_EQUAL(m.getAtomContainer(1), &bfy)
	TEST_EQUAL(m.getAtomContainer(2), &bf3)
	m.splice(bf4);
	TEST_EQUAL(m.getAtomContainer(0), &bf5)
	TEST_EQUAL(m.getAtomContainer(1), &bf2)
RESULT

CHECK(Molecule::remove(AtomContainer& atom_container))
	Molecule m;
	AtomContainer bf2;
	AtomContainer bf3;
	m.append(bf2);
	m.append(bf3);
	m.remove(bf2);
	TEST_EQUAL(m.getAtomContainer(0), &bf3)
RESULT

CHECK(Molecule::isValid() const )
	Molecule m("a");
	Atom a1;
	m.append(a1);
	TEST_EQUAL(m.isValid(), true)
RESULT

CHECK(Molecule::dump(std::ostream& s = std::cout, Size depth = 0) const )
	Molecule m("molecule1");
	Atom a1, a2;
	a1.setName("atom1");
	a2.setName("atom2");
	m.append(a1);
	m.append(a2);
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	m.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Molecule_test.txt", true)
RESULT

TextPersistenceManager pm;
using namespace RTTI;
pm.registerClass(getStreamName<Atom>(), Atom::createDefault);
pm.registerClass(getStreamName<Molecule>(), Molecule::createDefault);
NEW_TMP_FILE(filename)
CHECK(persistentWrite(PersistenceManager&, String, bool))
	std::ofstream	ofile(filename.c_str(), File::OUT);
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

CHECK(persistentRead(PersistenceManager&))
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

CHECK(operator ==)
	Molecule b1;
	Molecule b2;
	TEST_EQUAL(b1 == b2, false)
	b1 = b2;
	TEST_EQUAL(b1 == b1, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
