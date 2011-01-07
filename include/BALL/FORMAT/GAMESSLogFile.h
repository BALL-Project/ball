// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
			GAMESSLogFile();

			/** Copy constructor
		   *  @throw Exception::FileNotFound if the file could not be opened
			 */
			GAMESSLogFile(const GAMESSLogFile& file);

			/** Detailed constructor
		   *  @throw Exception::FileNotFound if the file could not be opened
			 */
			GAMESSLogFile(const String& filename, File::OpenMode open_mode = std::ios::in);

			/** Destructor
			 */
			virtual ~GAMESSLogFile();
			
			//@}
			/** @name Assignment.
			 */
			//@{
			
			/** Assignment operator.
		   *  @throw Exception::FileNotFound if the file could not be opened
			 */
			const GAMESSLogFile& operator = (const GAMESSLogFile& rhs);

			//@}

			/** @name Reading and Writing of Kernel Logastructures
			 */
			//@{
			
			/** Write a molecule to a GAMESSLogFile.
			 *  If additional GAMESS - keywords are stored in this class, they
			 *  will be written as well.
		 	 *  @throw File::CannotWrite if writing to the file failed
			 */
			virtual bool write(const Molecule& molecule);

			/** Write a system to a GAMESSLogFile.
			 *  If additional GAMESS - keywords are stored in this class, they
			 *  will be written as well.
		 	 *  @throw File::CannotWrite if writing to the file failed
			 */
			virtual bool write(const System& molecule);

			/** Read a Molecule from the GAMESSLogFile.
			 *  If the GAMESS .log - file contains additional lines apart from the
			 *  molecule itself, they are stored in this class.
		   *  @throw Exception::ParseError if a syntax error was encountered
			 */
			virtual Molecule* read();

			/** Read a System from the GAMESSLogFile.
			 *  If the GAMESS .log - file contains additional lines apart from the
			 *  molecule itself, they are stored in this class.
		   *  @throw Exception::ParseError if a syntax error was encountered
			 */
			virtual bool read(System& system);

			//@}

			/** @name Accessors
			 */
			//@{
			void newMolecule();

			void setMoleculeName(char* name);			
			
			void insertAtom(const String& element, float charge, float x, float y, float z);

			void insertBond(Index a1, Index a2);

			void clearBonds();

			void setCurrentCharge(float charge);

			void setUnitConversionFactor(float factor);

			void addCoefficient(float coefficient);

			void initializeBasisSet();

			QMBasisSet& getBasisSet();

			const QMBasisSet& getBasisSet() const;

			void addBasisOption(const String& key, const String& value);

			String getBasisOption(const String& key);
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
