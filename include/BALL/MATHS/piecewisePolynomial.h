// $Id: piecewisePolynomial.h,v 1.2 2000/08/31 20:47:43 oliver Exp $

#ifndef BALL_MATHS_PPOLYNOMIAL_H
#define BALL_MATHS_PPOLYNOMIAL_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
//#include <BALL/MATHS/piecewiseFunction.h>
#include "piecewiseFunction.h"
#endif

namespace BALL 
{

	/** Piecewise polynomial curve object.
		This class provides a trivial implementation of piecewise polynomial
		curves. More useful spline/B-spline implementations might follow in the
		future. The PPCurves are of the form 
		$\sum_{i=0}^{d} a_i^{(k)} (x - x_0^{(k)})^i$, where $k$ is the index of
		the interval and $d$ is the overall degree of the PPCurve. Note that
		there is no warranty that the defined curve ist continuously
		differentiable or even continuous.
		{\bf Definition:} \URL{BALL/MATHS/pPolynomial.h}
	 */

	class PiecewisePolynomial
		:	public PiecewiseFunction
	{
		public:

		BALL_CREATE(PiecewisePolynomial)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/** Default constructor */
		PiecewisePolynomial();

		/** Copy constructor */
		PiecewisePolynomial(const PiecewisePolynomial& polynomial);

		/** Detailed Constructor.
			This constructor does {\bf not} check sanity of the arguments 
		 */
		PiecewisePolynomial(Size degree, const std::vector<Interval>& intervals,
				const std::vector<Coefficients>& coefficients);

		/** Destructor */
		virtual ~PiecewisePolynomial();

		/** Destroy method */
		virtual void destroy();

		/** Clear method */
		virtual void clear();

		//@}


		/** */
		void set(const PiecewisePolynomial& poly);

		/** */
		void set(Size degree, const std::vector<Interval>& intervals,
			const std::vector<Coefficients>& coeffs);
		
		/** */
		PiecewisePolynomial& operator = (const PiecewisePolynomial& poly);


		/** @name Accessors
		*/
		//@{

		/// set the degree of the polynomial
		void setDegree(Size degree);

		/// get the degree of the polynomial
		Size getDegree() const;

		/// compute the value of the PiecewisePolynomial at a given x
		virtual double operator() (double x) const;

		//@}

		protected:

		Size degree_;

	};

}

#endif // BALL_MATHS_PPPOLYNOMIAL_H
