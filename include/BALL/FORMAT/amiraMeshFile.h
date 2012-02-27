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
	/** AmiraMesh grid file format. This class enables BALL to read 
	 		the native format of Amira. The academic version of Amira is
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

		/** Write a RegularData3D to a AmiraMesh File
				Return true if the map could be written successfully, false ow.
		*/
		virtual bool write(RegularData3D& map);

		private:
			const AmiraMeshFile& operator = (const AmiraMeshFile& file);

		protected:

			///  Find string in given buffer and return a pointer
    	/// to the contents directly behind the SearchString
			const char* findAndJump_(const char* buffer, const char* search_String);
			
			/// flag indicating whether a ascii or binary data section
			bool binary_;
			
			/// the extent of the map
			Vector3 extent_;
			
			/// Vector representing the first point of the maps bounding box
			Vector3 min_;

			/// Vector representing the second point of the maps bounding box
			Vector3 max_;

			
			long idx_start_data_;
			
			// number of components of the map
			Position num_components_;
	};
} // namespace BALL

#endif // BALL_FORMAT_DCDFILE_H 
