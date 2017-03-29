// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>

///////////////////////////

START_TEST(LineSearch)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(LineSearch::takeStep(double &st_a, double &f_a, double &g_a, double &st_b, 
			double &f_b, double &g_b, double &stp, double f, double g, double minstp, double maxstp))
	LineSearch ls;
	
	// Check different cases by f(x) = -x^3 + 2x^2 - x - 1
	// This function has a minimum at 1/3 and a maximum at 1 and tends to -infinity for x->infinity
	
	// We also check every time if the interval bounds are set correctly.
	
	// Slight correction test: bracketed flag not set, but we set stp to almost the minimizer
	ls.setBracketedFlag(false);
	double st_a = 0.;
	double f_a = -1.;
	double g_a = -1.;
	double st_b = 1.;
	double f_b = 1.;
	double g_b = 0.; 
	double stp = 0.33333;
	double f = -1.148148147;
	double g = -.6667e-4;
	double minstp = 0.;
	double maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 0.33333)
	TEST_REAL_EQUAL(f_a, -1.148148147)
	TEST_REAL_EQUAL(g_a, -6.667e-05)
	TEST_REAL_EQUAL(st_b, 1.0)
	TEST_REAL_EQUAL(f_b, 1.0)
	TEST_REAL_EQUAL(g_b, 0.0)
	TEST_REAL_EQUAL(stp, 0.333363348343)
	TEST_EQUAL(ls.isBracketed(), false)
	
	// First case: f > f_a
	// Result: correct cubic step
	ls.setBracketedFlag(false);
	st_a = 0.1;
	f_a = -1.081;
	g_a = -0.63;
	st_b = 1.;
	f_b = -1.;
	g_b = 0.; 
	stp = 0.9;
	f = -1.009;
	g = 0.17;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 0.1)
	TEST_REAL_EQUAL(f_a, -1.081)
	TEST_REAL_EQUAL(g_a, -0.63)
	TEST_REAL_EQUAL(st_b, 0.9)
	TEST_REAL_EQUAL(f_b, -1.009)
	TEST_REAL_EQUAL(g_b, 0.17)
	TEST_REAL_EQUAL(stp, 0.333333333333)
	TEST_EQUAL(ls.isBracketed(), true)
	
	// Second case: f <= f_a but sgn(g) !=  sgn(g_a)
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 1.;
	f_b = -1.;
	g_b = 0.; 
	stp = 0.9;
	f = -1.009;
	g = 0.17;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 0.9)
	TEST_REAL_EQUAL(f_a, -1.009)
	TEST_REAL_EQUAL(g_a, 0.17)
	TEST_REAL_EQUAL(st_b, 0.0)
	TEST_REAL_EQUAL(f_b, -1.0)
	TEST_REAL_EQUAL(g_b, -1.0)
	TEST_REAL_EQUAL(stp, 0.333333333333)
	TEST_EQUAL(ls.isBracketed(), true)

	// Third case: |g| < |g_a| and f <= f_a and sgn(g) == sgn(g_a) 
	// AND the cubic (interpolation) tends to -infinity but its minimum is beyond stp
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 1.;
	f_b = -1.;
	g_b = 0.; 
	stp = 0.2;
	f = -1.128;
	g = -0.32;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 0.2)
	TEST_REAL_EQUAL(f_a, -1.128)
	TEST_REAL_EQUAL(g_a, -0.32)
	TEST_REAL_EQUAL(st_b, 1.0)
	TEST_REAL_EQUAL(f_b, -1.0)
	TEST_REAL_EQUAL(g_b, 0.0)
	TEST_REAL_EQUAL(stp, 0.333333333333)
	TEST_EQUAL(ls.isBracketed(), false)

	// Third case: |g| < |g_a| and f <= f_a and sgn(g) == sgn(g_a) 
	// AND the cubic (interpolation) tends to -infinity and its minimum is on this side of stp
	// (so stp has to be maxstp)
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 1.9;
	f_b = -2.539;
	g_b = -4.23; 
	stp = 1.1;
	f = -1.011;
	g = -0.23;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 1.1)
	TEST_REAL_EQUAL(f_a, -1.011)
	TEST_REAL_EQUAL(g_a, -0.23)
	TEST_REAL_EQUAL(st_b, 1.9)
	TEST_REAL_EQUAL(f_b, -2.539)
	TEST_REAL_EQUAL(g_b, -4.23)
	TEST_REAL_EQUAL(stp, 2.0)
	TEST_EQUAL(ls.isBracketed(), false)

	// Same case as above but now we force the step computation to
	// assume that a minimizer has already been bracketed
	ls.setBracketedFlag(true);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 1.9;
	f_b = -2.539;
	g_b = -4.23; 
	stp = 1.1;
	f = -1.011;
	g = -0.23;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 1.1)
	TEST_REAL_EQUAL(f_a, -1.011)
	TEST_REAL_EQUAL(g_a, -0.23)
	TEST_REAL_EQUAL(st_b, 1.9)
	TEST_REAL_EQUAL(f_b, -2.539)
	TEST_REAL_EQUAL(g_b, -4.23)
	TEST_REAL_EQUAL(stp, 1.42857142857)
	TEST_EQUAL(ls.isBracketed(), true)

	// Fourth case: f <= f_a, sgn(g) == sgn(g_a), |g| >= |g_a|
	// AND we force the step computation to assume that a minimizer could not have been bracketed so far
	// (so stp has to be maxstp in this case)
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 1.9;
	f_b = -2.539;
	g_b = -4.23; 
	stp = 1.5;
	f = -1.375;
	g = -1.75;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 1.5)
	TEST_REAL_EQUAL(f_a, -1.375)
	TEST_REAL_EQUAL(g_a, -1.75)
	TEST_REAL_EQUAL(st_b, 1.9)
	TEST_REAL_EQUAL(f_b, -2.539)
	TEST_REAL_EQUAL(g_b, -4.23)
	TEST_REAL_EQUAL(stp, 2.0)
	TEST_EQUAL(ls.isBracketed(), false)

	// Same case as above but now we force the step computation to
	// assume that a minimizer has already been bracketed (so a cubic should be taken).
	ls.setBracketedFlag(true);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 1.9;
	f_b = -2.539;
	g_b = -4.23; 
	stp = 1.5;
	f = -1.375;
	g = -1.75;
	minstp = 0.;
	maxstp = 2.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 1.5)
	TEST_REAL_EQUAL(f_a, -1.375)
	TEST_REAL_EQUAL(g_a, -1.75)
	TEST_REAL_EQUAL(st_b, 1.9)
	TEST_REAL_EQUAL(f_b, -2.539)
	TEST_REAL_EQUAL(g_b, -4.23)
	TEST_REAL_EQUAL(stp, 0.333333333333)
	TEST_EQUAL(ls.isBracketed(), true)


	// Now chek a few things we couldn't check so far.
	// We use the function f(x) = x^3 - 2x^2 - x - 1.
	// This function has a minimizer at 2/3 + sqrt(7)/3 (about 1.54858)
	// and tends to infinity for x->infinity

	// Third case, but the cubic (interpolation) tends to infinity
	// We force the step computation to assume that a minimizer could not have been bracketed
	// so far. In this case the quadratic step should be taken.
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 2.;
	f_b = -3.;
	g_b = 3.; 
	stp = 1.4;
	f = -3.576;
	g = -0.72;
	minstp = 0.;
	maxstp = 10.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 1.4)
	TEST_REAL_EQUAL(f_a, -3.576)
	TEST_REAL_EQUAL(g_a, -0.72)
	TEST_REAL_EQUAL(st_b, 2.0)
	TEST_REAL_EQUAL(f_b, -3.0)
	TEST_REAL_EQUAL(g_b, 3.0)
	TEST_REAL_EQUAL(stp, 5.0)
	TEST_EQUAL(ls.isBracketed(), false)

	// Same case as above but we force the step computation to assume that a 
	// minimizer has already been bracketed.
	ls.setBracketedFlag(true);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 2.;
	f_b = -3.;
	g_b = 3.; 
	stp = 1.4;
	f = -3.576;
	g = -0.72;
	minstp = 0.;
	maxstp = 10.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 1.4)
	TEST_REAL_EQUAL(f_a, -3.576)
	TEST_REAL_EQUAL(g_a, -0.72)
	TEST_REAL_EQUAL(st_b, 2.0)
	TEST_REAL_EQUAL(f_b, -3.0)
	TEST_REAL_EQUAL(g_b, 3.0)
	TEST_REAL_EQUAL(stp, 1.54858377035)
	TEST_EQUAL(ls.isBracketed(), true)

	// Two more examples where stp is not between st_a and st_b 
	// (to check cases when the quadratic (or the cubic) seems to be better.
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 2.;
	f_b = -3.;
	g_b = 3.; 
	stp = -1.4;
	f = -6.264;
	g = 10.48;
	minstp = 0.;
	maxstp = 10.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, -1.4)
	TEST_REAL_EQUAL(f_a, -6.264)
	TEST_REAL_EQUAL(g_a, 10.48)
	TEST_REAL_EQUAL(st_b, 0.0)
	TEST_REAL_EQUAL(f_b, -1.0)
	TEST_REAL_EQUAL(g_b, -1.0)
	TEST_REAL_EQUAL(stp, 1.54858377035)
	TEST_EQUAL(ls.isBracketed(), true)

	// Now the quadratic seems to be better (since the cubic step is not farther from
	// stp than the quadratic step)
	ls.setBracketedFlag(false);
	st_a = 0.;
	f_a = -1.;
	g_a = -1.;
	st_b = 2.;
	f_b = -3.;
	g_b = 3.; 
	stp = 15.;
	f = 2909.;
	g = 614.;
	minstp = 0.;
	maxstp = 10.;
	ls.takeStep(st_a, f_a, g_a, st_b, f_b, g_b, stp, f, g, minstp, maxstp);
	TEST_REAL_EQUAL(st_a, 0.0)
	TEST_REAL_EQUAL(f_a, -1.0)
	TEST_REAL_EQUAL(g_a, -1.0)
	TEST_REAL_EQUAL(st_b, 15.0)
	TEST_REAL_EQUAL(f_b, 2909.0)
	TEST_REAL_EQUAL(g_b, 614.0)
	TEST_REAL_EQUAL(stp, 0.793522654408)
	TEST_EQUAL(ls.isBracketed(), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
