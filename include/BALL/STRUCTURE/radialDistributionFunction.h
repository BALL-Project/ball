// $Id: radialDistributionFunction.h,v 1.4 2000/09/25 16:29:10 anker Exp $

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
#define BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H

#ifndef BALL_MATHS_PPOLYNOMIAL_H
#include <BALL/MATHS/piecewisePolynomial.h>
#endif

namespace BALL
{

	/** Radial distribution function.
		This class provides an interface for the use of radial distribution
		functions. These functions describe the deviation of the number density
		in a real fluid from the ideal (isotropic) case. The representation of
		such a function is a piecewise polynomial obtained by smoothing spline
		fit from molecular dynamics simulation.
		\\
		{\bf Definition:} \{URL:BALL/.../RadialDistributionFunction.h}
	 */
	
	class RadialDistributionFunction 
	{

		public:

		
		BALL_CREATE(RadialDistributionFunction)

		/** @name Constructors and Destructors 
		 */
		//@{

		/// Default constructor
		RadialDistributionFunction();

		/// Copy constructor
		RadialDistributionFunction(const RadialDistributionFunction& rdf);

		/// Detailed constructor
		RadialDistributionFunction(const PiecewisePolynomial& polynomial);
		
		/// Destructor
		virtual ~RadialDistributionFunction();

		///
		void clear();

		///
		void destroy();

		//@}


		/** @name Assignment */
		//@{

		/** */
		void set(const RadialDistributionFunction& rdf);

		/** */
		const RadialDistributionFunction& operator = 
			(const RadialDistributionFunction& rdf);

		//@}


		/** @name Accessors
		 */
		//@{

		/** Set the representing PiecewisePolynomial */
		void setRepresentation(const PiecewisePolynomial& polynomial);

		/** Get the representation of the RDF */
		PiecewisePolynomial getRepresentation() const;

		/** Get the range of the representing function */
		Interval getRange() const;

		/** compute the value of the distribution function at a given x */
		virtual double operator () (double x) const;

		//@}


		/** @name Predicates */
		//@{

		/** Check, whether a given x is in the range of the RDF. */
		bool isInRange(double x) const;

		/** check validity */
		bool isValid() const;

		//@}


		/** Debugging and Diagnostics */
		//@{

		/** 	Dumps the whole content of the object */
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;

		//@}


		protected:

		PiecewisePolynomial representation_;
		bool valid_;

	};

}


#endif // BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
