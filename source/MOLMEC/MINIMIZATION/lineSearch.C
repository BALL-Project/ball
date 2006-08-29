// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineSearch.C,v 1.20.2.2 2006/08/29 12:50:36 aleru Exp $
//

#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/COMMON/limits.h>

// Parameter alpha for 'sufficient energy decrease'.
#define LINESEARCH__DEFAULT_ALPHA 1.e-3

// Parameter beta for 'sufficient gradient reduction'.
#define LINESEARCH__DEFAULT_BETA 0.9

// Maximum number of interpolation steps for a line search.
#define LINESEARCH__DEFAULT_MAX_STEPS 50

// Lower bound for energy values.
#define LINESEARCH__DEFAULT_MIN_ENERGY -1.e+30

// Nonnegative relative tolerance for an acceptable step.
#define LINESEARCH__DEFAULT_XTOL 0.1

//#define BALL_DEBUG
#undef BALL_DEBUG

namespace BALL 
{
	// Default constructor
	LineSearch::LineSearch()
		:	alpha_(LINESEARCH__DEFAULT_ALPHA),
			beta_(LINESEARCH__DEFAULT_BETA),
			max_steps_(LINESEARCH__DEFAULT_MAX_STEPS),
			lower_energy_bound_(LINESEARCH__DEFAULT_MIN_ENERGY),
			stptol_(LINESEARCH__DEFAULT_XTOL),
			is_bracketed_(false),
			minimizer_(0)
	{
	}


	// Copy constructor 
	LineSearch::LineSearch(const LineSearch& line_search)
		:	alpha_(line_search.alpha_),
			beta_(line_search.beta_),
			max_steps_(line_search.max_steps_),
			lower_energy_bound_(line_search.lower_energy_bound_),
			stptol_(line_search.stptol_),
			is_bracketed_(line_search.is_bracketed_),
			minimizer_(line_search.minimizer_)
	{
	}

	// Assignment operator
	const LineSearch& LineSearch::operator = (const LineSearch& line_search)
	{
		alpha_ = line_search.alpha_;
		beta_ = line_search.beta_;
		max_steps_ = line_search.max_steps_;
		lower_energy_bound_ = line_search.lower_energy_bound_;
		stptol_ = line_search.stptol_;
		is_bracketed_ = line_search.is_bracketed_;
		minimizer_ = line_search.minimizer_;

		return *this;
	}

	// Detailed constructor
	LineSearch::LineSearch(const EnergyMinimizer& minimizer)
		:	alpha_(LINESEARCH__DEFAULT_ALPHA),
			beta_(LINESEARCH__DEFAULT_BETA),
			max_steps_(LINESEARCH__DEFAULT_MAX_STEPS),
			lower_energy_bound_(LINESEARCH__DEFAULT_MIN_ENERGY),
			stptol_(LINESEARCH__DEFAULT_XTOL),
			is_bracketed_(false),
			minimizer_(const_cast<EnergyMinimizer*>(&minimizer))
	{
	}

	// Destructor
	LineSearch::~LineSearch()
		throw()
	{
	}
		
	// Set the parameter alpha_.
	void LineSearch::setAlpha(double alpha)
	{
		alpha_ = alpha;
	}

	// Get the parameter alpha_.
	double LineSearch::getAlpha() const
	{
		return alpha_;
	}

	// Set the parameter beta_.
	void LineSearch::setBeta(double beta)
	{
		beta_ = beta;
	}

	// Get the parameter beta_.
	double LineSearch::getBeta() const
	{
		return beta_;
	}

	// Set the parameter max_steps_.
	void LineSearch::setMaxSteps(Size max_steps)
	{
		max_steps_	= max_steps;
	}

	// Get the parameter max_steps_.
	Size LineSearch::getMaxSteps() const
	{
		return max_steps_;
	}
	
	// Set the lower bound on energy values.
	void LineSearch::setLowerBound(double lbound)
	{
		lower_energy_bound_ = lbound;
	}
	
