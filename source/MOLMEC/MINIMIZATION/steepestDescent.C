// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: steepestDescent.C,v 1.27.26.5 2007/05/18 10:58:39 aleru Exp $
//

#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/MOLMEC/COMMON/snapShotManager.h>

#include <limits>

//#define BALL_DEBUG
#undef BALL_DEBUG

using namespace std;

namespace BALL
{
	
	// Default constructor
	 SteepestDescentMinimizer::SteepestDescentMinimizer()
		:	EnergyMinimizer(),
			line_search_()
	{
		line_search_.setMinimizer(*this);
	}
	
	// Copy constructor 
	SteepestDescentMinimizer::SteepestDescentMinimizer 
		(const SteepestDescentMinimizer & minimizer)
		: EnergyMinimizer(minimizer),
			line_search_(minimizer.line_search_)
	{
		line_search_.setMinimizer(*this);
	}
	
	// Constructor initialized with a force field
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field)
		:	EnergyMinimizer(),
			line_search_()
	{
		line_search_.setMinimizer(*this);

		valid_ = setup(force_field);
		
		if (!valid_)
		{
			Log.error() << "SteepestDescentMinimizer: setup failed! " << std::endl;
		}
	}
	
	// Constructor initialized with a force field and a snapshot  manager 
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field, SnapShotManager* ssm)
		:	EnergyMinimizer(),
			line_search_()
	{
		line_search_.setMinimizer(*this);

		valid_ = setup (force_field, ssm);
		
		if (!valid_)
		{
			Log.error() << "SteepestDescentMinimizer: setup failed! " << std::endl;
		}
	}
	
	// Constructor initialized with a force field and a set of options
	SteepestDescentMinimizer::SteepestDescentMinimizer
		(ForceField& force_field, const Options& new_options)
		:	EnergyMinimizer(),
			line_search_()
	{
		line_search_.setMinimizer(*this);

		options = new_options;
		valid_  = setup (force_field, new_options);
		
		if (!valid_)
		{
			Log.error() << "SteepestDescentMinimizer: setup failed! " << endl;
		}
	}
	
	// Constructor initialized with a force field, a snapshot manager, and a set of options
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field,
																										 SnapShotManager* ssm,
																										 const Options& new_options)
		:	EnergyMinimizer(),
			line_search_()
	{
		line_search_.setMinimizer(*this);

		options = new_options;
		valid_  = setup(force_field, ssm, new_options);
		
		if (!valid_)
		{
			Log.error() << "SteepestDescentMinimizer: setup failed! " << std::endl;
		}
	}
	
	// Destructor
	SteepestDescentMinimizer::~SteepestDescentMinimizer()
	{
	}
	
	// Assignment operator
	const SteepestDescentMinimizer& SteepestDescentMinimizer::operator=
			(const SteepestDescentMinimizer& minimizer)
	{
		if (&minimizer != this)
		{
			EnergyMinimizer::operator=(minimizer);
			line_search_      = minimizer.line_search_;
			line_search_.setMinimizer(*this);
		}
		return *this;
	}
	
	// virtual function for the specific setup of derived classes
	bool SteepestDescentMinimizer::specificSetup()
	{
		// Make sure the force field is assigned and valid!
		if (force_field_ == 0 || !force_field_->isValid())
		{
			return false;
		}
		
		// Invalidate the initial gradient in order to ensure
		// its re-evaluation at the start of minimize().
		initial_grad_.invalidate();
		
		return true;
	}
	
	/*  The minimizer optimizes the energy of the system
	*/
	bool SteepestDescentMinimizer::minimize(Size iterations, bool resume)
	{
		aborted_ = false;
		
		// Check for validity of minimizer and force field
		if (!isValid() || getForceField() == 0 || !getForceField()->isValid())
		{
			Log.error() << "SteepestDescentMinimizer: minimizer is not initialized correctly!" << std::endl;
			aborted_ = true;
			return false;
		}
		
		// Make sure we have something worth moving.
		if (getForceField()->getNumberOfMovableAtoms() == 0)
		{
			return true;
		}
		
		// Some aliases
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		// If the run is to be continued, don't reset the iteration counter and the initial step size
		if (!resume)
		{
			// reset the number of iterations for a restart
			setNumberOfIterations(0);
			same_energy_counter_ = 0;
			initial_grad_.invalidate();
			current_grad_.invalidate();
			
			// Obviously, we don't have "old" energies yet, so we initialize it a with 
			// sensible value. We don't need "old" gradients.
			old_energy_ = std::numeric_limits<float>::max();
		}
		Size max_iterations = std::min(getNumberOfIterations() + iterations, getMaxNumberOfIterations());
		
		// Save the current atom positions
		atoms.savePositions();
		bool converged = false;	
		while (!converged && (getNumberOfIterations() < max_iterations))
		{
			// Try to take a new step
			double stp = findStep();
			
			// Check whether we were successful.
			if (stp > 0.0)
			{
				// Use this step as new reference step if findStep was successful
				atoms.savePositions();
			}
			
			// Store the energy, there's no need to store the old gradient
			old_energy_ = initial_energy_;
			
			// Store the current gradient and energy
			storeGradientEnergy();
			
			#ifdef BALL_DEBUG
				Log.info() << "SDM::minimize: end of main: current grad RMS = " << current_grad_.rms << std::endl;
			#endif
			
			// Check for convergence.
			converged = isConverged() || (stp == 0.);
			
			// Increment iteration counter, take snapshots, print energy,
			// update pair lists, and check the same-energy counter
			finishIteration();
			
			if ((!converged) && (stp < 0.))
			{
				// Nasty case: No convergence and the step computation failed.
				// We must give up:-(
				aborted_ = true;
				return false;
			}
			if (Maths::isNan(force_field_->getEnergy()))
			{
				aborted_ = true;
				return false;
			}
			
			if (Maths::isNan(getGradient().rbegin()->x) ||
					Maths::isNan(getGradient().rbegin()->y) ||
					Maths::isNan(getGradient().rbegin()->z)) 
			{
				aborted_ = true;
				return false;
			}
			
			if (abort_by_energy_enabled_)
			{
				if (force_field_->getEnergy() > abort_energy_) 
				{
					aborted_ = true;
					return false;
				}
			}
		}
		
		return converged;
	}
	
	double SteepestDescentMinimizer::findStep()
	{
		// Compute the new direction
		updateDirection();
		
		double step = 0.;
		bool result = line_search_.minimize(step);
		
		if (!result)
		{
			// Something went wrong.
			
			// Some aliases
			AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
			
			// Just in case: force field update (to update the pair list)
			atoms.resetPositions();
			force_field_->update();
			
			// Compute the initial energy and the initial forces
			initial_energy_ = force_field_->updateEnergy();
			force_field_->updateForces();
			initial_grad_.set(force_field_->getAtoms());
			
			direction_ = initial_grad_;
			direction_.negate();
			direction_.normalize();
			
			Size iter = 0;
			while ((!result) && (iter < 12))
			{
				// No need to assure the maximum displacement here since our
				// line search pays attention to this constraint.
				
				result = line_search_.minimize(step);
			
				if (!result)
				{
					Size n = getForceField()->getNumberOfMovableAtoms();
					for(Size i = 0; i < n; ++i)
					{
						direction_[i] *= 0.5;
					}
					direction_.norm     *= 0.5;
					direction_.rms      *= 0.5;
					direction_.inv_norm *= 2.;
					atoms.resetPositions();
				}
				else
				{
					return step;
				}
				++iter;
			}
			// If we are here something went wrong
			// Not even such scaled steepest descent steps can manage
			// the line search to exit successfully?
			// We must be at a local minimizer...
			step_ = 0.;
		}
		return step;
	}
	
	void SteepestDescentMinimizer::updateDirection()
	{
		// If we do not have a valid gradient, recalculate the gradient, the energy,
		// and the search direction
		if (!initial_grad_.isValid())
		{
			// Compute the initial energy and the initial forces
			updateEnergy();
			updateForces();
			
			// Store them for later use
			storeGradientEnergy();
		}
		
		// The direction is the normalized negative gradient
		direction_ = initial_grad_;
		direction_.negate();
		direction_.normalize();
	}
	
} // namespace BALL
