// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GAMESSLogFile.h,v 1.1 2005/08/23 18:20:05 anhi Exp $
//

#ifndef BALL_FORMAT_GAMESSLOGFILE_H
#define BALL_FORMAT_GAMESSLOGFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

namespace BALL
{
	/** GAMESSLog file class.
	 * 	This class enables BALL to read and write input and output .log files for
	 * 	the GAMESS quantum chemistry application.
	 *
	 * 	\ingroup StructureFormats
	 */
	class BALL_EXPORT GAMESSLogFile
		: public GenericMolFile
	{
		public:

			BALL_CREATE(GAMESSLogFile)

			/** State of the parser **/
			struct State
			{
				GAMESSLogFile* current_parser;
			};

			/** @name Constructors and Destructors
			 */
			//@{
			
			/** Default constructor
			 */
			GAMESSLogFile()
				throw();

			/** Copy constructor
			 */
			GAMESSLogFile(const GAMESSLogFile& file)
				throw(Exception::FileNotFound);

			/** Detailed constructor
			 */
			GAMESSLogFile(const String& filename, File::OpenMode open_mode = std::ios::in)
				throw(Exception::FileNotFound);

			/** Destructor
			 */
			virtual ~GAMESSLogFile()
				throw();
			
			//@}
			/** @name Assignment.
			 */
			//@{
			
			/** Assignment operator.
			 */
			const GAMESSLogFile& operator = (const GAMESSLogFile& rhs)
				throw(Exception::FileNotFound);

			//@}

			/** @name Reading and Writing of Kernel Logastructures
			 */
			//@{
			
			/** Write a molecule to a GAMESSLogFile.
			 *  If additional GAMESS - keywords are stored in this class, they
			 *  will be written as well.
			 */
			virtual bool write(const Molecule& molecule)
				throw(File::CannotWrite);

			/** Write a system to a GAMESSLogFile.
			 *  If additional GAMESS - keywords are stored in this class, they
			 *  will be written as well.
			 */
			virtual bool write(const System& molecule)
				throw(File::CannotWrite);

			/** Read a Molecule from the GAMESSLogFile.
			 *  If the GAMESS .log - file contains additional lines apart from the
			 *  molecule itself, they are stored in this class.
			 */
			virtual Molecule* read()
				throw(Exception::ParseError);

			/** Read a System from the GAMESSLogFile.
			 *  If the GAMESS .log - file contains additional lines apart from the
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
			
			void insertAtom(const String& element, float charge, float x, float y, float z)
				throw();

			void insertBond(Index a1, Index a2)
				throw();

			void setCurrentCharge(float charge)
				throw();

			void setUnitConversionFactor(float factor)
				throw();
			//@}
			
			static State state;
			Index  current_atom;
			bool	 molecule_already_defined;
			
		protected:
			virtual void initRead_();

			Molecule*	molecule_;
			float			factor_;
	};
}

#endif // BALL_FORMAT_GAMESSLOGFILE_H
