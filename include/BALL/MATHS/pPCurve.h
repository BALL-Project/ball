// $Id: pPCurve.h,v 1.3 2000/07/13 16:27:05 anker Exp $

#ifndef BALL_MATHS_PPCURVE_H
#define BALL_MATHS_PPCURVE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

namespace BALL {

	/** Piecewise polynomial curve object.
		This class provides a trivial implementation of piecewise polynomial
		curves. More useful spline/B-spline implementations might follow in the
		future. The PPCurves are of the form 
		$\sum_{i=0}^{d} a_i^{(k)} (x - x_0^{(k)})^i$, where $k$ is the index of
		the interval and $d$ is the overall degree of the PPCurve. Note that
		there is no warranty that the defined curve ist continuously
		differentiable or even continuous.
		{\bf Definition:} |URL{BALL/MATHS/pPCurve.h}
	 */

	class PPCurve
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/// Default constructor
		PPCurve();

		/// Copy constructor
		PPCurve(const PPCurve& ppcurve);

		/// Detailed Constructor
		PPCurve(Size degree, Size number_of_intervals, vector<float>& intervals,
				vector< vector<float> >& coefficients);

		/// Destructor
		virtual ~PPCurve();

		/// 
		void clear();

		///
		void destroy();

		//@}

		/** @name Accessors
		*/
		//@{

		/// set the degree of the ppcurve
		void setDegree(Size degree);

		/// get the degree of the ppcurve
		Size getDegree() const;

		/// set the interval limits of the ppcurve 
		void setIntervals(const vector<float>& intervals);

		/// get the interval limits of the ppcurve
		vector<float> getIntervals() const;

		/// set the coefficients of the ppcurve 
		void setCoefficients(const vector< vector<float> >& coefficients);

		/// get the coefficients of the ppcurve
		vector< vector<float> > getCoefficients() const;

		/// check if the boundaries are consistent
		void checkValidity();

		/// compute the value of the PPCurve at a given x
		float eval(float x);

		//@}

		protected:

		Size degree_;
		Size number_of_intervals_;
		vector<float> intervals_;
		vector< vector<float> >	coefficients_;
		bool valid_;

		private:

		float sum(float x, Size index);

	};

}

#endif // BALL_MATHS_PPCURVE_H
