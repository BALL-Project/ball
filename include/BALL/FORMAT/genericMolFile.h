// $Id: genericMolFile.h,v 1.6 2001/12/19 02:40:23 oliver Exp $

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#define BALL_FORMAT_GENERICMOLFILE_H

#ifndef BALL_FORMAT_LINEBASEDFILE_H
#	include <BALL/FORMAT/lineBasedFile.h>
#endif

namespace BALL 
{
	class Atom;
	class System;
	class Molecule;

	/**	Base class for all molecule file format classes. 
			{\bf Definition:} \URL{BALL/FORMAT/genericMolFile.h} 
	*/
	class GenericMolFile
		: public LineBasedFile
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		GenericMolFile()
			throw();

		/** Detailed constructor.
		*/
		GenericMolFile(const String& filename, File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		/**	Copy constructor
		*/
		GenericMolFile(const GenericMolFile& file);

		/** Destructor
		*/
		virtual ~GenericMolFile()
			throw();
		
		//@}
		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write the molecules of a system.
				If the file format does not support multiple 
				molecules in a single file, a warning is printed
				and only the first molecule is stored.\\
				The default implementation iterates over
				the system and calls \Ref{write(const Molecule& molecule)} 
				for each molecule. 
		*/
		virtual void write(const System& system);
		
		/**	Write a molecule.
				Repeated invocations of this method append
				molecules to the same file.\\
				The default implementation does nothing.
		*/
		virtual void write(const Molecule& molecule);
		
		/**	Read a system.
				This method will read all molecules contained in the file
				and add them to the system.\\
				The default implementation calls \Ref{read()}
				until {\bf false} is returned and adds the molecules read to 
				the system.
				@return {\bf true} if anything could be read
				@exception Exception::ParseError if the file could not be parsed while reading a molecule
		*/
		virtual bool read(System&	system)
			throw(Exception::ParseError);

		/**	Read a molecule.
				This method will load the 
				first (or the next, on subsequent invocation) molecule
				from the file. If the file format does not support 
				multiple molecules, only the first call to \Ref{read}
				will be successful. This method will create an instance of molecule
				and its the user's responsibility to destroy that molecule.
				@return a pointer to a molecule, {\bf 0} if the file was not open, empty, or at its end
				@exception Exception::ParseError if the contents of the file could not be parsed
		*/
		virtual Molecule* read()
			throw(Exception::ParseError);
		//@}

		/**	@name Operators
		*/
		//@{
		/** Stream operator for reading a system.
				Calls \Ref{read(System&)}.
		*/
		GenericMolFile& operator >> (System& system)
			throw(Exception::ParseError);
		
		/**	Stream operator for writing a system of molecules.
				Calls \Ref{write(const System& system) const}.
		*/
		GenericMolFile& operator << (const System& system);

		/** Stream operator for reading a molecule.
		*/
		GenericMolFile& operator >> (Molecule& molecule)
			throw(Exception::ParseError);
		
		/**	Stream operator for writing a molecule of molecules.
		*/
		GenericMolFile& operator << (const Molecule& molecule);
		//@}
	};

} // namespace BALL

#endif // BALL_FORMAT_GENERICMOLFILE_H
