// $Id: pairExpRDFIntegrator.C,v 1.8 2000/09/22 16:29:06 anker Exp $

#include <BALL/SOLVATION/pairExpRDFIntegrator.h>

using namespace std;

namespace BALL
{

	const char* PairExpRDFIntegrator::Option::VERBOSITY = "verbosity";
	const char* PairExpRDFIntegrator::Option::SAMPLES = "samples";

	const int PairExpRDFIntegrator::Default::VERBOSITY = 0;
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

		Size k = poly.getIntervalIndex(from);
		if (k == INVALID_POSITION)
		{
			return 0.0;
		}

		// the last interval has to be defined to infinity
		Size number_of_intervals = poly.getIntervals().size();
		interval = poly.getInterval(number_of_intervals - 1);
		// the point from where the integration to inf will start.
		double lower_inf;
		if (from < interval.first)
		{
			lower_inf = interval.first;
		}
		else
		{
			lower_inf = from;
		}
		if (interval.second != INFINITY)
		{
			Log.error() << "PairExpRDFIntegrator::integrateToInf(): "
				<< "Last interval must have infinity as upper limit." << endl;
			rdf_.dump();
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

		// only for readibility
		double r = lower_inf;
		double b = alpha_ / R_ij_o_;
		double d = pow(R_ij_o_, 6);
		double exp_br = exp( - b * r );
		double r3 = r * r * r;
		double infval = a[0] / (3 * pow(b, 3) * r3) *
			(
				  C1_ * 3 * b * b * r3 * r * r * exp_br
				+ C1_ * 6 * b * r3 * r * exp_br
				+ C1_ * 6 * r3 * exp_br
				- C2_ * d * b * b * b
				/*
			 		2 * C1_                   * R_ij_o_ * R_ij_o_ * E * r3
				+ 2 * C1_ * alpha_          * R_ij_o_           * E * r3 * from
				+     C1_ * alpha_ * alpha_                     * E * r3 * from * from
				-     C2_ * alpha_ * alpha_ * alpha_  * pow(R_ij_o_, 5)
				*/
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
		/*
		Log.info() << "alpha_ = " << alpha_ << endl;
		Log.info() << "C1_ = " << C1_ << endl;
		Log.info() << "C2_ = " << C2_ << endl;
		Log.info() << "R_ij_o_ = " << R_ij_o_ << endl;
		*/
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

		double val = 0.0;
		double r = interval.first;
		double R = interval.second;
		double b = alpha_/R_ij_o_;
		double R_ij_o_6 = pow(R_ij_o_, 6);

		// DEBUG
		/*
		Log.info() << "r = " << r << endl;
		Log.info() << "R = " << R << endl;
		Log.info() << "b = " << b << endl;
		Log.info() << "R_ij_o_6 = " << R_ij_o_6 << endl;
		Log.info() << "a[0] = " << a[0] << " a[1] = " << a[1] 
			<< " a[2] = " << a[2] << " a[3] = " << a[3] << endl;
		Log.info() << "k1 = " << k1_ << endl;
		Log.info() << "k2 = " << k2_ << endl;
		*/


		// this is the case where we have to consider the geometry of the
		// situation. As this seems analytically impossible, we have to do it
		// numerically. The method we use is the trapezium method.

		double area = 0;
		if (verbosity > 2)
		{
			Log.info() << "Using " << samples 
				<< " sample points for numerical integration" << endl;
		}
		unsigned int n = samples;

		// lower case variables are for the potential term
		double x = r;
		double s = (R-r)/n;
		// upper case variables are for the rdf term (representing the
		// geometrical correction)
		double X = sqrt((r*r + k1_ * r + k2_));
		double S = (sqrt((R*R + k1_ * R + k2_))-X)/n;
		while (n > 0)
		{
			// DEBUG
			/*
			Log.info() << "rdf_(" << X << ") = " << rdf_(X) << endl;
			Log.info() << "e^(-b*" << x << ") - R_ij_o/(" << x << ")^6 = " << 
				(C1_ * exp(-b*x) - C2_ * R_ij_o_6/pow(x,6)) << endl;
			Log.info() << "e^(-b*" << x << ") = " << C1_ * exp(-b*x) << endl;
			Log.info() << "R_ij_o/(" << x << ")^6 = " << C2_ * R_ij_o_6/pow(x,6) << endl;
			*/

			area += (x*x*(C1_ * exp(-b*x) - C2_ * R_ij_o_6/pow(x,6)) * rdf_(X) 
					+ (x+s)*(x+s)*(C1_ * exp(-b*(x+s)) - C2_ * R_ij_o_6/pow(x+s,6)) * rdf_(X+S))
				/2.0 * s;
			x += s;
			X += S;
			--n;
		}

		//Log.info() << "Ergebnis der numerischen Integration: " << area << endl;
		return area;
	}

} // namespace BALL
