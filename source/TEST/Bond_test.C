// $Id: Bond_test.C,v 1.9 2000/05/26 19:25:01 amoll Exp $
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

START_TEST(Bond, "$Id: Bond_test.C,v 1.9 2000/05/26 19:25:01 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;
using namespace RTTI;
	
Bond*	b;

CHECK(Bond())
	b = new Bond;
	TEST_NOT_EQUAL(b, 0)
RESULT											

CHECK(~Bond())
	delete b;
RESULT

CHECK(Bond(const Bond& bond, bool deep = true))
	Atom a1;
	Atom a2;
	Bond b1, b2;
	b1.setFirstAtom(&a1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	b1.setFirstAtom(&a2);
	TEST_EQUAL(b1.getFirstAtom(), &a2);
RESULT		

CHECK(Bond(String&, Atom&, Atom&, Order, Type))
	Atom a1;
	Atom a2;
	Bond& b1 = *new Bond("name", a1, a2, 1, 2);
	TEST_NOT_EQUAL(&b1, 0)
	TEST_EQUAL(b1.getName(), "name")
	TEST_EQUAL(b1.getFirstAtom(), &a1)
	TEST_EQUAL(b1.getSecondAtom(), &a2)
	TEST_EQUAL(b1.getOrder(), 1)
	TEST_EQUAL(b1.getType(), 2)
	delete &b1;
RESULT

CHECK(createBond(Bond&, Atom&, Atom&))
/*	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2), b2;
	b2.createBond(b1, a1, a2)
	TEST_EQUAL(b2.getFirstAtom(), &a1)
	TEST_EQUAL(b2.getSecondAtom(), &a2)
	TEST_EQUAL(a1.countBonds(), 1)
	TEST_EQUAL(a2.countBonds(), 1) ???
	Exception*/
RESULT

CHECK(clear())
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2);
	b1.clear();
	TEST_EQUAL(a1.countBonds(), 0)
	TEST_EQUAL(a2.countBonds(), 0)
	TEST_EQUAL(b1.getFirstAtom(), 0)
	TEST_EQUAL(b1.getSecondAtom(), 0)
	TEST_EQUAL(b1.getName(), "")
	TEST_EQUAL(b1.getOrder(), Bond::ORDER__UNKNOWN)
	TEST_EQUAL(b1.getType(), Bond::TYPE__UNKNOWN)
RESULT

CHECK(destroy())
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2);
	b1.destroy();
	TEST_EQUAL(a1.countBonds(), 0)
	TEST_EQUAL(a2.countBonds(), 0)
RESULT

CHECK(set(Bond&, bool))
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2), b2;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("abc");
	b2.set(b1);
	TEST_EQUAL(b1.getFirstAtom(), b2.getFirstAtom())
	TEST_EQUAL(b1.getSecondAtom(), b2.getSecondAtom())
	TEST_EQUAL(b1.getName(), b2.getName())
	TEST_EQUAL(b1.getOrder(), b2.getOrder())
	TEST_EQUAL(b1.getType(), b2.getType())
RESULT

CHECK(operator = (Bond&))
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2), b2;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("abc");
	b2 = b1;
	TEST_EQUAL(b1.getFirstAtom(), b2.getFirstAtom())
	TEST_EQUAL(b1.getSecondAtom(), b2.getSecondAtom())
	TEST_EQUAL(b1.getName(), b2.getName())
	TEST_EQUAL(b1.getOrder(), b2.getOrder())
	TEST_EQUAL(b1.getType(), b2.getType())
RESULT

CHECK(get(Bond&, bool))
	Atom a1;
	Atom a2;
	Bond b1("bond", a1, a2), b2;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("abc");
	b2.get(b1);
	TEST_EQUAL(b1.getFirstAtom(), b2.getFirstAtom())
	TEST_EQUAL(b1.getSecondAtom(), b2.getSecondAtom())
	TEST_EQUAL(b1.getName(), b2.getName())
	TEST_EQUAL(b1.getOrder(), b2.getOrder())
	TEST_EQUAL(b1.getType(), b2.getType())
RESULT

