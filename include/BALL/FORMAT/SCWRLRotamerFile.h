// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SCWRLRotamerFile.h,v 1.1.2.1 2007/04/02 21:02:11 bertsch Exp $
//

#ifndef BALL_FORMAT_SCWRLROTAMERFILE_H
#define BALL_FORMAT_SCWRLROTAMERFILE_H

#include <BALL/FORMAT/lineBasedFile.h>

namespace BALL
{
	/// forward declaration
	class RotamerLibrary;
	/** @brief Reads SCWRL rotamer library files
	 
			This class is able to read the Dunbrack rotamer library files
			of SCWRL. It decides if the file contains  backbone dependent or 
			a backbone independent rotamers. Currently are only the files of 
			type <b>bbdep02.May.lib</b> and <b>bbind02.May.lib</b> supported. 
			The filenames contain the type and the date of creation. 
	*/
	class BALL_EXPORT SCWRLRotamerFile : public LineBasedFile
	{

		public:

		/** @name Constructors and Destructor
		*/
		//@{
		/// Default constructor
		SCWRLRotamerFile() throw();

		/// Copy constructor
		SCWRLRotamerFile(const SCWRLRotamerFile& file) throw();

		/// Detailed constructor
		SCWRLRotamerFile(const String& name, File::OpenMode open_mode = std::ios::in) throw();

		/// Destructor
		virtual ~SCWRLRotamerFile() throw();
		//@}


		/** @name Assignment
		*/
		//@{
		/// Assignment operator
		const SCWRLRotamerFile& operator = (const SCWRLRotamerFile& file);

		/// input operator
		void operator >> (RotamerLibrary& rotamer_library) throw();

		// TODO output operator????
		//@}


		protected:

			// backbone dependent read method
			void readSCWRLBackboneDependentLibraryFile_(RotamerLibrary& library) throw(Exception::ParseError);

			// backbone independent read method
			void readSCWRLBackboneIndependentLibraryFile_(RotamerLibrary& library) throw(Exception::ParseError);

	};
} // namespace BALL

#endif // BALL_FORMAT_SCWRLROTAMERFILE_H
