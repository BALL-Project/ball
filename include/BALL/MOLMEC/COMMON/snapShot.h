// $Id: snapShot.h,v 1.16 2001/03/07 22:19:44 amoll Exp $

// This file contains the definitions of the classes 
// SnapshotManager and Snapshot. 
// They can be used to obtain snapshots from an MD simulation or an energy 
// minimisation.
// A snapshot contains atom positions, velocities, forces and potential energy. 


#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#define BALL_MOLMEC_COMMON_SNAPSHOT_H
 
#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/common/exception.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_KERNEL_PTE_H
#	include <BALL/KERNEL/PTE.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETER_H
#	include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_MOLMEC_COMMON_PERIODIC_BOUNDARY_H
#	include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif


namespace BALL
{

	class TrajectoryFile;

	/** SnapShot class, containing the actual data of one single snapshot.
			It consists of the posistions, velocities and forces for each atom
			along with kinetic and potential energy of selected atoms. It is used
			by the \Ref{SnapShotManager}. \\
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/snapShot.h}
	*/
  class SnapShot
  {

		friend class SnapShotManager;
		
		public:

    BALL_CREATE(SnapShot)

    /// @name Constructors and Destructors
    //@{

    /** The default constructor. It creates an empty SnapShot object. 
    */
    SnapShot()
			throw();

		/// Copy constructor.
		SnapShot(const SnapShot& snapshot)
			throw();

    /** This constructor expects the number of atoms in the system and
        reserves enough memory. 
    */
    SnapShot(const Size no_of_atoms)
			throw();

    /// The destructor 
    virtual ~SnapShot()
			throw();

    //@}
    /// @name Assignment 
    //@{ 

    /// Assignment operator.
    const SnapShot& operator = (const SnapShot& snapshot)
			throw();

		/// Clear method.
		virtual void clear()
			throw();

    //@}
    /// @name Predicates
    //@{

		/// Equality operator
		bool operator == (const SnapShot& snapshot) const
			throw();

    /** Indicates validity of the SnapShot
				@return true if this instance is valid
    */
    bool isValid() const
			throw();

    //@}
		/// @name Accessors
		//@{

		/// Set the index attribute.
		void setIndex(Size index)
			throw();

		/// Get the index of this instance.
		Size getIndex() const
			throw();

		/// Set the number of atoms that are covered by this snapshot.
		void setNumberOfAtoms(Size number_of_atoms)
			throw();

		/// Get the number of atoms that are covered by this snapshot.
		Size getNumberOfAtoms() const
			throw();

		/// Get the number of bytes needed for writing the data of this instance 
		Size getDataLength() const
			throw();

		/// Get the total number of bytes needed for writing this instance 
		Size getTotalLength() const
			throw();

		/// Get the value for the potential energy of this snapshot's system
		double getPotentialEnergy() const
			throw();

		/// Get the value for the kinetic energy of this snapshot's system
		double getKineticEnergy() const
			throw();

		/// Specify all atom positions
		void setAtomPositions(const std::vector<Vector3>& atom_postions)
			throw();

		/// Get the vector containing the atom postions
		const std::vector<Vector3>& getAtomPositions() const
			throw();

		/// Specify all atom velocities
		void setAtomVelocitites(const std::vector<Vector3>& atom_velocities)
			throw();

		/// Get the vector containing the atom velocitites
		const std::vector<Vector3>& getAtomVelocities() const
			throw();

		/// Specify all atom forces
		void setAtomForces(const std::vector<Vector3>& atom_forces)
			throw();

		/// Get all atom forces
		const std::vector<Vector3>& getAtomForces() const
			throw();

		//@}

		protected:

    //_ @name Protected Attributes
    //@{

    //_ The index of the current snapshot object. Valid indices start at 1.
    Size index_; 

    /*_ The number of atoms of the underlying system. The system being
        snapshot must always be the same, especially with respect to
        the number of atoms. 
    */
    Size number_of_atoms_; 

    /*_ The potential energy in the system when the snapshot is taken.
        Only contributions from selected atoms are considered. 
    */
    double potential_energy_;

    /*_ The  kinetic energy due to electrostatic interactions. 
        Only contributions from selected atoms are considered. 
    */
    double kinetic_energy_; 

    /*_ The number of bytes needed for writing the snapshot
        data section to a file 
    */
    Size data_length_; 

    /*_ The total number of bytes needed for writing the snapshot 
        to a file 
    */
    Size total_length_;

    //_ An STL vector with the positions of all atoms 
    vector<Vector3> atom_positions_;

    //_ An STL vector with the velocities of all atoms 
    vector<Vector3> atom_velocities_;

    //_ An STL vector with the forces of all atoms 
    vector<Vector3> atom_forces_;

    //_ Indicates whether the snapshot object is valid
    bool valid_;

    //@}

	}; // Snapshot 


  /**	Snapshot management for MD simulations.
			This class manages a list of single snapshot objects.
			Snapshots are numbered starting with 1.	\\
			{\bf Definition:}\URL{BALL/MOLMEC/COMMON/snapShot.h}
	*/
  class SnapShotManager
  {

    public:

    ///  Local class for handling options
    struct Option
    {
      /** After how many snapshots shall they be flushed to the hard disk 
					@see Default::FLUSH_TO_DISK_FREQUENCY
					@param frequency integer
      */
      static const char* FLUSH_TO_DISK_FREQUENCY; 
    };

