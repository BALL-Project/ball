// $Id: pairExpRDFIntegrator.h,v 1.6 2000/09/25 16:26:47 anker Exp $

#ifndef BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
#define BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_STRUCTURE_RDFINTEGRATOR_H
#include <BALL/STRUCTURE/RDFIntegrator.h>
#endif

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
#include <BALL/STRUCTURE/piecewiseFunction.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#include <BALL/DATATYPE/options.h>
#endif

namespace BALL
{
	/** */

	class PairExpRDFIntegrator
		:	public RDFIntegrator
	{

		public:

		BALL_CREATE(PairExpRDFIntegrator)

		/** Symbolic names for option keys.
			This struct contains a symbolic name for each recognized key in
			ReissCavFreeEnergyProcessor::options.
		 */
		struct Option
		{
			/** The verbosity level.
				Use integer values with this option.
				@see Default::VERBOSITY
				@param verbosity integer
			 */
			static const char* VERBOSITY;

			/** Number of sampling points for the numerical integration.
			@see Default::SAMPLES
			@param samples integer
			*/
			static const char* SAMPLES;
		};

		struct Default
		{
			/** Default verbosity level.
				@see Option::VERBOSITY
			 */
			static const int VERBOSITY;
			static const int SAMPLES;
		};

		/** @name Constructors and destructors */
		//@{

		/** Default constructor */
		PairExpRDFIntegrator();

		/** Copy constructor */
		PairExpRDFIntegrator(const PairExpRDFIntegrator& integrator);

		/** Detailed constructor */
		PairExpRDFIntegrator(double alpha, double C1, double C2, double R_ij_o,
				double k1, double k2, const RadialDistributionFunction& rdf);

		/** Destructor */
		virtual ~PairExpRDFIntegrator();

		/** */
		virtual void destroy();

		/** */
		virtual void clear();

		//@}

		
		/** @name Assignment */
		//@{

		/** */
		void set(const PairExpRDFIntegrator& integrator);

		/** */
		const PairExpRDFIntegrator& operator = 
			(const PairExpRDFIntegrator& integrator);

		//@}


		/** @name Accessors */
		//@{

		/** */
		void setConstants(double alpha, double C1, double C2, double R_ij_o,
				double k1, double k2);

		/** */
		double integrateToInf(double from) const;

		/** */
		double integrateToInf(double from, double alpha, double C1, double C2,
				double R_ij_o, double k1, double k2);

		/** */
		double integrate(double from, double to) const;

		/** */
		double integrate(double from, double to, double alpha, double C1, 
				double C2, double R_ij_o, double k1, double k2);
		
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

		double alpha_;
		double C1_;
		double C2_;
		double R_ij_o_;
		double k1_;
		double k2_;

		bool valid_;


		private:

		double numericallyIntegrateInterval(Interval interval) const;

	};

} // namespace BALL

#endif //  BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
