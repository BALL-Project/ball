// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: LineSearch_test.C,v 1.2 2003/02/03 21:38:19 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>

///////////////////////////

START_TEST(LineSearch, "$Id: LineSearch_test.C,v 1.2 2003/02/03 21:38:19 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(LineSearch::interpolate(double lambda_0, double lambda_1, double energy_, double energy_1, double grad_0, double grad_1) const)
	LineSearch ls;
	// Something trivial: y = x - 20 x^2 + x^3
	// This should yield a minimum at 13.30828628172376
	double result = ls.interpolate(0.0, 20.0, 0.0, 20. - 20. * 400. + 8000., 1.0, 1. - 800. + 1200.);
	TEST_REAL_EQUAL(result, 13.30828628172376)

	// And its inverse: y = -x + 20 x^2 - x^3
	// This should yield a minimum at 0.02502
	result = ls.interpolate(0.0, 20.0, 0.0, -20. + 20. * 400. - 8000., -1.0, -1. + 800. - 1200.);
	TEST_REAL_EQUAL(result, 20./3. - sqrt(397./9.))

	// Next, a purely quadratic one: y = -8 x + x^2
	result = ls.interpolate(0.0, 10.0, 0.0, 20.0, -8., 12.0);
	TEST_REAL_EQUAL(result, 4.0)

	// Next, a purely quadratic one: y = -8 x + x^2, swapped boundaries
	result = ls.interpolate(10.0, 0.0, 20.0, 0.0, 12., -8.0);
	TEST_REAL_EQUAL(result, 4.0)

	// Next, a purely quadratic one: y = -8 x + x^2, with the min outside the range
	result = ls.interpolate(0.0, 1.0, 0.0, -7, -8., -6.);
	TEST_REAL_EQUAL(result, 4.0)

	// Next, a linear one: y = 7 x
	result = ls.interpolate(0.0, 1.0, 0.0, 7.0, 7.0, 7.0);
	TEST_REAL_EQUAL(result, 0.0)

	// Next, a linear one: y = 7 x, with incorrect order of lambda_0/1
	result = ls.interpolate(1.0, 0.0, 7.0, 0.0, 7.0, 7.0);
	TEST_REAL_EQUAL(result, 0.0)

	// Then, consistency check: lambda_0 == lambda_1, we return lambda_0 in this case
	result = ls.interpolate(0.12345, 0.12345, 23.456, 3.4567, -123.455, 234.567);
	TEST_REAL_EQUAL(result, 0.12345)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
