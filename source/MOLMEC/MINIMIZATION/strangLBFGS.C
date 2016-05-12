// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: strangLBFGS.C,v 1.1.4.7 2007/08/17 09:22:32 anhi Exp $
//
// Minimize the potential energy of a system using an improved version
// of the limited memory BFGS with Strang recurrences.

#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

#include <limits>

//#define BALL_DEBUG
#undef BALL_DEBUG

// The default maximum number of stored vector pairs.
#define DEFAULT_NUMBER_OF_VECTOR_PAIRS 5

// The default "improved" flag
#define DEFAULT_IMPROVED true

namespace BALL 
{
	using namespace std; 
	
	const char* StrangLBFGSMinimizer::Option::IMPROVED = "improved";
	const char* StrangLBFGSMinimizer::Option::NUM_OF_STORED_VECT_PAIRS = "num_of_stored_vect_pairs";
	
	const bool StrangLBFGSMinimizer::Default::IMPROVED = DEFAULT_IMPROVED;
	const Size StrangLBFGSMinimizer::Default::NUM_OF_STORED_VECT_PAIRS = DEFAULT_NUMBER_OF_VECTOR_PAIRS;
	
	
	// Default constructor 
	StrangLBFGSMinimizer::StrangLBFGSMinimizer()
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			improved_(DEFAULT_IMPROVED),
			rho_(),
			stored_s_(),
			stored_y_(),
			work_val_(),
			index_of_free_vect_(0),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options.setDefaultBool(Option::IMPROVED, Default::IMPROVED);
		options.setDefaultInteger(Option::NUM_OF_STORED_VECT_PAIRS, Default::NUM_OF_STORED_VECT_PAIRS);
	}
	
	// Constructor initialized with a force field
	StrangLBFGSMinimizer::StrangLBFGSMinimizer(ForceField& force_field)
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			improved_(DEFAULT_IMPROVED),
			rho_(),
			stored_s_(),
			stored_y_(),
			work_val_(),
			index_of_free_vect_(0),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options.setDefaultBool(Option::IMPROVED, Default::IMPROVED);
		options.setDefaultInteger(Option::NUM_OF_STORED_VECT_PAIRS, Default::NUM_OF_STORED_VECT_PAIRS);
		
		valid_ = setup(force_field);
		
		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl;
		}
	}
	
	// Constructor initialized with a force field and a snapshot manager 
	StrangLBFGSMinimizer::StrangLBFGSMinimizer
		(ForceField& force_field, SnapShotManager* ssm)
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			improved_(DEFAULT_IMPROVED),
			rho_(),
			stored_s_(),
			stored_y_(),
			work_val_(),
			index_of_free_vect_(0),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options.setDefaultBool(Option::IMPROVED, Default::IMPROVED);
		options.setDefaultInteger(Option::NUM_OF_STORED_VECT_PAIRS, Default::NUM_OF_STORED_VECT_PAIRS);
		
		valid_ = setup(force_field, ssm);
		
		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl;
		}
	}
	
	
	// Constructor initialized with a force field and a set of options
	StrangLBFGSMinimizer::StrangLBFGSMinimizer
		(ForceField& force_field, const Options& new_options) 
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			improved_(DEFAULT_IMPROVED),
			rho_(),
			stored_s_(),
			stored_y_(),
			work_val_(),
			index_of_free_vect_(0),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options = new_options;
		
		// The actual work is done in setup 
		valid_ = setup(force_field, new_options);

		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl; 
		}
	}

	// Constructor initialized with a force field, a snapshot manager, and a set of options
	StrangLBFGSMinimizer::StrangLBFGSMinimizer
		(ForceField& force_field, SnapShotManager* ssm, const Options& new_options)
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			improved_(DEFAULT_IMPROVED),
			rho_(),
			stored_s_(),
			stored_y_(),
			work_val_(),
			index_of_free_vect_(0),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options = new_options;
		
		// The actual work is done in setup 
		valid_ = setup(force_field, ssm, new_options);
		
		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl; 
		}
	}
	
	
	// The destructor
	StrangLBFGSMinimizer::~StrangLBFGSMinimizer()
	{
	}
	
	
	// The copy constructor 
	StrangLBFGSMinimizer::StrangLBFGSMinimizer 
		(const StrangLBFGSMinimizer& rhs)
		: EnergyMinimizer(rhs),
			line_search_(rhs.line_search_),
			first_iter_(rhs.first_iter_),
			number_of_atoms_(rhs.number_of_atoms_),
			max_num_of_vect_pairs_(rhs.max_num_of_vect_pairs_),
			curr_num_of_vect_pairs_(rhs.curr_num_of_vect_pairs_),
			improved_(rhs.improved_),
			rho_(rhs.rho_),
			stored_s_(rhs.stored_s_),
			stored_y_(rhs.stored_y_),
			work_val_(rhs.work_val_),
			index_of_free_vect_(rhs.index_of_free_vect_),
			initial_atoms_(rhs.initial_atoms_)
	{
		line_search_.setMinimizer(*this);
	}
	
	// The assignment operator
	const StrangLBFGSMinimizer& StrangLBFGSMinimizer::operator =
		(const StrangLBFGSMinimizer& rhs)
	{
		EnergyMinimizer::operator = (rhs);
		line_search_            = rhs.line_search_;
		first_iter_             = rhs.first_iter_;
		number_of_atoms_        = rhs.number_of_atoms_;
		max_num_of_vect_pairs_  = rhs.max_num_of_vect_pairs_;
		curr_num_of_vect_pairs_ = rhs.curr_num_of_vect_pairs_;
		improved_               = rhs.improved_;
		rho_                    = rhs.rho_;
		stored_s_               = rhs.stored_s_;
		stored_y_               = rhs.stored_y_;
		work_val_               = rhs.work_val_;
		index_of_free_vect_     = rhs.index_of_free_vect_;
		initial_atoms_          = rhs.initial_atoms_;
		line_search_.setMinimizer(*this);
		return *this;
	}
	
	// This method is responsible for doing the specific setup of this class
	bool StrangLBFGSMinimizer::specificSetup()
	{
		// Make sure the force field is assigned and valid!
		if (force_field_ == 0 || !force_field_->isValid())
		{
			return false;
		}
		
		// Invalidate the initial gradient in order to ensure
		// its re-evaluation at the start of minimize().
		initial_grad_.invalidate();
		
		// Get the options
		max_num_of_vect_pairs_ = (Size)options.getInteger(Option::NUM_OF_STORED_VECT_PAIRS);
		improved_              = options.getBool(Option::IMPROVED);
		
		// Determine the number of atoms
		number_of_atoms_ = force_field_->getNumberOfMovableAtoms();
		
		// Start with the (initial) first iteration
		first_iter_ = true;
		
		return true;
	}
	
	// Set the maximum number of stored vector pairs
	void StrangLBFGSMinimizer::setMaxNumOfStoredVectPairs(Size num)
	{
		max_num_of_vect_pairs_ = num;
		options.setInteger(Option::NUM_OF_STORED_VECT_PAIRS, num);
	}
	
	// Return the maximum number of stored vector pairs
	Size StrangLBFGSMinimizer::getMaxNumOfStoredVectPairs() const
	{
		return max_num_of_vect_pairs_;
	}
	
	// Set the improved flag
	void StrangLBFGSMinimizer::setImprovedFlag(bool flag)
	{
		improved_ = flag;
		options.setBool(Option::IMPROVED, flag);
	}

	// Return the improved flag
	bool StrangLBFGSMinimizer::getImprovedFlag() const
	{
		return improved_;
	}
	
	void StrangLBFGSMinimizer::updateDirection()
	{
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		if (!initial_grad_.isValid())
		{
			// Compute the initial energy and the initial forces
			updateEnergy();
			updateForces();
			
			// Store them for later use
			storeGradientEnergy();
		}
		
		if (first_iter_)
		{
			// This is the (initial) first iteration
			
			// Save old positions
			for(Size i = 0; i < number_of_atoms_; ++i)
			{
				initial_atoms_[i] = atoms[i]->getPosition();
			}
			
			// The first search direction is the normalized negative gradient
			direction_ = initial_grad_;
			direction_.negate();
			direction_.normalize();
			first_iter_ = false;
			return;
		}
		
		// Just in case: check whether 
		//  (1) (enough) memory is allocated for our search direction
		//  (2) the old gradient are invalid.
		if ((direction_.size() == 0) || !old_grad_.isValid())
		{
			// This mustn't happen at all. If this routine is called all data(-structures) should be fine.
			// But we are here! Something went wrong and we don't know what. Most kindly the data is 
			// modified by an external program/thread, so we do the best we can do: first we update the forces
			// and set the direction to the negative gradient. Additionally, we force a restart.
			Log.error() << "dir: " << direction_.isValid() 
									<< "  initial_grad: " << initial_grad_.isValid() 
									<< "  current_grad: " << current_grad_.isValid() << endl;
			Log.error() << "StrangLBFGSMinimizer::updateDirection: invalid gradient or direction " << endl;
			Log.error() << "  cannot update stored vectors" << endl;
			Log.error() << "  cannot compute L-BFGS direction." << endl;

			// Calculate the current gradient and determine the current
			// direction as the direction of steepest descent.
			updateForces();
			direction_ = current_grad_;
			direction_.negate();
			direction_.normalize();
			
			// Discard all data we have collected so far since we cannot trust in it any longer.
			curr_num_of_vect_pairs_ = 0;
			index_of_free_vect_ = 0;
			first_iter_ = false;
			return;
		}
		
		// First we update our stored vectors, so we're going to store an additional vector
		// if we have space left, otherwise we replace the oldest vector pair by the new pair.
		if (curr_num_of_vect_pairs_ < max_num_of_vect_pairs_)
		{
			++curr_num_of_vect_pairs_;
		}
		
		double sty = 0.;
		double yty = 0.;
		Size i;
		Index k = number_of_atoms_*index_of_free_vect_;
		
		// Main update loop. Do everything in one loop.
		for(i = 0; i < number_of_atoms_; ++i, ++k)
		{
			// Compute the recent step in a way that reduces 
			// slightly rounding errors
			stored_s_[k] = atoms[i]->getPosition() - initial_atoms_[i];
			stored_y_[k] = initial_grad_[i] - old_grad_[i];
			
			sty += stored_y_[k]*stored_s_[k];
			yty += stored_y_[k]*stored_y_[k];
		}
		
		direction_ = initial_grad_;
		direction_.negate();
		
		// Compute the new scaling factor
		double scale;
		
		if (sty == 0.)
		{
			// We check for "exactly 0". If this happens something went wrong.
			// We try to do our very best, which means we force the direction computation
			// to be "possible".
			rho_[index_of_free_vect_] = 1.;
			scale = 1.;
		}
		else
		{
			rho_[index_of_free_vect_] = sty;
			
			if ((sty > 0.) && (yty > cutlo_))
			{
				scale = sty/yty;
			}
			else
			{
				// This case should not happen due to our line search.
				// Well, once more: we do our best...
				scale = 1.;
			}
		}
		
		// Now we are able to compute the search direction.
		
		Size r;
		Size j;
		k = index_of_free_vect_+1;
		
		if (improved_)
		{
			// First recurrence. Attention: we use an improved version. We use more up-to-date 
			// information for computing our initial hessian approximation. We use the techniques in
			
			// Mehiddin Al-Baali, "Improved Hessian approximations for the limited memory BFGS method",
			// Numerical Algorithms 22 (1999), 99-112
			
			// with the newest correction pair. Therefore we have to set 
			// "i <= curr_num_of_vect_pairs_" instead of "i < curr_num_of_vect_pairs_".
			for(i = 0; i <= curr_num_of_vect_pairs_; ++i)
			{
				--k;
				if (k == -1)
				{
					k = curr_num_of_vect_pairs_-1;
				}
				double tmp = 0.;
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					tmp += stored_s_[j]*direction_[r];
				}
				
				tmp /= rho_[k];

				work_val_[k] = tmp;
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					direction_[r] = -stored_y_[j]*tmp + direction_[r];
				}
			}
		}
		else
		{
			// First recurrence. Standard version
			for(i = 0; i < curr_num_of_vect_pairs_; ++i)
			{
				--k;
				if (k == -1)
				{
					k = curr_num_of_vect_pairs_-1;
				}
				double tmp = 0.;
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					tmp += stored_s_[j]*direction_[r];
				}
				
				tmp /= rho_[k];
				work_val_[k] = tmp;
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					direction_[r] = -stored_y_[j]*tmp + direction_[r];
				}
			}
		}
		
		// Scaling
		for(r = 0; r < number_of_atoms_; ++r)
		{
			direction_[r] *= scale;
		}
		
		if (improved_)
		{
			// Second recurrence. Attention: same case as above. We have to set
			// "i <= curr_num_of_vect_pairs_" instead of "i < curr_num_of_vect_pairs_".
			for(i = 0; i <= curr_num_of_vect_pairs_; ++i)
			{
				double tmp = 0.;
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					tmp += stored_y_[j]*direction_[r];
				}
				tmp = work_val_[k] - tmp/rho_[k];
				
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					direction_[r] = stored_s_[j]*tmp + direction_[r];
				}
				++k;
				if (k == (Index) curr_num_of_vect_pairs_)
				{
					k = 0;
				}
			}
		}
		else
		{
			// Second recurrence, standard version
			for(i = 0; i < curr_num_of_vect_pairs_; ++i)
			{
				double tmp = 0.;
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					tmp += stored_y_[j]*direction_[r];
				}
				tmp = work_val_[k] - tmp/rho_[k];
				
				for(r = 0, j = k*number_of_atoms_; r < number_of_atoms_; ++r, ++j)
				{
					direction_[r] = stored_s_[j]*tmp + direction_[r];
				}
				++k;
				if (k == (Index) curr_num_of_vect_pairs_)
				{
					k = 0;
				}
			}
		}
		
		// Compute norm and current directional derivative.
		double norm = 0.0;
		double dir_d = 0.0;
		for (i = 0; i < number_of_atoms_; i++)
		{
			norm += direction_[i] * direction_[i];
			dir_d += direction_[i] * initial_grad_[i];
		}
		norm = sqrt(norm);
		
		if (dir_d > 0.)
		{
			// If the current search direction is NOT a descent direction
			// something went wrong. We set the search direction to the 
			// normalized negative gradient and force a 'restart'.
			direction_ = initial_grad_;
			direction_.negate();
			direction_.normalize();
			curr_num_of_vect_pairs_ = 0;
			index_of_free_vect_ = 0;
		}
		else
		{
			// Don't risc a "NaN"
			if (norm >= cutlo_)
			{
				direction_.inv_norm = 1.0 / norm;
			}
			else
			{
				direction_.inv_norm = sqrt(std::numeric_limits<float>::max());
			}
			
			// Assign the norm and rms of the new direction
			direction_.norm = norm;
			direction_.rms = norm / (3.0 * (double)number_of_atoms_);
			
			// Set the index for the next update
			index_of_free_vect_ = (index_of_free_vect_+1) % max_num_of_vect_pairs_;
		}
		
		// Save old positions
		for(Size i = 0; i < number_of_atoms_; ++i)
		{
			initial_atoms_[i] = atoms[i]->getPosition();
		}
		
	} // end of method 'updateDirection'
	
	
	// The minimizer optimizes the energy of the system 
	// by using the limited memory BFGS method with Strang recurrence with improvement 
	// ideas of Al-Baali.
	// Return value is true when no further steps can be taken!
	bool StrangLBFGSMinimizer::minimize(Size iterations, bool resume)
	{
		aborted_ = false;

		// Check for validity of minimizer and force field
		if (!isValid() || getForceField() == 0 || !getForceField()->isValid())
		{
			Log.error() << "StrangLBFGSMinimizer::minimize: Minimizer is not initialized correctly!" << std::endl;
			aborted_ = true;
			return false;
		}
		
		// Check our number of the movable atoms...
		
		Size noatoms = force_field_->getNumberOfMovableAtoms();
		// (1) Make sure we have something worth moving.
		if (noatoms == 0)
		{
			return true;
		}
		// (2) Check that outside of this minimizer the number of movable atoms didn't change
		if (number_of_atoms_ != noatoms)
		{
			// The number of movable atoms has changed, so we must start from scratch
			number_of_atoms_ = noatoms;
			resume = false;
		}
		// (3) Check that outside of this minimizer the options didn't change
		if ((Size)options.getInteger(Option::NUM_OF_STORED_VECT_PAIRS) != max_num_of_vect_pairs_)
		{
			max_num_of_vect_pairs_ = (Size)options.getInteger(Option::NUM_OF_STORED_VECT_PAIRS);
			resume = false;
		}
		improved_ = options.getBool(Option::IMPROVED);
		
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		// If we start from scratch or the number of movable atoms has changed 
		// outside of this minimizer (i.e. no restart) we have to make sure
		// to calculate all the quantities we need before we start and to allocate 
		// enough memory.
		if (!resume)
		{
			// Reset the number of iterations if the job is not resumed.
			setNumberOfIterations(0);
			same_energy_counter_ = 0;
			index_of_free_vect_ = 0;
			curr_num_of_vect_pairs_ = 0;
			current_grad_.invalidate();
			initial_grad_.invalidate();
			first_iter_ = true;
			
			// Allocate our memory needed for the algorithm
			rho_.resize(max_num_of_vect_pairs_);
			stored_s_.resize(max_num_of_vect_pairs_*number_of_atoms_);
			stored_y_.resize(max_num_of_vect_pairs_*number_of_atoms_);
			work_val_.resize(max_num_of_vect_pairs_);
			initial_atoms_.resize(number_of_atoms_);
			
			// Obviously, we don't have "old" energies yet, so we initialize it a with 
			// sensible value. We don't need "old" gradients here.
			old_energy_ = std::numeric_limits<float>::max();
		}
		
		Size max_iterations = std::min(getNumberOfIterations() + iterations, getMaxNumberOfIterations());
		
		#ifdef BALL_DEBUG
			Log.info() << "StrangLBFGSMinimizer: minimize(" << iterations << ", " << resume << ")" << endl;
		#endif
		
		// Save the current atom positions
		atoms.savePositions();
		bool converged = false;	
		
		// Iterate: while not converged and not enough iterations
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
			
			// Store the energy and gradient
			old_energy_ = initial_energy_;
			old_grad_   = initial_grad_;
			
			// Store the current gradient and energy
			storeGradientEnergy();
			
			#ifdef BALL_DEBUG
				Log.info() << "StrangLBFGSMinimizer::minimize: end of main: current grad RMS = " << current_grad_.rms << std::endl;
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
	} // end of method 'minimize'
	
	double StrangLBFGSMinimizer::findStep()
	{
		// Compute the new direction
		updateDirection();
		
		#ifdef BALL_DEBUG
			Log.info() << "StrangLBFGSMinimizer::findStep: " << initial_energy_ << " " << current_grad_.norm << " " << direction_.norm << endl;
		#endif
		
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		// We perform a line search along direction_
		
		// No need to assure the maximum displacement here since our
		// line search pays attention to this constraint.
		
		bool result = line_search_.minimize(step_);
		
		if (!result)
		{
			// Something went wrong.
			// No success. We proceed with a restart.
			
			// Set the search direction to the normalized negative gradient. Since we proceed
			// with a restart, we mustn't update the stored vectors by 'updateDirection' and there is
			// no need to compute anything by 'updateDirection'.
			
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
			
			// We cannot trust in our data any more, so we force all routines to 
			// assume that we haven't collected any data so far.
			curr_num_of_vect_pairs_ = 0;
			index_of_free_vect_     = 0;
			Size iter               = 0;
			while ((!result) && (iter < 12))
			{
				result = line_search_.minimize(step_);
				if (!result)
				{
					for(Size i = 0; i <  number_of_atoms_; ++i)
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
					return step_;
				}
				++iter;
			}
			// If we are here something went wrong.
			// Not even such scaled steepest descent steps can manage
			// the line search to exit successfully?
			// We must be at a local minimizer...
			step_ = 0.;
		}
			
		#ifdef BALL_DEBUG
			Log.info() << "LineSearch: step = " << step_ << " result = " << result << endl;
		#endif
			
		return step_;
	} // end of method 'findStep'

} // end of namespace BALL
