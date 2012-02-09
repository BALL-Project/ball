// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_FORMAT_AMIRAMESHFILE_H
#define BALL_FORMAT_AMIRAMESHFILE_H

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

namespace BALL
{
	/** AmiraMesh grid file format. This class enables BALL to read (and
	 		write) the native format of Amira. The academic version of Amira is
			developed by the Visualization and Data Analysis Group at Zuse
			Institute Berlin. Commercial versions are available from Visage
			Imaging, Berlin and VSG - Visualization Sciences Group, France.
			
			For detailed information on the file format see:
			http://amira.zib.de/mol/usersguide/HxFileFormat_AmiraMesh.html
			
			At the moment, we only support uniform grids in little endian binary
			and  ASCII data sections. 
			The following parser code is inspired by the code of Tino Weinkauf
			(cf. http://www.mpi-inf.mpg.de/~weinkauf/notes/amiramesh.html)
			
    	\ingroup  Format
	*/
	class BALL_EXPORT AmiraMeshFile
		:	public File
	{

		public:

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		AmiraMeshFile();

		/** Detailed constructor
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		AmiraMeshFile(const String& name, File::OpenMode open_mode = std::ios::in);

		/// Destructor
		virtual ~AmiraMeshFile();

		//@}
		/** @name Assignment
		*/
		//@{

		/// Clear method
		virtual void clear();

		//@}
		/** @name Predicates
		*/
		//@{

		/// Equality operator
		bool operator == (const AmiraMeshFile& file) const;

		//@}
		/// @name Public methods for file handling
		//@{

		/** open a AmiraMeshFile
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		virtual bool open(const String& name, File::OpenMode open_mode = std::ios::in);

		/** Read the header of an existing file.
				Return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader();
		
		/** Read the file into a RegularData3D field.
		*/
		virtual bool read(RegularData3D& map);
		
		/** Write a header
				Return true if the header could be written successfully, false ow.
		*/
		virtual bool writeHeader();

		/** Write a RegularData3D to a CCP4 File
				Return true if the map could be written successfully, false ow.
		*/
		virtual bool write(RegularData3D& map);

		//@}
		/** @name Accessors 
		*/
		//@{

		//@}

		private:
			const AmiraMeshFile& operator = (const AmiraMeshFile& file);

		protected:

			const char* findAndJump_(const char* buffer, const char* search_String);
			
			/// flag indicating whether a ascii or binary data section
			bool binary_;
			
			/// the start coordinates in grid coordinates, i.e. indices
			Vector3 start_;

			/// the extent of the map
			Vector3 extent_;

			/// the sampling rates used for the map
			Vector3 sampling_rate_;
			
			/// dimension of the crystal unit cell in Angstrom/sample
			Vector3 cell_dimension_;

			/// angles of the crystal unit cell
			Angle alpha_, beta_, gamma_;

			/// mean density value
			float mean_density_;
			
			/// rms deviation of whole map from mean density value
			float deviation_sigma_;
			
			/// space group number
			int space_group_;

			/// geometry in cartesian coordinates
			Vector3 origin_, xaxis_, yaxis_, zaxis_;
	};
} // namespace BALL

#endif // BALL_FORMAT_DCDFILE_H 
