// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.h,v 1.29 2005/02/11 15:27:12 oliver Exp $
//

#ifndef BALL_FORMAT_PDBFILE_H
#define BALL_FORMAT_PDBFILE_H

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
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
	class PDBFile
		: public File,
			public PropertyManager
	{
		public:

		/**	Symbolic names for option keys.
				This struct contains a symbolic name
				for each recognized key in PDBFile::options. \par
				For each symbol the required type is given under parameters.
		*/
		struct Option 
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
		};

		/** Default values for PDBFile options.  
		*/
		struct Default 
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
		/// Default constructor.
		PDBFile();
			
		/// File-like constructor
		PDBFile(const String& filename, File::OpenMode open_mode = File::IN) throw(Exception::FileNotFound);

		/// Construct with options.
		PDBFile(const Options& new_options);

		/// Copy constructor
		PDBFile(const PDBFile& file) throw(Exception::FileNotFound);
		
		/// Destructor
		virtual ~PDBFile()
			throw();
		//@}

		/** @name Assignment.
		*/
		//@{

		/// Clear method.
		virtual void clear(int state = 0);
		//@}

		/** @name Accessors.
		*/
		//@{

		/// Returns the version number this PDB file reader is able to read.
		virtual float getVersion() const;

		/** Selects one of multiple models for reading. Default behaviour is
				reading model 1. If a file does not contain a MODEL specifier, all
				coordinates will be assigned to model 1.
		*/
		void selectModel(Index index);

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
		virtual bool readRecordANISOU
			(PDB::Integer serial_number,
			 PDB::Atom atom_name,
			 PDB::Character alternate_location_indicator,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Integer u11,
			 PDB::Integer u22,
			 PDB::Integer u33,
			 PDB::Integer u12,
			 PDB::Integer u13,
			 PDB::Integer u23,
			 PDB::LString4 segment_ID,
			 PDB::LString2 element_symbol,
			 PDB::LString2 charge);

		/// Reads an atom record.
		virtual bool readRecordATOM
			(PDB::Integer serial_number,
			 PDB::Atom atom_name,
			 PDB::Character alternate_location_indicator,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Real orthogonal_vector[3],
			 PDB::Real occupancy,
			 PDB::Real temperature_factor,
			 PDB::LString4 segment_ID,
			 PDB::LString2 element_symbol,
			 PDB::LString2 charge);

		/// Reads an author record.
		virtual bool readRecordAUTHOR
			(PDB::Continuation continuation,
			 PDB::PDBList authors);

		/// Reads a cave-at record.
		virtual bool readRecordCAVEAT
			(PDB::Continuation continuation,
			 PDB::IDcode entry_code,
			 PDB::PDBString comment);

		/// Reads a record specifying peptides in cis conformation.
		virtual bool readRecordCISPEP
			(PDB::Integer record_serial_number,
			 PDB::RecordCISPEP::CisPeptide cis_peptide[2],
			 PDB::Integer specific_model_ID,
			 PDB::Real angle_measure);

		/// Reads the title record containing macroscopic compoubd information.
		virtual bool readRecordCOMPND
			(PDB::Continuation continuation,
			 PDB::SpecificationList component_description);

		/// Reads a connection record.
		virtual bool readRecordCONECT
			(PDB::Integer atom_serial_number,
			 PDB::Integer bonded_atom_serial_number[4],
			 PDB::Integer hydrogen_bonded_atom_serial_number[4],
			 PDB::Integer salt_bridged_atom_serial_number[2]);

		/** Reads the record specifying the unit cell parameters, space group,
				and Z value for crystographically determined structures.
		*/
		virtual bool readRecordCRYST1
			(PDB::RecordCRYST1::UnitCell& unit_cell);

		/// Reads a record containing database cross-reference links.
		virtual bool readRecordDBREF
			(PDB::IDcode entry_code,
			 PDB::Character chain_ID,
			 PDB::RecordDBREF::InitialSequence& initial_sequence,
			 PDB::RecordDBREF::EndingSequence& ending_sequence,
			 PDB::LString6 sequence_database_name,
			 PDB::LString8 sequence_database_accession_code,
			 PDB::LString12 sequence_database_ID_code,
			 PDB::RecordDBREF::InitialDatabaseSegment& initial_database_segment,
			 PDB::RecordDBREF::EndingDatabaseSegment& ending_database_segment);

		/// Reads the record defining the end of a PDB file.
		virtual bool readRecordEND();

		/// Reads a model ending record.
		virtual bool readRecordENDMDL();

		/// Reads a record containing data about the experiment.
		virtual bool readRecordEXPDTA
			(PDB::Continuation continuation,
			 PDB::SList technique);

		/** Reads a record containing the chemical formula of a non-standard
				group.
		*/
		virtual bool readRecordFORMUL
			(PDB::Integer component_number,
			 PDB::LString3 het_ID,
			 PDB::Integer continuation_number,
			 PDB::Character is_water,
			 PDB::PDBString chemical_formula);

		/// Reads a record containing a footnote.
		virtual bool readRecordFTNOTE
			(PDB::Integer number,
			 PDB::PDBString text);

		/** Reads the header recors which contains the idCode field, entry
				classification and deposition date.
		*/
		virtual bool readRecordHEADER
			(PDB::String40 classification,
			 PDB::Date deposition_date,
			 PDB::IDcode ID_code);

		/// Reads a helix defining record.
		virtual bool readRecordHELIX
			(PDB::Integer serial_number,
			 PDB::LString3 helix_ID,
			 PDB::RecordHELIX::InitialResidue& initial_residue,
			 PDB::RecordHELIX::TerminalResidue& terminal_residue,
			 PDB::Integer helix_class,
			 PDB::PDBString comment,
			 PDB::Integer length);

		/// Reads a record defining a non-standard residue.
		virtual bool readRecordHET
			(PDB::LString3 het_ID,
			 PDB::Character chain_ID,
			 PDB::Integer sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Integer number_of_HETATM_records,
			 PDB::PDBString text);

		/// Reads atomic coordinates for atoms in non-standard groups.
		virtual bool readRecordHETATM
			(PDB::Integer serial_number,
			 PDB::Atom atom_name,
			 PDB::Character alternate_location_indicator,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Real orthogonal_vector[3],
			 PDB::Real occupancy,
			 PDB::Real temperature_factor,
			 PDB::LString4 segment_ID,
			 PDB::LString2 element_symbol,
			 PDB::LString2 charge);

		/// Reads a record defining the name of a non-standard group.
		virtual bool readRecordHETNAM
			(PDB::Continuation continuation,
			 PDB::LString3 het_ID,
			 PDB::PDBString chemical_name);

		/// Reads a record defining a hydrogen bond.
		virtual bool readRecordHYDBND
			(PDB::RecordHYDBND::HydrogenPartnerAtom hydrogen_partner_atom[2],
			 PDB::RecordHYDBND::HydrogenAtom& hydrogen_atom,
			 PDB::SymmetryOperator first_non_hydrogen_atom,
			 PDB::SymmetryOperator second_non_hydrogen_atom);

		/// Reads a record containing a journal reference.
		virtual bool readRecordJRNL
			(PDB::LString text);

		/// Reads a record containing keywords for this entry.
		virtual bool readRecordKEYWDS
			(PDB::Continuation continuation,
			 PDB::PDBList keywords);

		/// Reads a record containing supplemental connectivity information.
		virtual bool readRecordLINK
			(PDB::RecordLINK::LinkPartner link_partner[2],
			 PDB::SymmetryOperator first_atom,
			 PDB::SymmetryOperator second_atom);

		/** Reads the master record containing numerous counts for bookkeeping
				purposes.
		*/
		virtual bool readRecordMASTER
			(PDB::Integer number_of_REMARK_records,
			 PDB::Integer zero,
			 PDB::Integer number_of_HET_records,
			 PDB::Integer number_of_HELIX_records,
			 PDB::Integer number_of_SHEET_records,
			 PDB::Integer number_of_TURN_records,
			 PDB::Integer number_of_SITE_records,
			 PDB::Integer number_of_ORIGX_SCALE_MTRIX_records,
			 PDB::Integer number_of_ATOM_HETATM_records,
			 PDB::Integer number_of_TER_records,
			 PDB::Integer number_of_CONECT_records,
			 PDB::Integer number_of_SEQRES_records);

		/// Reads a record indicating the beginning of a new model.
		virtual bool readRecordMODEL
			(PDB::Integer model_serial_number);

		/// Reads a record identifying residue modifications.
		virtual bool readRecordMODRES
			(PDB::IDcode entry_code,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer sequence_number,
			 PDB::AChar insertion_code,
			 PDB::ResidueName standardresidue_name,
			 PDB::PDBString comment);

		/** Reads a record defining transformations expressing
				non-crystallographic symmetry.
		*/
		virtual bool readRecordMTRIX1
			(PDB::Integer serial_number,
			 PDB::Real transformation_matrix[4],
			 PDB::Integer is_given);

		/** Reads a record defining transformations expressing
				non-crystallographic symmetry.
		*/
		virtual bool readRecordMTRIX2
			(PDB::Integer serial_number,
			 PDB::Real transformation_matrix[4],
			 PDB::Integer is_given);

		/** Reads a record defining transformations expressing
				non-crystallographic symmetry.
		*/
		virtual bool readRecordMTRIX3
			(PDB::Integer serial_number,
			 PDB::Real transformation_matrix[4],
			 PDB::Integer is_given);

		/** Reads a record indicating that this structura has been
				withdrawnform the database.
		*/
		virtual bool readRecordOBSLTE
			(PDB::Continuation continuation,
			 PDB::Date entry_replaced_date,
			 PDB::IDcode entry_code,
			 PDB::IDcode replacing_entry_code[8]);

		/** Reads a record defining the transformation from the orthogonal
				coordinates contained in the entry to the submitted coordinates.
		*/
		virtual bool readRecordORIGX1
			(PDB::Real transformation_matrix[4]);

		/** Reads a record defining the transformation from the orthogonal
				coordinates contained in the entry to the submitted coordinates.
		*/
		virtual bool readRecordORIGX2
			(PDB::Real transformation_matrix[4]);

		/** Reads a record defining the transformation from the orthogonal
				coordinates contained in the entry to the submitted coordinates.
		*/
		virtual bool readRecordORIGX3
			(PDB::Real transformation_matrix[4]);

		/// Reads a record containing remarks.
		virtual bool readRecordREMARK
			(PDB::Integer remark_number,
			 PDB::LString text);

		/// Reads a record containing a revision history.
		virtual bool readRecordREVDAT
			(PDB::Integer modification_number,
			 PDB::Continuation continuation,
			 PDB::Date modification_date,
			 PDB::String5 modification_ID,
			 PDB::Integer modification_type,
			 PDB::LString6 name_of_modified_record[4]);

		/// Reads a scale transformation record.
		virtual bool readRecordSCALE1
			(PDB::Real transformation_matrix[4]);

		/// Reads a scale transformation record.
		virtual bool readRecordSCALE2
			(PDB::Real transformation_matrix[4]);

		/// Reads a scale transformation record.
		virtual bool readRecordSCALE3
			(PDB::Real transformation_matrix[4]);

		/// Reads a record containing the sequence of residues.
		virtual bool readRecordSEQRES
			(PDB::Integer serial_number,
			 PDB::Character chain_ID,
			 PDB::Integer number_of_residues_in_chain,
			 PDB::ResidueName residue_name[13]);

		/// Reads a record defining a beta-sheet.
		virtual bool readRecordSHEET
			(PDB::Integer strand_number,
			 PDB::LString3 sheet_ID,
			 PDB::Integer number_of_strands,
			 PDB::RecordSHEET::InitialResidue& initial_residue,
			 PDB::RecordSHEET::TerminalResidue& terminal_residue,
			 PDB::Integer sense_of_strand,
			 PDB::Atom atom_name_in_current_strand,
			 PDB::RecordSHEET::ResidueInCurrentStrand& residue_in_current_strand,
			 PDB::Atom atom_name_in_previous_strand,
			 PDB::RecordSHEET::ResidueInPreviousStrand& residue_in_previous_strand);

		/// Reads a record giving the standard deviation of atomic coordinates.
		virtual bool readRecordSIGATM
			(PDB::Integer serial_number,
			 PDB::Atom atom_name,
			 PDB::Character alternate_location_indicator,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Real standard_vector_deviation[3],
			 PDB::Real standard_occupancy_deviation,
			 PDB::Real standard_temperature_deviation,
			 PDB::LString4 segment_ID,
			 PDB::LString2 element_symbol,
			 PDB::LString2 charge);

		/** Reads a record giving the standard deviation of anisotropic
				temperature factors.
		*/
		virtual bool readRecordSIGUIJ
			(PDB::Integer serial_number,
			 PDB::Atom atom_name,
			 PDB::Character alternate_location_indicator,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Integer sig11,
			 PDB::Integer sig22,
			 PDB::Integer sig33,
			 PDB::Integer sig12,
			 PDB::Integer sig13,
			 PDB::Integer sig23,
			 PDB::LString4 segment_ID,
			 PDB::LString2 element_symbol,
			 PDB::LString2 charge);

		/// Reads a record containing groups comprising a site.
		virtual bool readRecordSITE
			(PDB::Integer sequence_number,
			 PDB::LString3 name,
			 PDB::Integer number_of_residues,
			 PDB::RecordSITE::Residue residue[4]);

		/// Reads a record defining a salt bridge.
		virtual bool readRecordSLTBRG
			(PDB::RecordSLTBRG::PartnerAtom partner_atom[2],
			 PDB::SymmetryOperator first_atom,
			 PDB::SymmetryOperator second_atom);

		/** Reads a record specifies the chemical/biological source of each
				molecule in the entry.
		*/
		virtual bool readRecordSOURCE
			(PDB::Continuation continuation,
			 PDB::SpecificationList sources);

		/// Reads a record specifying a disulfide bond.
		virtual bool readRecordSSBOND
			(PDB::Integer serial_number,
			 PDB::RecordSSBOND::PartnerResidue partner_residue[2]);

		/// Reads a record terminating molecule.
		virtual bool readRecordTER
			(PDB::Integer serial_number,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code);

		/// Reads a record specifying the title of the experiment or anaysis.
		virtual bool readRecordTITLE
			(PDB::Continuation continuation,
			 PDB::PDBString title);

		/// Reads a record specifying a turn.
		virtual bool readRecordTURN
			(PDB::Integer sequence_number,
			 PDB::LString3 turn_ID,
			 PDB::RecordTURN::InitialResidue& initial_residue,
			 PDB::RecordTURN::TerminalResidue& terminal_residue,
			 PDB::PDBString comment);

		/** Reads a record specifying the translation vector for infinite
				covalently connected structures.
		*/
		virtual bool readRecordTVECT
			(PDB::Integer serial_number,
			 PDB::Real translation_vector[3],
			 PDB::PDBString comment);
		//@}

		///
		virtual bool hasFormat();

		///
		bool hasFormat() const;

		///
		virtual bool hasFormat(const String& s) const;


		typedef HashMap<PDB::Integer, PDBAtom*> PDBAtomMap;
		typedef Quadruple<String, PDB::Character, PDB::Integer, PDB::AChar> ResidueQuadruple;
		typedef HashMap<ResidueQuadruple, Residue*> ResidueMap;
		typedef std::list<ResidueQuadruple> QuadrupleList;
		typedef std::list<SecondaryStructure*> SecStructList;
		

		/**	@name Reading and writing */
		//@{
		/// Read a protein from the file
		PDBFile& operator >> (Protein& protein);
		/// Read a molecule from the file
		PDBFile& operator >> (Molecule& molecule);
		// Read system from the file
		PDBFile& operator >> (System& system);
		/// Write a protein to the file
		PDBFile& operator << (const Protein& protein);
		// Write system to the file
		PDBFile& operator << (const System& system);
		// Write molecule to the file
		PDBFile& operator << (const Molecule& molecule);
		///
		void read(Protein& protein);
		///
		void read(System& system);
		///
		bool write(const Protein& protein) throw(File::CannotWrite);
		///
		bool write(const Molecule& molecule) throw(File::CannotWrite);
		///
		bool write(const System& system) throw(File::CannotWrite);
		//@}

		protected:

		//_
		void clear_();

		//_
		static const PDB::RecordTypeFormat record_type_format_[];

		//_
		PDBFile(const File& pdbf) throw(Exception::FileNotFound);

		//_
		PDBFile& operator = (const PDBFile& pdbf);

		/*_ A helper function for initializing certain data structures. Used by
				constructors.
		*/
		void init_() throw();

		// 
		void write_(const Composite& composite, bool system = false);

		void postprocessSSBonds_();
		void postprocessHelices_();
		void postprocessSheetsTurns_(QuadrupleList& sectruct_list, SecStructList& new_secstruct_list);
		void postprocessRandomCoils_();



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

		union
		{
			PDB::RecordUNKNOWN record_UNKNOWN;
			PDB::RecordANISOU  record_ANISOU;
			PDB::RecordATOM    record_ATOM;
			PDB::RecordAUTHOR  record_AUTHOR;
			PDB::RecordCAVEAT  record_CAVEAT;
			PDB::RecordCISPEP  record_CISPEP;
			PDB::RecordCOMPND  record_COMPND;
			PDB::RecordCONECT  record_CONECT;
			PDB::RecordCRYST1  record_CRYST1;
			PDB::RecordDBREF   record_DBREF;
			PDB::RecordEND     record_END;
			PDB::RecordENDMDL  record_ENDMDL;
			PDB::RecordEXPDTA  record_EXPDTA;
			PDB::RecordFORMUL  record_FORMUL;
			PDB::RecordFTNOTE  record_FTNOTE;
			PDB::RecordHEADER  record_HEADER;
			PDB::RecordHELIX   record_HELIX;
			PDB::RecordHET     record_HET;
			PDB::RecordHETATM  record_HETATM;
			PDB::RecordHETNAM  record_HETNAM;
			PDB::RecordHYDBND  record_HYDBND;
			PDB::RecordJRNL    record_JRNL;
			PDB::RecordKEYWDS  record_KEYWDS;
			PDB::RecordLINK    record_LINK;
			PDB::RecordMASTER  record_MASTER;
			PDB::RecordMODEL   record_MODEL;
			PDB::RecordMODRES  record_MODRES;
			PDB::RecordMTRIX1  record_MTRIX1;
			PDB::RecordMTRIX2  record_MTRIX2;
			PDB::RecordMTRIX3  record_MTRIX3;
			PDB::RecordOBSLTE  record_OBSLTE;
			PDB::RecordORIGX1  record_ORIGX1;
			PDB::RecordORIGX2  record_ORIGX2;
			PDB::RecordORIGX3  record_ORIGX3;
			PDB::RecordREMARK  record_REMARK;
			PDB::RecordREVDAT  record_REVDAT;
			PDB::RecordSCALE1  record_SCALE1;
			PDB::RecordSCALE2  record_SCALE2;
			PDB::RecordSCALE3  record_SCALE3;
			PDB::RecordSEQRES  record_SEQRES;
			PDB::RecordSHEET   record_SHEET;
			PDB::RecordSIGATM  record_SIGATM;
			PDB::RecordSIGUIJ  record_SIGUIJ;
			PDB::RecordSITE    record_SITE;
			PDB::RecordSLTBRG  record_SLTBRG;
			PDB::RecordSOURCE  record_SOURCE;
			PDB::RecordSSBOND  record_SSBOND;
			PDB::RecordTER     record_TER;
			PDB::RecordTITLE   record_TITLE;
			PDB::RecordTURN    record_TURN;
			PDB::RecordTVECT   record_TVECT;
		};
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
	};

	/** Hash 
	* \ingroup  StructureFormats
  */
	inline
	HashIndex Hash(const PDBFile::ResidueQuadruple& quadruple)
		throw()
	{
		return (Index)quadruple.third;
	}

	/** Operator <<
	 		\ingroup StructureFormats
	*/
	inline
	std::ostream& operator << (std::ostream& s, const Quadruple<String,PDB::Character,PDB::Integer,PDB::AChar>& /* q */)
	{
		return s;
	}



#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL

#endif // BALL_FORMAT_PDBFILE_H
