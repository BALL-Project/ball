// $Id: PairExpRDFIntegrator_test.C,v 1.10 2001/12/30 13:28:59 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/pairExpRDFIntegrator.h>

///////////////////////////

START_TEST(PairExpRDFIntegrator, "$Id: PairExpRDFIntegrator_test.C,v 1.10 2001/12/30 13:28:59 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace std;
using namespace BALL;

///  insert tests for each member function here         
///
	
// tests for class PairExpRDFIntegrator::

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

double alpha = 1;
double C1 = 1; 
double C2 = 1; 
double R_ij_o = 1; 
double k1 = 0;
double k2 = 0;

PairExpRDFIntegrator* pointer;

CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator())
  pointer = new PairExpRDFIntegrator();
	TEST_NOT_EQUAL(pointer, 0)
RESULT


CHECK(PairExpRDFIntegrator::~PairExpRDFIntegrator())
  delete pointer;
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator(const PairExpRDFIntegrator& integrator))
	PairExpRDFIntegrator int1;
	int1.setRDF(RadialDistributionFunction(poly));
	int1.setConstants(alpha, C1, C2, R_ij_o, k1, k2);
	PairExpRDFIntegrator int2(int1);
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// ?????: Konstanten
	// ?????: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator(double alpha, double C1, double C2, double R_ij_o, double k1, double k2, const RadialDistributionFunction& rdf))
	PairExpRDFIntegrator integrator(alpha, C1, C2, R_ij_o, k1, k2, rdf);
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (integrator.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// ?????: Konstanten
	// ?????: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::clear())
	PairExpRDFIntegrator integrator(alpha, C1, C2, R_ij_o, k1, k2, rdf);
	integrator.clear();
	vector<Interval> nope;
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == nope);
	TEST_EQUAL(test, true);
	vector<Coefficients> naught;
	test = (integrator.getRDF().getRepresentation().getCoefficients() == naught);
	TEST_EQUAL(test, true);
	// ?????: Konstanten
	// ?????: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator& operator = 
			(const PairExpRDFIntegrator& integrator))
	PairExpRDFIntegrator int1;
	int1.setRDF(RadialDistributionFunction(poly));
	int1.setConstants(alpha, C1, C2, R_ij_o, k1, k2);
	PairExpRDFIntegrator int2;
	int2 = int1;
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// ?????: Konstanten
	// ?????: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::setConstants(double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
  //?????
RESULT


CHECK(PairExpRDFIntegrator::integrateToInf(double from) const )
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
	double val;

	PRECISION(0.01)

	val = integrator.integrateToInf(3);
	double rel_err = fabs((val - 1.66807) / 1.66807);
	TEST_REAL_EQUAL(rel_err, 0.01);

	integrator = PairExpRDFIntegrator(3, 1, 1, 2, 0, 0, rdf); 
	val = integrator.integrateToInf(3);
	rel_err = fabs((val - (-1.37452)) / (-1.37452));
	TEST_REAL_EQUAL(rel_err, 0.01);

	val = integrator.integrateToInf(2);
	rel_err = fabs((val - (-2.09822)) / -2.09822);
	TEST_REAL_EQUAL(rel_err, 0.01);
  //?????
RESULT


