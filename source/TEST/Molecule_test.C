// $Id: Molecule_test.C,v 1.2 2000/04/12 18:01:48 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/COMMON/exception.h>
#include <BALL/CONCEPT/persistenceManager.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(class_name, "$Id: Molecule_test.C,v 1.2 2000/04/12 18:01:48 amoll Exp $")

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

CHECK(Molecule::persistentWrite(PersistenceManager& pm, const char* name = 0) const )
  //BAUSTELLE
RESULT

CHECK(Molecule::persistentRead(PersistenceManager& pm))
  //BAUSTELLE
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

CHECK(Molecule::prepend(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	m.prepend(bf2);
	m.prepend(bf3);
	TEST_EQUAL(m.getBaseFragment(1), &bf2)
	TEST_EQUAL(m.getBaseFragment(0), &bf3)
RESULT

CHECK(Molecule::append(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	m.append(bf2);
	m.append(bf3);
	TEST_EQUAL(m.getBaseFragment(0), &bf2)
	TEST_EQUAL(m.getBaseFragment(1), &bf3)
RESULT

CHECK(Molecule::insert(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	m.insert(bf2);
	m.insert(bf3);
	TEST_EQUAL(m.getBaseFragment(0), &bf2)
	TEST_EQUAL(m.getBaseFragment(1), &bf3)
RESULT

CHECK(Molecule::insertBefore(BaseFragment& base_fragment, Composite& before))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	m.append(bf2);
	m.append(bf3);
	m.insertBefore(bf4, bf3);
	TEST_EQUAL(m.getBaseFragment(0), &bf2)
	TEST_EQUAL(m.getBaseFragment(1), &bf4)
	TEST_EQUAL(m.getBaseFragment(2), &bf3)
RESULT

CHECK(Molecule::insertAfter(BaseFragment& base_fragment, Composite& after))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	m.append(bf2);
	m.append(bf3);
	m.insertAfter(bf4, bf2);
	TEST_EQUAL(m.getBaseFragment(0), &bf2)
	TEST_EQUAL(m.getBaseFragment(1), &bf4)
	TEST_EQUAL(m.getBaseFragment(2), &bf3)
RESULT

CHECK(Molecule::spliceBefore(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	m.append(bf2);
	bf4.append(bf3);
	bf4.spliceBefore(m);
	TEST_EQUAL(m.getBaseFragment(0), 0)
	TEST_EQUAL(bf4.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(1), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getBaseFragment(0), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(1), &bf3)
RESULT

CHECK(Molecule::spliceAfter(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	m.append(bf2);
	bf4.append(bf3);
	bf4.spliceAfter(m);
	TEST_EQUAL(m.getBaseFragment(0), 0)
	TEST_EQUAL(bf4.getBaseFragment(1), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(0), &bf3)
	bf4.spliceBefore(bf4);
	TEST_EQUAL(bf4.getBaseFragment(1), &bf2)
	TEST_EQUAL(bf4.getBaseFragment(0), &bf3)
RESULT

CHECK(Molecule::splice(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	BaseFragment bf4;
	BaseFragment bf5;
	BaseFragment bfx;
	BaseFragment bfy;
	m.append(bf2);
	m.append(bfx);
	m.append(bf3);
	bfx.append(bfy);
	bf4.append(bf5);
	m.splice(bfx);
	TEST_EQUAL(m.getBaseFragment(0), &bf2)
	TEST_EQUAL(m.getBaseFragment(1), &bfy)
	TEST_EQUAL(m.getBaseFragment(2), &bf3)
	m.splice(bf4);
	TEST_EQUAL(m.getBaseFragment(0), &bf5)
	TEST_EQUAL(m.getBaseFragment(1), &bf2)
RESULT

CHECK(Molecule::remove(BaseFragment& base_fragment))
	Molecule m;
	BaseFragment bf2;
	BaseFragment bf3;
	m.append(bf2);
	m.append(bf3);
	m.remove(bf2);
	TEST_EQUAL(m.getBaseFragment(0), &bf3)
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
	std::ofstream outfile(filename.c_str(), ios::out);
	m.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Molecule_test.txt", true)
RESULT


CHECK(Molecule::read(std::istream& s))
/*std::ifstream instr("data/Molecule.txt2");
	Molecule m;
	m.read(instr);
	instr.close();
	TEST_EQUAL(m.getName(), "molecule1")	
	TEST_EQUAL(m.getBaseFragment(0)->getName(), "BF")	
	TEST_EQUAL(m.getAtom(0)->getName(), "atom1")	*/
RESULT

CHECK(Molecule::write(std::ostream& s) const ) // NotImplemented
/*	Molecule m("molecule1");
	Atom a1;
	BaseFragment Bf;
	Bf.setName("BF");
	a1.setName("atom1");
	m.append(a1);
	m.append(Bf);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	m.write(outstr);
	outstr.close();*/
RESULT

CHECK(Molecule::BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
