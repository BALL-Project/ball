// $Id: canonicalMD.C,v 1.15 2001/09/01 16:09:26 oliver Exp $

// BALL includes 
#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h>

namespace BALL
{
	using namespace std;

	// The default constructor with no arguments
	CanonicalMD::CanonicalMD()
		throw()
		:	MolecularDynamics()
	{
		valid_ = false;
	}


	// This constructor uses the given force field 
	CanonicalMD::CanonicalMD(ForceField& myforcefield)
		throw()
		:	MolecularDynamics(myforcefield)
	{
		// the user does not want to take snapshots.
		// Create an invalid dummy manager
		SnapShotManager tmp;
		valid_ = setup(myforcefield, &tmp);
	}


	// This constructor uses the given force field and a snapshot manager 
	CanonicalMD::CanonicalMD(ForceField& myforcefield,
			SnapShotManager* ssm)
		throw()
		:	MolecularDynamics(myforcefield)
	{
		valid_ = setup(myforcefield, ssm);
	}


	// This constructor uses the given force field and options 
	CanonicalMD::CanonicalMD(ForceField &myforcefield,
			SnapShotManager *ssm, const Options &myoptions)
		throw()
		:	MolecularDynamics(myforcefield)
	{
		valid_ = setup (myforcefield, ssm, myoptions);
	}


	// The copy constructor 
	CanonicalMD::CanonicalMD(const CanonicalMD& rhs, bool deep)
		throw()
		:	MolecularDynamics (rhs, deep)
	{
		// copy class specific variables 
		mass_factor_ = rhs.mass_factor_;
	}


	// The destructor
	CanonicalMD::~CanonicalMD()
		throw()
	{
		// Nothing to do 
	}


	// This method does the general setup. 
	bool CanonicalMD::setup(ForceField & myforcefield, SnapShotManager * ssm)
		throw()
	{
		// No specific options have been named -> we use the force field's options
		valid_ = setup (myforcefield, ssm, myforcefield.options);

		return valid_;
	}


	bool CanonicalMD::setup(ForceField & myforcefield, SnapShotManager * ssm,
			const Options & myoptions)
		throw()
	{
		// First check whether the force field is valid. If not, then it is useless
		// to do anything here.
		if (myforcefield.isValid() == false)
		{
			// The setup has failed for some reason. Output an error message.
			Log.error() << "CanonicalMD::setup(): "
				<< "forcefield is not valid." << endl;
			valid_ = false;
			return false;
		}

		// first call the base class setup method
		valid_ = MolecularDynamics::setup(myforcefield, ssm, myoptions);

		if (valid_ == false)
		{
			return false;
		}

		// base class setup was successful; we can go on

		// class specific Options ?
		// none

		// call the specific Setup
		valid_ = specificSetup();

		return valid_;
	}


  // Choose a new time step. This means that all pre-factors must be
  // recomputed.
	void CanonicalMD::setTimeStep(double time)
		throw()
  {
		MolecularDynamics::setTimeStep(time);

		// calculate the new factors
		calculateFactors_();
	}
 

  // This method allows us to set the coupling to a thermal bath 
  void CanonicalMD::setBathRelaxationTime(double time)
		throw()
  {
		bath_relaxation_time_ = time;
		options[MolecularDynamics::Option::BATH_RELAXATION_TIME] = time;
	}
                                           

  // This method allows us to get the current value for the bath 
  // relaxation time (coupling to an external heat bath) 
  double CanonicalMD::getBathRelaxationTime() const
		throw()
	{
		return bath_relaxation_time_;
	}       


	// This method calculates certain factors that are needed
	// throughout the simulation
	void CanonicalMD::calculateFactors_()
		throw()
	{
		// precompute a vector of factors to save some work 
		// Clear the vector of factors if it is already existing
		mass_factor_.clear();

		vector<Atom*>::iterator it;
		Aux_Factors item;
		Atom* atom_ptr;
		Size index = 0;
		mass_factor_.resize(atom_vector_.size());
		for (it = atom_vector_.begin(); it != atom_vector_.end(); ++it, ++index)
		{
			// Factor1 = time_step_ * time_step_ / (2 * mass)
			// Factor2 = time_step_ / (2 * mass)
			// Factors must be scaled by 6.022 * 10^12 to adjust units
			atom_ptr = *it;
			item.factor2 = Constants::AVOGADRO / 1e23 * 1e12 * 0.5 * time_step_ 
				/ atom_ptr->getElement().getAtomicWeight();
			item.factor1 = item.factor2 * time_step_;
			// BAUSTELLE: use op[] and resize instead of push_back
			mass_factor_[index] = item;
		}
	}	// end of 'calculateFactors' 


	// This method performs additional setup preparations in addition 
	// to those done in MolecularDynamics::setup 
	bool CanonicalMD::specificSetup()
		throw()
	{
		if (!valid_)	
		{
			Log.error() << "CanonicalMD::specificSetup(): "
				<< "Instance is not valid." << endl;
			return false;
		}

		// Set some class specific options
		// The bath relaxation time. This indicates how closely the system will
		// be coupled to a heat bath in order to keep the temperature constant.
		options.setDefaultReal(MolecularDynamics::Option::BATH_RELAXATION_TIME,
				MolecularDynamics::Default::BATH_RELAXATION_TIME);
		bath_relaxation_time_ 
			= options.getReal(MolecularDynamics::Option::BATH_RELAXATION_TIME);

		return true;
	}	


