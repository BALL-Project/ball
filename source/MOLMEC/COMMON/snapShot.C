// $Id: snapShot.C,v 1.1 1999/12/17 18:31:40 pmueller Exp $

#include <BALL/MOLMEC/COMMON/snapShot.h>
#include <BALL/CONCEPT/XDRPersistenceManager.h> 
#include <algorithm> 

using namespace std; 


namespace BALL
  {
  // Definition of class-specific options and default values
  const char *SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY =
                             "Flush_to_disk_frequency";

  const unsigned int SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY = 100;

  // The default constructor of the SnapShot class. 
  SnapShot::SnapShot()
    {
    valid = false; 
    index = 0;
    }

   // The constructor of this class expects the number of atoms in the system
   // and reserves the necessary amount of memory for the data
   SnapShot::SnapShot(Size number)
      {
      Vector3 tmp;

      no_of_atoms = number;  
      valid = true; 
      index = 0;

      data_length = 9 * no_of_atoms * sizeof(tmp.x) + 2 * sizeof(potential_energy);
      total_length = data_length + sizeof(no_of_atoms) + sizeof(valid) +  sizeof(index) +
                        sizeof(data_length) + sizeof(total_length); 

      position.reserve(number);
      velocity.reserve(number);
      force.reserve(number);
      potential_energy = 0; 
      kinetic_energy = 0; 
      }

    // The  destructor just deletes all STL components (superfluous and just
    // for clarity) 
    SnapShot::~SnapShot()
      {
      position.clear();
      velocity.clear();
      force.clear(); 
      }


  // The copy constructor of the SnapShot class. 
  SnapShot::SnapShot(const SnapShot &rhs, bool deep)
    {
    valid = rhs.valid; 
    index = rhs.index;
    no_of_atoms = rhs.no_of_atoms;
    data_length = rhs.data_length;
    total_length = rhs.total_length;
    potential_energy = rhs.potential_energy;
    kinetic_energy = rhs.kinetic_energy;
    position = rhs.position;
    velocity = rhs.velocity;
    force = rhs.force; 
    }

  // The assignment operator 
  SnapShot &SnapShot::operator=(const SnapShot &rhs)
    {
    valid = rhs.valid; 
    index = rhs.index;
    no_of_atoms = rhs.no_of_atoms;
    data_length = rhs.data_length;
    total_length = rhs.total_length;
    potential_energy = rhs.potential_energy;
    kinetic_energy = rhs.kinetic_energy;
    position = rhs.position;
    velocity = rhs.velocity;
    force = rhs.force; 

    return *this; 
    }

  // The constructor of the SnapshotManager.
  // It expects a valid system and force field, a filename  of a snapshot file.
  // If the file does not exist yet, it will be created and a header consisting
  // of the system and some internal information will be written to it.
  // It the file is already existent, the header will read in and compared
  // with the given system. They must match. Subsequent snapshots will
  // be appended to the file.
  SnapShotManager::SnapShotManager(System &my_system,
                                   ForceField &my_forcefield,
                                   char  *my_snapshot_file) 
    {
    // create empty options and call the setup method
    Options options; 
    valid_ = setup(my_system,my_forcefield,my_snapshot_file,options);
    }


  // The constructor of the SnapshotManager.
  // It expects a valid system and force field, a filename  of a snapshot file. 
  // and options. 
  // If the file does not exist yet, it will be created and a header consisting
  // of the system and some internal information will be written to it.
  // It the file is already existent, the header will read in and compared
  // with the given system. They must match. Subsequent snapshots will 
  // be appended to the file. 
  SnapShotManager::SnapShotManager(System &my_system, 
                                   ForceField &my_forcefield,
                                   char  *my_snapshot_file,
                                   Options &myoptions)
    {
    // simply call the setup method
    valid_ = setup(my_system,my_forcefield,my_snapshot_file,myoptions);
    }

   // A simple constructor for SnapShotManager, which does basically nothing 
   SnapShotManager::SnapShotManager()
     {
     valid_ = false; 
     force_field_ptr_ = 0;
     system_ptr_ = 0;
     snapshot_counter_ = 0; 
     }



   // The copy constructor of the SnapShotManager
   // All data will be copied. But the copy will flush snapshots to the same
   // file as the original instance and it will use the same system and force field!
   SnapShotManager::SnapShotManager(const  SnapShotManager &rhs, bool deep )
     {
     // local variables
     vector<SnapShot *>::iterator it;
     SnapShot *tmp_ptr;

     options = rhs.options;
     valid_ = rhs.valid_; 
     flush_to_disk_freq_ = rhs.flush_to_disk_freq_;
     system_ptr_ = rhs.system_ptr_; 
     force_field_ptr_ = rhs.force_field_ptr_; 
     sprintf(snapshot_filename_,rhs.snapshot_filename_); 
     header_ = rhs.header_; 
     snapshot_counter_ = rhs.snapshot_counter_; 


     // copy the actual snapshot data
     // the STL copy constructor will only create a copy of the
     // right size. The actual contents (=pointers) must be copied then:
     snapshot_list_ = rhs.snapshot_list_; 

     for(it = snapshot_list_.begin(); it != snapshot_list_.end(); ++it)
       {
       // create a new SnapShot object and copy all data from the original to it
       tmp_ptr = new SnapShot(no_of_atoms_);

       if(tmp_ptr == 0)
         {
         Log.level(LogStream::ERROR) 
                << "No valid copy of SnapShotManager possible because not enough memory!" 
                << endl; 

         valid_ = false; 
         return; 
         }
       else
         {
         *tmp_ptr = *(*it);
         *it = tmp_ptr; 
         }
       }
     }

    // BAUSTELLE : braucht man den ? Ist das sinnvoll?
    // The assignment copy constructor. The copy will operate on the same
    // system, force field AND snapshot file 
    SnapShotManager  &SnapShotManager::operator=(const SnapShotManager &rhs)
      {
      // local variables
      vector<SnapShot *>::iterator it;
      SnapShot *tmp_ptr;

      options = rhs.options;
      valid_ = rhs.valid_; 
      flush_to_disk_freq_ = rhs.flush_to_disk_freq_;
      system_ptr_ = rhs.system_ptr_; 
      force_field_ptr_ = rhs.force_field_ptr_; 
      sprintf(snapshot_filename_,rhs.snapshot_filename_); 
      header_ = rhs.header_; 
      snapshot_counter_ = rhs.snapshot_counter_; 
      snapshot_list_ = rhs.snapshot_list_; 


      for(it = snapshot_list_.begin(); it != snapshot_list_.end(); ++it)
       {
       // create a new SnapShot object and copy all data from the original to it
       tmp_ptr = new SnapShot(no_of_atoms_);

       if(tmp_ptr == 0)
         {
         Log.level(LogStream::ERROR)
                             << "No valid copy of SnapShotManager because not enough memory!"
                             << endl;
         valid_ = false;
         break; 
         }
       else
         {
         *tmp_ptr = *(*it);
         *it = tmp_ptr; 
         }
       }


      return *this; 
      }

   // The destructor of SnapShotManager 
   SnapShotManager::~SnapShotManager()
     {
     // Flush all remaining snapshots to disk
     flushToDisk(); 
     }


  // The setup method does the actual preparations
  bool SnapShotManager::setup(System &my_system, 
                              ForceField &my_forcefield,char *my_snapshot_file,
                              Options &myoptions)
    {
    // local variables
    bool result;

    vector<SnapShot *>::iterator it; 

    cout << "SnapShotManager::Setup wird aufgerufen" << endl;
    cout.flush();


    // delete the vector of pointers to SnapShot objects
    for(it = snapshot_list_.begin(); it != snapshot_list_.end(); ++it)
      {
      delete *it;
      }

    snapshot_list_.clear(); 

    // first store the current force field
    force_field_ptr_ = &my_forcefield; 

    if(force_field_ptr_ == 0 || force_field_ptr_->isValid() == false)
       {
       // no valid force field: no more to do
       valid_ = false;
       Log.level(LogStream::ERROR) << "No valid SnapShotManager because no valid force field!" 
                                   << endl; 
       return false; 
       }
      
    // get the current system      
    system_ptr_ = &my_system; 

    if(system_ptr_ != force_field_ptr_->getSystem())
      {
      Log.level(LogStream::ERROR) << "The given force field is not bound to the given system!" 
                                  << endl;
      Log.level(LogStream::WARNING) << "Setup of SnapShotManager has failed!"  << endl; 
      valid_ = false;
      return false;
      }

    if(system_ptr_ == 0 || system_ptr_->isValid() == false)
       {
       // no valid force field: no more to do
       Log.level(LogStream::WARNING) << "No valid SnapShotManager because no valid system!" 
                                   << endl; 
       valid_ = false;
       return false; 
       }

    // the number of atoms in the system
    no_of_atoms_ = system_ptr_->countAtoms(); 

    // set the options 
    options = myoptions;

    // Set class--specific options to their default  values if necessary
    options.setDefaultInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY,
                              SnapShotManager::Default::FLUSH_TO_DISK_FREQUENCY); 
    flush_to_disk_freq_ = options.getInteger(SnapShotManager::Option::FLUSH_TO_DISK_FREQUENCY); 

    // The name of the output file
    sprintf(snapshot_filename_,my_snapshot_file); 

    // we start out with zero snapshots 
    snapshot_counter_ = 0;

    fstream input_file;

    // try to open the file in read-mode
    input_file.open(snapshot_filename_,ios::in|ios::binary);

    if(!input_file)
      {
      // the file does not exist. Create a new snapshot file 
      result = writeHeader(header_,snapshot_filename_);
      }
    else
      {
      //  the file exists. Try to read the header in it 
      result = readHeader(header_,snapshot_filename_);
      }

    snapshot_counter_ = header_.no_of_snapshots; 

      cout << "Anzahl snapshots nach setup ist " << snapshot_counter_ << endl; 

    if(result == false)
      {
      // The file does not contain a valid header or is not writable
      Log.level(LogStream::ERROR) << "Setup of SnapShotManager has failed." 
                                  << "Invalid header or wrong access permission" 
                                  << snapshot_filename_ << endl;
      valid_ = false;
      return false; 
      }
    else
      {
      // setup was successful 
      valid_ = true;
      return true;
      }

    } // end of SnapShotManager::setup



    // This method reads the header of a snapshot file
    bool SnapShotManager::readHeader(SnapShotHeader &header,const char *filename)
      {
      // local variables
      fstream input_file; 

      // open the file in read-mode
      input_file.open(filename,ios::in|ios::binary); 

      if(!input_file)
        {
        // something went wrong   
        return false; 
        }


      // read the header of the snapshot file
      input_file.read(header.Title,sizeof(header.Title));

      // if the first line is not what we expect then this is not the correct file format
      if(strcmp(header.Title,"BALL SNAPSHOT      ") != 0)
        {
        Log.level(LogStream::ERROR) << "File " << filename << " is no valid Snapshot file!" << endl;
        input_file.close(); 
        return false; 
        }

      input_file.read((char *) &header.valid,sizeof(header.valid));
      input_file.read((char *) &header.no_of_snapshots,sizeof(header.no_of_snapshots));
      input_file.read((char *) &header.start_position,sizeof(header.start_position));
      input_file.read((char *) &header.reserved1,sizeof(header.reserved1));
      input_file.read((char *) &header.reserved2,sizeof(header.reserved2));
      input_file.read((char *) &header.reserved3,sizeof(header.reserved3));
      input_file.read((char *) &header.reserved4,sizeof(header.reserved4));
      input_file.read((char *) &header.reserved5,sizeof(header.reserved5));


      cout << "gelesener Header ist " << endl << "Title " << header.Title << endl <<
                                                 "valid : " << header.valid << endl << 
                                                 "no. of snapshots " <<
                                                 header.no_of_snapshots << endl << 
                                                 "start pos : " << 
                                                 header.start_position << endl << 
                                                 header.reserved1 << endl << 
                                                 header.reserved2 << endl << 
                                                 header.reserved3 << endl << 
                                                 header.reserved4 << endl << 
                                                 header.reserved5 << endl <<  endl; 

      // now read in the system from this file
      System  disk_system;
      XDRPersistenceManager pm;
      PersistentObject *po_ptr;
      System *sys_ptr; 

      pm.setIstream(input_file);
      po_ptr = pm.readObject(); 
      input_file.close(); 

      // the persistance manager class returns a pointer
      // that we must cast to the correct type
      sys_ptr = dynamic_cast< System *>(po_ptr);

      if(sys_ptr == 0)
        {
        // the dynamic cast has failed
        Log.level(LogStream::ERROR) << "No suitable system found in file "
                                    << filename << " !" << endl;
        return false; 
        }

      disk_system =  *sys_ptr; 


      // do a superficial check to see if the two systems are equivalent 
      if(disk_system.countAtoms() != system_ptr_->countAtoms() ||
         disk_system.countFragments() != system_ptr_->countFragments())
        {
        Log.level(LogStream::ERROR) << "System in file " 
                                    << filename << " is different from system in"   
                                    << " main memory!" << endl;
        Log.level(LogStream::ERROR) << "System in memory has " 
                                    << system_ptr_->countAtoms()   
                                    << " atoms and " << system_ptr_->countFragments()  
                                    << " fragments" << endl  
                                    << " system in snapshot file has " 
                                    << disk_system.countAtoms() 
                                    << " atoms and "  << disk_system.countFragments() 
                                    << " fragments" 
                                    << endl;

        return false; 
        }

      return header.valid; 
      }

    
    // This method writes the header of a snapshot file
    bool SnapShotManager::writeHeader(SnapShotHeader &header,  const char *filename)
      {
      fstream output_file; 

      Size position; 


      // try to open the file in update mode
      output_file.open(filename,ios::in|ios::out|ios::binary); // egcs does not know ios_base

      if(!output_file)
        {
        // the file cannot be opened
        return false; 
        }


      // overwrite the old header 
      output_file.seekp(ios::beg);   // egcs does not know ios_base! 

      output_file.write(header.Title,sizeof(header.Title)); 

      output_file.write((char *) &header.valid,sizeof(header.valid)); 
      output_file.write((char *) &header.no_of_snapshots,sizeof(header.no_of_snapshots)); 

      // we will later on save the offset here for appending snapshots 
      position = output_file.tellp();
      output_file.write((char *) &header.start_position,sizeof(header.start_position)); 

      output_file.write((char *) &header.reserved1,sizeof(header.reserved1)); 
      output_file.write((char *) &header.reserved2,sizeof(header.reserved2)); 
      output_file.write((char *) &header.reserved3,sizeof(header.reserved3)); 
      output_file.write((char *) &header.reserved4,sizeof(header.reserved4)); 
      output_file.write((char *) &header.reserved5,sizeof(header.reserved5)); 


      // now write the system to the file 
      XDRPersistenceManager pm;
      pm.setOstream(output_file);
      *system_ptr_ >> pm; 


      // remember where the first snapshot entry will be written 
      header.start_position = output_file.tellp();                    
      output_file.seekp(position);
      output_file.write((char *) &header.start_position,sizeof(header.start_position));

      cout << "geschriebener Header ist " << endl << header.Title << endl <<
                                                 header.valid << endl << 
                                                 header.no_of_snapshots  << endl << 
                                                 header.start_position << endl << 
                                                 header.reserved1 << endl << 
                                                 header.reserved2 << endl << 
                                                 header.reserved3 << endl << 
                                                 header.reserved4 << endl << 
                                                 header.reserved5 << endl ;    

      output_file.close();             
      return true; 
      }
  

  // This method calculates the current kinetic energy of the system
  // (only selected atoms are considered) 
  float SnapShotManager::calculateKineticEnergy()
    {
    // First define some local variables 
    float sq_velocity; 
    float sum,result ; 

    vector<Atom *>::iterator atom_it;
    vector<Atom *> selected_atoms; 

    sum = 0.0; 
    selected_atoms = force_field_ptr_->getAtoms(); 

    // The current temperature (calculated as instantaneous kinetic energy)
    // If we use a periodic boundary box, then we use the molecules' centres of
    // gravity, otherwise we iterate directly over the individual atoms of the system 

    atom_it = selected_atoms.begin(); 

      if(force_field_ptr_->periodic_boundary.isEnabled() == true)
        {
        float molecule_mass = 0;
        float mass; 

        int no_of_molecules = 0; 
         
        Molecule *old,*current; 
        Vector3 centre_velocity;


        centre_velocity.x = centre_velocity.y = centre_velocity.z = 0;

        // Get the molecule of the first atom in the system 
        old = (*atom_it)->getMolecule(); 

        // Iterate over all atoms in the system and determine for every
        // molecule the velocity of its centre of mass
        // This velocity contributes to the overall temperature 
        vector<Atom *>::iterator end_it = selected_atoms.end();   

        while(atom_it != end_it)
          {
          // determine the molecule of the current atom
          current = (*atom_it)->getMolecule(); 

          if(current != old)
            {
            // This atom belongs to an other molecule than the 
            // previous atom -> all atoms of the previous 
            // molecule have been visited
            centre_velocity /= molecule_mass; 
            sq_velocity = centre_velocity * centre_velocity; 
            sum += molecule_mass * sq_velocity; 

            no_of_molecules++; 

            // reset some values for the new molecule
            molecule_mass = 0;
            centre_velocity.x = centre_velocity.y = centre_velocity.z = 0; 

            // the new molecule is now the old one
            old = current; 
            }
          
          // add the atom's velocity to the total velocity  
          // of the current molecule
          mass = (*atom_it)->getElement().getAtomicWeight(); 
          molecule_mass += mass; 
          centre_velocity += mass * (*atom_it)->getVelocity();


          // go on to the next atom
          ++atom_it; 
          }

         // The last molecule must be added as this is not done in the while-loop
         if(molecule_mass == 0)
           {
           // A strange molecule. Let the centre_velocity as it is
           }
         else
           {
           centre_velocity /= molecule_mass;
           }

        sq_velocity = centre_velocity * centre_velocity;
        sum += molecule_mass * sq_velocity;
        no_of_molecules++;

        // BAUSTELLE EINHEITEN? 
cout << "BAUSTELLE Einheiten in Zeile " << __LINE__ << "File " << __FILE__ << endl; 
        result = 0.5 * sum; 

        }
      else
        {
        // No periodic boundary conditions, i.e., just a few molecules 
        // are in the system
        int no_of_atoms = 0; 

        // Iterate over all atoms and calculate \sum m_i \cdot v_i^2  
        for(atom_it =  selected_atoms.begin(); 
                                     atom_it != selected_atoms.end(); ++atom_it)
          {
          no_of_atoms++; 
          sq_velocity = (*atom_it)->getVelocity() * (*atom_it)->getVelocity();  
          sum += (*atom_it)->getElement().getAtomicWeight() * sq_velocity;
          }
// BAUSTELLE Einheiten??  
cout << "BAUSTELLE Einheiten in Zeile " << __LINE__ << "File " << __FILE__ << endl; 
        result = 0.5 * sum; 
        }

      return result;  

    } // end of 'calculateKineticEnergy'            


  // This method takes a snapshot of the system's current state and stores it in
  // memory. If no memory is available or a maximum number of snapshots has been
  // taken, all snapshots so far are saved to disk 
  // The first snapshot taken (and with no other previous snapshots in a file)
  // has index 1. 
  void SnapShotManager::takeSnapShot()
    {
    // local variables
    AtomIterator atom_it; 

    if(valid_ == false)
      return; 

    // create a new Snapshot-Object
    SnapShot *snapshot_ptr;

    snapshot_ptr = new SnapShot(no_of_atoms_);

    
    if(snapshot_ptr == 0)               
      {
      // there was not enough memory available 
      // flush all snapshots taken so far to disk and try again
      flushToDisk(); 

      snapshot_ptr = new SnapShot(no_of_atoms_);

      if(snapshot_ptr == 0 || snapshot_ptr->valid == false)
        {
        // can't help but giving up
        Log.level(LogStream::ERROR) << "Not enough memory for taking Snapshots!" << endl;      
        return;
        }

      }

    // store all current positions, forces, velocities in the
    // snapshot object

    // This is the data section of the snapshot object 
    for(atom_it = system_ptr_->beginAtom(); atom_it != system_ptr_->endAtom(); ++atom_it)
      {
      snapshot_ptr->position.push_back(atom_it->getPosition());
      snapshot_ptr->velocity.push_back(atom_it->getVelocity());
      snapshot_ptr->force.push_back(atom_it->getForce());
      }

    // store the potential energies      
    snapshot_ptr->potential_energy = force_field_ptr_->getEnergy(); 

    // store the kinetic energy of all selected atoms in the system 
    // the current value must be calculated as it is not provided by
    // the force field 
    snapshot_ptr->kinetic_energy = calculateKineticEnergy(); 

    // These items are the admininistrative  data 
    // First store the index of this snapshot
    snapshot_ptr->index = snapshot_counter_ + 1; 
      
    // One more snapshot. Add it to the current list of snapshots
    snapshot_list_.push_back(snapshot_ptr); 
    snapshot_counter_++; 

    if(snapshot_list_.size() >= flush_to_disk_freq_)
      {
      // write all snapshots to disk in order to prevent memory overflow
      flushToDisk(); 
      }
    } // end of SnapShotManager::takeSnapShot() 


    // This method writes all snapshots in memory to the snapshot file on disk
    void SnapShotManager::flushToDisk()
      {
      // local variables
      bool result; 
      fstream output_file;
      vector<SnapShot *>::iterator it;


      // if no snapshots are in main memory, then there is nothing to do
      if(snapshot_list_.size() == 0 || valid_ == false)
         return; 


      // read the current header of the file and then write an updated file header
      // The header keeps the total number of snapshots in this file 
      result = readHeader(header_,snapshot_filename_);

      if(result == false)
        {
        Log.level(LogStream::ERROR) << "Cannot read header of snapshot file " <<
                                    snapshot_filename_ << endl;
        return; 
        }

      header_.no_of_snapshots = snapshot_counter_; 
      result = writeHeader(header_,snapshot_filename_);

      if(result == false)
        {
        Log.level(LogStream::ERROR) << "FlushToDisk of ShotManager has failed." 
                                    << "Cannot write to file " << snapshot_filename_ << endl;
        return; 
        }

      // open the output file and append all snapshots to the end of the file
      // When a snapshot has been written, it is deleted in main memory 
      output_file.open(snapshot_filename_,ios::app|ios::binary);


      if(!output_file)
        {
        Log.level(LogStream::ERROR) << "Cannot open file " << snapshot_filename_ << 
                                       " for appending snapshots! " << endl; 
        return; 
        }

      for(it = snapshot_list_.begin(); it != snapshot_list_.end(); ++it)
         {
         output_file << *(*it); 
         delete *it;
         }




      // clear the snapshot list
      snapshot_list_.clear(); 

      // close the output file
      output_file.close(); 

      } // end of SnapShotManager::flushToDisk()

    // This method does the same as flushToDisk(). Included for convenience and 
    // sloth of users only.
    void SnapShotManager::close()
      {
      flushToDisk();
      }


    // This method searches for a snapshot entry and transforms it into
    // a system object. This is useful for displaying, etc. 
    System SnapShotManager::getAsSystem(Size index)
      {
      SnapShot tmp;
      System cmp; 

      // Look for the snapshot in main memory and on disk
      tmp = getSnapShot(index);  

      if(tmp.valid == false)
        {
        // the search was not successful -> return an empty system
        return cmp; 
        }

      // Now transform the SnapShot object into a Composite object 
      // This is done by making a copy of the  underlying system object and
      // replacing all positions and velocities  by the current values of
      // the SnapShot object
      cmp = *system_ptr_; 

      AtomIterator atom_it;
      vector<Vector3>::iterator force_it,pos_it,vel_it;

      for(atom_it = cmp.beginAtom(), force_it = tmp.force.begin(),
                    pos_it = tmp.position.begin(),
                    vel_it = tmp.velocity.end(); 
                                          atom_it != cmp.endAtom(); 
                                          ++atom_it,++force_it,++pos_it,
                                          ++vel_it)
        {
        atom_it->setPosition(*pos_it);
        atom_it->setVelocity(*vel_it); 
        atom_it->setForce(*force_it);  
        }

      // The energy cannot be set as this belongs to the underlying force field, 
      // so this item will be ignored here 

      return cmp; 
      } // end of SnapShotManager::getSystem()




    // This method searches the given snaphot (by index) and returns a copy
    // of it.  
    // If it does not find the item, an invalid snapshot object is returned 
    SnapShot SnapShotManager::getSnapShot(Size index)
      {
      SnapShot tmp;                 

      // first look in main memory
      int pos = index - snapshot_counter_ + snapshot_list_.size() - 1; 

      if(pos >= 0 && pos < (int) snapshot_list_.size())
        {
        // one of the recent snapshots is wanted -> it should be in
        // main memory 
        if(snapshot_list_[pos]->index == index)
          {
          return *snapshot_list_[pos]; 
          }
        else
          {
          // something must be wrong with the data structure
          Log.level(LogStream::ERROR) << "Error in SnapShotManager::getSnapShot. " 
                                      << "Internal data structure must be corrupted! " << endl;
          return tmp; 
          }
        }

      if(pos >=  static_cast<int>(snapshot_list_.size()))
        {
        // the list in memory always contains the highest index. If the calculated 
        // position is beyond the current size, this means that no such snapshot
        // exists, neither in memory nor on disk 
        return tmp; 
        }
                

      // The item is not in main memory. 
      // we have to go the hard way and search the file on disk
      Size counter = 0;
      Size position; 

      fstream input_file(snapshot_filename_,ios::in|ios::binary);

      if(!input_file)
        {
        // the file cannot be opened
        Log.level(LogStream::ERROR) 
                << "Cannot open snapshot file " << snapshot_filename_ << endl;

        tmp.valid = false; 
        return tmp; 
        }


      // set the file pointer to the first entry
      input_file.seekp(header_.start_position);


      while(counter < header_.no_of_snapshots)
        {
        // read the index of the snapshot object and check if it is
        // the right one
        position = input_file.tellp(); 

        input_file.read((char *) &tmp.valid, sizeof(tmp.valid)); 
        input_file.read((char *) &tmp.index, sizeof(tmp.index)); 
        input_file.read((char *) &tmp.total_length, sizeof(tmp.total_length)); 

        // DEBUG
        cout << "lese von Disk Snapshot Nr.   " << tmp.index << endl; 
        cout << "                       valid " << tmp.valid << endl; 

        if(tmp.valid == true && tmp.index == index)
          {
          // the search has been successful 

          cout << "gefunden: lese Snapshot Nr. " << tmp.index << endl; 

          input_file.seekp(position); 
          input_file >> tmp; 

          input_file.close(); 
          return tmp; 
          }
        else
         {
         // skipt this  entry 
         input_file.seekp(position + tmp.total_length); 
         counter++; 
         }
        }
      
      input_file.close(); 

      // unsuccessful search
      tmp.valid = false;
      tmp.index = 0; 
      return tmp; 

      } // end of SnapShotManager::getSnapShot



    // The output operator << for writing SnapShot objects to an output filestream
    fstream &operator<<(fstream &os, const SnapShot &snap_shot)
      {
      // local variables
      vector<Vector3>::const_iterator it;
      Vector3 tmp; 
  

      // first write the simple values
      os.write((char *) &snap_shot.valid, sizeof(snap_shot.valid)); 
      os.write((char *) &snap_shot.index, sizeof(snap_shot.index)); 

      // write the total number of bytes that will be nessary for storing the object  
      os.write((char *) &snap_shot.total_length, sizeof(snap_shot.total_length)); 

      // write the number of bytes that will be nessary for storing the actual
      // data section of the snapshot object (potential energies plus positions,
      // velocities, and forces). 
      os.write((char *) &snap_shot.data_length, sizeof(snap_shot.data_length)); 

      // the number of atoms 
      os.write((char *) &snap_shot.no_of_atoms,sizeof(snap_shot.no_of_atoms)); 

      // here starts the data section 
      os.write((char *) &snap_shot.potential_energy, sizeof(snap_shot.potential_energy)); 
      os.write((char *) &snap_shot.kinetic_energy, sizeof(snap_shot.kinetic_energy)); 

      // Now output the positions of all atoms
      for(it = snap_shot.position.begin(); it != snap_shot.position.end(); ++it)
        {
        os.write((char *) &(it->x),sizeof(it->x)); 
        os.write((char *) &(it->y),sizeof(it->y)); 
        os.write((char *) &(it->z),sizeof(it->z)); 
        }

      // Now output the velocities of all atoms
      for(it = snap_shot.velocity.begin(); it != snap_shot.velocity.end(); ++it)
        {
        os.write((char *) &(it->x),sizeof(it->x)); 
        os.write((char *) &(it->y),sizeof(it->y)); 
        os.write((char *) &(it->z),sizeof(it->z)); 
        }

      // Now output the forces of all atoms
      for(it = snap_shot.force.begin(); it != snap_shot.force.end(); ++it)
        {
        os.write((char *) &(it->x),sizeof(it->x)); 
        os.write((char *) &(it->y),sizeof(it->y)); 
        os.write((char *) &(it->z),sizeof(it->z)); 
        }

      return os; 
      }

    // The input operator >> for reading a snapshot object from a file
    fstream &operator>>( fstream &in, SnapShot &snap_shot)
      {
      // local variables
      Vector3 tmp; 

      // first read the simple values
      in.read((char *) &snap_shot.valid, sizeof(snap_shot.valid)); 
      in.read((char *) &snap_shot.index, sizeof(snap_shot.index)); 

      in.read((char *) &snap_shot.total_length,sizeof(snap_shot.total_length)); 
      in.read((char *) &snap_shot.data_length,sizeof(snap_shot.data_length)); 

      in.read((char *) &snap_shot.no_of_atoms,sizeof(snap_shot.no_of_atoms)); 

      // now read in the potential energies 
      in.read((char *) &snap_shot.potential_energy,sizeof(snap_shot.potential_energy)); 
      in.read((char *) &snap_shot.kinetic_energy,sizeof(snap_shot.kinetic_energy)); 

      // now read all atom positions
      Size i;

      for(i = 1; i <= snap_shot.no_of_atoms; i++)
        {
        in.read((char *) &tmp.x,sizeof(tmp.x)); 
        in.read((char *) &tmp.y,sizeof(tmp.y)); 
        in.read((char *) &tmp.z,sizeof(tmp.z)); 

        snap_shot.position.push_back(tmp);
        }

      // now read all atom velocities
      for(i = 1; i <= snap_shot.no_of_atoms; i++)
        {
        in.read((char *) &tmp.x,sizeof(tmp.x)); 
        in.read((char *) &tmp.y,sizeof(tmp.y)); 
        in.read((char *) &tmp.z,sizeof(tmp.z)); 

        snap_shot.velocity.push_back(tmp);
        }

      // now read all atom forces
      for(i = 1; i <= snap_shot.no_of_atoms; i++)
        {
        in.read((char *) &tmp.x,sizeof(tmp.x)); 
        in.read((char *) &tmp.y,sizeof(tmp.y)); 
        in.read((char *) &tmp.z,sizeof(tmp.z)); 

        snap_shot.force.push_back(tmp);
        }

      return in; 
      }


  } // end of namespace BALL

