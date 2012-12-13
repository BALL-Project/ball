// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#define BALL_FORMAT_TRAJECTORYFILE_H

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
			This class will be specialized by actual formats, like DCD.	 \par
			
    	\ingroup  MDFormats
	*/
	class BALL_EXPORT TrajectoryFile
		:	public File
	{

		public:

		/// @name Constructors and Destructor
		//@{

		/// Default constructor
		TrajectoryFile();

		/** Detailed constructor requiring a filename and the mode in which
		 *	this file should be opened.
		 *	@param filename the name of the file
		 *	@param open_mode the mode in which this file should be opened
		 *  @throw Exception::FileNotFound if the file could not be openend
		 *	@see File
		 */
		TrajectoryFile(const String& filename,
			File::OpenMode open_mode = std::ios::in);

		/// Destructor
		virtual ~TrajectoryFile();

		//@}
		/// @name Assignment
		//@{

		/// Clear method
		virtual void clear();

		//@}
		/// @name Predicates
		//@{

		/// Equality operator
		bool operator == (const TrajectoryFile& file) const;

		//@}
		/// @name Accessors
		//@{

		/** get the number of snapshots stored in this instance.
				@return the number of snapshots of this instance
		*/
		virtual Size getNumberOfSnapShots();

		/** get the number of atoms coverd by each snapshot.
				@return the number of atoms 
		*/
		Size getNumberOfAtoms() const;

		//@}
		/// @name Public methods for file handling
		//@{

		/** Read the header of an existing file.
				@return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader();

		/** Write a header.
				@return true if the header could be written successfully, false ow.
		*/
		virtual bool writeHeader();

		/** Append a SnapShot to an existing file. <b>Note</b> that this method
				does <b>note</b> update the header.
				@param snapshot the SnapShot we want to save
				@return true, if writing was successful
		*/
		virtual bool append(const SnapShot& snapshot);

		/** Read the next SnapShot from the file.
				@param snapshot a buffer for result delivery
				@return true if a snapshot could be read, <tt>false</tt> ow.
		*/
		virtual bool read(SnapShot& snapshot);

		/** Write several SnapShots to disk.
		 *	@param buffer a vector of snapshots
		 *	@return true, if flushing was successful, false ow.
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		virtual bool flushToDisk(const std::vector<SnapShot>& buffer);
		//@}

		private:
			const TrajectoryFile& operator = (const TrajectoryFile& file);

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