	// Get the lower bound on energy values.
	double LineSearch::getLowerBound() const
	{
		return lower_energy_bound_;
	}
	
	// Set the nonnegative relative tolerance for an acceptable step.
	void LineSearch::setXTol(double xtol)
	{
		stptol_ = xtol;
	}

	// Get the nonnegative relative tolerance for an acceptable step.
	double LineSearch::getXTol() const
	{
		return stptol_;
	}

	// Set the calling minimizer class.
	void LineSearch::setMinimizer(const EnergyMinimizer& minimizer)
	{
		minimizer_ = const_cast<EnergyMinimizer*>(&minimizer);
	}

	/*	The minimizer optimizes the energy of the system using a two stage line 
			search algorithm based on a method of More and Thuente.
	*/
	bool LineSearch::minimize(double& stp, double scale, bool keep_gradient)
	{
		#ifdef BALL_DEBUG
			Log.info() << "LS:minimize(" << stp << ")" << std::endl;
		#endif

		// Check whether a valid minimizer and a valid force field exist.
		if ((minimizer_ == 0) || (minimizer_->getForceField() == 0))
		{
			return false;
		}

		// Define some aliases for convenience.
		AtomVector&				atoms(const_cast<AtomVector&>(minimizer_->getForceField()->getAtoms()));
		const Gradient&		direction(minimizer_->getDirection());
		EnergyMinimizer&	minimizer(*minimizer_);
		Gradient&					gradient(minimizer.getGradient());
		Gradient&					initial_gradient(minimizer.getInitialGradient());

		// If we do not have a valid gradient for the first step,
		// calculate it.
		if (!initial_gradient.isValid())
		{
			// Reset the atoms to the start position (lambda = 0)
			atoms.resetPositions();

			// Calculate the initial energy and forces
			minimizer_->updateForces();
			minimizer_->updateEnergy();
			initial_gradient = gradient;
			
			// Force a recalculation of the current gradient
			// as well since updateForces overwrote everything!
			gradient.invalidate();
		}
		
		// Initial energy value.
		double f_init = minimizer_->getInitialEnergy();
		
		// Initial (scaled) directional derivative.
		double g_init = (initial_gradient * direction) * scale;
		
		// Minimum and maximum stepsizes.
		double minstp = 0.;
		double maxstp = (lower_energy_bound_-f_init)/(beta_*g_init);
		
		// A minimum has not been bracketed so far.
		is_bracketed_ = false;
		
		// Set initial step size to 1 (with safeguard)
		stp = (1. < maxstp) ? 1. : maxstp;
		
		// Used for directional derivative stopping criterion
		double g_test = alpha_*g_init;
		
		// Used for computations of the interval width 
		// (interval which brackets the minimizer).
		double int_width = maxstp - minstp;
		double bisec_int_width = int_width / 0.5;
		
		// st_left and st_right will bracket the minimizer
		double st_left = 0.;
		double st_right = 0.;
		
		// f_left and f_right will contain the energy values at st_left and st_right.
		double f_left = f_init;
		double f_right = f_init;
		
		// g_left and g_right will contain the directional derivatives at st_left and st_right.
		double g_left = g_init;
		double g_right = g_init;
		
		// Used for internal bracketing
		double stmin = 0.;
		double stmax = stp + stp*4.;

		// Remember the best step and the best energy
		double best_stp = 0.0;
		double best_f = f_init;

		// If we do not have a valid current gradient for the first step, or if we are
		// told to force an update (i.e. keep_gradient == false), or if 
		// our internal safeguards force stp not to equal 1, calculate it
		if (!keep_gradient || !gradient.isValid() || stp != 1.)
		{
			gradient.invalidate();

			#ifdef BALL_DEBUG
				Log << "  LineSearch: recalculate Energy/grad @ l = " << stp << " " << std::endl;
			#endif
			// Recalculate the gradient and energy
			atoms.moveTo(direction, stp*scale);
			minimizer_->updateEnergy();
			minimizer_->updateForces();
		}
		
		// Energy value at stp.
		double f = minimizer_->getEnergy();
		
		// Directional derivative at stp.
		double g = (gradient * direction)*scale;

		Size iteration = 0;
		bool result = false;
		
		// We start with the first stage
		bool first_stage = true;
		
		// Used for energy decrease criterion
		double f_test = f_init + stp*g_test;
		while (!result && iteration < max_steps_)
		{
			// First check a few numerical things
			if (((stp < stmin) || (stp > stmax)))
			{
				// Rounding errors prevent progress
				result = false;
				break;
			}
			if (is_bracketed_ && stmax - stmin <= stptol_*stmax)
			{
				// Xtol condition satisfied
				result = true;
				break;
			}
			
			// Test for convergence.
			if ((f <= f_test) && (fabs(g) <= beta_*(-g_init)))
			{
				result = true;
				break;
			}
			
			if ((stp == maxstp) && (f <= f_test) && (g <= g_test))
			{
				// Maximum stepsize achieved. This doesn't have to be a problem.
				// We return a 'false' and let the calling algorithm decide
				// what to do. A 'false' means that neither the convergence 
				// conditions hold (weak Wolfe conditions) nor the condition 
				// on the step tolerance is satisfied.
				result = false;
				break;
			}
			if ((stp == minstp) && ((f > f_test) || (g >= g_test)))
			{
				// Minimum stepsize achieved. This doesn't have to be a problem.
				// We return a 'false' and let the calling algorithm decide
				// what to do. A 'false' means that neither the convergence 
				// conditions hold (weak Wolfe conditions) nor the condition 
				// on the step tolerance is satisfied.
				result = false;
				break;
			}
			
			
			// In first stage we use a modified function, proposed by More and Thuente,
			// in case of a lower function value but the decrease is not sufficient.
			if (first_stage && (f <= f_left) && (f > f_test))
			{
				// We have to compute the modified energy... 
				double f_mod = f - stp*g_test;
				double f_left_mod = f_left - st_left*g_test;
				double f_right_mod = f_right - st_right*g_test;
				
				// ... and derivative values.
				double g_mod = g - g_test;
				double g_left_mod = g_left - g_test;
				double g_right_mod = g_right - g_test;

				// Compute a safeguarded, interpolating step and use the modified function.
				lsStep(st_left, f_left_mod, g_left_mod, st_right, f_right_mod, g_right_mod, stp, f_mod, g_mod, stmin, stmax);

				// Compute back all values for the original energy function.
				f_left = f_left_mod + st_left * g_test;
				f_right = f_right_mod + st_right * g_test;
				g_left = g_left_mod + g_test;
				g_right = g_right_mod + g_test;
			}
			else
			{
				// Compute a safeguarded, interpolating step.
				lsStep(st_left, f_left, g_left, st_right, f_right, g_right, stp, f, g, stmin, stmax);
			}

			if (is_bracketed_)
			{
				// The minimizer has already been bracketed.
				if (fabs(st_right - st_left) >= bisec_int_width*0.66)
				{
					// We make a bisection step.
					stp = st_left + (st_right - st_left)/2.;
				}
				bisec_int_width = int_width;
				int_width = fabs(st_right - st_left);
				
				// Set the minimum and maximum steps allowed for stp.
				stmin = std::min(st_left, st_right);
				stmax = std::max(st_left, st_right);
			}
			else
			{
				// The minimizer couldn't be bracketed so far, thus we make an extrapolation step.
				// Set the minimum and maximum steps allowed for stp.
				stmin = stp + (stp - st_left) * 1.1;
				stmax = stp + (stp - st_left) * 4.;
			}

			// Use the safeguards...
			stp = (stp < minstp) ? minstp : stp;
			stp = (stp < maxstp) ? stp : maxstp;

			if ((is_bracketed_) && ((stp <= stmin) || (stp >= stmax)) || (is_bracketed_) && (stmax - stmin <= stptol_ * stmax))
			{
				// If we are not able to make further progress, let stp be the best point we could find.
				stp = st_left;
			}

			// Obtain new energy and derivative.
			
			// Move the atoms to the new position.
			atoms.moveTo(direction, stp*scale);

			// Update energy and gradient
			f = minimizer.updateEnergy();
			minimizer.updateForces();
			g = (gradient * direction)*scale;
			
			// Check whether we can enter the second stage.
			f_test = f_init + stp*g_test;
			if ((f <= f_test) && (g >= 0.))
			{
				first_stage = false;
			}

			// Remember the best stepsize found
			// (just in case)
			if (f < best_f)
			{
				best_stp = stp;
				best_f = f;
			}
			
			// Increment the number of iterations.
			iteration++;
		}

		// If the line search failed, reset the atom positions and return the
		// best stepsize we have to offer
		if (!result)
		{
			stp = best_stp;

			// In this case, we also want to move the atoms to the position of the
			// best step, if the energy at that point has improved.
			if (best_f < f_init)
			{
				atoms.moveTo(direction, stp*scale);
			}
			else
			{
				atoms.resetPositions();
			}
			gradient.invalidate();	
		}

		return result;
	}

