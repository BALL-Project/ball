// $Id: SDFile.h,v 1.2 2001/12/18 01:19:10 oliver Exp $

#ifndef BALL_FORMAT_SDFILE_H
#define BALL_FORMAT_SDFILE_H

#ifndef BALL_FORMAT_MOLFILE_H
#	include <BALL/FORMAT/MOLFile.h>
#endif

namespace BALL 
{
	class System;
	class Atom;
	class Molecule;

	/**	MDL SD file class.
			This class enables BALL to read and write MDL SD files.\\
			A description of this file format (and the related \Ref{MOLFile} 
			format) can be obtained from the MDL webpage \URL{http://www.mdli.com}.
			\\
			{\bf Definition:} \URL{BALL/FORMAT/SDFile.h} \\
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
		SDFile(const String& filename, File::OpenMode open_mode = File::IN)
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
		virtual void read(System&	system)
			throw(Exception::ParseError);

		/**	Read a single molecule from an SD file.
		*/
		virtual void read(Molecule& molecule)
			throw(Exception::ParseError);
		//@}

		protected:
		/** Read the propertyblock of an SD file.
				The values are stored as named properties in the 
				molecule read.
		*/
		void readPropertyBlock_(Molecule& molecule);

		/** Read the propertyblock of an SD file.
				The values are stored as named properties in the 
				molecule read.
		*/
		void writePropertyBlock_(const Molecule& molecule);
	};

} // namespace BALL

#endif // BALL_FORMAT_SDFILE_H
