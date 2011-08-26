// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_MOPACOUTPUTFILE_H
#define BALL_FORMAT_MOPACOUTPUTFILE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_FORMAT_GENERICMOLFILE_H
# include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
# include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

namespace BALL
{
	/**
	 * MOPAC output file class.
	 * This class enables BALL to read and write MOPAC output files.	 
	 * \ingroup  StructureFormats
	 */
	class BALL_EXPORT MOPACOutputFile
		: public GenericMolFile
	{
		public:
			/** @name Constructors and Destructors
			 */
			//@{

			/** Default constructor
			 */
			MOPACOutputFile();

			/** Detailed constructor
			 *  @throw Exception::FileNotFound if the file could not be opened
			 */
			MOPACOutputFile(const String& filename,
					           File::OpenMode open_mode = std::ios::in);

			/**	Destructor
			 */
			virtual ~MOPACOutputFile();

			/**	@name Reading and Writing of Kernel Datastructures
			*/
			//@{

			// TODO
			/**	Write a molecule to a MOPAC output file.
			 *  
			 *  @throw File::CannotWrite if writing to the file failed
			 */
//			virtual bool write(const Molecule& molecule);

			// TODO
			/**	Write a system to a MOPAC output file.
			 *  
			 *  @throw File::CannotWrite if writing to the file failed
			 */
//			virtual bool write(const System& system);

			/**	Read a molecule from the MOPACOutputFile file
			 *  @throw Exception::ParseError if a syntax error was encountered
			 */
			virtual Molecule* read();

			/** @copydoc GenericMolFile(System& system)
			 */
			virtual bool read(System& system);

			//@}
			
			virtual void clear();

		protected:
			void addAtom_(const String& line, Molecule* mol);

			std::vector<Atom*> atoms_;
			std::map<String, Size> type_counts_;
	};
}
#endif // BALL_FORMAT_MOPACOUTPUTFILE_H


