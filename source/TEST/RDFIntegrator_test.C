// $Id: RDFIntegrator_test.C,v 1.2 2000/09/25 16:34:01 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/RDFIntegrator.h>

///////////////////////////

START_TEST(class_name, "$Id: RDFIntegrator_test.C,v 1.2 2000/09/25 16:34:01 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
Coefficients coef;
vector<Coefficients> coefs;
vector<Interval> intervals;

coef.push_back(0.0); coef.push_back(0.0);
coef.push_back(0.0); coef.push_back(0.0);
coefs.push_back(coef);
coef.clear();

coef.push_back(3.0); coef.push_back(1.0);
coef.push_back(0.0); coef.push_back(0.0);
coefs.push_back(coef);
coef.clear();

coef.push_back(0.0); coef.push_back(0.0);
coef.push_back(0.0); coef.push_back(3.0);
coefs.push_back(coef);
coef.clear();

coef.push_back(2.0); coef.push_back(0.0);
coef.push_back(0.0); coef.push_back(0.0);
coefs.push_back(coef);
coef.clear();

intervals.push_back(Interval(0.0, 1.0));
intervals.push_back(Interval(1.0, 2.0));
intervals.push_back(Interval(2.0, 3.0));
intervals.push_back(Interval(3.0, INFINITY));

PiecewisePolynomial poly(4, intervals, coefs);
RadialDistributionFunction rdf(poly);

RDFIntegrator* pointer;

CHECK(RDFIntegrator::RDFIntegrator())
  pointer = new RDFIntegrator;
	TEST_NOT_EQUAL(pointer, 0)
RESULT


CHECK(RDFIntegrator::~RDFIntegrator())
	delete pointer;
RESULT


CHECK(RDFIntegrator::RDFIntegrator(const RadialDistributionFunction& rdf))
  RDFIntegrator integrator(rdf);
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (integrator.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
RESULT


CHECK(RDFIntegrator::RDFIntegrator(const RDFIntegrator& integrator))
  RDFIntegrator integrator(rdf);
  RDFIntegrator integrator2(integrator);
	bool test = (integrator2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (integrator2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
RESULT


CHECK(RDFIntegrator::destroy())
  RDFIntegrator integrator(rdf);
	integrator.destroy();
	vector<Interval> no_intervals;
	vector<Coefficients> no_coeffs;
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == no_intervals);
	TEST_EQUAL(test, true);
	test = (integrator.getRDF().getRepresentation().getCoefficients() == no_coeffs);
	TEST_EQUAL(test, true);
	TEST_EQUAL(integrator.getRDF().isValid(), false);
RESULT


CHECK(RDFIntegrator::clear())
  RDFIntegrator integrator(rdf);
	integrator.clear();
	vector<Interval> no_intervals;
	vector<Coefficients> no_coeffs;
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == no_intervals);
	TEST_EQUAL(test, true);
	test = (integrator.getRDF().getRepresentation().getCoefficients() == no_coeffs);
	TEST_EQUAL(test, true);
	TEST_EQUAL(integrator.getRDF().isValid(), false);
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


CHECK(RDFIntegrator::setRDF(const RadialDistributionFunction& rdf))
  //BAUSTELLE
RESULT


CHECK(RDFIntegrator::RadialDistributionFunction getRDF())
	RDFIntegrator integrator(rdf);
	RadialDistributionFunction rdf2 = integrator.getRDF();
	bool test = (rdf2.getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (rdf2.getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
