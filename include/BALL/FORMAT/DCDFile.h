// $Id: DCDFile.h,v 1.1 2000/12/15 17:18:35 anker Exp $

#ifndef BALL_FORMAT_DCDFILE_H
#define BALL_FORMAT_DCDFILE_H

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif

namespace BALL
{

	/** DCD Trajectory file format. This class enables BALL to write DCD
			files that can be read by VMD. The format stems from the xplor
			package. This format is {\bf NOT} portable.
			\\
			The header is described in the documentation for the nested DCDHeader
			class.
			\\
			{\bf Definition:} \URL{BALL/Format/DCDFile.h}
			\\
	*/
	class DCDFile
		:	public TrajectoryFile
	{

		/// @name Type definitions
		//@{

		/** A Header of a DCD file. Got this description from
				http://www.arl.hpc.mil/PET/cta/ccm/software/fmd/Docs/html/fmd_ug.html.
				Quote: "The exact format of these files is quite ugly, as it
				reproduces the binary record format of unformatted FORTRAN files.
				Each such "record" begins with 4 bytes containing the integer value
				of the number of bytes in the following record.  It ends with a
				record containing the same 4 bytes."
				\\
				{\bf Definition:} \URL{BALL/Format/DCDFile.h}
				\\
		*/
		struct DCDHeader
		{
			/// The number of coordinate sets in this file
			int 			number_of_coordinate_sets;
			/// The number of the starting time step
			int 			step_number_of_starting_time;
			/// The number of steps between saves
			int				steps_between_saves;
			/// The length of one time step (in units of ???)
			double		time_step_length;

			/// the number of 80 byte comments in this record
			int				number_of_80_byte_records;

			/// the number of atoms covered by every timestep
			int				number_of_atoms;

			DCDHeader()
				throw()
				:	number_of_coordinate_sets(0),
					steps_between_saves(0),
					time_step_length(0),
					number_of_80_byte_records(2),
					number_of_atoms(0)
			{
			}

		};

		//@}

		public:

		BALL_CREATE(DCDFile)

		/// @name Constructors and Destructor
		//@{

		/// Default constructor
		DCDFile()
			throw();

		/// Copy constructor
		DCDFile(const DCDFile& file)
			throw();

		/// Detailed constructor
		DCDFile(const String& name, File::OpenMode open_mode)
			throw();

		/// Destructor
		virtual ~DCDFile()
			throw();

		//@}


		/// @name Assignment
		//@{

		/// Assignment operator
		const DCDFile& operator = (const DCDFile& file)
			throw();

		/// Clear method
		virtual void clear()
			throw();

		//@}


		/// @name Predicates
		//@{

		/// Equality operator
		bool operator == (const DCDFile& file)
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
			throw(Exception::NotImplemented);

		/** write a whole SnapShotManager instance as trajectory file
				@param manager the snapshot manager to be written
				@return true if the manager was written successfully
		*/
		virtual bool write(const SnapShotManager& manager)
			throw();

		/** append the data of a SnapShotManager to an existing file
				@param manager the SnapShotManager we want to save
				@return true, if writing was successful
		*/
		virtual bool append(const SnapShotManager& manager)
			throw();

		/** append a list of SnapShots to an existing file
				@param buffer the list os SnapShots we want to save
				@return true, if writing was successful
		*/
		virtual bool append(const std::vector<SnapShot>& buffer)
			throw();

		//@}


		protected:

		//_ the current file header. 
		DCDHeader header_;

		// a flag indicating that we have to swap bytes when reading data
		bool swap_bytes_;

	};
} // namespace BALL

#endif // BALL_FORMAT_DCDFILE_H 
