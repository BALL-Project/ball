// $Id: microCanonicalMD.C,v 1.3 2000/01/10 15:51:13 oliver Exp $

#include <BALL/MOLMEC/MDSIMULATION/microCanonicalMD.h>

namespace BALL 
  {
  using namespace std; 

  // The default constructor with no arguments
  MicroCanonicalMD::MicroCanonicalMD() : MolecularDynamics()
   {
   valid_ = false;  
   }


  // This constructor uses the given force field 
  MicroCanonicalMD::MicroCanonicalMD(ForceField &myforcefield) 
              : MolecularDynamics(myforcefield)
   {
   // the user does not want to take snapshots. 
   // Create a dummy manager 
   SnapShotManager tmp; 
   valid_ = setup(myforcefield,&tmp); 
   }

  // This constructor uses the given force field and snapshot manager 
  MicroCanonicalMD::MicroCanonicalMD(ForceField &myforcefield,SnapShotManager *ssm) 
              : MolecularDynamics(myforcefield)
   {
   valid_ = setup(myforcefield,ssm); 
   }

  // This constructor uses the given force field and options 
  MicroCanonicalMD::MicroCanonicalMD(ForceField &myforcefield,
             SnapShotManager *ssm,
             const Options &myoptions)  
             : MolecularDynamics(myforcefield)
   {
   valid_ = setup(myforcefield,ssm, myoptions); 
   }

   // The destructor
   MicroCanonicalMD::~MicroCanonicalMD()
     {
     // Nothing to do 
     }

   // This method does the general setup. 
   bool MicroCanonicalMD::setup(ForceField &myforcefield,SnapShotManager *ssm)
     {
     // No specific options have been named -> we use the force field's options
     valid_ = setup(myforcefield, ssm, myforcefield.options); 
     
     return valid_; 
     }

   bool MicroCanonicalMD::setup(ForceField &myforcefield, SnapShotManager *ssm, const Options &myoptions)
     {
     // First check whether the force field is valid. If not, then it is useless
    // to do anything here.
    if(myforcefield.isValid() == false)
      {
      // The setup has failed for some reason. Output an error message.
      Log.level(LogStream::ERROR) <<  
                         "Setup of instance of class 'MicroCanonical' has failed"
                                 << endl;

      valid_ = false;
      return false;
      }

     // call the base class setup method
     valid_ = MolecularDynamics::setup(myforcefield,ssm,myoptions); 

     if(valid_ == false)
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

     vector<Atom *>::iterator it;
     Aux_Factors item;
     Atom *atom_ptr;

     for(it = atom_vector_.begin(); it != atom_vector_.end(); ++it)
       {
       // Factor1 = time_step_ * time_step_ / (2 * mass)
       // Factor2 = time_step_ / (2 * mass)
       // Factors must be scaled by 6.022 * 10^12 to adjust units 
       atom_ptr = *it;
       item.factor2 = Constants::AVOGADRO / 1e23 * 1e12 * 
                      0.5 * time_step_ / atom_ptr->getElement().getAtomicWeight();
       item.factor1 = item.factor2 * time_step_;
       mass_factor_.push_back(item);
       }

     } // end of ' calculateFactors' 


   // This method performs additional setup preparations in addition 
   // to those done in MolecularDynamics::setup 
   bool MicroCanonicalMD::specificSetup()
     {
     if(!valid_)
       return false; 

     // Nothing more to do at  the moment 
     return true; 
     }

   // The copy constructor 
   MicroCanonicalMD::MicroCanonicalMD(const MicroCanonicalMD &rhs, bool deep)
       : MolecularDynamics(rhs,deep)
     {
     // copy class specific variables 
     mass_factor_ = rhs.mass_factor_;
     }

   // The assignment operator 
   MicroCanonicalMD &MicroCanonicalMD::operator=(const MicroCanonicalMD &rhs)
     {
     mass_factor_ = rhs.mass_factor_; 

     // call the assignment operator of the base class
     this->MolecularDynamics::operator=(rhs); 

     return *this; 
     }

   // This method does the actual simulation stuff
   // It runs for the number of iterations currently stored in
   // 'number_of_iterations_' 
   void MicroCanonicalMD::simulate(bool restart) 
     {
     simulateIterations(maximal_number_of_iterations_,restart); 
     }


   // This method does the actual simulation stuff
   // It runs for the indicated simulation time in picoseconds 
   void MicroCanonicalMD::simulateTime(float simulation_time,bool restart) 
     {
     Size number;

     // determine the number  of iterations and call 'simulate'
     if(valid_)
       {
       number = static_cast<Size> (simulation_time / time_step_);
       simulateIterations(number,restart);
       }
     }


   // This method does the actual simulation stuff
   // It runs for the indicated number of iterations           

   // Restart feature is not used at the moment!
   void MicroCanonicalMD::simulateIterations(Size iterations,bool restart) 
     {
     // local variables
     float current_energy;
     Size max_number; 

     Atom *atom_ptr; 
     Size force_update_freq; 
     Size iteration;

     vector<Atom *>::iterator atom_it;
     vector<Aux_Factors>::iterator factor_it; 


     // determine the largest value for the iteration counter 
     max_number = number_of_iteration_ + iterations;  


     // pre-calculate some needed factors
     calculateFactors();

     // make sure that the MD simulation operates on the same set of atoms
     // as the forcefield  does (this may have changed since setup was called)
     atom_vector_ = force_field_ptr_->getAtoms(); 

    
     // First check whether the  force field and the MD instance
     // are valid
     if(valid_ == false || force_field_ptr_ == 0 ||
                           force_field_ptr_->isValid() == false)
       {
       Log.level(LogStream::ERROR) << "MD simulation not possible! " 
                                   << "MD class is  not valid." << endl; 
       return; 
       }

     
     // Get the frequency for updating the Force Field pair lists
     force_update_freq = force_field_ptr_->getUpdateFrequency(); 

     // If the simulation runs with periodic boundary conditions, update the
     // list and position of molecules
     if(force_field_ptr_->periodic_boundary.isEnabled() == true)
       force_field_ptr_->periodic_boundary.updateMolecules(); 

     // Calculate the forces at the beginning of the simulation
     force_field_ptr_->updateForces();

// DEBUG
        force_field_ptr_->updateEnergy(); // nur fuer debugging



     // This is the main loop of the MD simulation. The Velocity-Verlet method
     // is used for the propagation of atomic positions  and velocities 
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
         current_energy = force_field_ptr_->updateEnergy();
         updateInstantaneousTemperature(); 

         Log.level(LogStream::INFORMATION) 
               << "Microcanonical MD simulation System has potential energy "
               << current_energy << " at time " 
               << current_time_ + (float)iteration * time_step_ << endl; 

         Log.level(LogStream::INFORMATION)
               << "MicroCanonicalMD simulation System has temperature  "
               << current_temperature_ << " at time "
               << current_time_ + (float)iteration * time_step_ << endl;
         }

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

       for(atom_it = atom_vector_.begin(),
           factor_it = mass_factor_.begin() ; 
                        atom_it != atom_vector_.end();
                                         ++atom_it, ++factor_it)
           {
           atom_ptr = *atom_it; 
 
           // Calculate the final velocity for the next iteration
           atom_ptr->setVelocity(atom_ptr->getVelocity()
                                  + factor_it->factor2 * atom_ptr->getForce());
           } // next atom

       // Take a snapshot in regular intervals if desired              
       if(snapshot_manager_ptr_ != 0 && iteration % snapshot_frequency_ == 0)
         {
         snapshot_manager_ptr_->takeSnapShot(); 
         }

       } // next iteration 

     // update the current time
     current_time_ += (float)iterations * time_step_; 

     // update the current temperature in the system
     force_field_ptr_->updateEnergy(); 
     updateInstantaneousTemperature(); 

     } // end of simulateIterations() 



  } // end of namespace Ball

