// $Id: genericPDBFile.h,v 1.6 2000/10/23 23:31:01 amoll Exp $

#ifndef BALL_FORMAT_GENERICPDBFILE_H
#define BALL_FORMAT_GENERICPDBFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_PROPERTY_H
#	include <BALL/CONCEPT/property.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
#	include <BALL/SYSTEM/file.h>
#endif

namespace BALL 
{
	
	/**	PDB namespace
	*/
	namespace PDB 
	{

		enum
		{
			SIZE_OF_PDB_RECORD_LINE      = 80,
			SIZE_OF_PDB_LINE_BUFFER      = 256,   
			SIZE_OF_FORMAT_STRING_BUFFER = 256,
			FIRST_MODEL                  = -1
		};

		enum Property
		{
			PROPERTY__PSEUDO_XPLOR_ATOM_IMPORT = 0
		};
		
		enum RecordType
		{
			RECORD_TYPE__UNKNOWN = 0,
			RECORD_TYPE__ANISOU,
			RECORD_TYPE__ATOM,
			RECORD_TYPE__AUTHOR,
			RECORD_TYPE__CAVEAT,
			RECORD_TYPE__CISPEP,
			RECORD_TYPE__COMPND,
			RECORD_TYPE__CONECT,
			RECORD_TYPE__CRYST1,
			RECORD_TYPE__DBREF,
			RECORD_TYPE__END,
			RECORD_TYPE__ENDMDL,
			RECORD_TYPE__EXPDTA,
			RECORD_TYPE__FORMUL,
			RECORD_TYPE__FTNOTE,
			RECORD_TYPE__HEADER,
			RECORD_TYPE__HELIX,
			RECORD_TYPE__HET,
			RECORD_TYPE__HETATM,
			RECORD_TYPE__HETNAM,
			RECORD_TYPE__HYDBND,
			RECORD_TYPE__JRNL,
			RECORD_TYPE__KEYWDS,
			RECORD_TYPE__LINK,
			RECORD_TYPE__MASTER,
			RECORD_TYPE__MODEL,
			RECORD_TYPE__MODRES,
			RECORD_TYPE__MTRIX1,
			RECORD_TYPE__MTRIX2,
			RECORD_TYPE__MTRIX3,
			RECORD_TYPE__OBSLTE,
			RECORD_TYPE__ORIGX1,
			RECORD_TYPE__ORIGX2,
			RECORD_TYPE__ORIGX3,
			RECORD_TYPE__REMARK,
			RECORD_TYPE__REVDAT,
			RECORD_TYPE__SCALE1,
			RECORD_TYPE__SCALE2,
			RECORD_TYPE__SCALE3,
			RECORD_TYPE__SEQRES,
			RECORD_TYPE__SHEET,
			RECORD_TYPE__SIGATM,
			RECORD_TYPE__SIGUIJ,
			RECORD_TYPE__SITE,
			RECORD_TYPE__SLTBRG,
			RECORD_TYPE__SOURCE,
			RECORD_TYPE__SSBOND,
			RECORD_TYPE__TER,
			RECORD_TYPE__TITLE,
			RECORD_TYPE__TURN,
			RECORD_TYPE__TVECT,
				
			NUMBER_OF_REGISTERED_RECORD_TYPES,
			
			ALL_RECORD_TYPES
		};

