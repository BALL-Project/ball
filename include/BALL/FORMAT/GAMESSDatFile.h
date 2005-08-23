// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GAMESSDatFile.h,v 1.1 2005/08/23 18:20:04 anhi Exp $
//

#ifndef BALL_FORMAT_GAMESSDATFILE_H
#define BALL_FORMAT_GAMESSDATFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

namespace BALL
{
	/** GAMESSDat file class.
	 * 	This class enables BALL to read and write input and output .dat files for
	 * 	the GAMESS quantum chemistry application.
	 *
	 * 	\ingroup StructureFormats
	 */
	class BALL_EXPORT GAMESSDatFile
		: public GenericMolFile
	{
		public:

			BALL_CREATE(GAMESSDatFile)

			/** State of the parser **/
			struct State
			{
				GAMESSDatFile* current_parser;
			};

			/** @name Constructors and Destructors
			 */
			//@{
			
			/** Default constructor
			 */
			GAMESSDatFile()
				throw();

			/** Copy constructor
			 */
			GAMESSDatFile(const GAMESSDatFile& file)
				throw(Exception::FileNotFound);

			/** Detailed constructor
			 */
			GAMESSDatFile(const String& filename, File::OpenMode open_mode = std::ios::in)
				throw(Exception::FileNotFound);

			/** Destructor
			 */
			virtual ~GAMESSDatFile()
				throw();
			
			//@}
			/** @name Assignment.
			 */
			//@{
			
			/** Assignment operator.
			 */
			const GAMESSDatFile& operator = (const GAMESSDatFile& rhs)
				throw(Exception::FileNotFound);

			//@}

			/** @name Reading and Writing of Kernel Datastructures
			 */
			//@{
			
			/** Write a molecule to a GAMESSDatFile.
			 *  If additional GAMESS - keywords are stored in this class, they
			 *  will be written as well.
			 */
			virtual bool write(const Molecule& molecule)
				throw(File::CannotWrite);

			/** Write a system to a GAMESSDatFile.
			 *  If additional GAMESS - keywords are stored in this class, they
			 *  will be written as well.
			 */
			virtual bool write(const System& molecule)
				throw(File::CannotWrite);

			/** Read a Molecule from the GAMESSDatFile.
			 *  If the GAMESS .dat - file contains additional lines apart from the
			 *  molecule itself, they are stored in this class.
			 */
			virtual Molecule* read()
				throw(Exception::ParseError);

			/** Read a System from the GAMESSDatFile.
			 *  If the GAMESS .dat - file contains additional lines apart from the
			 *  molecule itself, they are stored in this class.
			 */
			virtual bool read(System& system)
				throw(Exception::ParseError);

			//@}

			/** @name Accessors
			 */
			//@{
			void newMolecule()
				throw();

			void setMoleculeName(char* name)
				throw();			
			
			void insertAtom(char* element, float charge, float x, float y, float z)
				throw();

			void insertBond(Index a1, Index a2)
				throw();
			//@}
			
			static State state;

		protected:
			virtual void initRead_();

			Molecule*	molecule_;
	};
}

#endif // BALL_FORMAT_GAMESSDATFILE_H
