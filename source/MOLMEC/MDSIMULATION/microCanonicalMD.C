// $Id: microCanonicalMD.C,v 1.9 2001/12/30 13:28:50 sturm Exp $

// BALL includes 
#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>

namespace BALL
{
	using namespace std;

	// The default constructor with no arguments
	MicroCanonicalMD::MicroCanonicalMD()
		:	MolecularDynamics()
	{
		valid_ = false;
	}


	// This constructor uses the given force field 
	MicroCanonicalMD::MicroCanonicalMD(ForceField& my_force_field)
		:	MolecularDynamics(my_force_field)
	{
		// the user does not want to take snapshots. 
		// Create a dummy manager 
		SnapShotManager tmp;
		valid_ = setup(my_force_field, &tmp);
	}

	// This constructor uses the given force field and snapshot manager 
	MicroCanonicalMD::MicroCanonicalMD(ForceField& my_force_field, SnapShotManager * ssm)
		:	MolecularDynamics(my_force_field)
	{
		valid_ = setup(my_force_field, ssm);
	}

	// This constructor uses the given force field and options 
	MicroCanonicalMD::MicroCanonicalMD(ForceField& my_force_field,
																			SnapShotManager * ssm, const Options& my_options)
		:	MolecularDynamics(my_force_field)
	{
		valid_ = setup(my_force_field, ssm, my_options);
	}

	// The destructor
	MicroCanonicalMD::~MicroCanonicalMD()
	{
	}


  // Choose a new time step. This means that the pre-factors must be
  // computed anew
  void MicroCanonicalMD::setTimeStep(double time)
  {
		// call the corresponding method in the base class
		MolecularDynamics::setTimeStep(time);

		// calculate the new factors 
		MicroCanonicalMD::calculateFactors(); 
  }

	// This method does the general setup. 
	bool MicroCanonicalMD::setup(ForceField& my_force_field, SnapShotManager * ssm)
	{
		// No specific options have been named -> we use the force field's options
		valid_ = setup(my_force_field, ssm, my_force_field.options);

		return valid_;
	}

	bool MicroCanonicalMD::setup(ForceField& my_force_field, SnapShotManager * ssm, const Options& my_options)
	{
		// First check whether the force field is valid. If not, then it is useless
		// to do anything here.
		if (my_force_field.isValid() == false)
		{
			// The setup has failed for some reason. Output an error message.
			Log.error() << "MicroCanonicalMD::setup: setup failed because the force field was not valid!" << std::endl;

			valid_ = false;
			return false;
		}

		// call the base class setup method
		valid_ = MolecularDynamics::setup(my_force_field, ssm, my_options);

		if (valid_ == false)
			return false;

		// call the specific Setup
		valid_ = specificSetup();

		return valid_;
	}


	// This method calculates certain factors that are needed 
	// throughout the simulation 
	void MicroCanonicalMD::calculateFactors()
	{
		// Precompute a factor involving each atom's mass 
		mass_factor_.clear();

		vector < Atom * >::iterator it;
		AuxFactors item;
		Atom *atom_ptr;

		for (it = atom_vector_.begin(); it != atom_vector_.end(); ++it)
		{
			// Factor1 = time_step_ * time_step_ / (2 * mass)
			// Factor2 = time_step_ / (2 * mass)
			// Factors must be scaled by 6.022 * 10^12 to adjust units 
			atom_ptr = *it;
			item.factor2 = Constants::AVOGADRO / 1e23 * 1e12 * 0.5 * time_step_ / atom_ptr->getElement().getAtomicWeight();
			item.factor1 = item.factor2 * time_step_;
			mass_factor_.push_back(item);
		}

	}	// end of ' calculateFactors' 


	// This method performs additional setup preparations in addition 
	// to those done in MolecularDynamics::setup 
	bool MicroCanonicalMD::specificSetup()
	{
		// nothing to do...
		return valid_;
	}

	// The copy constructor 
	MicroCanonicalMD::MicroCanonicalMD(const MicroCanonicalMD& rhs, bool deep)
		:	MolecularDynamics(rhs, deep)
	{
		// copy class specific variables 
		mass_factor_ = rhs.mass_factor_;
	}

	// The assignment operator 
	MicroCanonicalMD& MicroCanonicalMD::operator = (const MicroCanonicalMD& rhs)
	{
		mass_factor_ = rhs.mass_factor_;

		// call the assignment operator of the base class
		this->MolecularDynamics::operator = (rhs);

		return *this;
	}

