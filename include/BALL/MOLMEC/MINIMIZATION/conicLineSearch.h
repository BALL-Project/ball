// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conicLineSearch.h,v 1.1.4.2 2007/05/16 15:56:11 aleru Exp $
//

// Line Search Minimizer: A special class for the line search minimization algorithm

#ifndef BALL_MOLMEC_MINIMIZATION_CONICLINESEARCH_H
#define BALL_MOLMEC_MINIMIZATION_CONICLINESEARCH_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class EnergyMinimizer;
	
	// AR: Note: this class is not derived from LineSearch since this class is
	// meant for replacing LineSearch after several tests.

	/** Line search class.
	 *  This method minimizes the energy of a system along a given
	 *  direction using a two stage algorithm. Usual line searches are based
	 *  on pure cubic and quadratic interpolation methods, see e.g.
	 *
	 *  [1] J. More and D. Thuente, "Line search algorithms with guaranteed
	 *  sufficient decrease," ACM Transactions on Mathematical Software 20
	 *  (1994), no. 3, pp. 286-307.
	 *
	 *  This line search is designed for potential functions. It uses the fact
	 *  that near a local minimum the usual potential functions are very well
	 *  quadratically approximable (due to their force terms). However, in the
	 *  case where two atoms are very close the dominating terms are more
	 *  rational than polynomial. So this line search is based on the idea of
	 *  different interpolation schemes as e.g. conic interpolation. 
	 *  The idea is "do interpolations with the most complicated models first,
	 *  decide which result is the most trustworthy" and if all these 
	 *  interpolations fail (in a certain sense) "try the next easier model 
	 *  down to the easiest model until we obtain a trustworthy result".
	 *  The interpolation schemes are
	 *   (1) conic interpolation
	 *   (2) cubic interpolation
	 *   (3) quadratic interpolation with one directional derivative
	 *   (4) quadratic interpolation with two directional derivatives
	 *   (5) bisection
	 *  The idea of conic interpolation is based on
	 *
	 *  [2] W. C. Davidon, "Conic Approximations and Collinear Scalings for 
	 *  Optimizers", SIAM Journal on Numerical Analysis, Vol. 17, No. 2 
	 *  (Apr., 1980), pp. 268-281 
	 *
	 *  [3] P. Bjorstad and J. Nocedal, "Analysis of a New Algorithm for 
	 *  One-Dimensional Minimization", Computing 22, 93-100 (1979).
	 *
	 *  Caution: The algorithm in [3] is wrong!
	 *
	 *  \ingroup  MolmecEnergyMinimizer
	 */
	class BALL_EXPORT ConicLineSearch
	{
		public:
			
			/** @name Constructors and Destructors
			*/
			//@{
			
			BALL_CREATE(ConicLineSearch)
			
			/** Default constructor.
			*/
			ConicLineSearch();
			
			/** Detailed constructor.
			*/
			ConicLineSearch(EnergyMinimizer& minimizer);
			
			/** Copy constructor
			*/
			ConicLineSearch(const ConicLineSearch& line_search);
			
			/** Destructor.
			*/
			virtual ~ConicLineSearch() throw();
			
			//@}
			/** @name Assignments
			*/
			//@{
			
			/** Assignment operator
			*/
			const ConicLineSearch& operator=(const ConicLineSearch& ConicLineSearch);
			
			//@}
			/** @name Accessors
			*/
			//@{
			
			/** Set the parameter alpha.
			*/
			void setAlpha(double alpha);
			
			/** Set the parameter beta.
			*/
			void setBeta(double beta);
			
			/** Get the parameter alpha.
			*/
			double getAlpha() const;
			
			/** Get the parameter beta.
			*/
			double getBeta() const;
			
			/** Get the parameter max_steps.
			*/
			Size getMaxSteps() const;
			
			/** Set the parameter max_steps.
			*/
			void setMaxSteps(Size steps);
			
			/** Set the lower bound on energy values.
			 */
			void setLowerBound(double lbound);
	
			/** Get the lower bound on energy values.
			 */
			double getLowerBound() const;
			
			/** Set the flag <tt>is_bracketed_</tt>, i.e. the algorithm will act as if a
			 *  minimizer has already been bracketed (true) or not (false). Warning:
			 *  this can be useful if a user exactly knows what he is doing. Usually,
			 *  this flag should not be touched! The algorithm sets this flag automatically
			 *  if a minimizer could be bracketed.
			 */
			void setBracketedFlag(bool bracktd);
		
			/** Return whether a minimizer has already been bracketed. Warning: this returns
			 *  only the flag <tt>is_bracketed_</tt>. If this flag was not set by the algorithm
			 *  but changed by <tt>setBracketedFlag</tt> this might not be trustworthy!
			 */
			bool isBracketed() const;
			
			/** Set the minimizer class which provides the search direction and
			 *  the force field among other things.
			 */
			void setMinimizer(EnergyMinimizer& minimizer);
			
			/** Computes a safeguarded step for a search procedure by case differentiation
			 *  dependend on whether a minimum could already be bracketed or not.
			 *  All computations are by interpolation.
			 *  The idea is "do interpolations with the most complicated models first,
			 *  decide which result is the most trustworthy" and if all these 
			 *  interpolations fail (in a certain sense) "try the next easier model 
			 *  down to the easiest model until we obtain a trustworthy result".
			 *  The interpolation schemes are
			 *   (1) conic interpolation
			 *   (2) cubic interpolation
			 *   (3) quadratic interpolation with one directional derivative
			 *   (4) quadratic interpolation with two directional derivatives
			 *   (5) bisection
			 *
			 *  @param stp_lo Lower bound for the step
			 *  @param f_lo Objective value at 'stp_lo'
			 *  @param g_lo Directional derivative at 'stp_lo'
			 *  @param stp_up Upper bound for the step
			 *  @param f_up Objective value at 'stp_up'
			 *  @param g_up Directional derivative at 'stp_up'
			 */
			virtual double takeStep(double stp_lo, double f_lo, double g_lo,
															double stp_up, double f_up, double g_up) const;
			
			//@}
			/** @name Minimization
			*/
			//@{
			
			/** Perform the line search.
			 *  Find the minimum position for all atoms along direction.
			 *  A two stage algorithm is used, simliar to that proposed in [1].
			 *  If necessary (the convergence criterion is not fulfilled) a minimizer along
			 *  the given direction is bracketed in the first stage. In the second stage this
			 *  function looks for a minimizer whithin the bracketed interval.
			 *  The routine exits
			 *
			 *  (1) if the weak Wolfe conditions are satisfied (convergence criterion for the line search),
			 *      that are
			 *          $E_{k+1} \leq E_k + \alpha$ <tt>stp</tt> $<g_k, d_k>$ 
			 *      and
			 *          $<g_{k+1}, d_k> \geq \beta <g_k, d_k>$
			 *      where $g_k$ and $g_{k+1}$ are the initial and the current gradient and $d_k$ is the
			 *      search direction, $E_{k+1}$ is the current and $E_k$ the initial energy (stp = 0),
			 *      $\alpha$ and $\beta$ are two parameters (usually 0.9 and 0.0001).
			 *
			 *  (2) if there were some numerical problems. In this case false is returned.
			 *
			 *  @param stp Unused on entry since the line search always computes its own first estimate
			 *    (usually stp = 1.0). On exit the step length corresponding to a successful step or
			 *    just the best this function was able to find.
			 */
			virtual bool minimize(double& stp);
			//@}
			
		protected:
			
			/*_ Computes a safeguarded step by the data computed by 'takeStep'.
					If the step is accepted 'true' is returned and 'stp' contains the 
					safeguarded step.
			*/
			virtual bool compAndSafeguardStep_(double stp_l, double stp_u, double den, double& stp) const;
			
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
			
			/*_	Specifies whether a minimizer has already been bracketed or not.
			*/
			bool is_bracketed_;
			
			/*_ Control step (old trial step for which a value of the
			    objective function already exists).
			*/
			double ctrl_stp_;
			
			/*_ Value at ctrl_stp_.
			*/
			double ctrl_val_;
			
			/*_ Maximum stepsize.
			*/
			double maxstp_;
			
			/*_	Calling minimizer class which provides the search direction among other things.
			*/
			EnergyMinimizer* minimizer_;
		
	};
	
} // namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION_CONICLINESEARCH_H
