// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: strangLBFGS.C,v 1.1.2.7 2006/10/19 11:16:23 aleru Exp $
//
// Minimize the potential energy of a system using an improved version
// of the limited memory BFGS with Strang recurrences.

#include <BALL/MOLMEC/MINIMIZATION/strangLBFGS.h>
#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/COMMON/limits.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

//#define BALL_DEBUG
#undef BALL_DEBUG

// The default maximum number of stored vector pairs.
#define DEFAULT_NUMBER_OF_VECTOR_PAIRS 5

namespace BALL 
{
  using namespace std; 


  // Default constructor 
  // It does nothing but calling its base class constructor 
  StrangLBFGSMinimizer::StrangLBFGSMinimizer()
		: EnergyMinimizer(),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			rho_(),
			stored_s_(),
			stored_y_(),
			initial_atoms_(),
			work_val_(),
			index_of_free_vect_(0)
	{
		// Compute cutlo_
		float epsilon;
		float eps = 1.;
		while (1. + eps > 1.)
		{
			epsilon = eps;
			eps /= 2.;
		}
		cutlo_ = sqrt(Limits<float>::min()/epsilon);
	}

	// Constructor initialized with a force field
	StrangLBFGSMinimizer::StrangLBFGSMinimizer(ForceField& force_field)
		: EnergyMinimizer(),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			rho_(),
			stored_s_(),
			stored_y_(),
			initial_atoms_(),
			work_val_(),
			index_of_free_vect_(0)
	{
		valid_ = setup(force_field);

		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl;
		}
		
