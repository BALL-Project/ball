// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
			complex and capricious structure and contents). \par
			GenericMolFile supports two type of operations: 
			reading/writing single molecules and reading/writing 
			systems (i.e. collections of molecules). \par
			When reading molecules, the structures are read one at
			a time from the file in the order they are stored.
			In contrast, reading systems retrieves all structures
			contained in the file. For file formats that do not support
			multiple structure in a file (e.g. the MDL  \link MOLFile MOLFile \endlink ),
			those two operations are basically equivalent.
			A fundamental difference however is the fact that
			reading a molecule <b>  create </b> a new molecule, whereas
			reading a system adds the molecules read to an
			existing instance of  \link System System \endlink . This implies that
			the system has to be cleared prior to reading a system
			if that incremental behaviour is not desired. \par
			The user interface of the class is mainly provided through
			the stream operator. A typical example for reading a structure 
			from a HyperChem file might look as follows: \par
			\begin{verbatim}
				HINFile hinfile(filename);
				System S;
				hinfile >> S;
			\end{verbatim}
			This interface applies to all derived classes as well, so that
			file formats can be exchanged conveniently. \par
			GenericMolFile is derived from  \link LineBasedFile LineBasedFile \endlink  since most
			molecular structure formats are line-based tagged formats,
			often containing Fortran-style formatted sections. 
			 \link LineBasedFile LineBasedFile \endlink  provides a number of convenient methods
			to parse that kind of format.
			 \par
			
    	\ingroup  StructureFormats
	*/
	class BALL_EXPORT GenericMolFile
		: public LineBasedFile
	{
		public:

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		GenericMolFile();

		/** Detailed constructor
		*/
		GenericMolFile(const String& filename, File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound);

		/** Destructor
		*/
		virtual ~GenericMolFile();
		
		//@}
		/**	@name Assignment
		*/
		//@{
	
		/**	Assignment operator.
				Create a new object pointing to the same filename.
		*/
		const GenericMolFile& operator = (const GenericMolFile& rhs)
			throw(Exception::FileNotFound);

		//@}
		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{

		/**	Write the molecules of a system.
				If the file format does not support multiple 
				molecules in a single file, a warning should be printed
				and only the first molecule should be stored. \par
				The default implementation iterates over
				the system and calls  \link write(const Molecule& molecule) write(const Molecule& molecule) \endlink  
				for each molecule. 
		*/
		virtual bool write(const System& system)
			throw(File::CannotWrite);
		
		/**	Write a molecule.
				Repeated invocations of this method append
				molecules to the same file. \par
				The default implementation does nothing.
		*/
		virtual bool write(const Molecule& molecule)
			throw(File::CannotWrite);
		
		/**	Read a system.
				This method will read all molecules contained in the file
				and add them to the system. \par
				The default implementation calls  \link read() read() \endlink 
				until <b>false</b> is returned and adds the molecules read to 
				the system.
				@return true if anything could be read
				@exception Exception::ParseError if the file could not be parsed while reading a molecule
		*/
		virtual bool read(System&	system)
			throw(Exception::ParseError);

		/**	Read a molecule.
				This method will load the 
				first (or the next, on subsequent invocation) molecule
				from the file. If the file format does not support 
				multiple molecules, only the first call to  \link read read \endlink 
				will be successful. This method will create an instance of molecule
				and its the user's responsibility to destroy that molecule.
				@return a pointer to a molecule, <b>0</b> if the file was not open, empty, or at its end
				@exception Exception::ParseError if the contents of the file could not be parsed
		*/
		virtual Molecule* read()
			throw(Exception::ParseError);

		//@}
		/**	@name Operators
		*/
		//@{
	
		/** Stream operator for reading a system.
				Calls  \link read(System&) read(System&) \endlink .
		*/
		virtual GenericMolFile& operator >> (System& system)
			throw(Exception::ParseError);
		
		/**	Stream operator for writing a system of molecules.
				Calls  \link write(const System& system) const write(const System& system) const \endlink .
		*/
		virtual GenericMolFile& operator << (const System& system)
			throw(File::CannotWrite);

		/** Stream operator for reading a molecule.
				Calls  \link read() read() \endlink 
		*/
		virtual GenericMolFile& operator >> (Molecule& molecule)
			throw(Exception::ParseError);
		
		/**	Stream operator for writing a system of molecules.
				Calls  \link write() write() \endlink 
		*/
		virtual GenericMolFile& operator << (const Molecule& molecule)
			throw(File::CannotWrite);
		//@}
		
		protected:
		/**	Initialize internals for read.
				This method is called by the default implementation 
				of  \link read(System& system) read(System& system) \endlink . Its purpose is the 
				initialization of internal members holding, for example,
				header information from the file.
				The default implementation provided is empty.
		*/
		virtual void initRead_();
			
		/**	Initialize internals for write.
				Same functionality as  \link initRead initRead \endlink , but is called 
				prior to writing a system.
		*/
		virtual void initWrite_();
		
	};
} // namespace BALL

#endif // BALL_FORMAT_GENERICMOLFILE_H
