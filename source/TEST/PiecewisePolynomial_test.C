// $Id: PiecewisePolynomial_test.C,v 1.1 2000/08/31 12:28:11 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include "piecewisePolynomial.h"

///////////////////////////

START_TEST(class_name, "$Id: PiecewisePolynomial_test.C,v 1.1 2000/08/31 12:28:11 anker Exp $")

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

PiecewisePolynomial* poly;

CHECK(PiecewisePolynomial::PiecewisePolynomial())
	poly = new PiecewisePolynomial;
	TEST_NOT_EQUAL(poly, 0)
RESULT

CHECK(PiecewisePolynomial::~PiecewisePolynomial())
	delete poly;
RESULT

CHECK(PiecewisePolynomial::PiecewisePolynomial(const PiecewisePolynomial& function))
	poly->set(4, intervals, coefs);
	PiecewisePolynomial poly2(*poly);
	TEST_EQUAL(poly2.getDegree(), poly->getDegree())
	bool test = (poly2.getIntervals() == poly->getIntervals());
	TEST_EQUAL(test, true)
	test = (poly2.getCoefficients() == poly->getCoefficients());
	TEST_EQUAL(test, true)
	// BAUSTELLE: Spezialfälle
RESULT

CHECK(PiecewisePolynomial::PiecewisePolynomial(Size degree, const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs))
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_EQUAL(poly2.getDegree(), 4);
	bool test = (poly2.getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (poly2.getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (poly2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
	// BAUSTELLE: Spezialfälle
RESULT


CHECK(PiecewisePolynomial::clear())
	PiecewisePolynomial poly2(4, intervals, coefs);
	poly2.clear();
	std::vector<Interval> i2 = poly2.getIntervals();
	std::vector<Interval> i3;
	bool test = (i2 == i3);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> c2 = poly2.getCoefficients();
	std::vector<Coefficients> c3;
	test = (c2 == c3);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Spezialfälle
RESULT


CHECK(PiecewisePolynomial::destroy())
	PiecewisePolynomial poly2(4, intervals, coefs);
	poly2.clear();
	std::vector<Interval> i2 = poly2.getIntervals();
	std::vector<Interval> i3;
	bool test = (i2 == i3);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> c2 = poly2.getCoefficients();
	std::vector<Coefficients> c3;
	test = (c2 == c3);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Spezialfälle
RESULT


CHECK(PiecewisePolynomial::set(const PiecewisePolynomial& function))
	PiecewisePolynomial poly2;
	PiecewisePolynomial poly3(4, intervals, coefs);
	poly2.set(poly3);
	TEST_EQUAL(poly2.getDegree(), 4)
	bool test = (poly2.getIntervals() == poly3.getIntervals());
	TEST_EQUAL(test, true)
	test = (poly2.getCoefficients() == poly3.getCoefficients());
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (poly2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
	// BAUSTELLE: Spezialfälle
RESULT


CHECK(PiecewisePolynomial::set(Size degree, const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs))
	PiecewisePolynomial poly2;
	poly2.set(4, intervals, coefs);
	TEST_EQUAL(poly2.getDegree(), 4)
	bool test = (poly2.getIntervals() == intervals);
	TEST_EQUAL(test, true)
	test = (poly2.getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (poly2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
	// BAUSTELLE: Spezialfälle
RESULT


CHECK(PiecewisePolynomial::PiecewisePolynomial& operator = (const PiecewisePolynomial& function))
	PiecewisePolynomial poly2;
	PiecewisePolynomial poly3;
	poly2 = poly3;
	TEST_EQUAL(poly2.getDegree(), poly3.getDegree()) 
	bool test = (poly2.getIntervals() == poly3.getIntervals());
	TEST_EQUAL(test, true);
	test = (poly2.getCoefficients() == poly3.getCoefficients());
	TEST_EQUAL(test, true);
RESULT


CHECK(PiecewisePolynomial::setDegree(Size degree))
	PiecewisePolynomial poly2;
	TEST_EQUAL(poly2.getDegree(), 0)
	poly2.setDegree(3);
	TEST_EQUAL(poly2.getDegree(), 3)
RESULT


CHECK(PiecewisePolynomial::getDegree() const )
	PiecewisePolynomial poly2;
	TEST_EQUAL(poly2.getDegree(), 0)
	poly2.set(4, intervals, coefs);
	TEST_EQUAL(poly2.getDegree(), 4)
RESULT


CHECK(PiecewisePolynomial::setIntervals(const std::vector<Interval>& intervals))
	PiecewisePolynomial poly2;
	poly2.setIntervals(intervals);
	bool test = (poly2.getIntervals() == intervals);  
	TEST_EQUAL(test,true);
RESULT


CHECK(PiecewisePolynomial::getInterval(double x) const )
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_REAL_EQUAL(poly2.getInterval(0.5).first, 0.0)
	TEST_REAL_EQUAL(poly2.getInterval(0.5).second, 1.0)
	TEST_REAL_EQUAL(poly2.getInterval(1.0).first, 1.0)
	TEST_REAL_EQUAL(poly2.getInterval(1.0).second, 2.0)
RESULT


CHECK(PiecewisePolynomial::getInterval(Position index) const )
	PiecewisePolynomial poly2(4, intervals, coefs);
	Position index = 1;
	TEST_REAL_EQUAL(poly2.getInterval(index).first, 1.0)
	TEST_REAL_EQUAL(poly2.getInterval(index).second, 2.0)
RESULT


CHECK(PiecewisePolynomial::getIntervalIndex(double x) const )
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_EQUAL(poly2.getIntervalIndex(0.5), 0)
RESULT


CHECK(PiecewisePolynomial::getRange() const )
	PiecewisePolynomial poly2(4, intervals, coefs);
	Interval val = poly2.getRange();
	TEST_REAL_EQUAL(val.first, 0.0)
	TEST_REAL_EQUAL(val.second, 3.0)
RESULT


CHECK(PiecewisePolynomial::setCoefficients(const vector<Coefficients>& coefficients))
	PiecewisePolynomial poly2;
	poly2.setCoefficients(coefs);
	bool test = (poly2.getCoefficients() == coefs);
	TEST_EQUAL(test, true)
	// BAUSTELLE: false
RESULT


CHECK(PiecewisePolynomial::getCoefficients(double x) const )
	PiecewisePolynomial poly2;
	poly2.setCoefficients(coefs);
	bool test = (poly2.getCoefficients(0.5) == INVALID_COEFFICIENTS);
	TEST_EQUAL(test, true)
	poly2.setIntervals(intervals);
	test = (poly2.getCoefficients(0.5) == coefs[0]);
	// BAUSTELLE: false
RESULT


CHECK(PiecewisePolynomial::getCoefficients(Position index) const )
	PiecewisePolynomial poly2;
	poly2.setCoefficients(coefs);
	Position index = 0;
	bool test = (poly2.getCoefficients(index) == coefs[0]);
	TEST_EQUAL(test, true)
	// BAUSTELLE: false
RESULT


CHECK(PiecewisePolynomial::double operator () (double x) const )
  //BAUSTELLE
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_REAL_EQUAL(poly2(0.5), 0.0)
	TEST_REAL_EQUAL(poly2(1.0), 3.0)
	TEST_REAL_EQUAL(poly2(1.5), 3.5)
	TEST_REAL_EQUAL(poly2(2.5), 0.375)
	TEST_REAL_EQUAL(poly2(6.0), 0.0)
RESULT


CHECK(PiecewisePolynomial::isInRange(double x) const )
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_EQUAL(poly2.isInRange(2.5), true)
	TEST_EQUAL(poly2.isInRange(6.5), false)
RESULT


CHECK(PiecewisePolynomial::isValid() const )
	PiecewisePolynomial poly2;
	TEST_EQUAL(poly2.isValid(), false)
	poly2 = PiecewisePolynomial(4, intervals, coefs);
	TEST_EQUAL(poly2.isValid(), true);
RESULT




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
