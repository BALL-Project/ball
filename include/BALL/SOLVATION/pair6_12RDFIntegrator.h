// $Id: pair6_12RDFIntegrator.h,v 1.1 2000/09/28 12:14:31 anker Exp $

#ifndef BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H
#define BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL__RDFINTEGRATOR_H
#include <BALL/STRUCTURE/RDFIntegrator.h>
#endif

#ifndef BALL__PIECEWISEFUNCTION_H
#include <BALL/MATHS/piecewiseFunction.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

namespace BALL
{
	/** */

	class Pair6_12RDFIntegrator
		:	public RDFIntegrator
	{

		public:

		BALL_CREATE(Pair6_12RDFIntegrator)

		/** Symbolic names for option keys.
			This struct contains a symbolic name for each recognized key in
			Pair6_12RDFIntegrator::options.
		 */
		struct Option
		{
			/** The verbosity level.
				Use integer values with this option.
				@see Default::VERBOSITY
				@param verbosity integer
			 */
			static const char* VERBOSITY;
		};

		struct Default
		{
			/** Default verbosity level.
				@see Option::VERBOSITY
			 */
			static const int VERBOSITY;
		};

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		Pair6_12RDFIntegrator();

		/** Copy constructor */
		Pair6_12RDFIntegrator(const Pair6_12RDFIntegrator& integrator);

		/** Detailed constructor */
		Pair6_12RDFIntegrator(double A, double B, double k1, double k2,
				const RadialDistributionFunction& rdf);

		/** Destructor */
		virtual ~Pair6_12RDFIntegrator();

		/** Destroy method */
		virtual void destroy();

		/** clear method */
		virtual void clear();

		//@}

		
		/** @name Assignment */
		//@{

		/** Set from another instance */
		void set(const Pair6_12RDFIntegrator& integrator);

		/** Set by specifing this instance */
		void set(double A, double B, double k1, double k2,
				const RadialDistributionFunction& rdf);

		/** Assignment operator */
		const Pair6_12RDFIntegrator& Pair6_12RDFIntegrator::operator =
			(const Pair6_12RDFIntegrator& proc);

		//@}


		/** @name Accessors */
		//@{

		/** */
		void setConstants(double A, double B, double k1, double k2);

		/** */
		double integrateToInf(double from) const;

		/** */
		double integrateToInf(double from, double A, double B, double k1,
				double k2);

		/** */
		double integrate(double from, double to, double A, double B,
				double k1, double k2);
		
		/** */
		double integrate(double from, double to) const;

		/** */
		virtual double operator () (double x) const;

		//@}


		/** */
		Options options;

		/** @name Debugging and diagnostics */
		//@{

		/** Dumps the whole content of the object */
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;
		
		//@}



		protected:

		double A_;
		double B_;
		double k1_;
		double k2_;

		bool valid_;


		private:

		double analyticallyIntegrateInterval(Interval interval,
				Coefficients coeffs, Position index) const;

	};

} // namespace BALL

#endif //  BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H