		// Compute cutlo_
		float epsilon;
		float eps = 1.;
		while (1. + eps > 1.)
		{
			epsilon = eps;
			eps /= 2.;
		}
		cutlo_ = sqrt(Limits<float>::min()/epsilon);
	}

	// Constructor initialized with a force field and a snapshot manager 
	StrangLBFGSMinimizer::StrangLBFGSMinimizer
		(ForceField& force_field, SnapShotManager* ssm)
		: EnergyMinimizer(),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			rho_(),
			stored_s_(),
			stored_y_(),
			initial_atoms_(),
			work_val_(),
			index_of_free_vect_(0)
	{
		valid_ = setup(force_field, ssm);

		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl;
		}
		
		// Compute cutlo_
		float epsilon;
		float eps = 1.;
		while (1. + eps > 1.)
		{
			epsilon = eps;
			eps /= 2.;
		}
		cutlo_ = sqrt(Limits<float>::min()/epsilon);
	}

	
	// Constructor initialized with a force field and a set of options
	StrangLBFGSMinimizer::StrangLBFGSMinimizer
		(ForceField& force_field, const Options& new_options) 
		: EnergyMinimizer(),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			rho_(),
			stored_s_(),
			stored_y_(),
			initial_atoms_(),
			work_val_(),
			index_of_free_vect_(0)
	{
		// The actual work is done in setup 
		valid_ = setup(force_field, new_options);

		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl; 
		}
		
		// Compute cutlo_
		float epsilon;
		float eps = 1.;
		while (1. + eps > 1.)
		{
			epsilon = eps;
			eps /= 2.;
		}
		cutlo_ = sqrt(Limits<float>::min()/epsilon);
	}

	// Constructor initialized with a force field, a snapshot manager, and a set of options
	StrangLBFGSMinimizer::StrangLBFGSMinimizer
		(ForceField& force_field, SnapShotManager* ssm, const Options& new_options)
		: EnergyMinimizer(),
			number_of_atoms_(0),
			max_num_of_vect_pairs_(DEFAULT_NUMBER_OF_VECTOR_PAIRS),
			curr_num_of_vect_pairs_(0),
			rho_(),
			stored_s_(),
			stored_y_(),
			initial_atoms_(),
			work_val_(),
			index_of_free_vect_(0)
	{
		// The actual work is done in setup 
		valid_ = setup(force_field, ssm, new_options);

		if (!valid_)
		{
			Log.error() << "StrangLBFGSMinimizer: setup failed! " << endl; 
		}
		
		// Compute cutlo_
		float epsilon;
		float eps = 1.;
		while (1. + eps > 1.)
		{
			epsilon = eps;
			eps /= 2.;
		}
		cutlo_ = sqrt(Limits<float>::min()/epsilon);
	}


  // The destructor
  StrangLBFGSMinimizer::~StrangLBFGSMinimizer()
  {
  }


  // The copy constructor 
  StrangLBFGSMinimizer::StrangLBFGSMinimizer 
		(const StrangLBFGSMinimizer& rhs)
		: EnergyMinimizer(rhs),
			number_of_atoms_(rhs.number_of_atoms_),
			max_num_of_vect_pairs_(rhs.max_num_of_vect_pairs_),
			curr_num_of_vect_pairs_(rhs.curr_num_of_vect_pairs_),
			rho_(rhs.rho_),
			stored_s_(rhs.stored_s_),
			stored_y_(rhs.stored_y_),
			initial_atoms_(rhs.initial_atoms_),
			work_val_(rhs.work_val_),
			index_of_free_vect_(rhs.index_of_free_vect_),
			cutlo_(rhs.cutlo_)
	{
	}

  // The assignment operator
  const StrangLBFGSMinimizer& StrangLBFGSMinimizer::operator =
		(const StrangLBFGSMinimizer& rhs)
	{
    EnergyMinimizer::operator = (rhs);
		number_of_atoms_ = rhs.number_of_atoms_;
		max_num_of_vect_pairs_ = rhs.max_num_of_vect_pairs_;
		curr_num_of_vect_pairs_ = rhs.curr_num_of_vect_pairs_;
		rho_ = rhs.rho_;
		stored_s_ = rhs.stored_s_;
		stored_y_ = rhs.stored_y_;
		initial_atoms_ = rhs.initial_atoms_;
		work_val_ = rhs.work_val_;
		index_of_free_vect_ = rhs.index_of_free_vect_;
		cutlo_ = rhs.cutlo_;
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

		// Set the options to their default values if not already set
		max_num_of_vect_pairs_ = DEFAULT_NUMBER_OF_VECTOR_PAIRS;

		// Determine the number of atoms
		number_of_atoms_ = force_field_->getNumberOfMovableAtoms();
		
    return true;
	}

  // Set explicitly the maximum number of stored vector pairs.
	void StrangLBFGSMinimizer::setNumOfStoredVects(Size num)
	{
		max_num_of_vect_pairs_ = num;
	}

  // Get the value of the maximum number of stored vector pairs.
	Size StrangLBFGSMinimizer::getNumOfStoredVects() const 
	{
		return max_num_of_vect_pairs_; 
	}


	void StrangLBFGSMinimizer::updateDirection()
	{
		// Just in case: check whether 
		//  (1) (enough) memory is allocated for our search direction
		//  (2) the recent search direction is valid
		//  (3) both the initial and the current gradient are invalid.
		if (!direction_.isValid() || (direction_.size() == 0) || !initial_grad_.isValid() || !current_grad_.isValid())
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
			return;
		}
		
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
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
			stored_y_[k] = current_grad_[i] - initial_grad_[i];
			stored_s_[k] = atoms[i]->getPosition() - initial_atoms_[i];
			
			sty += stored_y_[k]*stored_s_[k];
			yty += stored_y_[k]*stored_y_[k];
		}
		
		direction_ = current_grad_;
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
			
			if (sty > 0.)
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
		
		// Scaling
		for(r = 0; r < number_of_atoms_; ++r)
		{
			direction_[r] *= scale;
		}
		
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
			if (k == curr_num_of_vect_pairs_)
			{
				k = 0;
			}
		}
		
		// Compute norm and current directional derivative.
		double norm = 0.0;
		double dir_d = 0.0;
		for (i = 0; i < number_of_atoms_; i++)
		{
			norm += direction_[i] * direction_[i];
			dir_d += direction_[i] * current_grad_[i];
		}
		norm = sqrt(norm);
		
		if (dir_d > 0.)
		{
			// If the current search direction is NOT a descent direction
			// something went wrong. We set the search direction to the 
			// normalized negative gradient.
			direction_ = current_grad_;
			direction_.negate();
			direction_.normalize();
		}
		else
		{
			// Don't risc a "NaN"
			if (norm >= cutlo_)
			{
				direction_.inv_norm = 1.0 / norm;
			}
		
			// Assign the norm and rms of the new direction
			direction_.norm = norm;
			direction_.rms = norm / (3.0 * (double)number_of_atoms_);
		}
		
		// Set the index for the next update
		index_of_free_vect_ = (index_of_free_vect_+1) % max_num_of_vect_pairs_;
		
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
			
			// Allocate our memory needed for the algorithm
			rho_.resize(max_num_of_vect_pairs_);
			stored_s_.resize(max_num_of_vect_pairs_*number_of_atoms_);
			stored_y_.resize(max_num_of_vect_pairs_*number_of_atoms_);
			work_val_.resize(max_num_of_vect_pairs_);
			initial_atoms_.resize(number_of_atoms_);

			// If we do not have a valid gradient, recalculate the gradient, the energy,
			// and the search direction
			if (!initial_grad_.isValid())
			{
				#ifdef BALL_DEBUG
					Log.info() << "StrangLBFGSMinimizer::minimize calculating initial gradient...";
				#endif

				// Compute the initial energy and the initial forces
				updateEnergy();
				updateForces();

				// Store them for later use
				storeGradientEnergy();

				// Obviously, we don't have "old" energies yet, so we initialize it a with 
				// sensible value. We don't need "old" gradients.
				old_energy_ = Limits<float>::max();

				// The first search direction is the normalized negative gradient
				direction_ = current_grad_;
				direction_.negate();
				direction_.normalize();
				
				// Copy atom positions.
				// AR: TODO: avoid this bloody necessity
				for(Size i = 0; i < number_of_atoms_; ++i)
				{
					initial_atoms_[i] = atoms[i]->getPosition();
				}
				
				#ifdef BALL_DEBUG
					Log.info() << " [" << current_grad_.size() << "/" << current_grad_.norm << "/" << current_grad_.rms << " / " << initial_energy_ << "]" << endl;
				#endif
			}
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
			// Try to take a new step along the direction
			double stp = findStep();
			
			// Check whether we were successful.
			if (stp > 0.0)
			{
				// Use this step as new reference step if findStep was successful
				atoms.savePositions();
				
				// Do all updates we must do, i.e. update of the stored vectors and
				// computation of the new search direction. Since everything went o.k.
				// we operate on initial_grad_ and current_grad_.
				updateDirection();
				
				// AR: TODO: avoid this bloody necessity. ---------------------------------------------------
				for(Size i = 0; i < number_of_atoms_; ++i)
				{
					initial_atoms_[i] = atoms[i]->getPosition();
				}
				// ------------------------------------------------------------------------------------------

			}
			else
			{
				// No success. We proceed with a restart.

				// Let's see if we have to recompute the forces and the energy
				if (!current_grad_.isValid())
				{
					#ifdef BALL_DEBUG
						Log << "StrangLBFGSMinimizer::minimize: Computing new energy/forces!" << std::endl;
					#endif
					// ...calculate energy and forces for the new position
					updateForces();
					updateEnergy();
				}
				
				// Set the search direction to the normalized negative gradient. Since we proceed
				// with a restart, we mustn't update the stored vectors by 'updateDirection' and there is
				// no need to compute anything by 'updateDirection'.
				direction_ = current_grad_;
				direction_.negate();
				direction_.normalize();
				
				// We cannot trust in our data any more, so we force all routines to 
				// assume that we haven't collected any data so far.
				curr_num_of_vect_pairs_ = 0;
				index_of_free_vect_ = 0;
			}
			
			// Store the energy, we don't need to store the initial gradient as "old" gradient.
			old_energy_ = initial_energy_;
			
			// Store the current gradient and energy
			storeGradientEnergy();

			#ifdef BALL_DEBUG
				Log.info() << "StrangLBFGSMinimizer::minimize: end of main: current grad RMS = " << current_grad_.rms << std::endl;
			#endif

			// Check for convergence.
			converged = isConverged();
			
			// Increment iteration counter, take snapshots, print energy,
			// update pair lists, and check the same-energy counter
			finishIteration();

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
		#ifdef BALL_DEBUG
			Log.info() << "StrangLBFGSMinimizer::findStep: " << initial_energy_ << " " << current_grad_.norm << " " << direction_.norm << endl;
		#endif

		// We perform a line search along direction_
		LineSearch line_search(*this);

		double step;
		bool result = line_search.minimize(step);
		
		#ifdef BALL_DEBUG
			Log.info() << "LineSearch: step = " << step << " result = " << result << endl;
		#endif

		// If this line search fails we do an internal restart.
		if (!result)
		{
			// Reset the search direction to the normalized negative gradient
			direction_ = initial_grad_;
			direction_.negate();
			direction_.normalize();
			
			#ifdef BALL_DEBUG
				Log.info() << direction_.rms << "]" << endl;
			#endif

			// Invalidate the current gradient (LineSearch::minimize())
			// recalculate it for step = 1.0
			current_grad_.invalidate();

			// ...and try another line search
			result = line_search.minimize(step);
			
			#ifdef BALL_DEBUG
				Log.info() << "LineSearch: step = " << step << " result = " << result << endl;
			#endif
			
			// We cannot trust in our data any more, so we force all routines to 
			// assume that we haven't collected any data so far.
			curr_num_of_vect_pairs_ = 0;
			index_of_free_vect_ = 0;
		}

		if (result == true)
		{
			return step;
		} 
		else 
		{
			return -1.0;
		}
	} // end of method 'findStep'

} // end of namespace BALL
