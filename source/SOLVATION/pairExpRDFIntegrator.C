// $Id: pairExpRDFIntegrator.C,v 1.4 2000/09/02 17:36:31 anker Exp $

#include <BALL/SOLVATION/pairExpRDFIntegrator.h>

using namespace std;

namespace BALL
{

	const char* PairExpRDFIntegrator::Option::VERBOSITY = "verbosity";
	const char* PairExpRDFIntegrator::Option::SAMPLES = "samples";

	const int PairExpRDFIntegrator::Default::VERBOSITY = 1;
	const int PairExpRDFIntegrator::Default::SAMPLES = 30;

	PairExpRDFIntegrator::PairExpRDFIntegrator()
		:	RDFIntegrator(),
			alpha_(0.0),
			C1_(0.0),
			C2_(0.0),
			R_ij_o_(0.0),
			k1_(0.0),
			k2_(0.0),
			rdf_()
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultInteger(Option::SAMPLES, Default::SAMPLES);
	}


	PairExpRDFIntegrator::PairExpRDFIntegrator
			(const PairExpRDFIntegrator& integrator)
		:	RDFIntegrator(integrator),
			alpha_(integrator.alpha_),
			C1_(integrator.C1_),
			C2_(integrator.C2_),
			R_ij_o_(integrator.R_ij_o_),
			k1_(integrator.k1_),
			k2_(integrator.k2_),
			rdf_(integrator.rdf_)
	{
	}


	PairExpRDFIntegrator::PairExpRDFIntegrator(double alpha, double C1,
			double C2, double R_ij_o, double k1, double k2, 
			const RadialDistributionFunction& rdf)
		:	alpha_(alpha),
			C1_(C1),
			C2_(C2),
			R_ij_o_(R_ij_o),
			k1_(k1),
			k2_(k2),
			rdf_(rdf)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultInteger(Option::SAMPLES, Default::SAMPLES);
	}


	PairExpRDFIntegrator::~PairExpRDFIntegrator()
	{
		destroy();
	}


	void PairExpRDFIntegrator::destroy()
	{
		clear();
	}


	void PairExpRDFIntegrator::clear()
	{
		rdf_.clear();
	}


	void PairExpRDFIntegrator::set(const PairExpRDFIntegrator& integrator)
	{
		alpha_ = integrator.alpha_;
		C1_ = integrator.C1_;
		C2_ = integrator.C2_;
		R_ij_o_ = integrator.R_ij_o_;
		k1_ = integrator.k1_;
		k2_ = integrator.k2_;
		rdf_ = integrator.rdf_;
	}


	const PairExpRDFIntegrator& PairExpRDFIntegrator::operator = 
		(const PairExpRDFIntegrator& integrator)
	{
		set(integrator);
		return *this;
	}


	void PairExpRDFIntegrator::setConstants(double alpha, double C1, double C2, 
			double R_ij_o, double k1, double k2)
	{
		alpha_ = alpha;
		C1_ = C1;
		C2_ = C2;
		R_ij_o_ = R_ij_o;
		k1_ = k1;
		k2_ = k2;
	}


	double PairExpRDFIntegrator::integrateToInf(double from) const
	{

		PiecewisePolynomial poly = rdf_.getRepresentation();
		Interval interval;
		double val = 0.0;

		Size k = poly.isInRange(from);
		if (k == INVALID_POSITION)
		{
			return 0.0;
		}

		// the last interval has to be defined to infinity
		Size number_of_intervals = poly.getIntervals().size();
		interval = poly.getInterval(number_of_intervals - 1);
		if (interval.second != INFINITY)
		{
			Log.error() << "PairExpRDFIntegrator::integrateToInf(): "
				<< "Last interval has to have infinity as upper limit." << endl;
				return 0.0;
		}

		// first "finish" the interval we start at, if this isn't already the
		// last interval

		if (k < number_of_intervals - 1)
		{
			interval = poly.getInterval(k);
			val += integrate(from, interval.second);
			++k;

			// then sum up all following intervals
			for (; k < number_of_intervals - 1; ++k)
			{
				interval = poly.getInterval(k);
				val += integrate(interval.first, interval.second);
			}
		}

		// now add the last interval (to infinity)

		Coefficients a = poly.getCoefficients(number_of_intervals - 1);
		if ((a[1] != 0.0) || (a[2] != 0.0) || (a[3] != 0.0)) 
		{
			Log.warn() << "RDF::integralToInf(): Got a non-constant polynomial."
				<< " There might be something wrong." << endl;
		}

		double E = 3.0 * exp(-alpha_ * from / R_ij_o_);
		double r3 = from * from * from;
		double infval = a[0] * R_ij_o_ / (3 * pow(alpha_, 3) * r3) *
			(
			 		2 * C1_                   * R_ij_o_ * R_ij_o_ * E * r3
				+ 2 * C1_ * alpha_          * R_ij_o_           * E * r3 * from
				+     C1_ * alpha_ * alpha_                     * E * r3 * from * from
				-     C2_ * alpha_ * alpha_ * alpha_  * pow(R_ij_o_, 5)
			);
		val += infval;
		return val;
	}


	double PairExpRDFIntegrator::integrateToInf(double from, double alpha, 
			double C1, double C2, double R_ij_o, double k1, double k2)
	{
		setConstants(alpha, C1, C2, R_ij_o, k1, k2);
		return integrateToInf(from);
	}


	double PairExpRDFIntegrator::integrate(double from, double to) const
	{

		PiecewisePolynomial poly = rdf_.getRepresentation();

		Size from_index = poly.getIntervalIndex(from);
		Size to_index = poly.getIntervalIndex(to);

		if ((from_index == INVALID_POSITION) || (to_index == INVALID_POSITION))
		{
			return 0.0;
		}

		Interval interval(from, to);
		Coefficients coeffs = poly.getCoefficients(from_index);

		if (from_index == to_index)
		{
			return numericallyIntegrateInterval(interval, coeffs, from_index);
		}

		// if we didn't return, the indices weren't equal.

		interval.second = poly.getInterval(from_index).second;
		double val = numericallyIntegrateInterval(interval, coeffs, from_index);

		for (Size k = from_index + 1; k < to_index; ++k)
		{
			coeffs = poly.getCoefficients(k);
			interval = poly.getInterval(k);
			val += numericallyIntegrateInterval(interval, coeffs, k);
		}

		coeffs = poly.getCoefficients(to_index);
		interval = poly.getInterval(to_index);
		interval.second = to;
		val += numericallyIntegrateInterval(interval, coeffs, to_index);
		
		return val;
	}


	double PairExpRDFIntegrator::integrate(double from, double to, double alpha, 
			double C1, double C2, double R_ij_o, double k1, double k2)
	{
		setConstants(alpha, C1, C2, R_ij_o, k1, k2);
		// DEBUG
		Log.info() << "alpha_ = " << alpha_ << endl;
		Log.info() << "C1_ = " << C1_ << endl;
		Log.info() << "C2_ = " << C2_ << endl;
		Log.info() << "R_ij_o_ = " << R_ij_o_ << endl;
		return integrate(from, to);
	}


	double PairExpRDFIntegrator::operator () (double x) const
	{
		return integrateToInf(x);
	}


	double PairExpRDFIntegrator::numericallyIntegrateInterval(Interval
			interval, Coefficients a, Position index) const
	{

		// BAUSTELLE
		int samples = (int) options.getInteger(Option::SAMPLES);
		int verbosity = (int) options.getInteger(Option::VERBOSITY);
		if (verbosity > 0)
		{
			Log.info() << "Using " << samples << " sample points for numerical integration" << endl;
		}

		double val = 0.0;
		double r = interval.first;
		double R = interval.second;
		double b = alpha_/R_ij_o_;
		double R_ij_o_6 = pow(R_ij_o_, 6);
		double d = R_ij_o_6;
		double k = rdf_.getRepresentation().getInterval(index).first;

		// DEBUG
		Log.info() << "r = " << r << endl;
		Log.info() << "R = " << R << endl;
		Log.info() << "b = " << b << endl;
		Log.info() << "R_ij_o_6 = " << R_ij_o_6 << endl;
		Log.info() << "a[0] = " << a[0] << " a[1] = " << a[1] 
			<< " a[2] = " << a[2] << " a[3] = " << a[3] << endl;
		Log.info() << "k = " << k << endl;

		if (fabs(k2_) < 1e-10)
		{

			Log.info() << "keine geometrische Korrektur nötig." << endl;

			// the molecule we sit on is that defining the current sphere, so we
			// dont have to consider the geometric correction. Thus we have an
			// analytical form we can use.


			double s3 = d*pow(b,4.0)*a[0]/5+d*pow(b,4.0)*a[1]*R/4+d*pow(b,4.0)*a[2]*R*R/3+d*pow(b,4.0)*a[3]*R*R*R/2-6.0*exp(-b*R)*a[3]*pow(R,5.0)-exp(-b*R)*a[0]*b*b*b*pow(R,5.0)-exp(-b*R)*a[1]*b*b*pow(R,5.0)-exp(-b*R)*a[1]*b*b*b*pow(R,6.0)-2.0*exp(-b*R)*a[2]*b*b*pow(R,6.0)-2.0*exp(-b*R)*a[2]*b*pow(R,5.0)-exp(-b*R)*a[2]*b*b*b*pow(R,7.0)-6.0*exp(-b*R)*a[3]*b*pow(R,6.0)-3.0*exp(-b*R)*a[3]*b*b*pow(R,7.0)-exp(-b*R)*a[3]*b*b*b*pow(R,8.0)-d*pow(b,4.0)*a[2]*k*R/2;
			double s2 = s3-d*pow(b,4.0)*a[3]*k*R*R+3.0/4.0*d*pow(b,4.0)*a[3]*k*k*R-d*pow(b,4.0)*a[1]*k/5+d*pow(b,4.0)*a[2]*k*k/5-d*pow(b,4.0)*a[3]*k*k*k/5-3.0*exp(-b*R)*a[3]*k*k*b*b*b*pow(R,6.0)+6.0*exp(-b*R)*a[3]*k*b*pow(R,5.0)+2.0*exp(-b*R)*a[2]*k*b*b*pow(R,5.0)+6.0*exp(-b*R)*a[3]*k*b*b*pow(R,6.0)+3.0*exp(-b*R)*a[3]*k*b*b*b*pow(R,7.0)-3.0*exp(-b*R)*a[3]*k*k*b*b*pow(R,5.0)+exp(-b*R)*a[1]*k*b*b*b*pow(R,5.0)+exp(-b*R)*a[3]*k*k*k*b*b*b*pow(R,5.0)-exp(-b*R)*a[2]*k*k*b*b*b*pow(R,5.0)+2.0*exp(-b*R)*a[2]*k*b*b*b*pow(R,6.0);
			s3 = 1/pow(b,4.0)/pow(R,5.0);
			double s1 = s2*s3;
			double s4 = -d*pow(b,4.0)*a[0]/5-d*pow(b,4.0)*a[1]*r/4-d*pow(b,4.0)*a[2]*r*r/3-d*pow(b,4.0)*a[3]*r*r*r/2+6.0*exp(-b*r)*a[3]*pow(r,5.0)+exp(-b*r)*a[0]*b*b*b*pow(r,5.0)+exp(-b*r)*a[1]*b*b*b*pow(r,6.0)+2.0*exp(-b*r)*a[2]*b*pow(r,5.0)+exp(-b*r)*a[1]*b*b*pow(r,5.0)+2.0*exp(-b*r)*a[2]*b*b*pow(r,6.0)+exp(-b*r)*a[2]*b*b*b*pow(r,7.0)+6.0*exp(-b*r)*a[3]*b*pow(r,6.0)+3.0*exp(-b*r)*a[3]*b*b*pow(r,7.0)+exp(-b*r)*a[3]*b*b*b*pow(r,8.0)+d*pow(b,4.0)*a[1]*k/5;
			s3 = s4-d*pow(b,4.0)*a[2]*k*k/5+d*pow(b,4.0)*a[3]*k*k*k/5-6.0*exp(-b*r)*a[3]*k*b*pow(r,5.0)+d*pow(b,4.0)*a[2]*k*r/2+d*pow(b,4.0)*a[3]*k*r*r-3.0/4.0*d*pow(b,4.0)*a[3]*k*k*r-exp(-b*r)*a[1]*k*b*b*b*pow(r,5.0)-2.0*exp(-b*r)*a[2]*k*b*b*b*pow(r,6.0)-2.0*exp(-b*r)*a[2]*k*b*b*pow(r,5.0)-3.0*exp(-b*r)*a[3]*k*b*b*b*pow(r,7.0)+3.0*exp(-b*r)*a[3]*k*k*b*b*b*pow(r,6.0)+3.0*exp(-b*r)*a[3]*k*k*b*b*pow(r,5.0)+exp(-b*r)*a[2]*k*k*b*b*b*pow(r,5.0)-6.0*exp(-b*r)*a[3]*k*b*b*pow(r,6.0)-exp(-b*r)*a[3]*k*k*k*b*b*b*pow(r,5.0);
			s4 = 1/pow(b,4.0)/pow(r,5.0);
			s2 = s3*s4;
			val = s1+s2;


			Log.info() << "RDF::integralToRC(), ohne G: " << val << endl;

			return val;

		}
		else
		{

			// this is the case where we have to consider the geometry of the
			// situation. As this seems analytically impossible, we have to do it
			// numerically. The method we use is the trapezium method.

			// BAUSTELLE: Die Anzahl der Stützstellen muss per Option konfiguriert
			// werden können.

			double area = 0;
			double x = r;
			unsigned int n = samples;
			double s = (R-r)/n;
			while (n > 0)
			{
				area += ((exp(-b*x) - R_ij_o_6/pow(x,6))*rdf_(x) 
						+ (exp(-b*x) - R_ij_o_6/pow(x,6))*rdf_(x+s))/2.0 * s;
				x += s;
				--n;
			}

			Log.info() << "RDF::integralToRC(), mit G: " << area << endl;

			return area;
		}
	}

} // namespace BALL
