// $Id: genericMolFile.h,v 1.5 2001/12/17 01:42:43 oliver Exp $

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#define BALL_FORMAT_GENERICMOLFILE_H

#ifndef BALL_FORMAT_LINEBASEDFILE_H
#	include <BALL/FORMAT/lineBasedFile.h>
#endif

namespace BALL 
{
	class Atom;
	class System;

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
		
		/**	Write a system to the HIN file
		*/
		virtual void write(const System& system);
		
		/**	Read a system from the HIN file
		*/
		virtual void read(System&	system)
			throw(Exception::ParseError);

		/**	Read a system from a HIN file.
		*/
		virtual GenericMolFile& operator >> (System& system)
			throw(Exception::ParseError);
		
		/**	Write a system to a HIN file.
		*/
		virtual GenericMolFile& operator << (const System& system);
		
		//@}
	};

} // namespace BALL

#endif // BALL_FORMAT_GENERICMOLFILE_H
