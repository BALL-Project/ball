// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SDFile.h,v 1.10 2003/03/14 11:49:00 sturm Exp $

#ifndef BALL_FORMAT_SDFILE_H
#define BALL_FORMAT_SDFILE_H

#ifndef BALL_FORMAT_MOLFILE_H
#	include <BALL/FORMAT/MOLFile.h>
#endif

namespace BALL 
{
    /**  \addtogroup  StructureFormats
     *  @{
     */
	class System;
	class Atom;
	class Molecule;

	/**	MDL SD file class.
			This class enables BALL to read and write MDL SD files. \par
			A description of this file format (and the related  \link MOLFile MOLFile \endlink  
			format) can be obtained from the MDL webpage http://www.mdli.com
			 \par
			
	*/
	class SDFile
		: public MOLFile
	{
		public:

		BALL_CREATE(SDFile)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		SDFile()
			throw();

		/** Detailed constructor.
		*/
		SDFile(const String& filename, File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		/**	Copy constructor
		*/
		SDFile(const SDFile& file)
			throw(Exception::FileNotFound);

		/// Destructor
		virtual ~SDFile()
			throw();
		
		//@}

		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write a system to the SD file.
				Write all molecules contained in the system to the
				SD file. All named properties are included in the
				property section.
		*/
		virtual void write(const System& system);
		
		/**	Append a single molecule to the SD file.
		*/
		virtual void write(const Molecule& molecule);

		/**	Read a system from the SD file
		*/
		virtual bool read(System& system)
			throw(Exception::ParseError);

		/**	Read a single molecule from an SD file.
		*/
		virtual Molecule* read()
			throw(Exception::ParseError);

		/** Do not read atoms and bonds.
				This (seemingly strange) option allows the user to read
				the properties of the molecules only. Since SD files can contain
				all kinds of information besides the molecular structure
				and reading the structure can be rather time and space consuming,
				you can disable it with this option. The  \link read read \endlink  and  \link write write \endlink 
				methods will still create molecules, but they will be empty. 
				However, the  \link NamedProperties NamedProperties \endlink  of the molecules contain
				the optional information contained in the SD file.
		*/
		void disableAtoms()
			throw();

		/** Read atoms and bonds.
				@see disableAtoms
		*/
		void enableAtoms()
			throw();
		//@}

		protected:
		/** Read the property block of an SD file.
				The values are stored as named properties in the 
				molecule read.
		*/
		void readPropertyBlock_(Molecule& molecule);

		/** Read the propertyblock of an SD file.
				The values are stored as named properties in the 
				molecule read.
		*/
		void writePropertyBlock_(const Molecule& molecule);

		/**	Whether atoms and bonds should be read
		*/
		bool read_atoms_;
	};
  /** @} */
} // namespace BALL

#endif // BALL_FORMAT_SDFILE_H
