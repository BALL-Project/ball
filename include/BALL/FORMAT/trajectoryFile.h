// $Id: trajectoryFile.h,v 1.9 2001/09/11 12:29:13 anker Exp $

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#define BALL_FORMAT_TRAJECTORYFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOT_H
#	include <BALL/MOLMEC/COMMON/snapShot.h>
#endif

namespace BALL
{

	class SnapShot;
	class SnapShotManager;

	/** Trajectory file format for MD simulation.
			This is more an interface definition than an actual class, because it
			would not make too much sense to create yet-another-trajectory-format.
			This class will be specialized by actual formats, like DCD.	\\
			{\bf Definition:} \URL{BALL/FORMAT/trajectoryFile.h} \\
	*/
	class TrajectoryFile
		:	public File
	{

		public:

		/// @name Constructors and Destructor
		//@{

		/// Default constructor
		TrajectoryFile()
			throw();

		/// Copy constructor
		TrajectoryFile(const TrajectoryFile& file)
			throw();

		/** Detailed constructor requiring a filename and the mode in which
				this file should be opened.
				@see File
				@param filename the name of the file
				@param open_mode the mode in which this file should be opened
		*/
		TrajectoryFile(const String& filename,
				File::OpenMode open_mode = File::IN)
			throw();

		/// Destructor
		virtual ~TrajectoryFile()
			throw();

		//@}
		/// @name Assignment
		//@{
		
		/// Assignment operator
		const TrajectoryFile& operator = (const TrajectoryFile& file)
			throw();

		/// Clear method
		virtual void clear()
			throw();

		//@}
		/// @name Predicates
		//@{

		/// Equality operator
		bool operator == (const TrajectoryFile& file) const
			throw();

		//@}
		/// @name Accessors
		//@{

		/** get the number of snapshots stored in this instance.
				@return the number of snapshots of this instance
		*/
		Size getNumberOfSnapShots() const
			throw();

		/** get the number of atoms coverd by each snapshot.
				@return the number of atoms 
		*/
		Size getNumberOfAtoms() const
			throw();

		//@}
		/// @name Public methods for file handling
		//@{

		/** Read the header of an existing file.
				@return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader()
			throw();

		/** Write a header.
				@return true if the header could be written successfully, false ow.
		*/
		virtual bool writeHeader()
			throw();

		/** Append a SnapShot to an existing file. {\bf Note} that this method
				does {\bf note} update the header.
				@param snapshot the SnapShot we want to save
				@return true, if writing was successful
		*/
		virtual bool append(const SnapShot& snapshot)
			throw();

		/** Read the next SnapShot from the file.
				@param snapshot a buffer for result delivery
				@return true if a snapshot could be read, {\tt false} ow.
		*/
		virtual bool read(SnapShot& snapshot)
			throw();

		/** Write several SnapShots to disk.
				@param buffer a vector of snapshots
				@return true, if flushing was successful, false ow.
		*/
		virtual bool flushToDisk(const ::std::vector<SnapShot> buffer)
			throw();
		//@}

		protected:

		//_ @name Protected Members
		//@{

		//_ The number of snapshots stored in that file
		Size number_of_snapshots_;
		
		//_ The number of atoms covered by the snapshots
		Size number_of_atoms_;
		
		//@}
		
	};

} // namespace BALL

#endif //  BALL_FORMAT_TRAJECTORYFILE_H
