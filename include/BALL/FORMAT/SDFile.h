// $Id: SDFile.h,v 1.4 2002/01/12 01:59:48 oliver Exp $

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
				you can disable it with this option. The \Ref{read} and \Ref{write}
				methods will still create molecules, but they will be empty. 
				However, the \Ref{NamedProperties} of the molecules contain
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

} // namespace BALL

#endif // BALL_FORMAT_SDFILE_H
