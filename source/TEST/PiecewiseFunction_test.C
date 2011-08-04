// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/MATHS/piecewiseFunction.h>

///////////////////////////

START_TEST(PiecewiseFunction)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

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

PiecewiseFunction* pointer;

CHECK(PiecewiseFunction() throw())
	pointer = new PiecewiseFunction;
	TEST_NOT_EQUAL(pointer, 0)
RESULT

CHECK(~PiecewiseFunction() throw())
	delete pointer;
RESULT

CHECK(PiecewiseFunction(const PiecewiseFunction& function) throw())
	PiecewiseFunction PWF;
	PWF.setIntervals(intervals);
	PiecewiseFunction PWF2(PWF);
	TEST_EQUAL(PWF2.getIntervals().size(), PWF.getIntervals().size())
RESULT

CHECK(PiecewiseFunction(const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs) throw())
	PiecewiseFunction PWF2(intervals, coefs);
	// Unter der Annahme, daÿ operator== für std::vector definiert ist und
	// funktioniert.
	bool test = (PWF2.getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (PWF2.getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	std::vector<Coefficients> naught;
	test = (PWF2.getCoefficients() == naught);
	TEST_EQUAL(test, false)
RESULT


CHECK(void clear() throw())
	PiecewiseFunction PWF2(intervals, coefs);
	PWF2.clear();
	std::vector<Interval> i2 = PWF2.getIntervals();
	std::vector<Interval> i3;
	bool test = (i2 == i3);
	TEST_EQUAL(test, true);
RESULT


CHECK(void set(const std::vector<Interval>& intervals, const std::vector<Coefficients>& coeffs) throw())
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


CHECK(PiecewiseFunction& operator = (const PiecewiseFunction& function) throw())
	PiecewiseFunction PWF2;
	PiecewiseFunction PWF3;
	PWF2 = PWF3;
	bool test = (PWF2.getIntervals() == PWF3.getIntervals());
	TEST_EQUAL(test, true);
	test = (PWF2.getCoefficients() == PWF3.getCoefficients());
	TEST_EQUAL(test, true);
RESULT


CHECK(void setIntervals(const std::vector<Interval>& intervals) throw())
	PiecewiseFunction PWF2;
	PWF2.setIntervals(intervals);
	bool test = (PWF2.getIntervals() == intervals);  
	TEST_EQUAL(test,true);
RESULT


CHECK(const Interval& getInterval(double x) const throw(Exception::OutOfRange))
	PiecewiseFunction PWF2(intervals, coefs);
	TEST_REAL_EQUAL(PWF2.getInterval(0.5).first, 0.0)
	TEST_REAL_EQUAL(PWF2.getInterval(0.5).second, 1.0)
	TEST_REAL_EQUAL(PWF2.getInterval(1.0).first, 1.0)
	TEST_REAL_EQUAL(PWF2.getInterval(1.0).second, 2.0)
RESULT


CHECK(const Interval& getInterval(Position index) const throw(Exception::IndexOverflow))
	PiecewiseFunction PWF2(intervals, coefs);
	Position index = 1;
	TEST_REAL_EQUAL(PWF2.getInterval(index).first, 1.0)
	TEST_REAL_EQUAL(PWF2.getInterval(index).second, 2.0)
RESULT


CHECK(Position getIntervalIndex(double x) const throw(Exception::OutOfRange))
	PiecewiseFunction PWF2(intervals, coefs);
	TEST_EQUAL(PWF2.getIntervalIndex(0.5), 0)
RESULT


CHECK(const Interval& getRange() const throw())
	PiecewiseFunction PWF2(intervals, coefs);
	Interval val = PWF2.getRange();
	TEST_REAL_EQUAL(val.first, 0.0)
	TEST_REAL_EQUAL(val.second, 3.0)
RESULT


CHECK(void setCoefficients(const vector<Coefficients>& coefficients) throw())
	PiecewiseFunction PWF2;
	PWF2.setCoefficients(coefs);
	bool test = (PWF2.getCoefficients() == coefs);
	TEST_EQUAL(test, true)
RESULT


CHECK(const Coefficients& getCoefficients(double x) const throw(Exception::OutOfRange))
	PiecewiseFunction PWF2;
	PWF2.setCoefficients(coefs);
	Coefficients cf;
	// bool test = (PWF2.getCoefficients(0.5) == cf);
	TEST_EXCEPTION(Exception::OutOfRange, PWF2.getCoefficients(0.5))
	PWF2.setIntervals(intervals);
	bool test = (PWF2.getCoefficients(0.5) == coefs[0]);
	TEST_EQUAL(test, true)
RESULT


CHECK(const Coefficients& getCoefficients(Position index) const throw(Exception::IndexOverflow))
	PiecewiseFunction PWF2;
	PWF2.setCoefficients(coefs);
	Position index = 0;
	bool test = (PWF2.getCoefficients(index) == coefs[0]);
	TEST_EQUAL(test, true)
RESULT


CHECK(double operator () (double x) const throw())
  //?????: Quasi abstrakt.
RESULT


CHECK(bool isInRange(double x) const throw())
	PiecewiseFunction PWF2(intervals, coefs);
	TEST_EQUAL(PWF2.isInRange(2.5), true)
	TEST_EQUAL(PWF2.isInRange(6.5), false)
RESULT


CHECK(bool isValid() const throw())
	PiecewiseFunction PWF2;
	TEST_EQUAL(PWF2.isValid(), false)
	PWF2 = PiecewiseFunction(intervals, coefs);
	TEST_EQUAL(PWF2.isValid(), true);
RESULT

CHECK(BALL_CREATE(PiecewiseFunction))
  // ???
RESULT

CHECK(bool operator == (const PiecewiseFunction& function) const throw())
	PiecewiseFunction PWF2;
	PiecewiseFunction PWF3;
	bool test = (PWF2 == PWF3);
	TEST_EQUAL(test, true)
	PWF2.setCoefficients(coefs);
	test = (PWF2 == PWF3);
	TEST_NOT_EQUAL(test, true)
	PWF3.setCoefficients(coefs);
	test = (PWF2 == PWF3);
	TEST_EQUAL(test, true)
	PWF2.setIntervals(intervals);
	test = (PWF2 == PWF3);
	TEST_NOT_EQUAL(test, true)
	PWF3.setIntervals(intervals);
	test = (PWF2 == PWF3);
	TEST_EQUAL(test, true)
RESULT

CHECK(const std::vector<Coefficients>& getCoefficients() const throw())
	PiecewiseFunction PWF2;
	TEST_EQUAL(PWF2.getCoefficients().size(), 0)
	PWF2.setCoefficients(coefs);
	TEST_EQUAL(PWF2.getCoefficients().size(), 3)
RESULT

CHECK(const std::vector<Interval>& getIntervals() const throw())
	PiecewiseFunction PWF2;
	TEST_EQUAL(PWF2.getIntervals().size(), 0)
	PWF2.setIntervals(intervals);
	TEST_EQUAL(PWF2.getIntervals().size(), 3)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	PiecewiseFunction PWF2(intervals, coefs);
	String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	PWF2.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(PiecewiseFunction_test.txt))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
