// $Id: genericMolFile.h,v 1.9 2001/12/21 11:47:04 oliver Exp $

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
			This class provides an interface for all molecular
			structure formats (except for PDB files, due to their 
			complex and capricious structure and contents).\\
			GenericMolFile supports two type of operations: 
			reading/writing single molecules and reading/writing 
			systems (i.e. collections of molecules).\\
			When reading molecules, the structures are read one at
			a time from the file in the order they are stored.
			In contrast, reading systems retrieves all structures
			contained in the file. For file formats that do not support
			multiple structure in a file (e.g. the MDL \Ref{MOLFile}),
			those two operations are basically equivalent.
			A fundamental difference however is the fact that
			reading a molecule {\em create} a new molecule, whereas
			reading a system adds the molecules read to an
			existing instance of \Ref{System}. This implies that
			the system has to be cleared prior to reading a system
			if that incremental behaviour is not desired.\\
			The user interface of the class is mainly provided through
			the stream operator. A typical example for reading a structure 
			from a HyperChem file might look as follows:\\
			\begin{verbatim}
				HINFile hinfile(filename);
				System S;
				hinfile >> S;
			\end{verbatim}
			This interface applies to all derived classes as well, so that
			file formats can be exchanged conveniently.\\
			GenericMolFile is derived from \Ref{LineBasedFile} since most
			molecular structure formats are line-based tagged formats,
			often containing Fortran-style formatted sections. 
			\Ref{LineBasedFile} provides a number of convenient methods
			to parse that kind of format.
			\\
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

		/** Detailed constructor
		*/
		GenericMolFile(const String& filename, File::OpenMode open_mode = File::IN)
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

		/**	Initialize internals for read.
				This method is called by the default implementation 
				of \Ref{read(System& system)}. Its purpose is the 
				initialization of internal members holding, for example,
				header information from the file.
				The default implementation provided is empty.
		*/
		virtual void initRead();
			
		/**	Initialize internals for write.
				Same functionality as \Ref{initRead}, but is called 
				prior to writing a system.
		*/
		virtual void initWrite();
		
		/**	Write the molecules of a system.
				If the file format does not support multiple 
				molecules in a single file, a warning should be printed
				and only the first molecule should be stored.\\
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
		virtual GenericMolFile& operator >> (System& system)
			throw(Exception::ParseError);
		
		/**	Stream operator for writing a system of molecules.
				Calls \Ref{write(const System& system) const}.
		*/
		virtual GenericMolFile& operator << (const System& system);

		/** Stream operator for reading a molecule.
				Calls \Ref{read()}
		*/
		virtual GenericMolFile& operator >> (Molecule& molecule)
			throw(Exception::ParseError);
		
		/**	Stream operator for writing a system of molecules.
				Calls \Ref{write()}
		*/
		virtual GenericMolFile& operator << (const Molecule& molecule);
		//@}
	};

} // namespace BALL

#endif // BALL_FORMAT_GENERICMOLFILE_H
