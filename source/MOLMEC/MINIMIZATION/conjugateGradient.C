// $Id: conjugateGradient.C,v 1.11 2001/05/17 01:30:55 oliver Exp $
// Minimize the potential energy of a system using a nonlinear conjugate 
// gradient method with  line search

#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/MINIMIZATION/lineSearch.h>
#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/COMMON/limits.h>


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
			method_(DEFAULT_METHOD)
	{
	}

	// Constructor initialized with a force field
	ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField& force_field)
		: EnergyMinimizer(),
			method_(DEFAULT_METHOD)
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
			method_(DEFAULT_METHOD)
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
			method_(DEFAULT_METHOD)
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
			method_(DEFAULT_METHOD)
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
		(const ConjugateGradientMinimizer& rhs, bool /* deep */)
		: EnergyMinimizer(rhs),
			method_(rhs.method_)
	{
		// BAUSTELLE
	}

  // The assignment operator
  ConjugateGradientMinimizer& ConjugateGradientMinimizer::operator =
        (const ConjugateGradientMinimizer& rhs)
	{
    EnergyMinimizer::operator = (rhs);

		// BAUSTELLE

    return *this;
	}
		
  // This method is responsible for doing the specific setup of this class       
  bool ConjugateGradientMinimizer::specificSetup()
	{
    // set the options  to their default values if not already set  
    step_ = options.setDefaultReal
			(Option::STEP_LENGTH, Default::STEP_LENGTH); 
		updateForces();
		step_ *= current_grad_.norm;

    // determine the number of atoms
    number_of_atoms_ = (Size)force_field_->getAtoms().size(); 

    return true;
	}

  // Set explicitly the option step_
  void	ConjugateGradientMinimizer::setStepLength(double length)
	{
    step_ = length;
	}

  // get  the value of option step_ 
  double	ConjugateGradientMinimizer::getStepLength() const 
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

			// calculate the current gradient and determine the current
			// direction as the direction of steepest descent
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
				static bool first_call = true;
				static vector <Vector3> a_i, b_i, p_t, y_t, p_i, y_i;
				static double D_1, D_4;
				static Size restart_frequency = 1;

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

				if (first_call == true)
				{
					restart_frequency = 3 * number_of_atoms_;

					p_t.resize (number_of_atoms_);
					y_t.resize (number_of_atoms_);
					y_i.resize (number_of_atoms_);
					p_i.resize (number_of_atoms_);
					a_i.resize (number_of_atoms_);
					b_i.resize (number_of_atoms_);

					first_call = false;

					D_1 = 0;
					D_4 = 0;

					// Do some initialisations 
					// Note: when this method is called for the first time, the 
					// minimisation algorithm must have already done one iteration! 

					for (i = 0; i < number_of_atoms_; i++)
					{
						p_t[i] = direction_[i] * lambda_ * step_;
						y_t[i] = initial_grad_[i] - old_grad_[i];

						D_1 += p_t[i] * y_t[i];
						D_4 += y_t[i] * y_t[i];
					}

					if (D_4 == 0)
					{
						throw Exception::DivisionByZero(__FILE__, __LINE__);
					}
				}

				double sum1 = 0;
				double sum2 = 0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					y_i[i] = initial_grad_[i] - old_grad_[i];
					p_i[i] = direction_[i] * step_ * lambda_;

					sum1 += y_i[i] * y_i[i];
					sum2 += p_i[i] * p_i[i];
				}

				if (sum1 == 0 || sum2 == 0)
				{
					// take the current gradient as the new search direction
					direction_ = initial_grad_;
					direction_.negate();

					return;
				}


				if ((number_of_iteration_ % restart_frequency == 0))
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
						D_1 = 0;
						D_4 = 0;

						for (i = 0; i < number_of_atoms_; i++)
						{
							p_t[i] = direction_[i] * lambda_ * step_;
							y_t[i] = initial_grad_[i] - old_grad_[i];

							D_1 += p_t[i] * y_t[i];
							D_4 += y_t[i] * y_t[i];
						}

						if (D_4 == 0)
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
					C_1 += p_t[i] * initial_grad_[i];
					C_2 += y_t[i] * initial_grad_[i];
					D_2 += p_t[i] * y_i[i];
					D_3 += y_t[i] * y_i[i];
				}

				double factor1 = D_1 / D_4;
				double factor2 = D_2 / D_4;
				double factor3 = (2 * D_2 / D_1 - D_3 / D_4);

				for (i = 0; i < number_of_atoms_; i++)
				{
					a_i[i] = y_i[i] * factor1 - y_t[i] * factor2 + p_t[i] * factor3;
				}

				factor2 = C_1 / D_4;
				factor3 = (2 * C_1 / D_1 - C_2 / D_4);

				for (i = 0; i < number_of_atoms_; i++)
				{
					b_i[i] = initial_grad_[i] * factor1 - y_t[i] * factor2 + p_t[i] * factor3;
				}

				double D_5 = 0;
				double D_6 = 0;
				double D_7 = 0;
				double D_8 = 0;
				for (i = 0; i < number_of_atoms_; i++)
				{
					D_5 += p_i[i] * y_i[i];
					D_6 += a_i[i] * y_i[i];
					D_7 += a_i[i] * initial_grad_[i];
					D_8 += p_i[i] * initial_grad_[i];
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
					direction_[i] = -b_i[i] + a_i[i] * factor1 - p_i[i] * factor2;
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


	bool ConjugateGradientMinimizer::findStep()
	{		
		// first, we search along the line by taking a step_ along direction_

		// define an alias for the atom vector
		AtomVector& atoms = const_cast<AtomVector&>(force_field_->getAtoms());
		
		bool success = true;
		while (success)
		{
			atoms.moveTo(direction_, step_);
			success = (updateEnergy() < initial_energy_);
			if (success)
			{
				// check whether the gradient decreases along direction_
				updateForces();
				double dir_grad = direction_ * current_grad_;
				success &= (dir_grad < 0.0);
			}
			
			if (success)
			{
				step_ *= 2.0;
			}
		}
		
		// now we perform line searches along direction_
		LineSearch line_search(*this);
	
		bool result =  line_search.minimize(lambda_, step_);
		//Log.info() << "LineSearch: lambda = " << lambda_ << " result = " << result << endl;
		
		if (!result)
		{
			// Log.info() << "resetting search direction to gradient [" << direction_.rms << "/";
			// reset the search direction to the 
			// negative gradient
			direction_ = initial_grad_;
			direction_.negate();
			//Log.info() << direction_.rms << "]" << endl;

			// invalidate the current gradient (LineSearch::minimize())
			// recalculate it for lambda = 1.0;
			current_grad_.invalidate();

			// ...and try another line search
			result = line_search.minimize(lambda_, step_);
			//Log.info() << "LineSearch: lambda = " << lambda_ << " result = " << result << endl;
		}

		if (!result)
		{
			//Log.info() << "resetting step length" << endl;
			step_ = 0.01 / initial_grad_.norm;
			
			// invalidate the current gradient (LineSearch::minimize())
			// recalculate it for lambda = 1.0;
			current_grad_.invalidate();

			// ...and try another line search
			result = line_search.minimize(lambda_, step_);
			//Log.info() << "LineSearch: lambda = " << lambda_ << " result = " << result << endl;
		}			

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

		return result;
	} // end of method 'findStep'

  // The minimiser optimises the energy of the system 
  // by using a conjugate gradient method. 
  // Return value is true when no further steps can be taken!
  bool ConjugateGradientMinimizer::minimize
		(Size iterations, bool restart)
  {
		// remember <g_i, d_i> (the scalar product of the last 
		// search direction and the last gradient)
		// keep them static for restarts
		static double old_dir_grad;
		static bool old_dir_grad_valid;
		if (!restart)
		{
			// invalidate old_dir_grad if this is not a restart
			old_dir_grad_valid = false;	
		}
		

		// Log.info() << "CGM: minimize(" << iterations << ", " << restart << ")" << endl;
		
    // Check the minimizer  and the force field connected to it 
    if (isValid() == false || force_field_->isValid() == false)
    {
      Log.error() << "The ConjugateGradientMinimizer is not valid or " << endl
                  << " the force field is not valid!. " << endl; 

      return true;
    }

		// define an alias for the atom vector
		AtomVector& atoms = const_cast<AtomVector&>(force_field_->getAtoms());

    // If there are no atoms in the system, minimization is easy!
    if (atoms.size() == 0)
    {
      return true;
    }

		// if this is not a restart or we do not have a valid
		// gradient, recalculate the gradient, the energy, and the search 
 		// direction
		if (!restart || !old_grad_.isValid() || old_grad_.size() == 0)
		{
			//Log.info() << "CGM: calculating initial gradient...";
			// ...forces
			updateForces();
			// Log.info() << " [" << current_grad_.size() << "/" << current_grad_.norm << "/" << current_grad_.rms << " / " << initial_energy_ << "]" << endl;
			initial_grad_ = current_grad_;
			initial_energy_ = updateEnergy();

			// Log.info() << " [" << current_grad_.size() << "/" << current_grad_.norm << "/" << current_grad_.rms << " / " << initial_energy_ << "]" << endl;

			// direction = -gradient
			direction_ = current_grad_;
			direction_.negate();

			// reset the same energy counter
			same_energy_counter_ = 0;
		}
		
		// set a limit for the iterations:
		// If no upper bound for the number of iterations is given, the value stored in 
		// the options will be the limit
		Size max_iteration = number_of_iteration_;
		if (iterations == 0)
		{
			max_iteration += maximal_number_of_iterations_;
		}
		else
		{
			max_iteration += iterations;
		}

		// save the current atom positions
		atoms.savePositions();

		// iterate: while not converged and not enough iterations
		while (!isConverged() && (number_of_iteration_ < max_iteration))
		{
			// invalidate the current direction: we don't want to
			// take this direction again. If findStep() 
			// creates a new direction, we keep it (since a new
			// Gradient object is valid). However, if the direction remains
			// invalid, we calculate a new direction at the end of this loop
			direction_.invalidate();

			// try to find a new solution
			bool result = findStep();
			// take the step and save these positions
			atoms.moveTo(direction_, lambda_ * step_);
			atoms.savePositions();

			// if findStep only found an emergency solution...
			if (!result)
			{	
				// ...calculate energy and forces for the new position
				updateForces();
				updateEnergy();
			} 
			
			// store the gradient and the energy
			old_grad_ = initial_grad_;
			old_energy_ = initial_energy_;
			
			// store the current gradient and energy
			initial_energy_ = current_energy_;
			initial_grad_ = current_grad_;

			// Calculate a new search direction if the search direction is invalid
			// 
			// If someone has already created a valid direction
			// (e.g. by using the negative gradient as is done by findStep)
			// it is flagged as valid (since it is new!)
			//
			if (!direction_.isValid())
			{
				// Log.info() << "CGM: updating direction..." << endl;
				updateDirection();
			}
			else 
			{
				// we have to use the current gradient
				direction_ = initial_grad_;
				direction_.negate();
			}

			// non-constant step size option
			//
			double dir_grad = initial_grad_ * direction_;
			if (old_dir_grad_valid)
			{
				step_ = lambda_ * step_ * old_dir_grad / dir_grad;
			}
			else
			{
				// for restart steps, we reset the step size 
				// to 0.01/(||d_i||)
				step_ = 0.01 / direction_.norm;
			}
			old_dir_grad = dir_grad;
			old_dir_grad_valid = true;

			
			// increment iteration counter, take snapshots, print energy,
			// update pair lists, and check the same-energy counter
			finishIteration();
	
		}	// end of main loop


		// check for convergence
		bool convergence_reached = isConverged();
		if (convergence_reached)
		{
			Log.info() << "convergence reached" << endl;
		}

		return convergence_reached;
  } // end of method 'minimize' 
			
} // end of namespace BALL
