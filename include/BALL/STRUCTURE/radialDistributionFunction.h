// $Id: radialDistributionFunction.h,v 1.5 2000/10/18 13:55:49 anker Exp $

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
		{\bf Definition:} \{URL:BALL/STRUCTURE/RadialDistributionFunction.h}
	 */
	class RadialDistributionFunction 
	{

		public:

		
		BALL_CREATE(RadialDistributionFunction)

		/** @name Constructors and Destructor 
		 */
		//@{

		/// Default constructor
		RadialDistributionFunction() throw();

		/// Copy constructor
		RadialDistributionFunction(const RadialDistributionFunction& rdf)
			throw();

		/// Detailed constructor
		RadialDistributionFunction(const PiecewisePolynomial& polynomial)
			throw();
		
		/// Destructor
		virtual ~RadialDistributionFunction() throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const RadialDistributionFunction& operator = 
			(const RadialDistributionFunction& rdf) throw();

		/// Clear function
		virtual void clear() throw();

		//@}


		/** @name Accessors
		 */
		//@{

		/** Set the representing PiecewisePolynomial */
		void setRepresentation(const PiecewisePolynomial& polynomial) throw();

		/** Get the representation of the RDF */
		const PiecewisePolynomial& getRepresentation() const throw();

		/** Get the range of the representing function */
		const Interval& getRange() const throw();

		/** compute the value of the distribution function at a given x */
		virtual double operator () (double x) const throw();

		//@}


		/** @name Predicates */
		//@{

		/** Check, whether a given x is in the range of the RDF. */
		bool isInRange(double x) const throw();

		/** check validity */
		bool isValid() const throw();

		/** Equality operator */
		bool operator == (const RadialDistributionFunction& rdf) const throw();

		//@}


		/** Debugging and Diagnostics */
		//@{

		/** 	Dumps the whole content of the object */
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}


		protected:

		/*_ The representation of the RDF */
		PiecewisePolynomial representation_;
		/*_ The valid flag */
		bool valid_;

	};

}


#endif // BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
