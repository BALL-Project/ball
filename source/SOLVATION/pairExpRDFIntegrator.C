// $Id: pairExpRDFIntegrator.C,v 1.12 2000/09/28 17:20:46 anker Exp $

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
			k2_(0.0)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultInteger(Option::SAMPLES, Default::SAMPLES);
	}


	PairExpRDFIntegrator::PairExpRDFIntegrator
			(const PairExpRDFIntegrator& integrator)
		:	RDFIntegrator(integrator),
			options(integrator.options),
			alpha_(integrator.alpha_),
			C1_(integrator.C1_),
			C2_(integrator.C2_),
			R_ij_o_(integrator.R_ij_o_),
			k1_(integrator.k1_),
			k2_(integrator.k2_)
	{
	}


	PairExpRDFIntegrator::PairExpRDFIntegrator(double alpha, double C1,
			double C2, double R_ij_o, double k1, double k2, 
			const RadialDistributionFunction& rdf)
		:	RDFIntegrator(rdf),
			alpha_(alpha),
			C1_(C1),
			C2_(C2),
			R_ij_o_(R_ij_o),
			k1_(k1),
			k2_(k2)
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
		alpha_ = 0.0;
		C1_ = 0.0;
		C2_ = 0.0;
		R_ij_o_ = 0.0;
		k1_ = 0.0;
		k2_ = 0.0;
		RDFIntegrator::clear();
	}


	void PairExpRDFIntegrator::set(const PairExpRDFIntegrator& integrator)
	{
		alpha_ = integrator.alpha_;
		C1_ = integrator.C1_;
		C2_ = integrator.C2_;
		R_ij_o_ = integrator.R_ij_o_;
		k1_ = integrator.k1_;
		k2_ = integrator.k2_;
		options = integrator.options;
		RDFIntegrator::set(integrator);
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

		PiecewisePolynomial poly = getRDF().getRepresentation();
		Interval interval;
		double FROM;
		double val = 0.0;

		FROM = project(from);

		Size k = poly.getIntervalIndex(FROM);
		if (k == INVALID_POSITION)
		{
			// no error message, because getIntervalIndex() handles this
			return 0.0;
		}

		Size number_of_intervals = poly.getIntervals().size();
		interval = poly.getInterval(number_of_intervals - 1);

		// the last interval has to be defined to infinity
		if (interval.second != INFINITY)
		{
			Log.error() << "PairExpRDFIntegrator::integrateToInf(): "
				<< "Last interval must have infinity as upper limit." << endl;
			getRDF().dump();
			return 0.0;
		}
		double lower_inf = interval.first;

		// the point from where the integration to inf will start.
		if (FROM < interval.first)
		{
			interval = Interval(from, unproject(lower_inf));
			val = numericallyIntegrateInterval(interval);
		}

		// only for readibility

		double a = poly.getCoefficients(number_of_intervals - 1)[0];
		double r = lower_inf;
		double b = alpha_ / R_ij_o_;
		double d = pow(R_ij_o_, 6);
		double exp_br = exp( - b * r );
		double r3 = r * r * r;
		double infval = a / (3 * pow(b, 3) * r3) *
			(
				  C1_ * 3 * b * b * r3 * r * r * exp_br
				+ C1_ * 6 * b * r3 * r * exp_br
				+ C1_ * 6 * r3 * exp_br
				- C2_ * d * b * b * b
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
		Interval interval(from, to);
		return numericallyIntegrateInterval(interval);
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


	double PairExpRDFIntegrator::numericallyIntegrateInterval(Interval interval) 
		const
	{

		// BAUSTELLE
		int samples = (int) options.getInteger(Option::SAMPLES);
		int verbosity = (int) options.getInteger(Option::VERBOSITY);

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
			Log.info() << "rdf(" << X << ") = " << getRDF()(X) << endl;
			Log.info() << "e^(-b*" << x << ") - R_ij_o/(" << x << ")^6 = " << 
				(C1_ * exp(-b*x) - C2_ * R_ij_o_6/pow(x,6)) << endl;
			Log.info() << "e^(-b*" << x << ") = " << C1_ * exp(-b*x) << endl;
			Log.info() << "R_ij_o/(" << x << ")^6 = " << C2_ * R_ij_o_6/pow(x,6) << endl;
			*/

			area += (x*x*(C1_ * exp(-b*x) - C2_ * R_ij_o_6/pow(x,6)) * getRDF()(X) 
					+ (x+s)*(x+s)*(C1_ * exp(-b*(x+s)) - C2_ * R_ij_o_6/pow(x+s,6)) *
					getRDF()(X+S)) /2.0 * s;
			x += s;
			X += S;
			--n;
		}

		//Log.info() << "Ergebnis der numerischen Integration: " << area << endl;
		return area;
	}


	void PairExpRDFIntegrator::dump(ostream& stream, Size /* depth */) const
	{
		stream << "[PairExpRDFIntegrator:]" << endl;
		stream << "alpha_ = " << alpha_ << endl;
		stream << "C1_ = " << C1_ << endl;
		stream << "C2_ = " << C2_ << endl;
		stream << "R_ij_o_ = " << R_ij_o_ << endl;
		stream << "k1_ = " << k1_ << endl;
		stream << "k2_ = " << k2_ << endl;
		getRDF().dump();
	}


	double PairExpRDFIntegrator::project(double x) const 
	{
		return sqrt(x*x + k1_ * x + k2_);
	}


	double PairExpRDFIntegrator::unproject(double x) const 
	{
		return sqrt(x*x - k1_*k1_ / 4 - k2_) - k1_ / 2;
	}

} // namespace BALL
