// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: pairExpRDFIntegrator.h,v 1.24 2005/12/23 17:02:00 amoll Exp $
//

#ifndef BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
#define BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_STRUCTURE_RDFINTEGRATOR_H
# include <BALL/STRUCTURE/RDFIntegrator.h>
#endif

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
# include <BALL/STRUCTURE/piecewiseFunction.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
# include <BALL/DATATYPE/options.h>
#endif

namespace BALL
{
	/** 6-exp pair potential integrator.
			This class provides a tool for calculating the integral part of the
			van-der-Waals interaction energies. The difference to the values
			calculated with the standard interaction energy processor is the
			inclusion of a radial distribution function  \link RadialDistributionFunction RadialDistributionFunction \endlink  
			into the computaion.  \par
		\ingroup Solvation	
	 */

	class BALL_EXPORT PairExpRDFIntegrator
		:	public RDFIntegrator
	{

		public:

		BALL_CREATE(PairExpRDFIntegrator)

		/** Symbolic names for option keys.
				This struct contains a symbolic name for each recognized key in
				PairExpRDFIntegrator::options.
		 */
		struct BALL_EXPORT Option
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

		struct BALL_EXPORT Default
		{
			/** Default verbosity level.
					@see Option::VERBOSITY
			 */
			static const int VERBOSITY;
			/** Default number of samples to use in numerical integration
					@see Option::SAMPLES
			*/
			static const int SAMPLES;
		};


		/** @name Constructors and destructors 
		*/
		//@{

		/** Default constructor
		*/
		PairExpRDFIntegrator();

		/** Copy constructor.
				@param integrator the integrator to copy construct from
		*/
		PairExpRDFIntegrator(const PairExpRDFIntegrator& integrator);

		/** Detailed constructor 
				@param alpha potential constant
				@param C1 potential constant
				@param C2 potential constant
				@param R_ij_o potential constant
				@param k1 geometric correction constant
				@param k2 geometric correction constant
				@param rdf a radial distribution functin  \link RadialDistributionFunction RadialDistributionFunction \endlink  
		*/
		PairExpRDFIntegrator(double alpha, double C1, double C2, double R_ij_o,
				double k1, double k2, const RadialDistributionFunction& rdf);

		/** Destructor 
		*/
		virtual ~PairExpRDFIntegrator();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
				@param integrator the integrator to assign from
				@return a const reference to this
		*/
		const PairExpRDFIntegrator& operator = 
			(const PairExpRDFIntegrator& integrator) ;

		/** Clear method 
		*/
		virtual void clear();

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Set the potential and geometric correction constants of this
				instance of PairExpRDFIntegrator
				@param alpha potential constant
				@param C1 potential constant
				@param C2 potential constant
				@param R_ij_o potential constant
				@param k1 geometric correction constant
				@param k2 geometric correction constant
		*/
		void setConstants(double alpha, double C1, double C2, double R_ij_o,
				double k1, double k2);

		/** Get the potential and geometric correction constants of this
				instance of PairExpRDFIntegrator
				@param alpha potential constant (set by this function)
				@param C1 potential constant (set by this function)
				@param C2 potential constant (set by this function)
				@param R_ij_o potential constant (set by this function)
				@param k1 geometric correction constant (set by this function)
				@param k2 geometric correction constant (set by this function)
		*/
		void getConstants(double& alpha, double& C1, double& C2, double& R_ij_o,
				double& k1, double& k2) ;

		/** Integrate to Infinity from <tt>from</tt> using previously set constants 
				@param from the lower limit of integration
				@return the value of the integration
		*/
		double integrateToInf(double from) const;

		/** Integrate from <tt>from</tt> to infinity using the specified constants 
				@param from the lower limit of the integration
				@param alpha potential constant
				@param C1 potential constant
				@param C2 potential constant
				@param R_ij_o potential constant
				@param k1 geometric correction constant
				@param k2 geometric correction constant
				@return the value of the integral
		*/
		double integrateToInf(double from, double alpha, double C1, double C2,
				double R_ij_o, double k1, double k2);

		/** Integrate from <tt>from</tt> to <tt>to</tt> using previously assigned	constants.
				@param from the lower limit
				@param to the upper limit 
				@return the value of the integration
		*/
		double integrate(double from, double to) const ;

		/** Integrate from <tt>from</tt> to <tt>to</tt> using the specified constants 
				@param from the lower limit of integration
				@param to the upper limit 
				@param alpha potential constant
				@param C1 potential constant
				@param C2 potential constant
				@param R_ij_o potential constant
				@param k1 geometric correction constant
				@param k2 geometric correction constant
				@return the value of the integration
		*/
		double integrate(double from, double to, double alpha, double C1, 
				double C2, double R_ij_o, double k1, double k2) ;
		
		/** Default operation, integrate from <tt>x</tt> to infinity using
				previously assigned constants 
				@param x the lower limit of the integration to infinity
				@return the value of the integration
		 */
		virtual double operator () (double x) const;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator. Tests whether two instances of
				PairExpRDFIntegrator have the same content.
				@param integrator another instance of PairExpRDFIntegrator
				@return true, if both instances are equal
		*/
		bool operator == (const PairExpRDFIntegrator& integrator) const;

		//@}

		/** 
		*/
		Options options;

		/** @name Debugging and diagnostics 
		*/
		//@{

		/** Dumps the whole content of the object
				@param s an ostream, defaults to std::cout
				@param depth the indentation depth of the output
		*/
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;
		
		//@}

		protected:

		/*_ potential constant 
		*/
		double alpha_;

		/*_ potential constant 
		*/
		double C1_;

		/*_ potential constant 
		*/
		double C2_;

		/*_ potential constant 
		*/
		double R_ij_o_;

		/*_ geometric correction 
		*/
		double k1_;

		/*_ geometric correction 
		*/
		double k2_;


		private:

		/*_ Integrate an interval numerically.
				@param interval the interval to be integrated
				@return the value of the integral
		*/
		double numericallyIntegrateInterval(Interval interval) const;

		/*_ Project a number from the integration beam to the projection beam
				of an atom center for the rdf thingy. 
				@param x the value to be projected
				@return the projection of <b>  x </b>
		*/
		double project(double x) const;

		/*_ Do the reverse of project(). 
				@param x the valut to be reversly projected
				@return the projection of <b>  x </b>
		*/
		double unproject(double x) const;

	};
   
} // namespace BALL

#endif //  BALL_SOLVATION_PAIREXPRDFINTEGRATOR_H
