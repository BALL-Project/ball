// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_NWCHEMTRJFILE_H
#define BALL_FORMAT_NWCHEMTRJFILE_H

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
#endif

namespace BALL
{
	/** NWChem Trajectory file format. This class enables BALL to read and
	 		write molecular dynamics trajectories that can be read by
			NWChem.			
    	\ingroup  MDFormats
	*/
	class BALL_EXPORT NWChemTRJFile
		:	public TrajectoryFile
	{

		public:

		BALL_CREATE(NWChemTRJFile)

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		NWChemTRJFile();

		/// Copy constructor
		NWChemTRJFile(const NWChemTRJFile& file);

		/// Detailed constructor
		NWChemTRJFile(const String& name, File::OpenMode open_mode = std::ios::in);

		/// Destructor
		virtual ~NWChemTRJFile();

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		const NWChemTRJFile& operator = (const NWChemTRJFile& file);

		/// Clear method
		virtual void clear();

		//@}
		/** @name Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const NWChemTRJFile& file) const;

		/// Returns true if the TRJ - File contains velocities
		bool hasSolventVelocities() const;

		bool hasSoluteVelocities() const;

		//@}
		/// @name Public methods for file handling
		//@{

		/** open a NWChemTRJFile
		*/
		virtual bool open(const String& name, File::OpenMode open_mode = std::ios::in);

		/** Initialize this instance, i. e. read the header and update members
		*/
		bool init();

		/** Read the header of an existing file.
				Return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader();

		/** Write a header
				Return true if the header could be written successfully, false ow.
		*/
		virtual bool writeHeader();

		// ?????: (Taken from DCDFile.h...)
		// should append() also write the header? what is more intuitive?
		/** Append a SnapShot to an existing file. <b>Note</b> that this method
				does <b>not</b> update the header. 
				@param snapshot the snapshot we want to save
				@return true, if writing was successful
		*/
//		virtual bool append(const SnapShot& snapshot);

		/** Read the next snapshot from the file
				@param snapshot a buffer for result delivery
				@return true, if a snapshot could be read, false ow.
		*/
		virtual bool read(SnapShot& snapshot);

		/** Return the system read from the header
		 */
		System& getSystem();

		/**
		*/
//		virtual bool flushToDisk(const std::vector<SnapShot>& buffer);

		//@}
		/** @name Accessors 
		*/
		//@{

		/// 
		void enableVelocityStorage();

		/// 
		void disableVelocityStorage();

		//@}

	protected:
		bool has_solvent_coordinates_;
		bool has_solute_coordinates_;
		bool has_solvent_velocities_;
		bool has_solute_velocities_;
			
		// a line buffer for parsing
		String current_line_;

		// the number of atoms per solvent molecule
		Size number_of_atoms_per_solvent_;

		// the number of bonds per solvent molecule
		Size number_of_bonds_per_solvent_;

		// the number of atoms in the solute
		Size number_of_solute_atoms_;

		// the number of solute bonds
		Size number_of_solute_bonds_;

		// the number of solvent molecules
		Size number_of_solvent_molecules_;

		// the name of the solvent
		String solvent_name_;

		// the solvent molecule
		Molecule* solvent_;

		// the solute molecule. currently we handle only proteins
		Protein* solute_;

		// the system
		System system_;

		// 
	};
} // namespace BALL

#endif // BALL_FORMAT_NWCHEMTRJFILE_H 
