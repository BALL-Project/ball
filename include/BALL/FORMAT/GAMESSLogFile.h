// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GAMESSLogFile.h,v 1.2 2005/10/05 09:59:46 anhi Exp $
//

#ifndef BALL_FORMAT_GAMESSLOGFILE_H
#define BALL_FORMAT_GAMESSLOGFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_STRUCTURE_QMBASISSET_H
# include <BALL/STRUCTURE/QMBasisSet.h>
#endif

#ifndef BALL_STRUCTURE_SPLITVALENCESET_H
# include <BALL/STRUCTURE/splitValenceSet.h>
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

			void clearBonds()
				throw();

			void setCurrentCharge(float charge)
				throw();

			void setUnitConversionFactor(float factor)
				throw();

			void addCoefficient(float coefficient)
				throw();

			void initializeBasisSet()
				throw();

			QMBasisSet& getBasisSet()
				throw();

			const QMBasisSet& getBasisSet() const
				throw();

			void addBasisOption(const String& key, const String& value)
				throw();

			String getBasisOption(const String& key)
				throw();
			//@}
			
			static State state;
			Index  current_atom;
			Index  current_set;
			Index  current_coefficient_line;
			Size	 basis_size;
			bool	 molecule_already_defined;
			
			/** All of this stuff should not really be placed _here_...
			 *  we need a data structure for QM data sets that stores
			 *  all the stuff here.
			 */
			System *system; // needed for the datasetControl stuff
		protected:
			virtual void initRead_();

			Molecule*		molecule_;
			float				factor_;
			//QMBasisSet  qmbs_;
			splitValenceSet  qmbs_;

			StringHashMap<String> basis_options_;
	};
}

#endif // BALL_FORMAT_GAMESSLOGFILE_H