	// The assignment operator 
	CanonicalMD & CanonicalMD::operator = (const CanonicalMD & rhs)
		throw()
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
	void CanonicalMD::simulateIterations(Size iterations, bool restart)
		throw()
	{
		// local variables
		double current_energy;
		float scaling_factor;
		Size max_number;

		Atom* atom_ptr;
		Size force_update_freq;
		Size iteration;

		vector <Atom*>::iterator atom_it;
		vector <Aux_Factors>::iterator factor_it;

		if (restart == false)
		{
			// reset the current number of iteration and the simulation time to
			// the values given in the options
			number_of_iteration_
				= (Size)options.getInteger(MolecularDynamics::Option::NUMBER_OF_ITERATION);
			current_time_
				= options.getReal(MolecularDynamics::Option::CURRENT_TIME);
		}
		else
		{
			// the values from the last simulation run are used; increase by one
			// to start in the next iteration
			number_of_iteration_++;
		}

		// determine the largest value for the iteration counter
		max_number = number_of_iteration_ + iterations;

		// First check whether the  force field and the MD instance
		// are valid
		if ((valid_ == false) || (force_field_ptr_ == 0) 
				|| (force_field_ptr_->isValid () == false))
		{
			Log.error() << "CanonicalMD::simulateIterations(): "
				<< "MD simulation not possible, class is not valid." << endl;
			return;
		}

		// pre-calculate some needed factors
		calculateFactors_();

		// make sure that the MD simulation operates on the same set of atoms
		// as the forcefield does (this may have changed since setup was called)
		atom_vector_ = force_field_ptr_->getAtoms();

		// Get the frequency for updating the Force Field pair lists
		force_update_freq = force_field_ptr_->getUpdateFrequency();

		// If the simulation runs with periodic boundary conditions, update the
		// position of molecules
		if (force_field_ptr_->periodic_boundary.isEnabled() == true)
		{
			force_field_ptr_->periodic_boundary.updateMolecules();
		}

		// Calculate the forces at the beginning of the simulation
		force_field_ptr_->updateForces();

		// This is the main loop of the MD simulation. The Velocity-Verlet method
		// is used for the propagation of atomic positions and velocities.
		// In addition, the temperature is kept roughly constant by applying 
		// Berendsen's velocity rescaling method. 
		for (iteration = number_of_iteration_; iteration < max_number; ++iteration)
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

			// The new velocities calculated a few lines further below will be
			// rescaled by a certain factor. We use the temperature from the
			// previous iteration for computing the factor. 
			updateInstantaneousTemperature();

			// In regular intervals, calculate and  output the current energy
			if (iteration % energy_output_frequency_ == 0)
			{
				// update the current values for energy and temperature and 
				// output them 
				current_energy = force_field_ptr_->updateEnergy();
				// updateInstantaneousTemperature();

				Log.info()
					<< "Canonical MD simulation System has potential energy "
					<< current_energy << " kJ/mol at time " 
					<< current_time_ + (double) iteration *time_step_ << " ps" << endl;
				Log.info()
					<< "Canonical MD simulation System has temperature  "
					<< current_temperature_ << " at time " 
					<< current_time_ + (double) iteration *time_step_ << " ps " << endl;
			}

			// check whether the rescaling will be successful
			// We trap a special case: at the start of a simulation, the current
			// temperature will be zero, so we do not rescale 
			if ((bath_relaxation_time_ != 0.0) && (current_temperature_ != 0.0))
			{
				// calculate the scaling factor (Berendsen's velocity scaling)
				scaling_factor = sqrt(1.0 + time_step_ / bath_relaxation_time_
						* (reference_temperature_ / current_temperature_ - 1.0));
			}
			else 
			{
				// we do not rescale for a temperature of zero
				scaling_factor = 1.0;
			}

			// Calculate new atomic positions and new tentative velocities 
			for (atom_it = atom_vector_.begin(), factor_it = mass_factor_.begin();
					atom_it != atom_vector_.end(); ++atom_it, ++factor_it)
			{
				atom_ptr = *atom_it;

				// First calculate the new atomic position
				// x(t+1) = x(t) + time_step_ * v(t) + time_step_^2/(2*mass) * F(t)
				atom_ptr->setPosition(atom_ptr->getPosition() 
						+ (float)time_step_ * atom_ptr->getVelocity() 
						+ (float) factor_it->factor1 * atom_ptr->getForce());

				// calculate a tentative  velocity 'v_tent' for the next iteration
				// v_tent(t+1) = v(t) + time_step_ / (2 * mass) * F(t)
				atom_ptr->setVelocity(atom_ptr->getVelocity() 
						+ (float) factor_it->factor2 * atom_ptr->getForce());

			}	// next atom 



			// Determine the forces for the next iteration
			force_field_ptr_->updateForces();

			// Calculate the final velocity for the next iteration
			// and rescale it in order to keep the temperature constant 
			for (atom_it = atom_vector_.begin(), factor_it = mass_factor_.begin();
					atom_it != atom_vector_.end(); ++atom_it, ++factor_it)
			{
				atom_ptr = *atom_it;
				atom_ptr->setVelocity(scaling_factor * (atom_ptr->getVelocity()
							+ (float)factor_it->factor2 * atom_ptr->getForce()));
			}	// next atom

			// Take a snapshot in regular intervals if desired
			if (snapshot_manager_ptr_ != 0 && iteration % snapshot_frequency_ == 0)
			{
				snapshot_manager_ptr_->takeSnapShot ();
			}

		}	// next iteration 


		// The simulation is finished. Update the current time and temperature  
		current_time_ += (double) iterations * time_step_;
		number_of_iteration_ = iteration - 1; 

		force_field_ptr_->updateEnergy();
		updateInstantaneousTemperature();

	}	// end of simulateIterations() 

}	// end of namespace Ball
