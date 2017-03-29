// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
///////////////////////////

START_TEST(Atom)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Atom::BondIteratorTraits* bit_ptr = 0;
CHECK(Atom::BondIteratorTraits() throw())
	bit_ptr = new Atom::BondIteratorTraits;
	TEST_NOT_EQUAL(bit_ptr, 0)
RESULT

CHECK(Atom::~BondIteratorTraits() throw())
	delete bit_ptr;
RESULT

CHECK(Atom::BondIteratorTraits(const Atom& atom) throw())
	Atom atom;
	Atom::BondIteratorTraits bit(atom);
	TEST_EQUAL(&atom, bit.getContainer())
RESULT

CHECK(Atom::BondIteratorTraits(const Atom::BondIteratorTraits& traits, bool /* deep */ = true) throw())
	Atom atom;
	Atom::BondIteratorTraits bit1(atom);
	Atom::BondIteratorTraits bit2(bit1);
	TEST_EQUAL(bit1.getContainer(), bit2.getContainer())
	TEST_EQUAL((bit1 == bit2), true)
RESULT

CHECK(Atom::BondIteratorTraits& operator = (const Atom::BondIteratorTraits& traits) throw())
	Atom atom;
	Atom::BondIteratorTraits bit1(atom);
	Atom::BondIteratorTraits bit2 = bit1;
	TEST_EQUAL(bit1.getContainer(), bit2.getContainer())
	TEST_EQUAL((bit1 == bit2), true)
RESULT

CHECK(Atom* getContainer() throw())
	Atom::BondIteratorTraits bit;
	TEST_EQUAL(bit.getContainer(), 0)
	Atom atom;
	Atom::BondIteratorTraits bit2(atom);
	TEST_EQUAL(&atom, bit2.getContainer())
RESULT

CHECK(bool isSingular() const throw())
	Atom::BondIteratorTraits bit;
	TEST_EQUAL(bit.isSingular(), true)
	Atom atom;
	Atom::BondIteratorTraits bit2(atom);
	TEST_EQUAL(bit2.isSingular(), false)
RESULT

Atom atom_a;
Atom atom_b1;
Atom atom_b2;
Bond* bond_a_b1 = atom_a.createBond(atom_b1);
Bond* bond_a_b2 = atom_a.createBond(atom_b2);
const Bond& bond_a_b1_const_ref = *bond_a_b1;
const Bond& bond_a_b2_const_ref = *bond_a_b2;

CHECK(BondIteratorPosition_& getPosition() throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.getPosition(), 0)
	bit.forward();
	TEST_EQUAL(bit.getPosition(), 1)
RESULT

CHECK(const BondIteratorPosition_& getPosition() const throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.getPosition(), 0)
	bit.forward();
	TEST_EQUAL(bit.getPosition(), 1)
RESULT

CHECK(bool operator == (const Atom::BondIteratorTraits& traits) const throw())
	Atom::BondIteratorTraits bit1(atom_a);
	bit1.forward();
	Atom::BondIteratorTraits bit2;
	TEST_EQUAL((bit1 == bit2), false);
	Atom::BondIteratorTraits bit3(atom_a);
	TEST_EQUAL((bit1 == bit3), false);
	bit3.forward();
	TEST_EQUAL((bit1 == bit3), true);
RESULT

CHECK(bool operator != (const Atom::BondIteratorTraits& traits) const throw())
	Atom::BondIteratorTraits bit1(atom_a);
	bit1.forward();
	Atom::BondIteratorTraits bit2;
	TEST_EQUAL((bit1 != bit2), true);
	Atom::BondIteratorTraits bit3(atom_a);
	TEST_EQUAL((bit1 != bit3), true);
	bit3.forward();
	TEST_EQUAL((bit1 != bit3), false);
RESULT

CHECK(bool operator < (const Atom::BondIteratorTraits& traits) const throw())
	Atom::BondIteratorTraits bit1(atom_a);
	Atom::BondIteratorTraits bit2(atom_a);
	TEST_EQUAL((bit1 < bit2), false);
	bit2.forward();
	TEST_EQUAL((bit1 < bit2), true);