CHECK(swap())
	Atom a1, a2;
	Atom a3, a4;
	Bond b1("bond1", a1, a2), b2;
	Bond b3("bond2", a3, a4), b4;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("a");
	b3.setType(0);
	b3.setOrder(0);
	b3.setName("b");
	b2 = b1;
	b4 = b3;
	b4.swap(b1);
	TEST_EQUAL(b1.getFirstAtom(), b3.getFirstAtom())
	TEST_EQUAL(b1.getSecondAtom(), b3.getSecondAtom())
	TEST_EQUAL(b1.getName(), b3.getName())
	TEST_EQUAL(b1.getOrder(), b3.getOrder())
	TEST_EQUAL(b1.getType(), b3.getType())

	TEST_EQUAL(b2.getFirstAtom(), b4.getFirstAtom())
	TEST_EQUAL(b2.getSecondAtom(), b4.getSecondAtom())
	TEST_EQUAL(b2.getName(), b4.getName())
	TEST_EQUAL(b2.getOrder(), b4.getOrder())
	TEST_EQUAL(b2.getType(), b4.getType())
RESULT

CHECK(setFirstAtom(Atom*))
	Atom a1;
	Bond b1;
	b1.setFirstAtom(&a1);
RESULT

CHECK(getFirstAtom())
	Atom a1;
	Atom a2;
	Bond b1;
	b1.setFirstAtom(&a1);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
	b1.setFirstAtom(&a2);
	TEST_EQUAL(b1.getFirstAtom(), &a2);
RESULT

CHECK(getFirstAtom() const)
	Atom a1;
	Atom a2;
	const Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.getFirstAtom(), &a1);
RESULT

CHECK(setSecond(Atom*))
	Atom a1;
	Bond b1;
	b1.setSecondAtom(&a1);
RESULT

CHECK(getSecondAtom())
	Atom a1;
	Atom a2;
	Bond b1;
	b1.setSecondAtom(&a1);
	TEST_EQUAL(b1.getSecondAtom(), &a1);
	b1.setSecondAtom(&a2);
	TEST_EQUAL(b1.getSecondAtom(), &a2);
RESULT

CHECK(getSecondAtom() const)
	Atom a1;
	Atom a2;
	const Bond* b1 = new Bond("bond", a1, a2);
	TEST_EQUAL(b1->getSecondAtom(), &a2);
RESULT

CHECK(setName(String&))
	Bond b1;
	b1.setName("abc");
	b1.setName("");
RESULT

CHECK(getName())
	Bond b1;
	b1.setName("abc");
	TEST_EQUAL(b1.getName(), "abc")
	b1.setName("");
	TEST_EQUAL(b1.getName(), "")
RESULT

CHECK(setOrder(Order))
	Bond b1;
	b1.setOrder(0);
	b1.setOrder(1);
RESULT

CHECK(getOrder())
	Bond b1;
	b1.setOrder(1);
	TEST_EQUAL(b1.getOrder(), 1)
	b1.setOrder(0);
	TEST_EQUAL(b1.getOrder(), 0)
RESULT

CHECK(setType(Type))
	Bond b1;
	b1.setType(0);
	b1.setType(1);
RESULT

CHECK(getType())
	Bond b1;
	b1.setType(1);
	TEST_EQUAL(b1.getType(), 1)
	b1.setType(0);
	TEST_EQUAL(b1.getType(), 0)
RESULT

CHECK(getLength())
	Atom a1;
	Atom a2;
	a1.setPosition(Vector3(1.0, 1.0, 1.0));
	a2.setPosition(Vector3(2.0, 2.0, 2.0));
	Bond b1("bond", a1, a2);
	TEST_REAL_EQUAL(b1.getLength(), ::sqrt(3.0))
	Bond b2;
	TEST_EXCEPTION(Exception::NullPointer, b2.getLength())
	b2.setFirstAtom(&a1);
	TEST_EXCEPTION(Exception::NullPointer, b2.getLength())
	b2.setSecondAtom(&a2);
	TEST_REAL_EQUAL(b2.getLength(), ::sqrt(3.0))
RESULT

