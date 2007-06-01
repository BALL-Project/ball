// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conicLineSearch.C,v 1.1.4.4 2007/06/01 12:41:37 bertsch Exp $
//

#include <BALL/MOLMEC/MINIMIZATION/conicLineSearch.h>
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/COMMON/limits.h>

// Parameter alpha for 'sufficient energy decrease'.
#define BALL_CONICLINESEARCH_DEFAULT_ALPHA 1.e-4

// Parameter beta for 'sufficient gradient reduction'.
#define BALL_CONICLINESEARCH_DEFAULT_BETA 0.9

// Lower bound for energy values.
#define BALL_CONICLINESEARCH_DEFAULT_MIN_ENERGY -1.e+10

// Maximum number of interpolation steps for a line search.
#define BALL_CONICLINESEARCH_DEFAULT_MAX_STEPS 50


//#define BALL_DEBUG
#undef BALL_DEBUG

using namespace std;

namespace BALL 
{
	// Default constructor
	ConicLineSearch::ConicLineSearch()
		:	alpha_(BALL_CONICLINESEARCH_DEFAULT_ALPHA),
			beta_(BALL_CONICLINESEARCH_DEFAULT_BETA),
			max_steps_(BALL_CONICLINESEARCH_DEFAULT_MAX_STEPS),
			lower_energy_bound_(BALL_CONICLINESEARCH_DEFAULT_MIN_ENERGY),
			is_bracketed_(false),
			ctrl_stp_(0.),
			ctrl_val_(1.e+100),
			minimizer_(0)
	{
	}
	
	
	// Copy constructor 
	ConicLineSearch::ConicLineSearch(const ConicLineSearch& line_search)
		:	alpha_(line_search.alpha_),
			beta_(line_search.beta_),
			max_steps_(line_search.max_steps_),
			lower_energy_bound_(line_search.lower_energy_bound_),
			is_bracketed_(line_search.is_bracketed_),
			ctrl_stp_(line_search.ctrl_stp_),
			ctrl_val_(line_search.ctrl_val_),
			minimizer_(line_search.minimizer_)
	{
	}
	
	// Assignment operator
	const ConicLineSearch& ConicLineSearch::operator = (const ConicLineSearch& line_search)
	{
		alpha_              = line_search.alpha_;
		beta_               = line_search.beta_;
		max_steps_          = line_search.max_steps_;
		lower_energy_bound_ = line_search.lower_energy_bound_;
		is_bracketed_       = line_search.is_bracketed_;
		ctrl_stp_           = line_search.ctrl_stp_;
		ctrl_val_           = line_search.ctrl_val_;
		minimizer_          = line_search.minimizer_;
		
		return *this;
	}
	
	// Detailed constructor
	ConicLineSearch::ConicLineSearch(EnergyMinimizer& minimizer)
		:	alpha_(BALL_CONICLINESEARCH_DEFAULT_ALPHA),
			beta_(BALL_CONICLINESEARCH_DEFAULT_BETA),
			max_steps_(BALL_CONICLINESEARCH_DEFAULT_MAX_STEPS),
			lower_energy_bound_(BALL_CONICLINESEARCH_DEFAULT_MIN_ENERGY),
			is_bracketed_(false),
			ctrl_stp_(0.),
			ctrl_val_(1.e+100),
			minimizer_(&minimizer)
	{
	}
	
	// Destructor
	ConicLineSearch::~ConicLineSearch()
		throw()
	{
	}
	
	// Set the parameter alpha_.
	void ConicLineSearch::setAlpha(double alpha)
	{
		alpha_ = alpha;
	}
	
	// Get the parameter alpha_.
	double ConicLineSearch::getAlpha() const
	{
		return alpha_;
	}
	
	// Set the parameter beta_.
	void ConicLineSearch::setBeta(double beta)
	{
		beta_ = beta;
	}
	
	// Get the parameter beta_.
	double ConicLineSearch::getBeta() const
	{
		return beta_;
	}
	
	// Set the parameter max_steps_.
	void ConicLineSearch::setMaxSteps(Size max_steps)
	{
		max_steps_	= max_steps;
	}
	
	// Get the parameter max_steps_.
	Size ConicLineSearch::getMaxSteps() const
	{
		return max_steps_;
	}
	
	// Set the lower bound on energy values.
	void ConicLineSearch::setLowerBound(double lbound)
	{
		lower_energy_bound_ = lbound;
	}
	
	// Get the lower bound on energy values.
	double ConicLineSearch::getLowerBound() const
	{
		return lower_energy_bound_;
	}
	
	// Set the flag is_bracketed_.
	void ConicLineSearch::setBracketedFlag(bool bracktd)
	{
		is_bracketed_ = bracktd;
	}
	
	// Return whether a minimizer has already been bracketed.
	bool ConicLineSearch::isBracketed() const
	{
		return is_bracketed_;
	}
	
	// Set the calling minimizer class.
	void ConicLineSearch::setMinimizer(EnergyMinimizer& minimizer)
	{
		minimizer_ = &minimizer;
	}
	
