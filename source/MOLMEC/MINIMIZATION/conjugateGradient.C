// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conjugateGradient.C,v 1.38.8.7 2007/08/07 09:13:05 aleru Exp $
//
// Minimize the potential energy of a system using a nonlinear conjugate 
// gradient method with  line search

#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/MOLMEC/COMMON/forceField.h>

#include <limits>

//#define BALL_DEBUG
#undef BALL_DEBUG

// The default method to use for the CG direction update
// (FLETCHER_REEVES | POLAK_RIBIERE | SHANNO)
#define DEFAULT_METHOD SHANNO

namespace BALL 
{
	using namespace std; 
	
	// Set the default values for options of this class 
	const char* ConjugateGradientMinimizer::Option::UPDATE_METHOD = "update_method"; 
	
	const Size ConjugateGradientMinimizer::Default::UPDATE_METHOD = DEFAULT_METHOD; 
	
	// Default constructor 
	// It does nothing but calling its base class constructor 
  ConjugateGradientMinimizer::ConjugateGradientMinimizer()
		: EnergyMinimizer(),
			line_search_(),
			unscaled_direction_(),
			number_of_atoms_(0),
			updt_method_(DEFAULT_METHOD),
			first_iter_(true),
			old_gtg_(0.),
			a_i_(),
			b_i_(),
			p_t_(),
			y_t_(),
			p_i_(),
			y_i_(),
			D_1_(0.),
			D_4_(0.),
			restart_frequency_(1),
			last_restart_iter_(0)
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
	}

	// Constructor initialized with a force field
	ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field)
		: EnergyMinimizer(),
			line_search_(),
			unscaled_direction_(),
			number_of_atoms_(0),
			updt_method_(DEFAULT_METHOD),
			first_iter_(true),
			old_gtg_(0.),
			a_i_(),
			b_i_(),
			p_t_(),
			y_t_(),
			p_i_(),
			y_i_(),
			D_1_(0.),
			D_4_(0.),
			restart_frequency_(1),
			last_restart_iter_(0)
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		valid_ = setup(force_field);
		
		if (!valid_)
		{
			Log.error() << "ConjugateGradientMinimizer: setup failed! " << endl;
		}
	}

	// Constructor initialized with a force field and a snapshot manager 
	ConjugateGradientMinimizer::ConjugateGradientMinimizer
		(ForceField& force_field, SnapShotManager* ssm)
		: EnergyMinimizer(),
			line_search_(),
			unscaled_direction_(),
			number_of_atoms_(0),
			updt_method_(DEFAULT_METHOD),
			first_iter_(true),
			old_gtg_(0.),
			a_i_(),
			b_i_(),
			p_t_(),
			y_t_(),
			p_i_(),
			y_i_(),
			D_1_(0.),
			D_4_(0.),
			restart_frequency_(1),
			last_restart_iter_(0)
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		valid_ = setup(force_field, ssm);
		
		if (!valid_)
		{
			Log.error() << "ConjugateGradientMinimizer: setup failed! " << endl;
		}
	}

	
	// Constructor initialized with a force field and a set of options
	ConjugateGradientMinimizer::ConjugateGradientMinimizer
		(ForceField& force_field, const Options& new_options) 
		: EnergyMinimizer(),
			line_search_(),
			unscaled_direction_(),
			number_of_atoms_(0),
			updt_method_(DEFAULT_METHOD),
			first_iter_(true),
			old_gtg_(0.),
			a_i_(),
			b_i_(),
			p_t_(),
			y_t_(),
			p_i_(),
			y_i_(),
			D_1_(0.),
			D_4_(0.),
			restart_frequency_(1),
			last_restart_iter_(0)
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		
		// The actual work is done in setup 
		valid_ = setup(force_field, new_options);
		
		if (!valid_)
		{
			Log.error() << " Setup of conjugate gradient minimizer has failed! " << endl; 
		}
	}
	
	// Constructor initialized with a force field, a snapshot manager, and a set of options
	ConjugateGradientMinimizer::ConjugateGradientMinimizer
		(ForceField& force_field, SnapShotManager* ssm, const Options& new_options)
		: EnergyMinimizer(),
			line_search_(),
			unscaled_direction_(),
			number_of_atoms_(0),
			updt_method_(DEFAULT_METHOD),
			first_iter_(true),
			old_gtg_(0.),
			a_i_(),
			b_i_(),
			p_t_(),
			y_t_(),
			p_i_(),
			y_i_(),
			D_1_(0.),
			D_4_(0.),
			restart_frequency_(1),
			last_restart_iter_(0)
	{
		line_search_.setMinimizer(*this);

		options.setDefaultInteger(Option::UPDATE_METHOD, Default::UPDATE_METHOD);
		
		// The actual work is done in setup 
		valid_ = setup(force_field, ssm, new_options);
		
		if (!valid_)
		{
			Log.error() << " Setup of conjugate gradient minimizer has failed! " << endl; 
		}
	}
	
	// Destructor
	ConjugateGradientMinimizer::~ConjugateGradientMinimizer()
	{
	}
	
	// The copy constructor 
	ConjugateGradientMinimizer::ConjugateGradientMinimizer 
		(const ConjugateGradientMinimizer& rhs)
		: EnergyMinimizer(rhs),
			line_search_(rhs.line_search_),
			unscaled_direction_(rhs.unscaled_direction_),
			number_of_atoms_(rhs.number_of_atoms_),
			updt_method_(rhs.updt_method_),
			first_iter_(rhs.first_iter_),
			old_gtg_(rhs.old_gtg_),
			a_i_(rhs.a_i_),
			b_i_(rhs.b_i_),
			p_t_(rhs.p_t_),
			y_t_(rhs.y_t_),
			p_i_(rhs.p_i_),
			y_i_(rhs.y_i_),
			D_1_(rhs.D_1_),
			D_4_(rhs.D_4_),
			restart_frequency_(rhs.restart_frequency_),
			last_restart_iter_(rhs.last_restart_iter_)
	{
		line_search_.setMinimizer(*this);
	}
	
	// The assignment operator
	const ConjugateGradientMinimizer& ConjugateGradientMinimizer::operator =
		(const ConjugateGradientMinimizer& rhs)
	{
		EnergyMinimizer::operator = (rhs);
		line_search_        = rhs.line_search_;
		updt_method_        = rhs.updt_method_;
		unscaled_direction_ = rhs.unscaled_direction_;
		number_of_atoms_    = rhs.number_of_atoms_;
		first_iter_         = rhs.first_iter_;
		old_gtg_            = rhs.old_gtg_;
		a_i_                = rhs.a_i_;
		b_i_                = rhs.b_i_;
		p_t_                = rhs.p_t_;
		y_t_                = rhs.y_t_;
		p_i_                = rhs.p_i_;
		y_i_                = rhs.y_i_;
		D_1_                = rhs.D_1_;
		D_4_                = rhs.D_4_;
		restart_frequency_  = rhs.restart_frequency_;
		last_restart_iter_  = rhs.last_restart_iter_;
		line_search_.setMinimizer(*this);
		return *this;
	}
	
	// This method is responsible for doing the specific setup of this class       
	bool ConjugateGradientMinimizer::specificSetup()
	{
		// Make sure the force field is assigned and valid!
		if (force_field_ == 0 || !force_field_->isValid())
		{
			return false;
		}
		
		// Invalidate the initial gradient in order to ensure
		// its re-evaluation at the start of minimize().
		initial_grad_.invalidate();
		old_grad_.invalidate();
		
		// Get the options
		updt_method_ = (Size) options.getInteger(Option::UPDATE_METHOD);
		
		// determine the number of atoms
		number_of_atoms_ = (Size)force_field_->getNumberOfMovableAtoms();
		
		// Reset the restart counter
		last_restart_iter_ = 0;
		
		return true;
	}
	
	// Set explicitly the update method
	void ConjugateGradientMinimizer::setUpdateMethod(UpdateMethod updt)
	{
		updt_method_ = updt;
		options.setInteger(Option::UPDATE_METHOD, (Size)updt);
	}
	
	// Return the update method
	ConjugateGradientMinimizer::UpdateMethod ConjugateGradientMinimizer::getUpdateMethod() const
	{
		return (UpdateMethod)updt_method_;
	}
	
	// This method determines the new search direction. Along this
	// vector we will try to find the next solution.
	void ConjugateGradientMinimizer::updateDirection()
	{
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
			// This is the first iteration. We use the
			// normalized negative gradient as first direction
			direction_ = initial_grad_;
			direction_.negate();
			if (updt_method_ != SHANNO)
			{
				old_gtg_ = 0.;
				for(Size i = 0; i < number_of_atoms_; ++i)
				{
					// We do the calculation in 'double' accuracy
					old_gtg_ += (double)initial_grad_[i].x*(double)initial_grad_[i].x;
					old_gtg_ += (double)initial_grad_[i].y*(double)initial_grad_[i].y;
					old_gtg_ += (double)initial_grad_[i].z*(double)initial_grad_[i].z;
				}
				unscaled_direction_ = direction_;
			}
			direction_.normalize();
			first_iter_        = false;
			last_restart_iter_ = 0;
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
									<< "  old_grad: " << old_grad_.isValid() << endl;
			Log.error() << "ConjugateGradient::updateDirection: invalid gradient or direction  -  cannot use CG." << endl;
			
			// Calculate the current gradient and determine the current
			// direction as the direction of steepest descent.
			updateForces();
			direction_ = current_grad_;
			direction_.negate();
			unscaled_direction_ = direction_;
			direction_.normalize();
			last_restart_iter_ = 0;
			return;
		}
		
		/* There are three different conjugate gradient methods implemented:
			
				- Fletcher-Reeves (FR)
				- Polak-Ribiere (PR)
				- Shanno
				
			 For FR and PR methods, the new direction vector is calculated as follows:
				
					d_k = - g_k + \beta d_{k-1}
				
			 Where d_k and d_{k-1} are the new and the old search direction 
			 and g_k is the current gradient. \beta is determined by one of 
			 methods stated above.
		*/
		switch (updt_method_)
		{
			case FLETCHER_REEVES:
			{
				// We compute the new direction by the Fletcher-Reeves update.
				
				// \beta = \frac{<g_{k}, g_{k}>}{<g_{k-1}, g_{k-1}>}, see [1].
				
				double gtg = 0.;
				
				// In order to use 'double' accuracy the calculation is done
				// 'by hand'.
				for(Size i = 0; i < number_of_atoms_; ++i)
				{
					gtg += (double)initial_grad_[i].x*(double)initial_grad_[i].x;
					gtg += (double)initial_grad_[i].y*(double)initial_grad_[i].y;
					gtg += (double)initial_grad_[i].z*(double)initial_grad_[i].z;
				}
				
				// Check whether we should proceed by a restart
				if ((last_restart_iter_ == restart_frequency_) || (same_energy_counter_ > 0))
				{
					direction_ = initial_grad_;
					direction_.negate();
					unscaled_direction_ = direction_;
					direction_.normalize();
					old_gtg_ = gtg;
					last_restart_iter_ = 0;
					return;
				}
				
				double beta = gtg;
				if (old_gtg_ > cutlo_)
				{
					beta    /= old_gtg_;
					old_gtg_ = gtg;
				}
				else
				{
					// Something went wrong. The recent gradient is about 0 or
					// is something wrong with the stored value???
					// We force a restart.
					direction_ = initial_grad_;
					direction_.negate();
					unscaled_direction_ = direction_;
					direction_.normalize();
					old_gtg_ = gtg;
					last_restart_iter_ = 0;
					return;
				}
				
				// Calculate the new conjugate gradient search direction:
				// direction_ = - initial_gradient_ + beta * direction_;
				// and a simple angle test for restart, see [5]
				direction_.norm = 0.0;
				double stg = 0.;
				for (Size i = 0; i < number_of_atoms_; ++i)
				{
					// Same case as above, use 'double' accuracy
					direction_[i].x  = -initial_grad_[i].x + beta*unscaled_direction_[i].x;
					direction_[i].y  = -initial_grad_[i].y + beta*unscaled_direction_[i].y;
					direction_[i].z  = -initial_grad_[i].z + beta*unscaled_direction_[i].z;
					
					// We don't need the following values computed in 'double' accuracy
					direction_.norm += direction_[i] * direction_[i];
					stg             += direction_[i]*initial_grad_[i];
				}
				direction_.norm = sqrt(direction_.norm);
				if (-stg/direction_.norm/initial_grad_.norm < 1.e-3)
				{
					// Case: beta = 0, i.e. a restart
					direction_ = initial_grad_;
					direction_.negate();
					unscaled_direction_ = direction_;
					direction_.normalize();
					last_restart_iter_ = 0;
					return;
				}
				else
				{
					// Assign the norm of the new direction
					direction_.rms = direction_.norm / (3.0 * (double)number_of_atoms_);
				
					// Don't risc a "NaN"
					if (direction_.norm >= cutlo_)
					{
						direction_.inv_norm = 1.0 / direction_.norm;
					}
					else
					{
						direction_.inv_norm = sqrt(std::numeric_limits<float>::max());
					}
					unscaled_direction_ = direction_;
				}
				break;
			}
			case POLAK_RIBIERE:
			{
				// We compute the new direction by the Polak-Ribiere update.
				
				// \beta = \frac{<g_k, (g_k - g_{k-1})>}{<g_{k-1}, g_{k-1}>}
				double gtg = 0.;
				double gg  = 0.;
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					// We want 'double' accuracy
					gtg += (double)initial_grad_[i].x*(double)initial_grad_[i].x;
					gg  += (double)initial_grad_[i].x*(double)old_grad_[i].x;
					gtg += (double)initial_grad_[i].y*(double)initial_grad_[i].y;
					gg  += (double)initial_grad_[i].y*(double)old_grad_[i].y;
					gtg += (double)initial_grad_[i].z*(double)initial_grad_[i].z;
					gg  += (double)initial_grad_[i].z*(double)old_grad_[i].z;
				}
				
				// Check whether we should proceed by a restart.
				// We also force a restart if the energy difference is too small
				if ((last_restart_iter_ == restart_frequency_) || (same_energy_counter_ > 0))
				{
					direction_ = initial_grad_;
					direction_.negate();
					unscaled_direction_ = direction_;
					direction_.normalize();
					old_gtg_ = gtg;
					last_restart_iter_ = 0;
					return;
				}
				
				double beta = gtg - gg;
				if (old_gtg_ > cutlo_)
				{
					beta    /= old_gtg_;
					old_gtg_ = gtg;
				}
				else
				{
					// Something went wrong. The recent gradient is about 0 or
					// is something wrong with the stored value???
					// We force a restart.
					direction_ = initial_grad_;
					direction_.negate();
					unscaled_direction_ = direction_;
					direction_.normalize();
					old_gtg_ = gtg;
					last_restart_iter_ = 0;
					return;
				}
				
				if (beta < 0.)
				{
					// This is the proposed criterion in [4]
					// beta = 0.;
					direction_ = initial_grad_;
					direction_.negate();
					unscaled_direction_ = direction_;
					direction_.normalize();
					last_restart_iter_ = 0;
				}
				else
				{
					// Calculate the new conjugate gradient search direction:
					// direction_ = - initial_gradient_ + beta * direction_;
					// and a smiple angle test for restart
					direction_.norm = 0.0;
					float stg = 0.;
					for (Size i = 0; i < number_of_atoms_; i++)
					{
						// Same case as above, use 'double' accuracy
						direction_[i].x  = -initial_grad_[i].x + beta*unscaled_direction_[i].x;
						direction_[i].y  = -initial_grad_[i].y + beta*unscaled_direction_[i].y;
						direction_[i].z  = -initial_grad_[i].z + beta*unscaled_direction_[i].z;
						
						// We don't need the following values computed in 'double' accuracy
						direction_.norm += direction_[i] * direction_[i];
						stg             += direction_[i]*initial_grad_[i];
					}
					// Assign the norm of the new direction
					direction_.norm = sqrt(direction_.norm);
					if (-stg/direction_.norm/initial_grad_.norm < 1.e-3)
					{
						// beta = 0.;
						direction_ = initial_grad_;
						direction_.negate();
						unscaled_direction_ = direction_;
						direction_.normalize();
						last_restart_iter_ = 0;
					}
					else
					{
						direction_.rms = direction_.norm / (3.0 * (double)number_of_atoms_);
						
						// Don't risc a "NaN"
						if (direction_.norm >= cutlo_)
						{
							direction_.inv_norm = 1.0 / direction_.norm;
						}
						else
						{
							direction_.inv_norm = sqrt(std::numeric_limits<float>::max());
						}
						unscaled_direction_ = direction_;
					}
				}
				break;
			}
			case SHANNO:
			default:
			{
				/* Literature: see [2], [3]
					 The new search direction d_k is calculated as follows:
					   d_i+1 = - b_i + D_8 / D_5 * a_i - (1 + D_6/D_5) * D_8/D_5 - D_7/D_5) * p_i
					 with
					 Index i = old iteration 
					 a_i = D_1 / D_4 * y_i - D_2 / D_4 * y_t + (2 D_2/D_1 - D_3/D_4) * p_t
					 b_i = D_1 / D_4 * g_i - C_1/D_4 * y_t + (2 C_1/D_1 - C_2/D_4) * p_t
					 p_i = step_length_ * d_old;
					 p_t = step_length_t * d_t
					 D_1 = p_t * y_t         D_2 = p_t * y_i
					 D_3 = y_t * y_i         C_1 = p_t * grad_new
					 C_2 = y_t * grad_new    D_4 = y_t * y_t
					 D_5 = p_i * y_i         D_6 = a_i * y_i 
					 D_7 = a_i * grad_new    D_8 = p_i * grad_new 
					 y_i = grad_new - grad_old 
					 y_t = grad_new - grad_old  in iteration t
					 d_t = search direction in iteration t
				*/
				double sum1 = 0;
				double sum2 = 0;
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					y_i_[i] = initial_grad_[i] - old_grad_[i];
					p_i_[i] = direction_[i] * step_;
					
					sum1 += y_i_[i] * y_i_[i];
					sum2 += p_i_[i] * p_i_[i];
				}
				
				if (sum1 < cutlo_ || sum2 < cutlo_)
				{
					// Take the current gradient as the new search direction
					direction_ = initial_grad_;
					direction_.negate();
					// Not necessary in Shanno case
					//unscaled_direction_ = direction_;
					direction_.normalize();
					last_restart_iter_ = 0;
					return;
				}
				
				if ((last_restart_iter_ == restart_frequency_) || (number_of_iterations_ == 1))
				{
					double condition = 0;
					for (Size i = 0; i < number_of_atoms_; i++)
					{
						condition += initial_grad_[i] * old_grad_[i];
					}
					
					// Take the absolute value
					condition = fabs(condition);
					
					if ((number_of_iterations_ == 1) || (condition >= 0.2 * initial_grad_.norm * initial_grad_.norm))
					{
						D_1_ = 0.;
						D_4_ = 0.;
						for (Size i = 0; i < number_of_atoms_; i++)
						{
							p_t_[i] = direction_[i]*step_;
							y_t_[i] = initial_grad_[i] - old_grad_[i];
							D_1_ += p_t_[i]*y_t_[i];
							D_4_ += y_t_[i]*y_t_[i];
						}
						
						if (fabs(D_4_) <= cutlo_)
						{
							throw Exception::DivisionByZero(__FILE__, __LINE__);
						}
					}
					last_restart_iter_ = 0;
				}
				
				// Calculate all the auxiliary values
				
				double C_1 = 0.;
				double C_2 = 0.;
				double D_2 = 0.;
				double D_3 = 0.;
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					C_1 += p_t_[i] * initial_grad_[i];
					C_2 += y_t_[i] * initial_grad_[i];
					D_2 += p_t_[i] * y_i_[i];
					D_3 += y_t_[i] * y_i_[i];
				}
				
				double factor1 = D_1_ / D_4_;
				double factor2 = D_2 / D_4_;
				double factor3 = (2. * D_2 / D_1_ - D_3 / D_4_);
				
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					a_i_[i] = y_i_[i] * factor1 - y_t_[i] * factor2 + p_t_[i] * factor3;
				}
				
				factor2 = C_1 / D_4_;
				factor3 = (2. * C_1 / D_1_ - C_2 / D_4_);
				
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					b_i_[i] = initial_grad_[i] * factor1 - y_t_[i] * factor2 + p_t_[i] * factor3;
				}
				
				double D_5 = 0.;
				double D_6 = 0.;
				double D_7 = 0.;
				double D_8 = 0.;
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					D_5 += p_i_[i] * y_i_[i];
					D_6 += a_i_[i] * y_i_[i];
					D_7 += a_i_[i] * initial_grad_[i];
					D_8 += p_i_[i] * initial_grad_[i];
				}
				
				if (fabs(D_5) <= cutlo_)
				{
					// Take the current gradient as the new search direction
					direction_ = initial_grad_;
					direction_.negate();
					// Not necessary in Shanno case
					//unscaled_direction_ = direction_;
					direction_.normalize();
					last_restart_iter_ = 0;
					return;
				}
				
				// Set the new search direction
				direction_.norm = 0.0;
				
				factor1 = D_8 / D_5;
				factor2 = (1. + D_6 / D_5) * factor1 - D_7 / D_5;
				
				for (Size i = 0; i < number_of_atoms_; i++)
				{
					direction_[i] = -b_i_[i] + a_i_[i] * factor1 - p_i_[i] * factor2;
					direction_.norm += direction_[i] * direction_[i];
				}
				
				// Assign the norm of the new direction
				direction_.norm = sqrt(direction_.norm);
				direction_.rms = direction_.norm / (3.0 * (double)number_of_atoms_);
				
				// Don't risc a "NaN"
				if (direction_.norm >= cutlo_)
				{
					direction_.inv_norm = 1.0 / direction_.norm;
				}
				else
				{
					direction_.inv_norm = sqrt(std::numeric_limits<float>::max());
				}
				// No necessary in Shanno case
				//unscaled_direction_ = direction_;
			}
		}
	} // end of method 'updateDirection'

	// The minimizer optimizes the energy of the system 
	// by using a conjugate gradient method. 
	// Return value is true when no further steps can be taken!
	bool ConjugateGradientMinimizer::minimize(Size iterations, bool resume)
	{
		aborted_ = false;
		
		// Check for validity of minimizer and force field
		if (!isValid() || getForceField() == 0 || !getForceField()->isValid())
		{
			Log.error() << "ConjugateGradientMinimizer: is not initialized correctly!" << std::endl;
			aborted_ = true;
			return false;
		}
		
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
		if ((Size)options.getInteger(Option::UPDATE_METHOD) != updt_method_)
		{
			updt_method_ = (Size)options.getInteger(Option::UPDATE_METHOD);
			resume = false;
		}
		
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		// If we start from scratch (i.e. no restart) we have to make sure
		// to calculate all the quantities we need before we start
		if (!resume)
		{
			// Reset the number of iterations if the job is not resumed.
			setNumberOfIterations(0);
			same_energy_counter_ = 0;
			initial_grad_.invalidate();
			current_grad_.invalidate();
			first_iter_ = true;
			last_restart_iter_ = 0;
			
			restart_frequency_ = 3 * number_of_atoms_;
			if (updt_method_ == SHANNO)
			{
				p_t_.resize(number_of_atoms_);
				y_t_.resize(number_of_atoms_);
				y_i_.resize(number_of_atoms_);
				p_i_.resize(number_of_atoms_);
				a_i_.resize(number_of_atoms_);
				b_i_.resize(number_of_atoms_);
			}
			
			// Obviously, we don't have "old" energies yet, so we initialize it a with 
			// sensible value. We don't need "old" gradients here.
			old_energy_ = std::numeric_limits<float>::max();
		}
		
		Size max_iterations = std::min(getNumberOfIterations() + iterations, getMaxNumberOfIterations());
		
		#ifdef BALL_DEBUG
			Log.info() << "CGM: minimize(" << iterations << ", " << resume << ")" << endl;
		#endif
		
		// save the current atom positions
		atoms.savePositions();
		bool converged = false;	
		
		// Iterate: while not converged and not enough iterations
		while (!converged && (getNumberOfIterations() < max_iterations))	
		{
			// Try to take a new step
			double stp = findStep();
			
			// use this step as new reference step if findStep was successful
			if (stp > 0.)
			{
				atoms.savePositions();
			}
			
			// Store the gradient and the energy
			old_grad_   = initial_grad_;
			old_energy_ = initial_energy_;
			
			// Store the current gradient and energy
			storeGradientEnergy();
			
			#ifdef BALL_DEBUG
				Log << "CGM: end of main: current grad RMS = " << current_grad_.rms << std::endl;
			#endif
			
			// Check for convergence.
			converged = isConverged() || (stp == 0.);
			
			// Increment iteration counter, take snapshots, print energy,
			// update pair lists, and check the same-energy counter
			finishIteration();
			++last_restart_iter_;
			
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
	
	
	double ConjugateGradientMinimizer::findStep()
	{
		// Compute the new direction
		updateDirection();
		
		// Define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		// We perform a line search
		
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
			if (updt_method_ != SHANNO)
			{
				unscaled_direction_ = direction_;
			}
			direction_.normalize();
			last_restart_iter_ = 0;
			
			Size iter = 0;
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
