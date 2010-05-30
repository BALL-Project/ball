// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
#define BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H

#ifndef BALL_MATHS_PPOLYNOMIAL_H
# include <BALL/MATHS/piecewisePolynomial.h>
#endif

namespace BALL
{
	/** Radial distribution function.
			This class provides an interface for the use of radial distribution
			functions. These functions describe the deviation of the number density
			in a real fluid from the ideal (isotropic) case. The representation of
			such a function is a piecewise polynomial obtained by smoothing spline
			fit from molecular dynamics simulation.	 \par
	\ingroup RDF	
	*/
	class BALL_EXPORT RadialDistributionFunction 
	{
		public:
		
		BALL_CREATE(RadialDistributionFunction)

		/** @name Constructors and Destructor.
		 */
		//@{

		/** Default constructor.
		*/
		RadialDistributionFunction() 
			;

		/** Copy constructor.
		*/
		RadialDistributionFunction(const RadialDistributionFunction& rdf)
			;

		/** Detailed constructor.
		*/
		RadialDistributionFunction(const PiecewisePolynomial& polynomial)
			;
		
		/** Destructor.
		*/
		virtual ~RadialDistributionFunction() 
			;

		//@}
		/** @name Assignment.
		*/
		//@{

		/** Assignment operator.
		*/
		const RadialDistributionFunction& operator = (const RadialDistributionFunction& rdf)
			;

		/** Clear function.
		*/
		virtual void clear() 
			;

		//@}
		/** @name Accessors.
		 */
		//@{

		/** Set the representing PiecewisePolynomial.
		*/
		void setRepresentation(const PiecewisePolynomial& polynomial) 
			;

		/** Get the representation of the RDF.
		*/
		const PiecewisePolynomial& getRepresentation() const 
			;

		/** Get the range of the representing function.
		*/
		const Interval& getRange() const 
			;

		/** Compute the value of the distribution function at a given x.
		*/
		virtual double operator () (double x) const 
			;

		//@}
		/** @name Predicates.
		*/
		//@{

		/** Check, whether a given x is in the range of the RDF. 
		*/
		bool isInRange(double x) const 
			;

		/** Check validity.
		*/
		bool isValid() const 
			;

		/** Equality operator.
		*/
		bool operator == (const RadialDistributionFunction& rdf) const 
			;

		//@}
		/** Debugging and Diagnostics.
		*/
		//@{

		/** Dumps the whole content of the object.
		*/
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const 
			;

		//@}

		protected:

		/*_ The representation of the RDF.
		*/
		PiecewisePolynomial representation_;

		/*_ The valid flag.
		*/
		bool								valid_;

	};
   
}

#endif // BALL_STRUCTURE_RADIALDISTRIBUTIONFUNCTION_H
