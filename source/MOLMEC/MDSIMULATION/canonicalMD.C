// $Id: canonicalMD.C,v 1.2 1999/12/30 18:14:56 pmueller Exp $

#include <BALL/MOLMEC/MDSIMULATION/canonicalMD.h> 

namespace BALL 
  {
  using namespace std;

  // The default constructor with no arguments
  CanonicalMD::CanonicalMD() : MolecularDynamics()
   {
   valid_ = false; 
   }


  // This constructor uses the given force field 
  CanonicalMD::CanonicalMD(ForceField &myforcefield) 
              : MolecularDynamics(myforcefield)
   {
   // the user does not want to take snapshots.
   // Create an invalid  dummy manager
   SnapShotManager tmp;
   valid_ = setup(myforcefield,&tmp);
   }

  // This constructor uses the given force field and a snapshot manager 
  CanonicalMD::CanonicalMD(ForceField &myforcefield, SnapShotManager *ssm) 
              : MolecularDynamics(myforcefield)
   {
   valid_ = setup(myforcefield,ssm);
   }

  // This constructor uses the given force field and options 
  CanonicalMD::CanonicalMD(ForceField &myforcefield, SnapShotManager *ssm,
             const Options &myoptions)  
             : MolecularDynamics(myforcefield)
   {
   valid_ = setup(myforcefield,ssm, myoptions);
   }

   // The destructor
   CanonicalMD::~CanonicalMD()
     {
     // Nothing to do 
     }

   // This method does the general setup. 
   bool CanonicalMD::setup(ForceField &myforcefield, SnapShotManager *ssm)
     {
     // No specific options have been named -> we use the force field's options
     valid_ = setup(myforcefield, ssm, myforcefield.options); 
     
     return valid_; 
     }

   bool CanonicalMD::setup(ForceField &myforcefield, SnapShotManager *ssm, const Options &myoptions)
     {
     // First check whether the force field is valid. If not, then it is useless
    // to do anything here.
    if(myforcefield.isValid() == false)
      {
      // The setup has failed for some reason. Output an error message.
      Log.level(LogStream::ERROR) <<  
                         "Setup of instance of class 'MicroCanonical' hast failed"
                                 << endl;

      valid_ = false;
      return false;
      }

     // first call the base class setup method
     valid_ = MolecularDynamics::setup(myforcefield,ssm, myoptions);

     if(valid_ == false)
        return false;

     // base class setup was successful; we can go on
     
     // class specific Options ?
     // none

     // call the specific Setup
     valid_ = specificSetup();


     return valid_; 

     }


   
   // This method calculates certain factors that are needed
   // throughout the simulation
   void CanonicalMD::calculateFactors()
     {
     // precompute a vector of factors to save some work 
     // Clear the vector of factors if it is already existing
     mass_factor_.clear();

     vector<Atom *>::iterator it; 
     Aux_Factors item; 
     Atom * atom_ptr; 


     for(it = atom_vector_.begin(); it != atom_vector_.end(); ++it)
       {
       // Factor1 = time_step_ * time_step_ / (2 * mass)
       // Factor2 = time_step_ / (2 * mass)
       // Factors must be scaled by 6.022 * 10^12 to adjust units
       atom_ptr = *it; 
       item.factor2 = Constants::AVOGADRO / 1e23 * 1e10 * 
                       0.5 * time_step_ / atom_ptr->getElement().getAtomicWeight();
       item.factor1 = item.factor2 * time_step_; 
       mass_factor_.push_back(item); 
       }


     } // end of 'calculateFactors' 


   // This method performs additional setup preparations in addition 
   // to those done in MolecularDynamics::setup 
   bool CanonicalMD::specificSetup()
     {
     if(!valid_)
       return false; 

     // Set some class specific options
     // The bath relaxation time. This indicates how closely the system will
     // be coupled to a heat bath in order to keep the temperature constant.
     options.setDefaultReal(MolecularDynamics::Option::BATH_RELAXATION_TIME,
                           MolecularDynamics::Default::BATH_RELAXATION_TIME);
     bath_relaxation_time_ = options.getReal(
                           MolecularDynamics::Option::BATH_RELAXATION_TIME);

     return true; 
     } // end of 'specificSetup' 

   // The copy constructor 
   CanonicalMD::CanonicalMD(const CanonicalMD &rhs, bool deep)
       : MolecularDynamics(rhs,deep)
     {
     // copy class specific variables 
     mass_factor_ = rhs.mass_factor_;
     }

   // The assignment operator 
   CanonicalMD &CanonicalMD::operator=(const CanonicalMD &rhs)
     {
     mass_factor_ = rhs.mass_factor_; 

     // call the assignment operator of the base class
     this->MolecularDynamics::operator=(rhs); 

     return *this; 
     }

   // This method does the actual simulation stuff
   // It runs for the number of iterations currently stored in
   // 'number_of_iterations_' 
   void CanonicalMD::simulate(bool restart) 
     {
     simulateIterations(maximal_number_of_iterations_,restart); 
     }


   // This method does the actual simulation stuff
   // It runs for the indicated simulation time in picoseconds 
   void CanonicalMD::simulateTime(float simulation_time,bool restart) 
     {
     Size number;

     // determine the number  of iterations and call 'simulateIterations'
     if(valid_)
       {
       number = static_cast<Size> (simulation_time / time_step_);
       simulateIterations(number,restart);
       }
     }


   // This method does the actual simulation stuff
   // It runs for the indicated number of iterations           
   // Restart feature is not used at the moment!
   void CanonicalMD::simulateIterations(Size iterations,bool restart) 
     {
     // local variables
     float current_energy;
     float scaling_factor; 
     Size max_number; 

     Atom *atom_ptr; 
     Size force_update_freq; 
     Size iteration;

     vector<Atom *>::iterator atom_it;
     vector<Aux_Factors>::iterator factor_it; 

     // determine the largest value for the iteration counter
     max_number = number_of_iteration_ + iterations;

     // First check whether the  force field and the MD instance
     // are valid
     if(valid_ == false || force_field_ptr_ == 0 ||
                           force_field_ptr_->isValid() == false)
       {
       Log.level(LogStream::ERROR) << "MD simulation not possible! " 
                                   << "MD class is  not valid." << endl; 
       return; 
       }

     // pre-calculate some needed factors
     calculateFactors(); 


     // make sure that the MD simulation operates on the same set of atoms
     // as the forcefield  does (this may have changed since setup was called)
     atom_vector_ = force_field_ptr_->getAtoms();


     // Get the frequency for updating the Force Field pair lists
     force_update_freq = force_field_ptr_->getUpdateFrequency(); 

     // If the simulation runs with periodic boundary conditions, update the
     // list and position of molecules
     if(force_field_ptr_->periodic_boundary.isEnabled() == true)
       force_field_ptr_->periodic_boundary.updateMolecules(); 

     // Calculate the forces at the beginning of the simulation
     force_field_ptr_->updateForces();


     // This is the main loop of the MD simulation. The Velocity-Verlet method
     // is used for the propagation of atomic positions  and velocities 
     // In addition, the temperature is kept roughly constant by applying 
     // Berendsen's velocity rescaling method. 
     for(iteration = number_of_iteration_; iteration < max_number; iteration++)
       {
       // The force field data structures must be  updated regularly
       if(iteration % force_update_freq == 0)
         {
         force_field_ptr_->update(); 
         }

       // If the simulation runs with periodic boundary conditions, update the
       // list and position of molecules
       if(force_field_ptr_->periodic_boundary.isEnabled() == true)
        force_field_ptr_->periodic_boundary.updateMolecules(); 


     // In regular intervals, calculate and  output the current energy
     if(iteration % energy_output_frequency_ == 0)
       {
       // update the current values for energy and temperature and 
       // output them 
       current_energy = force_field_ptr_->updateEnergy();
       updateInstantaneousTemperature(); 

       Log.level(LogStream::INFORMATION) 
               << "CanonicalMD simulation System has potential energy "
               << current_energy << " at time " 
               << current_time_ + iteration * time_step_ << endl; 
       Log.level(LogStream::INFORMATION) 
               << "CanonicalMD simulation System has temperature  "
               << current_temperature_ << " at time " 
               << current_time_ + iteration * time_step_ << endl; 
       }

      // The new velocities calculated a few lines further below will be
      // rescaled by a certain factor. We use the temperature from the previous
      // iteration for computing the factor. 
      updateInstantaneousTemperature(); 

      if(bath_relaxation_time_ == 0 || current_temperature_ == 0)
        {
        Log.level(LogStream::ERROR) << " Attempted division by zero in line "
                                    << __LINE__ << ", file " << __FILE__ << endl;
        Log.level(LogStream::ERROR) << " Program is aborted. " << endl << endl; 
        Log.level(LogStream::ERROR) << " bath_relaxation_time_ = " << bath_relaxation_time_
                                    << endl; 
        Log.level(LogStream::ERROR) << " current_temperature_ = " << current_temperature_
                                    << endl; 

        if(current_temperature_ == 0)
          {
          Log.level(LogStream::ERROR) << " Make sure that every atom has an initial velocity."
                                      << endl; 
          }

        exit(0); 
        }


      scaling_factor = sqrt(1 + time_step_ / bath_relaxation_time_  
                         * (reference_temperature_ / current_temperature_ - 1.0));  

      // Calculate new atomic positions and new tentative velocities 
      for(atom_it = atom_vector_.begin(), 
          factor_it = mass_factor_.begin();
                        atom_it != atom_vector_.end(); 
                                              ++atom_it, ++factor_it)
        {
        atom_ptr = *atom_it; 

        // First calculate the new atomic position
        // x(t+1) = x(t) + time_step_ * v(t) + time_step_^2/(2*mass) * F(t)
        atom_ptr->setPosition(atom_ptr->getPosition() 
                             + time_step_ * atom_ptr->getVelocity()
                             + factor_it->factor1 * atom_ptr->getForce()); 

        // calculate a tentative  velocity 'v_tent' for the next iteration
        // v_tent(t+1) = v(t) + time_step_ / (2 * mass) * F(t)
        atom_ptr->setVelocity(atom_ptr->getVelocity()
                                + factor_it->factor2 * atom_ptr->getForce()); 

        } // next atom 



        // Determine the forces for the next iteration
        force_field_ptr_->updateForces();

        // Calculate the final velocity for the next iteration
        // and rescale it in order to keep the temperature constant 
        for(atom_it = atom_vector_.begin(),
          factor_it = mass_factor_.begin();
                        atom_it != atom_vector_.end();
                                              ++atom_it, ++factor_it)

          {
          atom_ptr = *atom_it; 
          atom_ptr->setVelocity(scaling_factor * (atom_ptr->getVelocity()
                                 + factor_it->factor2 * atom_ptr->getForce()) );
          } // next atom

         // Take a snapshot in regular intervals if desired
         if(snapshot_manager_ptr_ != 0 && iteration % snapshot_frequency_ == 0)
           {
           snapshot_manager_ptr_->takeSnapShot();
           }

       } // next iteration 


     // The simulation is finished. Update the current time and temperature  
     current_time_ += iterations * time_step_; 

     force_field_ptr_->updateEnergy();
     updateInstantaneousTemperature(); 

     } // end of simulateIterations() 



  } // end of namespace Ball

