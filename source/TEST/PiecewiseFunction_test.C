// $Id: PiecewiseFunction_test.C,v 1.1 2000/08/31 12:18:20 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include "piecewiseFunction.h"

///////////////////////////

START_TEST(class_name, "$Id: PiecewiseFunction_test.C,v 1.1 2000/08/31 12:18:20 anker Exp $")

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

coef.push_back(1.0); coef.push_back(1.0);
coef.push_back(0.0); coef.push_back(0.0);
coefs.push_back(coef);
coef.clear();

coef.push_back(0.0); coef.push_back(0.0);
coef.push_back(1.0); coef.push_back(0.0);
coefs.push_back(coef);
coef.clear();

intervals.push_back(Interval(0.0, 1.0));
intervals.push_back(Interval(1.0, 2.0));
intervals.push_back(Interval(2.0, 3.0));

PiecewiseFunction* PWF;

CHECK(PiecewiseFunction::PiecewiseFunction())
	PWF = new PiecewiseFunction;
	TEST_NOT_EQUAL(PWF, 0)
RESULT

CHECK(PiecewiseFunction::~PiecewiseFunction())
	delete PWF;
RESULT

CHECK(PiecewiseFunction::PiecewiseFunction(const PiecewiseFunction& function))
	// BAUSTELLE
	PWF->setIntervals(intervals);
	PiecewiseFunction PWF2(*PWF);
	TEST_EQUAL(PWF2.getIntervals().size(), PWF->getIntervals().size())
RESULT

CHECK(PiecewiseFunction::PiecewiseFunction(const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs))
	PiecewiseFunction PWF2(intervals, coefs);
	// Unter der Annahme, daß operator== für std::vector definiert ist und
	// funktioniert.
	bool test = (PWF2.getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (PWF2.getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (PWF2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
RESULT


CHECK(PiecewiseFunction::clear())
	PiecewiseFunction PWF2(intervals, coefs);
	PWF2.clear();
	std::vector<Interval> i2 = PWF2.getIntervals();
	std::vector<Interval> i3;
	bool test = (i2 == i3);
	TEST_EQUAL(test, true);
RESULT


CHECK(PiecewiseFunction::destroy())
	PiecewiseFunction PWF2(intervals, coefs);
	PWF2.destroy();
	std::vector<Interval> i2 = PWF2.getIntervals();
	std::vector<Interval> i3;
	bool test = (i2 == i3);
	TEST_EQUAL(test, true);
RESULT


CHECK(PiecewiseFunction::set(const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs))
	PiecewiseFunction PWF2;
	PWF2.set(intervals, coefs);
	bool test = (PWF2.getIntervals() == intervals);
	TEST_EQUAL(test, true)
	test = (PWF2.getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (PWF2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
RESULT

CHECK(PiecewiseFunction::set(const PiecewiseFunction& function))
	PiecewiseFunction PWF2;
	PiecewiseFunction PWF3(intervals, coefs);
	PWF2.set(PWF3);
	bool test = (PWF2.getIntervals() == PWF3.getIntervals());
	TEST_EQUAL(test, true)
	test = (PWF2.getCoefficients() == PWF3.getCoefficients());
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (PWF2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
RESULT


CHECK(PiecewiseFunction::PiecewiseFunction& operator = (const PiecewiseFunction& function))
	PiecewiseFunction PWF2;
	PiecewiseFunction PWF3;
	PWF2 = PWF3;
	bool test = (PWF2.getIntervals() == PWF3.getIntervals());
	TEST_EQUAL(test, true);
	test = (PWF2.getCoefficients() == PWF3.getCoefficients());
	TEST_EQUAL(test, true);
RESULT


CHECK(PiecewiseFunction::setIntervals(const std::vector<Interval>& intervals))
	PiecewiseFunction PWF2;
	PWF2.setIntervals(intervals);
	bool test = (PWF2.getIntervals() == intervals);  
	TEST_EQUAL(test,true);
RESULT


CHECK(PiecewiseFunction::getInterval(double x) const )
	PiecewiseFunction PWF2(intervals, coefs);
	TEST_REAL_EQUAL(PWF2.getInterval(0.5).first, 0.0)
	TEST_REAL_EQUAL(PWF2.getInterval(0.5).second, 1.0)
	TEST_REAL_EQUAL(PWF2.getInterval(1.0).first, 1.0)
	TEST_REAL_EQUAL(PWF2.getInterval(1.0).second, 2.0)
RESULT


CHECK(PiecewiseFunction::getInterval(Position index) const )
	PiecewiseFunction PWF2(intervals, coefs);
	Position index = 1;
	TEST_REAL_EQUAL(PWF2.getInterval(index).first, 1.0)
	TEST_REAL_EQUAL(PWF2.getInterval(index).second, 2.0)
RESULT


CHECK(PiecewiseFunction::getIntervalIndex(double x) const )
	PiecewiseFunction PWF2(intervals, coefs);
	TEST_EQUAL(PWF2.getIntervalIndex(0.5), 0)
RESULT


CHECK(PiecewiseFunction::getRange() const )
	PiecewiseFunction PWF2(intervals, coefs);
	Interval val = PWF2.getRange();
	TEST_REAL_EQUAL(val.first, 0.0)
	TEST_REAL_EQUAL(val.second, 3.0)
RESULT


CHECK(PiecewiseFunction::setCoefficients(const vector<Coefficients>& coefficients))
	PiecewiseFunction PWF2;
	PWF2.setCoefficients(coefs);
	bool test = (PWF2.getCoefficients() == coefs);
	TEST_EQUAL(test, true)
	// BAUSTELLE: false
RESULT


CHECK(PiecewiseFunction::getCoefficients(double x) const )
	PiecewiseFunction PWF2;
	PWF2.setCoefficients(coefs);
	bool test = (PWF2.getCoefficients(0.5) == INVALID_COEFFICIENTS);
	TEST_EQUAL(test, true)
	PWF2.setIntervals(intervals);
	test = (PWF2.getCoefficients(0.5) == coefs[0]);
	// BAUSTELLE: false
RESULT


CHECK(PiecewiseFunction::getCoefficients(Position index) const )
	PiecewiseFunction PWF2;
	PWF2.setCoefficients(coefs);
	Position index = 0;
	bool test = (PWF2.getCoefficients(index) == coefs[0]);
	TEST_EQUAL(test, true)
	// BAUSTELLE: false
RESULT


CHECK(PiecewiseFunction::double operator () (double x) const )
  //BAUSTELLE: Quasi abstrakt.
RESULT


CHECK(PiecewiseFunction::isInRange(double x) const )
	PiecewiseFunction PWF2(intervals, coefs);
	TEST_EQUAL(PWF2.isInRange(2.5), true)
	TEST_EQUAL(PWF2.isInRange(6.5), false)
RESULT


CHECK(PiecewiseFunction::isValid() const )
	PiecewiseFunction PWF2;
	TEST_EQUAL(PWF2.isValid(), false)
	PWF2 = PiecewiseFunction(intervals, coefs);
	TEST_EQUAL(PWF2.isValid(), true);
RESULT




/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
