// $Id: snapShot.h,v 1.17 2001/03/11 19:31:58 anker Exp $

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

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL
{

	/** SnapShot class, containing the actual data of one single snapshot.
			It consists of the posistions, velocities and forces for each atom
			along with kinetic and potential energy of selected atoms. It is used
			by the \Ref{SnapShotManager}. \\
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/snapShot.h}
	*/
  class SnapShot
  {

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

		/// Set the value for the potential energy of this snapshot's system
		void setPotentialEnergy(DoubleReal potential_energy) 
			throw();

		/// Get the value for the potential energy of this snapshot's system
		DoubleReal getPotentialEnergy() const
			throw();

		/// Set the value for the kinetic energy of this snapshot's system
		void setKineticEnergy(DoubleReal kinetic_energy)
			throw();

		/// Get the value for the kinetic energy of this snapshot's system
		DoubleReal getKineticEnergy() const
			throw();

		/// Specify all atom positions
		void setAtomPositions(const ::std::vector<Vector3>& atom_postions)
			throw();

		/// Get the vector containing the atom postions
		const ::std::vector<Vector3>& getAtomPositions() const
			throw();

		/// Specify all atom velocities
		void setAtomVelocitites(const ::std::vector<Vector3>& atom_velocities)
			throw();

		/// Get the vector containing the atom velocitites
		const ::std::vector<Vector3>& getAtomVelocities() const
			throw();

		/// Specify all atom forces
		void setAtomForces(const ::std::vector<Vector3>& atom_forces)
			throw();

		/// Get all atom forces
		const ::std::vector<Vector3>& getAtomForces() const
			throw();

		//@}
		/// @name System interaction
		//@{

		/** Take a SnapShot from a system. Copy all positions, velocities and
				forces from the System {\tt system} to this instance of SnapShot.
				@param system the System from which to take the data
		*/
		void takeSnapShot(const System& system)
			throw(Exception::OutOfMemory);

		/** Apply the data contained in a SnapShot to a System. Copy all
				available data to the system. {\bf Note} that some trajectory file
				formats do not contain all this information and vectors might be
				zero or even worse undefined when read SnapShots are read from such
				a file.
				@param system the System which will be manipulated
		*/
		void applySnapShot(System& system) const
			throw();

		/** Read all atom positions from a System and store them in this
				instance of SnapShot.
				@param system the System from which data will be read
		*/
		void getAtomPositions(const System& system)
			throw(Exception::OutOfMemory);

		/** Set all Atom positions of a System to the values stored in this
				instance of SnapShot.
				@param system the system which will be manipulated
		*/
		void setAtomPositions(System& system) const
			throw();

		/** Read all atom velocities from a System and store them in this
				instance of SnapShot.
				@param system the System from which data will be read
		*/
		void getAtomVelocities(const System& system)
			throw(Exception::OutOfMemory);

		/** Set all Atom velocities of a System to the values stored in this
				instance of SnapShot.
				@param system the system which will be manipulated
		*/
		void setAtomVelocitites(System& system) const
			throw();

		/** Read all atom forces from a System and store them in this
				instance of SnapShot.
				@param system the System from which data will be read
		*/
		void getAtomForces(const System& system)
			throw(Exception::OutOfMemory);

		/** Set all Atom forces of a System to the values stored in this
				instance of SnapShot.
				@param system the system which will be manipulated
		*/
		void setAtomForces(System& system) const
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

	/// Output stream operator for SnapShots
	::std::ostream& operator << (::std::ostream& os, const SnapShot& ss);

	/// Input stream operator for SnapShots
	::std::istream& operator >> (::std::istream& is, SnapShot& ss);


} // end of namespace BALL  

#endif // BALL_MOLMEC_COMMON_SNAPSHOT_H
