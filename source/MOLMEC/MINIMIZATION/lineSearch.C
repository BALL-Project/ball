// $Id: lineSearch.C,v 1.3 2000/03/26 12:58:13 oliver Exp $

#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/COMMON/limits.h>

// parameter alpha for 'sufficient energy decrease'
#define LINESEARCH__DEFAULT_ALPHA 0.0001

// parameter beta for 'sufficient gradient reduction'
#define LINESEARCH__DEFAULT_BETA  0.95

// maximum number of interpolation steps for a line search
#define LINESEARCH__DEFAULT_MAX_STEPS			10

namespace BALL 
{
	// default constructor
	LineSearch::LineSearch()
		:	alpha_(LINESEARCH__DEFAULT_ALPHA),
			beta_(LINESEARCH__DEFAULT_BETA),
			max_steps_(LINESEARCH__DEFAULT_MAX_STEPS),
			minimizer_(0)
	{
	}


	// copy constructor 
	LineSearch::LineSearch(const LineSearch& line_search, bool /* deep */)
		:	alpha_(line_search.alpha_),
			beta_(line_search.beta_),
			max_steps_(line_search.max_steps_),
			minimizer_(line_search.minimizer_)
	{
	}

	// assignment operator
	LineSearch& LineSearch::operator = (const LineSearch& line_search)
	{
		alpha_ = line_search.alpha_;
		beta_ = line_search.beta_;
		max_steps_ = line_search.max_steps_;
		minimizer_ = line_search.minimizer_;

		return *this;
	}

	// detailed constructor
	LineSearch::LineSearch(const EnergyMinimizer& minimizer)
		:	alpha_(LINESEARCH__DEFAULT_ALPHA),
			beta_(LINESEARCH__DEFAULT_BETA),
			max_steps_(LINESEARCH__DEFAULT_MAX_STEPS),
			minimizer_(const_cast<EnergyMinimizer*>(&minimizer))
	{
	}

	// destructor
	LineSearch::~LineSearch()
	{
	}
		
	// Set the parameter alpha_
	void LineSearch::setAlpha(double alpha)
	{
		alpha_ = alpha;
	}

	// Get the parameter alpha_
	double LineSearch::getAlpha() const
	{
		return alpha_;
	}

	// Set the parameter beta_
	void LineSearch::setBeta(double beta)
	{
		beta_ = beta;
	}

	// Get the parameter beta_
	double LineSearch::getBeta() const
	{
		return beta_;
	}

	// Set the parameter max_steps_
	void LineSearch::setMaxSteps(Size max_steps)
	{
		max_steps_	= max_steps;
	}

	//	Get the parameter max_steps_
	Size LineSearch::getMaxSteps() const
	{
		return max_steps_;
	}

	void LineSearch::setMinimizer(const EnergyMinimizer& minimizer)
	{
		minimizer_ = const_cast<EnergyMinimizer*>(&minimizer);
	}

	/*	The minimizer optimizes the energy of the system 
			using a modified line search algorithm.
	*/
	bool LineSearch::minimize(double& lambda, double step)
	{
		//Log.info() << "LS:minimize(" << lambda << ", " << step << ")" << endl;
		// check whether a direction and a force field are defined
		if ((minimizer_ == 0) || (minimizer_->getForceField() == 0))
		{
			return false;
		}

		// define some aliases for convenience
		AtomVector&				atoms(const_cast<AtomVector&>(minimizer_->getForceField()->getAtoms()));
		Gradient&					direction(minimizer_->getDirection());
		EnergyMinimizer&	minimizer(*minimizer_);
		Gradient&					gradient(minimizer.getGradient());
		Gradient&					initial_gradient(minimizer.getInitialGradient());


		// if we do not have a valid gradient for the first step,
		// calculate it
		if (!initial_gradient.isValid())
		{
			// reset the atoms to the start position (lambda = 0)
			atoms.resetPositions();

			// calculate the initial energy and forces
			minimizer_->updateForces();
			minimizer_->updateEnergy();
			initial_gradient = gradient;
			
			// force a recalculation of the current gradient
			// as well since updateForces overwrote everything!
			gradient.invalidate();
		}
		initial_energy_ = minimizer_->getInitialEnergy();
		initial_dir_grad_ = (initial_gradient * direction) * direction.inv_norm;

		// remember the best lambda and the best energy
		double best_lambda = 0.0;
		double best_energy = initial_energy_;

		// if we do not have a valid current gradient for the first step,
		// calculate it
		if (!gradient.isValid())
		{
			// recalculate the gradient and energy for lambda = 1.0
			atoms.moveTo(direction, step);
			minimizer_->updateEnergy();
			minimizer_->updateForces();
		}
		current_energy_ = minimizer_->getEnergy();
		current_dir_grad_ = (gradient * direction) * direction.inv_norm;
		
		lambda_ = step;
		step_ = step; // this is required for criterion()

		Size iteration = 0;
		bool result = false;
		while (!result && iteration < max_steps_)
		{
			// remember the current lambda
			double last_lambda = lambda_;

			// interpolate
			lambda_ = step * interpolate
												(0.0, lambda_ / step, 
												 initial_energy_, current_energy_,	
												 initial_dir_grad_ * direction.norm * step, 
												 current_dir_grad_ * direction.norm * step);

			// check whether the criterions are fulfilled
			atoms.moveTo(direction, lambda_);

			// update energy and gradient
			current_energy_ = minimizer.updateEnergy();
			minimizer.updateForces();
			current_dir_grad_ = (gradient * direction) * direction.inv_norm;

			// remember the best lambda found
			// (just in case)
			if (current_energy_ < best_energy)
			{
				best_lambda = lambda_;
				best_energy = current_energy_;
			}
			
			// increment thenumber of iteration
			iteration++;

			if ((lambda_ / step_) < 1e-8)
			{
				lambda_ = 0.0;
				result = false;
				break;
			}

			if ((lambda_ == 0.0) || fabs((last_lambda - lambda_) / lambda_) < 1e-3)
			{
				// Log.info() << " === ";
				result = criterion();
				break;
			}

			// check for convergence
			result = criterion();
		}		

		if (result)
		{
			// return the current lambda if we found a true solution
			lambda = lambda_ / step;
		} 
		else
		{
			// otherwise return the best one we found
			lambda = best_lambda / step;
		}

		return result;
	}