RESULT

CHECK(Distance getDistance(const Atom::BondIteratorTraits& traits) const throw())
	Atom::BondIteratorTraits bit1(atom_a);
	Atom::BondIteratorTraits bit2(atom_a);
	TEST_EQUAL(bit1.getDistance(bit2), 0)
	bit1.forward();
	TEST_EQUAL(bit1.getDistance(bit2), 1)
RESULT

CHECK(bool isValid() const throw())
	Atom::BondIteratorTraits bit1;
	TEST_EQUAL(bit1.isValid(), false)
	Atom::BondIteratorTraits bit2(atom_a);
	TEST_EQUAL(bit2.isValid(), true)
RESULT

CHECK(void invalidate() throw())
	Atom::BondIteratorTraits bit1;
	TEST_EQUAL(bit1.isValid(), false)
	Atom::BondIteratorTraits bit2(atom_a);
	TEST_EQUAL(bit2.isValid(), true)
	bit2.invalidate();
	TEST_EQUAL(bit2.isValid(), false)
RESULT

CHECK(void toBegin() throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.forward();
	bit.toBegin();
	TEST_EQUAL(bit.getPosition(), 0)
RESULT

CHECK(bool isBegin() const throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.isBegin(), true);
	bit.forward();
	TEST_EQUAL(bit.isBegin(), false);
RESULT

CHECK(void toEnd() throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.toEnd();
	TEST_EQUAL(bit.getPosition(), 2)
RESULT

CHECK(bool isEnd() const throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.isEnd(), false)
	bit.toEnd();
	TEST_EQUAL(bit.isEnd(), true)
RESULT

CHECK(Bond& getData() throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(&bit.getData(), bond_a_b1)
	bit.forward();
	TEST_EQUAL(&bit.getData(), bond_a_b2)
RESULT
	
CHECK(const Bond& getData() const throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL((bit.getData() == bond_a_b1_const_ref), true);
	bit.forward();
	TEST_EQUAL((bit.getData() == bond_a_b2_const_ref), true);
RESULT

CHECK(void forward() throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.getPosition(), 0)
	bit.forward();
	TEST_EQUAL(bit.getPosition(), 1)
RESULT

CHECK(friend std::ostream& operator << (std::ostream& s, const Atom::BondIteratorTraits& traits) throw())
  // ???
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
  // ???
RESULT

CHECK(void toRBegin() throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.toRBegin();
	TEST_EQUAL(bit.getPosition(), 1);
RESULT

CHECK(void isRBegin() throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.isRBegin(), false)
	bit.toRBegin();
	TEST_EQUAL(bit.isRBegin(), true)
RESULT

CHECK(void toREnd() throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.toREnd();
	TEST_EQUAL(bit.getPosition(), -1)
RESULT

CHECK(bool isREnd() const throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(bit.isREnd(), false)
	bit.toREnd();
	TEST_EQUAL(bit.isREnd(), true)
RESULT

CHECK(void backward() throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.backward();
	TEST_EQUAL(bit.getPosition(), -1)
RESULT

CHECK(void backward(Distance distance) throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.toEnd();
	bit.backward(2);
	TEST_EQUAL(bit.getPosition(), 0)
RESULT

CHECK(void forward(Distance distance) throw())
	Atom::BondIteratorTraits bit(atom_a);
	bit.forward(2);
	TEST_EQUAL(bit.getPosition(), 2)
RESULT

CHECK(Bond& getData(Index index) throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL(&bit.getData(0), bond_a_b1)
	TEST_EQUAL(&bit.getData(1), bond_a_b2)
RESULT

CHECK(const Bond& getData(Index index) const throw())
	Atom::BondIteratorTraits bit(atom_a);
	TEST_EQUAL((&bit.getData(0) == &bond_a_b1_const_ref), true)
	TEST_EQUAL((&bit.getData(1) == &bond_a_b2_const_ref), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