	// This method does the actual simulation stuff
	// It runs for the indicated number of iterations           
  // restart=true means that the counting of iterations is started with the end
  // value of the previous run
	void MicroCanonicalMD::simulateIterations(Size iterations, bool restart)
	{
		// local variables
		double current_energy;
		Size max_number;

		Atom *atom_ptr;
		Size force_update_freq;
		Size iteration;

    if (restart == false)
    {
      // reset the current number of iteration and the simulation time  to the values given
      // in the options 
      number_of_iteration_  = (Size)options.getInteger(MolecularDynamics::Option::NUMBER_OF_ITERATION);
      current_time_ = options.getReal(MolecularDynamics::Option::CURRENT_TIME);
    }
    else
    {
			// the values from the last simulation run are used; increase by one to start in the
			// next iteration 
			number_of_iteration_++; 
    }

		// determine the largest value for the iteration counter 
	  max_number = number_of_iteration_ + iterations;

		// pre-calculate some needed factors
	  calculateFactors();

		// make sure that the MD simulation operates on the same set of atoms
		// as the forcefield  does (this may have changed since setup was called)
	  atom_vector_ = force_field_ptr_->getAtoms();


		// First check whether the  force field and the MD instance
		// are valid
		if (!valid_ || force_field_ptr_ == 0 || !force_field_ptr_->isValid())
		{
			Log.error() << "MD simulation not possible! " << "MD class is  not valid." << endl;
			return;
		}


		// Get the frequency for updating the Force Field pair lists
		force_update_freq = force_field_ptr_->getUpdateFrequency();

		// If the simulation runs with periodic boundary conditions, update the
		// list and position of molecules
		if (force_field_ptr_->periodic_boundary.isEnabled() == true)
		{
			force_field_ptr_->periodic_boundary.updateMolecules();
		}

		// Calculate the forces at the beginning of the simulation
		force_field_ptr_->updateForces();

		// DEBUG ???
		//force_field_ptr_->updateEnergy();	
		// only done for debugging purposes 



		// This is the main loop of the MD simulation. The Velocity-Verlet method
		// is used for the propagation of atomic positions  and velocities 
		for (iteration = number_of_iteration_; iteration < max_number; iteration++)
		{
			// The force field data structures must be  updated regularly
			if (iteration % force_update_freq == 0)
			{
				force_field_ptr_->update();
			}

			// If the simulation runs with periodic boundary conditions, update the
			// list and position of molecules
			if (force_field_ptr_->periodic_boundary.isEnabled() == true)
			{
				force_field_ptr_->periodic_boundary.updateMolecules();
			}

			// In regular intervals, calculate and  output the current energy
			if (iteration % energy_output_frequency_ == 0)
			{
				current_energy = force_field_ptr_->updateEnergy();
				updateInstantaneousTemperature();

				Log.info()
					<< "Microcanonical MD simulation System has potential energy "
					<< current_energy << " kJ/mol at time " << current_time_ + (double) iteration *time_step_ << " ps " << endl;


				Log.info()
					<< "Microcanonical MD simulation System has kinetic energy "
					<< kinetic_energy_ << " kJ/mol at time " << current_time_ + (double) iteration *time_step_ << " ps " << endl;

        
         /* ?????  Temperatur ausgeben? 
				Log.info()
					<< "MicroCanonical MD simulation System has temperature  "
					<< current_temperature_ << " K at time " << current_time_ + (double) iteration *time_step_ << " ps " << endl;
                                 */       
			}

			// Calculate new atomic positions and new tentative velocities 
			vector<Atom*>::iterator atom_it(atom_vector_.begin());
			vector<AuxFactors>::iterator factor_it(mass_factor_.begin());
			for (; atom_it != atom_vector_.end(); ++atom_it, ++factor_it)
			{
				atom_ptr = *atom_it;

				// First calculate the new atomic position
				// x(t+1) = x(t) + time_step_ * v(t) + time_step_^2/(2*mass) * F(t)
				atom_ptr->setPosition(atom_ptr->getPosition()
															 + (float)time_step_ * atom_ptr->getVelocity() + (float)factor_it->factor1 * atom_ptr->getForce());

				// calculate a tentative  velocity 'v_tent' for the next iteration
				// v_tent(t+1) = v(t) + time_step_ / (2 * mass) * F(t)
				atom_ptr->setVelocity(atom_ptr->getVelocity() + (float)factor_it->factor2 * atom_ptr->getForce());
			}	// next atom 


			// Determine the forces for the next iteration
			force_field_ptr_->updateForces();

			for (atom_it = atom_vector_.begin(),
					 factor_it = mass_factor_.begin(); atom_it != atom_vector_.end(); ++atom_it, ++factor_it)
			{
				atom_ptr = *atom_it;

				// Calculate the final velocity for the next iteration
				atom_ptr->setVelocity(atom_ptr->getVelocity() + (float)factor_it->factor2 * atom_ptr->getForce());
			}	// next atom

			// Take a snapshot in regular intervals if desired              
			if (snapshot_manager_ptr_ != 0 && iteration % snapshot_frequency_ == 0)
			{
				snapshot_manager_ptr_->takeSnapShot();
			}

		}	// next iteration 

		// update the current time
		current_time_ += (double)iterations * time_step_;

    // set the current iteration
    number_of_iteration_ = iteration - 1; 

		// update the current temperature in the system
		force_field_ptr_->updateEnergy();
		updateInstantaneousTemperature();

	}	// end of simulateIterations() 

}	// end of namespace BALL
