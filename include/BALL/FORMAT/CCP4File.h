// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_FORMAT_CCP4FILE_H
#define BALL_FORMAT_CCP4FILE_H

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif

namespace BALL
{
	/** CCP4 density map file format. This class enables BALL to read and
	 		write CCP4 - type binary electron density maps as used by the CCP4
      project
			For detailed information on the file format CCP4 see:
			http://www.ccp4.ac.uk/html/maplib.html#introduction
    	\ingroup  Format
	*/
	class BALL_EXPORT CCP4File
		:	public File
	{

		public:

		/** @name Constructors and Destructor
		*/
		//@{

		/// Default constructor
		CCP4File();

		/** Detailed constructor
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		CCP4File(const String& name, File::OpenMode open_mode = std::ios::in);

		/// Destructor
		virtual ~CCP4File();

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
		bool operator == (const CCP4File& file) const;

		//@}
		/// @name Public methods for file handling
		//@{

		/** open a CCP4File
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		virtual bool open(const String& name, File::OpenMode open_mode = std::ios::in);

		/** Read the header of an existing file.
				Return true if the header could be read successfully, false ow.
		*/
		virtual bool readHeader();
		
		/** Read the symmetry records of an existing file if present.
				Return true if the symmetry records could be read successfully, false ow.
		*/
		virtual bool readSymmetryRecords();

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
			const CCP4File& operator = (const CCP4File& file);

		protected:

			int readBinValueasInt_(char* header, Position pos);
			
			float readBinValueasFloat_(char* header, Position pos);
			
			/// flag indicating that we have to swap bytes when reading header values
			bool swap_bytes_;
			
			/// number of bytes used for storing symmetry operations
			Distance offset_symops_;
			
			/// how do columns, rows and sections correspond to specific axes: 0=x, 1=y, 2=z 
			Size col_axis_;
			Size row_axis_;
			Size sec_axis_;

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
			Index space_group_;

			/// geometry in cartesian coordinates
			Vector3 origin_, xaxis_, yaxis_, zaxis_;
	};
} // namespace BALL

#endif // BALL_FORMAT_DCDFILE_H 
