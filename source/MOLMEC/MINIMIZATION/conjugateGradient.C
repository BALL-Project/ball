// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: conjugateGradient.C,v 1.23 2003/03/21 14:37:30 anhi Exp $
//
// Minimize the potential energy of a system using a nonlinear conjugate 
// gradient method with  line search

#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/COMMON/limits.h>

//#define BALL_DEBUG
#undef BALL_DEBUG

// The default method to use for the CG direction update
// (FLETCHER_REEVES | POLAK_RIBIERE | SHANNO)
#define DEFAULT_METHOD SHANNO

namespace BALL 
{
  using namespace std; 

  // Set the default values for options of this class 
  // The control factors needed for the line search along a new search direction
  const char* ConjugateGradientMinimizer::Option::STEP_LENGTH = "step_length"; 

  double ConjugateGradientMinimizer::Default::STEP_LENGTH = 1.0; 

  // Default constructor 
  // It does nothing but calling its base class constructor 
  ConjugateGradientMinimizer::ConjugateGradientMinimizer()
		: EnergyMinimizer(),
			step_(1.0),
			lambda_(0.0),
			number_of_atoms_(0),
			method_(DEFAULT_METHOD),
			first_call_(true),
			restart_frequency_(1)
	{
	}

	// Constructor initialized with a force field
	ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field)
		: EnergyMinimizer(),
			step_(1.0),
			lambda_(0.0),
			number_of_atoms_(0),
			method_(DEFAULT_METHOD),
			first_call_(true),
			restart_frequency_(1)
	{
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
			step_(1.0),
			lambda_(0.0),
			number_of_atoms_(0),
			method_(DEFAULT_METHOD),
			first_call_(true),
			restart_frequency_(1)
	{
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
			step_(1.0),
			lambda_(0.0),
			number_of_atoms_(0),
			method_(DEFAULT_METHOD),
			first_call_(true),
			restart_frequency_(1)
	{
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
			step_(1.0),
			lambda_(0.0),
			number_of_atoms_(0),
			method_(DEFAULT_METHOD),
			first_call_(true),
			restart_frequency_(1)
	{
		// The actual work is done in setup 
		valid_ = setup(force_field, ssm, new_options);

		if (!valid_)
		{
			Log.error() << " Setup of conjugate gradient minimizer has failed! " << endl; 
		}
	}


  // The destructor
  ConjugateGradientMinimizer::~ConjugateGradientMinimizer()
  {
  }


  // The copy constructor 
  ConjugateGradientMinimizer::ConjugateGradientMinimizer 
		(const ConjugateGradientMinimizer& rhs)
		: EnergyMinimizer(rhs),
			step_(rhs.step_),
			lambda_(rhs.lambda_),
			number_of_atoms_(rhs.number_of_atoms_),
			method_(rhs.method_),
			first_call_(rhs.first_call_),
			a_i_(rhs.a_i_),
			b_i_(rhs.b_i_),
			p_t_(rhs.p_t_),
			y_t_(rhs.y_t_),
			p_i_(rhs.p_i_),
			y_i_(rhs.y_i_),
			D_1_(rhs.D_1_),
			D_4_(rhs.D_4_),
			restart_frequency_(rhs.restart_frequency_)
	{
	}

  // The assignment operator
  const ConjugateGradientMinimizer& ConjugateGradientMinimizer::operator =
		(const ConjugateGradientMinimizer& rhs)
	{
    EnergyMinimizer::operator = (rhs);
		method_ = rhs.method_;
		step_ = rhs.step_;
		lambda_ = rhs.lambda_;
		number_of_atoms_ = rhs.number_of_atoms_;

   	first_call_ = rhs.first_call_;
		a_i_ = rhs.a_i_;
		b_i_ = rhs.b_i_;
		p_t_ = rhs.p_t_;
		y_t_ = rhs.y_t_;
		p_i_ = rhs.p_i_;
		y_i_ = rhs.y_i_;
		D_1_ = rhs.D_1_;
		D_4_ = rhs.D_4_;
		restart_frequency_ = rhs.restart_frequency_;
 	
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

    // set the options  to their default values if not already set  
    step_ = options.setDefaultReal
			(Option::STEP_LENGTH, Default::STEP_LENGTH); 

    // determine the number of atoms
    number_of_atoms_ = (Size)force_field_->getAtoms().size(); 

    return true;
	}

  // Set explicitly the option step_
  void ConjugateGradientMinimizer::setStepLength(double length)
	{
    step_ = length;
	}

  // get  the value of option step_ 
  double ConjugateGradientMinimizer::getStepLength() const 
	{
    return step_; 
	}

  // This method determines the new search direction. Along this
  // vector we will try to find the next solution 
  // If called with a true value, the current negative gradient is taken as the search direction 
  // Literature: - David Shanno "Conjugate Gradient Methods With Inexact Searches"
  //               Mathematics of Operations Research, Vol. 3, No. 3, August 1978, pp. 244-256
  //             - S. Watowich et. al "A Stable Rapidly Converging Conjugate Gradient Method
  //                                   for Energy Minimization"
  //               Journal of Computational Chemistry, Vol. 9, No. 6, pp. 650-661 (1988)
	void ConjugateGradientMinimizer::updateDirection()
	{
		// if the current or the last gradient are undefined or the 
		// direction is not yet set, we set it to the negative gradient
		if ((direction_.size() == 0) || !old_grad_.isValid() || !initial_grad_.isValid())
		{
			Log.error() << "dir: " << direction_.isValid() 
									<< "  initial_grad: " << initial_grad_.isValid() 
									<< "  old_grad: " << old_grad_.isValid() << endl;
			Log.error() << "ConjugateGradient::determineNewSearchDirection_: invalid gradient or direction  -  cannot use Fletcher-Reeves." << endl;

			// Calculate the current gradient and determine the current
			// direction as the direction of steepest descent.
			updateForces();
			direction_ = current_grad_;
			direction_.negate();

			return;
		}

		// look for a restart
		if (!direction_.isValid())
		{
			first_call_ = true; // needed for the SHANNO - criterion
			updateForces();
			direction_ = current_grad_;
			direction_.negate();

			return;
		}

		// There are three different conjugate gradient methods
		// implemented:
		//
		//   - Fletcher-Reeves (FR)
		//	 - Polak-Ribiere (PR)
		//   - Shanno
		// 
		// For FR and PR methods, the new direction vector is calculated as follows:
		// 
		//     d_k = - g_k + \beta d_{k-1}
		//      
		// Where d_k and d_{k-1} are the new and the old search direction 
		// and g_k is the current gradient. \beta is determined by one of 
		// methods stated above.


		float beta = 0.0;
		Size i;
		switch (method_)
		{
			case	FLETCHER_REEVES:
				//
				//  Literature:
				//		Fletcher, R. and Reeves, C.M.: Function Minimization by 
				//       Conjugate Gradients, Comp. J., 7, 149-154, 1964
				//
				//     \beta = \frac{<g_{k}, g_{k}>}{<g_{k-1}, g_{k-1}>}
				//  
				beta = initial_grad_.norm * initial_grad_.norm / (old_grad_.norm * old_grad_.norm);

				// calculate the new conjugate gradient search direction:
				// direction_ = - initial_gradient_ + beta * direction_;
				// 
				// we do it a bit more efficient:
				direction_.norm = 0.0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					direction_[i] = - initial_grad_[i] + beta * direction_[i];
					direction_.norm += direction_[i] * direction_[i];
				}

				// assign the norm of the new direction
				direction_.norm = sqrt(direction_.norm);
				direction_.rms = direction_.rms / (3.0 * (double)number_of_atoms_);
				direction_.inv_norm = 1.0 / direction_.norm;
				break;
			
			case POLAK_RIBIERE:
				//
				//     \beta = \frac{<g_k, (g_k - g_{k-1})>}{<g_{k-1}, g_{k-1}>}
				//  
				for (i = 0; i < number_of_atoms_; i++)
				{
					beta += initial_grad_[i] * (initial_grad_[i] - old_grad_[i]);
				}
				beta *= old_grad_.inv_norm * old_grad_.inv_norm;

				// calculate the new conjugate gradient search direction:
				// direction_ = - initial_gradient_ + beta * direction_;
				// 
				// we do it a bit more efficient:
				direction_.norm = 0.0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					direction_[i] = - initial_grad_[i] + beta * direction_[i];
					direction_.norm += direction_[i] * direction_[i];
				}

				// assign the norm of the new direction
				direction_.norm = sqrt(direction_.norm);
				direction_.rms = direction_.norm / (3.0 * (double)number_of_atoms_);
				direction_.inv_norm = 1.0 / direction_.norm;
				break;

			case SHANNO:
			default:
				// Literature: 
				//
				//  - David Shanno "Conjugate Gradient Methods With Inexact Searches"
				//			Mathematics of Operations Research, Vol. 3, No. 3, August 1978, pp. 244-256
				// 
				//  - S. Watowich et. al "A Stable Rapidly Converging Conjugate Gradient Method
				//			for Energy Minimization"
				//      Journal of Computational Chemistry, Vol. 9, No. 6, pp. 650-661 (1988)
				//
				
				// The new search direction d_k is calculated as follows:
				// d_i+1 = - b_i + D_8 / D_5 * a_i - (1 + D_6/D_5) * D_8/D_5 - D_7/D_5) * p_i            
				// with
				// Index i = old iteration 
				// a_i = D_1 / D_4 * y_i - D_2 / D_4 * y_t + (2 D_2/D_1 - D_3/D_4) * p_t
				// b_i = D_1 / D_4 * g_i - C_1/D_4 * y_t + (2 C_1/D_1 - C_2/D_4) * p_t
				// p_i = step_length_ * d_old;
				// p_t = step_length_t * d_t
				// D_1 = p_t * y_t         D_2 = p_t * y_i
				// D_3 = y_t * y_i         C_1 = p_t * grad_new
				// C_2 = y_t * grad_new    D_4 = y_t * y_t
				// D_5 = p_i * y_i         D_6 = a_i * y_i 
				// D_7 = a_i * grad_new    D_8 = p_i * grad_new 
				// y_i = grad_new - grad_old 
				// y_t = grad_new - grad_old  in iteration t
				// d_t = search direction in iteration t

				if (first_call_ == true)
				{
					restart_frequency_ = 3 * number_of_atoms_;

					p_t_.resize (number_of_atoms_);
					y_t_.resize (number_of_atoms_);
					y_i_.resize (number_of_atoms_);
					p_i_.resize (number_of_atoms_);
					a_i_.resize (number_of_atoms_);
					b_i_.resize (number_of_atoms_);

					first_call_ = false;

					D_1_ = 0;
					D_4_ = 0;

					// Do some initialisations 
					// Note: when this method is called for the first time, the 
					// minimisation algorithm must have already done one iteration! 

					for (i = 0; i < number_of_atoms_; i++)
					{
						p_t_[i] = direction_[i] * lambda_ * step_;
						y_t_[i] = initial_grad_[i] - old_grad_[i];

						D_1_ += p_t_[i] * y_t_[i];
						D_4_ += y_t_[i] * y_t_[i];
					}

					if (D_4_ == 0)
					{
						throw Exception::DivisionByZero(__FILE__, __LINE__);
					}
				}

				double sum1 = 0;
				double sum2 = 0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					y_i_[i] = initial_grad_[i] - old_grad_[i];
					p_i_[i] = direction_[i] * step_ * lambda_;

					sum1 += y_i_[i] * y_i_[i];
					sum2 += p_i_[i] * p_i_[i];
				}

				if (sum1 == 0 || sum2 == 0)
				{
					// take the current gradient as the new search direction
					direction_ = initial_grad_;
					direction_.negate();

					return;
				}


				if ((number_of_iterations_ % restart_frequency_ == 0))
				{
					double condition = 0;

					for (i = 0; i < number_of_atoms_; i++)
					{
						condition += initial_grad_[i] * old_grad_[i];
					}

					// take the absolute value
					condition = (condition < 0) ? -condition : condition;

					if (condition >= 0.2 * initial_grad_.norm * initial_grad_.norm)
					{
						D_1_ = 0;
						D_4_ = 0;

						for (i = 0; i < number_of_atoms_; i++)
						{
							p_t_[i] = direction_[i] * lambda_ * step_;
							y_t_[i] = initial_grad_[i] - old_grad_[i];

							D_1_ += p_t_[i] * y_t_[i];
							D_4_ += y_t_[i] * y_t_[i];
						}

						if (D_4_ == 0)
						{
							throw Exception::DivisionByZero(__FILE__, __LINE__);
						}
					}
				}


				// calculate all the auxiliary values

				double C_1 = 0.0;
				double C_2 = 0.0;
				double D_2 = 0.0;
				double D_3 = 0.0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					C_1 += p_t_[i] * initial_grad_[i];
					C_2 += y_t_[i] * initial_grad_[i];
					D_2 += p_t_[i] * y_i_[i];
					D_3 += y_t_[i] * y_i_[i];
				}

				double factor1 = D_1_ / D_4_;
				double factor2 = D_2 / D_4_;
				double factor3 = (2 * D_2 / D_1_ - D_3 / D_4_);

				for (i = 0; i < number_of_atoms_; i++)
				{
					a_i_[i] = y_i_[i] * factor1 - y_t_[i] * factor2 + p_t_[i] * factor3;
				}

				factor2 = C_1 / D_4_;
				factor3 = (2 * C_1 / D_1_ - C_2 / D_4_);

				for (i = 0; i < number_of_atoms_; i++)
				{
					b_i_[i] = initial_grad_[i] * factor1 - y_t_[i] * factor2 + p_t_[i] * factor3;
				}

				double D_5 = 0;
				double D_6 = 0;
				double D_7 = 0;
				double D_8 = 0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					D_5 += p_i_[i] * y_i_[i];
					D_6 += a_i_[i] * y_i_[i];
					D_7 += a_i_[i] * initial_grad_[i];
					D_8 += p_i_[i] * initial_grad_[i];
				}

				if (D_5 == 0)
				{
					// take the current gradient as the new search direction
					direction_ = initial_grad_;
					direction_.negate();

					return;
				}

				// set the new search direction
				direction_.norm = 0.0;

				factor1 = D_8 / D_5;
				factor2 = (1 + D_6 / D_5) * factor1 - D_7 / D_5;

				for (i = 0; i < number_of_atoms_; i++)
				{
					direction_[i] = -b_i_[i] + a_i_[i] * factor1 - p_i_[i] * factor2;
					direction_.norm += direction_[i] * direction_[i];
				}

				// Calculate the inverse of the search direction length 
				// This is needed later on for the right scaling 
				if (direction_.norm != 0)
				{
					direction_.norm = sqrt(direction_.norm);
					direction_.inv_norm = 1.0 / direction_.norm;
					direction_.rms = direction_.norm / sqrt(3.0 * (double)direction_.size());
				}
				else
				{
					direction_ = initial_grad_;
					direction_.negate();
				}
		}
	} // end of method 'updateDirection'

  // The minimizer optimizes the energy of the system 
  // by using a conjugate gradient method. 
  // Return value is true when no further steps can be taken!
  bool ConjugateGradientMinimizer::minimize
		(Size iterations, bool resume)
  {
		// Check for validity of minimizer and force field
		if (!isValid() || getForceField() == 0 || !getForceField()->isValid())
		{
			Log.error() << "ConjugateGradientMinimizer: minimizer is not initialized correctly!" << std::endl;
			return false;
		}

		// Make sure we have something worth moving.
		if (getForceField()->getNumberOfMovableAtoms() == 0)
		{
			return true;
		}

		// remember <g_i, d_i> (the scalar product of the last 
		// search direction and the last gradient)
		// keep them static for restarts
		static double old_dir_grad;
		static bool old_dir_grad_valid;
		
		// define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));

		// If we start from scratch (i.e. no restart) we have to make sure
		// to calculate all the quantities we need before we start
		if (!resume)
		{
			// Reset the number of iterations if the job is not resumed.
			setNumberOfIterations(0);
			same_energy_counter_ = 0;

			current_grad_.invalidate();
			// Compute an initial direction (along the negative gradient).
			old_dir_grad_valid = false;	

			// If we do not have a valid gradient, recalculate the gradient, the energy, and the search 
			// direction
			if (!old_grad_.isValid() || old_grad_.size() == 0 || !initial_grad_.isValid())
			{
				#ifdef BALL_DEBUG
					Log.info() << "CGM: calculating initial gradient...";
				#endif

				// Compute the initial energy and the initial forces
				updateEnergy();
				updateForces();

				// store them for later use
				storeGradientEnergy();

				// obviously, we don't have "old" gradients and energies yet,
				// so we initialize them with sensible values
				old_grad_ = current_grad_;
				old_energy_ = Limits<float>::max();

				// the first direction we try is just the negative gradient
				direction_ = current_grad_;
				direction_.negate();

				#ifdef BALL_DEBUG
					Log.info() << " [" << current_grad_.size() << "/" << current_grad_.norm << "/" << current_grad_.rms << " / " << initial_energy_ << "]" << endl;
				#endif
			}	
		}

		Size max_iterations = std::max(getNumberOfIterations() + iterations, getMaxNumberOfIterations());

		#ifdef BALL_DEBUG
			Log.info() << "CGM: minimize(" << iterations << ", " << resume << ")" << endl;
		#endif
		
		// save the current atom positions
		atoms.savePositions();
		bool converged = false;	
		
		// iterate: while not converged and not enough iterations
		while (!converged && (getNumberOfIterations() < max_iterations))	
		{
			// Try to take a new step along the direction
			double lambda = findStep();
			
			// We were successul, if we found a lambda > 0.
			bool result = (lambda > 0.0);

			// use this step as new reference step if findStep was successful
			if (result)
			{
				atoms.savePositions();
			}
			else
			{
				// No success.
				// Mark the direction as bad.
				direction_.invalidate();

				// let's see if we have to recompute the forces and the energy
				if (!current_grad_.isValid())
				{	
					#ifdef BALL_DEBUG
						Log << "Computing new energy/forces!" << std::endl;
					#endif
					// ...calculate energy and forces for the new position
					updateForces();
					updateEnergy();
				}
			}
			
			// store the gradient and the energy
			old_grad_ = initial_grad_;
			old_energy_ = initial_energy_;
			
			// store the current gradient and energy
			storeGradientEnergy();

			// Calculate a new search direction if the search direction is invalid
			// 
			// If someone has already created a valid direction
			// (e.g. by using the negative gradient as is done by findStep)
			// it is flagged as valid (since it is new!)
			//
//			if (!direction_.isValid())
			{
				updateDirection();
			}

			// non-constant step size option
			//
			double dir_grad = initial_grad_ * direction_;
			if (old_dir_grad_valid)
			{
/*				step_ = lambda_ * step_ * old_dir_grad / dir_grad;

				// we have to capture those cases where step_ would be zero
				if (step_ == 0.0)
				{
					step_ = 0.01;
				}
	*/
				}
			else
			{
				// for restart steps, we reset the step size 
				// to 0.01/(||d_i||) ??? what and where is ||d_i|| ???
				step_ = 0.01;
			}
			old_dir_grad = dir_grad;
			old_dir_grad_valid = true;

			#ifdef BALL_DEBUG
				Log << "CGM: end of main: current grad RMS = " << current_grad_.rms << std::endl;
			#endif

			// Check for convergence.
			converged = isConverged();
			
			// increment iteration counter, take snapshots, print energy,
			// update pair lists, and check the same-energy counter
			finishIteration();
		}	

		// check for convergence
		if (converged)
		{
			Log.info() << "convergence reached" << endl;
		}

		return converged;
  } // end of method 'minimize' 
			
	double ConjugateGradientMinimizer::findStep()
	{		
		// define an alias for the atom vector
		AtomVector& atoms(const_cast<AtomVector&>(getForceField()->getAtoms()));
		
		#ifdef BALL_DEBUG
			Log.info() << "CG: " << step_ << " " << lambda_ << " " << initial_energy_ << " " << current_grad_.norm << " " << direction_.norm << endl;
		#endif
		
		bool success = true;
		bool step_too_large = false;

		if (lambda_ > 0.0)
		{
			step_ *= lambda_;
		}

		// In order to find a sensible starting step, we take a full step along direction_,
		// and if the energy decreases after that step, we double the step width. We iterate
		// this as long as the energy increases and our step width stays below maximum_displacement
		while (success && !step_too_large)
		{
			// We have to catch those cases where at least one atom might be
			// translated more than getMaximumDisplacement().
			if (step_<=getMaximumDisplacement())
			{
				atoms.moveTo(direction_, step_*direction_.inv_norm);
			}
			else
			{
				#ifdef BALL_DEBUG
					Log.info() << "CG: reached maximum step size... Adjusting..." << endl;
				#endif

				// find the maximal translation
				Gradient::ConstIterator grad_it(direction_.begin());
				double max = 0;
				double cur = 0;
				for (; grad_it != direction_.end(); ++grad_it)
				{
					cur = grad_it->getSquareLength();
					if (cur >= max)
					{
						max = cur;
					}
				}
				
				if (sqrt(max)*step_*direction_.inv_norm >= getMaximumDisplacement())
				{
					step_ = getMaximumDisplacement() / (direction_.inv_norm * sqrt(max));
				}

				#ifdef BALL_DEBUG
					Log.info() << "CG: new step = " << step_ << " " << max << " " << step_*direction_.inv_norm << endl;
				#endif

				atoms.moveTo(direction_, step_*direction_.inv_norm);
				step_too_large = true;
			}

			success = (updateEnergy() < initial_energy_);
			if (success)
			{
				// check whether the gradient decreases along direction
				updateForces();
				double dir_grad = direction_ * current_grad_;
				success &= (dir_grad < 0.0);
			}
			
			if (success && !step_too_large)
			{
				step_ *= 2.0;
			}
		}
		
		#ifdef BALL_DEBUG
			Log.info() << "CG: new step " << step_ << endl; 	
		#endif

		// now we perform a line search along direction_
		LineSearch line_search(*this);

		// ?? TEST if we need this ??
		// Make sure that the line search resets the atom positions, energies and forces
		initial_grad_.invalidate();
	
		bool result =  line_search.minimize(lambda_, step_*direction_.inv_norm);
		#ifdef BALL_DEBUG
			Log.info() << "LineSearch: lambda = " << lambda_ << " result = " << result << endl;
		#endif

		// This line search might fail...
		if (!result)
		{
			// reset the search direction to the 
			// negative gradient
			direction_ = initial_grad_;
			direction_.negate();
			#ifdef BALL_DEBUG
				Log.info() << direction_.rms << "]" << endl;
			#endif

			// invalidate the current gradient (LineSearch::minimize())
			// recalculate it for lambda = 1.0;
			current_grad_.invalidate();

			// ...and try another line search
			result = line_search.minimize(lambda_, step_*direction_.inv_norm);
			#ifdef BALL_DEBUG
				Log.info() << "LineSearch: lambda = " << lambda_ << " result = " << result << endl;
			#endif
		}

		if (!result)
		{
			#ifdef BALL_DEBUG
				Log.info() << "resetting step length" << endl;
			#endif
			step_ = 0.01;
			
			// invalidate the current gradient (LineSearch::minimize())
			// recalculate it for lambda = 1.0;
			current_grad_.invalidate();

			// ...and try another line search
			result = line_search.minimize(lambda_, step_*direction_.inv_norm);
			#ifdef BALL_DEBUG
				Log.info() << "LineSearch: lambda = " << lambda_ << " result = " << result << endl;
			#endif
		}			

		updateStepSize(result);
		//Log.info() << "CG fS end: step, lambda " << step_ << " " << lambda_ << std::endl;
		
		if (result == true)
		{
			return lambda_;
		} 
		else 
		{
			return -1.0;
		}
	} // end of method 'findStep'

	void ConjugateGradientMinimizer::updateStepSize(bool result)
	{
		if (result)
		{
			if (lambda_ < 0.1)
			{
				if (lambda_ < 1e-3)
				{
					step_ *= 1e-3;
					lambda_ *= 1e3;
				}
				else 
				{
					step_ *= 0.5;
					lambda_ *= 2.0;
				}
			}
			else if (lambda_ > 0.9)
			{
				step_ *= 2.0;
				lambda_ *= 0.5;
			}
		}
	}

} // end of namespace BALL
