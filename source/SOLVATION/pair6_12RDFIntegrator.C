// $Id: pair6_12RDFIntegrator.C,v 1.2 2000/09/28 17:32:03 anker Exp $

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
		double infval = a[0]/9 * (A_ - 3.0 * B_ * r3 *r3) / (r3 * r3 * r3);
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

		double FROM = project(from);
		double TO = project(to);
		
		Size from_index = poly.getIntervalIndex(FROM);
		Size to_index = poly.getIntervalIndex(TO);

		if ((from_index == INVALID_POSITION) || (to_index == INVALID_POSITION))
		{
			// no error message, because getIntervalIndex() handles this
			return 0.0;
		}

		Interval interval(from, to);
		Coefficients coeffs = poly.getCoefficients(from_index);

		if (from_index == to_index)
		{
			return analyticallyIntegrateInterval(interval, coeffs, from_index);
		}

		// if we didn't return, the indices weren't equal.

		interval.second = unproject(poly.getInterval(from_index).second);
		double val = analyticallyIntegrateInterval(interval, coeffs, from_index);
		Interval INTERVAL;

		for (Size k = from_index + 1; k < to_index; ++k)
		{
			coeffs = poly.getCoefficients(k);
			INTERVAL = poly.getInterval(k);
			interval.first = unproject(INTERVAL.first);
			interval.second = unproject(INTERVAL.second);
			val += analyticallyIntegrateInterval(interval, coeffs, k);
		}

		coeffs = poly.getCoefficients(to_index);
		INTERVAL = poly.getInterval(to_index);
		interval.first = unproject(INTERVAL.first);
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

		double k = rdf_.getRepresentation().getInterval(index).first;
		double r = interval.first;
		double R = interval.second;
		double val = 0.0;


		if (fabs(k2_) < 1e-6)
		{

			// no geometric correction of RDF argument needed.

			double s1 = (-56.0*A_*a[0]+56.0*A_*a[3]*k*k*k-56.0*A_*a[2]*k*k+56.0*A_*a[1]*k+504.0*B_*a[2]*pow(R,8.0)-84.0*A_*a[3]*R*R*R+252.0*B_*a[1]*pow(R,7.0)+168.0*B_*a[0]*pow(R,6.0)-189.0*A_*a[3]*k*k*R+168.0*B_*a[2]*k*k*pow(R,6.0)-72.0*A_*a[2]*R*R-504.0*B_*a[3]*log(R)*pow(R,9.0)+756.0*B_*a[3]*k*k*pow(R,7.0)-63.0*A_*a[1]*R-1512.0*B_*a[3]*k*pow(R,8.0)+216.0*A_*a[3]*k*R*R-504.0*B_*a[2]*k*pow(R,7.0)-168.0*B_*a[1]*k*pow(R,6.0)+126.0*A_*a[2]*k*R-168.0*B_*a[3]*k*k*k*pow(R,6.0))/pow(R,9.0)/504;
			double s2 = -(-189.0*A_*a[3]*k*k*r-56.0*A_*a[0]+56.0*A_*a[3]*k*k*k-56.0*A_*a[2]*k*k+56.0*A_*a[1]*k+252.0*B_*a[1]*pow(r,7.0)+756.0*B_*a[3]*k*k*pow(r,7.0)-504.0*B_*a[2]*k*pow(r,7.0)+168.0*B_*a[2]*k*k*pow(r,6.0)-1512.0*B_*a[3]*k*pow(r,8.0)-504.0*B_*a[3]*log(r)*pow(r,9.0)+504.0*B_*a[2]*pow(r,8.0)-72.0*A_*a[2]*r*r-63.0*A_*a[1]*r+168.0*B_*a[0]*pow(r,6.0)-168.0*B_*a[3]*k*k*k*pow(r,6.0)+126.0*A_*a[2]*k*r+216.0*A_*a[3]*k*r*r-84.0*A_*a[3]*r*r*r-168.0*B_*a[1]*k*pow(r,6.0))/pow(r,9.0)/504;
			val = s1+s2;
			return val;

		}	
		else
		{

			double s7 = A_*a[3]*pow(k,3.0)*sqrt(pow(k2_,35.0))/9-A_*a[2]*pow(k,2.0)*sqrt(pow(k2_,35.0))/9+A_*a[1]*k*sqrt(pow(k2_,35.0))/9+A_*a[3]*k*sqrt(pow(k2_,37.0))/3+11.0/4096.0*A_*a[3]*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,21.0))*pow(R,7.0)+429.0/512.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,6.0);
			double s6 = s7+385.0/1024.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,8.0)-A_*a[0]*sqrt(pow(k2_,35.0))/9+1573.0/2240.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,4.0)-A_*a[2]*sqrt(pow(k2_,37.0))/9+715.0/2688.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R+189.0/2048.0*A_*a[3]*pow(k1_,5.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12.0)*pow(R,9.0)+429.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,5.0);
			s7 = s6-2.0/3.0*B_*a[3]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*pow(R,10.0)+1155.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,10.0)+1771.0/4096.0*A_*a[1]*pow(k1_,5.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,9.0)-7293.0/8960.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,5.0)+935.0/49152.0*A_*a[3]*pow(k1_,7.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,9.0)-B_*a[1]*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*pow(R,9.0)/8;
			double s5 = s7+B_*a[2]*k1_*sqrt(pow(k2_,35.0))*pow(R,7.0)/2-3.0/2.0*B_*a[3]*k*k1_*sqrt(pow(k2_,35.0))*pow(R,7.0)+B_*a[2]*sqrt(pow(k2_,35.0))*pow(R,8.0)+1155.0/1024.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,7.0)+B_*a[3]*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*pow(R,9.0)/8-715.0/8192.0*A_*a[2]*k*pow(k1_,7.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,7.0)-143.0/32768.0*A_*a[3]*pow(k1_,9.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,9.0)-1001.0/8192.0*A_*a[1]*pow(k1_,6.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,10.0);
			s7 = s5+715.0/16384.0*A_*a[2]*k*pow(k1_,8.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*pow(R,8.0)+2145.0/32768.0*A_*a[3]*pow(k,2.0)*pow(k1_,8.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*pow(R,10.0)+B_*a[0]*sqrt(pow(k2_,35.0))*pow(R,6.0)/3-B_*a[1]*k*sqrt(pow(k2_,35.0))*pow(R,6.0)/3-3003.0/8192.0*A_*a[3]*pow(k,2.0)*pow(k1_,6.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,10.0)-B_*a[3]*k*sqrt(pow(k2_,37.0))*pow(R,6.0);
			s6 = s7-3.0*B_*a[3]*k*sqrt(pow(k2_,35.0))*pow(R,8.0)-1573.0/3360.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,4.0)+143.0/65536.0*A_*a[3]*pow(k1_,9.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10.0)*pow(R,9.0)+B_*a[2]*pow(k,2.0)*sqrt(pow(k2_,35.0))*pow(R,6.0)/3-715.0/12288.0*A_*a[1]*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,6.0)+2.0/21.0*A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R+3.0/7.0*A_*a[3]*k*sqrt(pow(k2_,35.0))*R*R;
			s7 = s6-A_*a[2]*k1_*sqrt(pow(k2_,35.0))*R/8-B_*a[3]*pow(k,3.0)*sqrt(pow(k2_,35.0))*pow(R,6.0)/3+105.0/256.0*A_*a[1]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12.0)*pow(R,9.0)+143.0/6144.0*A_*a[3]*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,23.0))*pow(R,5.0)+115.0/2048.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,21.0))*pow(R,8.0)-B_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,31.0))*pow(R,7.0)/12;
			double s4 = s7-5.0/4.0*B_*a[3]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,33.0))*pow(R,9.0)+715.0/6144.0*A_*a[2]*k*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,6.0)-693.0/2048.0*A_*a[1]*pow(k1_,5.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11.0)*pow(R,9.0)+B_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,10.0)/24-8.0/105.0*A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,4.0)+15.0/512.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,10.0)+4.0/63.0*A_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,31.0))*R*R-715.0/65536.0*A_*a[1]*pow(k1_,9.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,9.0)*pow(R,9.0);
			s7 = s4-3.0/4.0*B_*a[3]*pow(k,2.0)*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*pow(R,7.0)+455.0/512.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,9.0)+1001.0/2048.0*A_*a[2]*k*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,7.0)-5.0/256.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,25.0))*pow(R,6.0)-143.0/12288.0*A_*a[3]*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,21.0))*pow(R,6.0)+65.0/336.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R;
			s6 = s7+2431.0/4480.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,5.0)-715.0/16384.0*A_*a[2]*k*pow(k1_,8.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*pow(R,10.0)-35.0/256.0*A_*a[1]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13.0)*pow(R,9.0)+715.0/32768.0*A_*a[1]*pow(k1_,8.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*pow(R,10.0)-385.0/512.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,7.0)-143.0/1024.0*A_*a[2]*k*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,5.0)+2145.0/32768.0*A_*a[3]*pow(k,2.0)*pow(k1_,9.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*pow(R,9.0);
			s7 = s6+B_*a[3]*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*pow(R,10.0)/8+143.0/98304.0*A_*a[3]*pow(k1_,8.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,17.0))*pow(R,8.0)-35.0/256.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,8.0)+143.0/896.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,4.0)-5.0/24.0*A_*a[2]*k*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R-7.0/12.0*B_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*pow(R,9.0);
			s5 = s7-B_*a[3]*log(R+k1_/2+sqrt(R*R+k2_+k1_*R))*sqrt(pow(k2_,35.0))*pow(R,9.0)-2717.0/16384.0*A_*a[1]*pow(k1_,7.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,9.0)-15.0/256.0*A_*a[3]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,27.0))*pow(R,9.0)-5.0/256.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,9.0)-1001.0/4096.0*A_*a[2]*k*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,8.0)+B_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,33.0))*pow(R,6.0)/3+155.0/1024.0*A_*a[3]*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*pow(R,9.0)-35.0/256.0*A_*a[3]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13.0)*pow(R,9.0);
			s7 = s5-55.0/768.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,25.0))*pow(R,5.0)-715.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,6.0)+13.0/144.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,31.0))*R-143.0/98304.0*A_*a[3]*pow(k1_,9.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*pow(R,9.0)+B_*a[1]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,8.0)/8+15.0/512.0*A_*a[3]*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*pow(R,10.0);
			s6 = s7+143.0/2688.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,27.0))*R*R*R+3.0/8.0*A_*a[3]*k*k1_*sqrt(pow(k2_,35.0))*R+16.0/105.0*A_*a[3]*pow(k,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,6.0)+55.0/1536.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,23.0))*pow(R,6.0)+B_*a[2]*sqrt(pow(k2_,37.0))*pow(R,6.0)/3+2.0/7.0*A_*a[3]*pow(k,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R-105.0/256.0*A_*a[3]*pow(k,2.0)*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13.0)*pow(R,9.0);
			s7 = s6-2431.0/8960.0*A_*a[1]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,5.0)-1155.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,8.0)-143.0/256.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,6.0)-A_*a[2]*sqrt(pow(k2_,35.0))*R*R/7-2651.0/6720.0*A_*a[2]*k*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,5.0)+35.0/256.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,10.0)+B_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*pow(R,6.0)/3;
			double s3 = s7+35.0/128.0*A_*a[2]*k*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,7.0)+649.0/16384.0*A_*a[3]*pow(k1_,7.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,9.0)+385.0/2048.0*A_*a[1]*pow(k1_,4.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,10.0)+B_*a[3]*pow(k,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*pow(R,6.0)+715.0/8064.0*A_*a[1]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R-3003.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,7.0)-A_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,33.0))/9-143.0/1792.0*A_*a[1]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,4.0);
			s7 = s3+143.0/2048.0*A_*a[1]*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,5.0)-39.0/224.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R+5.0/128.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,27.0))*pow(R,5.0)-B_*a[3]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16.0)*pow(R,9.0)/16-2145.0/32768.0*A_*a[3]*pow(k,2.0)*pow(k1_,8.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*pow(R,8.0);
			s6 = s7-143.0/32768.0*A_*a[3]*pow(k1_,8.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,10.0)-715.0/32768.0*A_*a[1]*pow(k1_,8.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*pow(R,8.0)+2651.0/13440.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,5.0)-B_*a[3]*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,33.0))*pow(R,10.0)+3.0/8.0*B_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,8.0)+35.0/256.0*A_*a[1]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*pow(R,9.0)+539.0/24576.0*A_*a[3]*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,10.0);
			s7 = s6-3.0/8.0*B_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*pow(R,9.0)+5.0/256.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,25.0))*pow(R,7.0)-2.0/3.0*B_*a[2]*k*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))*pow(R,6.0)-143.0/2016.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,29.0))*R*R-B_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*pow(R,7.0)/4-125.0/2048.0*A_*a[3]*pow(k1_,4.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,10.0);
			double s8 = s7+B_*a[2]*k*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*pow(R,9.0)/4-105.0/128.0*A_*a[2]*k*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12.0)*pow(R,9.0)+B_*a[2]*k*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16.0)*pow(R,9.0)/2;
			s5 = s8+693.0/1024.0*A_*a[2]*k*pow(k1_,5.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11.0)*pow(R,9.0)+715.0/32768.0*A_*a[2]*k*pow(k1_,9.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,9.0)*pow(R,9.0)-65.0/224.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R-715.0/4032.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*R*R*R-95.0/3072.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,23.0))*pow(R,7.0);
			s7 = s5-429.0/2048.0*A_*a[2]*k*pow(k1_,7.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10.0)*pow(R,9.0)-B_*a[2]*k*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,15.0)*pow(R,9.0)/8+209.0/2240.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,27.0))*pow(R,4.0)+B_*a[2]*k*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*pow(R,7.0)/2-385.0/2048.0*A_*a[1]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,8.0)+1287.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,7.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10.0)*pow(R,9.0);
			s6 = s7-8613.0/8960.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,6.0)+35.0/512.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,8.0)-A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))/9+15.0/256.0*A_*a[3]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,14.0)*pow(R,9.0)+2145.0/16384.0*A_*a[3]*pow(k,2.0)*pow(k1_,7.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,7.0)-3.0/4.0*B_*a[3]*pow(k,2.0)*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16.0)*pow(R,9.0)+253.0/8192.0*A_*a[3]*pow(k1_,6.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,10.0);
			s7 = s6-99.0/4096.0*A_*a[3]*pow(k1_,7.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11.0)*pow(R,9.0)-143.0/98304.0*A_*a[3]*pow(k1_,8.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,17.0))*pow(R,10.0)-35.0/512.0*A_*a[1]*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,10.0)-539.0/24576.0*A_*a[3]*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,19.0))*pow(R,8.0)-455.0/1024.0*A_*a[1]*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,9.0)+3.0/16.0*B_*a[3]*pow(k,2.0)*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,15.0)*pow(R,9.0)+3.0/4.0*B_*a[3]*pow(k,2.0)*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*pow(R,9.0);
			s4 = s7+429.0/4096.0*A_*a[1]*pow(k1_,7.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10.0)*pow(R,9.0)+105.0/256.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*pow(R,9.0)+2.0/9.0*A_*a[2]*k*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))-35.0/256.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,7.0)-8151.0/16384.0*A_*a[3]*pow(k,2.0)*pow(k1_,7.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,9.0)-A_*a[3]*pow(k,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,33.0))/3-1365.0/1024.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,9.0)+5313.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,9.0);
			s7 = s4-4.0/21.0*A_*a[2]*k*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R*R-32.0/315.0*A_*a[2]*k*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,6.0)+35.0/128.0*A_*a[2]*k*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13.0)*pow(R,9.0)-143.0/3840.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,25.0))*pow(R,4.0)-B_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,29.0))*pow(R,8.0)/24-117.0/224.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R;
			s6 = s7-8.0/35.0*A_*a[3]*pow(k,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,4.0)+105.0/512.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,8.0)+3003.0/8192.0*A_*a[3]*pow(k,2.0)*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,8.0)+B_*a[1]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*pow(R,9.0)/4-B_*a[1]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16.0)*pow(R,9.0)/4+185.0/3072.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,9.0)-3.0/8.0*B_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*pow(R,10.0);
			s7 = s6+16.0/105.0*A_*a[2]*k*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,4.0)+3.0/4.0*B_*a[3]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,17.0)*pow(R,9.0)+143.0/49152.0*A_*a[3]*pow(k1_,7.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,19.0))*pow(R,7.0)+315.0/256.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12.0)*pow(R,9.0)-115.0/2048.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,10.0)+39.0/112.0*A_*a[2]*k*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R*R;
			s5 = s7+B_*a[2]*k*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*pow(R,10.0)/4-715.0/16384.0*A_*a[2]*k*pow(k1_,9.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*pow(R,9.0)-35.0/128.0*A_*a[2]*k*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25.0))*pow(R,9.0)+143.0/512.0*A_*a[1]*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,6.0)+1001.0/8192.0*A_*a[1]*pow(k1_,6.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,8.0)-1001.0/4096.0*A_*a[1]*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,7.0)+1573.0/6720.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,4.0)+16.0/315.0*A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,6.0);
			s7 = s5-65.0/672.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*R*R-2871.0/8960.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,6.0)+B_*a[3]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,9.0)/24-385.0/1024.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,10.0)+715.0/32768.0*A_*a[1]*pow(k1_,9.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17.0))*pow(R,9.0)+2651.0/4480.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,27.0))*pow(R,5.0);
			s6 = s7-1771.0/2048.0*A_*a[2]*k*pow(k1_,5.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21.0))*pow(R,9.0)-503.0/4096.0*A_*a[3]*pow(k1_,5.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,9.0)-241.0/4096.0*A_*a[3]*pow(k1_,5.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,21.0))*pow(R,9.0)+385.0/1024.0*A_*a[1]*pow(k1_,3.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,23.0))*pow(R,7.0)+5.0/48.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R+1001.0/4096.0*A_*a[2]*k*pow(k1_,6.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,10.0)-187.0/2016.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,29.0))*R*R*R-15.0/512.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,23.0))*pow(R,8.0);
			s7 = s6+2717.0/8192.0*A_*a[2]*k*pow(k1_,7.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19.0))*pow(R,9.0)-B_*a[1]*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29.0))*pow(R,10.0)/8-2079.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11.0)*pow(R,9.0)-8.0/315.0*A_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,29.0))*pow(R,4.0)-105.0/256.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,7.0)-B_*a[2]*k*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,29.0))*pow(R,8.0)/4+2871.0/4480.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,6.0);
			double s2 = s7-105.0/512.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23.0))*pow(R,10.0)+5.0/16.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,31.0))*R+715.0/16384.0*A_*a[1]*pow(k1_,7.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,19.0))*pow(R,7.0)-2145.0/65536.0*A_*a[3]*pow(k,2.0)*pow(k1_,9.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,9.0)*pow(R,9.0)+2.0/3.0*B_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5.0))*sqrt(pow(k2_,31.0))*pow(R,8.0)-429.0/1792.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(pow(R*R+k2_+k1_*R,3.0))*sqrt(pow(k2_,25.0))*pow(R,4.0)+B_*a[1]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,15.0)*pow(R,9.0)/16-B_*a[2]*k*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31.0))*pow(R,9.0)/2;
			s3 = 1/pow(R,9.0)/sqrt(pow(k2_,35.0));
			double s1 = s2*s3;
			s7 = -B_*a[0]*sqrt(pow(k2_,35.0))*pow(r,6.0)/3-A_*a[3]*pow(k,3.0)*sqrt(pow(k2_,35.0))/9+A_*a[2]*pow(k,2.0)*sqrt(pow(k2_,35.0))/9-A_*a[1]*k*sqrt(pow(k2_,35.0))/9-A_*a[3]*k*sqrt(pow(k2_,37.0))/3-2.0/3.0*B_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,31.0))*pow(r,8.0)-3.0/8.0*A_*a[3]*k*k1_*sqrt(pow(k2_,35.0))*r+3.0/2.0*B_*a[3]*k*k1_*sqrt(pow(k2_,35.0))*pow(r,7.0)-2.0/7.0*A_*a[3]*pow(k,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r-55.0/1536.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,23.0))*pow(r,6.0)+A_*a[0]*sqrt(pow(k2_,35.0))/9+A_*a[2]*sqrt(pow(k2_,37.0))/9-4.0/63.0*A_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,31.0))*r*r;
			s8 = s7+B_*a[3]*k*sqrt(pow(k2_,37.0))*pow(r,6.0)+429.0/2048.0*A_*a[2]*k*pow(k1_,7.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10.0)*pow(r,9.0)-2145.0/32768.0*A_*a[3]*pow(k,2.0)*pow(k1_,9.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17.0))*pow(r,9.0)-143.0/49152.0*A_*a[3]*pow(k1_,7.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,19.0))*pow(r,7.0)-16.0/105.0*A_*a[3]*pow(k,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,6.0)-2651.0/4480.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,5.0);
			s6 = s8-B_*a[2]*sqrt(pow(k2_,35.0))*pow(r,8.0)-1573.0/6720.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,4.0)+2.0/3.0*B_*a[2]*k*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*pow(r,6.0)+385.0/1024.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,10.0)+A_*a[2]*sqrt(pow(k2_,35.0))*r*r/7-B_*a[3]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,9.0)/24+1771.0/2048.0*A_*a[2]*k*pow(k1_,5.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,9.0)+B_*a[3]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16.0)*pow(r,9.0)/16;
			s8 = s6-15.0/512.0*A_*a[3]*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*pow(r,10.0)-5.0/256.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,25.0))*pow(r,7.0)-15.0/512.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,10.0)+143.0/32768.0*A_*a[3]*pow(k1_,8.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,10.0)+143.0/2016.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,29.0))*r*r+15.0/512.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,23.0))*pow(r,8.0);
			s7 = s8+187.0/2016.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,29.0))*r*r*r-649.0/16384.0*A_*a[3]*pow(k1_,7.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,9.0)-3.0/7.0*A_*a[3]*k*sqrt(pow(k2_,35.0))*r*r-3.0/16.0*B_*a[3]*pow(k,2.0)*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,15.0)*pow(r,9.0)+3.0/8.0*B_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*pow(r,10.0)+5.0/256.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,25.0))*pow(r,6.0)+B_*a[2]*k*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,8.0)/4;
			s8 = s7+A_*a[2]*k1_*sqrt(pow(k2_,35.0))*r/8+95.0/3072.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,23.0))*pow(r,7.0)-253.0/8192.0*A_*a[3]*pow(k1_,6.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,10.0)-3.0/4.0*B_*a[3]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,17.0)*pow(r,9.0)-3.0/8.0*B_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,8.0)-5.0/128.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,27.0))*pow(r,5.0);
			s5 = s8-539.0/24576.0*A_*a[3]*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,10.0)+5.0/4.0*B_*a[3]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,33.0))*pow(r,9.0)-B_*a[3]*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*pow(r,10.0)/8+125.0/2048.0*A_*a[3]*pow(k1_,4.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,10.0)+2.0/3.0*B_*a[3]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*pow(r,10.0)-B_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,10.0)/24+55.0/768.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,25.0))*pow(r,5.0)+3.0/4.0*B_*a[3]*pow(k,2.0)*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*pow(r,7.0);
			s8 = s5-115.0/2048.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,21.0))*pow(r,8.0)+115.0/2048.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,10.0)+5.0/256.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,9.0)-13.0/144.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,31.0))*r-209.0/2240.0*A_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,27.0))*pow(r,4.0)-143.0/2688.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,27.0))*r*r*r;
			s7 = s8-B_*a[3]*pow(k,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*pow(r,6.0)-143.0/98304.0*A_*a[3]*pow(k1_,8.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,17.0))*pow(r,8.0)-15.0/256.0*A_*a[3]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,14.0)*pow(r,9.0)+B_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,31.0))*pow(r,7.0)/12+B_*a[3]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,29.0))*pow(r,8.0)/24-35.0/128.0*A_*a[2]*k*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13.0)*pow(r,9.0)+32.0/315.0*A_*a[2]*k*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,6.0);
			s8 = s7-B_*a[2]*k*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*pow(r,10.0)/4-B_*a[2]*k*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16.0)*pow(r,9.0)/2-B_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,33.0))*pow(r,6.0)/3+B_*a[2]*k*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,15.0)*pow(r,9.0)/8+B_*a[2]*k*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*pow(r,9.0)/2-B_*a[2]*k*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*pow(r,9.0)/4;
			s6 = s8-B_*a[3]*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*pow(r,9.0)/8-B_*a[2]*k*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*pow(r,7.0)/2+15.0/256.0*A_*a[3]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,27.0))*pow(r,9.0)-143.0/6144.0*A_*a[3]*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,23.0))*pow(r,5.0)+143.0/3840.0*A_*a[3]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,25.0))*pow(r,4.0)-16.0/105.0*A_*a[2]*k*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,4.0)-B_*a[1]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,8.0)/8+B_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*pow(r,7.0)/4;
			s8 = s6-693.0/1024.0*A_*a[2]*k*pow(k1_,5.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11.0)*pow(r,9.0)-3.0/4.0*B_*a[3]*pow(k,2.0)*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*pow(r,9.0)+8.0/315.0*A_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,29.0))*pow(r,4.0)+143.0/32768.0*A_*a[3]*pow(k1_,9.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,9.0)-715.0/32768.0*A_*a[2]*k*pow(k1_,9.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,9.0)*pow(r,9.0)-155.0/1024.0*A_*a[3]*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*pow(r,9.0);
			s7 = s8-35.0/256.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,10.0)-2717.0/8192.0*A_*a[2]*k*pow(k1_,7.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,9.0)+5.0/24.0*A_*a[2]*k*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r+35.0/256.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,8.0)+A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))/9-B_*a[1]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31.0))*pow(r,9.0)/4+B_*a[1]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16.0)*pow(r,9.0)/4;
			s8 = s7-2145.0/32768.0*A_*a[3]*pow(k,2.0)*pow(k1_,8.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17.0))*pow(r,10.0)-715.0/6144.0*A_*a[2]*k*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,6.0)+B_*a[1]*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*pow(r,10.0)/8-35.0/128.0*A_*a[2]*k*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,7.0)+A_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,33.0))/9+2651.0/6720.0*A_*a[2]*k*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,5.0)-39.0/112.0*A_*a[2]*k*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r;
			s4 = s8+3003.0/8192.0*A_*a[3]*pow(k,2.0)*pow(k1_,6.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,10.0)-715.0/32768.0*A_*a[1]*pow(k1_,8.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17.0))*pow(r,10.0)+3003.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,7.0)-429.0/512.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,6.0)-1155.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,10.0)-35.0/256.0*A_*a[1]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*pow(r,9.0)-715.0/32768.0*A_*a[1]*pow(k1_,9.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17.0))*pow(r,9.0)+65.0/672.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r;
			s8 = s4+7293.0/8960.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,5.0)-105.0/512.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,8.0)-3003.0/8192.0*A_*a[3]*pow(k,2.0)*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,8.0)-2145.0/16384.0*A_*a[3]*pow(k,2.0)*pow(k1_,7.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,7.0)+2871.0/8960.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,6.0);
			s7 = s8+429.0/1792.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,4.0)-385.0/1024.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,8.0)-B_*a[2]*pow(k,2.0)*sqrt(pow(k2_,35.0))*pow(r,6.0)/3+3.0*B_*a[3]*k*sqrt(pow(k2_,35.0))*pow(r,8.0)+B_*a[3]*pow(k,3.0)*sqrt(pow(k2_,35.0))*pow(r,6.0)/3-143.0/896.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,4.0)+715.0/4032.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r;
			s8 = s7+8.0/105.0*A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,4.0)-185.0/3072.0*A_*a[3]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,9.0)+1365.0/1024.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,9.0)-105.0/256.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*pow(r,9.0)-935.0/49152.0*A_*a[3]*pow(k1_,7.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,9.0)+B_*a[3]*log(r+k1_/2+sqrt(r*r+k2_+k1_*r))*sqrt(pow(k2_,35.0))*pow(r,9.0);
			s6 = s8+1001.0/8192.0*A_*a[1]*pow(k1_,6.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,10.0)-385.0/2048.0*A_*a[1]*pow(k1_,4.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,10.0)-143.0/2048.0*A_*a[1]*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,5.0)+715.0/12288.0*A_*a[1]*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,6.0)-5313.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,9.0)-143.0/512.0*A_*a[1]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,6.0)+143.0/1792.0*A_*a[1]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,4.0)+2431.0/8960.0*A_*a[1]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,5.0);
			s8 = s6-1771.0/4096.0*A_*a[1]*pow(k1_,5.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21.0))*pow(r,9.0)-B_*a[2]*k1_*sqrt(pow(k2_,35.0))*pow(r,7.0)/2+1001.0/4096.0*A_*a[1]*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,7.0)+715.0/32768.0*A_*a[1]*pow(k1_,8.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,17.0))*pow(r,8.0)-65.0/336.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r-105.0/256.0*A_*a[1]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12.0)*pow(r,9.0);
			s7 = s8-429.0/4096.0*A_*a[1]*pow(k1_,7.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10.0)*pow(r,9.0)+143.0/1024.0*A_*a[2]*k*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,5.0)+715.0/65536.0*A_*a[1]*pow(k1_,9.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,9.0)*pow(r,9.0)+693.0/2048.0*A_*a[1]*pow(k1_,5.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11.0)*pow(r,9.0)+2717.0/16384.0*A_*a[1]*pow(k1_,7.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,9.0)-2651.0/13440.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,5.0)+35.0/256.0*A_*a[1]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13.0)*pow(r,9.0);
			s8 = s7-16.0/315.0*A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,6.0)+117.0/224.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r+385.0/2048.0*A_*a[1]*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,8.0)-35.0/512.0*A_*a[1]*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,8.0)+39.0/224.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r*r+35.0/512.0*A_*a[1]*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,10.0)-1287.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,7.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10.0)*pow(r,9.0);
			double s9 = s8+105.0/512.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,10.0)+105.0/256.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,7.0)-455.0/512.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,9.0);
			s5 = s9+105.0/256.0*A_*a[3]*pow(k,2.0)*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13.0)*pow(r,9.0)+2145.0/65536.0*A_*a[3]*pow(k,2.0)*pow(k1_,9.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,9.0)*pow(r,9.0)+1573.0/3360.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,4.0)+715.0/8192.0*A_*a[2]*k*pow(k1_,7.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,7.0)+715.0/16384.0*A_*a[2]*k*pow(k1_,9.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17.0))*pow(r,9.0);
			s8 = s5-1001.0/4096.0*A_*a[2]*k*pow(k1_,6.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,10.0)-1001.0/2048.0*A_*a[2]*k*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,7.0)+143.0/256.0*A_*a[2]*k*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,6.0)+1001.0/4096.0*A_*a[2]*k*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,8.0)-2.0/21.0*A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r+715.0/16384.0*A_*a[2]*k*pow(k1_,8.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17.0))*pow(r,10.0);
			s7 = s8-715.0/16384.0*A_*a[1]*pow(k1_,7.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,7.0)-2431.0/4480.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,5.0)+3.0/8.0*B_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*pow(r,9.0)+143.0/98304.0*A_*a[3]*pow(k1_,8.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,17.0))*pow(r,10.0)-5.0/16.0*A_*a[3]*pow(k,2.0)*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r-715.0/16384.0*A_*a[2]*k*pow(k1_,8.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,17.0))*pow(r,8.0)+385.0/512.0*A_*a[2]*k*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,7.0);
			s8 = s7-429.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,5.0)-1155.0/1024.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,7.0)+1155.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,4.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,8.0)-715.0/2688.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r-315.0/256.0*A_*a[3]*pow(k,2.0)*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12.0)*pow(r,9.0)+2145.0/32768.0*A_*a[3]*pow(k,2.0)*pow(k1_,8.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,17.0))*pow(r,8.0);
			s6 = s8+8613.0/8960.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,6.0)-1573.0/2240.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*pow(r,4.0)+455.0/1024.0*A_*a[1]*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,9.0)+99.0/4096.0*A_*a[3]*pow(k1_,7.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11.0)*pow(r,9.0)+A_*a[3]*pow(k,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))/3-2.0/9.0*A_*a[2]*k*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))+143.0/98304.0*A_*a[3]*pow(k1_,9.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,17.0))*pow(r,9.0)-B_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,33.0))*pow(r,6.0)/3;
			s8 = s6-11.0/4096.0*A_*a[3]*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,21.0))*pow(r,7.0)+539.0/24576.0*A_*a[3]*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,19.0))*pow(r,8.0)+B_*a[1]*pow(k1_,3.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29.0))*pow(r,9.0)/8-B_*a[1]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,15.0)*pow(r,9.0)/16-143.0/65536.0*A_*a[3]*pow(k1_,9.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10.0)*pow(r,9.0)+B_*a[3]*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,33.0))*pow(r,10.0);
			s7 = s8+8151.0/16384.0*A_*a[3]*pow(k,2.0)*pow(k1_,7.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19.0))*pow(r,9.0)-385.0/1024.0*A_*a[1]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,23.0))*pow(r,7.0)+8.0/35.0*A_*a[3]*pow(k,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*pow(r,4.0)+35.0/256.0*A_*a[3]*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13.0)*pow(r,9.0)-1001.0/8192.0*A_*a[1]*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,19.0))*pow(r,8.0)+715.0/4096.0*A_*a[3]*pow(k,2.0)*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,6.0)+143.0/12288.0*A_*a[3]*pow(k1_,6.0)*sqrt(pow(r*r+k2_+k1_*r,5.0))*sqrt(pow(k2_,21.0))*pow(r,6.0)+35.0/256.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,7.0);
			s8 = s7+7.0/12.0*B_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*pow(r,9.0)+3.0/4.0*B_*a[3]*pow(k,2.0)*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16.0)*pow(r,9.0)+105.0/128.0*A_*a[2]*k*pow(k1_,3.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12.0)*pow(r,9.0)-715.0/8064.0*A_*a[1]*pow(k1_,3.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,27.0))*r*r*r+35.0/128.0*A_*a[2]*k*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25.0))*pow(r,9.0)+65.0/224.0*A_*a[3]*pow(k,2.0)*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,29.0))*r*r+2079.0/2048.0*A_*a[3]*pow(k,2.0)*pow(k1_,5.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11.0)*pow(r,9.0);
			s3 = s8-B_*a[2]*sqrt(pow(k2_,37.0))*pow(r,6.0)/3-189.0/2048.0*A_*a[3]*pow(k1_,5.0)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12.0)*pow(r,9.0)-5.0/48.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r+4.0/21.0*A_*a[2]*k*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,31.0))*r*r+B_*a[1]*k*sqrt(pow(k2_,35.0))*pow(r,6.0)/3-2871.0/4480.0*A_*a[2]*k*pow(k1_,2.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,25.0))*pow(r,6.0)+503.0/4096.0*A_*a[3]*pow(k1_,5.0)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23.0))*pow(r,9.0)+241.0/4096.0*A_*a[3]*pow(k1_,5.0)*sqrt(pow(r*r+k2_+k1_*r,3.0))*sqrt(pow(k2_,21.0))*pow(r,9.0);
			s4 = 1/sqrt(pow(k2_,35.0))/pow(r,9.0);
			s2 = s3*s4;
			val = s1+s2;
			return val;

		}
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


	double Pair6_12RDFIntegrator::project(double x) const
	{
		return sqrt(x*x + k1_ * x + k2_);
	}


	double Pair6_12RDFIntegrator::unproject(double x) const
	{
		return sqrt(x*x - k1_*k1_ / 4 - k2_) - k1_ / 2;
	}


} // namespace BALL
