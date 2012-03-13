// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: shiftedLVMM.C,v 1.1.4.6 2007/08/07 09:13:06 aleru Exp $
//
// Minimize the potential energy of a system using a shifted 
// limited-memory variable metric method.


#include <BALL/MOLMEC/MINIMIZATION/shiftedLVMM.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

#include <limits>

//#define BALL_DEBUG
#undef BALL_DEBUG

// The default type of correction parameter
#define DEFAULT_CORRECTION_PARAMETER RATIO_OF_SHIFT_PARAMS

// The default method
#define DEFAULT_UPDATE_METHOD RANK_2

// The default number of columns of the factor of the shifted 
// inverse hessian approximation.
#define DEFAULT_NUMBER_OF_COLUMNS 5

namespace BALL 
{
	using namespace std; 
	
	const char* ShiftedLVMMMinimizer::Option::UPDATE_METHOD = "update_method";
	const char* ShiftedLVMMMinimizer::Option::CORRECTION_PARAMETER = "correction_parameter";
	const char* ShiftedLVMMMinimizer::Option::NUM_OF_COLUMNS = "num_of_columns";
	
	const Size ShiftedLVMMMinimizer::Default::UPDATE_METHOD = DEFAULT_UPDATE_METHOD;
	const Size ShiftedLVMMMinimizer::Default::CORRECTION_PARAMETER = DEFAULT_CORRECTION_PARAMETER;
	const Size ShiftedLVMMMinimizer::Default::NUM_OF_COLUMNS = DEFAULT_NUMBER_OF_COLUMNS;
	
	
	// Default constructor
	ShiftedLVMMMinimizer::ShiftedLVMMMinimizer()
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_number_of_cols_(DEFAULT_NUMBER_OF_COLUMNS),
			curr_number_of_cols_(0),
			updt_method_(DEFAULT_UPDATE_METHOD),
			corr_par_(DEFAULT_CORRECTION_PARAMETER),
			prev_shift_val_(1.),
			shift_s_(),
			grad_diff_(),
			updt_u_(),
			updt_v_(),
			shifted_direction_(),
			hess_factor_(),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		options.setDefaultInteger(Option::CORRECTION_PARAMETER, Default::CORRECTION_PARAMETER);
		options.setDefaultInteger(Option::NUM_OF_COLUMNS, Default::NUM_OF_COLUMNS);
	}
	
	// Constructor initialized with a force field
	ShiftedLVMMMinimizer::ShiftedLVMMMinimizer(ForceField& force_field)
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_number_of_cols_(DEFAULT_NUMBER_OF_COLUMNS),
			curr_number_of_cols_(0),
			updt_method_(DEFAULT_UPDATE_METHOD),
			corr_par_(DEFAULT_CORRECTION_PARAMETER),
			prev_shift_val_(1.),
			shift_s_(),
			grad_diff_(),
			updt_u_(),
			updt_v_(),
			shifted_direction_(),
			hess_factor_(),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		options.setDefaultInteger(Option::CORRECTION_PARAMETER, Default::CORRECTION_PARAMETER);
		options.setDefaultInteger(Option::NUM_OF_COLUMNS, Default::NUM_OF_COLUMNS);
		
		valid_ = setup(force_field);
		
		if (!valid_)
		{
			Log.error() << "ShiftedLVMMMinimizer: setup failed! " << endl;
		}
	}

	// Constructor initialized with a force field and a snapshot manager 
	ShiftedLVMMMinimizer::ShiftedLVMMMinimizer
		(ForceField& force_field, SnapShotManager* ssm)
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_number_of_cols_(DEFAULT_NUMBER_OF_COLUMNS),
			curr_number_of_cols_(0),
			updt_method_(DEFAULT_UPDATE_METHOD),
			corr_par_(DEFAULT_CORRECTION_PARAMETER),
			prev_shift_val_(1.),
			shift_s_(),
			grad_diff_(),
			updt_u_(),
			updt_v_(),
			shifted_direction_(),
			hess_factor_(),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		options.setDefaultInteger(Option::CORRECTION_PARAMETER, Default::CORRECTION_PARAMETER);
		options.setDefaultInteger(Option::NUM_OF_COLUMNS, Default::NUM_OF_COLUMNS);
		
		valid_ = setup(force_field, ssm);
		
		if (!valid_)
		{
			Log.error() << "ShiftedLVMMMinimizer: setup failed! " << endl;
		}
	}

	
	// Constructor initialized with a force field and a set of options
	ShiftedLVMMMinimizer::ShiftedLVMMMinimizer
		(ForceField& force_field, const Options& new_options) 
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_number_of_cols_(DEFAULT_NUMBER_OF_COLUMNS),
			curr_number_of_cols_(0),
			updt_method_(DEFAULT_UPDATE_METHOD),
			corr_par_(DEFAULT_CORRECTION_PARAMETER),
			prev_shift_val_(1.),
			shift_s_(),
			grad_diff_(),
			updt_u_(),
			updt_v_(),
			shifted_direction_(),
			hess_factor_(),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options = new_options;
		
		// The actual work is done in setup 
		valid_ = setup(force_field, new_options);
		
		if (!valid_)
		{
			Log.error() << "ShiftedLVMMMinimizer: setup failed! " << endl; 
		}
	}
	
	// Constructor initialized with a force field, a snapshot manager, and a set of options
	ShiftedLVMMMinimizer::ShiftedLVMMMinimizer
		(ForceField& force_field, SnapShotManager* ssm, const Options& new_options)
		: EnergyMinimizer(),
			line_search_(),
			first_iter_(true),
			number_of_atoms_(0),
			max_number_of_cols_(DEFAULT_NUMBER_OF_COLUMNS),
			curr_number_of_cols_(0),
			updt_method_(DEFAULT_UPDATE_METHOD),
			corr_par_(DEFAULT_CORRECTION_PARAMETER),
			prev_shift_val_(1.),
			shift_s_(),
			grad_diff_(),
			updt_u_(),
			updt_v_(),
			shifted_direction_(),
			hess_factor_(),
			initial_atoms_()
	{
		line_search_.setMinimizer(*this);

		options = new_options;
		
		// The actual work is done in setup 
		valid_ = setup(force_field, ssm, new_options);
		
		if (!valid_)
		{
			Log.error() << "ShiftedLVMMMinimizer: setup failed! " << endl; 
		}
	}
	
	
	// The destructor
	ShiftedLVMMMinimizer::~ShiftedLVMMMinimizer()
	{
	}
	
	
	// The copy constructor 
	ShiftedLVMMMinimizer::ShiftedLVMMMinimizer 
		(const ShiftedLVMMMinimizer& rhs)
		: EnergyMinimizer(rhs),
			line_search_(rhs.line_search_),
			first_iter_(rhs.first_iter_),
			number_of_atoms_(rhs.number_of_atoms_),
			max_number_of_cols_(rhs.max_number_of_cols_),
			curr_number_of_cols_(rhs.curr_number_of_cols_),
			updt_method_(rhs.updt_method_),
			corr_par_(rhs.corr_par_),
			prev_shift_val_(rhs.prev_shift_val_),
			shift_s_(rhs.shift_s_),
			grad_diff_(rhs.grad_diff_),
			updt_u_(rhs.updt_u_),
			updt_v_(rhs.updt_v_),
			shifted_direction_(rhs.shifted_direction_),
			hess_factor_(rhs.hess_factor_),
			initial_atoms_(rhs.initial_atoms_)
	{
		line_search_.setMinimizer(*this);
	}
	
	// The assignment operator
	const ShiftedLVMMMinimizer& ShiftedLVMMMinimizer::operator =
		(const ShiftedLVMMMinimizer& rhs)
	{
		EnergyMinimizer::operator = (rhs);
		line_search_         = rhs.line_search_;
		first_iter_          = rhs.first_iter_;
		number_of_atoms_     = rhs.number_of_atoms_;
		max_number_of_cols_  = rhs.max_number_of_cols_;
		curr_number_of_cols_ = rhs.curr_number_of_cols_;
		updt_method_         = rhs.updt_method_;
		corr_par_            = rhs.corr_par_;
		prev_shift_val_      = rhs.prev_shift_val_;
		shift_s_             = rhs.shift_s_;
		grad_diff_           = rhs.grad_diff_;
		updt_u_              = rhs.updt_u_;
		updt_v_              = rhs.updt_v_;
		shifted_direction_   = rhs.shifted_direction_;
		hess_factor_         = rhs.hess_factor_;
		initial_atoms_       = rhs.initial_atoms_;
		
		line_search_.setMinimizer(*this);
		return *this;
	}
	
	// This method is responsible for doing the specific setup of this class       
	bool ShiftedLVMMMinimizer::specificSetup()
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
		updt_method_        = (UpdateMethod) options.getInteger(Option::UPDATE_METHOD);
		max_number_of_cols_ = (Size) options.getInteger(Option::NUM_OF_COLUMNS);
		corr_par_           = (CorrectionParameter) options.getInteger(Option::CORRECTION_PARAMETER);
		
		// Determine the number of atoms
		number_of_atoms_ = force_field_->getNumberOfMovableAtoms();
		
		// Start with the (initial) first iteration
		first_iter_ = true;
		
		return true;
	}
	
	// Set explicitly the update method
	void ShiftedLVMMMinimizer::setUpdateMethod(UpdateMethod updt)
	{
		updt_method_ = updt;
		options.setInteger(Option::UPDATE_METHOD, (Size) updt);
	}
	
	// Return the update method
	ShiftedLVMMMinimizer::UpdateMethod ShiftedLVMMMinimizer::getUpdateMethod() const
	{
		return (UpdateMethod)updt_method_;
	}

	// Set explicitly the correction parameter
	void ShiftedLVMMMinimizer::setCorrectionParameter(CorrectionParameter corr)
	{
		corr_par_ = corr;
		options.setInteger(Option::CORRECTION_PARAMETER, (Size) corr);
	}
	
	// Return the correction parameter
	ShiftedLVMMMinimizer::CorrectionParameter ShiftedLVMMMinimizer::getCorrectionParameter() const
	{
		return (CorrectionParameter)corr_par_;
	}
			
	/* Set explicitly the maximum number of columns of the
	   factor of the shifted inverse hessian approximation.
	*/
	void ShiftedLVMMMinimizer::setMaxNumOfColumns(Size num)
	{
		max_number_of_cols_ = num;
		options.setInteger(Option::NUM_OF_COLUMNS, num);
	}
	
	/* Return the maximum number of columns of the
	   factor of the shifted inverse hessian approximation.
	*/
	Size ShiftedLVMMMinimizer::getMaxNumOfColumns() const
	{
		return max_number_of_cols_;
	}
	
	void ShiftedLVMMMinimizer::updateDirection()
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
		//  (2) the old gradient is valid
		if ((direction_.size() == 0) || !old_grad_.isValid())
		{
			// This mustn't happen at all. If this routine is called all data(-structures) should be fine.
			// But we are here! Something went wrong and we don't know what. Most kindly the data is
			// modified by an external program/thread, so we do the best we can do: first we update the forces
			// and set the direction to the negative gradient. Additionally, we force a restart.
			Log.error() << "dir: " << direction_.isValid() 
									<< "  initial_grad: " << initial_grad_.isValid() 
									<< "  current_grad: " << current_grad_.isValid() << endl;
			Log.error() << "ShiftedLVMMMinimizer::updateDirection: invalid gradient or direction " << endl;
			
			// Calculate the current gradient and determine the current
			// direction as the direction of steepest descent.
			updateForces();
			direction_ = current_grad_;
			direction_.negate();
			direction_.normalize();
			
			// Discard all data we have collected so far since we cannot trust in it any longer.
			curr_number_of_cols_ = 0;
			prev_shift_val_ = 1.;
			return;
		}
		
		// Computation of the shift parameter.
		
		// Compute the difference of gradients, the previous step (store it in shift_s_)
		// and compute a few other values which are named after their computation formula,
		// e.g. sty means s^t*y, the dot product between s and y.
		double yty = 0.;
		double sts = 0.;
		double sty = 0.;
		for(Size i = 0; i < number_of_atoms_; ++i)
		{
			grad_diff_[i] = initial_grad_[i] - old_grad_[i];
			shift_s_[i] = atoms[i]->getPosition() - initial_atoms_[i];
			
			sty += shift_s_[i] * grad_diff_[i];
			sts += shift_s_[i] * shift_s_[i];
			yty += grad_diff_[i] * grad_diff_[i];
		}
		
		double shift_val;
		Size i, j, k;
		
		// Check whether an update makes sense at all
		if (sty > 0.)
		{
			// Compute u = U^t*y and u^t*u
			double utu = 0.;
			for(i = 0, k = 0; i < curr_number_of_cols_; ++i)
			{
				double tmp = 0.;
				for(Size j = 0; j < number_of_atoms_; ++j, ++k)
				{
					tmp += hess_factor_[k]*grad_diff_[j];
				}
				updt_u_[i] = tmp;
				utu += tmp*tmp;
			}
			
			// ytHy means y^t*H*y, the other values are taken from Luksan et al.
			double ytHy    = prev_shift_val_*yty + utu;
			double epsilon = sqrt(max(0.,1.-utu/ytHy));
			double mu      = epsilon/(1.+sqrt(max(0., 1.-(sty/sts)*(sty/yty))));
			double gamma   = sty/yty;
			
			// The new shift value for this iteration.
			if (ytHy > 0.)
			{
				shift_val = mu*gamma;
				
				// Use safeguards
				shift_val = max(shift_val, gamma/5.);
				shift_val = min(shift_val, gamma/1.25);
			}
			else
			{
				shift_val = gamma/4.;
			}
			
			// Compute the shifted (previous) step with the new shift value.
			for(i = 0; i < number_of_atoms_; ++i)
			{
				shift_s_[i] -= grad_diff_[i]*shift_val;
			}
			
			// Compute \tilde{s}^t*y where \tilde{s} means the shifted step.
			double ssty = sty - shift_val*yty;
			
			// Check whether we are in the build up stage or in the update stage.
			if (curr_number_of_cols_ < max_number_of_cols_)
			{
				// We are in the build up stage.
				
				// Compute the correction for the already known columns.
				// Use the column order, so k is running over all entries.
				double tmp = -1./ssty;
				
				for(i = 0, k = 0; i < curr_number_of_cols_; ++i)
				{
					for(j = 0; j < number_of_atoms_; ++j, ++k)
					{
						hess_factor_[k] += shift_s_[j]*(tmp*updt_u_[i]);
					}
				}
				
				// Set the new column...
				k = curr_number_of_cols_*number_of_atoms_;
				ssty = sqrt(ssty);
				for(j = 0; j < number_of_atoms_; ++j, ++k)
				{
					hess_factor_[k] = shift_s_[j]/ssty;
				}
			
				// ... and increase the counter for the current number of columns
				++curr_number_of_cols_;
			}
			else
			{
				// We are in the update stage.
				
				// First: Determination of correction parameter rho.
				double rho;
				switch(corr_par_)
				{
					case UNIT_VALUE:
					{
						rho = 1.;
						break;
					}
					case BALANCING_VALUE:
					{
						rho = shift_val*yty/ssty;
						break;
					}
					case SQUARE_ROOT:
					{
						rho = epsilon;
						break;
					}
					case GEOMETRIC_MEAN:
					{
						rho = sqrt(epsilon*(shift_val*yty/ssty));
						break;
					}
					default:
					{
						rho = prev_shift_val_/(prev_shift_val_ + shift_val);
					}
				}
				
				// Second: Determination of the parameter theta from [3].
				double vtv = 0.;
				double utv = 0.;
				for(i = 0; i < curr_number_of_cols_; ++i)
				{
					vtv += updt_v_[i]*updt_v_[i];
					utv += updt_u_[i]*updt_v_[i];
				}
				
				double theta = (utv > 0.) ? sqrt(rho*ssty/vtv/step_/step_) : -sqrt(rho*ssty/vtv/step_/step_);
				
				// Now we can proceed with the update.
				if (updt_method_ == RANK_1)
				{
					// The update method is based on the rank one correction in [3].
					
					double s = -1./(rho*ssty + theta*utv*step_);
					for(i = 0, k = 0; i < curr_number_of_cols_; ++i)
					{
						double tmp = updt_u_[i] + theta*step_*updt_v_[i];
						for(j = 0; j < number_of_atoms_; ++j, ++k)
						{
							hess_factor_[k] += (shift_s_[j]*rho + shifted_direction_[j]*theta*step_)*(tmp*s);
						}
					}
				}
				else
				{
					// The update method is based on the rank two correction in [3].
					
					double tmp = rho/theta - utv*step_/ssty;
					for(i = 0, k = 0; i < curr_number_of_cols_; ++i)
					{
						for(j = 0; j < number_of_atoms_; ++j, ++k)
						{
							hess_factor_[k] -= shift_s_[j]*(updt_u_[i]/ssty)
									+ (shifted_direction_[j]*step_+shift_s_[j]*tmp)*(updt_v_[i]/vtv/step_);
						}
					}
				}
			}
			// Now we have done all updates concerning the inverse hessian approximation.
				
				// Compute the new direction, make use of the column order
				// First: compute v
			for(i = 0, k = 0; i < curr_number_of_cols_; ++i)
			{
				double tmp = 0.;
				for(j = 0; j < number_of_atoms_; ++j, ++k)
				{
					tmp -= hess_factor_[k]*initial_grad_[j];
				}
				updt_v_[i] = tmp;
			}
				
				// Second: compute the shifted direction
				// Initialize the direction by the first scaled vector instead of 0.
			for(j = 0; j < number_of_atoms_; ++j)
			{
				shifted_direction_[j] = hess_factor_[j] * updt_v_[0];
			}
				// Compute the remaining vector additions
			for(i = 1, k = number_of_atoms_; i < curr_number_of_cols_; ++i)
			{
				for(j = 0; j < number_of_atoms_; ++j, ++k)
				{
					shifted_direction_[j] += hess_factor_[k] * updt_v_[i];
				}
			}
				
				// Third: Compute the (unshifted) search direction
			for(j = 0; j < number_of_atoms_; ++j)
			{
				direction_[j] = shifted_direction_[j] - (initial_grad_[j]*shift_val);
			}
				
				// Shift value of this direction computation was shift_val
			prev_shift_val_ = shift_val;
				
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
					// negative normalized gradient and force a 'restart'.
				direction_ = initial_grad_;
				direction_.negate();
				direction_.normalize();
				curr_number_of_cols_ = 0;
				prev_shift_val_ = 1.;
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
			}
		}
		else
		{
			direction_ = initial_grad_;
			direction_.negate();
			direction_.normalize();
			curr_number_of_cols_ = 0;
			prev_shift_val_      = 1.;
		}
		
		// Save old positions
		for(Size i = 0; i < number_of_atoms_; ++i)
		{
			initial_atoms_[i] = atoms[i]->getPosition();
		}
		
	} // end of method 'updateDirection'
	
	
	// The minimizer optimizes the energy of the system 
	// by using a shifted limited memory variable metric method proposed
	// by Luksan et al [3].
	// Return value is true when no further steps can be taken!
	bool ShiftedLVMMMinimizer::minimize(Size iterations, bool resume)
	{
		aborted_ = false;
		
		// Check for validity of minimizer and force field
		if (!isValid() || getForceField() == 0 || !getForceField()->isValid())
		{
			Log.error() << "ShiftedLVMMMinimizer::minimize: Minimizer is not initialized correctly!" << std::endl;
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
		if ((Size)options.getInteger(Option::NUM_OF_COLUMNS) != max_number_of_cols_)
		{
			max_number_of_cols_ = (Size) options.getInteger(Option::NUM_OF_COLUMNS);
			resume = false;
		}
		updt_method_ = (Size) options.getInteger(Option::UPDATE_METHOD);
		corr_par_    = (Size) options.getInteger(Option::CORRECTION_PARAMETER);

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
			curr_number_of_cols_ = 0;
			prev_shift_val_ = 1.;
			current_grad_.invalidate();
			initial_grad_.invalidate();
			first_iter_ = true;
			
			// Allocate our memory needed for the algorithm
			shift_s_.resize(number_of_atoms_);
			grad_diff_.resize(number_of_atoms_);
			updt_u_.resize(max_number_of_cols_);
			updt_v_.resize(max_number_of_cols_);
			shifted_direction_.resize(number_of_atoms_);
			hess_factor_.resize(max_number_of_cols_*number_of_atoms_);
			initial_atoms_.resize(number_of_atoms_);
			
			// Obviously, we don't have "old" energies yet, so we initialize it a with 
			// sensible value. We don't need "old" gradients.
			old_energy_ = std::numeric_limits<float>::max();
		}
		
		Size max_iterations = std::min(getNumberOfIterations() + iterations, getMaxNumberOfIterations());
		
		#ifdef BALL_DEBUG
			Log.info() << "ShiftedLVMMMinimizer: minimize(" << iterations << ", " << resume << ")" << endl;
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
				Log.info() << "ShiftedLVMMMinimizer::minimize: end of main: current grad RMS = " << current_grad_.rms << std::endl;
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
	
	double ShiftedLVMMMinimizer::findStep()
	{
		// Compute the new direction
		updateDirection();
		
		#ifdef BALL_DEBUG
			Log.info() << "ShiftedLVMMMinimizer::findStep: " << initial_energy_ << " " << current_grad_.norm << " " << direction_.norm << endl;
		#endif
		
		// We perform a line search along direction_
		
		// No need to assure the maximum displacement here since our
		// line search pays attention to this constraint.
		
		bool result = line_search_.minimize(step_);
		
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
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
			curr_number_of_cols_ = 0;
			prev_shift_val_      = 1.;
			Size iter            = 0;
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
			// If we are here something went wrong
			// Not even such scaled steepest descent steps can manage
			// the line search to exit successfully?
			// We must be at a local minimizer...
			step_ = 0.;
		}
			
		#ifdef BALL_DEBUG
			Log.info() << "LineSearch: step = " << step << " result = " << result << endl;
		#endif
		
		return step_;
	} // end of method 'findStep'
	
} // end of namespace BALL
