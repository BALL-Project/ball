// $Id: RDFIntegrator_test.C,v 1.1 2000/08/31 13:59:02 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/RDFIntegrator.h>

///////////////////////////

START_TEST(class_name, "$Id: RDFIntegrator_test.C,v 1.1 2000/08/31 13:59:02 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	

CHECK(RDFIntegrator::RDFIntegrator())
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::RDFIntegrator(const RDFIntegrator& integrator))
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::~RDFIntegrator())
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::destroy())
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::clear())
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::set(const RDFIntegrator& integrator))
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::RDFIntegrator& operator = (const RDFIntegrator& integrator))
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::double operator () (double x) const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
