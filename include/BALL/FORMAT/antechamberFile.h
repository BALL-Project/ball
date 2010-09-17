// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_ANTECHAMBERFILE_H
#define BALL_FORMAT_ANTECHAMBERFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
# include <BALL/FORMAT/genericMolFile.h>
#endif

namespace BALL
{
	class Atom;
	class AtomContainer;
	class System;

	/**	Antechamber file format (ac-file) class.
			
			This class is used to read and write antechamber input/output files.	 
			\par

    	\ingroup  StructureFormats
	*/
	class BALL_EXPORT AntechamberFile
		: public GenericMolFile
	{
		public:

			/**	@name	Constructors and Destructors
			*/
			//@{

			/**	Default constructor
			*/
			AntechamberFile();

			/** Detailed constructor
			*/
			AntechamberFile(const String& filename, File::OpenMode open_mode = std::ios::in)
				throw(Exception::FileNotFound);

			/// Destructor
			virtual ~AntechamberFile();
			//@}

			/**	@name Reading and Writing of Kernel Datastructures
			*/
			//@{

			/** Write an AtomContainer to the ac file
			 */
			virtual bool write(const AtomContainer& ac)
				throw(File::CannotWrite);

			/**	Write a system to the ac file
			*/
			virtual bool write(const System& system)
				throw(File::CannotWrite);

			/**	Read a system from the ac file
			*/
			virtual bool read(System&	system)
				throw(Exception::ParseError);

			/**
			*/
			virtual Molecule* read()
				throw(Exception::ParseError);

			/**
			*/
			virtual bool write(const Molecule& molecule)
				throw(File::CannotWrite);
	
			//@}
		
	  protected:
			void ac_split_(const String& line, std::vector<String>& split);
	};
}
#endif
