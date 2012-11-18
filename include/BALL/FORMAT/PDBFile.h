// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PDBFILE_H
#define BALL_FORMAT_PDBFILE_H

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_DATATYPE_QUADRUPLE_H
#	include <BALL/DATATYPE/quadruple.h>
#endif

#ifndef BALL_FORMAT_PDBDEFS_H
#	include <BALL/FORMAT/PDBdefs.h>
#endif

#ifndef BALL_FORMAT_PDBINFO_H
#	include <BALL/FORMAT/PDBInfo.h>
#endif

#ifndef BALL_KERNEL_PDBAtom_H
#	include <BALL/KERNEL/PDBAtom.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif

namespace BALL
{

	/** PDB file class.
			 \par
			
	*/
	class BALL_EXPORT PDBFile
		: public GenericMolFile,
			public PropertyManager
	{
		public:

		/**	Symbolic names for option keys.
				This struct contains a symbolic name
				for each recognized key in PDBFile::options. \par
				For each symbol the required type is given under parameters.
		*/
		struct BALL_EXPORT Option 
		{
			/**	The verbosity level.
					Use integer values with this option.
					0 = terse \par
					99 = tell me everything
					@see		Default::VERBOSITY
			*/
			static const char* VERBOSITY;

			/**	Enable strict line checking. [Not yet implemented]
					This option turns line checking on or off. Line checking is a
					very simple test for the correct length of input lines. The
					default is to keep line checking turned off. 
					@see		Default::LINE_CHECKING
			*/
			static const char* STRICT_LINE_CHECKING;
			
			/** Choose a model. 
					If a PDB file contains several models, choose the model you want
					to have read via this option. Default is the first model
					appearing, denoted by Index 0. If you want to read all models,
					choose index -1.
					@see		Default::CHOOSE_MODEL
			*/
			static const char* CHOOSE_MODEL;

			/**	Store skipped records in info.
					If this option is set, all unparsed records will end up 
					in the skipped record list of info.
					@see		Default::STORE_SKIPPED_RECORDS
			*/
			static const char* STORE_SKIPPED_RECORDS;

			/**	Ignore XPLOR pseudo atoms.
					If this option is set XPLOR pseudo atoms (recognized
					by their x/y/z coordinates of 9999.0) will be dropped.
					@see		Default::IGNORE_XPLOR_PSEUDO_ATOMS
			*/
			static const char* IGNORE_XPLOR_PSEUDO_ATOMS;

			/**	Parse partial charges in columns 76-80.
					Some variants of PDB like to store atom partial charges in these
					columns (according to the standard, these columns contain 
					the element symbol and formal charges). 
			*/
			static const char* PARSE_PARTIAL_CHARGES;

			/**	Write PDB-CONECT records according to the format guidelines of 1996.
					default is supporting the current format from 2006. 
					@see		Default::WRITE_PDBFORMAT_1996
			*/
			static const char* WRITE_PDBFORMAT_1996;
		};

		/** Default values for PDBFile options.  
		*/
		struct BALL_EXPORT Default 
		{
			/** Default verbosity level.
					0 - shut up!
					@see	Option::VERBOSITY
			*/
			static const Index VERBOSITY;

			/** Default for strict line checking option.
					false - don't check lines lengths.
					@see	Option::STRICT_LINE_CHECKING
			*/
			static const bool STRICT_LINE_CHECKING;

			/** Default model to read from file.
					0 - read the first model from the file
					@see	Option::CHOOSE_MODEL
			*/
			static const Index CHOOSE_MODEL;

			/**	Store skipped records in info.
					true -- store everything we cannot interpret.
			*/
			static const bool STORE_SKIPPED_RECORDS;

			/**	Ignore XPLOR pseudo atoms.
					true -- skip them.
			*/
			static const bool IGNORE_XPLOR_PSEUDO_ATOMS;

			/**	Parse partial charges.
					false -- stick to the standard.
			*/
			static const bool PARSE_PARTIAL_CHARGES;

			/**	Enable writing PDB-CONECT records according to the format guidelines of 1996.
					default is supporting the current format from 2006. 
					@see		Default::WRITE_PDBFORMAT_1996
			*/
			static const bool WRITE_PDBFORMAT_1996;

		};

		/** @name Options
		*/
		//@{
		/// The options for parsing the file
		Options options;
		/// Summary information on the last file read
		PDBInfo info;
		//@}

		/** @name Constructurs and destructor.
		*/
		//@{

		/** Default constructor
		*/
		PDBFile();
			
		/** File-like constructor
		 *  @throw Exception::FileNotFound if the file could not be opened
		 */
		PDBFile(const String& filename, File::OpenMode open_mode = std::ios::in);

		/** Construct with options.
		 */
		PDBFile(const Options& new_options);

		/** Destructor
		 */
		virtual ~PDBFile();
		//@}

		/** @name Assignment.
		*/
		//@{

		/// Clear the stream state.
		virtual void clear(int state);

		/// Clear all members.
		virtual void clear();

		//@}

		/** @name Accessors.
		*/
		//@{

		/// Returns the version number this PDB file reader is able to read.
		virtual float getVersion() const;

		void open(const String& name, File::OpenMode open_mode=std::ios::in);

		/** Selects one of multiple models for reading. Default behaviour is
				reading model 1. If a file does not contain a MODEL specifier, all
				coordinates will be assigned to model 1.
		*/
		void selectModel(Index index);

		/**
		 * This function allows to select the alternate location indicator
		 * That should be parsed if one is specified. Default is 'A'.
		 *
		 * If other location indicators exist, but the specified one is missing
		 * for an atom nothing will be parsed. Thus this option should be used
		 * with caution.
		 *
		 * @throw IllegalArgument This exception is thrown if an invalid
		 *        Location indicator (not a letter) was passed.
		 */
		void selectAltLocationIndicator(char c);

		/// Return the model currently selected
		Index getSelectedModel() const;

		/// Selects all models for reading.
		void selectAllModels();

		/// Returns the number of the model weare currently reading. (?????)
		Index getCurrentModel() const;

		/// Return a mutable pointer to the line buffer.
		char* getRecordString();

		/// Return a constant pointer to the line buffer.
		const char* getRecordString() const;

		/// Returns the number of the record we are currently reading.
		Index getRecordNumber() const;

		/// Returns the type of the record we are currently reading.
		PDB::RecordType getRecordType() const;

		/** Returns the element symbol of an atom. If element_symbol is
				invalid we try to reconstruct the element symbol from the atom name.
		*/
		virtual const char* getAtomElementSymbol
			(const PDB::Atom atom_name,
			 PDB::Atom element_symbol);

		/// Returns the atom name.
		virtual const char* getAtomName
			(const PDB::Atom atom_name);

		/// Extract the PDB remoteness indicator of an atom record
		virtual char getAtomRemotenessIndicator
			(const PDB::Atom atom_name);

		/// Extract the PDB branch designator of an atom record
		virtual short getAtomBranchDesignator
			(const PDB::Atom atom_name);

		/// Returns the number of record fields.
		Size countRecordFields() const;

		/** Returns the number of records of type record_type in the file being
				read.
		*/
		Size countRecord
			(PDB::RecordType record_type,
			 bool from_begin_of_file = true);

		/** Returns the number of all records in the file.
		*/
		Size countRecords
			(bool from_begin_of_file = true);
		//@}

		/** @name Methods for parsing individual records.
				These methods are typically required if you want to 
				implement your own PDB parser (derived from PDBFile or
				PDBFile).
		*/
		//@{
		/// Parse a line from a PDBFile. This is a helper function for readLine().
		bool parseLine(const char* line, Size size, const char* format_string, ...);

		/// Read and parse a line from a PDB file.
		bool readLine(char* line, Size size, bool extract_values);

		/// Read the firts record of a file. 
		bool readFirstRecord(bool read_values = true);

		/// Read the next record of a PDB file.
		bool readNextRecord(bool read_values = true);

		/** Reads all records of a file.
				@return true if all records were parsed without error
				@return false otherwise
		*/
		bool readRecords();

		/** Called for unknown PDB records.
				The default implementation does nothing, it always returns true.
				@return true
		*/
		virtual bool readUnknownRecord(const char* line);

		/** Called an invalid record by simply returning true.
		*/
		virtual bool readInvalidRecord(const char* line);

		/**	Skip the current record.
				If storeSkippedRecords is set to true, the records are
				added to the skipped records of info. Otherwise, they are
				just ignored. This method should be called by
				those readRecord methods that want to skip a particular record
				(this is the default behavior implemented in PDBFile).
				@return true
		*/
		bool skipCurrentRecord();

		/// Reads an anisotropic temperature factor record.
		virtual bool interpretRecord(const PDB::RecordANISOU& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordANISOU& record);
		virtual bool parseRecordANISOU(const char* line, Size size);

		/// Reads an atom record.
		virtual bool interpretRecord(const PDB::RecordATOM& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordATOM& record);
		virtual bool parseRecordATOM(const char* line, Size size);

		/// Reads an author record.
		virtual bool interpretRecord(const PDB::RecordAUTHOR& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordAUTHOR& record);
		virtual bool parseRecordAUTHOR(const char* line, Size size);

		/// Reads a caveat record.
		virtual bool interpretRecord(const PDB::RecordCAVEAT& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordCAVEAT& record);
		virtual bool parseRecordCAVEAT(const char* line, Size size);

		/// Reads a record specifying peptides in cis conformation.
		virtual bool interpretRecord(const PDB::RecordCISPEP& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordCISPEP& record);
		virtual bool parseRecordCISPEP(const char* line, Size size);

		/// Reads the title record containing macroscopic compoubd information.
		virtual bool interpretRecord(const PDB::RecordCOMPND& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordCOMPND& record);
		virtual bool parseRecordCOMPND(const char* line, Size size);

		/// Reads a connection record.
		virtual bool interpretRecord(const PDB::RecordCONECT& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordCONECT& record);
		virtual bool parseRecordCONECT(const char* line, Size size);

		/** Reads the record specifying the unit cell parameters, space group,
				and Z value for crystographically determined structures.
		*/
		virtual bool interpretRecord(const PDB::RecordCRYST1& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordCRYST1& record);
		virtual bool parseRecordCRYST1(const char* line, Size size);

		/// Reads a record containing database cross-reference links.
		virtual bool interpretRecord(const PDB::RecordDBREF& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordDBREF& record);
		virtual bool parseRecordDBREF(const char* line, Size size);

		/// Reads the record defining the end of a PDB file.
		virtual bool interpretRecord(const PDB::RecordEND& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordEND& record);
		virtual bool parseRecordEND(const char* line, Size size);

		/// Reads a model ending record.
		virtual bool interpretRecord(const PDB::RecordENDMDL& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordENDMDL& record);
		virtual bool parseRecordENDMDL(const char* line, Size size);

		/// Reads a record containing data about the experiment.
		virtual bool interpretRecord(const PDB::RecordEXPDTA& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordEXPDTA& record);
		virtual bool parseRecordEXPDTA(const char* line, Size size);

		/** Reads a record containing the chemical formula of a non-standard
				group.
		*/
		virtual bool interpretRecord(const PDB::RecordFORMUL& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordFORMUL& record);
		virtual bool parseRecordFORMUL(const char* line, Size size);

		/// Reads a record containing a footnote.
		virtual bool interpretRecord(const PDB::RecordFTNOTE& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordFTNOTE& record);
		virtual bool parseRecordFTNOTE(const char* line, Size size);

		/** Reads the header recors which contains the idCode field, entry
				classification and deposition date.
		*/
		virtual bool interpretRecord(const PDB::RecordHEADER& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHEADER& record);
		virtual bool parseRecordHEADER(const char* line, Size size);

		/// Reads a helix defining record.
		virtual bool interpretRecord(const PDB::RecordHELIX& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHELIX& record);
		virtual bool parseRecordHELIX(const char* line, Size size);

		/// Reads a record defining a non-standard residue.
		virtual bool interpretRecord(const PDB::RecordHET& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHET& record);
		virtual bool parseRecordHET(const char* line, Size size);

		/// Reads atomic coordinates for atoms in non-standard groups.
		virtual bool interpretRecord(const PDB::RecordHETATM& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHETATM& record);
		virtual bool parseRecordHETATM(const char* line, Size size);

		/// Reads a record defining the name of a non-standard group.
		virtual bool interpretRecord(const PDB::RecordHETNAM& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHETNAM& record);
		virtual bool parseRecordHETNAM(const char* line, Size size);

		/// Reads a record defining synonyms of a non-standard group.
		virtual bool interpretRecord(const PDB::RecordHETSYN& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHETSYN& record);
		virtual bool parseRecordHETSYN(const char* line, Size size);

		/// Reads a record defining a hydrogen bond.
		virtual bool interpretRecord(const PDB::RecordHYDBND& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordHYDBND& record);
		virtual bool parseRecordHYDBND(const char* line, Size size);

		/// Reads a record containing a journal reference.
		virtual bool interpretRecord(const PDB::RecordJRNL& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordJRNL& record);
		virtual bool parseRecordJRNL(const char* line, Size size);

		/// Reads a record containing keywords for this entry.
		virtual bool interpretRecord(const PDB::RecordKEYWDS& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordKEYWDS& record);
		virtual bool parseRecordKEYWDS(const char* line, Size size);

		/// Reads a record containing supplemental connectivity information.
		virtual bool interpretRecord(const PDB::RecordLINK& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordLINK& record);
		virtual bool parseRecordLINK(const char* line, Size size);

		/** Reads the master record containing numerous counts for bookkeeping
				purposes.
		*/
		virtual bool interpretRecord(const PDB::RecordMASTER& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordMASTER& record);
		virtual bool parseRecordMASTER(const char* line, Size size);

		/// Reads a record indicating the beginning of a new model.
		virtual bool interpretRecord(const PDB::RecordMODEL& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordMODEL& record);
		virtual bool parseRecordMODEL(const char* line, Size size);

		/// Reads a record identifying residue modifications.
		virtual bool interpretRecord(const PDB::RecordMODRES& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordMODRES& record);
		virtual bool parseRecordMODRES(const char* line, Size size);

		/** Reads a record defining transformations expressing
				non-crystallographic symmetry.
		*/
		virtual bool interpretRecord(const PDB::RecordMTRIX1& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordMTRIX1& record);
		virtual bool parseRecordMTRIX1(const char* line, Size size);

		/** Reads a record defining transformations expressing
				non-crystallographic symmetry.
		*/
		virtual bool interpretRecord(const PDB::RecordMTRIX2& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordMTRIX2& record);
		virtual bool parseRecordMTRIX2(const char* line, Size size);

		/** Reads a record defining transformations expressing
				non-crystallographic symmetry.
		*/
		virtual bool interpretRecord(const PDB::RecordMTRIX3& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordMTRIX3& record);
		virtual bool parseRecordMTRIX3(const char* line, Size size);

		/** Reads a record indicating that this structura has been
				withdrawnform the database.
		*/
		virtual bool interpretRecord(const PDB::RecordOBSLTE& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordOBSLTE& record);
		virtual bool parseRecordOBSLTE(const char* line, Size size);

		/** Reads a record defining the transformation from the orthogonal
				coordinates contained in the entry to the submitted coordinates.
		*/
		virtual bool interpretRecord(const PDB::RecordORIGX1& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordORIGX1& record);
		virtual bool parseRecordORIGX1(const char* line, Size size);

		/** Reads a record defining the transformation from the orthogonal
				coordinates contained in the entry to the submitted coordinates.
		*/
		virtual bool interpretRecord(const PDB::RecordORIGX2& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordORIGX2& record);
		virtual bool parseRecordORIGX2(const char* line, Size size);

		/** Reads a record defining the transformation from the orthogonal
				coordinates contained in the entry to the submitted coordinates.
		*/
		virtual bool interpretRecord(const PDB::RecordORIGX3& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordORIGX3& record);
		virtual bool parseRecordORIGX3(const char* line, Size size);

		/// Reads a record containing remarks.
		virtual bool interpretRecord(const PDB::RecordREMARK& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordREMARK& record);
		virtual bool parseRecordREMARK(const char* line, Size size);

		/// Reads a record containing a revision history.
		virtual bool interpretRecord(const PDB::RecordREVDAT& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordREVDAT& record);
		virtual bool parseRecordREVDAT(const char* line, Size size);

		/// Reads a scale transformation record.
		virtual bool interpretRecord(const PDB::RecordSCALE1& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSCALE1& record);
		virtual bool parseRecordSCALE1(const char* line, Size size);

		/// Reads a scale transformation record.
		virtual bool interpretRecord(const PDB::RecordSCALE2& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSCALE2& record);
		virtual bool parseRecordSCALE2(const char* line, Size size);

		/// Reads a scale transformation record.
		virtual bool interpretRecord(const PDB::RecordSCALE3& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSCALE3& record);
		virtual bool parseRecordSCALE3(const char* line, Size size);

		/// Sequence information conflicts between atom records and dbref content
		virtual bool interpretRecord(const PDB::RecordSEQADV& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSEQADV& record);
		virtual bool parseRecordSEQADV(const char* line, Size size);

		/// Reads a record containing the sequence of residues.
		virtual bool interpretRecord(const PDB::RecordSEQRES& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSEQRES& record);
		virtual bool parseRecordSEQRES(const char* line, Size size);

		/// Reads a record defining a beta-sheet.
		virtual bool interpretRecord(const PDB::RecordSHEET& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSHEET& record);
		virtual bool parseRecordSHEET(const char* line, Size size);

		/// Reads a record giving the standard deviation of atomic coordinates.
		virtual bool interpretRecord(const PDB::RecordSIGATM& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSIGATM& record);
		virtual bool parseRecordSIGATM(const char* line, Size size);

		/** Reads a record giving the standard deviation of anisotropic
				temperature factors.
		*/
		virtual bool interpretRecord(const PDB::RecordSIGUIJ& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSIGUIJ& record);
		virtual bool parseRecordSIGUIJ(const char* line, Size size);

		/// Reads a record containing groups comprising a site.
		virtual bool interpretRecord(const PDB::RecordSITE& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSITE& record);
		virtual bool parseRecordSITE(const char* line, Size size);

		/// Reads a record defining a salt bridge.
		virtual bool interpretRecord(const PDB::RecordSLTBRG& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSLTBRG& record);
		virtual bool parseRecordSLTBRG(const char* line, Size size);

		/** Reads a record specifies the chemical/biological source of each
				molecule in the entry.
		*/
		virtual bool interpretRecord(const PDB::RecordSOURCE& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSOURCE& record);
		virtual bool parseRecordSOURCE(const char* line, Size size);

		/// List of entries this file supersedes
		virtual bool interpretRecord(const PDB::RecordSPRSDE& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSPRSDE& record);
		virtual bool parseRecordSPRSDE(const char* line, Size size);

		/// Reads a record specifying a disulfide bond.
		virtual bool interpretRecord(const PDB::RecordSSBOND& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordSSBOND& record);
		virtual bool parseRecordSSBOND(const char* line, Size size);

		/// Reads a record terminating molecule.
		virtual bool interpretRecord(const PDB::RecordTER& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordTER& record);
		virtual bool parseRecordTER(const char* line, Size size);

		/// Reads a record specifying the title of the experiment or anaysis.
		virtual bool interpretRecord(const PDB::RecordTITLE& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordTITLE& record);
		virtual bool parseRecordTITLE(const char* line, Size size);

		/// Reads a record specifying a turn.
		virtual bool interpretRecord(const PDB::RecordTURN& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordTURN& record);
		virtual bool parseRecordTURN(const char* line, Size size);

		/** Reads a record specifying the translation vector for infinite
				covalently connected structures.
		*/
		virtual bool interpretRecord(const PDB::RecordTVECT& record);
		virtual bool fillRecord(const char* line, Size size, PDB::RecordTVECT& record);
		virtual bool parseRecordTVECT(const char* line, Size size);
		//@}

		/// Check whether the current file is in PDB format
		virtual bool hasFormat();

		typedef HashMap<PDB::Integer, PDBAtom*> PDBAtomMap;
		typedef Quadruple<String, PDB::Character, PDB::Integer, PDB::AChar> ResidueQuadruple;
		typedef HashMap<ResidueQuadruple, Residue*> ResidueMap;
		typedef std::list<ResidueQuadruple> QuadrupleList;
		typedef std::list<SecondaryStructure*> SecStructList;
		

		/**	@name Reading and writing */
		//@{

		/** Read a protein from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		PDBFile& operator >> (Protein& protein);

		/** Read a molecule from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		PDBFile& operator >> (Molecule& molecule);

		/** Read system from the file
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		PDBFile& operator >> (System& system);

		/** Write a protein to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		PDBFile& operator << (const Protein& protein);

		/** Write a system to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		PDBFile& operator << (const System& system);

		/** Write molecule to the file
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		PDBFile& operator << (const Molecule& molecule);

		/** Read a Protein from the file
		 *  return true if the Protein was read successfully
		 *  return false otherwise
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool read(Protein& protein);

		/** Read a Molecule from the file
		 *  return true if the Molecule was read successfully
		 *  return false otherwise
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool read(Molecule& protein);

		/** Read a System from the file
		 *  return true if the System was read successfully
		 *  return false otherwise
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		bool read(System& system);

		/** Read a Molecule from the file
		 *  @return Molecule* pointer to the newly created Molecule. 0 if no molecule could be read or
		 *          if no further Molecule could be read.
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		Molecule* read();

		/** Write a Protein to the file
		 *  return true if the writing finished successfully
		 *  return false otherwise
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		bool write(const Protein& protein);

		/** Write a Molecule to the file
		 *  return true if the writing finished successfully
		 *  return false otherwise
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		bool write(const Molecule& molecule);

		/** Write a System to the file
		 *  return true if the writing finished successfully
		 *  return false otherwise
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		bool write(const System& system);

		/** Write a System to the file
		 *  return true if the writing finished successfully
		 *  return false otherwise
		 *  @throw File::CannotWrite if writing to the file failed
		 */
		bool write(const System& system, const PDBInfo& info);
		//@}

		protected:

		//_
		PDBFile(const File& pdbf);

		//_
		PDBFile& operator = (const PDBFile& pdbf);

		/*_ A helper function for initializing certain data structures. Used by
				constructors.
		*/
		void init_() ;

		void postprocessSSBonds_();
		void postprocessHelices_();
		void postprocessSheetsTurns_(QuadrupleList& sectruct_list, SecStructList& new_secstruct_list);
		void postprocessRandomCoils_();

		

		// Method related to the writing of PDB files

		// 
		void write_(const AtomContainer& ac, const PDBInfo& info = PDBInfo());

		/** Write a record to the stream using a predefined record type.
				This method updates the internal book keeping data structure required
				for writing the MASTER record.
				@see PDB::RecordType
		*/
		void writeRecord_(PDB::RecordType record, ...);
		void writeCONECTRecords_(PDB::Structure::ConectAtomList& cl);

		/** Write a record to the stream using a predefined format string.
				You should not use this method unless you know what you are doing.
				Use \link writeRecord_ \endlink instead, as this method also updates
				the record book keeping required for the MASTER record.
				If you use it, you'll have to update the book keeping structure yourself!
		*/
		void writeRawRecord_(const char* format, const char* tag, ...);
		
		/// Add all records of a specific type in the info object to the current stream.
		void addAllRecords_(const PDBInfo& info, PDB::RecordType type);
		
		/**	Write a SEQRES record to the current stream */
		void writeRecord_(const PDB::RecordSEQRES& seqres);
		/**	Write a HELIX record to the current stream */
		void writeRecord_(const PDB::RecordHELIX& helix);
		/**	Write a SHEET record to the current stream */
		void writeRecord_(const PDB::RecordSHEET& helix);
		/**	Write a TURN record to the current stream */
		void writeRecord_(const PDB::RecordTURN& helix);
		/**	Write a SSBOND record to the current stream */
		void writeRecord_(const PDB::RecordSSBOND& helix);
		/**	Write a CONECT record to the current stream */
		void writeRecord_(const PDB::RecordCONECT& helix);
		/**	Write a CRYST1 record to the current stream */
		void writeRecord_(const PDB::RecordCRYST1& cryst1);
		/**	Write a MTRIX1 record to the current stream */
		void writeRecord_(const PDB::RecordMTRIX1& mtrix1);
		/**	Write a MTRIX2 record to the current stream */
		void writeRecord_(const PDB::RecordMTRIX2& mtrix2);
		/**	Write a MTRIX3 record to the current stream */
		void writeRecord_(const PDB::RecordMTRIX3& mtrix3);
		
		void writeAtom_(const PDB::Structure::AtomEntry& atom, PDB::AdditionalAtomInfo& cr, bool hetatm = false);
		
		void writeTitleSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writePrimaryStructureSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeHeterogenSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeSecondaryStructureSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeConnectivityAnnotationSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeMiscellaneousFeaturesSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeCrystallographicSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeCoordinateSection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeConnectivitySection_(const PDB::Structure& structure, const PDBInfo& info);
    void writeBookKeepingSection_(const PDB::Structure& structure, const PDBInfo& info);

		void writeSEQRESSection_(const std::vector<std::pair<char, String> >& chain_residues);
		void writeHELIXSection_(const PDB::Structure& structure);
		void writeSHEETSection_(const PDB::Structure& structure);
		void writeTURNSection_(const PDB::Structure& structure);
		void writeSSBONDSection_(const PDB::Structure& structure);
		void writeHYDBNDSection_(const PDB::Structure& structure);
		void writeSLTBRGSection_(const PDB::Structure& structure);
		void writeCRYST1Section_(const PDB::Structure& structure);
		void writeMTRIXnSection_(const PDB::Structure& structure);

		/**	Extract the atom and bond information from an atom container prior to writing a PDB file.
				This method is called by \link write_ \endlink.
		*/
		static void extractStructure_(const AtomContainer& ac, PDB::Structure& structure);
		
		/**	Determine whether an atom is a hetero atom or part of a standard amino acid or nucleotide.
				This test is performed by checking for the corresponding properties defined in Residue.
		*/
		static bool isHeteroAtom_(const Atom& atom);
		
		/**	Compute the current name and id of chain and residue.
				This method guesses more or less correct values for names and IDs or uses decent
				default values. It does so only of the pointers to the residue and chain in cr
				differ from the values in atom.
		*/
		static void updateAdditionalAtomInfo_(const PDB::Structure::AtomEntry& atom, PDB::AdditionalAtomInfo& cr);
			

		char line_buffer_[PDB::SIZE_OF_PDB_LINE_BUFFER];

		String residue_name_;
		Index residue_sequence_number_;
		Index current_model_;
		Index current_record_;
		Size record_fields_;
		PDB::RecordType current_record_type_;
		PDB::RecordTypeFormat compare_record_type_format_; 

		PDBAtomMap PDB_atom_map_;
		ResidueMap residue_map_;
		QuadrupleList ssbond_list_;
		QuadrupleList helix_list_;
		QuadrupleList sheet_list_;
		QuadrupleList turn_list_;

		SecStructList new_helix_secstruc_list_;
		SecStructList new_sheet_secstruc_list_;
		SecStructList new_turn_secstruc_list_;

		char alternate_location_indicator_;
		PDB::AChar insertion_code_;
		PDB::Character chain_ID_;
		PDB::Integer sequence_number_;
		PDB::AChar code_;
		Protein* current_protein_;
		const Protein* current_const_protein_;
		Chain* current_chain_;
		const Chain* current_const_chain_;
		Residue*	current_residue_;
		const Residue* current_const_residue_;
		PDBAtom* current_PDB_atom_;
		const Atom* current_const_atom_;
		String name_;
		HashMap<const Atom*, Position> atom_map_;
		
		/// Book keeping struct for the number of records written
		PDB::BookKeeping book_keeping_;

		///_Verbosity level
		int verbosity_;

		bool strict_line_checking_;

		/// The selected model_
		Index selected_model_;

		/// Store the skipped records in info?  
		bool store_skipped_records_;

		/// Ignore XPLOR pseudo atoms?
		bool ignore_xplor_pseudo_atoms_;
		
		/// Read partial charges from cols 76-80?
		bool parse_partial_charges_;

		///write in the 1996-format?
		bool write_pdbformat_1996_;

		/// Has the Protein already been read by use of read() ?
		/// If yes, the next call of read() will return 0. This is necesassry, since else the use of the GenericMolFile interface-function read() can result in endless loops.
		bool read_done_;
	};





	/** Hash 
	* \ingroup  StructureFormats
  */
	inline
	BALL_EXPORT HashIndex Hash(const PDBFile::ResidueQuadruple& quadruple)
		
	{
		return (Index)quadruple.third;
	}

	/** Operator <<
	 		\ingroup StructureFormats
	*/
	inline
	BALL_EXPORT std::ostream& operator << (std::ostream& s, const Quadruple<String,PDB::Character,PDB::Integer,PDB::AChar>& /* q */)
	{
		return s;
	}



#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL

#endif // BALL_FORMAT_PDBFILE_H