		typedef char   AChar;
		typedef char   Atom[5];
		typedef char   Character;
		typedef long   Continuation;
		typedef char   Date[10];
		typedef char   IDcode[5];
		typedef long   Integer;
		// typedef char   Token[CPDBFile::SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   PDBList[SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   LString[SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   LString2[3];
		typedef char   LString3[4];
		typedef char   LString4[5];
		typedef char   LString5[6];
		typedef char   LString6[7];
		typedef char   LString7[8];
		typedef char   LString8[9];
		typedef char   LString9[10];
		typedef char   LString10[11];
		typedef char   LString11[12];
		typedef char   LString12[13];
		typedef char   LString13[14];
		typedef double Real;
		typedef char   RecordName[7];
		typedef char   ResidueName[4];
		typedef char   SList[SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   Specification[SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   SpecificationList[SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   PDBString[SIZE_OF_PDB_RECORD_LINE + 1];
		typedef char   String2[3];
		typedef char   String3[4];
		typedef char   String4[5];
		typedef char   String5[6];
		typedef char   String40[41];
		typedef long   SymmetryOperator;
		
		struct RecordTypeFormat
		{
			public:
			RecordType		record_type;
			char					string[7];
			const char*		format_string;
		};

		struct RecordUNKNOWN
		{
			Real max_record_size[SIZE_OF_PDB_RECORD_LINE];
		};
		
		struct RecordANISOU
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     residue_sequence_number;
			AChar       insertion_code;
			Integer     u11;
			Integer     u22;
			Integer     u33;
			Integer     u12;
			Integer     u13;
			Integer     u23;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};

		struct RecordATOM
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     residue_sequence_number;
			AChar       insertion_code;
			Real        orthogonal_vector[3];
			Real        occupancy;
			Real        temperature_factor;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};

		struct RecordAUTHOR
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBList      authors;
		};

		struct RecordCAVEAT
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			IDcode       entry_code;
			PDBString    comment;
		};

