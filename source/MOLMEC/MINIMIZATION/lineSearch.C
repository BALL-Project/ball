// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineSearch.C,v 1.11 2003/03/17 10:28:12 anhi Exp $

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

//#define BALL_DEBUG

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
	LineSearch::LineSearch(const LineSearch& line_search)
		:	alpha_(line_search.alpha_),
			beta_(line_search.beta_),
			max_steps_(line_search.max_steps_),
			minimizer_(line_search.minimizer_)
	{
	}

	// assignment operator
	const LineSearch& LineSearch::operator = (const LineSearch& line_search)
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
		throw()
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
	bool LineSearch::minimize(double& lambda, double step, bool keep_gradient)
	{
		#ifdef BALL_DEBUG
			Log.info() << "LS:minimize(" << lambda << ", " << step << ")" << std::endl;
		#endif

		// Check whether a valid minimizer and a valid force field exist.
		if ((minimizer_ == 0) || (minimizer_->getForceField() == 0))
		{
			return false;
		}

		// Define some aliases for convenience
		AtomVector&				atoms(const_cast<AtomVector&>(minimizer_->getForceField()->getAtoms()));
		const Gradient&		direction(minimizer_->getDirection());
		EnergyMinimizer&	minimizer(*minimizer_);
		Gradient&					gradient(minimizer.getGradient());
		Gradient&					initial_gradient(minimizer.getInitialGradient());

		// this is required for isSufficient()
		step_ = step; 

		// If we do not have a valid gradient for the first step,
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
		initial_dir_grad_ = (initial_gradient * direction) * step_;

		// remember the best lambda and the best energy
		double best_lambda = 0.0;
		double best_energy = initial_energy_;

		// if we do not have a valid current gradient for the first step, or if we are
		// told to force an update (i.e. keep_gradient == false), calculate it
		if (!keep_gradient || !gradient.isValid())
		{
			gradient.invalidate();

			#ifdef BALL_DEBUG
				Log << "  LineSearch: recalculate Energy/grad @ l = 1.0" << std::endl;
			#endif
			// recalculate the gradient and energy for lambda = 1.0
			atoms.moveTo(direction, step);
			minimizer_->updateEnergy();
			minimizer_->updateForces();
		}
		double current_energy = minimizer_->getEnergy();
		double current_dir_grad = (gradient * direction) * step_;

		// Start with lambda = 1.0
		Size iteration = 0;
		bool result = false;
		lambda = 1.0;

		while (!result && iteration < max_steps_)
		{
			// remember the current lambda
			double last_lambda = lambda;

			// interpolate
			lambda = interpolate
										(0.0, lambda, 
										 initial_energy_, current_energy,	
										 initial_dir_grad_, current_dir_grad);

			// Move the atoms to the new position.
			atoms.moveTo(direction, lambda * step_);

			// Update energy and gradient
			current_energy = minimizer.updateEnergy();
			minimizer.updateForces();
			current_dir_grad = (gradient * direction) * step_;

			// remember the best lambda found
			// (just in case)
			if (current_energy < best_energy)
			{
				best_lambda = lambda;
				best_energy = current_energy;
			}
			
			// Increment the number of iterations.
			iteration++;

			// Make sure we don't run into numerical inaccuracies due to 
			// *too* small a lambda.
			if (lambda < 1e-8)
			{
				lambda = 0.0;
				result = false;
				break;
			}

			// If lambda is changing to slowly, there's some problem.
			if ((lambda == 0.0) || fabs((last_lambda - lambda) / lambda) < 1e-3)
			{
				#ifdef BALL_DEBUG
					Log.info() << " === ABORT ===";
				#endif
				result = isSufficient(lambda, current_energy, current_dir_grad);
				break;
			}

			// check for convergence
			result = isSufficient(lambda, current_energy, current_dir_grad);
		}		

		// if the line search failed, reset the atom positions and return the
		// best lambda we have to offer
		if (!result)
		{
			lambda = best_lambda;

			// in this case, we also want to move the atoms to the position of the
			// best lambda. since we don't know where the atoms are right now, we
			// do a reset first
			atoms.resetPositions();
			atoms.moveTo(direction, lambda*step_);

			gradient.invalidate();	
		}

		return result;
	}

	double LineSearch::interpolate
		(double lambda_0, double lambda_1, 
		 double energy_0, double energy_1, 
		 double grad_0, double grad_1) const
	{
		#ifdef BALL_DEBUG
		Log.info() << "LS:interpolate(" 
								<< lambda_0 << ", " << lambda_1 << ", "
								<< energy_0 << ", " << energy_1 << ", "
								<< grad_0 << ", " << grad_1 << ")";
		#endif

    // Do a cubic interpolation based on (0, energy_0), (1, energy__1), (0, grad_0), (1, grad_1)
    // energy_0 : energy at left point
    // energy_1 : energy at right point
    // grad_0 : direction gradient at left point
    // grad_1 : direction gradient at right point
    double lambda_diff = lambda_1 - lambda_0;
    double delta_energy = energy_1 - energy_0;


		// If the lambdas are identical, there's nothing to be done.
    if (lambda_diff == 0)
    {
			#ifdef BALL_DEBUG
				Log.info() << "LS: lambda_0 == lambda_1 = " << lambda_0 << " !" << std::endl;
			#endif

      return lambda_0;
		}

		// compute the lambda squared/cubed
    double lambda_diff_2 = lambda_diff * lambda_diff;
    double lambda_diff_3 = lambda_diff_2 * lambda_diff;


    double a = 3.0 * (-2.0 * delta_energy + (grad_0 + grad_1) * lambda_diff) / lambda_diff_3;
    double b = (3.0 * delta_energy - (2.0 * grad_0 + grad_1) * lambda_diff) / lambda_diff_2;

    double tmp2 = b * b - a * grad_0;

		double result = 0.0;
    if ((a != 0.0) && (tmp2 > 0.0))
    {
      result = lambda_0 + (-b + sqrt(tmp2)) / a;
		}
    else if (b != 0.0)
    {
      // just a quadratic interpolation
      // Note: b = 0 is then not possible by construction as this would be a linear
      // interpolation
      result = lambda_0 - grad_0 / (2 * b);
			#ifdef BALL_DEBUG
				Log.info() << "[quadr.] ";
			#endif
		}
		else
		{
			#ifdef BALL_DEBUG
				Log.info() << "[linear.] ";
			#endif			

      // This bloody thing is linear!
      result = ((energy_0 < energy_1) ? lambda_0 : lambda_1);
		}

		#ifdef BALL_DEBUG
			Log.info() << " = " << result << std::endl;
		#endif

    return result;
	}
	
	bool LineSearch::isSufficient(double lambda, double current_energy, double current_dir_grad) const
	{	
		// Armijo & Goldstein criterion:
		//   
		// twofold criterion:
		// - sufficient decrease of energy
		//    E(i+1) <= E(i) + alpha * lambda * step * <g(i), dir> 
		//
		// - sufficient gradient reduction
		//    |<g(i+1), dir>| <= beta <g(i), dir>
		//
		// where g(i) and g(i+1) are the initial and the current gradient
		// dir is the search direction
		// E(i+1) is the current and E(i) the initial energy (lambda = 0)
		// alpha and beta are two parameters (usually 0.9 and 1e-4)
		// 
		#ifdef BALL_DEBUG
			Log.info() << " C1: " << current_energy - initial_energy_ << "/" << alpha_ * lambda * step_ * current_dir_grad
								<< "  C2: " << fabs(current_dir_grad) << "/" << beta_ * fabs(initial_dir_grad_) << " --- ";
		#endif
		return ((current_energy <= (initial_energy_ + alpha_ * lambda * step_ * current_dir_grad))
						&& (fabs(current_dir_grad) <= beta_ * fabs(initial_dir_grad_)));
	}

} // namespace BALL
