// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineSearch.h,v 1.11 2003/02/03 21:38:17 oliver Exp $
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
			Without being a true energy minimizer, this method minimizes
			the energy of a system along a given direction using	
			cubic interpolation. \\
			{\bf Definition:} \URL{BALL/MOLMEC/MINIMIZATION/lineSearch.h}
	*/
	class LineSearch
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

		/**	Set the parameter alpha
		*/
		void setAlpha(double alpha);

		/**	Set the parameter beta
		*/
		void setBeta(double beta);

		/**	Get the parameter alpha
		*/
		double getAlpha() const;

		/**	Get the parameter beta
		*/
		double getBeta() const;

		/**	Get the parameter max_steps
		*/
		Size getMaxSteps() const;

		/**	Set the parameter max_steps
		*/
		void setMaxSteps(Size steps);

		/** Set the minimizer
		*/
		void setMinimizer(const EnergyMinimizer& minimizer);

		/**	Line search criterion.
				This predicate implements the Armijo-Goldstein criterion:
				\begin{itemize}
					\item sufficient decrease of energy:
							$E(i+1) \leq E(i) + \alpha \lambda <g(i), dir>$
					\item sufficient gradient reduction: $|<g(i+1), dir>| \leq \beta <g(i), dir>$
				\end{itemize}
				
				where $g(i)$ and $g(i+1)$ are the initial and the current gradient
				$dir$ is the (normalized) search direction
				$E(i+1)$ is the current and $E(i)$ the initial energy ($\lambda = 0$)
				$\alpha$ and $\beta$ are two parameters (usually 0.9 and 0.0001).
				The line search was successful, if it could determine a value for $\lambda$
				fulfilling this criterion. The function is used in the \Ref{minimize} method.
		*/
		virtual bool isSufficient(double lambda, double current_energy, double current_dir_grad) 
			const;

		/**	Cubic interpolation routine.
				Use a cubic interpolation to estimate the minimum of the function.
				if the function is linear, either {\bf lambda_0} or {\bf lambda_1} is returned
				(the one with the lower energy associated).
				The value returned may otherwise lie outside of the interval defined by {\bf lambda_0} and {\bf lambda_1}.
		*/
		virtual double interpolate	
			(double lambda_0, double lambda_1, 
			 double energy_,  double energy_1, 
			 double grad_0,   double grad_1) const;		
		//@}

		/**	@name	Minimization
		*/
		//@{

		/**	Perform a line search.
				Find the minimum position for all atoms along direction
		*/
		virtual bool minimize(double& lambda, double step = 1.0);

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

		/*_	Search direction.
		*/
		EnergyMinimizer* minimizer_;

		double initial_dir_grad_;
		double initial_energy_;
		double step_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_MINIMIZATION/LINESEARCH_H
