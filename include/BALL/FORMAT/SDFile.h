// $Id: SDFile.h,v 1.1 2001/12/17 01:43:09 oliver Exp $

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
			This class enables BALL to read and write MDL SD files. \\
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
				Note that this changes the properties of atoms in the system.
		*/
		virtual void write(const System& system);
		
		/**	Read a system from the SD file
		*/
		virtual void read(System&	system)
			throw(Exception::ParseError);

		//@}
	};

} // namespace BALL

#endif // BALL_FORMAT_SDFILE_H
