// $Id: snapShot.h,v 1.6 2000/03/28 15:29:25 oliver Exp $
// This file contains the definitions of the classes 
// SnapshotManager and Snapshot. 
// They can be used to obtain snapshots from an MD simulation or an energy 
// minimisation.
// A snapshot contains atom positions, velocities, forces and potential energy. 


#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#define BALL_MOLMEC_COMMON_SNAPSHOT_H
 

// Include all necessary BALL header files
#ifndef BALL_COMMON_H
#       include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#       include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#       include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#       include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#       include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#       include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETER_H
#       include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#       include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_MOLMEC_COMMON_PERIODIC_BOUNDARY_H
#       include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#       include <BALL/MOLMEC/COMMON/forceField.h>
#endif


// basic includes
#include <iostream> 
#include <fstream> 
#include <stdio.h>

// STL include commands
#include <vector> 

namespace BALL
{
  // This class contains the actual data of a single snapshot
  struct SnapShot
  {
    /** @name Constructors and Destructors
    */
    //@{
    BALL_CREATE(SnapShot)

    /** The default constructor. It creates an empty SnapShot object. 
    */
    SnapShot(); 

    /** The constructor expects the number of atoms in the system and
        reserves enough memory. 
    */
    SnapShot(const Size no_of_atoms); 

    /** The copy constructor 
    */
    SnapShot(const SnapShot &rhs, bool deep = true); 

    /** The destructor 
    */
    virtual ~SnapShot(); 
    //@}

    /** @name Friends 
    */
    //@{
    /** The output operator << for output streams 
    */
    friend std::fstream& operator<<(std::fstream& os, const SnapShot&  snap_shot); 

    /** The  input operator >> for reading a snapshot object from a file
    */
    friend std::fstream& operator>>(std::fstream& in, SnapShot& snap_shot);
    //@}

    /** @name Assignments and operators
    */
    //@{ 
    /** The assignment operator 
    */
    SnapShot &operator=(const SnapShot &rhs); 
    //@}

    /** @name Public accessors
    */
    //@{
    /** Indicates if the snapshot object is ready for use
        Just for consistency 
    */
    bool isValid() 
      { return valid; } 
    //@}

    /** @name  Public Attributes
    */
    //@{
    /** Indicates whether the snapshot object is ready for use
    */
    bool valid;   

    /** The index of the current snapshot object. Valid indices start at 1.
    */
    Size index; 

    /** The total number of bytes needed for writing the snapshot 
        to a file 
    */
    Size total_length; 

    /** The number of bytes needed for writing the snapshot
        data section to a file 
    */
    Size data_length; 


    /** The number of atoms of the underlying system. The system being
        snapshot must always be the same, especially with respect to
        the number of atoms. 
    */
    Size no_of_atoms; 



    /** The potential energy in the system when the snapshot is taken.
        Only contributions from selected atoms are considered. 
    */
    double potential_energy; 

    /** The  kinetic energy due to electrostatic interactions. 
        Only contributions from selected atoms are considered. 
    */
    double kinetic_energy; 

    /** An STL vector with the positions of all atoms 
    */
    vector<Vector3> position;

    /** An STL vector with the velocities of all atoms 
    */
    vector<Vector3> velocity;

    /** An STL vector with the forces of all atoms 
    */
    vector<Vector3> force;
    //@}

    }; // end of struct Snapshot 





  // This class manages a list of single snapshot objects 
  // Snapshots are numbered starting with 1 
  class SnapShotManager
    {
    public:

    //  Local class for handling options
    struct Option
      {
      /** After how many snapshots shall  they be flushed to the hard disk 
      */
      static const char *FLUSH_TO_DISK_FREQUENCY; 
      };

     // Local class for handling default values for the options
     struct Default
       {
       /** After how many snapshots shall  they be flushed to the hard disk
       */
       static  const unsigned int FLUSH_TO_DISK_FREQUENCY; 
       }; 


    /** @name Constructors and Destructors
    */

    //@{
    BALL_CREATE(SnapShotManager)

    // The default constructor which does nothing at all 
    SnapShotManager(); 

    /** This constructor expects a valid system and a valid force field
        and the name of a snapshot file
    */
    SnapShotManager(System &my_system, ForceField &my_force_field,
                            char *my_snapshot_file); 