CHECK(getBond(Atom&, Atom&))
/*	Atom a1;
	Atom a2;
	a1.setName("a1");
	a2.setName("a2");
	Bond b1("bond", a1, a2), b2;
	b1.setType(1);
	b1.setOrder(1);
	b1.setName("abc");
	b2.getBond(a1, a2);
	TEST_EQUAL(b1.getFirstAtom()->getName(), b2.getFirstAtom()->getName())
	TEST_EQUAL(b1.getSecondAtom()->getName(), b2.getSecondAtom()->getName())*/
RESULT

CHECK(getBondedAtomOf(Atom&))
	Atom a1, a2;
	Bond b1("bond", a1, a2);
	b1.getBondedAtomOf(a1)->setCharge(0.1);
	TEST_REAL_EQUAL(a2.getCharge(), 0.1)
RESULT

CHECK(getBondedAtomOf(Atom&) const )
	Atom a1, a2;
	a2.setCharge(0.1);
	Bond b1("bond", a1, a2);
	const Atom* a3 = b1.getBondedAtomOf(a1);
	TEST_REAL_EQUAL(a3->getCharge(), 0.1)
RESULT

CHECK(isBondOf(Atom&))
	Atom a1, a2, a3;
	Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.isBondOf(a1), true)
	TEST_EQUAL(b1.isBondOf(a2), true)
	TEST_EQUAL(b1.isBondOf(a3), false)
RESULT

CHECK(isBonded())
	Atom a1, a2, a3;
	Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.isBonded(), true)
	TEST_EQUAL(b1.isBondOf(a3), false)
RESULT

CHECK(isInterBond())
	Fragment f("F1");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isInterBond(), false)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isInterBond(), true)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isInterBond(), true)

	Molecule m("m1");
	System s("S1");
	Atom a5, a6, a7, a8;
	m.append(a5);
	m.append(a6);
	Bond b4("bond2", a5, a6);
	TEST_EQUAL(b4.isInterBond(), false)
RESULT

CHECK(isInterBondOf(BaseFragment&) const)
	Fragment f("F1");
	Fragment f2("F2");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	f2.append(a3);
	f2.append(a4);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isInterBondOf(f), false)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isInterBondOf(f), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isInterBondOf(f), true)
RESULT

CHECK(isInterBondOf(System& system) const)
	Molecule m("m1");
	System s("S1");
	Molecule m2("m2");
	System s2("S2");
	Atom a1, a2, a3, a4;
	m.append(a1);
	m.append(a2);
	m2.append(a3);
	m2.append(a4);
	Bond b1("bond1", a1, a2);
	s.append(m);
	TEST_EQUAL(b1.isInterBondOf(s), false)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isInterBondOf(s), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isInterBondOf(s), true)
RESULT

CHECK(isIntraBond())
	Fragment f("F1");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isIntraBond(), true)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isIntraBond(), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isIntraBond(), false)
RESULT

CHECK(isIntraBondOf(BaseFragment&))
	Fragment f("F1");
	Fragment f2("F2");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	f2.append(a3);
	f2.append(a4);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isIntraBondOf(f), true)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isIntraBondOf(f), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isIntraBondOf(f), false)
RESULT

CHECK(isIntraBondOf(BaseFragment&) const)
	Fragment f("F1");
	Fragment f2("F2");
	Atom a1, a2, a3, a4;
	f.append(a1);
	f.append(a2);
	f2.append(a3);
	f2.append(a4);
	Bond b1("bond1", a1, a2);
	TEST_EQUAL(b1.isIntraBondOf(f), true)
	Bond b2("bond2", a3, a4);
	TEST_EQUAL(b2.isIntraBondOf(f), false)
	Bond b3("bond3", a1, a3);
	TEST_EQUAL(b3.isIntraBondOf(f), false)
RESULT

CHECK(isValid())
	Atom a1, a2;
	Bond b1("bond", a1, a2);
	TEST_EQUAL(b1.isValid(), true)
RESULT

CHECK(dump(ostream&, Size))
	Atom a1;
	Atom a2;
	a1.setName("a1");
	a2.setName("a2");
	Bond b1("bond", a1, a2);
	b1.setType(1);
	b1.setOrder(1);
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	b1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Bond_test.txt", true)
RESULT

CHECK(finalize())
//BAUSTELLE
RESULT

CHECK(persistentWrite(PersistenceManager&, String&, bool))
//BAUSTELLE
RESULT

CHECK(persistentRead(PersistenceManager&))
//BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
