// $Id: pairExpRDFIntegrator.C,v 1.1 2000/08/31 18:12:29 anker Exp $

#include <BALL/SOLVATION/pairExpRDFIntegrator.h>

namespace BALL
{

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
		setConstants(alpha, C1, C2, R_ij_o, k1, k2);
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

		double val = 0.0;
		double r = interval.first;
		double R = interval.second;
		double b = alpha_/R_ij_o_;
		double R_ij_o_6 = pow(R_ij_o_, 6);
		double d = R_ij_o_6;

		if (fabs(k2_) < 1e-10)
		{

			// the molecule we sit on is that defining the current sphere, so we
			// dont have to consider the geometric correction. Thus we have an
			// analytical form, we can use.

			val = (720*pow(r,3)*a[3]*C1_ + 
						144*b*pow(r,3)*
						 (a[2] + 5*r*a[3])*C1_ + 
						36*pow(b,2)*pow(r,3)*
						 (a[1] + 4*r*a[2] + 
							 10*pow(r,2)*a[3])*C1_ + 
						6*pow(b,5)*pow(r,5)*
						 (a[0] + r*a[1] + 
							 pow(r,2)*a[2] + 
							 pow(r,3)*a[3])*C1_ + 
						6*pow(b,4)*pow(r,4)*
						 (2*a[0] + 3*r*a[1] + 
							 4*pow(r,2)*a[2] + 
							 5*pow(r,3)*a[3])*C1_ + 
						12*pow(b,3)*pow(r,3)*
						 (a[0] + 3*r*a[1] + 
							 6*pow(r,2)*a[2] + 
							 10*pow(r,3)*a[3])*C1_ - 
						pow(b,6)*d*exp(b*r)*
						 (2*a[0] + 3*r*a[1] + 
							 6*pow(r,2)*a[2])*C2_ + 
						6*pow(b,6)*d*exp(b*r)*
						 pow(r,3)*a[3]*C2_*log(r))/
					(6.*pow(b,6)*exp(b*r)*
						pow(r,3)) - 
				 (720*pow(R,3)*a[3]*C1_ + 
						144*b*pow(R,3)*
						 (a[2] + 5*R*a[3])*C1_ + 
						36*pow(b,2)*pow(R,3)*
						 (a[1] + 4*R*a[2] + 
							 10*pow(R,2)*a[3])*C1_ + 
						6*pow(b,5)*pow(R,5)*
						 (a[0] + R*a[1] + 
							 pow(R,2)*a[2] + 
							 pow(R,3)*a[3])*C1_ + 
						6*pow(b,4)*pow(R,4)*
						 (2*a[0] + 3*R*a[1] + 
							 4*pow(R,2)*a[2] + 
							 5*pow(R,3)*a[3])*C1_ + 
						12*pow(b,3)*pow(R,3)*
						 (a[0] + 3*R*a[1] + 
							 6*pow(R,2)*a[2] + 
							 10*pow(R,3)*a[3])*C1_ - 
						pow(b,6)*d*exp(b*R)*
						 (2*a[0] + 3*R*a[1] + 
							 6*pow(R,2)*a[2])*C2_ + 
						6*pow(b,6)*d*exp(b*R)*
						 pow(R,3)*a[3]*C2_*log(R))/
					(6.*pow(b,6)*exp(b*R)*
						pow(R,3));

			Log.info() << "RDF::integralToRC(), ohne G: " << val << endl;

			return val;

		}
		else
		{

			// this is the case where we have to consider the geometry of the
			// situation. As this seems analytically impossible, we have to do it
			// numerically. The method we use is the trapezium method.

			// BAUSTELLE

			double area = 0;
			double x = r;
			unsigned int n = 10;
			double s = (R-r)/n;
			while (n > 0)
			{
				area += (rdf_(x) + rdf_(x+s))/2.0 * s;
				x += s;
				--n;
			}

			Log.info() << "RDF::integralToRC(), mit G: " << area << endl;

			return area;
		}
	}

} // namespace BALL
