// $Id: Gradient_test.C,v 1.1 2000/03/25 22:41:19 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/gradient.h>

///////////////////////////

START_TEST(Gradient, "$Id: Gradient_test.C,v 1.1 2000/03/25 22:41:19 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Gradient::Gradient())
  //BAUSTELLE
RESULT


CHECK(Gradient::Gradient(const AtomVector& atoms))
  //BAUSTELLE
RESULT


CHECK(Gradient::Gradient(const Gradient& gradient, bool deep = true))
  //BAUSTELLE
RESULT


CHECK(Gradient::~Gradient())
  //BAUSTELLE
RESULT


CHECK(Gradient::Gradient& operator = (const Gradient& rhs))
  //BAUSTELLE
RESULT


CHECK(Gradient::Gradient& operator = (const AtomVector& rhs))
  //BAUSTELLE
RESULT


CHECK(Gradient::set(const Gradient& gradient))
  //BAUSTELLE
RESULT


CHECK(Gradient::set(const AtomVector& atoms))
  //BAUSTELLE
RESULT


CHECK(Gradient::negate())
  //BAUSTELLE
RESULT


CHECK(Gradient::normalize())
  //BAUSTELLE
RESULT


CHECK(Gradient::invalidate())
  //BAUSTELLE
RESULT


CHECK(Gradient::isValid() const )
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
