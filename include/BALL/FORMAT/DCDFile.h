// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DCDFile.h,v 1.26 2003/07/11 15:27:03 amoll Exp $

#ifndef BALL_FORMAT_DCDFILE_H
#define BALL_FORMAT_DCDFILE_H

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif

namespace BALL
{
	/** DCD Trajectory file format. This class enables BALL to write DCD
			files that can be read by VMD. The format stems from the xplor
			package. This format is <b>NOT</b> portable.	 \par
			The header is described in the documentation for the nested DCDHeader	class.  \par
			Header of a DCD file: Got this description from
			http://www.arl.hpc.mil/PET/cta/ccm/software/fmd/Docs/html/fmd_ug.html.
			Quote: "The exact format of these files is quite ugly, as it
			reproduces the binary record format of unformatted FORTRAN files.
			Each such "record" begins with 4 bytes containing the integer value
			of the number of bytes in the following record.  It ends with a
			record containing the same 4 bytes."  \par
			The header consists of several blocks. Each block starts and ends
			with the number of bytes enclosed between those numbers, i. e. a
			block consisting of 8 bytes starts with the number 8 followed by 8
			bytes of data and ends with the number 8.	 \par
			TODO: complete header description  \par
			
    	\ingroup  MDFormats
	*/
	class DCDFile
		:	public TrajectoryFile
	{

		public:

		BALL_CREATE(DCDFile)

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		DCDFile()
			throw();

		/// Copy constructor
		DCDFile(const DCDFile& file)
			throw(Exception::FileNotFound);

		/// Detailed constructor
		DCDFile(const String& name, File::OpenMode open_mode = std::ios::in)
			throw();

		/// Destructor
		virtual ~DCDFile()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		const DCDFile& operator = (const DCDFile& file)
			throw();

		/// Clear method
		virtual void clear()
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const DCDFile& file) const
			throw();

		///
		bool isSwappingBytes() const
			throw();

		///
		bool hasVelocities() const
			throw();

		//@}
		/// @name Public methods for file handling
		//@{

		// ?????
		/** open a DCDFile
		*/
		virtual bool open(const String& name,
				File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		/** Initialize this instance, i. e. read the header and update members
		*/
		bool init()
			throw();

		/** Read the header of an existing file.
				Return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader()
			throw();

		/** Write a header
				Return true if the header could be written successfully, false ow.
		*/
		virtual bool writeHeader()
			throw();

		/** 
		*/
		virtual bool seekAndWriteHeader()
			throw();

		// ?????:
		// should append() also write the header? what is more intuitive?
		/** Append a SnapShot to an existing file. <b>Note</b> that this method
				does <b>not</b> update the header. 
				@param snapshot the snapshot we want to save
				@return true, if writing was successful
		*/
		virtual bool append(const SnapShot& snapshot)
			throw();

		/** Read the next snapshot from the file
				@param snapshot a buffer for result delivery
				@return true, if a snapshot could be read, false ow.
		*/
		virtual bool read(SnapShot& snapshot)
			throw();

		/**
		*/
		virtual bool flushToDisk(const std::vector<SnapShot>& buffer)
			throw(File::CanNotWrite);
		//@}
		/// @name Accessors 
		//@{

		/// 
		void enableVelocityStorage()
			throw();

		/// 
		void disableVelocityStorage()
			throw();

		//@}

		protected:

		//_
		Size verbosity_;

		// a flag indicating that we have to swap bytes when reading data
		bool swap_bytes_;

		// a floag indicating that this DCD file contains atom velocities
		bool has_velocities_;

		//_
		bool charmm_extra_block_A_;

		//_
		bool charmm_extra_block_B_;

		//_
		String CORD_;

		//_
		Size step_number_of_starting_time_;

		//_
		Size steps_between_saves_;

		//_
		double time_step_length_;

		//_
		Size number_of_comments_;

	};
} // namespace BALL

#endif // BALL_FORMAT_DCDFILE_H 
