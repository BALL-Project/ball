// $Id: pair6_12RDFIntegrator.C,v 1.1 2000/09/28 12:14:34 anker Exp $

#include <BALL/SOLVATION/pair6_12RDFIntegrator.h>

using namespace std;

namespace BALL
{

	const char* Pair6_12RDFIntegrator::Option::VERBOSITY = "verbosity";

	const int Pair6_12RDFIntegrator::Default::VERBOSITY = 0;

	Pair6_12RDFIntegrator::Pair6_12RDFIntegrator()
		: RDFIntegrator(),
			A_(0.0),
			B_(0.0),
			k1_(0.0),
			k2_(0.0)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	}


	Pair6_12RDFIntegrator::Pair6_12RDFIntegrator(const Pair6_12RDFIntegrator& integrator)
		:	RDFIntegrator(integrator),
			options(integrator.options),
			A_(integrator.A_),
			B_(integrator.B_),
			k1_(integrator.k1_),
			k2_(integrator.k2_)
	{
	}


	Pair6_12RDFIntegrator::Pair6_12RDFIntegrator(double A, double B,
			double k1, double k2, const RadialDistributionFunction& rdf)
		:	RDFIntegrator(rdf),
			A_(A),
			B_(B),
			k1_(k1),
			k2_(k2)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
	}


	Pair6_12RDFIntegrator::~Pair6_12RDFIntegrator()
	{
		destroy();
	}


	void Pair6_12RDFIntegrator::destroy()
	{
		clear();
	}


	void Pair6_12RDFIntegrator::clear()
	{
		A_ = 0.0;
		B_ = 0.0;
		k1_ = 0.0;
		k2_ = 0.0;
		// BAUSTELLE: options.clear() ?
		RDFIntegrator::clear();
	}


	void Pair6_12RDFIntegrator::set(const Pair6_12RDFIntegrator& integrator)
	{
		A_ = integrator.A_;
		B_ = integrator.B_;
		k1_ = integrator.k1_;
		k2_ = integrator.k2_;
		options = integrator.options;
		RDFIntegrator::set(integrator);
	}


	const Pair6_12RDFIntegrator& Pair6_12RDFIntegrator::operator =
		(const Pair6_12RDFIntegrator& integrator)
	{
		set(integrator);
		return *this;
	}


	void Pair6_12RDFIntegrator::setConstants(double A, double B, double k1, 
			double k2)
	{
		A_ = A;
		B_ = B;
		k1_ = k1;
		k2_ = k2;
	}


	double Pair6_12RDFIntegrator::integrateToInf(double from) const
	{

		PiecewisePolynomial poly = getRDF().getRepresentation();
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
			Log.error() << "Pair6_12RDFIntegrator::integrateToInf(): "
				<< "Last interval must have infinity as upper limit." << endl;
			getRDF().dump();
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
		double r3 = r * r * r;
		double infval = a[0]/9 * (A_ - 3 * B_ * r3 * r3) / (r3 * r3 * r3);
		val += infval;
		return val;
	}


	double Pair6_12RDFIntegrator::integrateToInf(double from, double A, 
			double B, double k1, double k2)
	{
		setConstants(A, B, k1, k2);
		return integrateToInf(from);
	}


	double Pair6_12RDFIntegrator::integrate(double from, double to) const
	{

		PiecewisePolynomial poly = getRDF().getRepresentation();

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
			return analyticallyIntegrateInterval(interval, coeffs, from_index);
		}

		// if we didn't return, the indices weren't equal.

		interval.second = poly.getInterval(from_index).second;
		double val = analyticallyIntegrateInterval(interval, coeffs, from_index);

		for (Size k = from_index + 1; k < to_index; ++k)
		{
			coeffs = poly.getCoefficients(k);
			interval = poly.getInterval(k);
			val += analyticallyIntegrateInterval(interval, coeffs, k);
		}

		coeffs = poly.getCoefficients(to_index);
		interval = poly.getInterval(to_index);
		interval.second = to;
		val += analyticallyIntegrateInterval(interval, coeffs, to_index);
		
		return val;
	}


	double Pair6_12RDFIntegrator::integrate(double from, double to, double A, 
			double B, double k1, double k2)
	{
		setConstants(A, B, k1, k2);
		return integrate(from, to);
	}


	double Pair6_12RDFIntegrator::operator () (double x) const
	{
		return integrateToInf(x);
	}


	double Pair6_12RDFIntegrator::analyticallyIntegrateInterval(Interval
			interval, Coefficients a, Position index) const
	{

		double s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, t0;
		double x0 = rdf_.getRepresentation().getInterval(index).first;
		double r = interval.first;
		double r9 = pow(r, 9);
		double R = interval.second;
		double R9 = pow(R, 9);

		s3 = -r9*B_*R*R*R*R*R*R*a[0]*x0/3.0-r9*A_*k1_*R*a[1]/8.0+3.0/8.0*r9*A_*k1_*R*a[2]*x0-r9*A_*R*R*a[1]/7.0+3.0/7.0*r9*A_*R*R*a[2]*x0-R9*B_*r*r*r*r*r*r*r*r*a[1]+R9*A_*a[1]*k2_/9.0-R9*A_*a[0]*x0/9.0+r9*B_*R*R*R*R*R*R*a[1]*k2_/3.0+R9*A_*r*r*a[1]/7.0-R9*B_*r*r*r*r*r*r*a[-1]/3.0-r9*B_*R*R*R*R*R*R*a[2]*x0*x0*x0/3.0+r9*B_*R*R*R*R*R*R*a[1]*x0*x0/3.0-R9*B_*r*r*r*r*r*r*a[1]*k2_/3.0+R9*B_*r*r*r*r*r*r*a[2]*x0*k2_+R9*B_*r*r*r*r*r*r*a[2]*x0*x0*x0/3.0-R9*B_*r*r*r*r*r*r*a[1]*x0*x0/3.0-3.0/7.0*R9*A_*r*r*a[2]*x0-3.0/8.0*R9*A_*k1_*r*a[2]*x0+R9*A_*k1_*r*a[1]/8.0;
		s2 = s3-3.0*r9*B_*R*R*R*R*R*R*R*R*a[2]*x0-3.0/2.0*r9*B_*k1_*R*R*R*R*R*R*R*a[2]*x0+r9*B_*k1_*R*R*R*R*R*R*R*a[1]/2.0-R9*B_*k1_*r*r*r*r*r*r*r*a[1]/2.0+3.0*R9*B_*r*r*r*r*r*r*r*r*a[2]*x0+3.0/2.0*R9*B_*k1_*r*r*r*r*r*r*r*a[2]*x0+R9*B_*r*r*r*r*r*r*a[0]*x0/3.0-r9*A_*a[1]*k2_/9.0+r9*A_*a[0]*x0/9.0+R9*A_*a[1]*x0*x0/9.0+r9*B_*R*R*R*R*R*R*a[-1]/3.0+r9*B_*R*R*R*R*R*R*R*R*a[1]-R9*A_*a[2]*x0*x0*x0/9.0-r9*A_*a[1]*x0*x0/9.0+r9*A_*a[2]*x0*x0*x0/9.0-r9*B_*R*R*R*R*R*R*a[2]*x0*k2_+r9*A_*a[2]*x0*k2_/3.0-R9*A_*a[2]*x0*k2_/3.0-r9*A_*a[-1]/9.0+R9*A_*a[-1]/9.0;
		s3 = 1/(R9)/(r9);
		s1 = s2*s3;
		s9 = 143.0/2048.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R-2651.0/6720.0*r9*A_*a[1]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R-143.0/896.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r-35.0/256.0*r9*A_*a[0]*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,13.0)*R9-r9*B_*a[0]*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,29.0))/8.0;
		s8 = s9-r9*A_*a[0]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))/9.0+3003.0/8192.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,19.0))-8.0/35.0*r9*A_*a[2]*x0*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R-32.0/315.0*r9*A_*a[1]*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R*R+4.0/21.0*R9*A_*a[1]*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r+8.0/105.0*R9*A_*a[0]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r;
		s9 = s8+1001.0/4096.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r*r+385.0/1024.0*r9*A_*a[0]*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R+1001.0/4096.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r+R9*B_*a[2]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r*r*r*r*r*r*r/8.0+35.0/256.0*r9*A_*a[1]*x0*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,23.0));
		s10 = s9-r9*B_*a[0]*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,16.0)*R9/4.0-1001.0/2048.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r+R9*B_*a[2]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,16.0)/16.0;
		s7 = s10+5.0/48.0*r9*A_*a[0]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R-715.0/4032.0*r9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R-r9*A_*a[2]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,35.0))/9.0-715.0/32768.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_;
		s9 = s7-35.0/256.0*r9*A_*a[1]*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R*R+16.0/105.0*r9*A_*a[1]*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R-125.0/2048.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r*r+3.0/4.0*r9*B_*a[2]*x0*x0*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*R9+65.0/224.0*R9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r;
		s10 = s9-105.0/128.0*r9*A_*a[1]*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,12.0)*R9-2431.0/4480.0*R9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r;
		s8 = s10-1573.0/2240.0*R9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r+143.0/896.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R+2145.0/16384.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R-15.0/512.0*R9*A_*a[2]*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,25.0));
		s9 = s8+1063.0/13440.0*R9*A_*a[2]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r*r-R9*B_*a[2]*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*r9/8.0-649.0/16384.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*r9+15.0/256.0*r9*A_*a[2]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R*R*R-2079.0/2048.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,11.0)*R9;
		s6 = s9-105.0/256.0*r9*A_*a[2]*x0*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R*R-r9*A_*a[2]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R*R/21.0+R9*B_*a[2]*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,33.0))-143.0/256.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R-r9*B_*a[0]*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*R9/8.0+65.0/672.0*R9*A_*a[0]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r+2.0/7.0*r9*A_*a[2]*x0*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R;
		s10 = -2871.0/8960.0*r9*A_*a[0]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R-35.0/256.0*R9*A_*a[1]*x0*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,23.0))-143.0/16384.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R;
		s9 = s10-r9*B_*a[2]*log(k1_+2.0*R+2.0*sqrt(R*R+k2_+k1_*R))*sqrt(pow(k2_,35.0))*R9-99.0/4096.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,11.0)*R9-715.0/16384.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,17.0));
		s8 = s9+2871.0/4480.0*r9*A_*a[1]*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R-3.0/8.0*r9*B_*a[2]*x0*x0*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,29.0))+715.0/32768.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,17.0))-3.0/8.0*r9*B_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*R9-1287.0/4096.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,10.0)-35.0/512.0*r9*A_*a[0]*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,23.0));
		s9 = s8-715.0/2688.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r-715.0/32768.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*R*R*R*R*R*R*R*R+125.0/2048.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R*R+671.0/8960.0*r9*A_*a[2]*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R+2871.0/8960.0*R9*A_*a[0]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r;
		s7 = s9+r9*B_*a[2]*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,31.0))/8.0-715.0/12288.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R-7293.0/8960.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R+41.0/672.0*r9*A_*a[2]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R*R-455.0/1024.0*r9*A_*a[0]*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*R9-187.0/2560.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R-143.0/32768.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*R9;
		s10 = 3.0/16.0*r9*B_*a[2]*x0*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,15.0)*R9-2717.0/16384.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*R9-1573.0/6720.0*R9*A_*a[0]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r;
		s9 = s10-r9*B_*a[2]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,16.0)*R9/16.0+715.0/16384.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R-715.0/32768.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*r9*sqrt(pow(k2_,17.0));
		s10 = s9+2.0/9.0*r9*A_*a[1]*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))+r9*B_*a[1]*x0*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,16.0)*R9/2.0;
		s8 = s10-385.0/2048.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R*R-35.0/256.0*r9*A_*a[2]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,13.0)*R9+2079.0/2048.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,11.0)-15.0/256.0*R9*A_*a[2]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r*r*r;
		s10 = s8-15.0/512.0*r9*A_*a[2]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R*R*R+1001.0/4096.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,19.0));
		s9 = s10+35.0/128.0*r9*A_*a[1]*x0*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,13.0)*R9-3.0/4.0*R9*B_*a[2]*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,17.0)+3.0/8.0*r9*B_*a[2]*x0*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R*R*R*R*R+R9*A_*a[2]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r/48.0;
		s5 = s9+315.0/256.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,12.0)*R9+1001.0/2048.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R-1063.0/13440.0*r9*A_*a[2]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R*R-429.0/512.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r+715.0/12288.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r+s6+s7;
		s9 = -65.0/224.0*r9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R+715.0/32768.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*R9-5.0/16.0*R9*A_*a[2]*x0*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r+3.0/4.0*R9*B_*a[2]*x0*x0*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,16.0)+2651.0/13440.0*r9*A_*a[0]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R;
		s10 = s9+715.0/32768.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*R9-715.0/8064.0*R9*A_*a[0]*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r;
		s8 = s10-1365.0/1024.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*R9+385.0/2048.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r*r+715.0/6144.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R+3.0/4.0*r9*B_*a[2]*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,17.0)*R9;
		s9 = s8+385.0/1024.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R*R-3.0/4.0*r9*B_*a[2]*x0*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R*R*R*R*R*R+2145.0/32768.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*R9-3003.0/4096.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R-105.0/256.0*R9*A_*a[2]*x0*x0*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*r9;
		s10 = s9+429.0/2048.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,10.0)+143.0/65536.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,10.0)*R9+r9*B_*a[2]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R*R*R*R*R*R*R/4.0;
		s7 = s10-R9*B_*a[1]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r*r*r*r*r*r/2.0-r9*B_*a[1]*x0*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*R9/2.0-1001.0/4096.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R-2145.0/32768.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*R*R*R*R*R*R*R*R;
		s10 = 35.0/512.0*r9*A_*a[0]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R*R+1771.0/4096.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*R9+1287.0/4096.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,10.0)*R9;
		s9 = s10-R9*B_*a[0]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,15.0)/16.0-385.0/1024.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,21.0))+1365.0/1024.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*r9;
		s8 = s9+649.0/16384.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*R9-1155.0/1024.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r-1155.0/2048.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R*R+r9*B_*a[1]*x0*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,29.0))/4.0+R9*A_*a[2]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,35.0))/9.0+1771.0/2048.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*r9;
		s9 = s8+143.0/12288.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R+1001.0/8192.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R*R-65.0/672.0*r9*A_*a[0]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R-4.0/21.0*r9*A_*a[1]*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R+253.0/4096.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R+105.0/256.0*r9*A_*a[0]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,12.0)*R9;
		s10 = s9+143.0/32768.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*r9-35.0/128.0*R9*A_*a[1]*x0*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,13.0)+16.0/105.0*r9*A_*a[2]*x0*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R*R;
		s6 = s10-2431.0/8960.0*r9*A_*a[0]*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R-715.0/8192.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R-189.0/2048.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,12.0)+s7;
		s9 = 105.0/256.0*r9*A_*a[2]*x0*x0*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*R9-8.0/315.0*r9*A_*a[2]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R*R*R-455.0/512.0*R9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*r9+r9*B_*a[2]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,35.0))*R*R*R*R*R*R/3.0-R9*B_*a[0]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r*r*r*r*r/8.0-2651.0/13440.0*R9*A_*a[0]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r;
		s8 = s9+2717.0/16384.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*r9-473.0/6720.0*r9*A_*a[2]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R+r9*B_*a[2]*x0*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R*R*R*R*R*R+r9*B_*a[1]*x0*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*R9/4.0+5.0/16.0*r9*A_*a[2]*x0*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R-3.0/4.0*r9*B_*a[2]*x0*x0*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,16.0)*R9;
		s9 = s8-125.0/1024.0*r9*A_*a[2]*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R*R-R9*B_*a[2]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r*r*r*r*r*r*r/4.0+5313.0/4096.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*R9-35.0/512.0*R9*A_*a[0]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r*r+R9*B_*a[0]*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,29.0))/8.0;
		s7 = s9-105.0/256.0*R9*A_*a[0]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,12.0)+s5+39.0/224.0*R9*A_*a[0]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r+35.0/256.0*R9*A_*a[0]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r*r-105.0/512.0*r9*A_*a[2]*x0*x0*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,23.0))-715.0/6144.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r+s6;
		s10 = -117.0/224.0*r9*A_*a[2]*x0*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R-r9*B_*a[1]*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,15.0)*R9/8.0+2431.0/4480.0*r9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R;
		s9 = s10-41.0/672.0*R9*A_*a[2]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r*r+143.0/1792.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r-105.0/256.0*r9*A_*a[2]*x0*x0*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,13.0)*R9;
		s10 = s9+R9*B_*a[1]*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,15.0)/8.0+R9*B_*a[0]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r*r*r*r*r*r/4.0-r9*A_*a[2]*x0*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))/3.0;
		s8 = s10-5.0/48.0*R9*A_*a[0]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r+R9*B_*a[1]*x0*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*r9/2.0+105.0/512.0*r9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R*R+35.0/512.0*R9*A_*a[0]*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,23.0));
		s9 = s8+15.0/512.0*r9*A_*a[2]*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,25.0))+2651.0/4480.0*r9*A_*a[2]*x0*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R-r9*B_*a[2]*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,33.0))+105.0/128.0*R9*A_*a[1]*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,12.0)+R9*A_*a[2]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r*r/21.0-143.0/8064.0*r9*A_*a[2]*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R;
		s4 = s9+r9*B_*a[2]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R*R*R*R*R*R*R*R+r9*B_*a[0]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,15.0)*R9/16.0+35.0/256.0*r9*A_*a[0]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*R9-143.0/10240.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R-385.0/512.0*r9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R+13.0/672.0*r9*A_*a[2]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R+s7;
		s9 = s4-39.0/224.0*r9*A_*a[0]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R+r9*B_*a[2]*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*R9/8.0-429.0/1792.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R+3.0/4.0*R9*B_*a[2]*x0*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r*r*r*r*r*r;
		s8 = s9-143.0/2048.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r-2717.0/8192.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*r9-R9*B_*a[2]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r*r*r*r*r*r*r*r+105.0/256.0*R9*A_*a[2]*x0*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r*r+715.0/2688.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R+715.0/16384.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*R*R*R*R*R*R*R*R;
		s9 = s8-2.0/9.0*R9*A_*a[1]*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))+2.0/3.0*R9*B_*a[1]*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r*r*r*r*r*r-3.0/16.0*R9*B_*a[2]*x0*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,15.0)-r9*B_*a[2]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R*R*R*R*R*R*R/8.0+1155.0/1024.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R*R;
		s10 = s9+429.0/4096.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,10.0)*R9-R9*B_*a[1]*x0*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,16.0)/2.0-1771.0/4096.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*r9;
		s7 = s10+5.0/4.0*R9*B_*a[2]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,33.0))*r9+503.0/4096.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*r9+429.0/2048.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R-1001.0/8192.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r*r;
		s9 = 35.0/128.0*R9*A_*a[1]*x0*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*r9-143.0/1024.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R-385.0/2048.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,21.0))-3.0/8.0*R9*B_*a[2]*x0*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r*r*r*r*r-R9*B_*a[0]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*r9/4.0+15.0/512.0*R9*A_*a[2]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r*r*r;
		s8 = s9-R9*B_*a[2]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,35.0))*r*r*r*r*r*r/3.0+8613.0/8960.0*R9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r-1001.0/4096.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,19.0))-r9*B_*a[0]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R*R*R*R*R*R/4.0+455.0/1024.0*R9*A_*a[0]*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*r9+1155.0/2048.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r*r;
		s10 = s8-R9*B_*a[1]*x0*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,29.0))/4.0+715.0/65536.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_;
		s9 = s10+143.0/32768.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,19.0))+1573.0/6720.0*r9*A_*a[0]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R-15.0/256.0*R9*A_*a[2]*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,14.0)+715.0/4032.0*R9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r;
		s6 = s9+99.0/4096.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,11.0)-143.0/8960.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r+715.0/16384.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,17.0))+3.0/8.0*R9*B_*a[2]*x0*x0*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,29.0))-13.0/672.0*R9*A_*a[2]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r-16.0/105.0*R9*A_*a[2]*x0*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r*r+s7;
		s9 = s6+429.0/512.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R+2431.0/8960.0*R9*A_*a[0]*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r+385.0/512.0*R9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r-35.0/256.0*R9*A_*a[0]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*r9-3003.0/8192.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,19.0));
		s8 = s9+125.0/2048.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,23.0))+r9*B_*a[0]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*R9/4.0+8.0/35.0*R9*A_*a[2]*x0*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r-39.0/112.0*R9*A_*a[1]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r+7293.0/8960.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r-143.0/65536.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,10.0);
		s9 = s8+1155.0/2048.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,21.0))-1001.0/4096.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R*R-35.0/128.0*R9*A_*a[1]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r*r+35.0/256.0*R9*A_*a[1]*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r*r-15.0/256.0*r9*A_*a[2]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,27.0))*R9;
		s7 = s9-65.0/336.0*R9*A_*a[1]*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r+R9*B_*a[1]*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r*r*r*r*r/4.0-35.0/256.0*r9*A_*a[0]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R*R-385.0/1024.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r*r+32.0/315.0*R9*A_*a[1]*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r*r-5.0/24.0*r9*A_*a[1]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R-3003.0/8192.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r*r;
		s9 = R9*B_*a[0]*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*r9/8.0-2871.0/4480.0*R9*A_*a[1]*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r-5313.0/4096.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*r9-715.0/4096.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R+1573.0/3360.0*R9*A_*a[1]*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r+1573.0/2240.0*r9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R;
		s10 = s9+15.0/256.0*R9*A_*a[2]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,27.0))*r9+R9*B_*a[2]*log(k1_+2.0*r+2.0*sqrt(r*r+k2_+k1_*r))*sqrt(pow(k2_,35.0))*r9;
		s8 = s10-693.0/1024.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,11.0)+r9*B_*a[0]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R*R*R*R*R*R/3.0-693.0/2048.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,11.0)*R9+385.0/1024.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,21.0));
		s9 = s8-R9*B_*a[2]*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,31.0))/8.0+r9*B_*a[1]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R*R*R*R*R*R/2.0+253.0/8192.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,21.0))-2651.0/4480.0*R9*A_*a[2]*x0*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r-2.0/3.0*r9*B_*a[1]*x0*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R*R*R*R*R*R+8.0/315.0*R9*A_*a[2]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r*r*r;
		s5 = s9+715.0/4096.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r-715.0/16384.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*r*r*r*r*r*r*r*r*sqrt(pow(k2_,17.0))-125.0/2048.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,23.0))-2145.0/32768.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*r9*sqrt(pow(k2_,17.0))-385.0/1024.0*R9*A_*a[0]*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r-2.0/7.0*R9*A_*a[2]*x0*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r+s7;
		s9 = s5-715.0/16384.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r+2651.0/6720.0*R9*A_*a[1]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r+715.0/32768.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*r*r*r*r*r*r*r*r*sqrt(pow(k2_,17.0))-35.0/128.0*r9*A_*a[1]*x0*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*R9;
		s8 = s9-715.0/32768.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,17.0))-715.0/16384.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*R9-429.0/2048.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r+715.0/8064.0*r9*A_*a[0]*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R-r9*A_*a[2]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*R/48.0+35.0/128.0*r9*A_*a[1]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R*R;
		s9 = s8+253.0/8192.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r*r+15.0/256.0*r9*A_*a[2]*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,14.0)*R9-2145.0/32768.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,17.0))-16.0/105.0*R9*A_*a[1]*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r-3.0/4.0*R9*B_*a[2]*x0*x0*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*r9;
		s10 = s9+105.0/256.0*R9*A_*a[2]*x0*x0*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,13.0)+2145.0/32768.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*r*r*r*r*r*r*r*r*sqrt(pow(k2_,17.0))+715.0/16384.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*r9*sqrt(pow(k2_,17.0));
		s7 = s10+143.0/1024.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r+143.0/16384.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r+455.0/512.0*r9*A_*a[1]*x0*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*R9+35.0/256.0*R9*A_*a[2]*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,13.0);
		s10 = -2145.0/16384.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r-429.0/4096.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,10.0)+143.0/256.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r;
		s9 = s10+3003.0/4096.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*r*r*r*r*r*r*r-1011.0/8960.0*R9*A_*a[2]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r*r+35.0/256.0*R9*A_*a[0]*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,13.0);
		s8 = s9+2717.0/8192.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*R9+105.0/512.0*R9*A_*a[2]*x0*x0*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,23.0))+693.0/2048.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,11.0)+1001.0/8192.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,19.0))-253.0/4096.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r+143.0/8960.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R;
		s9 = s8-1771.0/2048.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*R9-143.0/512.0*R9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r+143.0/10240.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r+125.0/1024.0*R9*A_*a[2]*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r*r+2145.0/32768.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,17.0))+39.0/112.0*r9*A_*a[1]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R;
		s6 = s9-671.0/8960.0*R9*A_*a[2]*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r+187.0/2560.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r*r*r+143.0/8064.0*R9*A_*a[2]*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r-5.0/4.0*r9*B_*a[2]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,33.0))*R9+117.0/224.0*R9*A_*a[2]*x0*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r-143.0/1792.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R+s7;
		s9 = s6-253.0/8192.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*R*R*R*R*R*R*R*R-4.0/105.0*R9*A_*a[2]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r*r*r-503.0/4096.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*R9-R9*B_*a[0]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r*r*r*r*r*r/3.0-r9*B_*a[1]*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R*R*R*R*R/4.0;
		s8 = s9-143.0/32768.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,19.0))+3.0/8.0*R9*B_*a[2]*x0*x0*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*r9+143.0/32768.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R*R+8151.0/16384.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*r9-R9*B_*a[1]*x0*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*r9/4.0-143.0/12288.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r;
		s9 = s8-105.0/512.0*R9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*r*r*r*r*r*r*r*r-1001.0/8192.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,19.0))-143.0/32768.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r*r+693.0/1024.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,11.0)*R9+r9*B_*a[0]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R*R*R*R*R/8.0;
		s10 = s9-715.0/65536.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*R9+143.0/512.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*R*R*R*R*R*R+2.0/21.0*r9*A_*a[0]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R;
		s7 = s10-2145.0/65536.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*R9-8151.0/16384.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*R9+473.0/6720.0*R9*A_*a[2]*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r*r-16.0/315.0*R9*A_*a[0]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r*r*r*r;
		s9 = s7+715.0/8192.0*R9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*r*r*r*r*r*r*r+429.0/1792.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*r*r*r*r+1011.0/8960.0*r9*A_*a[2]*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R*R-429.0/2048.0*r9*A_*a[1]*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,10.0)*R9+5.0/24.0*R9*A_*a[1]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r;
		s8 = s9+385.0/2048.0*r9*A_*a[0]*k1_*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*pow(R,10.0)*sqrt(pow(k2_,21.0))+R9*A_*a[0]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))/9.0-R9*B_*a[2]*x0*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*r*r*r*r*r*r-1155.0/2048.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,21.0))-253.0/8192.0*R9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*pow(r,10.0)*sqrt(pow(k2_,21.0))-2.0/21.0*R9*A_*a[0]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r+R9*A_*a[2]*x0*x0*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))/3.0;
		s10 = s8+2145.0/65536.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_*k2_-315.0/256.0*R9*A_*a[2]*x0*x0*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,12.0);
		s9 = s10-8.0/105.0*r9*A_*a[0]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R*R-155.0/1024.0*R9*A_*a[2]*k1_*k1_*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*r9+155.0/1024.0*r9*A_*a[2]*k1_*k1_*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*R9+16.0/315.0*r9*A_*a[0]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R*R*R;
		s10 = s9+4.0/105.0*r9*A_*a[2]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R*R*R+189.0/2048.0*r9*A_*a[2]*k1_*k1_*k1_*k1_*k1_*(log(1.0+(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2.0)/2.0)*pow(k2_,12.0)*R9+R9*B_*a[0]*k1_*(log(1.0+(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0-log(1.0-(2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2.0)/2.0)*r9*pow(k2_,16.0)/4.0;
		s3 = s10+65.0/336.0*r9*A_*a[1]*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R+3003.0/8192.0*r9*A_*a[2]*x0*x0*k1_*k1_*k1_*k1_*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*R*R*R*R*R*R*R*R-1573.0/3360.0*r9*A_*a[1]*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R*R-8613.0/8960.0*r9*A_*a[2]*x0*x0*k1_*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*R*R*R*R*R*R;
		s4 = 1/sqrt(pow(k2_,35.0))/(R9)/(r9);
		s2 = s3*s4;
		t0 = s1+s2;

		return t0;
	}


	void Pair6_12RDFIntegrator::dump(ostream& stream, Size /* depth */) const
	{
		stream << "[Pair6_12RDFIntegrator:]" << endl;
		stream << "A_ = " << A_ << endl;
		stream << "B_ = " << B_ << endl;
		stream << "k1_ = " << k1_ << endl;
		stream << "k2_ = " << k2_ << endl;
		getRDF().dump();
	}

} // namespace BALL
