// $Id: pair6_12RDFIntegrator.h,v 1.8 2001/03/05 01:55:44 amoll Exp $

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

// BAUSTELLE: DOCU ... documentation for constants

namespace BALL
{
	/** 6-12 pair potential integrator.
			This class provides a tool for calculating the integral part of the
			van-der-Waals interaction energies. The difference to the values
			calculated with the standard interaction energy processor is the
			inclusion of a radial distribution function 
			(@see	RadialDistributionFunction) into the computaion.\\
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
			/** The integration method.
					Use integer values with this option.
					@see Default::METHOD
					@param integration_method integer
			 */
			static const char* METHOD;

			/** This option sets the number of samples to be used in numerical integration.
					Use integer values with this option.
					@see Default::SAMPLES
					@param samples integer
			 */
			static const char* SAMPLES;

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
			
			/** Default integration method.
					@see Option::METHOD
			 */
			static const int METHOD;

			/** Default number of inetgartion samples.
					@see Option::SAMPLES
			 */
			static const int SAMPLES;
		};

		/** The available methods for integration 
		*/
		enum IntegrationMethod
		{
			METHOD__UNKNOWN = 0,
			METHOD__ANALYTICAL = 1,
			METHOD__TRAPEZIUM = 2
		};

		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor 
		*/
		Pair6_12RDFIntegrator() 
			throw();

		/** Copy constructor 
				@param integrator the integrator we want to copy
		*/
		Pair6_12RDFIntegrator(const Pair6_12RDFIntegrator& integrator) 
			throw();

		/** Detailed constructor
				@param A repulsion constant of the 6-12 potential in units of ...
				@param B dispersion constant of the 6-12 potential in units of ...
				@param k1 geometric correction constant
				@param k2 geometric correction constant
				@param rdf a radial distribution function
		*/
		Pair6_12RDFIntegrator(double A, double B, double k1, double k2,
				const RadialDistributionFunction& rdf) 
			throw();

		/** Destructor 
		*/
		virtual ~Pair6_12RDFIntegrator() 
			throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
				@param integrator the integrator to assign from
				@return a constant reference to {\rm this}
		*/
		const Pair6_12RDFIntegrator& operator =
			(const Pair6_12RDFIntegrator& integrator) 
			throw();

		/** Clear method 
		*/
		virtual void clear() 
			throw();

		//@}
		/// @name Accessors 
		//@{

		/** Set the constants for the integration, usually done by the calling energy processor.
				@param A repulsion constant of the 6-12 potential in units of ...
				@param B dispersion constant of the 6-12 potential in units of ...
				@param k1 geometric correction constant
				@param k2 geometric correction constant
		*/
		void setConstants(double A, double B, double k1, double k2) 
			throw();

		/** Get the constants from this processor.
				@param A repulsion constant of the 6-12 potential in units of ...
				@param B dispersion constant of the 6-12 potential in units of ...
				@param k1 geometric correction constant
				@param k2 geometric correction constant
		*/
		void getConstants(double& A, double& B, double& k1, double& k2)	
			throw();

		/** Integrate to Infinity from {\tt from} using previously set constants 
				@param from the lower limit of integration
				@return the value of the integration
		*/
		double integrateToInf(double from) const 
			throw();

		/** Integrate from {\tt from} to infinity using the specified constants 
				@param A repulsion constant of the 6-12 potential in units of ...
				@param B dispersion constant of the 6-12 potential in units of ...
				@param k1 geometric correction constant
				@param k2 geometric correction constant
				@param from the lower limit of integration
				@return the value of the integration
		*/
		double integrateToInf(double from, double A, double B, double k1,
				double k2) 
			throw();

		/** Integrate from {\tt from} to {\tt to} using previously assigned
				constants 
				@param from the lower limit
				@param to the upper limit 
				@return the value of the integration
		*/
		double integrate(double from, double to) const 
			throw();
		
		/** Integrate from {\tt from} to {\tt to} using the specified constants 
				@param from the lower limit of integration
				@param to the upper limit 
				@param A repulsion constant of the 6-12 potential in units of ...
				@param B dispersion constant of the 6-12 potential in units of ...
				@param k1 geometric correction constant
				@param k2 geometric correction constant
				@return the value of the integration
		*/
		double integrate(double from, double to, double A, double B, double k1,
				double k2) 
			throw();

		/** Default operation, integrate from {\tt x} to infinity using
				previously assigned constants.
				@param x the lower limit of the integration to infinity
				@return the value of the integration
		 */
		virtual double operator () (double x) const 
			throw();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator. Tests whether two instances of
				Pair6_12RDFIntegrator have the same content.
				@param integrator another instance of Pair6_12RDFIntegrator
				@return true, if both instances are equal
		*/
		bool operator == (const Pair6_12RDFIntegrator& integrator) const 
			throw();

		//@}

		/** The options of this Integrator (@see Options) 
		*/
		Options options;


		/** @name Debugging and diagnostics 
		*/
		//@{

		/** Dumps the whole content of the object
				@param s an ostream, defaults to std::cout
				@param depth the indentation depth of the output
		*/
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const
			throw();
		
		//@}

		protected:

		/*_ Repulsion constant 
		*/
		double A_;

		/*_ Dispersion constant 
		*/
		double B_;

		/*_ Geometry constant 
		*/
		double k1_;

		/*_ Geometry constant 
		*/
		double k2_;


		private:

		/*_ Integrate an interval analytically. This method does the actual work. 
				@param interval the interval to be integrated
				@param coeffs the coefficients for this interval
				@param x0 the value to be subtracted from x
				@return the value of the integral
		*/
		double analyticallyIntegrateInterval(const Interval& interval,
				const Coefficients& coeffs, float x0) const 
			throw();

		/*_ Integrate an interval numerically.
				@param interval the interval to be integrated
				@return the value of the integral
		*/
		double numericallyIntegrateInterval(const Interval& interval) const
			throw();

		/*_ Project a number from the integration beam to the projection beam
				of an atom center for the rdf thingy. 
				@param x the value to be projected
				@return the projection of {\em x}
		*/
		double project(double x) const throw();

		/*_ Do the reverse of project(). 
				@param x the valut to be reversly projected
				@return the projection of {\em x}
		*/
		double unproject(double x) const throw();
	};

} // namespace BALL

#endif //  BALL_SOLVATION_PAIR6_12RDFINTEGRATOR_H
