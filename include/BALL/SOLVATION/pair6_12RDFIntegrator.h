// $Id: pair6_12RDFIntegrator.h,v 1.4 2000/10/17 17:14:35 anker Exp $

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
	/** 6-12 pair potential integrator.
		This class provides a tool for calculating the integral part of the
		van-der-Waals interaction energies. The difference to the values
		calculated with the standard interaction energy processor is the
		inclusion of a radial distribution function (@see
		RadialDistributionFunction) into the computaion.
		\\
		{\bf Definition:} \URL{BALL/SOLVATION/pair6_12RDFIntegrator}
	*/

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
		Pair6_12RDFIntegrator() throw();

		/** Copy constructor */
		Pair6_12RDFIntegrator(const Pair6_12RDFIntegrator& integrator) throw();

		/** Detailed constructor */
		Pair6_12RDFIntegrator(double A, double B, double k1, double k2,
				const RadialDistributionFunction& rdf) throw();

		/** Destructor */
		virtual ~Pair6_12RDFIntegrator() throw();

		//@}

		
		/** @name Assignment */
		//@{

		/** Set by specifing this instance */
		void set(double A, double B, double k1, double k2,
				const RadialDistributionFunction& rdf) throw();

		/** Assignment operator */
		const Pair6_12RDFIntegrator& operator =
			(const Pair6_12RDFIntegrator& proc) throw();

		/** clear method */
		virtual void clear() throw();

		//@}


		/** @name Accessors */
		//@{

		/** set the constants for the integration, usually done by the calling
			energy processor */
		void setConstants(double A, double B, double k1, double k2) throw();

		/** get the constants from this processor */
		void getConstants(double& A, double& B, double& k1, double& k2)
		throw();

		/** integrate to Infinity from {\tt from} using previously set 
			constants */
		double integrateToInf(double from) const throw();

		/** integrate from {\tt from} to infinity using the specified constants */
		double integrateToInf(double from, double A, double B, double k1,
				double k2) throw();

		/** integrate from {\tt from} to {\tt to} using previously assigned
		  constants */
		double integrate(double from, double to) const throw();
		
		/** integrate from {\tt from} to {\tt to} using the specified
		  constants */
		double integrate(double from, double to, double A, double B,
				double k1, double k2) throw();

		/** Default operation, integrate from {\tt x} to infinity using
		 * previously assigned constants */
		virtual double operator () (double x) const throw();

		//@}


		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const Pair6_12RDFIntegrator& integrator) const
			throw();
		//@}


		/** The options of this Integrator */
		Options options;

		/** @name Debugging and diagnostics */
		//@{

		/** Dumps the whole content of the object */
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const
		throw();
		
		//@}



		protected:

		/*_ Repulsion constant */
		double A_;
		/*_ Dispersion constant */
		double B_;
		/*_ Geometry constant */
		double k1_;
		/*_ Geometry constant */
		double k2_;

		/*_ The valid flag. Should move to RDFIntegrator. */
		bool valid_;


		private:

		/*_ Integrate an interval analytically. This method does the actual
		 * work. */
		double analyticallyIntegrateInterval(Interval interval,
				Coefficients coeffs, Position index) const throw();
		/*_ Project a number from the integration beam to the projection beam
		 * of an atom center for the rdf thingy. */
		double project(double x) const throw();
		/*_ Do the reverse of project(). */
		double unproject(double x) const throw();

	};

} // namespace BALL

#endif //  BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H
