// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_SCWRLROTAMERFILE_H
#define BALL_FORMAT_SCWRLROTAMERFILE_H

#include <BALL/FORMAT/lineBasedFile.h>

namespace BALL
{
	/// forward declaration
	class RotamerLibrary;
	/** @brief Reads Dunbrack/SCWRL rotamer library files 
	 
			This class is able to read the rotamer library files from Dunbrack including
      the one used in SCWRL3.0 .
      It decides if the file contains backbone dependent or 
			backbone independent rotamers. Currently only files with the same format
			as <b>bbdep02.May.lib</b> or <b>bbind02.May.lib</b> are supported. 
	*/
	class BALL_EXPORT SCWRLRotamerFile 
		: public LineBasedFile
	{
		public:

		/** @name Constructors and Destructor
		*/
		//@{
		/// Default constructor
		SCWRLRotamerFile();

		/// Detailed constructor
		SCWRLRotamerFile(const String& name, File::OpenMode open_mode = std::ios::in);

		/// Destructor
		virtual ~SCWRLRotamerFile();
		//@}


		/** @name Assignment
		*/
		//@{
		/// Assignment operator
		const SCWRLRotamerFile& operator = (const SCWRLRotamerFile& file);

		/// input operator
		void operator >> (RotamerLibrary& rotamer_library);

		// TODO output operator????
		//@}

		/** Backbone depend read md method
		 *  @throw Exception::ParseError if a syntax error encouncountered
		 */
		void readSCWRLBackboneDependentLibraryFile(RotamerLibrary& library);

		/** Backbone independ readead method
		 *  @throw Exception::ParseError if a syntax error encouncountered
		 */
		void readSCWRLBackboneIndependentLibraryFile(RotamerLibrary& library);
	};
} // namespace BALL

#endif // BALL_FORMAT_SCWRLROTAMERFILE_H
