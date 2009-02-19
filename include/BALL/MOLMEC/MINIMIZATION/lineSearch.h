// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineSearch.h,v 1.20.18.3 2007/08/07 09:12:34 aleru Exp $
//

// Line Search Minimizer: A special class for the line search minimization algorithm

#ifndef BALL_MOLMEC_MINIMIZATION_LINESEARCH_H
#define BALL_MOLMEC_MINIMIZATION_LINESEARCH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class EnergyMinimizer;

	/** Basic line search class.
	 *  This method minimizes the energy of a system along a given
	 *  direction using a two stage algorithm. It is based on
	 *  cubic and quadratic interpolation methods of Jorge J. More
	 *  and David J. Thuente. See
	 *
	 *  [1] J. More and D. Thuente, "Line search algorithms with guaranteed
	 *  sufficient decrease," ACM Transactions on Mathematical Software 20
	 *  (1994), no. 3, pp. 286-307.
	 *
	 *  \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT LineSearch
	{
		public:
			
			/** @name Constructors and Destructors
			*/
			//@{
			
			BALL_CREATE(LineSearch)
			
			/** Default constructor.
			*/
			LineSearch();
			
			/** Detailed constructor.
			*/
			LineSearch(EnergyMinimizer& minimizer);
			
			/** Copy constructor
			*/
			LineSearch(const LineSearch& line_search);
			
			/** Destructor.
			*/
			virtual ~LineSearch() ;
			
			//@}
			/** @name Assignments
			*/
			//@{
			
			/** Assignment operator
			*/
			const LineSearch& operator=(const LineSearch& LineSearch);
			
			//@}
			/** @name Accessors
			*/
			//@{
			
			/** Set the parameter alpha (convergence criterion for the line search).
			 *  @see minimize
			*/
			void setAlpha(double alpha);
			
			/** Set the parameter beta (convergence criterion for the line search).
			 *  @see minimize
			*/
			void setBeta(double beta);
			
			/** Get the parameter alpha (convergence criterion for the line search).
			 *  @see minimize
			*/
			double getAlpha() const;
			
			/** Get the parameter beta (convergence criterion for the line search).
			 *  @see minimize
			*/
			double getBeta() const;
			
			/** Get the parameter max_steps.
			*/
			Size getMaxSteps() const;
			
			/** Set the parameter max_steps.
			*/
			void setMaxSteps(Size steps);
			
			/** Set the lower bound on energy values. An estimation for the maximum
			 *  step length is computed based on this value.
			 */
			void setLowerBound(double lbound);
			
			/** Get the lower bound on energy values.
			*/
			double getLowerBound() const;
			
			/** Set the nonnegative relative tolerance for an acceptable step length.
			*/
			void setXTol(double xtol);
			
			/** Get the nonnegative relative tolerance for an acceptable step length.
			 */
			double getXTol() const;
			
			/** Set the flag <tt>is_bracketed_</tt>.
			 *  The algorithm will act as if a
			 *  minimizer has already been bracketed (true) or not (false). Warning:
			 *  this can be useful if a user exactly knows what they are doing. Usually,
			 *  this flag should not be touched! The algorithm sets this flag automatically
			 *  if a minimizer could be bracketed.
			 */
			void setBracketedFlag(bool bracktd);
		
			/** Return whether a minimizer has already been bracketed. Warning: this returns
			 *  only the flag <tt>is_bracketed_</tt>. If this flag has not been set by the algorithm
			 *  but changed by <tt>setBracketedFlag</tt> this might not be trustworthy!
			 */
			bool isBracketed() const;
			
			/** Set the minimizer class which provides the search direction and
			 *  the force field among other things.
			 */
		void setMinimizer(EnergyMinimizer& minimizer);
			
			/** Computes a safeguarded step for a search procedure by case differentiation
			 *  dependend on whether a minimum could already be bracketed or not. All
			 *  computations are done by safeguarded quadratic and cubic interpolation.
			 *  The interval that contains a step that satisfies a sufficient decrease and
			 *  the curvature condition is updated. This function is based on the proposed step
			 *  computation of Jorge J. More and David J. Thuente. Don't worry if interval bounds
			 *  are changed after this routine exits.
			 *
			 *  @param st_a Best step obtained so far and an endpoint of the interval that contains the minimizer.
			 *  @param f_a Energy value at <tt>st_a</tt>.
			 *  @param g_a Directional derivative at <tt>st_a</tt>.
			 *  @param st_b Second endpoint of the interval that contains the minimizer.
			 *  @param f_b Energy value at <tt>st_b</tt>.
			 *  @param g_b Directional derivative at <tt>st_b</tt>.
			 *  @param stp Current step. If <tt>is_bracketed_</tt> is set to <tt>true</tt> then <tt>stp</tt>
			 *    must be between <tt>st_a</tt> and <tt>st_b</tt>.
			 *  @param f Energy value at <tt>stp</tt>.
			 *  @param g Directional derivative at <tt>stp</tt>.
			 *  @param minstp Lower bound for the step.
			 *  @param maxstp Upper bound for the step.
			 */
			virtual void takeStep(double &st_a, double &f_a, double &g_a, double &st_b, double &f_b, 
								 double &g_b, double &stp, double f, double g, double minstp, double maxstp);
			
			//@}
			/** @name Minimization
			*/
			//@{
			
			/** Perform a line search.
			 *  Find the minimum position for all atoms along the direction provided by the minimization algorithm.
			 *  A two stage algorithm is used proposed by Jorge J. More and David J. Thuente.
			 *  If necessary (the convergence criterion is not fulfilled) a minimizer along
			 *  the given direction is bracketed in the first stage. In the second stage this
			 *  function looks for a minimizer whithin the bracketed interval.
			 *  The routine exits
			 *
			 *  (1) if the strong Wolfe conditions are satisfied (convergence criterion for the line search),
			 *      that are
			 *          \f[E_{k+1} \leq E_k + \alpha\cdot stp\cdot<g_k, d_k>\f] 
			 *      and
			 *          \f[|<g_{k+1}, d_k>| \leq \beta\cdot |<g_k, d_k>|\f]
			 *      where \f$g_k\f$ and \f$g_{k+1}\f$ are the initial and the current gradient and \f$d_k\f$ is the
			 *      search direction, \f$E_{k+1}\f$ is the current and \f$E_k\f$ the initial energy (stp = 0),
			 *      \f$\alpha\f$ and \f$\beta\f$ are two parameters (usually 0.9 and 0.0001).
			 *
			 *  (2) if the relative tolerance for an acceptable step is reached
			 *      (length of the bracketing interval).
			 *
			 *  (3) if there has been some numerical problems. In this case the best step length which could
			 *      be found is returned.
			 *
			 *  @param stp Unused on entry since the line search always starts with stp = 1.0. On exit
			 *    the step length corresponding to a successful step or just the best this function was
			 *    able to find.
			 *  @param keep_gradient If <tt>true</tt>, we will not calculate the gradient for stp = 1 but
			 *    assume that this has already been performed instead.
			 */
			virtual bool minimize(double& stp, bool keep_gradient = false);
			//@}
			
		protected:
			
			/*_	Parameter alpha for line search criterion.
			*/
			double alpha_;
			
			/*_	Parameter beta for line search criterion.
			*/
			double beta_;
			
			/*_	Parameter for the number of interpolation steps
			*/
			Size max_steps_;
			
			/*_	Lower bound for energy values.
			*/
			double lower_energy_bound_;
			
			/*_	Nonnegative relative tolerance for an acceptable step.
			*/
			double stptol_;
			
			/*_	Specifies whether a minimizer has already been bracketed or not.
			*/
			bool is_bracketed_;
			
			/*_	Calling minimizer class which provides the search direction among other things.
			*/
			EnergyMinimizer* minimizer_;
		
	};
	
} // namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION/LINESEARCH_H