	double LineSearch::interpolate
		(double lambda_0, double lambda_1, 
		 double energy_0, double energy_1, 
		 double grad_0, double grad_1) const
	{
		//Log.info() << "LS:interpolate(" 
		//						<< lambda_0 << ", " << lambda_1 << ", "
		//						<< energy_0 << ", " << energy_1 << ", "
		//						<< grad_0 << ", " << grad_1 << ")";
    // local variables
    double tmp1, a, b, tmp2;
    double lambda_diff, lambda_diff_2;
    double lambda_diff_3;
    double result;

    // Do a cubic interpolation based on (0,energy_0), (1,energy__1), (0,grad_0), (1,grad_1)
    // energy_0 : energy at left point
    // energy_1 : energy at right point
    // grad_0 : direction gradient at left point
    // grad_1 : direction gradient at right point
    lambda_diff = lambda_1 - lambda_0;

    if (lambda_diff == 0)
    {
			//Log.info() << " = " << lambda_0 << endl;
      // the intervall is of length 0
      return lambda_0;
		}

    lambda_diff_2 = lambda_diff * lambda_diff;
    lambda_diff_3 = lambda_diff_2 * lambda_diff;


    tmp1 = energy_1 - energy_0;

    a = 3.0 * (-2.0 * tmp1 + (grad_0 + grad_1) * lambda_diff) / lambda_diff_3;
    b = (3.0 * tmp1 - (2.0 * grad_0 + grad_1) * lambda_diff) / lambda_diff_2;

    tmp2 = b * b - a * grad_0;

    if ((a != 0.0) && (tmp2 > 0.0))
    {
      result = lambda_0 + (-b + sqrt(tmp2)) / a;
		}
    else
    {
      // just a quadratic interpolation
      // Note: b = 0 is then not possible by construction as this would be a linear
      // interpolation
      result = lambda_0 - grad_0 / (2 * b);
			//Log.info() << "[quadr.] ";
		}

		//Log.info() << " = " << result << endl;
    return result;
	}
	
	bool LineSearch::criterion() const
	{	
		// Armijo & Goldstein criterion:
		//   
		// twofold criterion:
		// - sufficient decrease of energy
		//    E(i+1) <= E(i) + alpha * lambda * <g(i), dir> 
		//
		// - sufficient gradient reduction
		//    |<g(i+1), dir>| <= beta <g(i), dir>
		//
		// where g(i) and g(i+1) are the initial and the current gradient
		// dir is the (normalized) search direction
		// E(i+1) is the current and E(i) the initial energy (lambda = 0)
		// alpha and beta are two parameters (usually 0.9 and 1e-4)
		// 
		//Log.info() << " C1: " << current_energy_ - initial_energy_ << "/" << alpha_ * lambda_ * current_dir_grad_ 
		//					 << "  C2: " << fabs(current_dir_grad_) << "/" << beta_ * fabs(initial_dir_grad_) << " --- ";
		return ((current_energy_ < (initial_energy_ + alpha_ * lambda_ * current_dir_grad_))
						&& (fabs(current_dir_grad_) < beta_ * fabs(initial_dir_grad_)));
	}

} // namespace Ball
