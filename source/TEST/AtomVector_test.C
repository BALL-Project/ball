// $Id: AtomVector_test.C,v 1.1 2000/03/25 22:41:19 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>

///////////////////////////

START_TEST(AtomVector, "$Id: AtomVector_test.C,v 1.1 2000/03/25 22:41:19 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(AtomVector::AtomVector())
  //BAUSTELLE
RESULT


CHECK(AtomVector::AtomVector(const Composite& composite, bool selected_only = false))
  //BAUSTELLE
RESULT


CHECK(AtomVector::AtomVector(const AtomVector& atoms, bool deep = true))
  //BAUSTELLE
RESULT


CHECK(AtomVector::~AtomVector())
  //BAUSTELLE
RESULT


CHECK(AtomVector::AtomVector& operator = (const AtomVector& rhs))
  //BAUSTELLE
RESULT


CHECK(AtomVector::set(const AtomVector& atoms))
  //BAUSTELLE
RESULT


CHECK(AtomVector::AtomVector& operator = (const Composite& rhs))
  //BAUSTELLE
RESULT


CHECK(AtomVector::set(const Composite& composite, bool selected_only = false))
  //BAUSTELLE
RESULT


CHECK(AtomVector::savePositions())
  //BAUSTELLE
RESULT


CHECK(AtomVector::resetPositions())
  //BAUSTELLE
RESULT


CHECK(AtomVector::moveTo(const Gradient& direction, double step = 1.0))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
