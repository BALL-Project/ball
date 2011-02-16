// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_PDBDEFS_H
#define BALL_FORMAT_PDBDEFS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_SECONDARYSTRUCTURE_H
#	include <BALL/KERNEL/secondaryStructure.h>
#endif

#ifndef BALL_KERNEL_CHAIN_H
#	include <BALL/KERNEL/chain.h>
#endif

#ifndef BALL_KERNEL_RESIDUE_H
#	include <BALL/KERNEL/residue.h>
#endif

#ifndef BALL_DATATYPES_HASHSET_H
#	include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
#	include <BALL/MATHS/matrix44.h>
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
			RECORD_TYPE__CON06,
			RECORD_TYPE__CON061,
			RECORD_TYPE__CON062,
			RECORD_TYPE__CON063,
			RECORD_TYPE__CON064,
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
			RECORD_TYPE__HETSYN,
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
			RECORD_TYPE__SEQADV,
			RECORD_TYPE__SEQRES,
			RECORD_TYPE__SHEET,
			RECORD_TYPE__SIGATM,
			RECORD_TYPE__SIGUIJ,
			RECORD_TYPE__SITE,
			RECORD_TYPE__SLTBRG,
			RECORD_TYPE__SOURCE,
			RECORD_TYPE__SPRSDE,
			RECORD_TYPE__SSBOND,
			RECORD_TYPE__TER,
			RECORD_TYPE__TITLE,
			RECORD_TYPE__TURN,
			RECORD_TYPE__TVECT,
				
			NUMBER_OF_REGISTERED_RECORD_TYPES,
			
			ALL_RECORD_TYPES
		};

		/**	@name PDB record format definitions */
		//@{
		BALL_EXPORT extern const char* FORMAT_UNKNOWN;
		BALL_EXPORT extern const char* FORMAT_ANISOU;
		BALL_EXPORT extern const char* FORMAT_ATOM;
		BALL_EXPORT extern const char* FORMAT_ATOM_PARTIAL_CRG;
		BALL_EXPORT extern const char* FORMAT_AUTHOR;
		BALL_EXPORT extern const char* FORMAT_CAVEAT;
		BALL_EXPORT extern const char* FORMAT_CISPEP;
		BALL_EXPORT extern const char* FORMAT_COMPND;
		BALL_EXPORT extern const char* FORMAT_CONECT;
		BALL_EXPORT extern const char* FORMAT_CON06;
		BALL_EXPORT extern const char* FORMAT_CON06_1;
		BALL_EXPORT extern const char* FORMAT_CON06_2;
		BALL_EXPORT extern const char* FORMAT_CON06_3;
		BALL_EXPORT extern const char* FORMAT_CON06_4;
		BALL_EXPORT extern const char* FORMAT_CRYST1;
		BALL_EXPORT extern const char* FORMAT_DBREF;
		BALL_EXPORT extern const char* FORMAT_END;
		BALL_EXPORT extern const char* FORMAT_ENDMDL;
		BALL_EXPORT extern const char* FORMAT_EXPDTA;
		BALL_EXPORT extern const char* FORMAT_FORMUL;
		BALL_EXPORT extern const char* FORMAT_FTNOTE;
		BALL_EXPORT extern const char* FORMAT_HEADER;
		BALL_EXPORT extern const char* FORMAT_HELIX;
		BALL_EXPORT extern const char* FORMAT_HET;
		BALL_EXPORT extern const char* FORMAT_HETATM;
		BALL_EXPORT extern const char* FORMAT_HETNAM;
		BALL_EXPORT extern const char* FORMAT_HETSYN;
		BALL_EXPORT extern const char* FORMAT_HYDBND;
		BALL_EXPORT extern const char* FORMAT_JRNL;
		BALL_EXPORT extern const char* FORMAT_KEYWDS;
		BALL_EXPORT extern const char* FORMAT_LINK;
		BALL_EXPORT extern const char* FORMAT_MASTER;
		BALL_EXPORT extern const char* FORMAT_MODEL;
		BALL_EXPORT extern const char* FORMAT_MODRES;
		BALL_EXPORT extern const char* FORMAT_MTRIX1;
		BALL_EXPORT extern const char* FORMAT_MTRIX2;
		BALL_EXPORT extern const char* FORMAT_MTRIX3;
		BALL_EXPORT extern const char* FORMAT_OBSLTE;
		BALL_EXPORT extern const char* FORMAT_ORIGX1;
		BALL_EXPORT extern const char* FORMAT_ORIGX2;
		BALL_EXPORT extern const char* FORMAT_ORIGX3;
		BALL_EXPORT extern const char* FORMAT_REMARK;
		BALL_EXPORT extern const char* FORMAT_REVDAT;
		BALL_EXPORT extern const char* FORMAT_SCALE1;
		BALL_EXPORT extern const char* FORMAT_SCALE2;
		BALL_EXPORT extern const char* FORMAT_SCALE3;
		BALL_EXPORT extern const char* FORMAT_SEQADV;
		BALL_EXPORT extern const char* FORMAT_SEQRES;
		BALL_EXPORT extern const char* FORMAT_SHEET;
		BALL_EXPORT extern const char* FORMAT_SIGATM;
		BALL_EXPORT extern const char* FORMAT_SIGUIJ;
		BALL_EXPORT extern const char* FORMAT_SITE;
		BALL_EXPORT extern const char* FORMAT_SLTBRG;
		BALL_EXPORT extern const char* FORMAT_SOURCE;
		BALL_EXPORT extern const char* FORMAT_SPRSDE;
		BALL_EXPORT extern const char* FORMAT_SSBOND;
		BALL_EXPORT extern const char* FORMAT_TER;
		BALL_EXPORT extern const char* FORMAT_TITLE;
		BALL_EXPORT extern const char* FORMAT_TURN;
		BALL_EXPORT extern const char* FORMAT_TVECT;
		//@}

		/**	@name PDB record tag names */
		//@{
		BALL_EXPORT extern const char* RECORD_TAG_ANISOU;
		BALL_EXPORT extern const char* RECORD_TAG_ATOM;
		BALL_EXPORT extern const char* RECORD_TAG_AUTHOR;
		BALL_EXPORT extern const char* RECORD_TAG_CAVEAT;
		BALL_EXPORT extern const char* RECORD_TAG_CISPEP;
		BALL_EXPORT extern const char* RECORD_TAG_COMPND;
		BALL_EXPORT extern const char* RECORD_TAG_CONECT;
		BALL_EXPORT extern const char* RECORD_TAG_CON06;
		BALL_EXPORT extern const char* RECORD_TAG_CON061;
		BALL_EXPORT extern const char* RECORD_TAG_CON062;
		BALL_EXPORT extern const char* RECORD_TAG_CON063;
		BALL_EXPORT extern const char* RECORD_TAG_CON064;
		BALL_EXPORT extern const char* RECORD_TAG_CRYST1;
		BALL_EXPORT extern const char* RECORD_TAG_DBREF;
		BALL_EXPORT extern const char* RECORD_TAG_END;
		BALL_EXPORT extern const char* RECORD_TAG_ENDMDL;
		BALL_EXPORT extern const char* RECORD_TAG_EXPDTA;
		BALL_EXPORT extern const char* RECORD_TAG_FORMUL;
		BALL_EXPORT extern const char* RECORD_TAG_FTNOTE;
		BALL_EXPORT extern const char* RECORD_TAG_HEADER;
		BALL_EXPORT extern const char* RECORD_TAG_HELIX;
		BALL_EXPORT extern const char* RECORD_TAG_HET;
		BALL_EXPORT extern const char* RECORD_TAG_HETATM;
		BALL_EXPORT extern const char* RECORD_TAG_HETNAM;
		BALL_EXPORT extern const char* RECORD_TAG_HETSYN;
		BALL_EXPORT extern const char* RECORD_TAG_HYDBND;
		BALL_EXPORT extern const char* RECORD_TAG_JRNL;
		BALL_EXPORT extern const char* RECORD_TAG_KEYWDS;
		BALL_EXPORT extern const char* RECORD_TAG_LINK;
		BALL_EXPORT extern const char* RECORD_TAG_MASTER;
		BALL_EXPORT extern const char* RECORD_TAG_MODEL;
		BALL_EXPORT extern const char* RECORD_TAG_MODRES;
		BALL_EXPORT extern const char* RECORD_TAG_MTRIX1;
		BALL_EXPORT extern const char* RECORD_TAG_MTRIX2;
		BALL_EXPORT extern const char* RECORD_TAG_MTRIX3;
		BALL_EXPORT extern const char* RECORD_TAG_OBSLTE;
		BALL_EXPORT extern const char* RECORD_TAG_ORIGX1;
		BALL_EXPORT extern const char* RECORD_TAG_ORIGX2;
		BALL_EXPORT extern const char* RECORD_TAG_ORIGX3;
		BALL_EXPORT extern const char* RECORD_TAG_REMARK;
		BALL_EXPORT extern const char* RECORD_TAG_REVDAT;
		BALL_EXPORT extern const char* RECORD_TAG_SCALE1;
		BALL_EXPORT extern const char* RECORD_TAG_SCALE2;
		BALL_EXPORT extern const char* RECORD_TAG_SCALE3;
		BALL_EXPORT extern const char* RECORD_TAG_SEQADV;
		BALL_EXPORT extern const char* RECORD_TAG_SEQRES;
		BALL_EXPORT extern const char* RECORD_TAG_SHEET;
		BALL_EXPORT extern const char* RECORD_TAG_SIGATM;
		BALL_EXPORT extern const char* RECORD_TAG_SIGUIJ;
		BALL_EXPORT extern const char* RECORD_TAG_SITE;
		BALL_EXPORT extern const char* RECORD_TAG_SLTBRG;
		BALL_EXPORT extern const char* RECORD_TAG_SOURCE;
		BALL_EXPORT extern const char* RECORD_TAG_SPRSDE;
		BALL_EXPORT extern const char* RECORD_TAG_SSBOND;
		BALL_EXPORT extern const char* RECORD_TAG_TER;
		BALL_EXPORT extern const char* RECORD_TAG_TITLE;
		BALL_EXPORT extern const char* RECORD_TAG_TURN;
		BALL_EXPORT extern const char* RECORD_TAG_TVECT;
		//@}

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
		typedef char   LString21[22];
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
		struct BALL_EXPORT RecordTypeFormat
		{
			public:
			RecordType		record_type;
			char					string[7];
			const char*		format_string;
		};

		struct BALL_EXPORT Residue
		{
			ResidueName name;
			Character   chain_ID;
			Integer     sequence_number;
			AChar       insertion_code;
			
			Residue() { clear(); }
			void clear()
			{
				name[0] = '\0';
				chain_ID = ' ';
				sequence_number = 0;
				insertion_code = ' ';
			}
			void set(const ::BALL::Residue& residue)
			{
				strncpy(name, residue.getName().c_str(), 3);
				sequence_number = residue.getID().toInt();
				const Chain* chain = residue.getChain();
				chain_ID = (chain == 0 ? ' ' : chain->getName().c_str()[0]);
				insertion_code = residue.getInsertionCode();
			}
		};


		/**
		*/
		struct BALL_EXPORT RecordUNKNOWN
		{
			Real max_record_size[SIZE_OF_PDB_RECORD_LINE];
		};
		
		/**
		*/
		struct BALL_EXPORT RecordANISOU
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			Residue			residue;
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
		struct BALL_EXPORT RecordATOM
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			Residue			residue;
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
		struct BALL_EXPORT RecordAUTHOR
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBList      authors;
		};

		/**
		*/
		struct BALL_EXPORT RecordCAVEAT
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			IDcode       entry_code;
			PDBString    comment;
		};

		/**
		*/
		struct BALL_EXPORT RecordCISPEP
		{
			///
			RecordType     record_type;
			RecordName     record_name;
			Integer        record_serial_number;
			Residue				 residue[2];
			Integer        specific_model_ID;
			Real           angle_measure;
		};

		/**
		*/
		struct BALL_EXPORT RecordCOMPND
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			SpecificationList component_description;
		};
		
		/**
		*/
		struct BALL_EXPORT RecordCONECT
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bond_atom[4];
			Integer    hbond_atom[4];
			Integer    salt_bridge_atom[2];
			
			void clear()
			{
				record_type = PDB::RECORD_TYPE__CONECT;
				record_name[0] = '\0';
				atom_serial_number = 0;
				bond_atom[0] = bond_atom[1] = bond_atom[2] = bond_atom[3] = 0;
				hbond_atom[0] = hbond_atom[1] = hbond_atom[2] = hbond_atom[3] = 0;
				salt_bridge_atom[0] = salt_bridge_atom[1] = 0;
			}
		};

		/**
		*/
		struct BALL_EXPORT RecordCON06
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bond_atom[4];
			
			void clear()
			{
				record_type = PDB::RECORD_TYPE__CON06;
				record_name[0] = '\0';
				atom_serial_number = 0;
				bond_atom[0] = bond_atom[1] = bond_atom[2] = bond_atom[3] = 0;
			}
		};

		struct BALL_EXPORT RecordCON061
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bond_atom[4];
			
			void clear()
			{
				record_type = PDB::RECORD_TYPE__CON061;
				record_name[0] = '\0';
				atom_serial_number = 0;
				bond_atom[0] = bond_atom[1] = bond_atom[2] = bond_atom[3] = 0;
			}
		};

		struct BALL_EXPORT RecordCON062
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bond_atom[1];
			
			void clear()
			{
				record_type = PDB::RECORD_TYPE__CON062;
				record_name[0] = '\0';
				atom_serial_number = 0;
				bond_atom[0] = 0;
			}
		};

		struct BALL_EXPORT RecordCON063
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bond_atom[2];
			
			void clear()
			{
				record_type = PDB::RECORD_TYPE__CON063;
				record_name[0] = '\0';
				atom_serial_number = 0;
				bond_atom[0] = bond_atom[1] = 0;
			}
		};

		struct BALL_EXPORT RecordCON064
		{
			RecordType record_type;
			RecordName record_name;
			Integer    atom_serial_number;
			Integer    bond_atom[3];
			
			void clear()
			{
				record_type = PDB::RECORD_TYPE__CON064;
				record_name[0] = '\0';
				atom_serial_number = 0;
				bond_atom[0] = bond_atom[1] = bond_atom[2]  = 0;
			}
		};


		/**
		*/
		struct BALL_EXPORT RecordCRYST1
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
		struct BALL_EXPORT RecordDBREF
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
		struct BALL_EXPORT RecordEND
		{
			RecordType record_type;
			RecordName record_name;
		};

		///
		struct BALL_EXPORT RecordENDMDL
		{
			RecordType record_type;
			RecordName record_name;
		};
		
		///
		struct BALL_EXPORT RecordEXPDTA
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			SList        technique;
		};
		
		///
		struct BALL_EXPORT RecordFORMUL
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
		struct BALL_EXPORT RecordFTNOTE
		{
			RecordType record_type;
			RecordName record_name;
			Integer    number;
			PDBString  text;
		};
		
		///
		struct BALL_EXPORT RecordHEADER
		{
			RecordType record_type;
			RecordName record_name;
			String40   classification;
			Date       deposition_date;
			IDcode     ID_code;
		};
		
		///
		struct BALL_EXPORT RecordHELIX
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       serial_number;
			LString3      helix_ID;
			Residue				initial_residue;
			Residue				terminal_residue;
			Integer       helix_class;
			PDBString     comment;
			Integer       length;

			RecordHELIX() { clear(); }
			void clear()
			{
				record_type = RECORD_TYPE__HELIX;
				strncpy(record_name, RECORD_TAG_HELIX, 6);
				serial_number = 0;
				helix_ID[0] = '\0';
				initial_residue.clear();
				terminal_residue.clear();
				helix_class = 0;
				comment[0] = '\0';
				length = 0;
			}
		};
		
		///
		struct BALL_EXPORT RecordHET
		{
			RecordType record_type;
			RecordName record_name;
			Residue		 residue;
			Integer    number_of_HETATM_records;
			PDBString  text;
		};
		
		///
		struct BALL_EXPORT RecordHETATM
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			Residue			residue;
			Real        orthogonal_vector[3];
			Real        occupancy;
			Real        temperature_factor;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};
		
		///
		struct BALL_EXPORT RecordHETNAM
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			LString3     het_ID;
			PDBString    chemical_name;
		};
		
		///
		struct BALL_EXPORT RecordHETSYN
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			LString3     het_ID;
			SList				 het_synonyms;
		};
		
		///
		struct BALL_EXPORT RecordHYDBND
		{
			RecordType       record_type;
			RecordName       record_name;
			///
			struct BALL_EXPORT HydrogenPartnerAtom
			{
				Atom           atom_name;
				Character      alternate_location_indicator;
				Residue				 residue;
			};

			HydrogenPartnerAtom hydrogen_partner_atom[2];
			///
			struct BALL_EXPORT HydrogenAtom
			{
				Atom           atom_name;
				Character      alternate_location_indicator;
				Residue				 residue;
			};
			HydrogenAtom hydrogen_atom;
			SymmetryOperator first_non_hydrogen_atom;
			SymmetryOperator second_non_hydrogen_atom;
		};
		
		///
		struct BALL_EXPORT RecordJRNL
		{
			RecordType record_type;
			RecordName record_name;
			LString    text;
		};
		///
		struct BALL_EXPORT RecordKEYWDS
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBList      keywords;
		};
		///
		struct BALL_EXPORT RecordLINK
		{
			RecordType      record_type;
			RecordName      record_name;
			///			
			struct BALL_EXPORT LinkPartner
			{
				Atom          atom_name;
				Character     alternate_location_indicator;
				Residue				residue;
			};
			LinkPartner link_partner[2];
			SymmetryOperator first_atom;
			SymmetryOperator second_atom;
		};
		///
		struct BALL_EXPORT RecordMASTER
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

			RecordMASTER() { clear(); }
			void clear()
			{
				record_type = RECORD_TYPE__MASTER;
				strcpy(record_name, RECORD_TAG_MASTER);
				zero = number_of_HET_records = number_of_HELIX_records
						 = number_of_SHEET_records = number_of_TURN_records
						 = number_of_SITE_records = number_of_ORIGX_SCALE_MTRIX_records
						 = number_of_ATOM_HETATM_records = number_of_TER_records
						 = number_of_CONECT_records = number_of_SEQRES_records = 0;
			}
		};
		///
		struct BALL_EXPORT RecordMODEL
		{
			RecordType record_type;
			RecordName record_name;
			Integer    model_serial_number;
		};
		///
		struct BALL_EXPORT RecordMODRES
		{
			RecordType  record_type;
			RecordName  record_name;
			IDcode      entry_code;
			Residue			residue;
			ResidueName standard_residue_name;
			PDBString   comment;
		};
		///
		struct BALL_EXPORT RecordMTRIX1
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		///
		struct BALL_EXPORT RecordMTRIX2
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		///
		struct BALL_EXPORT RecordMTRIX3
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       transformation_matrix[4];
			Integer    is_given;
		};
		///
		struct BALL_EXPORT RecordOBSLTE
		{
			RecordType	 record_type;
			RecordName	 record_name;
			Continuation continuation;
			Date         entry_replaced_date;
			IDcode       entry_code;
			IDcode       replacing_entry_code[8];
		};
		///
		struct BALL_EXPORT RecordORIGX1
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		///
		struct BALL_EXPORT RecordORIGX2
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		///
		struct BALL_EXPORT RecordORIGX3
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};
		///
		struct BALL_EXPORT RecordREMARK
		{
			RecordType record_type;
			RecordName record_name;
			Integer    remark_number;
			LString    text;
		};
		///
		struct BALL_EXPORT RecordREVDAT
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
		struct BALL_EXPORT RecordSCALE1
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		///
		struct BALL_EXPORT RecordSCALE2
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		///
		struct BALL_EXPORT RecordSCALE3
		{
			RecordType record_type;
			RecordName record_name;
			Real       transformation_matrix[4];
		};

		///
		struct BALL_EXPORT RecordSEQADV
		{
			RecordType	record_type;
			RecordName	record_name;
			IDcode			id_code;
			Residue			residue;
			LString4		database;
			LString9		db_id_code;
			LString3		db_res;
			Integer			db_seq;
			LString21		comment;
		};

		///
		struct BALL_EXPORT RecordSEQRES
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Character   chain_ID;
			Integer     number_of_residues_in_chain;
			ResidueName residue_name[13];

			RecordSEQRES()
			{
				clear();
			}

			void clear()
			{
				record_type = RECORD_TYPE__SEQRES;
				strncpy(record_name, RECORD_TAG_SEQRES, 6);
				serial_number = 0;
				chain_ID = ' ';
				number_of_residues_in_chain = 0L;
				residue_name[0][0] = '\0';
				residue_name[1][0] = '\0';
				residue_name[2][0] = '\0';
				residue_name[3][0] = '\0';
				residue_name[4][0] = '\0';
				residue_name[5][0] = '\0';
				residue_name[6][0] = '\0';
				residue_name[7][0] = '\0';
				residue_name[8][0] = '\0';
				residue_name[9][0] = '\0';
				residue_name[10][0] = '\0';
				residue_name[11][0] = '\0';
				residue_name[12][0] = '\0';
			}
		};

		///
		struct BALL_EXPORT RecordSHEET
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       strand_number;
			LString3      sheet_ID;
			Integer       number_of_strands;
			Residue				initial_residue;
			Residue				terminal_residue;
			Integer       sense_of_strand;
			Atom          atom_name_in_current_strand;
			Residue				residue_in_current_strand;
			Atom          atom_name_in_previous_strand;
			Residue				residue_in_previous_strand;

			RecordSHEET() { clear(); }
			void clear()
			{
				record_type = RECORD_TYPE__SHEET;
				strncpy(record_name, RECORD_TAG_SHEET, 6);
				strand_number = 0;
				sheet_ID[0] = '\0';;
				number_of_strands = 0; 
				initial_residue.clear();
				terminal_residue.clear();
				sense_of_strand = 0;
				atom_name_in_current_strand[0] = '\0';
				residue_in_current_strand.clear();
				atom_name_in_previous_strand[0] = '\0';
				residue_in_previous_strand.clear();
			}
		};

		///
		struct BALL_EXPORT RecordSIGATM
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;
			Residue			residue;
			Real        standard_vector_deviation[3];
			Real        standard_occupancy_deviation;
			Real        standard_temperature_deviation;
			LString4    segment_ID;
			LString2    element_symbol;
			LString2    charge; 
		};

		///
		struct BALL_EXPORT RecordSIGUIJ
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Atom        atom_name;
			Character   alternate_location_indicator;	
			Residue			residue;
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
		struct BALL_EXPORT RecordSITE
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       sequence_number;
			LString3      name;
			Integer       number_of_residues;
			Residue				residue[4];
		};

		///
		struct BALL_EXPORT RecordSLTBRG
		{
			RecordType      record_type;
			RecordName      record_name;
			///
			struct BALL_EXPORT PartnerAtom
			{
				Atom          atom_name;
				Character     alternate_location_indicator;
				Residue				residue;
			};
			
			PartnerAtom partner_atom[2];
			SymmetryOperator first_atom;
			SymmetryOperator second_atom;
		};

		///
		struct BALL_EXPORT RecordSOURCE
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			SpecificationList sources;
		};

		///
		struct BALL_EXPORT RecordSPRSDE
		{
			RecordType        record_type;
			RecordName        record_name;
			Continuation      continuation;
			Date							date;
			IDcode						id_code;
			IDcode						old_codes[8];
		};

		///
		struct BALL_EXPORT RecordSSBOND
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       serial_number;
			Residue				partner_residue[2];
			SymmetryOperator symmetry_operator[2];
			
			RecordSSBOND() { clear(); }
			void clear()
			{
				record_type = RECORD_TYPE__SSBOND;
				strncpy(record_name, RECORD_TAG_SSBOND, 6);
				serial_number = 0;
				partner_residue[0].clear();
				partner_residue[1].clear();
				symmetry_operator[0] = 0;
				symmetry_operator[1] = 0;
			}
		};

		/**
		*/
		struct BALL_EXPORT RecordTER
		{
			RecordType  record_type;
			RecordName  record_name;
			Integer     serial_number;
			Residue			residue;
		};

		/**
		*/
		struct BALL_EXPORT RecordTITLE
		{
			RecordType   record_type;
			RecordName   record_name;
			Continuation continuation;
			PDBString    title;
		};

		/**
		*/
		struct BALL_EXPORT RecordTURN
		{
			RecordType    record_type;
			RecordName    record_name;
			Integer       sequence_number;
			LString3      turn_ID;
			Residue				initial_residue;
			Residue				terminal_residue;
			PDBString     comment;
			
			RecordTURN() { clear(); }
			void clear()
			{
 				record_type = RECORD_TYPE__TURN;
				strncpy(record_name, RECORD_TAG_TURN, 6);
				sequence_number = 0;
				turn_ID[0] = '\0';;
				initial_residue.clear();
				terminal_residue.clear();
				comment[0] = '\0';
			}
		};

		/**
		*/
		struct BALL_EXPORT RecordTVECT
		{
			RecordType record_type;
			RecordName record_name;
			Integer    serial_number;
			Real       translation_vector[3];
			PDBString  comment;
		};
		//@}

		/** This struct contains the number of records in a PDB file
				as required for the MASTER record.		
		*/
		struct BALL_EXPORT BookKeeping
		{
			Size remark_records;
			Size het_records;
			Size helix_records;
			Size sheet_records;
			Size turn_records;
			Size site_records;
			Size coordinate_transformation_records;
			Size atomic_coordinate_records;
			Size ter_records;
			Size conect_records;
			Size seqres_records;
			
			BookKeeping()
			{
				clear();
			}

			void clear()
			{
				remark_records = 0;
				het_records = 0;
				helix_records = 0;
				sheet_records = 0;
				turn_records = 0;
				site_records = 0;
				coordinate_transformation_records = 0;
				atomic_coordinate_records = 0;
				ter_records = 0;
				conect_records = 0;
				seqres_records = 0;
			}
		};


		struct BALL_EXPORT Structure	
		{
			struct BALL_EXPORT AtomEntry
			{
				const BALL::Atom* atom;
				const BALL::Residue* residue;
				const BALL::SecondaryStructure* sec_struct;
				const BALL::Chain* chain;
				AtomEntry(const BALL::Atom& a)
				{
					atom = &a;
					residue = a.getResidue();
					sec_struct = a.getSecondaryStructure();
					chain = a.getChain();
				}
			};
			

			struct BALL_EXPORT ConectAtomList
			{
				Position serial_number;
				std::list<Position> bonds;
				std::list<Position> hbonds;
				std::list<Position> saltbridges;
			};

			struct BALL_EXPORT UnitCellInfo
			{
				Real     a;
				Real     b;
				Real     c;
				Real     alpha;
				Real     beta;
				Real     gamma;
				LString  space_group;
				Integer  z_value;

				UnitCellInfo()
					: a(0),
						b(0),
						c(0),
						alpha(0),
						beta(0),
						gamma(0),
						z_value(-1)
				{
					strcpy(space_group, "");	
				}
			} unitcell_info;

			struct BALL_EXPORT NCSMatrix
			{
				Real     matrix[12];
				Integer  isgiven;
				NCSMatrix(const BALL::Matrix4x4& m, bool is_given)
				{
					matrix[0] = m(0,0); matrix[1] = m(0,1); matrix[2] = m(0,2); matrix[3] = m(0,3);
					matrix[4] = m(1,0); matrix[5] = m(1,1); matrix[6] = m(1,2); matrix[7] = m(1,3);
					matrix[8] = m(2,0); matrix[9] = m(2,1); matrix[10] = m(2,2); matrix[11] = m(2,3);
					isgiven = is_given;
				}
			};

			std::vector<NCSMatrix> ncs_matrices;
			std::vector<AtomEntry> atoms;

			HashSet<const ::BALL::Atom*>	conect_atoms;
			std::vector<const Bond*>			hydrogen_bonds;
			std::vector<const Bond*>			saltbridges;
			std::vector<const Bond*>			ssbonds;

			std::vector<std::pair<AChar, String> >		chain_residue_names;
			std::vector<const SecondaryStructure*>		sec_structs;

			void clear()
			{
				atoms.clear();
				conect_atoms.clear();
				hydrogen_bonds.clear();
				saltbridges.clear();
				ssbonds.clear();
			}

		};

		/**	A structure tracking the current values of all auxiliary information required to write ATOM or HETATM records.
				This is used for keeping track of chain and residue names while writing multiple atoms.
		*/
		struct BALL_EXPORT AdditionalAtomInfo
		{
			const Chain* current_chain;
			const ::BALL::Residue* current_residue;
			Integer residue_id;
			AChar residue_insertion_code;
			AChar	chain_id;
			LString3 residue_name;
			LString4 pdb_id;
			Position number; // number of the ATOM/HETATM/TER record
			
			AdditionalAtomInfo()
				:	current_chain(0x0),
					current_residue(0x0),
					residue_id(1),
					residue_insertion_code(' '),
					chain_id(' '),
					number(1)
			{
				strncpy(residue_name, "UNK", 4);
				strncpy(pdb_id, "    ", 5);
			}
		};

		/**	Maps all record formats and tags to the record type.
		*/
    BALL_EXPORT extern const RecordTypeFormat RECORD_TYPE_FORMAT [];

	} // namespace PDB

} // namespace BALL

#endif // BALL_FORMAT_PDBDEFS_H
