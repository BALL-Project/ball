// $Id: trajectoryFile.h,v 1.2 2000/12/19 13:20:40 anker Exp $

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#define BALL_FORMAT_TRAJECTORYFILE_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

namespace BALL
{

	class SnapShot;
	class SnapShotManager;

	/** Trajectory file format for MD simulation.
			This is more an interface definition than an actual class, because it
			would not make too much sense to create yet-another-trajecory-format.
			This class will be specialized by actual formats, like DCD.
			\\
			{\bf Definition:} \URL{BALL/FORMAT/trajectoryFile.h}
			\\
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
				@param filename the name of the file
				@param open_mode the mode in which this file should be opened (@see
				File)
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


		/// @name Public methods for file handling
		//@{

		/** read the header of an existing file
				return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader()
			throw();

		/** update the internal header with information from the
				SnapShotManager
				@param manager the snapshot manager from which data will be written
				@return true, if the update was successful, false ow.
		*/
		virtual bool updateHeader(const SnapShotManager& manager)
			throw();

		/** write a header
				return true if the header could be written successfully, false ow.
		*/
		virtual bool writeHeader()
			throw();

		/** read a whole SnapShotManager instance from a trjectory file
				@param the SnapShotManager instance to be assigned from file
				contents
				@return true, if reading was succesful, false ow.
		*/
		virtual bool read(SnapShotManager& manager)
			throw();

		/** write a whole SnapShotManager instance as trajectory file
				@param manager the snapshot manager to be written
				@return true if the manager was written successfully
		*/
		virtual bool write(const SnapShotManager& manager)
			throw();

		/** append a list of SnapShots to an existing file
				@param buffer the list os SnapShots we want to save
				@return true, if writing was successful
		*/
		virtual bool append(const SnapShot& snapshot)
			throw();

		//@}
		
	};

} // namespace BALL

#endif //  BALL_FORMAT_TRAJECTORYFILE_H
