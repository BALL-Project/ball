// $Id: Molecule_test.C,v 1.1 2000/04/12 17:29:29 amoll Exp $
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

START_TEST(class_name, "$Id: Molecule_test.C,v 1.1 2000/04/12 17:29:29 amoll Exp $")

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


//line 78: method Molecule::persistentWrite(PersistenceManager& pm, const char* name = 0) const 
CHECK(Molecule::persistentWrite(PersistenceManager& pm, const char* name = 0) const )
  //BAUSTELLE
RESULT


//line 81: method Molecule::persistentRead(PersistenceManager& pm)
CHECK(Molecule::persistentRead(PersistenceManager& pm))
  //BAUSTELLE
RESULT


//line 90: method Molecule::set(const Molecule& molecule, bool deep = true)
CHECK(Molecule::set(const Molecule& molecule, bool deep = true))
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m2.set(m);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT


//line 93: method Molecule::Molecule& operator = (const Molecule& molecule)
CHECK(Molecule::Molecule& operator = (const Molecule& molecule))
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m2 = m;
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT


//line 96: method Molecule::get(Molecule& molecule, bool deep = true) const 
CHECK(Molecule::get(Molecule& molecule, bool deep = true) const )
	Molecule m("a"), m2;
	Atom a1;
	m.append(a1);
	m.get(m2);
	TEST_EQUAL(m2.getName(), "a")
	TEST_EQUAL(m2.countAtoms(), 1)
RESULT


//line 99: method Molecule::swap(Molecule& molecule)
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


//line 108: method Molecule::getSystem()
CHECK(Molecule::getSystem())
	System s;
	Molecule m("a");
	s.append(m);
	m.getSystem()->setName("b");
	TEST_EQUAL(s.getName(), "b")
RESULT


//line 111: method Molecule::getSystem() const 
CHECK(Molecule::getSystem() const )
	System s1("aya");
	Molecule m("a");
	s1.append(m);
	const System s(s1);
	TEST_EQUAL(m.getSystem()->getName(), "aya")
RESULT


//line 114: method Molecule::prepend(Atom& atom)
CHECK(Molecule::prepend(Atom& atom))
  //BAUSTELLE
RESULT


//line 117: method Molecule::append(Atom& atom)
CHECK(Molecule::append(Atom& atom))
  //BAUSTELLE
RESULT


//line 120: method Molecule::insert(Atom& atom)
CHECK(Molecule::insert(Atom& atom))
  //BAUSTELLE
RESULT

//line 123: method Molecule::insertBefore(Atom& atom, Composite& before)
CHECK(Molecule::insertBefore(Atom& atom, Composite& before))
  //BAUSTELLE
RESULT


//line 126: method Molecule::insertAfter(Atom& atom, Composite& after)
CHECK(Molecule::insertAfter(Atom& atom, Composite& after))
  //BAUSTELLE
RESULT


//line 129: method Molecule::remove(Atom& atom)
CHECK(Molecule::remove(Atom& atom))
	Molecule m("a");
	Atom a1;
	m.append(a1);
	m.remove(a1);
	TEST_EQUAL(m.countAtoms(), 0)
RESULT


//line 132: method Molecule::prepend(BaseFragment& base_fragment)
CHECK(Molecule::prepend(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 135: method Molecule::append(BaseFragment& base_fragment)
CHECK(Molecule::append(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 138: method Molecule::insert(BaseFragment& base_fragment)
CHECK(Molecule::insert(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 141: method Molecule::insertBefore(BaseFragment& base_fragment, Composite& before)
CHECK(Molecule::insertBefore(BaseFragment& base_fragment, Composite& before))
  //BAUSTELLE
RESULT


//line 144: method Molecule::insertAfter(BaseFragment& base_fragment, Composite& after)
CHECK(Molecule::insertAfter(BaseFragment& base_fragment, Composite& after))
  //BAUSTELLE
RESULT


//line 147: method Molecule::spliceBefore(BaseFragment& base_fragment)
CHECK(Molecule::spliceBefore(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 150: method Molecule::spliceAfter(BaseFragment& base_fragment)
CHECK(Molecule::spliceAfter(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 153: method Molecule::splice(BaseFragment& base_fragment)
CHECK(Molecule::splice(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 156: method Molecule::remove(BaseFragment& base_fragment)
CHECK(Molecule::remove(BaseFragment& base_fragment))
  //BAUSTELLE
RESULT


//line 165: method Molecule::isValid() const 
CHECK(Molecule::isValid() const )
	Molecule m("a");
	Atom a1;
	m.append(a1);
	TEST_EQUAL(m.isValid(), true)
RESULT


//line 168: method Molecule::dump(std::ostream& s = std::cout, Size depth = 0) const 
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


//line 177: method Molecule::read(std::istream& s)
CHECK(Molecule::read(std::istream& s))
  //BAUSTELLE
RESULT


//line 180: method Molecule::write(std::ostream& s) const 
CHECK(Molecule::write(std::ostream& s) const )
	Molecule m("molecule1");
	Atom a1, a2;
	a1.setName("atom1");
	a2.setName("atom2");
	m.append(a1);
	m.append(a2);
	std::ofstream outstr(filename.c_str(), std::ios::out);
	m.write(outstr);
	outstr.close();
RESULT


//line 188: method Molecule::BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment)
CHECK(Molecule::BALL_KERNEL_DEFINE_ITERATOR_CREATORS(Fragment))
  //BAUSTELLE
RESULT


//line 192: method Molecule::getMolecule() const 
CHECK(Molecule::getMolecule() const )
  //BAUSTELLE
RESULT


//line 194: method Molecule::getSuperBaseFragment()
CHECK(Molecule::getSuperBaseFragment())
  //BAUSTELLE
RESULT


//line 196: method Molecule::getSuperBaseFragment() const 
CHECK(Molecule::getSuperBaseFragment() const )
  //BAUSTELLE
RESULT


//line 198: method Molecule::prepend(Molecule& molecule)
CHECK(Molecule::prepend(Molecule& molecule))
  //BAUSTELLE
RESULT


//line 200: method Molecule::append(Molecule& molecule)
CHECK(Molecule::append(Molecule& molecule))
  //BAUSTELLE
RESULT


//line 202: method Molecule::insert(Molecule& molecule)
CHECK(Molecule::insert(Molecule& molecule))
  //BAUSTELLE
RESULT


//line 204: method Molecule::insertBefore(Molecule& molecule, Composite& composite)
CHECK(Molecule::insertBefore(Molecule& molecule, Composite& composite))
  //BAUSTELLE
RESULT


//line 206: method Molecule::insertAfter(Molecule& molecule, Composite& composite)
CHECK(Molecule::insertAfter(Molecule& molecule, Composite& composite))
  //BAUSTELLE
RESULT


//line 208: method Molecule::remove(Molecule& molecule)
CHECK(Molecule::remove(Molecule& molecule))
  //BAUSTELLE
RESULT


//line 210: method Molecule::isSubBaseFragmentOf(const BaseFragment& base_fragment) const 
CHECK(Molecule::isSubBaseFragmentOf(const BaseFragment& base_fragment) const )
  //BAUSTELLE
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
