// $Id: snapShotManager.h,v 1.3 2001/05/14 19:22:09 amoll Exp $

#ifndef BALL_MOLMEC_COMMON_SNAPSHOTMANAGER_H
#define BALL_MOLMEC_COMMON_SNAPSHOTMANAGER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/common/exception.h>
#endif

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#	include <BALL/MOLMEC/COMMON/forceField.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#	include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif


namespace BALL
{

	class TrajectoryFile;

  /**	Snapshot management for MD simulations.
			This class manages a list of single snapshot objects.
			Snapshots are numbered starting with 1.	\\
			{\bf Definition:}\URL{BALL/MOLMEC/COMMON/snapShotManager.h}
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
				otherwise the new data will be appended, provided that the systems
				match. 
				@param my_system the system to bind this manager to
				@param my_force_field the force field that is bound to the system
				@param my_snapshot_file 
				@param overwrite
    */
    SnapShotManager
			(System* my_system, const ForceField* my_force_field,
			 TrajectoryFile* my_snapshot_file, bool overwrite = true)
			throw();

    /** This constructor expects a valid system, a valid force field
				and the name of a snapshot file. If the {\tt overwrite} is true
				then any existing file of the given name will be overwritten,
				otherwise the new data will be appended, provided that the systems
				match. 
				@param my_system the system to bind this manager to
				@param my_force_field the force field that is bound to the system
				@param my_options
				@param filename the name of the snapshot file
				@param overwrite {\bf true}: overwrite existing snapshot file, 
												 {\bf false}: append to the file.
    */
    SnapShotManager	
			(System* my_system, const ForceField* my_force_field,
			 const Options& my_options, TrajectoryFile* file, bool overwrite = true)
			throw();

    /// Copy constructor.
    SnapShotManager(const SnapShotManager& manager)
			throw();

    /// Destructor.
    virtual ~SnapShotManager()
			throw();

    //@}
    /** @name Setup methods
		*/
    //@{

		/** This setup method sets all necessary members and calls @see setup
				afterwards.
				@param my_system the System to which we want to bind this
				SnapShotManager
				@param my_force_field the respective forcefield
				@param my_snapshot_file the trajectory file we want to use
				@return true, if the setup was succesful, false ow.
		*/
		bool setup(System* my_system, const ForceField* my_forcefield,
				TrajectoryFile* my_snapshot_file)
			throw();

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
    /// @name Debugging and diagnostics
    //@{

    /** Is the SnapshotManager ready for use?
    */
    virtual bool isValid() const
			throw();

    //@}
    /// @name Accessors
    //@{

		/** Set the system member
				@param my_system a const pointer to the system to which this
				SnapshotManager will be bound
		*/
		void setSystem(System* my_system)
			throw();

		/// get a const pointer to the system member
		System* getSystem() const
			throw();

		/// set the force field
		void setForceField(const ForceField* my_ff)
			throw();

		/// get a const pointer to the force field
		const ForceField* getForceField() const
			throw();

		/// set the trajectory file
		void setTrajectoryFile(TrajectoryFile* my_file)
			throw();

		/// get a pointer to the trajectory file
		TrajectoryFile* getTrajectoryFile() const
			throw();

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
			throw();

		/** Read a certain SnapShot from a TrajectoryFile. This method tries to
				read SnapShot number {\bf number} from the file
				@param number the number of the snapshot we want to read
				@param snapshot a buffer for returning the snapshot
				@return true if the snapshot could be read, {\bf false} ow.
		*/
		virtual bool applySnapShot(Size number)
			throw();

		/** Read a the first SnapShot from the associated TrajectoryFile.
				@param snapshot a buffer for returning the snapshot
				@return true if the snapshot could be read, {\bf false} ow.
		*/
		virtual bool applyFirstSnapShot()
			throw();

		/** Read a the next SnapShot from the associated TrajectoryFile and
				apply it to the system
				@param snapshot a buffer for returning the snapshot
				@return true if the snapshot could be read, {\bf false} ow.
		*/
		virtual bool applyNextSnapShot()
			throw();

		/** This method applies the last SnapShot of the associated file
				i. e. writes all available data (positions, forces, etc.) from the
				SnapShot to the System. Note that a SnapShot does not need to have
				all data.
		*/
		virtual bool applyLastSnapShot()
			throw();

    /// This method writes all snapshots taken so far to hard disk
    virtual void flushToDisk()
			throw();

    //@}
    /// @name Public Attributes
    //@{

    /// The available options for this class.
    Options options; 

    //@} 

    protected:

    //_ @name Protected Attributes
    //@{

		// BAUSTELLE: this was a const ptr before, but applySnapShot needs a
		// mutable System. is that reasonable
    //_ The system to which the SnapshotManager is bound
    System* system_ptr_; 

    //_ The force field of the current system
    const ForceField* force_field_ptr_; 

    //_ A vector containing those snapshot objects currently in memory
    vector<SnapShot> snapshot_buffer_; 

		//_ The trajectory file where the data is saved in
		TrajectoryFile* trajectory_file_ptr_;

    /*_ The frequency of saving snapshots in memory to disk.
        After flush_to_disk_frequency_ iterations, a save is done. 
    */
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
}

#endif //  BALL_MOLMEC_COMMON_SNAPSHOTMANAGER_H
