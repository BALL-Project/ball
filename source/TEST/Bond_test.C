// $Id: Bond_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/COMMON/exception.h>
#include <BALL/CONCEPT/persistenceManager.h>
///////////////////////////

START_TEST(Bond, "$Id: Bond_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
	
Bond*	b;
CHECK(Bond())
b = new Bond;
TEST_NOT_EQUAL(b, 0)
RESULT											

CHECK(~Bond())
delete b;
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
b1.getName() = "abc";
TEST_EQUAL(b1.getName(), "abc")
b1.getName() = "";
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

CHECK(getLength())
Atom a1;
Atom a2;
a1.setPosition(Vector3(1.0, 1.0, 1.0));
a2.setPosition(Vector3(2.0, 2.0, 2.0));
Bond b1("bond", a1, a2);
TEST_REAL_EQUAL(b1.getLength(), sqrt(3))
Bond b2;
TEST_EXCEPTION(Exception::NullPointer, b2.getLength())
b2.setFirstAtom(&a1);
TEST_EXCEPTION(Exception::NullPointer, b2.getLength())
b2.setSecondAtom(&a2);
TEST_REAL_EQUAL(b2.getLength(), sqrt(3))
RESULT

CHECK(createBond(Bond&, Atom&, Atom&))
//BAUSTELLE
RESULT

CHECK(clone(bool))
//BAUSTELLE
RESULT

CHECK(clear())
//BAUSTELLE
RESULT

CHECK(destroy())
//BAUSTELLE
RESULT

CHECK(persistentWrite(PersistenceManager&, String&, bool))
//BAUSTELLE
RESULT

CHECK(persistentRead(PersistenceManager&))
//BAUSTELLE
RESULT

CHECK(set(Bond&, bool))
//BAUSTELLE
RESULT

CHECK(operator = (Bond&))
//BAUSTELLE
RESULT

CHECK(get(Bond&, bool))
//BAUSTELLE
RESULT

CHECK(swap())
//BAUSTELLE
RESULT

CHECK(getBond(Atom&, Atom&))
//BAUSTELLE
RESULT

CHECK(getBondedAtomOf(Atom&))
//BAUSTELLE
RESULT

CHECK(getBondedAtomOf(Atom&) const )
//BAUSTELLE
RESULT

CHECK(isBondOf(Atom&))
//BAUSTELLE
RESULT

CHECK(isBonded())
//BAUSTELLE
RESULT

CHECK(isInterBond())
//BAUSTELLE
RESULT

CHECK(isInterBondOf(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(isInterBondOf(BaseFragment&) const)
//BAUSTELLE
RESULT

CHECK(isIntraBond())
//BAUSTELLE
RESULT

CHECK(isIntraBondOf(BaseFragment&))
//BAUSTELLE
RESULT

CHECK(isIntraBondOf(BaseFragment&) const)
//BAUSTELLE
RESULT

CHECK(isValid())
//BAUSTELLE
RESULT

CHECK(dump(ostream&, unsigned long))
//BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
