// $Id: pair6_12RDFIntegrator.C,v 1.12 2001/05/17 01:30:57 oliver Exp $

#include <BALL/MATHS/common.h>
#include <BALL/SOLVATION/pair6_12RDFIntegrator.h>

using namespace std;

namespace BALL
{

	// BAUSTELLE
	float MIN_DISTANCE = 1e-6;

	const char* Pair6_12RDFIntegrator::Option::VERBOSITY = "verbosity";
	const char* Pair6_12RDFIntegrator::Option::METHOD = "integration_method";
	const char* Pair6_12RDFIntegrator::Option::SAMPLES = "samples";

	const int Pair6_12RDFIntegrator::Default::VERBOSITY = 0;
	const int Pair6_12RDFIntegrator::Default::METHOD = METHOD__ANALYTICAL;
	const int Pair6_12RDFIntegrator::Default::SAMPLES = 30;

	Pair6_12RDFIntegrator::Pair6_12RDFIntegrator() throw()
		: RDFIntegrator(),
			A_(0.0),
			B_(0.0),
			k1_(0.0),
			k2_(0.0)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultInteger(Option::METHOD, Default::METHOD);
		options.setDefaultInteger(Option::SAMPLES, Default::SAMPLES);
	}


	Pair6_12RDFIntegrator::Pair6_12RDFIntegrator(const Pair6_12RDFIntegrator&
	integrator) throw()
		:	RDFIntegrator(integrator),
			options(integrator.options),
			A_(integrator.A_),
			B_(integrator.B_),
			k1_(integrator.k1_),
			k2_(integrator.k2_)
	{
	}


	Pair6_12RDFIntegrator::Pair6_12RDFIntegrator(double A, double B,
			double k1, double k2, const RadialDistributionFunction& rdf) throw()
		:	RDFIntegrator(rdf),
			A_(A),
			B_(B),
			k1_(k1),
			k2_(k2)
	{
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultInteger(Option::METHOD, Default::METHOD);
		options.setDefaultInteger(Option::SAMPLES, Default::SAMPLES);
	}


	Pair6_12RDFIntegrator::~Pair6_12RDFIntegrator() throw()
	{
		clear();

		valid_ = false;
	}


	void Pair6_12RDFIntegrator::clear() throw()
	{
		A_ = 0.0;
		B_ = 0.0;
		k1_ = 0.0;
		k2_ = 0.0;
		// BAUSTELLE: options.clear() ?
		options.clear(); 
		RDFIntegrator::clear();
	}


	const Pair6_12RDFIntegrator& Pair6_12RDFIntegrator::operator =
		(const Pair6_12RDFIntegrator& integrator) throw()
	{
		A_ = integrator.A_;
		B_ = integrator.B_;
		k1_ = integrator.k1_;
		k2_ = integrator.k2_;
		options = integrator.options;
		RDFIntegrator::operator = (integrator);

		return *this;
	}


	void Pair6_12RDFIntegrator::setConstants(double A, double B, double k1, 
			double k2) throw()
	{
		A_ = A;
		B_ = B;
		k1_ = k1;
		k2_ = k2;
	}


	void Pair6_12RDFIntegrator::getConstants(double& A, double& B, double& k1, 
			double& k2) 
		throw()
	{
		A = A_;
		B = B_;
		k1 = k1_;
		k2 = k2_;
	}


	double Pair6_12RDFIntegrator::integrateToInf(double from) 
		const throw()
	{

		Index verbosity =
			(Index)options.getInteger(Pair6_12RDFIntegrator::Option::VERBOSITY);
		Index method = options.getInteger(Pair6_12RDFIntegrator::Option::METHOD);

		if (method == METHOD__UNKNOWN)
		{
			Log.warn() << "Unknown integration method, defaulting to analytical."
				<< endl;
			method = METHOD__ANALYTICAL;
		}

		PiecewisePolynomial poly = getRDF().getRepresentation();
		Interval interval;
		double val = 0.0;
		double lower_inf; 
		
		// now build the interval we want to integrate
		Size number_of_intervals = poly.getIntervals().size();
		interval = poly.getInterval(number_of_intervals - 1);

		// the last interval has to be defined to infinity
		if (interval.second != INFINITY)
		{
			// BAUSTELLE: Sollte hier eine Exception geworfen werden?
			Log.error() << "Pair6_12RDFIntegrator::integrateToInf(): "
				<< "Last interval must have infinity as upper limit." << endl;
			getRDF().dump();
			return 0.0;
		}

		// k2_ is always > 0, so we don't have to fabs() here
		if (k2_ < MIN_DISTANCE)
		{

			// the point from where the integration to inf will start.
			lower_inf = interval.first;

		}
		else
		{

			// the point from where the integration to inf will start. As interval
			// is an interval of the RDF we have to project it to the integration
			// beam
			lower_inf = unproject(interval.first);

		}

		// first compute the integral addends with limits < infinity

		if (from < lower_inf)
		{
			interval = Interval(from, lower_inf);
			val = integrate(from, interval.second);
		}
		else 
		{
			lower_inf = from;
		}

		// now compute the rest of the integral, i. e. the term to infinity.

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
		if (verbosity > 9)
		{
			Log.info() << "r = " << r << endl;
			Log.info() << "infval = " << infval << endl;
		}
		return val;
	}


	double Pair6_12RDFIntegrator::integrateToInf(double from, double A, 
			double B, double k1, double k2) throw()
	{
		setConstants(A, B, k1, k2);
		return integrateToInf(from);
	}


	double Pair6_12RDFIntegrator::integrate(double from, double to) const
		throw()
	{

		// This is hack. I think. 
		if (to < from)
		{
			Log.warn() << "to < from, exchanging" << endl;
			double tmp = to;
			to = from;
			from = tmp;
		}

		// int verbosity =
		//	options.getInteger(Pair6_12RDFIntegrator::Option::VERBOSITY);
		int method = options.getInteger(Pair6_12RDFIntegrator::Option::METHOD);

		if (method == METHOD__UNKNOWN)
		{
			Log.warn() << "Unknown integration method, defaulting to analytical."
				<< endl;
			method = METHOD__ANALYTICAL;
		}

		if (method == METHOD__ANALYTICAL)
		{
			// analytical integration. We need to build the intervals for
			// integration according to the limits of the polynomial
			// representation of the radial distribution function. If geometrical
			// correction has to be performed, these limits need to be projected
			// to and from the integration beam.

			// we need the parameters of the plynomial description of the radial
			// distribution function.

			PiecewisePolynomial poly = getRDF().getRepresentation();

			// k2_ is the squared distance between spehere center and atom
			// center, so if this is small it is very likely that they are the
			// same and thus projecting values is not necessary.

			double FROM;
			double TO;

			// k2_ is always > 0, so we don't have to fabs() here
			if (k2_ < MIN_DISTANCE)
			{
				FROM = from;
				TO = to;
			}
			else
			{
				FROM = project(from);
				TO = project(to);
			}

			Size from_index = poly.getIntervalIndex(FROM);
			Size to_index = poly.getIntervalIndex(TO);

			if ((from_index == INVALID_POSITION) || (to_index == INVALID_POSITION))
			{
				// no error message, because getIntervalIndex() handles this
				return 0.0;
			}

			// Although we might have to project, we are still integrating the
			// interval [from, to).
			
			Interval interval(from, to);
			Coefficients coeffs = poly.getCoefficients(from_index);
			double x0 = poly.getInterval(from_index).first;

			// If the (projected) limits yield one interval, just compute and
			// return the value of it.

			if (from_index == to_index)
			{
				// this REQUIRES that the first integral is the one starting at
				// zero
				if (from_index == 0)
				{
					return 0.0;
				}
				else
				{
					return analyticallyIntegrateInterval(interval, coeffs, x0);
				}
			}

			// if we didn't return, the indices weren't equal, so we have to sum
			// up at least two intervals.

			// we have to set the upper limit which is the back projected
			// interval limit of the rdf definition

			double val = 0.0;

			// k2_ is always > 0, so we don't have to fabs() here
			if (k2_ < MIN_DISTANCE)
			{

				// this REQUIRES that the first integral is the one starting at
				// zero, i. e. has zero parameters, yielding a computed value of zero
				if (from_index > 0) 
				{
					interval.second = poly.getInterval(from_index).second;
					x0 = poly.getInterval(from_index).first;
					val = analyticallyIntegrateInterval(interval, coeffs, from_index);
				}

				// if we are below the last interval, sum up the results from each
				// interval integration.
				for (Size k = from_index + 1; k < to_index; ++k)
				{
					coeffs = poly.getCoefficients(k);
					interval = poly.getInterval(k);
					x0 = poly.getInterval(k).first;
					val += analyticallyIntegrateInterval(interval, coeffs, x0);
				}

				// if the decision from_index == to_index was false, to_index should
				// not be 0 here, so I won't test that...

				coeffs = poly.getCoefficients(to_index);
				interval = poly.getInterval(to_index);
				interval.second = to;
				x0 = poly.getInterval(to_index).first;
				val += analyticallyIntegrateInterval(interval, coeffs, x0);

				return val;
			}
			else
			{
				// this REQUIRES that the first integral is the one starting at
				// zero, i. e. has zero parameters, yielding a computed value of zero
				if (from_index > 0) 
				{
					interval.second = unproject(poly.getInterval(from_index).second);
					x0 = unproject(poly.getInterval(from_index).first);
					val = analyticallyIntegrateInterval(interval, coeffs, x0);
				}

				Interval INTERVAL;
				// if we are below the last interval, sum up the results from each
				// interval integration.
				for (Size k = from_index + 1; k < to_index; ++k)
				{
					coeffs = poly.getCoefficients(k);
					INTERVAL = poly.getInterval(k);
					x0 = unproject(poly.getInterval(k).first);
					interval.first = unproject(INTERVAL.first);
					interval.second = unproject(INTERVAL.second);
					val += analyticallyIntegrateInterval(interval, coeffs, x0);
				}

				// if the decision from_index == to_index was false, to_index should
				// not be 0 here, so I won't test that...
				coeffs = poly.getCoefficients(to_index);
				INTERVAL = poly.getInterval(to_index);
				interval.first = unproject(INTERVAL.first);
				interval.second = to;
				x0 = unproject(poly.getInterval(to_index).first);
				val += analyticallyIntegrateInterval(interval, coeffs, x0);

				return val;
			}
		}
		else
		{
			if (method == METHOD__TRAPEZIUM)
			{
				// numerical integration does not need this projecting thing
				// because this method uses values of the integrand, that are
				// computed with respect to the geometry

				Interval interval(from, to);
				return numericallyIntegrateInterval(interval);
			}
			else
			{
				Log.error() << "Unknown integration method" << endl;
				return 0;
			}
		}
	}


	double Pair6_12RDFIntegrator::integrate(double from, double to, double A, 
			double B, double k1, double k2) throw()
	{
		setConstants(A, B, k1, k2);
		return integrate(from, to);
	}


	double Pair6_12RDFIntegrator::operator () (double x) const throw()
	{
		return integrateToInf(x);
	}


	bool Pair6_12RDFIntegrator::operator == 
		(const Pair6_12RDFIntegrator& integrator) const 
		throw()
	{
		return ((RDFIntegrator::operator == (integrator))
			&& (A_ == integrator.A_)
			&& (B_ == integrator.B_)
			&& (k1_ == integrator.k1_)
			&& (k2_ == integrator.k2_));
	}


	double Pair6_12RDFIntegrator::numericallyIntegrateInterval
		(const Interval& interval) const 
		throw()
	{

		int samples = (int) options.getInteger(Option::SAMPLES);
		int verbosity = (int) options.getInteger(Option::VERBOSITY);

		double lower_limit = interval.first;
		double upper_limit = interval.second;

		if (verbosity > 9)
		{
			Log.info() << "lower_limit = " << lower_limit << endl;
			Log.info() << "upper_limit = " << upper_limit << endl;
			Log.info() << "k1 = " << k1_ << endl;
			Log.info() << "k2 = " << k2_ << endl;
		}

		// this is the case where we have to consider the geometry of the
		// situation. As this seems analytically impossible, we have to do it
		// numerically. The method we use is the trapezium method.

		double area = 0;
		if (verbosity > 9)
		{
			Log.info() << "Using " << samples 
				<< " sample points for numerical integration" << endl;
		}
		unsigned int n = samples;

		// lower case variables are for the potential term
		// upper case variables are for the rdf term (representing the
		// geometrical correction)
		double x, s;
		double X = x = lower_limit;
		double S = s = (upper_limit-lower_limit)/n;

		if (k2_ > MIN_DISTANCE) 
		{
			X = sqrt((lower_limit*lower_limit + k1_ * lower_limit + k2_));
			S = (sqrt((upper_limit*upper_limit + k1_ * upper_limit + k2_))-X)/n;
		}

		// temporary variables
		double x6;
		double xs6;

		while (n > 0)
		{
			if (verbosity > 9)
			{
				Log.info() << "rdf(" << X << ") = " << getRDF()(X) << endl;
			}

			x6 = pow(x,6);
			xs6 = pow(x+s,6);

			area += (x*x*(A_/(x6*x6) - B_/x6) * getRDF()(X) 
					+ (x+s)*(x+s)*(A_/(xs6*xs6) - B_/xs6) * getRDF()(X+S)) / 2.0 * s;
			x += s;
			X += S;
			--n;
		}

		return area;
	}


	double Pair6_12RDFIntegrator::analyticallyIntegrateInterval
		(const Interval& interval, const Coefficients& a, float x0) 
		const throw()
	{

		double r = interval.first;
		double R = interval.second;
		if (BALL::Maths::isNan(r) || BALL::Maths::isNan(R))
		{
			Log.error() << 
				"Pair6_12RDFIntegrator::analyticallyIntegrateInterval(): "
				<< "detected NaN in line " << __LINE__  << endl;
			Log.error() << "r = " << r << endl;
			Log.error() << "R = " << R << endl;
		}

		// k2_ is always > 0, so we don't have to fabs() here
		if (k2_ < MIN_DISTANCE)
		{

			// This is the case where no projection has to be done, because the
			// distance between the sphere center and the atom center is below
			// 1e-6 Angstroms.

			// Erzeugt Mon Nov  6 16:37:25 MET 2000 mit Maple V R3
			//
			// Kommandos:
			//
			// f:=x^2*(A/x^12 - B/x^6)*sum(a[i]*(x-k)^i, i=0..3);
			// int(f,x=r..R);
			// readlib(C);
			// C(int(f,x=r..R), filename=newC);

			double s1 = (-189.0*A_*a[3]*x0*x0*R-56.0*A_*a[0]-1512.0*B_*a[3]*x0*pow(R,8.0)-168.0*B_*a[1]*x0*pow(R,6.0)-84.0*A_*a[3]*R*R*R+756.0*B_*a[3]*x0*x0*pow(R,7.0)+252.0*B_*a[1]*pow(R,7.0)+168.0*B_*a[0]*pow(R,6.0)+168.0*B_*a[2]*x0*x0*pow(R,6.0)-504.0*B_*a[2]*x0*pow(R,7.0)-63.0*A_*a[1]*R+126.0*A_*a[2]*x0*R-56.0*A_*a[2]*x0*x0+56.0*A_*a[1]*x0+504.0*B_*a[2]*pow(R,8.0)+216.0*A_*a[3]*x0*R*R+56.0*A_*a[3]*x0*x0*x0-72.0*A_*a[2]*R*R-504.0*B_*a[3]*log(R)*pow(R,9.0)-168.0*B_*a[3]*x0*x0*x0*pow(R,6.0))/pow(R,9.0)/504;
			double s2 = (-168.0*B_*a[2]*x0*x0*pow(r,6.0)+56.0*A_*a[0]+504.0*B_*a[2]*x0*pow(r,7.0)-504.0*B_*a[2]*pow(r,8.0)-252.0*B_*a[1]*pow(r,7.0)-168.0*B_*a[0]*pow(r,6.0)+84.0*A_*a[3]*r*r*r-56.0*A_*a[3]*x0*x0*x0+56.0*A_*a[2]*x0*x0-56.0*A_*a[1]*x0+504.0*B_*a[3]*log(r)*pow(r,9.0)+168.0*B_*a[3]*x0*x0*x0*pow(r,6.0)-756.0*B_*a[3]*x0*x0*pow(r,7.0)+189.0*A_*a[3]*x0*x0*r+1512.0*B_*a[3]*x0*pow(r,8.0)+168.0*B_*a[1]*x0*pow(r,6.0)+72.0*A_*a[2]*r*r+63.0*A_*a[1]*r-216.0*A_*a[3]*x0*r*r-126.0*A_*a[2]*x0*r)/pow(r,9.0)/504;
			double t0 = s1+s2;

			if (BALL::Maths::isNan(t0))
			{
				Log.warn() << "Return value is NaN." << endl;
			}

			return t0;

		}
		else
		{

			// Erzeugt am Mon Nov  6 16:33:02 MET 2000 mit MAPLE V Release 3.
			// 
			// Kommandos:
			// 
			// f:=x^2 * (A/x^12 - B/x^6) * sum(a[i] * (sqrt(x^2 + k[1]*x + k[2]) -
			// k[3])^i, i=0..3);
			// int(f, x=r..R);
			// readlib(C);
			// C(int(f,x=r..R), filename=newC);

			double s6 = 16.0/315.0*A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,6)+A_*a[3]*x0*sqrt(pow(k2_,37))/3+A_*a[3]*pow(x0,3)*sqrt(pow(k2_,35))/9+B_*a[0]*sqrt(pow(k2_,35))*pow(R,6)/3+A_*a[1]*x0*sqrt(pow(k2_,35))/9+1001.0/4096.0*A_*a[2]*x0*pow(k1_,6)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,10)+B_*a[3]*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31))*pow(R,9)/8+B_*a[2]*x0*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29))*pow(R,10)/4-A_*a[0]*sqrt(pow(k2_,35))/9-A_*a[2]*sqrt(pow(k2_,37))/9-A_*a[2]*pow(x0,2)*sqrt(pow(k2_,35))/9+B_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,33))*pow(R,6)/3-3.0/4.0*B_*a[3]*pow(x0,2)*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16)*pow(R,9);
			double s7 = s6-117.0/224.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*R*R*R-B_*a[3]*x0*sqrt(pow(k2_,37))*pow(R,6)+35.0/128.0*A_*a[2]*x0*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13)*pow(R,9)-143.0/3840.0*A_*a[3]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,25))*pow(R,4)+B_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,33))*pow(R,6)/3-1771.0/2048.0*A_*a[2]*x0*pow(k1_,5)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,9);
			double s5 = s7+16.0/105.0*A_*a[3]*pow(x0,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,6)-3.0*B_*a[3]*x0*sqrt(pow(k2_,35))*pow(R,8)+2651.0/13440.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,5)+2145.0/16384.0*A_*a[3]*pow(x0,2)*pow(k1_,7)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,7)-B_*a[2]*x0*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31))*pow(R,9)/2+143.0/6144.0*A_*a[3]*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,23))*pow(R,5)-715.0/16384.0*A_*a[2]*x0*pow(k1_,9)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17))*pow(R,9)-B_*a[3]*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16)*pow(R,9)/16;
			s7 = s5-35.0/256.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,7)+1155.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,10)-143.0/98304.0*A_*a[3]*pow(k1_,8)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,17))*pow(R,10)-65.0/672.0*A_*a[1]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*R*R-A_*a[2]*k1_*sqrt(pow(k2_,35))*R/8+2651.0/4480.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,5);
			s6 = s7-8.0/105.0*A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,4)+2.0/9.0*A_*a[2]*x0*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,33))-B_*a[3]*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,33))*pow(R,10)-B_*a[2]*x0*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,8)/4+B_*a[2]*x0*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29))*pow(R,9)/4-B_*a[3]*pow(x0,3)*sqrt(pow(k2_,35))*pow(R,6)/3+15.0/256.0*A_*a[3]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,14)*pow(R,9);
			s7 = s6-2651.0/6720.0*A_*a[2]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,5)+143.0/49152.0*A_*a[3]*pow(k1_,7)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,19))*pow(R,7)+35.0/256.0*A_*a[1]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25))*pow(R,9)-39.0/224.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*R*R*R-143.0/98304.0*A_*a[3]*pow(k1_,9)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,17))*pow(R,9)+2.0/21.0*A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*R*R;
			double s4 = s7+455.0/512.0*A_*a[2]*x0*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,9)+143.0/98304.0*A_*a[3]*pow(k1_,8)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,17))*pow(R,8)-143.0/12288.0*A_*a[3]*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,21))*pow(R,6)-8.0/315.0*A_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,29))*pow(R,4)-187.0/2016.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,29))*R*R*R+39.0/112.0*A_*a[2]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*R*R*R-99.0/4096.0*A_*a[3]*pow(k1_,7)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11)*pow(R,9)+2145.0/32768.0*A_*a[3]*pow(x0,2)*pow(k1_,8)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17))*pow(R,10);
			s7 = s4-35.0/256.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,8)+1771.0/4096.0*A_*a[1]*pow(k1_,5)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,9)+143.0/2688.0*A_*a[3]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,27))*R*R*R+209.0/2240.0*A_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,27))*pow(R,4)-32.0/315.0*A_*a[2]*x0*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,6)+115.0/2048.0*A_*a[3]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,21))*pow(R,8);
			s6 = s7+143.0/512.0*A_*a[1]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,6)-1001.0/4096.0*A_*a[1]*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,7)-2.0/3.0*B_*a[2]*x0*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,33))*pow(R,6)-385.0/2048.0*A_*a[1]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,8)+2431.0/4480.0*A_*a[2]*x0*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,5)-B_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*pow(R,7)/4+4.0/63.0*A_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,31))*R*R;
			s5 = s6+B_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,10)/24-715.0/12288.0*A_*a[1]*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,6)-143.0/32768.0*A_*a[3]*pow(k1_,8)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,10)-1001.0/8192.0*A_*a[1]*pow(k1_,6)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,10)+B_*a[2]*k1_*sqrt(pow(k2_,35))*pow(R,7)/2+B_*a[2]*pow(x0,2)*sqrt(pow(k2_,35))*pow(R,6)/3+3.0/7.0*A_*a[3]*x0*sqrt(pow(k2_,35))*R*R+55.0/1536.0*A_*a[3]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,23))*pow(R,6)+B_*a[2]*sqrt(pow(k2_,35))*pow(R,8)-3.0/2.0*B_*a[3]*x0*k1_*sqrt(pow(k2_,35))*pow(R,7)+3.0/8.0*A_*a[3]*x0*k1_*sqrt(pow(k2_,35))*R-B_*a[1]*x0*sqrt(pow(k2_,35))*pow(R,6)/3-A_*a[2]*sqrt(pow(k2_,35))*R*R/7+B_*a[2]*sqrt(pow(k2_,37))*pow(R,6)/3;
			s7 = s5-2717.0/16384.0*A_*a[1]*pow(k1_,7)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,9)+13.0/144.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,31))*R+715.0/32768.0*A_*a[1]*pow(k1_,9)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17))*pow(R,9)-125.0/2048.0*A_*a[3]*pow(k1_,4)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,10)+B_*a[3]*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31))*pow(R,10)/8+15.0/512.0*A_*a[3]*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25))*pow(R,10);
			s6 = s7+35.0/512.0*A_*a[1]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,8)+1155.0/1024.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,7)-385.0/512.0*A_*a[2]*x0*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,7)+539.0/24576.0*A_*a[3]*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,10)+5.0/16.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*R-35.0/512.0*A_*a[1]*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,10)-8151.0/16384.0*A_*a[3]*pow(x0,2)*pow(k1_,7)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,9);
			s7 = s6+3003.0/8192.0*A_*a[3]*pow(x0,2)*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,8)-2145.0/65536.0*A_*a[3]*pow(x0,2)*pow(k1_,9)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,9)*pow(R,9)-35.0/256.0*A_*a[1]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13)*pow(R,9)-143.0/2016.0*A_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,29))*R*R+2.0/7.0*A_*a[3]*pow(x0,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*R*R-105.0/256.0*A_*a[3]*pow(x0,2)*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13)*pow(R,9)+105.0/256.0*A_*a[1]*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12)*pow(R,9);
			double s8 = s7+5.0/256.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,25))*pow(R,7)+429.0/4096.0*A_*a[1]*pow(k1_,7)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10)*pow(R,9)-2.0/3.0*B_*a[3]*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*pow(R,10);
			double s3 = s8+16.0/105.0*A_*a[2]*x0*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,4)+2871.0/4480.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,6)-115.0/2048.0*A_*a[3]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,10)+315.0/256.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12)*pow(R,9)-2079.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11)*pow(R,9);
			s7 = s3-7.0/12.0*B_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*pow(R,9)+35.0/128.0*A_*a[2]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,7)-2431.0/8960.0*A_*a[1]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,5)-B_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,29))*pow(R,8)/24+B_*a[3]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,9)/24;
			s6 = s7-B_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,31))*pow(R,7)/12-539.0/24576.0*A_*a[3]*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,19))*pow(R,8)+105.0/512.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,8)+105.0/256.0*A_*a[3]*pow(x0,2)*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25))*pow(R,9)+1287.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,7)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10)*pow(R,9)-5.0/4.0*B_*a[3]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,33))*pow(R,9)+11.0/4096.0*A_*a[3]*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,21))*pow(R,7);
			s7 = s6+3.0/4.0*B_*a[3]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,17)*pow(R,9)-105.0/256.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,7)-1155.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,8)-1573.0/3360.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,4)-1365.0/1024.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,9)+3.0/8.0*B_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,8);
			s5 = s7+143.0/2048.0*A_*a[1]*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,5)-8.0/35.0*A_*a[3]*pow(x0,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,4)+2145.0/32768.0*A_*a[3]*pow(x0,2)*pow(k1_,9)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17))*pow(R,9)-429.0/1792.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,4)-A_*a[1]*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,33))/9-143.0/1792.0*A_*a[1]*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,4)-8613.0/8960.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,6)+2717.0/8192.0*A_*a[2]*x0*pow(k1_,7)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,9);
			s7 = s5+429.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,5)-105.0/512.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,10)+B_*a[2]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*pow(R,7)/2+715.0/8064.0*A_*a[1]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*R*R*R+2.0/3.0*B_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,31))*pow(R,8)+143.0/65536.0*A_*a[3]*pow(k1_,9)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10)*pow(R,9);
			s6 = s7-A_*a[3]*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,33))/9-35.0/256.0*A_*a[3]*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,13)*pow(R,9)-503.0/4096.0*A_*a[3]*pow(k1_,5)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,9)-4.0/21.0*A_*a[2]*x0*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*R*R-429.0/2048.0*A_*a[2]*x0*pow(k1_,7)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,10)*pow(R,9)-B_*a[2]*x0*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,15)*pow(R,9)/8+649.0/16384.0*A_*a[3]*pow(k1_,7)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,9);
			s7 = s6-241.0/4096.0*A_*a[3]*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,9)+5.0/48.0*A_*a[1]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*R-1001.0/4096.0*A_*a[2]*x0*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,8)+693.0/1024.0*A_*a[2]*x0*pow(k1_,5)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11)*pow(R,9)-105.0/128.0*A_*a[2]*x0*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12)*pow(R,9)+715.0/16384.0*A_*a[1]*pow(k1_,7)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,7)-715.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,6);
			s4 = s7-2871.0/8960.0*A_*a[1]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,6)-A_*a[3]*pow(x0,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,33))/3-3003.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,7)+429.0/512.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,6)-715.0/32768.0*A_*a[1]*pow(k1_,8)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,17))*pow(R,8)+253.0/8192.0*A_*a[3]*pow(k1_,6)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,10)-95.0/3072.0*A_*a[3]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,23))*pow(R,7)+B_*a[1]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31))*pow(R,9)/4;
			s7 = s4+5.0/128.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,27))*pow(R,5)+185.0/3072.0*A_*a[3]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,9)-B_*a[1]*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16)*pow(R,9)/4-35.0/128.0*A_*a[2]*x0*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25))*pow(R,9)-5.0/256.0*A_*a[3]*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,9)-15.0/256.0*A_*a[3]*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,27))*pow(R,9);
			s6 = s7+935.0/49152.0*A_*a[3]*pow(k1_,7)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,9)+B_*a[3]*pow(x0,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,33))*pow(R,6)+189.0/2048.0*A_*a[3]*pow(k1_,5)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,12)*pow(R,9)-7293.0/8960.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,5)-3003.0/8192.0*A_*a[3]*pow(x0,2)*pow(k1_,6)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,10)-455.0/1024.0*A_*a[1]*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,9)-693.0/2048.0*A_*a[1]*pow(k1_,5)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,11)*pow(R,9);
			s7 = s6-2145.0/32768.0*A_*a[3]*pow(x0,2)*pow(k1_,8)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,17))*pow(R,8)+715.0/2688.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*R*R*R-65.0/224.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*R*R-B_*a[3]*log(R+k1_/2+sqrt(R*R+k2_+k1_*R))*sqrt(pow(k2_,35))*pow(R,9)+1573.0/2240.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,4)+B_*a[1]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*pow(R,8)/8;
			s5 = s7+15.0/512.0*A_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,10)+3.0/4.0*B_*a[3]*pow(x0,2)*k1_*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,31))*pow(R,9)-5.0/256.0*A_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,25))*pow(R,6)+3.0/16.0*B_*a[3]*pow(x0,2)*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,15)*pow(R,9)-3.0/8.0*B_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29))*pow(R,10)-15.0/512.0*A_*a[3]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,23))*pow(R,8)+1001.0/2048.0*A_*a[2]*x0*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,7)-143.0/256.0*A_*a[2]*x0*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,6);
			s7 = s5-3.0/8.0*B_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29))*pow(R,9)-143.0/32768.0*A_*a[3]*pow(k1_,9)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,19))*pow(R,9)-55.0/768.0*A_*a[3]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,5))*sqrt(pow(k2_,25))*pow(R,5)+B_*a[1]*pow(k1_,3)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,15)*pow(R,9)/16+155.0/1024.0*A_*a[3]*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,25))*pow(R,9)+385.0/1024.0*A_*a[1]*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,7);
			s6 = s7+B_*a[2]*x0*k1_*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,16)*pow(R,9)/2+65.0/336.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,29))*R*R-5.0/24.0*A_*a[2]*x0*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*R-715.0/4032.0*A_*a[2]*x0*pow(k1_,3)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*R*R*R-715.0/8192.0*A_*a[2]*x0*pow(k1_,7)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,7)-715.0/65536.0*A_*a[1]*pow(k1_,9)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,9)*pow(R,9)+385.0/2048.0*A_*a[1]*pow(k1_,4)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,10)+715.0/16384.0*A_*a[2]*x0*pow(k1_,8)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,17))*pow(R,8);
			s7 = s6-143.0/1024.0*A_*a[2]*x0*pow(k1_,5)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,23))*pow(R,5)+715.0/6144.0*A_*a[2]*x0*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,6)+143.0/896.0*A_*a[2]*x0*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,25))*pow(R,4)-B_*a[1]*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29))*pow(R,10)/8+385.0/1024.0*A_*a[2]*x0*pow(k1_,4)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,21))*pow(R,8)+715.0/32768.0*A_*a[2]*x0*pow(k1_,9)*atanh((2.0*k2_+k1_*R)/sqrt(k2_)/sqrt(R*R+k2_+k1_*R)/2)*pow(k2_,9)*pow(R,9)-715.0/16384.0*A_*a[2]*x0*pow(k1_,8)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17))*pow(R,10);
			double s2 = s7-385.0/1024.0*A_*a[2]*x0*pow(k1_,4)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,10)+35.0/256.0*A_*a[2]*x0*pow(k1_,2)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,23))*pow(R,10)-3.0/4.0*B_*a[3]*pow(x0,2)*k1_*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,31))*pow(R,7)+1001.0/8192.0*A_*a[1]*pow(k1_,6)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,19))*pow(R,8)+1573.0/6720.0*A_*a[1]*pow(k1_,2)*sqrt(pow(R*R+k2_+k1_*R,3))*sqrt(pow(k2_,27))*pow(R,4)-B_*a[1]*pow(k1_,3)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,29))*pow(R,9)/8+5313.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,21))*pow(R,9)+715.0/32768.0*A_*a[1]*pow(k1_,8)*sqrt(R*R+k2_+k1_*R)*sqrt(pow(k2_,17))*pow(R,10);
			s3 = 1/pow(R,9)/sqrt(pow(k2_,35));
			double s1 = s2*s3;
			s7 = 65.0/224.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*r*r-B_*a[0]*sqrt(pow(k2_,35))*pow(r,6)/3-35.0/256.0*A_*a[1]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25))*pow(r,9)+3.0/2.0*B_*a[3]*x0*k1_*sqrt(pow(k2_,35))*pow(r,7)-B_*a[2]*sqrt(pow(k2_,37))*pow(r,6)/3-209.0/2240.0*A_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,27))*pow(r,4)-A_*a[3]*x0*sqrt(pow(k2_,37))/3-A_*a[3]*pow(x0,3)*sqrt(pow(k2_,35))/9+5.0/256.0*A_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,25))*pow(r,6)-A_*a[1]*x0*sqrt(pow(k2_,35))/9+A_*a[0]*sqrt(pow(k2_,35))/9+8.0/35.0*A_*a[3]*pow(x0,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,4)+A_*a[2]*sqrt(pow(k2_,37))/9;
			s8 = s7-15.0/512.0*A_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,10)-B_*a[2]*sqrt(pow(k2_,35))*pow(r,8)+A_*a[2]*pow(x0,2)*sqrt(pow(k2_,35))/9-3.0/8.0*A_*a[3]*x0*k1_*sqrt(pow(k2_,35))*r+A_*a[2]*sqrt(pow(k2_,35))*r*r/7-3.0/7.0*A_*a[3]*x0*sqrt(pow(k2_,35))*r*r;
			s6 = s8-935.0/49152.0*A_*a[3]*pow(k1_,7)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,9)-115.0/2048.0*A_*a[3]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,21))*pow(r,8)-253.0/8192.0*A_*a[3]*pow(k1_,6)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,10)-539.0/24576.0*A_*a[3]*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,10)+539.0/24576.0*A_*a[3]*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,19))*pow(r,8)+241.0/4096.0*A_*a[3]*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,9)+95.0/3072.0*A_*a[3]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,23))*pow(r,7)+143.0/12288.0*A_*a[3]*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,21))*pow(r,6);
			s8 = s6-143.0/2688.0*A_*a[3]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,27))*r*r*r-143.0/49152.0*A_*a[3]*pow(k1_,7)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,19))*pow(r,7)-5.0/128.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,27))*pow(r,5)+143.0/32768.0*A_*a[3]*pow(k1_,8)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,10)+143.0/3840.0*A_*a[3]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,25))*pow(r,4)-143.0/6144.0*A_*a[3]*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,23))*pow(r,5);
			s7 = s8+15.0/256.0*A_*a[3]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,27))*pow(r,9)+143.0/98304.0*A_*a[3]*pow(k1_,8)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,17))*pow(r,10)-2.0/9.0*A_*a[2]*x0*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,33))-315.0/256.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12)*pow(r,9)-55.0/1536.0*A_*a[3]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,23))*pow(r,6)-3003.0/8192.0*A_*a[3]*pow(x0,2)*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,8)+125.0/2048.0*A_*a[3]*pow(k1_,4)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,10);
			s8 = s7+5.0/256.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,9)+3003.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,7)+8151.0/16384.0*A_*a[3]*pow(x0,2)*pow(k1_,7)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,9)-5.0/48.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*r-13.0/144.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,31))*r+187.0/2016.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,29))*r*r*r;
			double s9 = s8+7293.0/8960.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,5)-143.0/98304.0*A_*a[3]*pow(k1_,8)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,17))*pow(r,8)+143.0/2016.0*A_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,29))*r*r;
			s5 = s9+2145.0/65536.0*A_*a[3]*pow(x0,2)*pow(k1_,9)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,9)*pow(r,9)+8613.0/8960.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,6)+2145.0/32768.0*A_*a[3]*pow(x0,2)*pow(k1_,8)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,17))*pow(r,8)-429.0/512.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,6)-1287.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,7)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10)*pow(r,9);
			s8 = s5+503.0/4096.0*A_*a[3]*pow(k1_,5)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,9)+55.0/768.0*A_*a[3]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,25))*pow(r,5)-1155.0/1024.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,7)+105.0/256.0*A_*a[3]*pow(x0,2)*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13)*pow(r,9)+8.0/315.0*A_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,29))*pow(r,4)-15.0/256.0*A_*a[3]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,14)*pow(r,9);
			s7 = s8-5.0/256.0*A_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,25))*pow(r,7)-189.0/2048.0*A_*a[3]*pow(k1_,5)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12)*pow(r,9)+35.0/256.0*A_*a[3]*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13)*pow(r,9)-B_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,10)/24+3003.0/8192.0*A_*a[3]*pow(x0,2)*pow(k1_,6)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,10)+715.0/65536.0*A_*a[1]*pow(k1_,9)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,9)*pow(r,9)-1155.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,10);
			s8 = s7+99.0/4096.0*A_*a[3]*pow(k1_,7)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11)*pow(r,9)-2145.0/32768.0*A_*a[3]*pow(x0,2)*pow(k1_,9)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17))*pow(r,9)-2145.0/16384.0*A_*a[3]*pow(x0,2)*pow(k1_,7)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,7)+3.0/8.0*B_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29))*pow(r,9)-715.0/32768.0*A_*a[1]*pow(k1_,9)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17))*pow(r,9)-16.0/315.0*A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,6);
			s6 = s8+429.0/2048.0*A_*a[2]*x0*pow(k1_,7)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10)*pow(r,9)+429.0/1792.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,4)-429.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,5)+715.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,6)+1001.0/8192.0*A_*a[1]*pow(k1_,6)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,10)-2145.0/32768.0*A_*a[3]*pow(x0,2)*pow(k1_,8)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17))*pow(r,10)-715.0/2688.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*r*r*r+1155.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,8);
			s8 = s6-1573.0/2240.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,4)-105.0/256.0*A_*a[3]*pow(x0,2)*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25))*pow(r,9)+B_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*pow(r,7)/4+39.0/224.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*r*r*r-2.0/21.0*A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*r*r+35.0/512.0*A_*a[1]*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,10);
			s7 = s8+B_*a[3]*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16)*pow(r,9)/16-16.0/105.0*A_*a[3]*pow(x0,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,6)+105.0/512.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,10)+8.0/105.0*A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,4)+117.0/224.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*r*r*r+105.0/256.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,7)-B_*a[1]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31))*pow(r,9)/4;
			s8 = s7-105.0/512.0*A_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,8)-5.0/16.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*r+B_*a[1]*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29))*pow(r,9)/8-35.0/128.0*A_*a[2]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,7)-B_*a[1]*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,15)*pow(r,9)/16-B_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,33))*pow(r,6)/3+B_*a[1]*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29))*pow(r,10)/8;
			s4 = s8+143.0/98304.0*A_*a[3]*pow(k1_,9)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,17))*pow(r,9)-B_*a[1]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,8)/8+B_*a[2]*x0*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31))*pow(r,9)/2+A_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,33))/9-B_*a[2]*x0*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29))*pow(r,9)/4+B_*a[2]*x0*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,8)/4+2.0/3.0*B_*a[3]*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*pow(r,10)+B_*a[2]*x0*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,15)*pow(r,9)/8;
			s8 = s4-3.0/4.0*B_*a[3]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,17)*pow(r,9)-B_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,33))*pow(r,6)/3+2.0/3.0*B_*a[2]*x0*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,33))*pow(r,6)+7.0/12.0*B_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*pow(r,9)-B_*a[2]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*pow(r,7)/2;
			s7 = s8-2651.0/4480.0*A_*a[3]*pow(x0,2)*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,5)-105.0/256.0*A_*a[1]*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12)*pow(r,9)-1771.0/4096.0*A_*a[1]*pow(k1_,5)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,9)-B_*a[3]*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31))*pow(r,10)/8+455.0/1024.0*A_*a[1]*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,9)+693.0/2048.0*A_*a[1]*pow(k1_,5)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11)*pow(r,9)-429.0/4096.0*A_*a[1]*pow(k1_,7)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10)*pow(r,9);
			s8 = s7+2717.0/16384.0*A_*a[1]*pow(k1_,7)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,9)+A_*a[1]*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,33))/9+B_*a[3]*k1_*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,31))*pow(r,7)/12-B_*a[2]*pow(x0,2)*sqrt(pow(k2_,35))*pow(r,6)/3+35.0/256.0*A_*a[1]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13)*pow(r,9)+B_*a[1]*x0*sqrt(pow(k2_,35))*pow(r,6)/3;
			s6 = s8-B_*a[2]*k1_*sqrt(pow(k2_,35))*pow(r,7)/2-4.0/63.0*A_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,31))*r*r+B_*a[3]*x0*sqrt(pow(k2_,37))*pow(r,6)+5.0/4.0*B_*a[3]*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,33))*pow(r,9)-2.0/7.0*A_*a[3]*pow(x0,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*r*r+A_*a[3]*pow(x0,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,33))/3+3.0*B_*a[3]*x0*sqrt(pow(k2_,35))*pow(r,8)-2.0/3.0*B_*a[3]*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,31))*pow(r,8);
			s8 = s6-385.0/1024.0*A_*a[1]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,7)+385.0/2048.0*A_*a[1]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,8)-143.0/512.0*A_*a[1]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,6)+A_*a[2]*k1_*sqrt(pow(k2_,35))*r/8-1573.0/6720.0*A_*a[1]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,4)+1001.0/4096.0*A_*a[1]*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,7);
			s7 = s8-455.0/512.0*A_*a[2]*x0*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,9)+115.0/2048.0*A_*a[3]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,10)+B_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,29))*pow(r,8)/24+B_*a[3]*pow(x0,3)*sqrt(pow(k2_,35))*pow(r,6)/3-B_*a[3]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,9)/24+2431.0/8960.0*A_*a[1]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,5)+B_*a[3]*log(r+k1_/2+sqrt(r*r+k2_+k1_*r))*sqrt(pow(k2_,35))*pow(r,9);
			s8 = s7-143.0/896.0*A_*a[2]*x0*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,4)-715.0/16384.0*A_*a[2]*x0*pow(k1_,8)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,17))*pow(r,8)+143.0/1024.0*A_*a[2]*x0*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,5)+715.0/8192.0*A_*a[2]*x0*pow(k1_,7)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,7)+35.0/256.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,7)-715.0/32768.0*A_*a[2]*x0*pow(k1_,9)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,9)*pow(r,9)-35.0/512.0*A_*a[1]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,8);
			s5 = s8+35.0/128.0*A_*a[2]*x0*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25))*pow(r,9)-385.0/1024.0*A_*a[2]*x0*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,8)+105.0/128.0*A_*a[2]*x0*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,12)*pow(r,9)+143.0/1792.0*A_*a[1]*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,4)-385.0/2048.0*A_*a[1]*pow(k1_,4)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,10)-2651.0/13440.0*A_*a[1]*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,5)+715.0/16384.0*A_*a[2]*x0*pow(k1_,8)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17))*pow(r,10)+715.0/16384.0*A_*a[2]*x0*pow(k1_,9)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17))*pow(r,9);
			s8 = s5+5.0/24.0*A_*a[2]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*r-693.0/1024.0*A_*a[2]*x0*pow(k1_,5)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11)*pow(r,9)-649.0/16384.0*A_*a[3]*pow(k1_,7)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,9)-2431.0/4480.0*A_*a[2]*x0*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,5)+715.0/4032.0*A_*a[2]*x0*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*r*r*r+35.0/256.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,8);
			s7 = s8+385.0/512.0*A_*a[2]*x0*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,7)+1573.0/3360.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,4)-39.0/112.0*A_*a[2]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*r*r*r+65.0/672.0*A_*a[1]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*r*r-143.0/2048.0*A_*a[1]*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,5)+715.0/12288.0*A_*a[1]*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,6)-715.0/16384.0*A_*a[1]*pow(k1_,7)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,7);
			s8 = s7-B_*a[3]*pow(x0,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,33))*pow(r,6)-715.0/32768.0*A_*a[1]*pow(k1_,8)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,17))*pow(r,10)-3.0/4.0*B_*a[3]*pow(x0,2)*k1_*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31))*pow(r,9)+715.0/32768.0*A_*a[1]*pow(k1_,8)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,17))*pow(r,8)+143.0/256.0*A_*a[2]*x0*pow(k1_,4)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,6)+3.0/8.0*B_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29))*pow(r,10);
			s6 = s8-1001.0/2048.0*A_*a[2]*x0*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,7)+3.0/4.0*B_*a[3]*pow(x0,2)*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*pow(r,7)+1001.0/4096.0*A_*a[2]*x0*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,8)+2651.0/6720.0*A_*a[2]*x0*k1_*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,5)+385.0/1024.0*A_*a[2]*x0*pow(k1_,4)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,10)-3.0/8.0*B_*a[3]*pow(x0,2)*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,8)-1001.0/4096.0*A_*a[2]*x0*pow(k1_,6)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,10)-16.0/105.0*A_*a[2]*x0*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*pow(r,4);
			s8 = s6-2871.0/4480.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,6)+32.0/315.0*A_*a[2]*x0*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*pow(r,6)+B_*a[3]*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,33))*pow(r,10)-B_*a[2]*x0*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16)*pow(r,9)/2+B_*a[1]*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16)*pow(r,9)/4+143.0/32768.0*A_*a[3]*pow(k1_,9)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,9);
			s7 = s8-B_*a[2]*x0*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,29))*pow(r,10)/4-B_*a[3]*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,31))*pow(r,9)/8+3.0/4.0*B_*a[3]*pow(x0,2)*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,16)*pow(r,9)+2871.0/8960.0*A_*a[1]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,25))*pow(r,6)-35.0/256.0*A_*a[2]*x0*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,10)-35.0/128.0*A_*a[2]*x0*k1_*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,13)*pow(r,9)-2717.0/8192.0*A_*a[2]*x0*pow(k1_,7)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,19))*pow(r,9)-185.0/3072.0*A_*a[3]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,23))*pow(r,9);
			s8 = s7-1001.0/8192.0*A_*a[1]*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,19))*pow(r,8)-715.0/8064.0*A_*a[1]*pow(k1_,3)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,27))*r*r*r-5313.0/4096.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,9)-3.0/16.0*B_*a[3]*pow(x0,2)*pow(k1_,3)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,15)*pow(r,9)+1771.0/2048.0*A_*a[2]*x0*pow(k1_,5)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,21))*pow(r,9)+1365.0/1024.0*A_*a[3]*pow(x0,2)*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,23))*pow(r,9)+4.0/21.0*A_*a[2]*x0*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,31))*r*r;
			s3 = s8+2079.0/2048.0*A_*a[3]*pow(x0,2)*pow(k1_,5)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,11)*pow(r,9)-65.0/336.0*A_*a[2]*x0*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,29))*r*r+15.0/512.0*A_*a[3]*pow(k1_,2)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,23))*pow(r,8)-715.0/6144.0*A_*a[2]*x0*pow(k1_,6)*sqrt(pow(r*r+k2_+k1_*r,3))*sqrt(pow(k2_,21))*pow(r,6)-143.0/65536.0*A_*a[3]*pow(k1_,9)*atanh((2.0*k2_+k1_*r)/sqrt(k2_)/sqrt(r*r+k2_+k1_*r)/2)*pow(k2_,10)*pow(r,9)-155.0/1024.0*A_*a[3]*pow(k1_,3)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25))*pow(r,9)-11.0/4096.0*A_*a[3]*pow(k1_,5)*sqrt(pow(r*r+k2_+k1_*r,5))*sqrt(pow(k2_,21))*pow(r,7)-15.0/512.0*A_*a[3]*pow(k1_,2)*sqrt(r*r+k2_+k1_*r)*sqrt(pow(k2_,25))*pow(r,10);
			s4 = 1/sqrt(pow(k2_,35))/pow(r,9);
			s2 = s3*s4;
			double t0 = s1+s2;

			if (BALL::Maths::isNan(t0))
			{
				Log.warn() << "Return value is NaN." << endl;
				Log.warn() << "Error occured while integrating [" << r << ","
					<< R << ") - " << x0 << endl
					<< "Coefs: " << a[0] << " " << a[1] << " " << a[2] << " " 
					<< a[3] << endl;
				dump();
			}

			return t0;
		}

	}


	void Pair6_12RDFIntegrator::dump(ostream& stream, Size /* depth */) const
	throw()
	{
		stream << "[Pair6_12RDFIntegrator:]" << endl;
		stream << "A_ = " << A_ << endl;
		stream << "B_ = " << B_ << endl;
		stream << "k1_ = " << k1_ << endl;
		stream << "k2_ = " << k2_ << endl;
		getRDF().dump();
	}


	double Pair6_12RDFIntegrator::project(double x) const throw()
	{
		// k2_ is always > 0, so we don't have to fabs() here
		if (k2_ < MIN_DISTANCE)
		{
			Log.warn() << "project called with k2_ == " << k2_ 
				<< ". Something seemeingly went wrong." << endl;
			return x;
		}

		double arg = x*x + k1_ * x + k2_;
		if (arg < 0)
		{
			Log.error() << "Pair6_12RDFIntegrator::project(): " 
				<< "square root of negative term!" << endl;
			dump();
			return 0.0;
		}
		else
		{
			return sqrt(arg);
		}
	}


	double Pair6_12RDFIntegrator::unproject(double x) const throw()
	{
		// k2_ is always > 0, so we don't have to fabs() here
		if (k2_ < MIN_DISTANCE)
		{
			Log.warn() << "unproject called with k2_ == " << k2_ 
				<< ". Something seemeingly went wrong." << endl;
			return x;
		}

		double arg = x*x + k1_*k1_ / 4 - k2_;
		if (arg < 0)
		{
			Log.error() << "Pair6_12RDFIntegrator::unproject(): " 
				<< "square root of negative term! " << x << endl;
			dump();
			return 0.0;
		}
		else
		{
			return sqrt(arg) - k1_ / 2;
		}
	}


} // namespace BALL
