// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MATHS/piecewisePolynomial.h>

///////////////////////////

START_TEST(PiecewisePolynomial)

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

CHECK(PiecewisePolynomial() throw())
	poly = new PiecewisePolynomial;
	TEST_NOT_EQUAL(poly, 0)
RESULT

CHECK(~PiecewisePolynomial() throw())
	delete poly;
RESULT

CHECK(PiecewisePolynomial(const PiecewisePolynomial& polynomial) throw())
	poly = new PiecewisePolynomial(4, intervals, coefs);
	PiecewisePolynomial poly2(*poly);
	TEST_EQUAL(poly2.getDegree(), poly->getDegree())
	bool test = (poly2.getIntervals() == poly->getIntervals());
	TEST_EQUAL(test, true)
	test = (poly2.getCoefficients() == poly->getCoefficients());
	TEST_EQUAL(test, true)
	delete poly;
RESULT

CHECK(PiecewisePolynomial(Size degree, const std::vector<Interval>& intervals, const std::vector<Coefficients>& coefficients) throw())
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_EQUAL(poly2.getDegree(), 4);
	bool test = (poly2.getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (poly2.getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (poly2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
	// ?????: Spezialfälle
RESULT


CHECK(void clear() throw())
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
	// ?????: Spezialfälle
RESULT


CHECK(void set(Size degree, const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs) throw())
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
	// ?????: Spezialfälle
RESULT


CHECK(PiecewisePolynomial& operator = (const PiecewisePolynomial& poly) throw())
	PiecewisePolynomial poly2;
	PiecewisePolynomial poly3;
	poly2 = poly3;
	TEST_EQUAL(poly2.getDegree(), poly3.getDegree()) 
	bool test = (poly2.getIntervals() == poly3.getIntervals());
	TEST_EQUAL(test, true);
	test = (poly2.getCoefficients() == poly3.getCoefficients());
	TEST_EQUAL(test, true);
RESULT


CHECK(void setDegree(Size degree) throw())
	PiecewisePolynomial poly2;
	TEST_EQUAL(poly2.getDegree(), 0)
	poly2.setDegree(3);
	TEST_EQUAL(poly2.getDegree(), 3)
RESULT


CHECK(Size getDegree() const throw())
	PiecewisePolynomial poly2;
	TEST_EQUAL(poly2.getDegree(), 0)
	poly2.set(4, intervals, coefs);
	TEST_EQUAL(poly2.getDegree(), 4)
RESULT


CHECK(setIntervals(const std::vector<Interval>& intervals))
	PiecewisePolynomial poly2;
	poly2.setIntervals(intervals);
	bool test = (poly2.getIntervals() == intervals);  
	TEST_EQUAL(test,true);
RESULT


CHECK(getInterval(double x) const throw())
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_REAL_EQUAL(poly2.getInterval(0.5).first, 0.0)
	TEST_REAL_EQUAL(poly2.getInterval(0.5).second, 1.0)
	TEST_REAL_EQUAL(poly2.getInterval(1.0).first, 1.0)
	TEST_REAL_EQUAL(poly2.getInterval(1.0).second, 2.0)
RESULT


CHECK(getInterval(Position index) const throw())
	PiecewisePolynomial poly2(4, intervals, coefs);
	Position index = 1;
	TEST_REAL_EQUAL(poly2.getInterval(index).first, 1.0)
	TEST_REAL_EQUAL(poly2.getInterval(index).second, 2.0)
RESULT


CHECK(getIntervalIndex(double x) const throw())
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_EQUAL(poly2.getIntervalIndex(0.5), 0)
RESULT


CHECK(getRange() const throw())
	PiecewisePolynomial poly2(4, intervals, coefs);
	Interval val = poly2.getRange();
	TEST_REAL_EQUAL(val.first, 0.0)
	TEST_REAL_EQUAL(val.second, 3.0)
RESULT


CHECK(setCoefficients(const vector<Coefficients>& coefficients))
	PiecewisePolynomial poly2;
	poly2.setCoefficients(coefs);
	bool test = (poly2.getCoefficients() == coefs);
	TEST_EQUAL(test, true)
	// ?????: false
RESULT


CHECK(getCoefficients(double x) const throw())
	PiecewisePolynomial poly2;
	poly2.setCoefficients(coefs);
	TEST_EXCEPTION(Exception::OutOfRange, poly2.getCoefficients(0.5))
	poly2.setIntervals(intervals);
	bool test = (poly2.getCoefficients(0.5) == coefs[0]);
	TEST_EQUAL(test, true);
	// ?????: false
RESULT


CHECK(getCoefficients(Position index) const throw())
	PiecewisePolynomial poly2;
	poly2.setCoefficients(coefs);
	Position index = 0;
	bool test = (poly2.getCoefficients(index) == coefs[0]);
	TEST_EQUAL(test, true)
	// ?????: false
RESULT


CHECK(double operator () (double x) const throw())
  //?????
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_REAL_EQUAL(poly2(0.5), 0.0)
	TEST_REAL_EQUAL(poly2(1.0), 3.0)
	TEST_REAL_EQUAL(poly2(1.5), 3.5)
	TEST_REAL_EQUAL(poly2(2.5), 0.375)
	TEST_REAL_EQUAL(poly2(6.0), 0.0)
RESULT


CHECK(isInRange(double x) const throw())
	PiecewisePolynomial poly2(4, intervals, coefs);
	TEST_EQUAL(poly2.isInRange(2.5), true)
	TEST_EQUAL(poly2.isInRange(6.5), false)
RESULT


CHECK(isValid() const throw())
	PiecewisePolynomial poly2;
	TEST_EQUAL(poly2.isValid(), false)
	poly2 = PiecewisePolynomial(4, intervals, coefs);
	TEST_EQUAL(poly2.isValid(), true);
RESULT

CHECK(BALL_CREATE(PiecewisePolynomial))
  // ???
RESULT

CHECK(bool operator == (const PiecewisePolynomial& poly) const throw())
  // ???
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