CHECK(PairExpRDFIntegrator::integrateToInf(double from, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
  //?????
RESULT


CHECK(PairExpRDFIntegrator::integrate(double from, double to) const )
	
	// Zuerst der triviale Test (keine geometrische Korrektur)
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
	double val;
	double rel_err;

	PRECISION(0.01)

	val = integrator.integrate(0.1, 0.9, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - (-0.023580702)) / 0.023580702);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(2.3, 2.7, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - 0.0837784) / 0.0837784);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(1.3, 2.3, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - 0.93246306) / 0.93246306);
	TEST_REAL_EQUAL(rel_err, 0.03);

	// now come the tests involving geometric correction (and therefore
	// numerical integration)

	// val = integrator.integrate(0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	// TEST_REAL_EQUAL(val, 0.0)

	// these limits correspond to 1.5 .. 1.7 as argument for the rdf.
	val = integrator.integrate(0.72474, 0.96287, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-1.53602))/ -1.53602);
	TEST_REAL_EQUAL(rel_err, 0.01);
	// these limits correspond to 1.7 .. 1.9 as argument for the rdf.
	val = integrator.integrate(0.96287, 1.19115, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-0.32462))/ -0.32462);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1
	val = integrator.integrate(0.37038, 0.53470, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0);
	rel_err = fabs((val - (-15.50386))/ -15.50386);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(0.53470, 0.70454, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0);
	Log.info() << val;
	rel_err = fabs((val - (-4.50856))/ -4.50856);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1 and k2
	val = integrator.integrate(0.08113, 0.27200, 1.0, 1.0, 1.0, 1.0, 3.0, 2.0);
	rel_err = fabs((val - (-2147.68792)) / -2147.68792);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(0.27200, 0.46469, 1.0, 1.0, 1.0, 1.0, 3.0, 2.0);
	rel_err = fabs((val - (-49.80145)) / -49.80145);
	TEST_REAL_EQUAL(rel_err, 0.01);
RESULT


CHECK(PairExpRDFIntegrator::integrate(double from, double to, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))

	// Zuerst der triviale Test (keine geometrische Korrektur)
	PairExpRDFIntegrator integrator;
	integrator.setRDF(rdf);
	double val;
	double rel_err;

	PRECISION(0.01)

	val = integrator.integrate(0.1, 0.9, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - (-0.023580702)) / 0.023580702);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(2.3, 2.7, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - 0.0837784) / 0.0837784);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(1.3, 2.3, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - 0.93246306) / 0.93246306);
	TEST_REAL_EQUAL(rel_err, 0.03);

	// now come the tests involving geometric correction (and therefore
	// numerical integration)

	// val = integrator.integrate(0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	// TEST_REAL_EQUAL(val, 0.0)

	// these limits correspond to 1.5 .. 1.7 as argument for the rdf.
	val = integrator.integrate(0.72474, 0.96287, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-1.53602))/ -1.53602);
	TEST_REAL_EQUAL(rel_err, 0.01);
	// these limits correspond to 1.7 .. 1.9 as argument for the rdf.
	val = integrator.integrate(0.96287, 1.19115, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-0.32462))/ -0.32462);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1
	val = integrator.integrate(0.37038, 0.53470, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0);
	rel_err = fabs((val - (-15.50386))/ -15.50386);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(0.53470, 0.70454, 1.0, 1.0, 1.0, 1.0, 3.0, 1.0);
	Log.info() << val;
	rel_err = fabs((val - (-4.50856))/ -4.50856);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1 and k2
	val = integrator.integrate(0.08113, 0.27200, 1.0, 1.0, 1.0, 1.0, 3.0, 2.0);
	rel_err = fabs((val - (-2147.68792)) / -2147.68792);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(0.27200, 0.46469, 1.0, 1.0, 1.0, 1.0, 3.0, 2.0);
	rel_err = fabs((val - (-49.80145)) / -49.80145);
	TEST_REAL_EQUAL(rel_err, 0.01);
RESULT


CHECK(PairExpRDFIntegrator::double operator () (double x) const )
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
	double val;

	PRECISION(0.01)

	val = integrator(3);
	double rel_err = fabs((val - 1.66807) / 1.66807);
	TEST_REAL_EQUAL(rel_err, 0.01);

	integrator = PairExpRDFIntegrator(3, 1, 1, 2, 0, 0, rdf); 
	val = integrator(3);
	rel_err = fabs((val - (-1.37452)) / (-1.37452));
	TEST_REAL_EQUAL(rel_err, 0.01);

	val = integrator(2);
	rel_err = fabs((val - (-2.09822)) / -2.09822);
	TEST_REAL_EQUAL(rel_err, 0.01);
  //?????
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