    /// Local class for handling default values for the options
    struct Default
    {
      /** This is the default value of the number of snapshots to be taken
					before writing them to hard disk. Default is ...
					@see Option::FLUSH_TO_DISK_FREQUENCY 
			*/
      static const unsigned int FLUSH_TO_DISK_FREQUENCY; 
    }; 


    BALL_CREATE(SnapShotManager)


    /// @name Constructors and Destructors
    //@{

    /// Default constructor
    SnapShotManager()
			throw();

    /** This constructor expects a valid system, a valid force field
				and the name of a snapshot file. If the {\tt overwrite} is true
				then any existing file of the given name will be overwritten,
				otherwise the new data will be appended, provided that the systems match. 
				@param my_system the system to bind this manager to
				@param my_force_field the force field that is bound to the system
				@param my_snapshot_file 
				@param overwrite
    */
    SnapShotManager
			(const System* my_system, const ForceField* my_force_field,
			 TrajectoryFile* my_snapshot_file, bool overwrite = true)
			throw();

    /** This constructor expects a valid system, a valid force field
				and the name of a snapshot file. If the {\tt overwrite} is true
				then any existing file of the given name will be overwritten,
				otherwise the new data will be appended, provided that the systems match. 
				@param my_system the system to bind this manager to
				@param my_force_field the force field that is bound to the system
				@param my_options
				@param filename the name of the snapshot file
				@param overwrite {\bf true}: overwrite existing snapshot file, 
												 {\bf false}: append to the file.
    */
    SnapShotManager	
			(const System* my_system, const ForceField* my_force_field,
			 const Options& my_options, TrajectoryFile* file, bool overwrite = true)
			throw();

    /// Copy constructor.
    SnapShotManager(const SnapShotManager& manager)
			throw();

    /// Destructor.
    virtual ~SnapShotManager()
			throw();

    //@}
    /// @name Setup methods
    //@{

		/** The setup method does all preparations necessary for using the
				SnapshotManager.
				@return true, if setup was succesful
    */
    virtual bool setup()
			throw();

    //@}
    /** @name Assignment
    */
    //@{

    /** The assignment operator.  
    */
    const SnapShotManager& operator = (const SnapShotManager& manager)
			throw();

		/// Clear method
		virtual void clear()
			throw();

    //@}
    /** @name Debugging and diagnostics
    */
    //@{

    /** Is the SnapshotManager ready for use?
    */
    virtual bool isValid() const
			throw();

    //@}
    /** @name Accessors
    */

    //@{

    /** Set the frequency for saving snapshots to hard disk. 
        Every 'number' iterations, a save will be  done.         
				@param number the number of snapshots to take before flushing to disk
    */
    void setFlushToDiskFrequency(Size number)
			throw();

    /** Get the current frequency for doing saves to hard disk
				@return the number of snapshots to take before flushing them to disk
    */
    Size getFlushToDiskFrequency() const
			throw();

    /** This method takes a snapshot of the system's current state and stores
				it in main memory. If there is not sufficient space, the snapshots
				collected so far are flushed to hard disk. The first snapshot taken
				has index 1.
    */
    virtual void takeSnapShot()
			throw(Exception::OutOfMemory);

    /** This method writes all snapshots taken so far to hard disk
    */
    virtual void flushToDisk()
			throw();

    /** This method returns the total number of snapshots (sum of
        snapshots in memory and on disk).
    */ 
    Size getNumberOfSnapShots() const 
			throw();

		/// Obtain the number of atoms covered by this SnapshotManager 
		Size getNumberOfAtoms() const
			throw();

    //@}
    /** @name Public Attributes
    */
    //@{

    /** The available options for this class.
    */
    Options options; 

    //@} 

    protected:

    /*_ @name Protected Attributes
    */
    //@{

    /*_ The system to which the SnapshotManager is bound
    */
    const System* system_ptr_; 

    /*_ The force field of the current system
    */
    const ForceField* force_field_ptr_; 

    /*_ The total number of snapshots 
    */
    Size snapshot_counter_;

		// Baustelle: Warum? Steht doch im System.
    /*_ the number of atoms in the system
    */
    Size number_of_atoms_; 

    /*_ A vector containing those snapshot objects currently in memory
    */
    vector<SnapShot> snapshot_list_; 

		/*_ The trajectory file where the data is saved in
		*/
		TrajectoryFile* trajectory_file_;

    /*_ The frequency of saving snapshots in memory to disk.
        After flush_to_disk_frequency_ iterations, a save is done. 
    */
		// BAUSTELLE: Gehoert das hierher? Macht das Åberhaupt Sinn? Werden denn
		// bei einem Flush alle bis dahin noch nicht gespeicherten Snapshots
		// gespeichert oder nur der letzte, also damit eine Art Zeitraffer
		// generiert?
    Size flush_to_disk_frequency_; 

		/*_
		*/
		Size buffer_counter_;

    /*_  This status flag indicates if the class is ready for use
    */
    bool valid_;

    //@}
    /*_ @name Protected methods
    */
    //@{

    /*_ Calculate the kinetic energy 
    */
    double calculateKineticEnergy_();

    //@}

	}; // end of class SnapshotManager 

} // end of namespace BALL  

#endif // BALL_MOLMEC_COMMON_SNAPSHOT_H
