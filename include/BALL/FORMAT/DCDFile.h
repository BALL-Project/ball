// $Id: DCDFile.h,v 1.13 2001/09/11 12:28:31 anker Exp $

#ifndef BALL_FORMAT_DCDFILE_H
#define BALL_FORMAT_DCDFILE_H

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif

namespace BALL
{

	/** DCD Trajectory file format. This class enables BALL to write DCD
			files that can be read by VMD. The format stems from the xplor
			package. This format is {\bf NOT} portable.	\\
			The header is described in the documentation for the nested DCDHeader	class. \\
			{\bf Definition:} \URL{BALL/Format/DCDFile.h}	\\
	*/
	class DCDFile
		:	public TrajectoryFile
	{

		public:

		/** @name Type definitions
		*/
		//@{

		/** A Header of a DCD file. Got this description from
				http://www.arl.hpc.mil/PET/cta/ccm/software/fmd/Docs/html/fmd_ug.html.
				Quote: "The exact format of these files is quite ugly, as it
				reproduces the binary record format of unformatted FORTRAN files.
				Each such "record" begins with 4 bytes containing the integer value
				of the number of bytes in the following record.  It ends with a
				record containing the same 4 bytes." \\
				The header consists of several blocks. Each block starts and ends
				with the number of bytes enclosed between those numbers, i. e. a
				block consisting of 8 bytes starts with the number 8 followed by 8
				bytes of data and ends with the number 8.	\\
				BASUTELLE: complete header description \\
				{\bf Definition:} \URL{BALL/Format/DCDFile.h}	\\
		*/
		class DCDHeader
		{

			public: 

			/// Start tag of the first block
			Size				start_info_block;

			/** Four characters used as a kind of magic number. They have to form
				  the string CORD.
			*/
			char				CORD[4];

			/// The number of coordinate sets in this file
			Size 				number_of_coordinate_sets;

			/// The number of the starting time step
			Size 				step_number_of_starting_time;

			/// The number of time steps between saves
			Size				steps_between_saves;

			/** BALL additional flag.
					1 indicates that this file additionally contains atom velocities 
			*/
			Size				BALL_flag;

			/** An array of 5 unuses integers (specialized versions use them, e.g. CHARMM)
			*/
			Size 				unused_1[5];

			/// The length of one time step (in units of ???)
			DoubleReal	time_step_length;

			/// Another array of unused integers
			Size				unused_2[9];

			/// End tag of the first block
			Size				end_info_block;

			/// Start tag of the title block.
			Size				start_title_block;

			/** The number of 80 byte comments in this record. In our case we
			    restrict this to be 2 for simplicity reasons.
			*/
			Size				number_of_comments;

			/** This 160 char array (actually consisting of two 80 char arrays)
					may be used for arbitrary comments.
			*/
			char				title[160];
			
			/// End tag of the title block
			Size				end_title_block;

			/** Start tag of the little block containing only the number of atoms
					in the system
			*/
			Size				start_atomnumber_block;

			/// the number of atoms covered by every timestep
			Size				number_of_atoms;

			/// End tag of the atom number block
			Size				end_atomnumber_block;

			DCDHeader()
				throw()
				:	start_info_block(84),
					// CORD("CORD"),
					number_of_coordinate_sets(0),
					step_number_of_starting_time(1),
					steps_between_saves(0),
					BALL_flag(0),
					// unused_1(),
					time_step_length(0.0),
					// unused_2()
					end_info_block(84),
					start_title_block(164),
					number_of_comments(2),
					end_title_block(164),
					start_atomnumber_block(4),
					number_of_atoms(0),
					end_atomnumber_block(4)
			{
				// BAUSTELLE: What about using c_str() for initializing CORD?
				CORD[0] = 'C';
				CORD[1] = 'O';
				CORD[2] = 'R';
				CORD[3] = 'D';
				// BAUSTELLE: This is more than ugly
				Size argh;
				for (argh = 0; argh < 6; ++argh)
				{
					unused_1[argh] = 0;
				}
				for (argh = 0; argh < 9; ++argh)
				{
					unused_2[argh] = 0;
				}
				for (argh = 0; argh < 160; ++argh)
				{
					title[argh] = ' ';
				}
				// BAUSTELLE: this only works with <cstdio> which does not seem to
				// be available under IRIX
				// sprintf(title, "BALL DCD export file");
				// sprintf(title+80, "Here should be the date");
			}

		};

		//@}

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
		DCDFile(const String& name, File::OpenMode open_mode = File::IN)
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

		// BAUSTELLE
		/** open a DCDFile
		*/
		/*
		virtual bool open(const String& name,
				File::OpenMode open_mode = File::OUT)
			throw();
		*/

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

		// BAUSTELLE:
		// should append() also write the header? what is more intuitive?
		/** Append a SnapShot to an existing file. {\bf Note} that this method
				does {\bf not} update the header. 
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
		virtual bool flushToDisk(const ::std::vector<SnapShot>& buffer)
			throw();
		//@}
		/// @name Accessors 
		//@{

		///
		const DCDHeader& getHeader() const
			throw();

		///
		void setHeader(const DCDHeader& header)
			throw();

		/// 
		void enableVelocityStorage()
			throw();

		/// 
		void disableVelocityStorage()
			throw();

		//@}

		protected:

		//_ the current file header. 
		DCDHeader header_;

		// a flag indicating that we have to swap bytes when reading data
		bool swap_bytes_;

		// a floag indicating that this DCD file contains atom velocities
		bool has_velocities_;

	};
} // namespace BALL

#endif // BALL_FORMAT_DCDFILE_H 