	/*	The minimizer optimizes the energy of the system using a two stage line 
			search algorithm similar to that proposd by More and Thuente.
	*/
	bool ConicLineSearch::minimize(double& stp)
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
			// Reset the atoms to the start position (stp = 0)
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
		
		// Initial directional derivative.
		double g_init = (initial_gradient * direction);
		
		// Minimum and maximum stepsizes.
		
		// Compute the maximum step size by the minimizers 'maximum displacement'
		maxstp_ = minimizer.getMaximumDisplacement();
		double minstp = 1.e-10;
		if (maxstp_ < 0.)
		{
			// No maximum displacement given, estimate the maximum stepsize
			double tmp = gradient.norm/direction.norm;
			maxstp_ = 1.e+5*tmp;
			
			// Useful safeguard for minimum stepsize
			minstp = 1.e-10*tmp;
		}
		else
		{
			// Find the maximum translation
			Gradient::ConstIterator git(direction.begin());
			double max = 0.;
			double cur = 0.;
			for (; git != direction.end(); ++git)
			{
				cur = git->getSquareLength();
				if (cur > max)
				{
					max = cur;
				}
			}
			max = sqrt(max);
			if (max > 1.e-16)
			{
				maxstp_ /= max;
			}
			else
			{
				// Something went wrong, we estimate the maximum stepsize
				double tmp = gradient.norm/direction.norm;
				maxstp_ = 1.e+5*tmp;
				
				// Useful safeguard for minimum stepsize
				minstp = 1.e-10*tmp;
			}
		}
		
		// A minimum has not been bracketed so far.
		is_bracketed_ = false;
		
		// Compute initial stepsize
		stp = min(1., (lower_energy_bound_ - f_init)*4./g_init);
		stp = max(stp, minstp);
		stp = min(stp, maxstp_);
		
		// Objective value at stp
		double f;
		
		// Directional derivative at stp
		double g = 0.;
		
		// Interval data
		double stp_up = 0.;
		double f_up = f_init;
		double g_up = g_init;
		double stp_lo = 0.;
		double f_lo = f_init;
		double g_lo = g_init;
		
		// Main loop
		for(Size iteration = 0; iteration < max_steps_; ++iteration)
		{
			// Get energy and directional derivative at 'stp'
			
			// Move the atoms to the new position.
			atoms.moveTo(direction, stp);
			
			// Update energy and gradient
			f = minimizer.updateEnergy();
			minimizer.updateForces();
			g = (gradient * direction);
			
			// We check a few things
			bool con1  = f - f_init <= alpha_*stp*g_init;
			bool con2  = g >= beta_*g_init;
			
			// Check whether we cannot proceed because the minimum
			// tolerable stepsize is achieved.
			if ((stp <= minstp) && !con1)
			{
				return false;
			}
			
			// Check whether we have the maximum stepsize achieved
			if ((stp >= maxstp_) && con1 && !con2)
			{
				return false;
			}
			
			// Convergence test (weak Wolfe conditions)
			if (con1 && con2)
			{
				return true;
			}
			
			if (!is_bracketed_)
			{
				// Bracketing stage
				ctrl_stp_ = stp_lo;
				ctrl_val_ = f_lo;
				
				stp_lo = stp_up;
				f_lo   = f_up;
				g_lo   = g_up;
				stp_up = stp;
				f_up   = f;
				g_up   = g;
				
				if (!con1)
				{
					is_bracketed_ = true;
				}
			}
			else
			{
				// Interpolation stage
				if (!con1)
				{
					ctrl_stp_ = stp_up;
					ctrl_val_ = f_up;
					
					stp_up = stp;
					f_up   = f;
					g_up   = g;
				}
				else
				{
					ctrl_stp_ = stp_lo;
					ctrl_val_ = f_lo;
					
					stp_lo = stp;
					f_lo   = f;
					g_lo   = g;
				}
			}
			
			if (stp >= maxstp_)
			{
				// New stepsize
				stp = takeStep(stp_lo, f_lo, g_lo, stp_up, f_up, g_up);
				if (stp >= maxstp_)
				{
					return false;
				}
			}
			else
			{
				// New stepsize
				stp = takeStep(stp_lo, f_lo, g_lo, stp_up, f_up, g_up);
			}
		}
		