		struct RecordCISPEP
		{
			RecordType     record_type;
			RecordName     record_name;
			Integer        record_serial_number;
			struct CisPeptide
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     residue_sequence_number;
				AChar       insertion_code;
			} cis_peptide[2];
			Integer        specific_model_ID;
			Real           angle_measure;
		};

		struct RecordCOMPND
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			SpecificationList component_description;
		};
		
		struct RecordCONECT
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bonded_atom_serial_number[4];
			Integer    hydrogen_bonded_atom_serial_number[4];
			Integer    salt_bridged_atom_serial_number[2];
		};

		struct RecordCRYST1
		{
			RecordType record_type;
			RecordName record_name;
			struct UnitCell
			{
				Real     a;
				Real     b;
				Real     c;
				Real     alpha;
				Real     beta;
				Real     gamma;
				LString  space_group;
				Integer  z_value;
			} unit_cell;
		};

		struct RecordDBREF
		{
			RecordType record_type;
			RecordName record_name;
			IDcode     entry_code;
			Character  chain_ID;
			struct InitialSequence
			{
				Integer  number;
				AChar    insertion_code;
			} initial_sequence;
			struct EndingSequence
			{
				Integer  number;
				AChar    insertion_code;
			} ending_sequence;
			LString6   sequence_database_name;
			LString8   sequence_database_accession_code;
			LString12  sequence_database_ID_code;
			struct InitialDatabaseSegment
			{
				Integer  number;
				AChar    insertion_code;
			} initial_database_segment;
			struct EndingDatabaseSegment
			{
				Integer  number;
				AChar    insertion_code;
			} ending_database_segment;
		};

		struct RecordEND
		{
			RecordType record_type;
			RecordName record_name;
		};
		
		struct RecordENDMDL
		{
			RecordType record_type;
			RecordName record_name;
		};
		
		struct RecordEXPDTA
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			SList        technique;
		};
		
		struct RecordFORMUL
		{
			RecordType record_type;
			RecordName record_name;
			Integer    component_number;
			LString3   het_ID;
			Integer    continuation_number;
			Character  is_water;
			PDBString  chemical_formula;
		};
		
		struct RecordFTNOTE
		{
			RecordType record_type;
			RecordName record_name;
			Integer    number;
			PDBString  text;
		};
		
		struct RecordHEADER
		{
			RecordType record_type;
			RecordName record_name;
			String40   classification;
			Date       deposition_date;
			IDcode     ID_code;
		};
		
		struct RecordHELIX
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       serial_number;
			LString3      helix_ID;
			struct InitialResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} initial_residue;
			struct TerminalResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} terminal_residue;
			Integer       helix_class;
			PDBString     comment;
			Integer       length;
		};
		
		struct RecordHET
		{
			RecordType record_type;
			RecordName record_name;
			LString3   het_ID;
			Character  chain_ID;
			Integer    sequence_number;
			AChar      insertion_code;
			Integer    number_of_HETATM_records;
			PDBString  text;
		};
		
		struct RecordHETATM
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     residue_sequence_number;
			AChar       insertion_code;
			Real        orthogonal_vector[3];
			Real        occupancy;
			Real        temperature_factor;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};
		
		struct RecordHETNAM
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			LString3     het_ID;
			PDBString    chemical_name;
		};
		
		struct RecordHYDBND
		{
			RecordType       record_type;
			RecordName       record_name;
			struct HydrogenPartnerAtom
			{
				Atom           atom_name;
				Character      alternate_location_indicator;
				struct Residue
				{
					ResidueName  residue_name;
					Character    chain_ID;
					Integer      sequence_number;
					AChar        insertion_code;
				} residue;
			} hydrogen_partner_atom[2];
			struct HydrogenAtom
			{
				Atom           atom_name;
				Character      alternate_location_indicator;
				struct Residue
				{
					Character    chain_ID;
					Integer      sequence_number;
					AChar        insertion_code;
				} residue;
			} hydrogen_atom;
			SymmetryOperator first_non_hydrogen_atom;
			SymmetryOperator second_non_hydrogen_atom;
		};
		
		struct RecordJRNL
		{
			RecordType record_type;
			RecordName record_name;
			LString    text;
		};
		
		struct RecordKEYWDS
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBList      keywords;
		};
		
		struct RecordLINK
		{
			RecordType      record_type;
			RecordName      record_name;
			struct LinkPartner
			{
				Atom          atom_name;
				Character     alternate_location_indicator;
				struct Residue
				{
					ResidueName residue_name;
					Character   chain_ID;
					Integer     sequence_number;
					AChar       insertion_code;
				} residue;
			} link_partner[2];
			SymmetryOperator first_atom;
			SymmetryOperator second_atom;
		};
		
		struct RecordMASTER
		{
			RecordType record_type;
			RecordName record_name;
			Integer    number_of_REMARK_records;
			Integer    zero;
			Integer    number_of_HET_records;
			Integer    number_of_HELIX_records;
			Integer    number_of_SHEET_records;
			Integer    number_of_TURN_records;
			Integer    number_of_SITE_records;
			Integer    number_of_ORIGX_SCALE_MTRIX_records;
			Integer    number_of_ATOM_HETATM_records;
			Integer    number_of_TER_records;
			Integer    number_of_CONECT_records;
			Integer    number_of_SEQRES_records;
		};
		
		struct RecordMODEL
		{
			RecordType record_type;
			RecordName record_name;
			Integer    model_serial_number;
		};
		
		struct RecordMODRES
		{
			RecordType  record_type;
			RecordName  record_name;
			IDcode      entry_code;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     sequence_number;
			AChar       insertion_code;
			ResidueName standard_residue_name;
			PDBString   comment;
		};
		
		struct RecordMTRIX1
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		
		struct RecordMTRIX2
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		
		struct RecordMTRIX3
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		
		struct RecordOBSLTE
		{
			RecordType  record_type;
			RecordName  record_name;
			Continuation continuation;
			Date         entry_replaced_date;
			IDcode       entry_code;
			IDcode       replacing_entry_code[8];
		};
		
		struct RecordORIGX1
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		
		struct RecordORIGX2
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		
		struct RecordORIGX3
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		
		struct RecordREMARK
		{
			RecordType record_type;
			RecordName record_name;
			Integer    remark_number;
			LString    text;
		};
		
		struct RecordREVDAT
		{
			RecordType   record_type;
			RecordName   record_name;
			Integer      modification_number;
			Continuation continuation;
			Date         modification_date;
			String5      modification_ID;
			Integer      modification_type;
			LString6     name_of_modified_record[4];
		};

		struct RecordSCALE1
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		struct RecordSCALE2
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		struct RecordSCALE3
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		struct RecordSEQRES
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Character   chain_ID;
			Integer     number_of_residues_in_chain;
			ResidueName residue_name[13];
		};

		struct RecordSHEET
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       strand_number;
			LString3      sheet_ID;
			Integer       number_of_strands;
			struct InitialResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} initial_residue;
			struct TerminalResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} terminal_residue;
			Integer       sense_of_strand;
			Atom          atom_name_in_current_strand;
			struct ResidueInCurrentStrand
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} residue_in_current_strand;
			Atom          atom_name_in_previous_strand;
			struct ResidueInPreviousStrand
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} residue_in_previous_strand;
		};

		struct RecordSIGATM
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     residue_sequence_number;
			AChar       insertion_code;
			Real        standard_vector_deviation[3];
			Real        standard_occupancy_deviation;
			Real        standard_temperature_deviation;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};

		struct RecordSIGUIJ
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     residue_sequence_number;
			AChar       insertion_code;
			Integer     sig11;
			Integer     sig22;
			Integer     sig33;
			Integer     sig12;
			Integer     sig13;
			Integer     sig23;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};

		struct RecordSITE
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       sequence_number;
			LString3      name;
			Integer       number_of_residues;
			struct Residue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} residue[4];
		};

		struct RecordSLTBRG
		{
			RecordType      record_type;
			RecordName      record_name;
			struct PartnerAtom
			{
				Atom          atom_name;
				Character     alternate_location_indicator;
				struct Residue
				{
					ResidueName residue_name;
					Character   chain_ID;
					Integer     sequence_number;
					AChar       insertion_code;
				} residue;
			} partner_atom[2];
			SymmetryOperator first_atom;
			SymmetryOperator second_atom;
		};

		struct RecordSOURCE
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			SpecificationList sources;
		};

		struct RecordSSBOND
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       serial_number;
			struct PartnerResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
				SymmetryOperator symmetry_operator;
			} partner_residue[2];
		};

		struct RecordTER
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			ResidueName residue_name;
			Character   chain_ID;
			Integer     residue_sequence_number;
			AChar       insertion_code;
		};

		struct RecordTITLE
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBString    title;
		};

		struct RecordTURN
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       sequence_number;
			LString3      turn_ID;
			struct InitialResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} initial_residue;
			struct TerminalResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} terminal_residue;
			PDBString     comment;
		};

		struct RecordTVECT
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       translation_vector[3];
			PDBString  comment;
		};
	}

	/**	Generic PDB file class
	*/
	class GenericPDBFile
		: public File,
			public PropertyManager
	{
		public:

		GenericPDBFile();

		virtual ~GenericPDBFile();

		virtual void clear(int state = 0);
	
		virtual float getVersion() const;

		void selectModel(Index index);

		void selectAllModels();

		Index getSelectedModel() const;

		Index getCurrentModel() const;

		char* getRecordString();
	
		const char* getRecordString() const;
	
		Index getRecordNumber() const;

		PDB::RecordType getRecordType() const;

		virtual const char* getAtomElementSymbol
			(const PDB::Atom atom_name,
			 PDB::Atom element_symbol);
	
		virtual const char* getAtomName
			(const PDB::Atom atom_name);
	
		virtual char getAtomRemotenessIndicator
			(const PDB::Atom atom_name);
	
		virtual short getAtomBranchDesignator
			(const PDB::Atom atom_name);
	
		Size countRecordFields() const;

		Size countRecord
			(PDB::RecordType record_type,
			 bool from_begin_of_file = true);
	
		Size countRecords
			(bool from_begin_of_file = true);
	
		bool parseLine(const char* line, Size size, const char* format_string, ...);

		bool readLine(char* line, Size size, bool extract_values);

		bool readFirstRecord(bool read_values = true);
	
		bool readNextRecord(bool read_values = true);

		bool readRecords();
	
		virtual bool readUnknownRecord(const char* line);
	
		virtual bool readInvalidRecord(const char* line);
	
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

		virtual bool readRecordAUTHOR
			(PDB::Continuation continuation,
			 PDB::PDBList authors);

		virtual bool readRecordCAVEAT
			(PDB::Continuation continuation,
			 PDB::IDcode entry_code,
			 PDB::PDBString comment);

		virtual bool readRecordCISPEP
			(PDB::Integer record_serial_number,
			 PDB::RecordCISPEP::CisPeptide cis_peptide[2],
			 PDB::Integer specific_model_ID,
			 PDB::Real angle_measure);

		virtual bool readRecordCOMPND
			(PDB::Continuation continuation,
			 PDB::SpecificationList component_description);
	
		virtual bool readRecordCONECT
			(PDB::Integer atom_serial_number,
			 PDB::Integer bonded_atom_serial_number[4],
			 PDB::Integer hydrogen_bonded_atom_serial_number[4],
			 PDB::Integer salt_bridged_atom_serial_number[2]);

		virtual bool readRecordCRYST1
			(PDB::RecordCRYST1::UnitCell& unit_cell);

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

		virtual bool readRecordEND();
	
		virtual bool readRecordENDMDL();
	
		virtual bool readRecordEXPDTA
			(PDB::Continuation continuation,
			 PDB::SList technique);
	
		virtual bool readRecordFORMUL
			(PDB::Integer component_number,
			 PDB::LString3 het_ID,
			 PDB::Integer continuation_number,
			 PDB::Character is_water,
			 PDB::PDBString chemical_formula);
	
		virtual bool readRecordFTNOTE
			(PDB::Integer number,
			 PDB::PDBString text);
	
		virtual bool readRecordHEADER
			(PDB::String40 classification,
			 PDB::Date deposition_date,
			 PDB::IDcode ID_code);
	
		virtual bool readRecordHELIX
			(PDB::Integer serial_number,
			 PDB::LString3 helix_ID,
			 PDB::RecordHELIX::InitialResidue& initial_residue,
			 PDB::RecordHELIX::TerminalResidue& terminal_residue,
			 PDB::Integer helix_class,
			 PDB::PDBString comment,
			 PDB::Integer length);
	
		virtual bool readRecordHET
			(PDB::LString3 het_ID,
			 PDB::Character chain_ID,
			 PDB::Integer sequence_number,
			 PDB::AChar insertion_code,
			 PDB::Integer number_of_HETATM_records,
			 PDB::PDBString text);
	
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
	
		virtual bool readRecordHETNAM
			(PDB::Continuation continuation,
			 PDB::LString3 het_ID,
			 PDB::PDBString chemical_name);
	
		virtual bool readRecordHYDBND
			(PDB::RecordHYDBND::HydrogenPartnerAtom hydrogen_partner_atom[2],
			 PDB::RecordHYDBND::HydrogenAtom& hydrogen_atom,
			 PDB::SymmetryOperator first_non_hydrogen_atom,
			 PDB::SymmetryOperator second_non_hydrogen_atom);
	
		virtual bool readRecordJRNL
			(PDB::LString text);
	
		virtual bool readRecordKEYWDS
			(PDB::Continuation continuation,
			 PDB::PDBList keywords);
	
		virtual bool readRecordLINK
			(PDB::RecordLINK::LinkPartner link_partner[2],
			 PDB::SymmetryOperator first_atom,
			 PDB::SymmetryOperator second_atom);
	
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
	
		virtual bool readRecordMODEL
			(PDB::Integer model_serial_number);
	
		virtual bool readRecordMODRES
			(PDB::IDcode entry_code,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer sequence_number,
			 PDB::AChar insertion_code,
			 PDB::ResidueName standardresidue_name,
			 PDB::PDBString comment);
	
		virtual bool readRecordMTRIX1
			(PDB::Integer serial_number,
			 PDB::Real transformation_matrix[4],
			 PDB::Integer is_given);
	
		virtual bool readRecordMTRIX2
			(PDB::Integer serial_number,
			 PDB::Real transformation_matrix[4],
			 PDB::Integer is_given);
	
		virtual bool readRecordMTRIX3
			(PDB::Integer serial_number,
			 PDB::Real transformation_matrix[4],
			 PDB::Integer is_given);
	
		virtual bool readRecordOBSLTE
			(PDB::Continuation continuation,
			 PDB::Date entry_replaced_date,
			 PDB::IDcode entry_code,
			 PDB::IDcode replacing_entry_code[8]);
	
		virtual bool readRecordORIGX1
			(PDB::Real transformation_matrix[4]);
	
		virtual bool readRecordORIGX2
			(PDB::Real transformation_matrix[4]);
	
		virtual bool readRecordORIGX3
			(PDB::Real transformation_matrix[4]);
	
		virtual bool readRecordREMARK
			(PDB::Integer remark_number,
			 PDB::LString text);
	
		virtual bool readRecordREVDAT
			(PDB::Integer modification_number,
			 PDB::Continuation continuation,
			 PDB::Date modification_date,
			 PDB::String5 modification_ID,
			 PDB::Integer modification_type,
			 PDB::LString6 name_of_modified_record[4]);

		virtual bool readRecordSCALE1
			(PDB::Real transformation_matrix[4]);

		virtual bool readRecordSCALE2
			(PDB::Real transformation_matrix[4]);

		virtual bool readRecordSCALE3
			(PDB::Real transformation_matrix[4]);

		virtual bool readRecordSEQRES
			(PDB::Integer serial_number,
			 PDB::Character chain_ID,
			 PDB::Integer number_of_residues_in_chain,
			 PDB::ResidueName residue_name[13]);

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

		virtual bool readRecordSITE
			(PDB::Integer sequence_number,
			 PDB::LString3 name,
			 PDB::Integer number_of_residues,
			 PDB::RecordSITE::Residue residue[4]);

		virtual bool readRecordSLTBRG
			(PDB::RecordSLTBRG::PartnerAtom partner_atom[2],
			 PDB::SymmetryOperator first_atom,
			 PDB::SymmetryOperator second_atom);

		virtual bool readRecordSOURCE
			(PDB::Continuation continuation,
			 PDB::SpecificationList sources);

		virtual bool readRecordSSBOND
			(PDB::Integer serial_number,
			 PDB::RecordSSBOND::PartnerResidue partner_residue[2]);

		virtual bool readRecordTER
			(PDB::Integer serial_number,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code);

		virtual bool readRecordTITLE
			(PDB::Continuation continuation,
			 PDB::PDBString title);

		virtual bool readRecordTURN
			(PDB::Integer sequence_number,
			 PDB::LString3 turn_ID,
			 PDB::RecordTURN::InitialResidue& initial_residue,
			 PDB::RecordTURN::TerminalResidue& terminal_residue,
			 PDB::PDBString comment);

		virtual bool readRecordTVECT
			(PDB::Integer serial_number,
			 PDB::Real translation_vector[3],
			 PDB::PDBString comment);

		virtual bool hasFormat();

		bool hasFormat() const;

		virtual bool hasFormat(const String& s) const;


		protected:
		
		void clear_();


		private:

		GenericPDBFile(const File& generic_PDB_file);

		GenericPDBFile& operator = (const GenericPDBFile& generic_PDB_file);

		Index current_model_;
		Index selected_model_;
		Index current_record_;
		Size record_fields_;
		PDB::RecordType current_record_type_;
		PDB::RecordTypeFormat compare_record_type_format_; 
		char line_buffer_[PDB::SIZE_OF_PDB_LINE_BUFFER];
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

		protected:
		
		static const PDB::RecordTypeFormat record_type_format_[];
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/genericPDBFile.iC>
#	endif

} // namespace BALL

#endif // BALL_FORMAT_GENERICPDBFILE_H
