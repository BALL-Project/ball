// $Id: Pair6_12RDFIntegrator_test.C,v 1.2 2000/10/18 12:00:33 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/pair6_12RDFIntegrator.h>

///////////////////////////

START_TEST(class_name, "$Id: Pair6_12RDFIntegrator_test.C,v 1.2 2000/10/18 12:00:33 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

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

Pair6_12RDFIntegrator* pointer;

CHECK(Pair6_12RDFIntegrator::Pair6_12RDFIntegrator())
  pointer = new Pair6_12RDFIntegrator();
	TEST_NOT_EQUAL(pointer, 0)
RESULT


CHECK(Pair6_12RDFIntegrator::~Pair6_12RDFIntegrator())
  delete pointer;
RESULT


CHECK(Pair6_12RDFIntegrator::Pair6_12RDFIntegrator(const Pair6_12RDFIntegrator& integrator))
	Pair6_12RDFIntegrator int1(1.0, 1.0, 1.0, 1.0, rdf);
	Pair6_12RDFIntegrator int2(int1);
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(Pair6_12RDFIntegrator::Pair6_12RDFIntegrator(double alpha, double C1, double C2, double R_ij_o, double k1, double k2, const RadialDistributionFunction& rdf))
	Pair6_12RDFIntegrator integrator(1.0, 1.0, 1.0, 1.0, rdf);
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (integrator.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(Pair6_12RDFIntegrator::destroy())
	Pair6_12RDFIntegrator integrator(1.0, 1.0, 1.0, 1.0, rdf);
	integrator.destroy();
	vector<Interval> nope;
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == nope);
	TEST_EQUAL(test, true);
	vector<Coefficients> naught;
	test = (integrator.getRDF().getRepresentation().getCoefficients() == naught);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(Pair6_12RDFIntegrator::clear())
	Pair6_12RDFIntegrator integrator(1.0, 1.0, 1.0, 1.0, rdf);
	integrator.destroy();
	vector<Interval> nope;
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == nope);
	TEST_EQUAL(test, true);
	vector<Coefficients> naught;
	test = (integrator.getRDF().getRepresentation().getCoefficients() == naught);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(Pair6_12RDFIntegrator::Pair6_12RDFIntegrator& operator = 
			(const Pair6_12RDFIntegrator& integrator))
	Pair6_12RDFIntegrator int1;
	int1.setRDF(RadialDistributionFunction(poly));
	int1.setConstants(1.0, 1.0, 1.0, 1.0);
	Pair6_12RDFIntegrator int2;
	int2 = int1;
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(Pair6_12RDFIntegrator::setConstants(double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
  //BAUSTELLE
RESULT


CHECK(Pair6_12RDFIntegrator::integrateToInf(double from) const )
	Pair6_12RDFIntegrator integrator(1, 1, 0, 0, rdf);
	double val;

	PRECISION(0.01)

	val = integrator.integrateToInf(3);
	double rel_err = fabs((val - (-0.02468006797)) / (-0.02468006797));
	TEST_REAL_EQUAL(rel_err, 0.01);

	integrator = Pair6_12RDFIntegrator(3, 2, 0, 0, rdf); 
	val = integrator.integrateToInf(3);
	rel_err = fabs((val - (-0.04934884587)) / (-0.04934884587));
	TEST_REAL_EQUAL(rel_err, 0.01);

	/*
	val = integrator.integrateToInf(2);
	rel_err = fabs((val - (-2.09822)) / -2.09822);
	TEST_REAL_EQUAL(rel_err, 0.01);
	*/
  //BAUSTELLE
RESULT


CHECK(Pair6_12RDFIntegrator::integrateToInf(double from, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
	Pair6_12RDFIntegrator integrator(1, 1, 0, 0, rdf);
  //BAUSTELLE
RESULT


CHECK(Pair6_12RDFIntegrator::integrate(double from, double to) const )
	
	// Zuerst der triviale Test (keine geometrische Korrektur)
	Pair6_12RDFIntegrator integrator(1, 1, 0, 0, rdf);
	double val;
	double rel_err;

	PRECISION(0.01)

	val = integrator.integrate(0.1, 0.9, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - (-0.4103491733)) / -0.4103491733);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(2.3, 2.7, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - (-0.004029138)) / -0.004029138);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(1.3, 2.3, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - (-0.3558524516)) / -0.3558524516);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now come the tests involving geometric correction (and therefore
	// numerical integration)

	// these limits correspond to 1.5 .. 1.7 as argument for the rdf.
	val = integrator.integrate(0.7247448714, 0.9628738838, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - 4.80942202) / 4.80942202);
	TEST_REAL_EQUAL(rel_err, 0.01);
	// these limits correspond to 1.7 .. 1.9 as argument for the rdf.
	val = integrator.integrate(0.9628738838, 1.191153453, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-0.1653446139)) / -0.1653446139);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1
	val = integrator.integrate(1.022497216, 1.278404875, 1.0, 1.0, 0.2, 1.0);
	rel_err = fabs((val - (-0.2650117748)) / -0.2650117748);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(1.278404875, 1.518641406, 1.0, 1.0, 0.2, 1.0);
	rel_err = fabs((val - (-0.2076186615)) / -0.2076186615);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1 and k2
	val = integrator.integrate(0.4099019514, 0.8486832981, 1.0, 1.0, 0.2, 2.0);
	rel_err = fabs((val - 1179.861575) / 1179.861575);
	TEST_REAL_EQUAL(rel_err, 0.01);

	val = integrator.integrate(0.8486832981, 1.172792206, 1.0, 1.0, 0.2, 2.0);
	rel_err = fabs((val - 0.4461636106) / 0.4461636106);
	TEST_REAL_EQUAL(rel_err, 0.01);
