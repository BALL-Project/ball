// $Id: NumericalIntegrator_test.C,v 1.1 2000/12/19 14:05:43 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/MATHS/numericalIntegrator.h>

///////////////////////////

START_TEST(class_name, "$Id: NumericalIntegrator_test.C,v 1.1 2000/12/19 14:05:43 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

// tests for class NumericalIntegrator::

CHECK(GROSSBAUSTELLE!)
	TEST_EQUAL(0,1)
RESULT

CHECK(NumericalIntegrator::NumericalIntegrator() throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::NumericalIntegrator(const NumericalIntegrator& nint) throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::~NumericalIntegrator() throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::NumericalIntegrator& operator = (const NumericalIntegrator& nint) throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::clear() throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::bool operator == (const NumericalIntegrator& nint) const  throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::setFunction(const Function& function) throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::getFunction() const  throw())
  //BAUSTELLE
RESULT


CHECK(NumericalIntegrator::getValue(DataType x))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
