// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: piecewisePolynomial.h,v 1.17 2003/08/17 20:13:29 oliver Exp $
//

#ifndef BALL_MATHS_PPOLYNOMIAL_H
#define BALL_MATHS_PPOLYNOMIAL_H

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
# include <BALL/MATHS/piecewiseFunction.h>
#endif

namespace BALL 
{
	/** Piecewise polynomial curve object.
			This class provides a trivial implementation of piecewise polynomial
			curves. More useful spline/B-spline implementations might follow in the
			future. The PPCurves are of the form 
			\f[\sum_{i=0}^{d} a_i^{(k)} (x - x_0^{(k)})^i\f], where \f$k\f$ is the index of
			the interval and \f$d\f$ is the overall degree of the PPCurve. Note that
			there is no warranty that the defined curve is continuously
			differentiable or even continuous.
		\ingroup 	FunctionClasses
	 */

	class PiecewisePolynomial
		:	public PiecewiseFunction
	{
		public:

		BALL_CREATE(PiecewisePolynomial)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default constructor 
		*/
		PiecewisePolynomial() throw();

		/** Copy constructor 
		*/
		PiecewisePolynomial(const PiecewisePolynomial& polynomial) throw();

		/** Detailed Constructor.
				This constructor does <b>not</b> check sanity of the arguments 
		 */
		PiecewisePolynomial(Size degree, const std::vector<Interval>& intervals,
				const std::vector<Coefficients>& coefficients) throw();

		/** Destructor 
		*/
		virtual ~PiecewisePolynomial() throw();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		PiecewisePolynomial& operator = (const PiecewisePolynomial& poly)
			throw();

		/** Clear method 
		*/
		virtual void clear() throw();

		//@}
		/** @name Accessors
		*/
		//@{

		/** Set the instance manually 
		*/
		void set(Size degree, const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs) throw();

		/// set the degree of the polynomial
		void setDegree(Size degree) throw();

		/// get the degree of the polynomial
		Size getDegree() const throw();

		/// compute the value of the PiecewisePolynomial at a given x
		virtual double operator () (double x) const throw();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const PiecewisePolynomial& poly) const throw();

		//@}
		/** @name Debugging and Diagnostics 
		*/
		//@{

		/** Dumps the whole content of the object 
		*/
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const
			throw();

		//@}

		protected:

		/*_ The degree of the polynomial 
		*/
		Size degree_;

	};

}

#endif // BALL_MATHS_PPPOLYNOMIAL_H
