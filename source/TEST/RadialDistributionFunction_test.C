// $Id: RadialDistributionFunction_test.C,v 1.1 2000/08/31 13:04:21 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/STRUCTURE/radialDistributionFunction.h>

///////////////////////////

START_TEST(class_name, "$Id: RadialDistributionFunction_test.C,v 1.1 2000/08/31 13:04:21 anker Exp $")

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

intervals.push_back(Interval(0.0, 1.0));
intervals.push_back(Interval(1.0, 2.0));
intervals.push_back(Interval(2.0, 3.0));

PiecewisePolynomial poly(4, intervals, coefs);

RadialDistributionFunction* RDFp;

CHECK(RadialDistributionFunction::RadialDistributionFunction())
	RDFp = new RadialDistributionFunction();
	TEST_NOT_EQUAL(RDFp, 0)
RESULT


CHECK(RadialDistributionFunction::~RadialDistributionFunction())
  delete RDFp;
RESULT


CHECK(RadialDistributionFunction::RadialDistributionFunction(const RadialDistributionFunction& rdf))
	RadialDistributionFunction RDF2(poly);
	RadialDistributionFunction RDF(RDF2);
	TEST_EQUAL(RDF.getRepresentation().getDegree(),
			RDF2.getRepresentation().getDegree())
	bool test = (RDF.getRepresentation().getIntervals() ==
			RDF2.getRepresentation().getIntervals());
	TEST_EQUAL(test, true)
	test = (RDF.getRepresentation().getCoefficients() ==
			RDF2.getRepresentation().getCoefficients());
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::RadialDistributionFunction(const PiecewisePolynomial& polynomial))
	RadialDistributionFunction RDF(poly);
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 4)
	bool test = (RDF.getRepresentation().getIntervals() == poly.getIntervals());
	TEST_EQUAL(test, true)
	test = (RDF.getRepresentation().getCoefficients() == 
			poly.getCoefficients());
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::clear())
	RadialDistributionFunction RDF(poly);
	RDF.clear();
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 0)
	std::vector<Interval> nope;
	bool test = (RDF.getRepresentation().getIntervals() == nope);
	TEST_EQUAL(test, true)
	std::vector<Coefficients> naught;
	test = (RDF.getRepresentation().getCoefficients() == naught);
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::destroy())
	RadialDistributionFunction RDF(poly);
	RDF.destroy();
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 0)
	std::vector<Interval> nope;
	bool test = (RDF.getRepresentation().getIntervals() == nope);
	TEST_EQUAL(test, true)
	std::vector<Coefficients> naught;
	test = (RDF.getRepresentation().getCoefficients() == naught);
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::set(const RadialDistributionFunction& rdf))
	RadialDistributionFunction RDF;
	RadialDistributionFunction RDF2(poly);
	RDF.set(RDF2);
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 4)
	bool test = (RDF.getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true)
	std::vector<Coefficients> naught;
	test = (RDF.getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::RadialDistributionFunction& operator = 
			(const RadialDistributionFunction& rdf))
	RadialDistributionFunction RDF;
	RadialDistributionFunction RDF2(poly);
	RDF = RDF2;
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 4)
	bool test = (RDF.getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true)
	std::vector<Coefficients> naught;
	test = (RDF.getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::setRepresentation(const PiecewisePolynomial& polynomial))
	RadialDistributionFunction RDF;
	RDF.setRepresentation(poly);
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 4)
	bool test = (RDF.getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true)
	std::vector<Coefficients> naught;
	test = (RDF.getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::getRepresentation() const )
	RadialDistributionFunction RDF;
	RDF.setRepresentation(poly);
	TEST_EQUAL(RDF.getRepresentation().getDegree(), 4)
	bool test = (RDF.getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true)
	std::vector<Coefficients> naught;
	test = (RDF.getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::getRange() const )
	RadialDistributionFunction RDF(poly);
	bool test = (RDF.getRange() == poly.getRange());
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::double operator () (double x) const )
	RadialDistributionFunction RDF(poly);
	TEST_REAL_EQUAL(RDF(0.5), 0.0)
	TEST_REAL_EQUAL(RDF(1.0), 3.0)
	TEST_REAL_EQUAL(RDF(1.5), 3.5)
	TEST_REAL_EQUAL(RDF(2.5), 0.375)
	TEST_REAL_EQUAL(RDF(6.0), 0.0)
RESULT


CHECK(RadialDistributionFunction::isInRange(double x) const )
	RadialDistributionFunction RDF(poly);
	bool test = (RDF.isInRange(2) == poly.isInRange(2));
	test = (RDF.isInRange(7) == poly.isInRange(7));
	TEST_EQUAL(test, true)
RESULT


CHECK(RadialDistributionFunction::isValid())
	RadialDistributionFunction RDF(poly);
	bool test = (RDF.isValid() == poly.isValid());
	TEST_EQUAL(test, true)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