    /** This constructor expects a valid system and a valid force field
        and the name of a snapshot file and options 
    */
    SnapShotManager(System &my_system, ForceField &my_force_field,
                           char *my_snapshot_file, Options &myoptions);


    /** The copy constructor 
    */
    SnapShotManager(const  SnapShotManager &rhs, bool deep = true);

    /** The destructor
    */
    virtual ~SnapShotManager(); 

    //@}

    /** @name Setup methods
    */

    //@{
    /** The setup method does all preparations for using the SnapshotManager
    */
    virtual bool setup( System &my_system, ForceField &my_force_field,
                             char *my_snapshot_file, Options &myoptions);

    //@}

    /** @name Assignments and operators
    */

    //@{
    /** The assignment operator.  BAUSTELLE Am besten sperren?
    */
    SnapShotManager &operator=(const SnapShotManager &rhs);

    //@}


    /** @name Debugging and diagnostics
    */

    //@{
    /** Is the SnapshotManager ready for use?
    */
    virtual bool isValid() const { return valid_; } ;
    //@}



    /** @name Accessors
    */

    //@{
    /** This method takes a snapshot of the system's current state and stores
        it in main memory. If there is not sufficient space, the snapshots collected
        so far are flushed to hard disk. The first snapshot taken has index 1.
    */
    virtual void takeSnapShot(); 

    /** This method writes all snapshots taken so far to hard disk
    */
    virtual void flushToDisk(); 

    /** This method writes all snapshots taken so far to hard disk
        Added for convenience as it does the same as flushToDisk() 
    */
    virtual void close(); 

    /** This method returns the total number of snapshots (sum of
        snapshots in memory and on disk) 
    */ 
    virtual Size getNumberOfSnapShots() const {return snapshot_counter_;} 

    /** This method returns the snapshot object of the given index 
        If it does not find the item, an invalid and empty snapshot object is returned. 
    */ 
    virtual SnapShot getSnapShot(Size); 

    /** This method returns the snapshot object of the given index 
        transformed into a System object. This is useful for displaying
        the snapshot via BALLVIEW. 
        If it does not find the item, an empty System object is returned
    */ 
    virtual System getSnapShotAsSystem(Size); 

    //@}

    public:
    /** @name Public Attributes
    */
    //@{
    /** The available options for this class
    */
    Options options; 
    //@} 

    protected:
    /*_ @name  Protected Attributes
    */
    //@{

    // Local class for the snapshot fileheader
    struct  SnapShotHeader
      {
      char Title[20]; 
      bool valid; 
      Size no_of_snapshots;
      Size start_position;      
      Size reserved1 ;
      Size reserved2 ;
      Size reserved3 ;
      Size reserved4 ;
      Size reserved5 ;

      // The constructor
      SnapShotHeader() 
          { sprintf(Title,"BALL SNAPSHOT      ");
            valid = true;
            no_of_snapshots = 0; 
            reserved1 = 99 ; reserved2 = 99;
            reserved3 = 99; reserved4 = 99 ; reserved5 = 99; 
          }
      }; 

    /*_  This status flag indicates if the  class is ready for use
    */
    bool valid_;

    /*_ The frequency of  saving snapshots in memory to disk
    */
    Size flush_to_disk_freq_; 

    /*_ The system to which the SnapshotManager is bound
    */
    System *system_ptr_; 

    /*_ The force field of the current system
    */
    ForceField *force_field_ptr_; 

    /*_ The name of the file used for storing snapshots
    */
    char snapshot_filename_[200];

    /*_ The header of the snapshot file
    */
    class SnapShotHeader header_;

    /*_ The total number of snapshots 
    */
    Size snapshot_counter_;

    /*_ the number of atoms in the system
    */
    Size no_of_atoms_; 


    /*_ A vector containing those snapshot objects currently in 
        memory
    */
    vector<SnapShot *> snapshot_list_; 

    //@}

    protected:
    /*_ Protected methods
    */
    //@{
    /*_ This method reads the header of a snapshot file
    */
    bool readHeader(SnapShotHeader &header,const char * filename);

    /*_ This method writes the header of a snapshot file
    */
    bool writeHeader(SnapShotHeader &header,const char * filename); 

    /*_ Calculate the kinetic energy 
    */
    double calculateKineticEnergy();

    //@}

    };  // end of class SnapshotManager 

  



  }  // end of namespace BALL  

#endif 
