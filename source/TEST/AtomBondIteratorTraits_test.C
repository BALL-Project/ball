// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: AtomBondIteratorTraits_test.C,v 1.1 2003/06/25 13:31:17 amoll Exp $
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

START_TEST(Atom, "$Id: AtomBondIteratorTraits_test.C,v 1.1 2003/06/25 13:31:17 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BondIteratorPosition_& getPosition() throw())
  // ???
RESULT

CHECK(BondIteratorTraits& operator = (const BondIteratorTraits& traits) throw())
  // ???
RESULT

CHECK(BondIteratorTraits() throw())
  // ???
RESULT

CHECK(BondIteratorTraits(const Atom& atom) throw())
  // ???
RESULT

CHECK(BondIteratorTraits(const BondIteratorTraits& traits, bool /* deep */ = true) throw())
  // ???
RESULT

CHECK(Distance getDistance(const BondIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(StaticAtomAttributes& operator = (const StaticAtomAttributes& attr))
  // ???
RESULT

CHECK(String getFullName(FullNameType type = ADD_VARIANT_EXTENSIONS) const throw())
  // ???
RESULT

CHECK(String getTypeName() const throw())
  // ???
RESULT

CHECK(bool applyBonds(UnaryProcessor<Bond>& processor) throw())
  // ???
RESULT

CHECK(bool isBegin() const throw())
  // ???
RESULT

CHECK(bool isEnd() const throw())
  // ???
RESULT

CHECK(bool isGeminal(const Atom& atom) const throw())
  // ???
RESULT

CHECK(bool isRBegin() const throw())
  // ???
RESULT

CHECK(bool isREnd() const throw())
  // ???
RESULT

CHECK(bool isSingular() const throw())
  // ???
RESULT

CHECK(bool isVicinal(const Atom& atom) const throw())
  // ???
RESULT

CHECK(bool operator != (const BondIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(bool operator < (const BondIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const BondIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(const Bond& getData() const throw())
  // ???
RESULT

CHECK(const Bond& getData(Index index) const throw())
  // ???
RESULT

CHECK(const BondIteratorPosition_& getPosition() const throw())
  // ???
RESULT

CHECK(friend std::ostream& operator << (std::ostream& s, const BondIteratorTraits& traits) throw())
  // ???
RESULT

CHECK(void backward() throw())
  // ???
RESULT

CHECK(void backward(Distance distance) throw())
  // ???
RESULT

CHECK(void clear())
  // ???
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
  // ???
RESULT

CHECK(void dump(std::ostream& s) const throw())
  // ???
RESULT

CHECK(void forward() throw())
  // ???
RESULT

CHECK(void forward(Distance distance) throw())
  // ???
RESULT

CHECK(void invalidate() throw())
  // ???
RESULT

CHECK(void toBegin() throw())
  // ???
RESULT

CHECK(void toEnd() throw())
  // ???
RESULT

CHECK(void toRBegin() throw())
  // ???
RESULT

CHECK(void toREnd() throw())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
