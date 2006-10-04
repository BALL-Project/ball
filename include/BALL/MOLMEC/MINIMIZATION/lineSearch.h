// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineSearch.h,v 1.20.12.4 2006/10/04 16:13:14 aleru Exp $
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

	/**	Basic line search class.
			This method minimizes the energy of a system along a given 
			direction using a two stage algorithm. It is based on
			a cubic or quadratic interpolation method of Jorge J. More 
			and David J. Thuente. See 
	
			J. More and D. Thuente, "Line search algorithms with guaranteed
			sufficient decrease," ACM Transactions on Mathematical Software 20 
			(1994), no. 3, pp. 286-307.
	
	
    	\ingroup  MolmecEnergyMinimizer
	*/
	class BALL_EXPORT LineSearch
	{
		public:

		/**	@name	Constructors and Destructors	
		*/
		//@{
		
		BALL_CREATE(LineSearch)

		/**	Default constructor.
		*/
		LineSearch();

		/**	Detailed constructor.
		*/
		LineSearch(const EnergyMinimizer& minimizer);

		/**	Copy constructor
		*/
		LineSearch(const LineSearch& line_search);

		/**	Destructor.
		*/
		virtual ~LineSearch() 
			throw();

		//@}
		/**	@name	Assignments 
		*/
		//@{

		/**	Assignment operator
		*/
		const LineSearch& operator = (const LineSearch& LineSearch);

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**	Set the parameter alpha.
		*/
		void setAlpha(double alpha);

		/**	Set the parameter beta.
		*/
		void setBeta(double beta);

		/**	Get the parameter alpha.
		*/
		double getAlpha() const;

		/**	Get the parameter beta.
		*/
		double getBeta() const;

		/**	Get the parameter max_steps.
		*/
		Size getMaxSteps() const;

		/**	Set the parameter max_steps.
		*/
		void setMaxSteps(Size steps);
		
		/** Set the lower bound on energy values. An estimation for the maximum 
				step length is computed during the algorithm based on this value.
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
		
		/** Set the flag <tt>is_bracketed_</tt>, i.e. the algorithm will act as if a 
				minimizer has already been bracketed (true) or not (false). Warning: 
				this can be useful if a user exactly knows what he is doing. Usually, 
				this flag should not be touched! The algorithm sets this flag automatically
				if a minimizer could be bracketed.
		*/
		void setBracketedFlag(bool bracktd);
		
		/** Return whether a minimizer has already been bracketed. Warning: this returns 
				only the flag <tt>is_bracketed_</tt>. If this flag was not set by the algorithm
				but changed by <tt>setBracketedFlag</tt> this might not be trustworthy!
		*/
		bool isBracketed() const;

		/** Set the minimizer class which provides the search direction and
				the force field among other things.
		*/
		void setMinimizer(const EnergyMinimizer& minimizer);
		
		/** Computes a safeguarded step for a search procedure by case differentiation 
				dependend on whether a minimum could already be bracketed or not. All
				computations are done by safeguarded quadratic and cubic interpolation.
				The interval that contains a step that satisfies a sufficient decrease and
				the curvature condition is updated. This function is based on the proposed step 
				computation of Jorge J. More and David J. Thuente. Dont't worry if interval bounds
				are changed after this routine exits.
		
				@param st_a Best step obtained so far and an endpoint of the interval that contains the minimizer.
				@param f_a Energy value at <tt>st_a</tt>.
				@param g_a Directional derivative at <tt>st_a</tt>.
				@param st_b Second endpoint of the interval that contains the minimizer.
				@param f_b Energy value at <tt>st_b</tt>.
				@param g_b Directional derivative at <tt>st_b</tt>.
				@param stp Current step. If <tt>is_bracketed_</tt> is set to <tt>true</tt> then <tt>stp</tt>
					must be between <tt>st_a</tt> and <tt>st_b</tt>.
				@param f Energy value at <tt>stp</tt>.
				@param g Directional derivative at <tt>stp</tt>.
				@param minstp Lower bound for the step.
				@param maxstp Upper bound for the step.
		*/
		virtual void takeStep(double &st_a, double &f_a, double &g_a, double &st_b, double &f_b, 
								 double &g_b, double &stp, double f, double g, double minstp, double maxstp);

		//@}
		/**	@name	Minimization
		*/
		//@{

	  /** Perform a line search.
				Find the minimum position for all atoms along direction. 
				A two stage algorithm is used proposed by Jorge J. More and David J. Thuente.
				If necessary (the convergence criterion is not fulfilled) a minimizer along 
				the given direction is bracketed in the first stage. In the second stage this
				function looks for a minimizer whithin the bracketed interval.
				The routine exits 
		
				(1) if the weak Wolfe conditions are satisfied (convergence criterion for the line search), 
						that are
							$E_{k+1} \leq E_k + \alpha$ <tt>stp</tt> $<g_k, d_k>$ 
						and
							$<g_{k+1}, d_k> \geq \beta <g_k, d_k>$
						where $g_k$ and $g_{k+1}$ are the initial and the current gradient and $d_k$ is the 
						search direction, $E_{k+1}$ is the current and $E_k$ the initial energy (stp = 0),
						$\alpha$ and $\beta$ are two parameters (usually 0.9 and 0.0001).
		
				(2) if the relative tolerance for an acceptable step is reached 
						(length of the bracketing interval).
		
				(3) if there were some numerical problems. In this case the best step length which could 
						be found is returned.
		
				@param stp Unused on entry since the line search always starts with stp = 1.0. On exit
					the step length corresponding to a successful step or just the best this function was
					able to find.
				@param scale A scaling parameter for the search direction. Internally <tt>scale</tt> $d_k$
					is used instead of $d_k$.
				@param keep_gradient If <tt>true</tt>, we will not calculate the gradient for stp = 1 but
					assume that this has already been performed instead.
		*/
		virtual bool minimize(double& stp, double scale = 1.0, bool keep_gradient = false);
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
