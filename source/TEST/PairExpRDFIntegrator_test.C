// $Id: PairExpRDFIntegrator_test.C,v 1.1 2000/08/31 18:12:28 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/pairExpRDFIntegrator.h>

///////////////////////////

START_TEST(class_name, "$Id: PairExpRDFIntegrator_test.C,v 1.1 2000/08/31 18:12:28 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class PairExpRDFIntegrator::

CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator())
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator(const PairExpRDFIntegrator& integrator))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator(double alpha, double C1, double C2, double R_ij_o, double k1, double k2, const RadialDistributionFunction& rdf))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::~PairExpRDFIntegrator())
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::destroy())
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::clear())
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::set(const PairExpRDFIntegrator& integrator))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator& operator = 
			(const PairExpRDFIntegrator& integrator))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::setConstants(double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrateToInf(double from) const )
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrateToInf(double from, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrate(double from, double to) const )
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrate(double from, double to, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::double operator () (double x) const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
