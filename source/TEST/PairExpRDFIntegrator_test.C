// $Id: PairExpRDFIntegrator_test.C,v 1.2 2000/09/02 16:19:23 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/SOLVATION/pairExpRDFIntegrator.h>

///////////////////////////

START_TEST(class_name, "$Id: PairExpRDFIntegrator_test.C,v 1.2 2000/09/02 16:19:23 anker Exp $")

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
	int1.getRDF().setRepresentation(poly);
	int1.setConstants(alpha, C1, C2, R_ij_o, k1, k2);
	PairExpRDFIntegrator int2(int1);
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator(double alpha, double C1, double C2, double R_ij_o, double k1, double k2, const RadialDistributionFunction& rdf))
	PairExpRDFIntegrator integrator(alpha, C1, C2, R_ij_o, k1, k2, rdf);
	bool test = (integrator.getRDF().getRepresentation().getIntervals() == intervals);
	Log.info() << "Intervalle: " << rdf.getRepresentation().getIntervals().size() << endl;
	Log.info() << "Intervalle: " << integrator.getRDF().getRepresentation().getIntervals().size() << endl;
	TEST_EQUAL(test, true);
	test = (integrator.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::destroy())
	PairExpRDFIntegrator integrator(alpha, C1, C2, R_ij_o, k1, k2, rdf);
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


CHECK(PairExpRDFIntegrator::clear())
	PairExpRDFIntegrator integrator(alpha, C1, C2, R_ij_o, k1, k2, rdf);
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


CHECK(PairExpRDFIntegrator::set(const PairExpRDFIntegrator& integrator))
	PairExpRDFIntegrator int1;
	int1.getRDF().setRepresentation(poly);
	int1.setConstants(alpha, C1, C2, R_ij_o, k1, k2);
	PairExpRDFIntegrator int2;
	int2.set(int1);
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::PairExpRDFIntegrator& operator = 
			(const PairExpRDFIntegrator& integrator))
	PairExpRDFIntegrator int1;
	int1.getRDF().setRepresentation(poly);
	int1.setConstants(alpha, C1, C2, R_ij_o, k1, k2);
	PairExpRDFIntegrator int2;
	int2 = int1;
	bool test = (int2.getRDF().getRepresentation().getIntervals() == intervals);
	TEST_EQUAL(test, true);
	test = (int2.getRDF().getRepresentation().getCoefficients() == coefs);
	TEST_EQUAL(test, true);
	// BAUSTELLE: Konstanten
	// BAUSTELLE: false, Spezialfälle
RESULT


CHECK(PairExpRDFIntegrator::setConstants(double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrateToInf(double from) const )
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
	double val;
	val = integrator.integrateToInf(3);
	TEST_REAL_EQUAL(val, 0.097928046)
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrateToInf(double from, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
  //BAUSTELLE
RESULT


CHECK(PairExpRDFIntegrator::integrate(double from, double to) const )
	PairExpRDFIntegrator integrator(1, 1, 1, 1, 0, 0, rdf);
	integrator.setRDF(rdf);
	double val;
	val = integrator.integrate(0.0, 1.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5);
	TEST_REAL_EQUAL(val, -0.080510343)
	val = integrator.integrate(2.3, 2.7);
	TEST_REAL_EQUAL(val, 0.0042484315)
	val = integrator.integrate(1.3, 2.3);
	TEST_REAL_EQUAL(val, 0.3282713538)
RESULT


CHECK(PairExpRDFIntegrator::integrate(double from, double to, double alpha, double C1, double C2, double R_ij_o, double k1, double k2))
	// Zuerst der triviale Test (keine geometrische Korrektur)
	PairExpRDFIntegrator integrator(0, 0, 0, 0, 0, 0, rdf);
	integrator.setRDF(rdf);
	double val;
	val = integrator.integrate(0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, -0.080510343)
	val = integrator.integrate(2.3, 2.7, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.012745294)
	val = integrator.integrate(1.3, 2.3, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0);
	TEST_REAL_EQUAL(val, 0.3282713538)

	val = integrator.integrate(0.1, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	TEST_REAL_EQUAL(val, 0.0)
	val = integrator.integrate(1.0, 1.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
	TEST_REAL_EQUAL(val, -0.1022999316)
RESULT


CHECK(PairExpRDFIntegrator::double operator () (double x) const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
