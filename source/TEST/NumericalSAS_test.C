// $Id: NumericalSAS_test.C,v 1.2 2000/05/23 10:23:46 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/STRUCTURE/numericalSAS.h>
#include <BALL/KERNEL/fragment.h>
///////////////////////////

START_TEST(NumericalSAS, "$Id: NumericalSAS_test.C,v 1.2 2000/05/23 10:23:46 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(calculateNumericalSASArea(const	Composite&, float, int))
Fragment	f;
Atom a1, a2;
a1.setRadius(1.0);
a2.setRadius(1.0);
a2.setPosition(Vector3(10.0, 0.0, 0.0));

f.insert(a1);
f.insert(a2);

float area = calculateNumericalSASArea(f, 1.5, 624);

PRECISION(0.001)
TEST_REAL_EQUAL(area, 157.07963)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
