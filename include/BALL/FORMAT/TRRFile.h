// $Id: TRRFile.h,v 1.1 2001/09/13 11:45:15 anhi Exp $

#ifndef BALL_FORMAT_TRRFILE_H
#define BALL_FORMAT_TRRFILE_H

#ifndef BALL_FORMAT_TRAJECTORYFILE_H
#	include <BALL/FORMAT/trajectoryFile.h>
#endif

namespace BALL
{

	/** TRR Trajectory file format. This class enables BALL to read and write
			TRR	files. The format stems from the gromacs molecular dynamics
			program. This format is portable.	\\
			In a TRR-file, each timestep is preceded by a header. The header
			is handled by the nested TRRHeader class. \\
			{\bf Definition:} \URL{BALL/Format/TRRFile.h}	\\
	*/
	class TRRFile
	  :	public TrajectoryFile
	{

		public:

		/** @name Type definitions
		*/
		//@{

		/** The header for each coordinate set stored in the TRR file.
				The format can be found at \\
				http://www.ks.uiuc.edu/~danorris/projects/formats/trr.shtml \\
				The header consists of several records that are stored sequentially. \\
				{\bf Definition:} \URL{BALL/Format/TRRFile.h}	\\
		*/
		class TRRHeader
		{

			public: 

			/** Magic number for the TRR format. This must be equal to 1993.
			 */
			Size        MAGIC;

			/** The version number of the TRR format.
			 */
			Size        VERSION;

			/** The length of the title string.
			 */
			Size        title_string_length;

			/** The title string.
			 */
			String      title_string;

			/** This record is called ir_size. I don't know what this is used for.
					We don't need it.
			*/
			Size        ir_size;

			/** This record is called e_size. I don't know what this is used for.
					We don't need it.
			*/
			Size        e_size;

			/** The size of the bounding box data: 9 means that we have an arbitrary
					box, i.e. the base vectors may have arbitray length and relative angles.
					The size is then stored as x1 y1 z1 x2 y2 z2 x3 y3 z3 \\
					3 means a shortened notation for rectangular boxes. The size is then
					stored as x1 y2 z3. All other components are assumed to be zero.
			*/
			Size        bounding_box_data_size;

			/** This record is called vir_size. It denotes the size of the vir_size block
					that is stored after the bounding box data. I don't know what kind of data
					is stored in this block. We don't need it.
			*/
			Size        vir_size;

			/** This record is called pres_size. It denotes the size of the pres_size block
					that is stored after the vir_size data (if present). I don't know what kind
					of data is stored in this block. We don't need it.
			*/
			Size        pres_size;

			/** This record is called top_size. I don't know what this is used for.
					We don't need it.
			*/
			Size        top_size;

			/** This record is called sym_size. I don't know what this is used for.
					We don't need it.
			*/
			Size        sym_size;

			/** The size of the position data: 4 means that each position vector is stored 
					using 4 bytes for each component, 8 means 8 bytes.
			*/
			Size        position_data_size;

			/** The size of the velocity data. 0 means that velocities are not stored in the
					file, 4 means that each velocity vector is stored using 4 bytes for each 
					component, 8 means 8 bytes.
			*/
			Size        velocity_data_size;

			/** The size of the force data. 0 means that forces are not stored in the file,
					4 means that each force vector is stored using 4 bytes for each component,
					8 means 8 bytes.
			*/
			Size        force_data_size;

			/** Number of atoms in the system.
			 */
			Size        number_of_atoms;

			/** Timestep index.
			 */
			Size        timestep_index;

			/** This record is called nre. I don't know what this is used for.
					We don't need it.
			*/
			Size        nre;

			/** The time corresponding to the timestep in picoseconds
			 */
			float       timestep_time;

			/** This record is called lambda. I don't know what it is used for.
					We don't need it.
			*/
			float       lambda;

			TRRHeader()
				throw()
				: MAGIC(1993),
					VERSION(13),
	  			title_string_length(24),
  				title_string("Created by BALL::TRRFile"),
				  ir_size(0),
				  e_size(0),
				  vir_size(0),
				  pres_size(0),
				  top_size(0),
  				sym_size(0),
   				velocity_data_size(0),
  				force_data_size(0),
					timestep_index(0),
				  nre(0),
  				timestep_time(0.002),
					lambda(0)
			{
			}
		};

		//@}

		BALL_CREATE(TRRFile)

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		TRRFile()
			throw();

		/// Copy constructor
		TRRFile(const TRRFile& file)
			throw(Exception::FileNotFound);

		/// Detailed constructor
		TRRFile(const String& name, File::OpenMode open_mode = File::IN)
			throw();

		/// Destructor
		virtual ~TRRFile()
			throw();

		//@}
		/** @name Assignment
		*/
		//@{

		/// Assignment operator
		const TRRFile& operator = (const TRRFile& file)
			throw();

		/// Clear method
		virtual void clear()
			throw();

		//@}
		/** @name Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const TRRFile& file) const
			throw();

		//@}
		/// @name Public methods for file handling
		//@{

		/** Initialize the file for usage.
		 */
		virtual bool init()
			throw();

		/** Writes the next header that shall preceed the next
				data block.
				@param header the TRRHeader we want to write
				@return true, if a header could be written, false ow.
		*/
		virtual bool writeNextHeader(const TRRHeader header)
			throw();

		/** Reads the next header preceding the next data block.
				@param header a buffer for result delivery
				@return true, if a header could be read, false ow.
		*/
		virtual bool readNextHeader(TRRHeader& header)
			throw();
		
		/** Append a SnapShot to an existing file
				@param snapshot the SnapShot we want to store
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

		/** Read the whole file into a SnapShotManager
		*/
		virtual TRRFile& operator >> (SnapShotManager& ssm)
			throw();
		
		/**
		*/
		virtual bool flushToDisk(const ::std::vector<SnapShot> buffer)
			throw();
		//@}
		/// @name Accessors 
		//@{

		///
		bool hasVelocities() const
			throw();

		///
		void setVelocityStorage(const bool storage)
			throw();

		///
		bool hasForces() const
			throw();

		///
		void setForceStorage(const bool storage)
			throw();

		///
		Size getPrecision() const
			throw();

		///
		bool setPrecision(const Size newprecision)
			throw();

		///
		float getTimestep() const
			throw();

		///
		void setTimestep(const float timestep)
			throw();

		///
		Vector3 getBoundingBoxX() const
			throw();

		///
		Vector3 getBoundingBoxY() const
			throw();

		///
		Vector3 getBoundingBoxZ() const
			throw();

		///
		void setBoundingBox(const Vector3 box[3])
			throw();

		//@}

		protected:

		// the current step's header
		TRRHeader header_;
		
		// the precision of the file in bytes per value
		Size precision_;
		
		// a flag deciding if the file contains velocities
		bool has_velocities_;

		// a flag deciding if the file contains forces
		bool has_forces_;

		// the index of the current timestep / snapshot
		Size timestep_index_;

		// the length of the timestep
		float timestep_;

		// three vectors containing the base vectors of the box in
		// nanometers
		Vector3 box1_, box2_, box3_;
	};
} // namespace BALL

#endif // BALL_FORMAT_TRRFILE_H 