		// If we're here something went wrong
		return false;
	}
	
	// Computes a step for a search procedure by case differentiation 
	// dependend on whether a minimum could already be bracketed or not.
	double ConicLineSearch::takeStep(double stp_lo, double f_lo, double g_lo, 
																	 double stp_up, double f_up, double g_up) const
	{
		double stp;
	
		// Compute needed values
		double q = (f_up - f_lo) / (g_lo*(stp_up - stp_lo));
		double p = g_up / g_lo;
		
		double conic_stp = -1.;
		double cubic_stp = -1.;
		double quad_stp_val = -1.;
		
		// Compute the conic step
		double tmp = q*q - p;
		if (tmp >= 0.)
		{
			tmp = q + sqrt(tmp);
			if (tmp > 0.)
			{
				tmp = 1. - p/tmp/tmp/tmp;
				if (!compAndSafeguardStep_(stp_lo, stp_up, tmp, conic_stp))
				{
					conic_stp = -1.;
				}
			}
		}
		
		// Compute the cubic step
		tmp = p - q*3. + 2.;
		double tmp2 = tmp*tmp - (p - q*2. + 1.)*3.;
		if (tmp2 >= 0.)
		{
			tmp += sqrt(tmp2);
			if (!compAndSafeguardStep_(stp_lo, stp_up, tmp, cubic_stp))
			{
				cubic_stp = -1.;
			}
		}
		
		// Compute the quadratic step based on one directional derivative
		tmp = (1. - q)*2.;
		if (!compAndSafeguardStep_(stp_lo, stp_up, tmp, quad_stp_val))
		{
			quad_stp_val = -1.;
		}
		
		// Check in which stage we are
		if (!is_bracketed_)
		{
			// In the (extrapolation) bracketing stage
			// we choose the step which is closest to stp_up
			stp = conic_stp;
			if (fabs(cubic_stp - stp_up) < fabs(stp - stp_up))
			{
				stp = cubic_stp;
			}
			if (fabs(quad_stp_val - stp_up) < fabs(stp - stp_up))
			{
				stp = quad_stp_val;
			}
		}
		else
		{
			// We are in the interpolation stage
			
			double val_diff = 1.e+100;
			stp = conic_stp;
			
			// Check whether the conic step is possible
			if (conic_stp != -1.)
			{
				// Check the value of the conic interpolating function at the
				// control step
				double tmp = (q + sqrt(q*q-p))/p - 1.;
				double s   = stp_up-stp_lo;
				
				double d   = 2.*tmp/s;
				double x   = ctrl_stp_ - stp_lo;
				double den = d*x + 1.;
				den       *= den;
				
				if (den < 1.e-100)
				{
					// We cannot check the value since the control
					// step is too close to a singularity
					conic_stp = -1.;
					stp       = cubic_stp;
				}
				else
				{
					double a   = -(p - ((4.*tmp+2.)*q + 1.)*q)/p/s/g_lo;
					double b   = g_lo;
					double nom = a*x + b;
					nom       *= x;
					
					double val = nom/den + f_lo;
					
					val_diff   = fabs(val - ctrl_val_);
					stp        = conic_stp;
				}
			}
			
			// Check whether the cubic step is possible
			if (cubic_stp != -1.)
			{
				// Check the value of the cubic interpolating function
				// at the control step
				double s = stp_up - stp_lo;
				
				double a = -g_lo*(-1. + 2.*q - p)/s/s;
				double b = (-2. + 3.*q - p)/p;
				double c = g_lo;
				
				double x   = ctrl_stp_ - stp_lo;
				double val = a*x + b;
				val        = val*x + c;
				val        = val*x + f_lo;
				
				double tmp = fabs(val - ctrl_val_);
				if (tmp < val_diff)
				{
					val_diff = tmp;
					stp = cubic_stp;
				}
			}
			
			// Check whether the quadratic step is possible
			if (quad_stp_val != -1.)
			{
				double s   = stp_up - stp_lo;
			
				double a   = -g_lo*(1.-q)/s;
				double b   = g_lo;
				double x   = ctrl_stp_ - stp_lo;
				double val = a*x + b;
				val        = val*x + f_lo;
				
				double tmp = fabs(val - ctrl_val_);
				if (tmp < val_diff)
				{
					val_diff = tmp;
					stp = quad_stp_val;
				}
			}
		}
		
		// Check whether all interpolations have failed
		if (stp == -1.)
		{
			
			//We try a quadratic step with two directional derivatives
			double tmp = 1. - p;
			if (compAndSafeguardStep_(stp_lo, stp_up, tmp, stp))
			{
				return stp;
			}
			
			// All other cases failed, we do a bisection
			if (!is_bracketed_)
			{
				stp = stp_up*2.;
			}
			else
			{
				stp = (stp_lo + stp_up)*0.5;
			}
			stp = min(stp, maxstp_);
		}
		return stp;
	}
	
	
	// Compute the safeguarded step and decide whether it can be accepted
	bool ConicLineSearch::compAndSafeguardStep_(double stp_lo, double stp_up, double den, double& stp) const
	{
		if (!is_bracketed_)
		{
			if ((den > 0.) && (den < 1.))
			{
				// We are in the extrapolation stage and accept 
				// this step
				stp = stp_lo + (stp_up - stp_lo)/den;
				stp = max(stp, stp_up*1.1);
				stp = min(stp, stp_up*2.);
				
				stp = min(stp, maxstp_);
				return true;
			}
		}
		else
		{
			if (den > 1.)
			{
				// We are in the interpolation stage and accept
				// this step
				stp = stp_lo + (stp_up - stp_lo)/den;
				stp = min(stp, stp_lo + (stp_up - stp_lo)*0.9999);
				return true;
			}
		}
		return false;
	}
	
	

} // namespace BALL
