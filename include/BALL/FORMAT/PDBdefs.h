// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBdefs.h,v 1.2 2005/02/12 23:08:26 oliver Exp $
//

#ifndef BALL_FORMAT_PDBDEFS_H
#define BALL_FORMAT_PDBDEFS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{

	/**	PDB namespace.
			This namespace collects some type and format definitions related to the PDB format.
			It is used by \link PDBFile \endlink and probably not of much use outside this class.
			\ingroup StructureFormats
	*/
	namespace PDB 
	{
		/**
		*/
		enum
		{
			SIZE_OF_PDB_RECORD_LINE      = 80,
			SIZE_OF_PDB_LINE_BUFFER      = 256,   
			SIZE_OF_FORMAT_STRING_BUFFER = 256
		};

		/** The record types of a PDB file.
		*/
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

		/** @name Some typedefs for simplifying parsing.
		*/
		//@{
		///
		typedef char   AChar;
		///
		typedef char   Atom[5];
		///
		typedef char   Character;
		///
		typedef long   Continuation;
		///
		typedef char   Date[10];
		///
		typedef char   IDcode[5];
		///
		typedef long   Integer;
		// typedef char   Token[CPDBFile::SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   PDBList[SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   LString[SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   LString2[3];
		///
		typedef char   LString3[4];
		///
		typedef char   LString4[5];
		///
		typedef char   LString5[6];
		///
		typedef char   LString6[7];
		///
		typedef char   LString7[8];
		///
		typedef char   LString8[9];
		///
		typedef char   LString9[10];
		///
		typedef char   LString10[11];
		///
		typedef char   LString11[12];
		///
		typedef char   LString12[13];
		///
		typedef char   LString13[14];
		///
		typedef double Real;
		///
		typedef char   RecordName[7];
		///
		typedef char   ResidueName[4];
		///
		typedef char   SList[SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   Specification[SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   SpecificationList[SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   PDBString[SIZE_OF_PDB_RECORD_LINE + 1];
		///
		typedef char   String2[3];
		///
		typedef char   String3[4];
		///
		typedef char   String4[5];
		///
		typedef char   String5[6];
		///
		typedef char   String40[41];
		///
		typedef long   SymmetryOperator;
		//@}

		/** @name Structs for storing records.
		*/
		//@{
		/**
		*/
		struct RecordTypeFormat
		{
			public:
			RecordType		record_type;
			char					string[7];
			const char*		format_string;
		};

		/**
		*/
		struct RecordUNKNOWN
		{
			Real max_record_size[SIZE_OF_PDB_RECORD_LINE];
		};
		
		/**
		*/
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

		/**
		*/
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
			LString4    partial_charge; // not standard! Read if option PARSE_PARTIAL_CHARGES is set!
		};

		/**
		*/
		struct RecordAUTHOR
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBList      authors;
		};

		/**
		*/
		struct RecordCAVEAT
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			IDcode       entry_code;
			PDBString    comment;
		};

		/**
		*/
		struct RecordCISPEP
		{
			///
			RecordType     record_type;
			RecordName     record_name;
			Integer        record_serial_number;
			///
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

		/**
		*/
		struct RecordCOMPND
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			SpecificationList component_description;
		};
		
		/**
		*/
		struct RecordCONECT
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bonded_atom_serial_number[4];
			Integer    hydrogen_bonded_atom_serial_number[4];
			Integer    salt_bridged_atom_serial_number[2];
		};

		/**
		*/
		struct RecordCRYST1
		{
			RecordType record_type;
			RecordName record_name;
			///
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

		/**
		*/
		struct RecordDBREF
		{
			RecordType record_type;
			RecordName record_name;
			IDcode     entry_code;
			Character  chain_ID;
			///
			struct InitialSequence
			{
				Integer  number;
				AChar    insertion_code;
			} initial_sequence;
			///
			struct EndingSequence
			{
				Integer  number;
				AChar    insertion_code;
			} ending_sequence;
			LString6   sequence_database_name;
			LString8   sequence_database_accession_code;
			LString12  sequence_database_ID_code;
			///
			struct InitialDatabaseSegment
			{
				Integer  number;
				AChar    insertion_code;
			} initial_database_segment;
			///
			struct EndingDatabaseSegment
			{
				Integer  number;
				AChar    insertion_code;
			} ending_database_segment;
		};

		///
		struct RecordEND
		{
			RecordType record_type;
			RecordName record_name;
		};

		///
		struct RecordENDMDL
		{
			RecordType record_type;
			RecordName record_name;
		};
		
		///
		struct RecordEXPDTA
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			SList        technique;
		};
		
		///
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
		
		///
		struct RecordFTNOTE
		{
			RecordType record_type;
			RecordName record_name;
			Integer    number;
			PDBString  text;
		};
		
		///
		struct RecordHEADER
		{
			RecordType record_type;
			RecordName record_name;
			String40   classification;
			Date       deposition_date;
			IDcode     ID_code;
		};
		
		///
		struct RecordHELIX
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       serial_number;
			LString3      helix_ID;
			///
			struct InitialResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} initial_residue;
			///
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
		
		///
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
		
		///
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
		
		///
		struct RecordHETNAM
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			LString3     het_ID;
			PDBString    chemical_name;
		};
		
		///
		struct RecordHYDBND
		{
			RecordType       record_type;
			RecordName       record_name;
			///
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
			///
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
		
		///
		struct RecordJRNL
		{
			RecordType record_type;
			RecordName record_name;
			LString    text;
		};
		///
		struct RecordKEYWDS
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBList      keywords;
		};
		///
		struct RecordLINK
		{
			RecordType      record_type;
			RecordName      record_name;
			///			
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
		///
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
		///
		struct RecordMODEL
		{
			RecordType record_type;
			RecordName record_name;
			Integer    model_serial_number;
		};
		///
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
		///
		struct RecordMTRIX1
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		///
		struct RecordMTRIX2
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		///
		struct RecordMTRIX3
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		///
		struct RecordOBSLTE
		{
			RecordType  record_type;
			RecordName  record_name;
			Continuation continuation;
			Date         entry_replaced_date;
			IDcode       entry_code;
			IDcode       replacing_entry_code[8];
		};
		///
		struct RecordORIGX1
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		///
		struct RecordORIGX2
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		///
		struct RecordORIGX3
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		///
		struct RecordREMARK
		{
			RecordType record_type;
			RecordName record_name;
			Integer    remark_number;
			LString    text;
		};
		///
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

		///
		struct RecordSCALE1
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		///
		struct RecordSCALE2
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		///
		struct RecordSCALE3
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		///
		struct RecordSEQRES
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Character   chain_ID;
			Integer     number_of_residues_in_chain;
			ResidueName residue_name[13];
		};

		///
		struct RecordSHEET
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       strand_number;
			LString3      sheet_ID;
			Integer       number_of_strands;
			///
			struct InitialResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} initial_residue;
			///
			struct TerminalResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} terminal_residue;
			Integer       sense_of_strand;
			Atom          atom_name_in_current_strand;
			///
			struct ResidueInCurrentStrand
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} residue_in_current_strand;
			Atom          atom_name_in_previous_strand;
			///
			struct ResidueInPreviousStrand
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} residue_in_previous_strand;
		};

		///
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

		///
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

		///
		struct RecordSITE
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       sequence_number;
			LString3      name;
			Integer       number_of_residues;
			///
			struct Residue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
			} residue[4];
		};

		///
		struct RecordSLTBRG
		{
			RecordType      record_type;
			RecordName      record_name;
			///
			struct PartnerAtom
			{
				Atom          atom_name;
				Character     alternate_location_indicator;
				///
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

		///
		struct RecordSOURCE
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			SpecificationList sources;
		};

		///
		struct RecordSSBOND
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       serial_number;
			///
			struct PartnerResidue
			{
				ResidueName residue_name;
				Character   chain_ID;
				Integer     sequence_number;
				AChar       insertion_code;
				SymmetryOperator symmetry_operator;
			} partner_residue[2];
		};

		/**
		*/
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

		/**
		*/
		struct RecordTITLE
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBString    title;
		};

		/**
		*/
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

		/**
		*/
		struct RecordTVECT
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       translation_vector[3];
			PDBString  comment;
		};
		//@}

		/**	@name PDB record format definitions */
		//@{

		extern const char* FORMAT_UNKNOWN;
		extern const char* FORMAT_ANISOU;
		extern const char* FORMAT_ATOM;
		extern const char* FORMAT_ATOM_PARTIAL_CRG;
		extern const char* FORMAT_AUTHOR;
		extern const char* FORMAT_CAVEAT;
		extern const char* FORMAT_CISPEP;
		extern const char* FORMAT_COMPND;
		extern const char* FORMAT_CONECT;
		extern const char* FORMAT_CRYST1;
		extern const char* FORMAT_DBREF;
		extern const char* FORMAT_END;
		extern const char* FORMAT_ENDMDL;
		extern const char* FORMAT_EXPDTA;
		extern const char* FORMAT_FORMUL;
		extern const char* FORMAT_FTNOTE;
		extern const char* FORMAT_HEADER;
		extern const char* FORMAT_HELIX;
		extern const char* FORMAT_HET;
		extern const char* FORMAT_HETATM;
		extern const char* FORMAT_HETNAM;
		extern const char* FORMAT_HYDBND;
		extern const char* FORMAT_JRNL;
		extern const char* FORMAT_KEYWDS;
		extern const char* FORMAT_LINK;
		extern const char* FORMAT_MASTER;
		extern const char* FORMAT_MODEL;
		extern const char* FORMAT_MODRES;
		extern const char* FORMAT_MTRIX1;
		extern const char* FORMAT_MTRIX2;
		extern const char* FORMAT_MTRIX3;
		extern const char* FORMAT_OBSLTE;
		extern const char* FORMAT_ORIGX1;
		extern const char* FORMAT_ORIGX2;
		extern const char* FORMAT_ORIGX3;
		extern const char* FORMAT_REMARK;
		extern const char* FORMAT_REVDAT;
		extern const char* FORMAT_SCALE1;
		extern const char* FORMAT_SCALE2;
		extern const char* FORMAT_SCALE3;
		extern const char* FORMAT_SEQRES;
		extern const char* FORMAT_SHEET;
		extern const char* FORMAT_SIGATM;
		extern const char* FORMAT_SIGUIJ;
		extern const char* FORMAT_SITE;
		extern const char* FORMAT_SLTBRG;
		extern const char* FORMAT_SOURCE;
		extern const char* FORMAT_SSBOND;
		extern const char* FORMAT_TER;
		extern const char* FORMAT_TITLE;
		extern const char* FORMAT_TURN;
		extern const char* FORMAT_TVECT;

	} // namespace PDB

} // namespace BALL

#endif // BALL_FORMAT_PDBDEFS_H