RESULT


/*
CHECK(Pair6_12RDFIntegrator::integrate(double from, double to, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))

	// Zuerst der triviale Test (keine geometrische Korrektur)
	Pair6_12RDFIntegrator integrator;
	integrator.setRDF(rdf);
	double val;
	double rel_err;

	PRECISION(0.01)

	val = integrator.integrate(0.1, 0.9, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - (-0.023580702)) / 0.023580702);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(2.3, 2.7, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - 0.0837784) / 0.0837784);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(1.3, 2.3, 1.0, 1.0, 0.0, 0.0);
	rel_err = fabs((val - 0.93246306) / 0.93246306);
	TEST_REAL_EQUAL(rel_err, 0.03);

	// now come the tests involving geometric correction (and therefore
	// numerical integration)

	// val = integrator.integrate(0.1, 1.0, 1.0, 1.0, 1.0, 1.0);
	// TEST_REAL_EQUAL(val, 0.0)

	// these limits correspond to 1.5 .. 1.7 as argument for the rdf.
	val = integrator.integrate(0.72474, 0.96287, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-1.53602))/ -1.53602);
	TEST_REAL_EQUAL(rel_err, 0.01);
	// these limits correspond to 1.7 .. 1.9 as argument for the rdf.
	val = integrator.integrate(0.96287, 1.19115, 1.0, 1.0, 1.0, 1.0);
	rel_err = fabs((val - (-0.32462))/ -0.32462);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1
	val = integrator.integrate(0.37038, 0.53470, 1.0, 1.0, 3.0, 1.0);
	rel_err = fabs((val - (-15.50386))/ -15.50386);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(0.53470, 0.70454, 1.0, 1.0, 3.0, 1.0);
	Log.info() << val;
	rel_err = fabs((val - (-4.50856))/ -4.50856);
	TEST_REAL_EQUAL(rel_err, 0.01);

	// now the same with some k1 and k2
	val = integrator.integrate(0.08113, 0.27200, 1.0, 1.0, 3.0, 2.0);
	rel_err = fabs((val - (-2147.68792)) / -2147.68792);
	TEST_REAL_EQUAL(rel_err, 0.01);
	val = integrator.integrate(0.27200, 0.46469, 1.0, 1.0, 3.0, 2.0);
	rel_err = fabs((val - (-49.80145)) / -49.80145);
	TEST_REAL_EQUAL(rel_err, 0.01);
RESULT


CHECK(Pair6_12RDFIntegrator::double operator () (double x) const )
	Pair6_12RDFIntegrator integrator(1, 1, 0, 0, rdf);
	double val;

	PRECISION(0.01)

	val = integrator(3);
	double rel_err = fabs((val - 1.66807) / 1.66807);
	TEST_REAL_EQUAL(rel_err, 0.01);

	integrator = Pair6_12RDFIntegrator(3, 1, 0, 0, rdf); 
	val = integrator(3);
	rel_err = fabs((val - (-1.37452)) / (-1.37452));
	TEST_REAL_EQUAL(rel_err, 0.01);

	val = integrator(2);
	rel_err = fabs((val - (-2.09822)) / -2.09822);
	TEST_REAL_EQUAL(rel_err, 0.01);
  //BAUSTELLE
RESULT
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