	// Computes a safeguarded step for a search procedure by case differentiation 
	// dependend on whether a minimum could already be bracketed or not.
	// This function is based on the proposed step computation of Jorge J. More and David J. Thuente.
	// A Fortran implementation can be found in MINPACK and MINPACK-2.
	void LineSearch::lsStep(double &st_left, double &f_left, double &g_left, double &st_right, 
					double &f_right, double &g_right, double &stp, double f, double g, double minstp, double maxstp)
	{
		// The new step, which will be returned by stp on exit.
		double new_stp;
		
		// Compute whether we have directional derivatives of opposite sign.
		bool opp_sign =  (g*(g_left/fabs(g_left)) < 0.) ? true : false;
		
		// Check the four possible cases.
		if (f > f_left)
		{
			// First case: We have a higher function value, so the minimum is bracketed.
		
			double theta = (f_left - f)*3./(stp - st_left) + g_left + g;
			double s = std::max(fabs(theta), fabs(g_left));
			s = (stp < st_left) ? -std::max(s, fabs(g)) : std::max(s, fabs(g));
			double gamma = s*sqrt((theta/s)*(theta/s)-(g_left/s)*(g/s));
			
			// We check both, the quadratic and the cubic step.
			// Compute the cubic step.
			double cub_stp = st_left + (((gamma - g_left) + theta)/(((gamma - g_left) + gamma) + g))*(stp - st_left);
			
			// Compute the quadratic step.
			double quad_stp = st_left + ((g_left /((f_left - f)/(stp - st_left) + g_left))/2.)*(stp - st_left);
			
			// We prefer the cubic step if it is closer to st_left than the quadratic step, 
			// otherwise we use the average of the quadratic and the cubic one.
			new_stp = (fabs(cub_stp - st_left) < fabs(quad_stp - st_left)) ? cub_stp : cub_stp + (quad_stp - cub_stp)/2.;
			
			// Minimum has been bracketed.
			is_bracketed_ = true;
		}
		else if (opp_sign)
		{
			// Second case: We have a lower function value but directional derivatives
			// of opposite sign, so the minimum is bracketed.
			
			double theta = (f_left - f)*3./(stp - st_left) + g_left + g;
			double s = std::max(fabs(theta), fabs(g_left));
			s = (stp > st_left) ? -std::max(s, fabs(g)) : std::max(s, fabs(g));
			double gamma = s*sqrt((theta/s)*(theta/s) - (g_left/s)*(g/s));
			
			// We check both, the quadratic and the cubic step.
			// Compute the cubic step.
			double cub_stp = stp + (((gamma - g) + theta)/(((gamma - g) + gamma) + g_left))*(st_left - stp);
			
			// Compute the quadratic step.
			double quad_stp = stp + (g/(g - g_left))*(st_left - stp);
			
			// We prefer the cubic step if it is farther from stp than the quadratic one, 
			// otherwise the quadratic step is used.
			new_stp = (fabs(cub_stp - stp) > fabs(quad_stp - stp)) ? cub_stp : quad_stp;
			
			// Minimum has been bracketed
			is_bracketed_ = true;
		}
		else if (fabs(g) < fabs(g_left))
		{
			// Third case: We have a lower function value and derivatives of the same sign
			// but the magnitude of the derivative decreases. There are three subcases:
			
			// (1) The cubic tends to infinity
			// (2) The cubic tends to -infinity but its minimum is beyond stp
			// (3) The cubic tends to -infinity and its minimum is on this side of stp
		
			double theta = (f_left - f)*3./(stp - st_left) + g_left + g;
			double s = std::max(fabs(theta), fabs(g_left));
			s = (stp > st_left) ? -std::max(s, fabs(g)) : std::max(s, fabs(g));
			
			// We can have gamma = 0 only if the cubic doesn't tend to infinity.
			double gamma = s*sqrt(std::max(0., (theta/s)*(theta/s) - (g_left/s)*(g/s)));
			double r = ((gamma - g) + theta)/((gamma + (g_left - g)) + gamma);
			
			// We compute the cubic step only in case (1) and (2) (only these cases make sense).
			double cub_stp;
			if ((r < 0.) && (gamma != 0.))
			{
				cub_stp = stp + r*(st_left - stp);
			}
			else
			{
				// Use the safeguards...
				cub_stp = (stp > st_left) ? maxstp : minstp;
			}
			
			// Compute the quadratic step.
			double quad_stp = stp + (g/(g - g_left))*(st_left - stp);
			
			if (is_bracketed_)
			{
				// A minimizer has been bracketed. We prefer the cubic step if it
				// is closer to stp than the quadratic step. Otherwise we use the 
				// quadratic one.
				new_stp = (fabs(cub_stp - stp) < fabs(quad_stp - stp)) ? cub_stp : quad_stp;
				
				// Use the safeguards (remember: it is assumed in the bracketed case, that
				// stp must lie in the interval between st_left and st_right).
				if (stp > st_left)
				{
					new_stp = std::min(new_stp, stp + (st_right - stp)*0.66);
				}
				else
				{
					new_stp = std::max(new_stp, stp + (st_right - stp)*0.66);
				}
			}
			else
			{
				// If a minimizer has not been bracketed, we prefer the cubic step 
				// if it is farther from stp than the quadratic step.
				// Otherwise we use the quadratic step.
				new_stp = (fabs(cub_stp - stp) > fabs(quad_stp - stp)) ? cub_stp : quad_stp;
				
				// Use the safeguards...
				if (maxstp < new_stp)
				{
					new_stp = maxstp;
				}
				if (minstp > new_stp)
				{
					new_stp = minstp;
				}
			}
		}
		else
		{
			// Fourth case: We have a lower function value and derivatives of 
			// the same sign, but the magnitude of the derivative doesn't 
			// decrease.
		
			if (is_bracketed_)
			{
				// If a minimum has already been bracketed, we use the cubic step
				double theta = (f - f_right)*3./(st_right - stp) + g_right + g;
				double s = std::max(fabs(theta), fabs(g_right));
				s = (stp > st_right) ? -std::max(s, fabs(g)) : std::max(s, fabs(g));
				double gamma = s*sqrt((theta/s)*(theta/s) - (g_right/s)*(g/s));
				
				// Compute the cubic step.
				new_stp = stp + (((gamma - g) + theta)/(((gamma - g) + gamma) + g_right))*(st_right - stp);
			}
			else 
			{
				// If we couldn't bracket the minimum so far, the 
				// step must be either minstp or maxstp.
				new_stp = (stp > st_left) ? maxstp : minstp;
			}
		}

		// Do all updates for the interval that contains a minimizer.	
		if (f > f_left)
		{
			st_right = stp;
			f_right = f;
			g_right = g;
		}
		else
		{
			if (opp_sign)
			{
				st_right = st_left;
				f_right = f_left;
				g_right = g_left;
			}
			st_left = stp;
			f_left = f;
			g_left = g;
		}
	
		// Set the new step.
		stp = new_stp;
	}
	

} // namespace BALL
