// $Id: steepestDescent.C,v 1.8 1999/12/28 17:53:25 oliver Exp $

#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/COMMON/limits.h>

using namespace std;

namespace BALL 
{
	Size	SteepestDescentMinimizer::Default::MAX_STEPS = 6;
	const char* SteepestDescentMinimizer::Option::MAX_STEPS = "max_steps";

	// default constructor
	SteepestDescentMinimizer::SteepestDescentMinimizer()
		:	EnergyMinimizer()
	{
	}


	// copy constructor 
	SteepestDescentMinimizer::SteepestDescentMinimizer(const SteepestDescentMinimizer& line_search_minimizer, bool /* deep */)
		:	EnergyMinimizer(line_search_minimizer)
	{
	}

	// assignment operator
	SteepestDescentMinimizer& SteepestDescentMinimizer::operator = (const SteepestDescentMinimizer& line_search_minimizer)
	{
		EnergyMinimizer::operator = (line_search_minimizer);
		return *this;
	}

	// Constructor initialized with a force field
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field)
		:	EnergyMinimizer()
	{
    valid_ = setup(force_field);


    if (!valid_)
    {
      Log.level(LogStream::ERROR) << " steepest descent minimizer setup failed! " << endl;
		}

	}

	// Constructor initialized with a force field and a snapshot  manager 
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field,
                                          SnapShotManager *ssm)
		:	EnergyMinimizer()
	  {
    valid_ = setup(force_field, ssm);


    if (!valid_)
      {
      Log.level(LogStream::ERROR) << " steepest descent minimizer setup failed! " << endl;
		  }

	  }
	
	// Constructor initialized with a force field and a set of options
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field, const Options& new_options)
		:	EnergyMinimizer()
	{
    valid_ = setup(force_field, new_options);

    if (!valid_)
    {
      Log.level(LogStream::ERROR) << " steepest descent minimizer setup failed! " << endl;
		}
	}

	// Constructor initialized with a force field, a snapshot manager, and a set of options
	SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField& force_field, 
                SnapShotManager *ssm, const Options& new_options)
		:	EnergyMinimizer()
	{
    valid_ = setup(force_field, ssm, new_options);

    if (!valid_)
    {
      Log.level(LogStream::ERROR) << " steepest descent minimizer setup failed! " << endl;
		}
	}




	// destructor
	SteepestDescentMinimizer::~SteepestDescentMinimizer()
	{
	}
		
	// virtual function for the specific setup of derived classes
	bool SteepestDescentMinimizer::specificSetup()
	{

		max_steps_ = (Size)options.setDefaultInteger(SteepestDescentMinimizer::Option::MAX_STEPS, (long)SteepestDescentMinimizer::Default::MAX_STEPS);

		return true;
	}

	// Set the parameter max_steps_
	void	SteepestDescentMinimizer::setMaxSteps(Size max_steps)
	{
		max_steps_	= max_steps;
	}

	//	Get the parameter max_steps_
	Size	SteepestDescentMinimizer::getMaxSteps() const
	{
		return max_steps_;
	}


	/*	The minimizer optimizes the energy of the system 
			using a modified line search algorithm.
	*/

	bool SteepestDescentMinimizer::minimize(Size steps, bool restart)
	{
		// define some static variables used for restarting
		
		static float	step_size = maximal_shift_;	// the current step size
		static Size		last_update = 1;						// last update of gradient (in number of iterations)
		static Size		same_energy_counter = 0;		// number of times the same energy was calculated (in number of iterations)
		static float	gradient_norm = 0;					// norm of the gradient
		static float	last_energy = 0;						// energy of the last iteration
		static float	min_energy = 0;							// minimum energy encountered
		static float	energy = 0;									// last calculated energy

		// Checking the minimizer 
		if (isValid() == false)
		{
			Log.level(LogStream::ERROR) << "The steepest descent minimizer is not valid!" << endl;
			return false;
		}

		// if there are no atoms to optimize, return immediately - we have found the optimal
		// solution!
		Size number_of_movable_atoms = force_field_->getNumberOfMovableAtoms();
		if (number_of_movable_atoms == 0)
		{
			return true;
		}

		vector<Atom*>::const_iterator it;
		if (!restart)
		{
			// reset the step size to its default value
			step_size = maximal_shift_;

      // reset the counters
      force_update_counter_ = 0;
      energy_update_counter_ = 0; 

			// calculate initial gradient and gradient norm
			force_field_->updateForces();
      force_update_counter_++; 

			gradient_norm = 0;
			it = force_field_->getAtoms().begin();
			for ( ; it != force_field_->getAtoms().end() ; ++it)
			{
				gradient_norm += (*it)->getForce().getSquareLength();
			} 

			// calculate the RMS gradient (termination condition)
			// in units of kJ/(mol A)
			gradient_norm = sqrt(gradient_norm / (3.0 * (float)number_of_movable_atoms));
			gradient_norm *= Constants::AVOGADRO / 1e13;

			// if the gradient norm is zero, return (we are converged)
			if (gradient_norm == 0.0)
			{
				return true;
			}
		}

		float factor;

		// Iterate and minimize
		Size iteration = 0;

		// if no limit to the number of steps is given, ignore it
		if (steps == 0)
		{
			steps = Limits<Size>::max();
		}

		float energy_change = 0.0;

    while(iteration < maximal_number_of_iterations_)
		{
			// calculate current energy 

			energy_change = energy;
			energy = force_field_->updateEnergy();
      energy_update_counter_++;

			energy_change = energy - energy_change;

      iteration++;

			// check whether the energy changed
			if (fabs(energy - last_energy)  < energy_difference_bound_)
			{
				same_energy_counter++;
				if (same_energy_counter == max_same_energy_)
				{
					break;
				}
			} else {
				last_energy = energy;
				same_energy_counter = 0;
			}
		

			// accept a taken step if it results in a lower energy
			if ((energy < min_energy) || (number_of_iteration_ == 0) 
					|| ((restart == false) && (iteration == 0))
					|| (last_update > 10))
			{
				// if the last 10 steps didn't improve energy, we're in trouble
				// let's recalculate the gradient and increase the step size to get
				// out of here
				if (last_update > 10)
				{
					step_size *= 500;
				} else {
					min_energy = energy;
				}
				last_update = 0;

				// update gradient and gradient norm
				force_field_->updateForces();
        force_update_counter_++; 

        // If the simulation runs with periodic boundary conditions, update the
        // list and position of molecules
        if(force_field_->periodic_boundary.isEnabled() == true)
                     force_field_->periodic_boundary.updateMolecules();

        // Update the force field pair lists in regular intervals
        if(number_of_iteration_ % force_field_->getUpdateFrequency() == 0)
                     force_field_->update();



				gradient_norm = 0;
				it = force_field_->getAtoms().begin();
				for ( ; it != force_field_->getAtoms().end() ; ++it)
				{
					gradient_norm += (*it)->getForce().getSquareLength();
				} 

				// calculate the RMS gradient (termination condition)
				// in units of kJ/(mol A)
				gradient_norm = sqrt(gradient_norm / (3.0 * (float)number_of_movable_atoms));
				gradient_norm *= Constants::AVOGADRO / 1.0e13;


				// check for convergence
				if (gradient_norm <= max_gradient_)
				{
					break;
				}


				// increase step_size
				step_size *= 1.1;

				// calculate factor for atom displacements
				factor = Constants::AVOGADRO / 1.0e13 * step_size / gradient_norm;

			} else {

				// this step didn't result in a lower energy
				// make half a step back and retry				

				// calculate factor for atom displacements
				factor = - Constants::AVOGADRO / 1.0e13 * (step_size * 0.5) / gradient_norm; // half step back

				// reduce step size
				step_size *= 0.5;
				last_update++;
			}


			//iterate over all movable atoms
			it = force_field_->getAtoms().begin();
			for (Size i = 0; i < number_of_movable_atoms ; ++i, ++it)
			{
				(*it)->setPosition((*it)->getPosition() + factor * (*it)->getForce());
			}

			// Test if the non-bonded list must be updated
			if ((number_of_iteration_ % force_field_->getUpdateFrequency()) == 0)
			{
				force_field_->update();
			}

			// Test if the energy has to be written
			if ((number_of_iteration_ % energy_output_frequency_) == 0)
			{
				Log.info() << "iteration " << number_of_iteration_ 
									 << " energy: " << energy << " kJ/mol"
									 << " grad  : " << gradient_norm  << " kJ/(mol A)"
									 << endl;
			}

      // Check if a snapshot of the system shall be made
      if(snapShot_ptr_ != 0 && number_of_iteration_ % snapshot_output_frequency_ == 0)
        {
        snapShot_ptr_->takeSnapShot();
        }

			// count iterations
			iteration++;
			number_of_iteration_++;
		}

    Log.info() << endl ;
    Log.info() << "No. of force updates : " << force_update_counter_ << endl;
    Log.info() << "No. of energy updates: " << energy_update_counter_ << endl;
    Log.info() << endl; 


		// test whether convergence was reached
		return (gradient_norm <= max_gradient_) || (same_energy_counter == max_same_energy_);
	}
	
} // namespace BALL
