// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.C,v 1.47 2005/02/11 15:27:13 oliver Exp $
//

#include <BALL/FORMAT/PDBFile.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/DATATYPE/regularExpression.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/DATATYPE/regularExpression.h>

#include <time.h> // time, asctime
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

using namespace std;

namespace BALL 
{


	extern "C" int 
	#ifdef BALL_COMPILER_MSVC
	__cdecl
	#endif
	PDBFileRecordNameComparator_
		(const void* a_ptr, const void* b_ptr)
	{
		return memcmp
			(((PDB::RecordTypeFormat*)a_ptr)->string, 
			 ((PDB::RecordTypeFormat*)b_ptr)->string, 6);
	}


	const PDB::RecordTypeFormat
	PDBFile::record_type_format_[] =
	{
		{ PDB::RECORD_TYPE__UNKNOWN,"      ", "%.6s" },
		{ PDB::RECORD_TYPE__ANISOU, "ANISOU", "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c %7ld%7ld%7ld%7ld%7ld%7ld  %4.4s%2.2s%2.2s" },
		{ PDB::RECORD_TYPE__ATOM,   "ATOM  ", "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%2.2s%2.2s" },
		{ PDB::RECORD_TYPE__AUTHOR, "AUTHOR", "%.6s  %2ld%-60.60s" },
		{ PDB::RECORD_TYPE__CAVEAT, "CAVEAT", "%.6s  %2ld %4.4s    %51.51s" },
		{ PDB::RECORD_TYPE__CISPEP, "CISPEP", "%.6s %3ld %3.3s %c %4ld%c   %3.3s %c %4ld%c       %3ld       %6f" },
		{ PDB::RECORD_TYPE__COMPND, "COMPND", "%.6s  %2ld%-60.60s" },
		{ PDB::RECORD_TYPE__CONECT, "CONECT", "%.6s%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld" },
		{ PDB::RECORD_TYPE__CRYST1, "CRYST1", "%.6s%9f%9f%9f%7f%7f%7f %11.11s%4ld"},
		{ PDB::RECORD_TYPE__DBREF,  "DBREF ", "%.6s %4.4s %c %4ld%c %4ld%c %6.6s %8.8s %12.12s %5ld%c %5ld%c"},
		{ PDB::RECORD_TYPE__END,    "END   ", "%.6s" },
		{ PDB::RECORD_TYPE__ENDMDL, "ENDMDL", "%.6s" },
		{ PDB::RECORD_TYPE__EXPDTA, "EXPDTA", "%.6s  %2ld%60.60s" },
		{ PDB::RECORD_TYPE__FORMUL, "FORMUL", "%.6s  %2ld  %3.3s %2ld%c%51.51s" },
		{ PDB::RECORD_TYPE__FTNOTE, "FTNOTE", "%.6s %3ld %59.59s" },
		{ PDB::RECORD_TYPE__HEADER, "HEADER", "%.6s    %-40.40s%9.9s   %4.4s" },
		{ PDB::RECORD_TYPE__HELIX,  "HELIX ", "%.6s %3ld %3.3s %3.3s %c %4ld%c %3.3s %c %4ld%c%2ld%30.30s %5ld" },
		{ PDB::RECORD_TYPE__HET,    "HET   ", "%.6s %3.3s  %c%4ld%c  %5ld  %43.43s" },
		{ PDB::RECORD_TYPE__HETATM, "HETATM", "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c   %8.3f%8.3f%8.3f%6.2f%6.2f      %4.4s%2.2s%2.2s" },
		{ PDB::RECORD_TYPE__HETNAM, "HETNAM", "%.6s  %2ld %3.3s %55.55s" },
		{ PDB::RECORD_TYPE__HYDBND, "HYDBND", "%.6s      %4.4s%c%3.3s %c%5ld%c %4.4s%c %c%5ld%c %4.4s%c%3.3s %c%5ld%c%6ld %6ld" },
		{ PDB::RECORD_TYPE__JRNL,   "JRNL  ", "%.6s      %58.58s" },
		{ PDB::RECORD_TYPE__KEYWDS, "KEYWDS", "%.6s  %2ld%60.60s" },
		{ PDB::RECORD_TYPE__LINK,   "LINK  ", "%.6s      %4.4s%c%3.3s %c%4ld%c               %4.4s%c%3.3s %c%4ld%c  %6ld %6ld" },
		{ PDB::RECORD_TYPE__MASTER, "MASTER", "%.6s    %5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld%5ld" },
		{ PDB::RECORD_TYPE__MODEL,  "MODEL ", "%.6s    %4ld" },
		{ PDB::RECORD_TYPE__MODRES, "MODRES", "%.6s %3.3s %3.3s %c %4ld%c %3.3s  %41.41s" },
		{ PDB::RECORD_TYPE__MTRIX1, "MTRIX1", "%.6s %3ld%10f%10f%10f     %10f    %1ld" },
		{ PDB::RECORD_TYPE__MTRIX2, "MTRIX2", "%.6s %3ld%10f%10f%10f     %10f    %1ld" },
		{ PDB::RECORD_TYPE__MTRIX3, "MTRIX3", "%.6s %3ld%10f%10f%10f     %10f    %1ld" },
		{ PDB::RECORD_TYPE__OBSLTE, "OBSLTE", "%.6s  %2ld %9.9s %4.4s      %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s %4.4s" },
		{ PDB::RECORD_TYPE__ORIGX1, "ORIGX1", "%.6s    %10f%10f%10f     %10f" },
		{ PDB::RECORD_TYPE__ORIGX2, "ORIGX2", "%.6s    %10f%10f%10f     %10f" },
		{ PDB::RECORD_TYPE__ORIGX3, "ORIGX3", "%.6s    %10f%10f%10f     %10f" },
		{ PDB::RECORD_TYPE__REMARK, "REMARK", "%.6s %3ld %-59.59s" },
		{ PDB::RECORD_TYPE__REVDAT, "REVDAT", "%.6s %3ld%2ld %9.9s %5.5s   %1ld       %6.6s %6.6s %6.6s %6.6s" },
		{ PDB::RECORD_TYPE__SCALE1, "SCALE1", "%.6s    %10f%10f%10f     %10f" },
		{ PDB::RECORD_TYPE__SCALE2, "SCALE2", "%.6s    %10f%10f%10f     %10f" },
		{ PDB::RECORD_TYPE__SCALE3, "SCALE3", "%.6s    %10f%10f%10f     %10f" },
		{ PDB::RECORD_TYPE__SEQRES, "SEQRES", "%.6s  %2ld %c %4ld  %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s %3.3s" },
		{ PDB::RECORD_TYPE__SHEET,  "SHEET ", "%.6s %3ld %3.3s%2ld %3.3s %c%4ld%c %3.3s %c%4ld%c%2ld %-4.4s%3.3s %c%4ld%c %-4.4s%3.3s %c%4ld%c" },
		{ PDB::RECORD_TYPE__SIGATM, "SIGATM", "%.6s%5ld %4.4s%c%3.3s %c%4ld%c   %8f%8f%8f%6f%6f      %4.4s%2.2s%2.2s" },
		{ PDB::RECORD_TYPE__SIGUIJ, "SIGUIJ", "%.6s%5ld %-4.4s%c%3.3s %c%4ld%c %7ld%7ld%7ld%7ld%7ld%7ld  %4.4s%2.2s%2.2s" },
		{ PDB::RECORD_TYPE__SITE,   "SITE  ", "%.6s %3ld %3.3s %2ld %3.3s %c%4ld%c %3.3s %c%4ld%c %3.3s %c%4ld%c %3.3s %c%4ld%c" },
		{ PDB::RECORD_TYPE__SLTBRG, "SLTBRG", "%.6s      %4.4s%c%3.3s %c%4ld%c               %4.4s%c%3.3s %c%4ld%c  %6ld%6ld" },
		{ PDB::RECORD_TYPE__SOURCE, "SOURCE", "%.6s  %2ld%-60.60s" },
		{ PDB::RECORD_TYPE__SSBOND, "SSBOND", "%.6s %3ld %3.3s %c %4ld%c   %3.3s %c %4ld%c                       %6ld %6ld" },
		{ PDB::RECORD_TYPE__TER,    "TER   ", "%.6s%5ld      %3.3s %c%4ld%c" },
		{ PDB::RECORD_TYPE__TITLE,  "TITLE ", "%.6s  %2ld%60.60s" },
		{ PDB::RECORD_TYPE__TURN,   "TURN  ", "%.6s %3ld %3.3s %3.3s %c%4ld%c %3.3s %c%4ld%c    %-30.30s" },
		{ PDB::RECORD_TYPE__TVECT,  "TVECT ", "%.6s %3ld%10f%10f%10f%30.30s" }
	};


	// options and defaults for the PDBFile class

	const char* PDBFile::Option::VERBOSITY = "verbosity";
	const char* PDBFile::Option::STRICT_LINE_CHECKING = "strict_line_checking";
	const char* PDBFile::Option::CHOOSE_MODEL = "choose_model";
	const char* PDBFile::Option::STORE_SKIPPED_RECORDS = "store_skipped_records";
	const char* PDBFile::Option::IGNORE_XPLOR_PSEUDO_ATOMS = "ignore_xplor_pseudo_atoms";

	const Index PDBFile::Default::VERBOSITY = 0;
	const bool  PDBFile::Default::STRICT_LINE_CHECKING = false;
	const Index PDBFile::Default::CHOOSE_MODEL = 1;
	const bool  PDBFile::Default::STORE_SKIPPED_RECORDS = true;
	const bool  PDBFile::Default::IGNORE_XPLOR_PSEUDO_ATOMS = true;

	PDBFile::PDBFile()
		:	File(),
			residue_name_(""),
			residue_sequence_number_(-1),
			current_model_(INVALID_INDEX),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN),

			insertion_code_(0),
			chain_ID_(0),
			sequence_number_(-1),
			code_(),
			current_protein_(0),
			current_const_protein_(0),
			current_chain_(0),
			current_const_chain_(0),
			current_residue_(0),
			current_const_residue_(0),
			current_PDB_atom_(0),
			current_const_atom_(0),
			name_(),
			verbosity_(0),
			strict_line_checking_(false),
			selected_model_(1),
			store_skipped_records_(true),
			ignore_xplor_pseudo_atoms_(true),
			parse_partial_charges_(false)
	{
		init_();
	}

	PDBFile::PDBFile(const Options& new_options)
		:	File(),
			options(new_options),
			residue_name_(""),
			residue_sequence_number_(-1),
			current_model_(INVALID_INDEX),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN),

			insertion_code_(0),
			chain_ID_(0),
			sequence_number_(-1),
			code_(),
			current_protein_(0),
			current_const_protein_(0),
			current_chain_(0),
			current_const_chain_(0),
			current_residue_(0),
			current_const_residue_(0),
			current_PDB_atom_(0),
			current_const_atom_(0),
			name_(),
			verbosity_(0),
			strict_line_checking_(false),
			selected_model_(1),
			store_skipped_records_(true),
			ignore_xplor_pseudo_atoms_(true),
			parse_partial_charges_(false)	
	{
		init_();
	}

	const char* PDBFile::getAtomElementSymbol
		(const PDB::Atom atom_name, PDB::Atom element_symbol)
	{
		// If the element_symbol entry is valid, it has precedence
		if (((element_symbol[0] == ' ') && (element_symbol[1] == ' '))
				|| (element_symbol[0] == '\0') || (element_symbol[1] == '\0')
				|| String::isDigit(element_symbol[0]) || String::isDigit(element_symbol[1])
				|| (String::isAlpha(element_symbol[0]) && (element_symbol[1] == ' ')))
		{
			// Otherwise, we try to reconstruct the element
			// from the atom name (which is dangerous if non-PDB names are
		  // present, e.g. names like HE12 which is read as He!)
			if (atom_name[0] == ' ' || isdigit(atom_name[0]))
			{
				if (atom_name[1] == ' ')
				{
					element_symbol[0] = '\0';
				}
				else
				{
					element_symbol[0] = atom_name[1];
					element_symbol[1] = '\0';
				}
			}
			else
			{
				element_symbol[0] = atom_name[0];
				element_symbol[1] = atom_name[1];
				element_symbol[2] = '\0';
			}
		}
		else
		{
 			// if the element symbol starts with a blank,
			// drop the blank
			if (element_symbol[0] == ' ')
			{
				element_symbol[0] = element_symbol[1];
				element_symbol[1] = '\0';
			}
		}

		return (const char*)element_symbol;
	}

	Size PDBFile::countRecord
		(PDB::RecordType record_type, bool from_begin_of_file)
	{
		if (eof())
		{
			clear(0); 
		}
		else if (bad())
		{
			return 0;
		}

		streampos old_pos = tellg();
		Size size = 0;

		if (from_begin_of_file == true)
		{
			readFirstRecord(false);
		}
		
		for (;good(); readNextRecord(false))
		{
			if (current_record_type_ == record_type)
			{
				++size;
			}
		}

		seekg(old_pos);

		return size;
	}

	Size PDBFile::countRecords(bool from_begin_of_file)
	{
		if (eof())
		{
			clear(0); 
		}
		else if (bad())
		{
			return 0;
		}

		streampos old_pos = tellg();
		Size size = 0;

		if (from_begin_of_file == true)
		{
			readFirstRecord(false);
		}
		
		for (; good(); readNextRecord(false))
		{
			++size;
		}

		seekg(old_pos);

		return size;
	}

	bool PDBFile::readFirstRecord(bool read_values)
	{
		if (eof())
		{
			clear(0); 
		}
		else if (bad())
		{
			return false;
		}
		
		seekg(0, ios::beg);
		current_record_ = -1;

		// initialize the model as model 1 to prevent reading nothing if the
		// model specifier is missing.
		current_model_ = 1;
		info.setCurrentModel(1);

		return readNextRecord(read_values);
	}
		
	bool PDBFile::readNextRecord(bool read_values)
	{
		getline(line_buffer_, sizeof(line_buffer_) - 1, '\n');
		line_buffer_[sizeof(line_buffer_) - 1] = '\0';
		
		if (eof())
		{
			return false;
		}

		Size size = (Size)gcount();
		
		// The PDB format description says: "Each line in the PDB entry file
		// consists of 80 columns." 
		if (strict_line_checking_ == true)
		{
			if (size <= PDB::SIZE_OF_PDB_RECORD_LINE)
			{ 
				return readInvalidRecord(line_buffer_);
			}
		}
			
		++current_record_;

		return readLine(line_buffer_, size, read_values);
	}

	bool PDBFile::readRecords()
	{
		// Extract all relevant options
		verbosity_ = options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		strict_line_checking_ = options.setDefaultBool(Option::STRICT_LINE_CHECKING, Default::STRICT_LINE_CHECKING);
		selected_model_ = options.setDefaultInteger(Option::CHOOSE_MODEL, Default::CHOOSE_MODEL);
		ignore_xplor_pseudo_atoms_ = options.setDefaultBool(Option::IGNORE_XPLOR_PSEUDO_ATOMS, Default::IGNORE_XPLOR_PSEUDO_ATOMS);
		store_skipped_records_ = options.setDefaultBool(Option::STORE_SKIPPED_RECORDS, Default::STORE_SKIPPED_RECORDS);
													 
		// Clear the information in info and prepare it for the new stuff.
		info.clear();

		if (readFirstRecord(true) == false)
		{
			return false;
		}

		while(good())
		{
			if (readNextRecord(true) == false && !eof())
			{
				return false;
			}
		}

		return true;
	}

	bool PDBFile::readUnknownRecord(const char* /* line */)
	{
		// Store the record in the skipped_records field of info
		// for retrieval by the used, we have no idea what
		// to do with it.
		return skipCurrentRecord();
	}

	bool PDBFile::readInvalidRecord(const char* line)
	{
		// We know how it *should* look like, but we cannot parse
		// it the way it is. Store it in invalid records of info
		info.getInvalidRecords().push_back(line);
		return true;
	}

	bool PDBFile::skipCurrentRecord()
	{
		// Store skipped stuff only if the corresponding flag is set
		if (store_skipped_records_ == true)
		{
			info.getSkippedRecords().push_back(line_buffer_);
		}
		return true;
	}

	bool PDBFile::readRecordANISOU
		(PDB::Integer /* serial_number */,
		 PDB::Atom /* atom_name */,
		 PDB::Character /* alternate_location_indicator */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Integer /* u11 */,
		 PDB::Integer /* u22 */,
		 PDB::Integer /* u33 */,
		 PDB::Integer /* u12 */,
		 PDB::Integer /* u13 */,
		 PDB::Integer /* u23 */,
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 /* element_symbol */,
		 PDB::LString2 /* charge */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordAUTHOR
		(PDB::Continuation /* continuation */,
		 PDB::PDBList /* authors */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordCAVEAT
		(PDB::Continuation  /* continuation */,
		 PDB::IDcode /* entry_code */,
		 PDB::PDBString /* comment */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordCISPEP
		(PDB::Integer /* record_serial_number */,
		 PDB::RecordCISPEP::CisPeptide* /* cis_peptide[2] */,
		 PDB::Integer /*  specific_model_ID */,
		 PDB::Real /* angle_measure */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordCOMPND
		(PDB::Continuation /* continuation */,
		 PDB::SpecificationList /* component_description */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordCRYST1
		(PDB::RecordCRYST1::UnitCell& /* unit_cell */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordDBREF
		(PDB::IDcode /* entry_code */,
		 PDB::Character /* chain_ID */,
		 PDB::RecordDBREF::InitialSequence& /* initial_sequence */,
		 PDB::RecordDBREF::EndingSequence& /* ending_sequence */,
		 PDB::LString6 /* sequence_database_name */,
		 PDB::LString8 /* sequence_database_accession_code*/,
		 PDB::LString12 /* sequence_database_ID_code */,
		 PDB::RecordDBREF::InitialDatabaseSegment& /* initial_database_segment */,
		 PDB::RecordDBREF::EndingDatabaseSegment& /* ending_database_segment */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordEND()
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordENDMDL()
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordEXPDTA
		(PDB::Continuation /* continuation */,
		 PDB::SList /* technique */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordFORMUL
		(PDB::Integer /* component_number */,
		 PDB::LString3 /* het_ID */,
		 PDB::Integer /* continuation_number */,
		 PDB::Character /* is_water */,
		 PDB::PDBString /* chemical_formula */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordFTNOTE
		(PDB::Integer /* number */,
		 PDB::PDBString /* text */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordHET
		(PDB::LString3 /* het_ID */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Integer /* number_of_HETATM_records */,
		 PDB::PDBString /* text */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordHETNAM
		(PDB::Continuation /* continuation */,
		 PDB::LString3 /* het_ID */,
		 PDB::PDBString /* chemical_name */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordHYDBND
		(PDB::RecordHYDBND::HydrogenPartnerAtom /* hydrogen_partner */[2],
		 PDB::RecordHYDBND::HydrogenAtom& /* hydrogen_atom */,
		 PDB::SymmetryOperator /* first_non_hydrogen_atom */,
		 PDB::SymmetryOperator /* second_non_hydrogen_atom */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordJRNL
		(PDB::LString /* text */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordKEYWDS
		(PDB::Continuation /* continuation */,
		 PDB::PDBList /* keywords */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordLINK
		(PDB::RecordLINK::LinkPartner /* link_partner */[2],
		 PDB::SymmetryOperator /* first_atom */,
		 PDB::SymmetryOperator /* second_atom */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordMASTER
		(PDB::Integer /* number_of_REMARK_records */,
		 PDB::Integer /* zero */,
		 PDB::Integer /* number_of_HET_records */,
		 PDB::Integer /* number_of_HELIX_records */,
		 PDB::Integer /* number_of_SHEET_records */,
		 PDB::Integer /* number_of_TURN_records */,
		 PDB::Integer /* number_of_SITE_records */,
		 PDB::Integer /* number_of_ORIGX_SCALE_MTRIX_records */,
		 PDB::Integer /* number_of_ATOM_HETATM_records */,
		 PDB::Integer /* number_of_TER_records */,
		 PDB::Integer /* number_of_CONECT_records */,
		 PDB::Integer /* number_of_SEQRES_records */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordMODEL
		(PDB::Integer /* model_serial_number */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordMODRES
		(PDB::IDcode /* entry_code */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::ResidueName /* standard_residue_name */,
		 PDB::PDBString /* comment */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordMTRIX1
		(PDB::Integer /* serial_number */,
		 PDB::Real /* transformation_matrix */[4],
		 PDB::Integer /* is_given */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordMTRIX2
		(PDB::Integer /* serial_number */,
		 PDB::Real /* transformation_matrix */[4],
		 PDB::Integer /* is_given */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordMTRIX3
		(PDB::Integer /* serial_number */,
		 PDB::Real /* transformation_matrix */[4],
		 PDB::Integer /* is_given */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordOBSLTE
		(PDB::Continuation /* continuation */,
		 PDB::Date /* entry_replaced_date */,
		 PDB::IDcode /* entry_code */,
		 PDB::IDcode /* replacing_entry */[8])
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordORIGX1
		(PDB::Real /* transformation_matrix */[4])
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordORIGX2
		(PDB::Real /* transformation_matrix */[4])
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordORIGX3
		(PDB::Real /* transformation_matrix */[4])
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordREMARK
		(PDB::Integer /* remark_number */,
		 PDB::LString /* text */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::readRecordREVDAT
		(PDB::Integer /* modification_number */,
		 PDB::Continuation /* continuation */,
		 PDB::Date /* modification_date */,
		 PDB::String5 /* modification_ID */,
		 PDB::Integer /* modification_type */,
		 PDB::LString6 /* name_of_modified_record */[4])
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSCALE1
		(PDB::Real /* transformation_matrix */[4])
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSCALE2
		(PDB::Real /* transformation_matrix */[4])
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSCALE3
		(PDB::Real /* transformation_matrix */[4])
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSEQRES
		(PDB::Integer /* serial_number */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* number_of_residues_in_chain */,
		 PDB::ResidueName /* residue_name */[13])
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSIGATM
		(PDB::Integer /* serial_number */,
		 PDB::Atom /* atom_name */,
		 PDB::Character /* alternate_location_indicator */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Real /* standard_vector_deviation */[3],
		 PDB::Real /* standard_occupancy_deviation */,
		 PDB::Real /* standard_temperature_deviation */,
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 /* element_symbol */,
		 PDB::LString2 /* charge */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSIGUIJ
		(PDB::Integer /* serial_number */,
		 PDB::Atom /* atom_name */,
		 PDB::Character /* alternate_location_indicator */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Integer /* sig11 */,
		 PDB::Integer /* sig22 */,
		 PDB::Integer /* sig33 */,
		 PDB::Integer /* sig12 */,
		 PDB::Integer /* sig13 */,
		 PDB::Integer /* sig23 */,
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 /* element_symbol */,
		 PDB::LString2 /* charge */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSITE
		(PDB::Integer /* sequence_number */,
		 PDB::LString3 /* name */,
		 PDB::Integer /* number_of_residues */,
		 PDB::RecordSITE::Residue /* residue */[4])
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSLTBRG
		(PDB::RecordSLTBRG::PartnerAtom /* partner_atom */[2],
		 PDB::SymmetryOperator /* first_atom */,
		 PDB::SymmetryOperator /* second_atom */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordSOURCE
		(PDB::Continuation /* continuation */,
		 PDB::SpecificationList /*sources */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordTITLE
		(PDB::Continuation /* continuation */,
		 PDB::PDBString /* title */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::readRecordTVECT
		(PDB::Integer /* serial_number */,
		 PDB::Real /* translation_vector */[3],
		 PDB::PDBString /*comment */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::hasFormat()
	{
		if (eof())
		{
			clear(0); 
		}
		else if (bad())
		{
			return false;
		}

		bool has_format;
		streampos old_pos = tellg();

		has_format= (readFirstRecord(false) == true
								 && current_record_type_ != PDB::RECORD_TYPE__UNKNOWN);

		seekg(old_pos);

		return has_format;
	}

	bool PDBFile::parseLine
		(const char* line, Size size, const char* format_string, ...)
	{
		static char formats[PDB::SIZE_OF_FORMAT_STRING_BUFFER];
		char* line_column = const_cast<char*>(line);
		char* format_column = 0;
		char* temp = 0;
		char* temp2 = 0;
		Size field_size = 0;
		char backup = 0;
		va_list var_args;
		union 
		{
			short *var_arg_short;
			long *var_arg_long; 
			double *var_arg_double;
			char *var_arg_char;
		};

		memcpy(formats, format_string, sizeof(formats));
		formats[sizeof(formats) - 1] = '\0';
		va_start(var_args, format_string);
		record_fields_ = 0;
		
		for (format_column = formats; *format_column != '\0'; ++format_column)
		{
			if (size < (Size)(line_column - line))
			{
				return readInvalidRecord(line);
			}
			
			if (*format_column == '%')
			{
				++record_fields_;
				
				++format_column;

				if (*format_column == '.' || *format_column == '-' || *format_column == '+')
				{
					++format_column;
				}

				temp2 = temp = format_column;

				for (; isdigit(*format_column); ++format_column);

				if (*format_column == '.')
				{
					++format_column;

					temp2 = format_column;
					for (; isdigit(*format_column); ++format_column)
					{
					}
				}
			}

			switch(*format_column)
			{
				case ' ':
					++line_column;
					continue;

				case 'l':
					*format_column = '\0';
					field_size = atoi(temp);
					temp = line_column;
					line_column += field_size;
					backup = *line_column;
					*line_column = '\0';
					var_arg_long = va_arg(var_args, long *);
					*var_arg_long = atol(temp);
					*line_column = backup;

					*format_column = 'l';

					// to skip the mandatory 'd'
					++format_column; 
					break;

				case 'f':
					*format_column = '\0';
					field_size = atoi(temp);
					temp = line_column;
					line_column += field_size;
					backup = *line_column;
					*line_column = '\0';
					var_arg_double = va_arg(var_args, double *);
					*var_arg_double = ::atof(temp);
					*line_column = backup;

					//	cout << "float (" << field_size << "): '" << *var_arg_double << "'" << endl;

					*format_column = 'd';
					break;

				case 's':
					*format_column = '\0';
					field_size = atoi(temp2);
					temp = line_column;
					line_column += field_size;
					backup = *line_column;
					*line_column = '\0';
					var_arg_char = va_arg(var_args, char *);
					strcpy(var_arg_char, temp);
					*line_column = backup;

					*format_column = 's';
					break;

				case 'c':
					var_arg_char = va_arg(var_args, char *);
					*var_arg_char = *line_column;

					++line_column;
					break;

				case 'h':
					*format_column = '\0';
					field_size = atoi(temp);
					temp = line_column;
					line_column += field_size;
					backup = *line_column;
					*line_column = '\0';
					var_arg_short = va_arg(var_args, short *);
					*var_arg_short = (short)atoi(temp);
					*line_column = backup;

					*format_column = 'h';
					// to skip the mandatory 'd'
					++format_column; 
					break;
			}
		}

		return true;
	}

	bool PDBFile::readLine(char* line, Size size, bool extract_values)
	{
		static PDB::RecordTypeFormat* record_type_format = 0;

		if (record_type_format == 0
				|| memcmp(record_type_format->string, line, 6) != 0)
		{
			memcpy(const_cast<char*>(compare_record_type_format_.string), line, 6);

			record_type_format = (PDB::RecordTypeFormat*)::bsearch 
				((const void*)&compare_record_type_format_, 
				 (const void*)record_type_format_, 
				 (Size)PDB::NUMBER_OF_REGISTERED_RECORD_TYPES, 
				 (Size)sizeof(PDB::RecordTypeFormat), 
				 PDBFileRecordNameComparator_);
			
			if (record_type_format == 0)
			{
				return readUnknownRecord(line);
			}
		}

		current_record_type_ = record_type_format->record_type;

		if (extract_values == false)
		{
			return true;
		}

		switch(current_record_type_)
		{
			case PDB::RECORD_TYPE__ANISOU:

				if ((selected_model_ != 0) && (selected_model_ != current_model_))
				{
					return true;
				}

				parseLine
					(line,
					 size,
					 record_type_format->format_string,
					 record_ANISOU.record_name,
					 &record_ANISOU.serial_number,
					 record_ANISOU.atom_name,
					 &record_ANISOU.alternate_location_indicator,
					 record_ANISOU.residue_name,
					 &record_ANISOU.chain_ID,
					 &record_ANISOU.residue_sequence_number,
					 &record_ANISOU.insertion_code,
					 &record_ANISOU.u11,
					 &record_ANISOU.u22,
					 &record_ANISOU.u33,
					 &record_ANISOU.u12,
					 &record_ANISOU.u13,
					 &record_ANISOU.u23,
					 record_ANISOU.segment_ID,
					 record_ANISOU.element_symbol,
					 record_ANISOU.charge);
				
				return readRecordANISOU
					(record_ANISOU.serial_number,
					 record_ANISOU.atom_name,
					 record_ANISOU.alternate_location_indicator,
					 record_ANISOU.residue_name,
					 record_ANISOU.chain_ID,
					 record_ANISOU.residue_sequence_number,
					 record_ANISOU.insertion_code,
					 record_ANISOU.u11,
					 record_ANISOU.u22,
					 record_ANISOU.u33,
					 record_ANISOU.u12,
					 record_ANISOU.u13,
					 record_ANISOU.u23,
					 record_ANISOU.segment_ID,
					 record_ANISOU.element_symbol,
					 record_ANISOU.charge);

				
			
			case PDB::RECORD_TYPE__ATOM:
				
				if ((selected_model_ != 0) && (selected_model_ != current_model_))
				{
					return true;
				}

				record_ATOM.element_symbol[0] = '\0';
				record_ATOM.occupancy = 1.0;
				record_ATOM.temperature_factor = 0.0;
				record_ATOM.segment_ID[0] = '\0';
				record_ATOM.charge[0] = '\0';

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_ATOM.record_name,
					 &record_ATOM.serial_number,
					 record_ATOM.atom_name,
					 &record_ATOM.alternate_location_indicator,
					 record_ATOM.residue_name,
					 &record_ATOM.chain_ID,
					 &record_ATOM.residue_sequence_number,
					 &record_ATOM.insertion_code,
					 &record_ATOM.orthogonal_vector[0],
					 &record_ATOM.orthogonal_vector[1],
					 &record_ATOM.orthogonal_vector[2],
					 &record_ATOM.occupancy,
					 &record_ATOM.temperature_factor,
					 record_ATOM.segment_ID,
					 record_ATOM.element_symbol,
					 record_ATOM.charge);

				if ((ignore_xplor_pseudo_atoms_ == true)
						&& record_ATOM.orthogonal_vector[0] >= 9998.0
						&& record_ATOM.orthogonal_vector[1] >= 9998.0
						&& record_ATOM.orthogonal_vector[2] >= 9998.0)
				{ // ignore XPLOR pseudo atoms (see Rasmol2.6 source 'molecule.c/ReadPDBAtom')
					return true;
				}

				return readRecordATOM
					(record_ATOM.serial_number,
					 record_ATOM.atom_name,
					 record_ATOM.alternate_location_indicator,
					 record_ATOM.residue_name,
					 record_ATOM.chain_ID,
					 record_ATOM.residue_sequence_number,
					 record_ATOM.insertion_code,
					 record_ATOM.orthogonal_vector,
					 record_ATOM.occupancy,
					 record_ATOM.temperature_factor,
					 record_ATOM.segment_ID,
					 record_ATOM.element_symbol,
					 record_ATOM.charge);
				


			case PDB::RECORD_TYPE__AUTHOR:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_AUTHOR.record_name,
					 &record_AUTHOR.continuation,
					 record_AUTHOR.authors);
				
				return readRecordAUTHOR(record_AUTHOR.continuation, record_AUTHOR.authors);


				
			case PDB::RECORD_TYPE__CAVEAT:

				parseLine
					(line, size, 
					 record_type_format->format_string,
					 record_CAVEAT.record_name,
					 &record_CAVEAT.continuation,
					 record_CAVEAT.entry_code,
					 record_CAVEAT.comment);

				return readRecordCAVEAT
					(record_CAVEAT.continuation,
					 record_CAVEAT.entry_code,
					 record_CAVEAT.comment);

				
			case PDB::RECORD_TYPE__CISPEP:

				parseLine
					(line, size, 
					 record_type_format->format_string,
					 record_CISPEP.record_name,
					 &record_CISPEP.record_serial_number,
					 record_CISPEP.cis_peptide[0].residue_name,
					 &record_CISPEP.cis_peptide[0].chain_ID,
					 &record_CISPEP.cis_peptide[0].residue_sequence_number,
					 &record_CISPEP.cis_peptide[0].insertion_code,
					 record_CISPEP.cis_peptide[1].residue_name,
					 &record_CISPEP.cis_peptide[1].chain_ID,
					 &record_CISPEP.cis_peptide[1].residue_sequence_number,
					 &record_CISPEP.cis_peptide[1].insertion_code,
					 &record_CISPEP.specific_model_ID,
					 &record_CISPEP.angle_measure);

				return readRecordCISPEP
					(record_CISPEP.record_serial_number,
					 record_CISPEP.cis_peptide,
					 record_CISPEP.specific_model_ID,
					 record_CISPEP.angle_measure);


				
			case PDB::RECORD_TYPE__COMPND:

				parseLine
					(line, size, 
					 record_type_format->format_string,
					 record_COMPND.record_name,
					 &record_COMPND.continuation,
					 record_COMPND.component_description);

				return readRecordCOMPND
					(record_COMPND.continuation,
					 record_COMPND.component_description);


				
			case PDB::RECORD_TYPE__CONECT:


				record_CONECT.bonded_atom_serial_number[0] = 0;
				record_CONECT.bonded_atom_serial_number[1] = 0;
				record_CONECT.bonded_atom_serial_number[2] = 0;
				record_CONECT.bonded_atom_serial_number[3] = 0;
				record_CONECT.hydrogen_bonded_atom_serial_number[0] = 0;
				record_CONECT.hydrogen_bonded_atom_serial_number[1] = 0;
				record_CONECT.hydrogen_bonded_atom_serial_number[2] = 0;
				record_CONECT.hydrogen_bonded_atom_serial_number[3] = 0;
				record_CONECT.salt_bridged_atom_serial_number[0] = 0;
				record_CONECT.salt_bridged_atom_serial_number[1] = 0;

				parseLine
					(line, size, 
					 record_type_format->format_string,
					 record_CONECT.record_name,
					 &record_CONECT.atom_serial_number,
					 &record_CONECT.bonded_atom_serial_number[0],
					 &record_CONECT.bonded_atom_serial_number[1],
					 &record_CONECT.bonded_atom_serial_number[2],
					 &record_CONECT.bonded_atom_serial_number[3],
					 &record_CONECT.hydrogen_bonded_atom_serial_number[0],
					 &record_CONECT.hydrogen_bonded_atom_serial_number[1],
					 &record_CONECT.salt_bridged_atom_serial_number[0],
					 &record_CONECT.hydrogen_bonded_atom_serial_number[2],
					 &record_CONECT.hydrogen_bonded_atom_serial_number[3],
					 &record_CONECT.salt_bridged_atom_serial_number[1]);

				return readRecordCONECT
					(record_CONECT.atom_serial_number,
					 record_CONECT.bonded_atom_serial_number,
					 record_CONECT.hydrogen_bonded_atom_serial_number,
					 record_CONECT.salt_bridged_atom_serial_number);
			


			case PDB::RECORD_TYPE__CRYST1:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_CRYST1.record_name,
					 &record_CRYST1.unit_cell.a,
					 &record_CRYST1.unit_cell.b,
					 &record_CRYST1.unit_cell.c,
					 &record_CRYST1.unit_cell.alpha,
					 &record_CRYST1.unit_cell.beta,
					 &record_CRYST1.unit_cell.gamma,
					 record_CRYST1.unit_cell.space_group,
					 &record_CRYST1.unit_cell.z_value);

				return readRecordCRYST1
					(record_CRYST1.unit_cell);



			case PDB::RECORD_TYPE__DBREF:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_DBREF.record_name,
					 record_DBREF.entry_code,
					 &record_DBREF.chain_ID,
					 &record_DBREF.initial_sequence.number,
					 &record_DBREF.initial_sequence.insertion_code,
					 &record_DBREF.ending_sequence.number,
					 &record_DBREF.ending_sequence.insertion_code,
					 record_DBREF.sequence_database_name,
					 record_DBREF.sequence_database_accession_code,
					 record_DBREF.sequence_database_ID_code,
					 &record_DBREF.chain_ID,
					 &record_DBREF.initial_database_segment.number,
					 &record_DBREF.initial_database_segment.insertion_code,
					 &record_DBREF.ending_database_segment.number,
					 &record_DBREF.ending_database_segment.insertion_code);

				return readRecordDBREF
					(record_DBREF.entry_code,
					 record_DBREF.chain_ID,
					 record_DBREF.initial_sequence,
					 record_DBREF.ending_sequence,
					 record_DBREF.sequence_database_name,
					 record_DBREF.sequence_database_accession_code,
					 record_DBREF.sequence_database_ID_code,
					 record_DBREF.initial_database_segment,
					 record_DBREF.ending_database_segment);



			case PDB::RECORD_TYPE__END:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_END.record_name);

				return readRecordEND();



			case PDB::RECORD_TYPE__ENDMDL:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_ENDMDL.record_name);

				current_model_ = INVALID_INDEX;

				return readRecordENDMDL();



			case PDB::RECORD_TYPE__EXPDTA:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_EXPDTA.record_name,
					 &record_EXPDTA.continuation,
					 record_EXPDTA.technique);

				return readRecordEXPDTA
					(record_EXPDTA.continuation,
					 record_EXPDTA.technique);



			case PDB::RECORD_TYPE__FORMUL:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_FORMUL.record_name,
					 &record_FORMUL.component_number,
					 record_FORMUL.het_ID,
					 &record_FORMUL.continuation_number,
					 &record_FORMUL.is_water,
					 record_FORMUL.chemical_formula);

				return readRecordFORMUL
					(record_FORMUL.component_number,
					 record_FORMUL.het_ID,
					 record_FORMUL.continuation_number,
					 record_FORMUL.is_water,
					 record_FORMUL.chemical_formula);



			case PDB::RECORD_TYPE__FTNOTE:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_FTNOTE.record_name,
					 &record_FTNOTE.number,
					 record_FTNOTE.text);

				return readRecordFTNOTE
					(record_FTNOTE.number,
					 record_FTNOTE.text);



			case PDB::RECORD_TYPE__HEADER:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_HEADER.record_name,
					 record_HEADER.classification,
					 record_HEADER.deposition_date,
					 record_HEADER.ID_code);

				return readRecordHEADER
					(record_HEADER.classification,
					 record_HEADER.deposition_date,
					 record_HEADER.ID_code);



			case PDB::RECORD_TYPE__HELIX:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_HELIX.record_name,
					 &record_HELIX.serial_number,
					 record_HELIX.helix_ID,
					 record_HELIX.initial_residue.residue_name,
					 &record_HELIX.initial_residue.chain_ID,
					 &record_HELIX.initial_residue.sequence_number,
					 &record_HELIX.initial_residue.insertion_code,
					 record_HELIX.terminal_residue.residue_name,
					 &record_HELIX.terminal_residue.chain_ID,
					 &record_HELIX.terminal_residue.sequence_number,
					 &record_HELIX.terminal_residue.insertion_code,
					 &record_HELIX.helix_class,
					 record_HELIX.comment,
					 &record_HELIX.length);

				return readRecordHELIX
					(record_HELIX.serial_number,
					 record_HELIX.helix_ID,
					 record_HELIX.initial_residue,
					 record_HELIX.terminal_residue,
					 record_HELIX.helix_class,
					 record_HELIX.comment,
					 record_HELIX.length);



			case PDB::RECORD_TYPE__HET:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_HET.record_name,
					 record_HET.het_ID,
					 &record_HET.chain_ID,
					 &record_HET.sequence_number,
					 &record_HET.insertion_code,
					 &record_HET.number_of_HETATM_records,
					 record_HET.text);

				return readRecordHET
					(record_HET.het_ID,
					 record_HET.chain_ID,
					 record_HET.sequence_number,
					 record_HET.insertion_code,
					 record_HET.number_of_HETATM_records,
					 record_HET.text);



			case PDB::RECORD_TYPE__HETATM:

				if ((selected_model_ != 0) && (selected_model_ != current_model_))
				{
					return true;
				}

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_HETATM.record_name,
					 &record_HETATM.serial_number,
					 record_HETATM.atom_name,
					 &record_HETATM.alternate_location_indicator,
					 record_HETATM.residue_name,
					 &record_HETATM.chain_ID,
					 &record_HETATM.residue_sequence_number,
					 &record_HETATM.insertion_code,
					 &record_HETATM.orthogonal_vector[0],
					 &record_HETATM.orthogonal_vector[1],
					 &record_HETATM.orthogonal_vector[2],
					 &record_HETATM.occupancy,
					 &record_HETATM.temperature_factor,
					 record_HETATM.segment_ID,
					 record_HETATM.element_symbol,
					 record_HETATM.charge);

				return readRecordHETATM
					(record_HETATM.serial_number,
					 record_HETATM.atom_name,
					 record_HETATM.alternate_location_indicator,
					 record_HETATM.residue_name,
					 record_HETATM.chain_ID,
					 record_HETATM.residue_sequence_number,
					 record_HETATM.insertion_code,
					 record_HETATM.orthogonal_vector,
					 record_HETATM.occupancy,
					 record_HETATM.temperature_factor,
					 record_HETATM.segment_ID,
					 record_HETATM.element_symbol,
					 record_HETATM.charge);



			case PDB::RECORD_TYPE__HETNAM:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_HETNAM.record_name,
					 &record_HETNAM.continuation,
					 record_HETNAM.het_ID,
					 record_HETNAM.chemical_name);

				return readRecordHETNAM
					(record_HETNAM.continuation,
					 record_HETNAM.het_ID,
					 record_HETNAM.chemical_name);



			case PDB::RECORD_TYPE__HYDBND:

				parseLine
					(line,
					 size, 
					 record_type_format->format_string,
					 record_HYDBND.record_name,
					 record_HYDBND.hydrogen_partner_atom[0].atom_name,
					 &record_HYDBND.hydrogen_partner_atom[0].alternate_location_indicator,
					 record_HYDBND.hydrogen_partner_atom[0].residue.residue_name,
					 &record_HYDBND.hydrogen_partner_atom[0].residue.chain_ID,
					 &record_HYDBND.hydrogen_partner_atom[0].residue.sequence_number,
					 &record_HYDBND.hydrogen_partner_atom[0].residue.insertion_code,
					 record_HYDBND.hydrogen_atom.atom_name,
					 &record_HYDBND.hydrogen_atom.alternate_location_indicator,
					 &record_HYDBND.hydrogen_atom.residue.chain_ID,
					 &record_HYDBND.hydrogen_atom.residue.sequence_number,
					 &record_HYDBND.hydrogen_atom.residue.insertion_code,
					 record_HYDBND.hydrogen_partner_atom[1].atom_name,
					 &record_HYDBND.hydrogen_partner_atom[1].alternate_location_indicator,
					 record_HYDBND.hydrogen_partner_atom[1].residue.residue_name,
					 &record_HYDBND.hydrogen_partner_atom[1].residue.chain_ID,
					 &record_HYDBND.hydrogen_partner_atom[1].residue.sequence_number,
					 &record_HYDBND.hydrogen_partner_atom[1].residue.insertion_code,
					 &record_HYDBND.first_non_hydrogen_atom,
					 &record_HYDBND.second_non_hydrogen_atom);

					 return readRecordHYDBND
						 (record_HYDBND.hydrogen_partner_atom,
							record_HYDBND.hydrogen_atom,
							record_HYDBND.first_non_hydrogen_atom,
							record_HYDBND.second_non_hydrogen_atom);



			case PDB::RECORD_TYPE__JRNL:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_JRNL.record_name,
							record_JRNL.text);

					 return readRecordJRNL
						 (record_JRNL.text);



			case PDB::RECORD_TYPE__KEYWDS:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_KEYWDS.record_name,
							&record_KEYWDS.continuation,
							record_KEYWDS.keywords);

					 return readRecordKEYWDS
						 (record_KEYWDS.continuation,
							record_KEYWDS.keywords);



			case PDB::RECORD_TYPE__LINK:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_LINK.record_name,
							record_LINK.link_partner[0].atom_name,
							&record_LINK.link_partner[0].alternate_location_indicator,
							record_LINK.link_partner[0].residue.residue_name,
							&record_LINK.link_partner[0].residue.chain_ID,
							&record_LINK.link_partner[0].residue.sequence_number,
							&record_LINK.link_partner[0].residue.insertion_code,
							record_LINK.link_partner[1].atom_name,
							&record_LINK.link_partner[1].alternate_location_indicator,
							record_LINK.link_partner[1].residue.residue_name,
							&record_LINK.link_partner[1].residue.chain_ID,
							&record_LINK.link_partner[1].residue.sequence_number,
							&record_LINK.link_partner[1].residue.insertion_code,
							&record_LINK.first_atom,
							&record_LINK.second_atom);

					 return readRecordLINK
						 (record_LINK.link_partner,
							record_LINK.first_atom,
							record_LINK.second_atom);



			case PDB::RECORD_TYPE__MASTER:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_MASTER.record_name,
							&record_MASTER.number_of_REMARK_records,
							&record_MASTER.zero,
							&record_MASTER.number_of_HET_records,
							&record_MASTER.number_of_HELIX_records,
							&record_MASTER.number_of_SHEET_records,
							&record_MASTER.number_of_TURN_records,
							&record_MASTER.number_of_SITE_records,
							&record_MASTER.number_of_ORIGX_SCALE_MTRIX_records,
							&record_MASTER.number_of_ATOM_HETATM_records,
							&record_MASTER.number_of_TER_records,
							&record_MASTER.number_of_CONECT_records,
							&record_MASTER.number_of_SEQRES_records);

					 return readRecordMASTER
						 (record_MASTER.number_of_REMARK_records,
							record_MASTER.zero,
							record_MASTER.number_of_HET_records,
							record_MASTER.number_of_HELIX_records,
							record_MASTER.number_of_SHEET_records,
							record_MASTER.number_of_TURN_records,
							record_MASTER.number_of_SITE_records,
							record_MASTER.number_of_ORIGX_SCALE_MTRIX_records,
							record_MASTER.number_of_ATOM_HETATM_records,
							record_MASTER.number_of_TER_records,
							record_MASTER.number_of_CONECT_records,
							record_MASTER.number_of_SEQRES_records);



			case PDB::RECORD_TYPE__MODEL:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_MODEL.record_name,
							&record_MODEL.model_serial_number);

					 current_model_ = (Index)record_MODEL.model_serial_number;

					 return readRecordMODEL
						 (record_MODEL.model_serial_number);



			case PDB::RECORD_TYPE__MODRES:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_MODRES.record_name,
							record_MODRES.entry_code,
							record_MODRES.residue_name,
							&record_MODRES.chain_ID,
							&record_MODRES.sequence_number,
							&record_MODRES.insertion_code,
							record_MODRES.standard_residue_name,
							record_MODRES.comment);

					 return readRecordMODRES
						 (record_MODRES.entry_code,
							record_MODRES.residue_name,
							record_MODRES.chain_ID,
							record_MODRES.sequence_number,
							record_MODRES.insertion_code,
							record_MODRES.standard_residue_name,
							record_MODRES.comment);



			case PDB::RECORD_TYPE__MTRIX1:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_MTRIX1.record_name,
							&record_MTRIX1.serial_number,
							&record_MTRIX1.transformation_matrix[0],
							&record_MTRIX1.transformation_matrix[1],
							&record_MTRIX1.transformation_matrix[2],
							&record_MTRIX1.transformation_matrix[3],
							&record_MTRIX1.is_given);

					 return readRecordMTRIX1
						 (record_MTRIX1.serial_number,
							record_MTRIX1.transformation_matrix,
							record_MTRIX1.is_given);



			case PDB::RECORD_TYPE__MTRIX2:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_MTRIX2.record_name,
							&record_MTRIX2.serial_number,
							&record_MTRIX2.transformation_matrix[0],
							&record_MTRIX2.transformation_matrix[1],
							&record_MTRIX2.transformation_matrix[2],
							&record_MTRIX2.transformation_matrix[3],
							&record_MTRIX2.is_given);

					 return readRecordMTRIX2
						 (record_MTRIX2.serial_number,
							record_MTRIX2.transformation_matrix,
							record_MTRIX2.is_given);



			case PDB::RECORD_TYPE__MTRIX3:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_MTRIX3.record_name,
							&record_MTRIX3.serial_number,
							&record_MTRIX3.transformation_matrix[0],
							&record_MTRIX3.transformation_matrix[1],
							&record_MTRIX3.transformation_matrix[2],
							&record_MTRIX3.transformation_matrix[3],
							&record_MTRIX3.is_given);

					 return readRecordMTRIX3
						 (record_MTRIX3.serial_number,
							record_MTRIX3.transformation_matrix,
							record_MTRIX3.is_given);



			case PDB::RECORD_TYPE__OBSLTE:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_OBSLTE.record_name,
							&record_OBSLTE.continuation,
							record_OBSLTE.entry_replaced_date,
							record_OBSLTE.entry_code,
							record_OBSLTE.replacing_entry_code[0],
							record_OBSLTE.replacing_entry_code[1],
							record_OBSLTE.replacing_entry_code[2],
							record_OBSLTE.replacing_entry_code[3],
							record_OBSLTE.replacing_entry_code[4],
							record_OBSLTE.replacing_entry_code[5],
							record_OBSLTE.replacing_entry_code[6],
							record_OBSLTE.replacing_entry_code[7]);

					 return readRecordOBSLTE
						 (record_OBSLTE.continuation,
							record_OBSLTE.entry_replaced_date,
							record_OBSLTE.entry_code,
							record_OBSLTE.replacing_entry_code);



			case PDB::RECORD_TYPE__ORIGX1:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_ORIGX1.record_name,
							&record_ORIGX1.transformation_matrix[0],
							&record_ORIGX1.transformation_matrix[1],
							&record_ORIGX1.transformation_matrix[2],
							&record_ORIGX1.transformation_matrix[3]);

					 return readRecordORIGX1
						 (record_ORIGX1.transformation_matrix);



			case PDB::RECORD_TYPE__ORIGX2:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_ORIGX2.record_name,
							&record_ORIGX2.transformation_matrix[0],
							&record_ORIGX2.transformation_matrix[1],
							&record_ORIGX2.transformation_matrix[2],
							&record_ORIGX2.transformation_matrix[3]);

					 return readRecordORIGX2
						 (record_ORIGX2.transformation_matrix);



			case PDB::RECORD_TYPE__ORIGX3:

					 parseLine
						 (line,
							size,
							record_type_format->format_string,
							record_ORIGX3.record_name,
							&record_ORIGX3.transformation_matrix[0],
							&record_ORIGX3.transformation_matrix[1],
							&record_ORIGX3.transformation_matrix[2],
							&record_ORIGX3.transformation_matrix[3]);

					 return readRecordORIGX3
						 (record_ORIGX3.transformation_matrix);



			case PDB::RECORD_TYPE__REMARK:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_REMARK.record_name,
							&record_REMARK.remark_number,
							record_REMARK.text);

					 return readRecordREMARK
						 (record_REMARK.remark_number,
							record_REMARK.text);



			case PDB::RECORD_TYPE__REVDAT:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_REVDAT.record_name,
							&record_REVDAT.modification_number,
							&record_REVDAT.continuation,
							record_REVDAT.modification_date,
							record_REVDAT.modification_ID,
							&record_REVDAT.modification_type,
							record_REVDAT.name_of_modified_record[0],
							record_REVDAT.name_of_modified_record[1],
							record_REVDAT.name_of_modified_record[2],
							record_REVDAT.name_of_modified_record[3]);

					 return readRecordREVDAT
						 (record_REVDAT.modification_number,
							record_REVDAT.continuation,
							record_REVDAT.modification_date,
							record_REVDAT.modification_ID,
							record_REVDAT.modification_type,
							record_REVDAT.name_of_modified_record);



			case PDB::RECORD_TYPE__SCALE1:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_SCALE1.record_name,
							&record_SCALE1.transformation_matrix[0],
							&record_SCALE1.transformation_matrix[1],
							&record_SCALE1.transformation_matrix[2],
							&record_SCALE1.transformation_matrix[3]);

					 return readRecordSCALE1
						 (record_SCALE1.transformation_matrix);



			case PDB::RECORD_TYPE__SCALE2:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_SCALE2.record_name,
							&record_SCALE2.transformation_matrix[0],
							&record_SCALE2.transformation_matrix[1],
							&record_SCALE2.transformation_matrix[2],
							&record_SCALE2.transformation_matrix[3]);

					 return readRecordSCALE2
						 (record_SCALE2.transformation_matrix);



			case PDB::RECORD_TYPE__SCALE3:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_SCALE3.record_name,
							&record_SCALE3.transformation_matrix[0],
							&record_SCALE3.transformation_matrix[1],
							&record_SCALE3.transformation_matrix[2],
							&record_SCALE3.transformation_matrix[3]);

					 return readRecordSCALE3
						 (record_SCALE3.transformation_matrix);



			case PDB::RECORD_TYPE__SEQRES:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_SEQRES.record_name,
							&record_SEQRES.serial_number,
							&record_SEQRES.chain_ID,
							&record_SEQRES.number_of_residues_in_chain,
							record_SEQRES.residue_name[0],
							record_SEQRES.residue_name[1],
							record_SEQRES.residue_name[2],
							record_SEQRES.residue_name[3],
							record_SEQRES.residue_name[4],
							record_SEQRES.residue_name[5],
							record_SEQRES.residue_name[6],
							record_SEQRES.residue_name[7],
							record_SEQRES.residue_name[8],
							record_SEQRES.residue_name[9],
							record_SEQRES.residue_name[10],
							record_SEQRES.residue_name[11],
							record_SEQRES.residue_name[12]);

					 return readRecordSEQRES
						 (record_SEQRES.serial_number,
							record_SEQRES.chain_ID,
							record_SEQRES.number_of_residues_in_chain,
							record_SEQRES.residue_name);



			case PDB::RECORD_TYPE__SHEET:

					 parseLine
						 (line,
							size, 
							record_type_format->format_string,
							record_SHEET.record_name,
							&record_SHEET.strand_number,
							record_SHEET.sheet_ID,
							&record_SHEET.number_of_strands,
							record_SHEET.initial_residue.residue_name,
							&record_SHEET.initial_residue.chain_ID,
							&record_SHEET.initial_residue.sequence_number,
							&record_SHEET.initial_residue.insertion_code,
							record_SHEET.terminal_residue.residue_name,
							&record_SHEET.terminal_residue.chain_ID,
							&record_SHEET.terminal_residue.sequence_number,
							&record_SHEET.terminal_residue.insertion_code,
							&record_SHEET.sense_of_strand,
							record_SHEET.atom_name_in_current_strand,
							record_SHEET.residue_in_current_strand.residue_name,
							&record_SHEET.residue_in_current_strand.chain_ID,
							&record_SHEET.residue_in_current_strand.sequence_number,
							&record_SHEET.residue_in_current_strand.insertion_code,
							record_SHEET.atom_name_in_previous_strand,
							record_SHEET.residue_in_previous_strand.residue_name,
							&record_SHEET.residue_in_previous_strand.chain_ID,
							&record_SHEET.residue_in_previous_strand.sequence_number,
							&record_SHEET.residue_in_previous_strand.insertion_code);

							return readRecordSHEET
								(record_SHEET.strand_number,
								 record_SHEET.sheet_ID,
								 record_SHEET.number_of_strands,
								 record_SHEET.initial_residue,
								 record_SHEET.terminal_residue,
								 record_SHEET.sense_of_strand,
								 record_SHEET.atom_name_in_current_strand,
								 record_SHEET.residue_in_current_strand,
								 record_SHEET.atom_name_in_previous_strand,
								 record_SHEET.residue_in_previous_strand);



			case PDB::RECORD_TYPE__SIGATM:

							if ((selected_model_ != 0) && (selected_model_ != current_model_))
							{
								return true;
							}

							parseLine
								(line,
								 size, 
								 record_type_format->format_string,
								 record_SIGATM.record_name,
								 &record_SIGATM.serial_number,
								 record_SIGATM.atom_name,
								 &record_SIGATM.alternate_location_indicator,
								 record_SIGATM.residue_name,
								 &record_SIGATM.chain_ID,
								 &record_SIGATM.residue_sequence_number,
								 &record_SIGATM.insertion_code,
								 &record_SIGATM.standard_vector_deviation[0],
								 &record_SIGATM.standard_vector_deviation[1],
								 &record_SIGATM.standard_vector_deviation[2],
								 &record_SIGATM.standard_occupancy_deviation,
								 &record_SIGATM.standard_temperature_deviation,
								 record_SIGATM.segment_ID,
								 record_SIGATM.element_symbol,
								 record_SIGATM.charge);

							return readRecordSIGATM
								(record_SIGATM.serial_number,
								 record_SIGATM.atom_name,
								 record_SIGATM.alternate_location_indicator,
								 record_SIGATM.residue_name,
								 record_SIGATM.chain_ID,
								 record_SIGATM.residue_sequence_number,
								 record_SIGATM.insertion_code,
								 record_SIGATM.standard_vector_deviation,
								 record_SIGATM.standard_occupancy_deviation,
								 record_SIGATM.standard_temperature_deviation,
								 record_SIGATM.segment_ID,
								 record_SIGATM.element_symbol,
								 record_SIGATM.charge);



			case PDB::RECORD_TYPE__SIGUIJ:

							if ((selected_model_ != 0) && (selected_model_ != current_model_))
							{
								return true;
							}

							parseLine
								(line,
								 size,
								 record_type_format->format_string,
								 record_SIGUIJ.record_name,
								 &record_SIGUIJ.serial_number,
								 record_SIGUIJ.atom_name,
								 &record_SIGUIJ.alternate_location_indicator,
								 record_SIGUIJ.residue_name,
								 &record_SIGUIJ.chain_ID,
								 &record_SIGUIJ.residue_sequence_number,
								 &record_SIGUIJ.insertion_code,
								 &record_SIGUIJ.sig11,
								 &record_SIGUIJ.sig22,
								 &record_SIGUIJ.sig33,
								 &record_SIGUIJ.sig12,
								 &record_SIGUIJ.sig13,
								 &record_SIGUIJ.sig23,
								 record_SIGUIJ.segment_ID,
								 record_SIGUIJ.element_symbol,
								 record_SIGUIJ.charge);

							return readRecordSIGUIJ
								(record_SIGUIJ.serial_number,
								 record_SIGUIJ.atom_name,
								 record_SIGUIJ.alternate_location_indicator,
								 record_SIGUIJ.residue_name,
								 record_SIGUIJ.chain_ID,
								 record_SIGUIJ.residue_sequence_number,
								 record_SIGUIJ.insertion_code,
								 record_SIGUIJ.sig11,
								 record_SIGUIJ.sig22,
								 record_SIGUIJ.sig33,
								 record_SIGUIJ.sig12,
								 record_SIGUIJ.sig13,
								 record_SIGUIJ.sig23,
								 record_SIGUIJ.segment_ID,
								 record_SIGUIJ.element_symbol,
								 record_SIGUIJ.charge);



			case PDB::RECORD_TYPE__SITE:

							parseLine
								(line,
								 size, 
								 record_type_format->format_string,
								 record_SITE.record_name,
								 &record_SITE.sequence_number,
								 record_SITE.name,
								 &record_SITE.number_of_residues,
								 record_SITE.residue[0].residue_name,
								 &record_SITE.residue[0].chain_ID,
								 &record_SITE.residue[0].sequence_number,
								 &record_SITE.residue[0].insertion_code,
								 record_SITE.residue[1].residue_name,
								 &record_SITE.residue[1].chain_ID,
								 &record_SITE.residue[1].sequence_number,
								 &record_SITE.residue[1].insertion_code,
								 record_SITE.residue[2].residue_name,
								 &record_SITE.residue[2].chain_ID,
								 &record_SITE.residue[2].sequence_number,
								 &record_SITE.residue[2].insertion_code,
								 record_SITE.residue[3].residue_name,
								 &record_SITE.residue[3].chain_ID,
								 &record_SITE.residue[3].sequence_number,
								 &record_SITE.residue[3].insertion_code);

								 return readRecordSITE
									 (record_SITE.sequence_number,
										record_SITE.name,
										record_SITE.number_of_residues,
										record_SITE.residue);



			case PDB::RECORD_TYPE__SLTBRG:

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_SLTBRG.record_name,
										record_SLTBRG.partner_atom[0].atom_name,
										&record_SLTBRG.partner_atom[0].alternate_location_indicator,
										record_SLTBRG.partner_atom[0].residue.residue_name,
										&record_SLTBRG.partner_atom[0].residue.chain_ID,
										&record_SLTBRG.partner_atom[0].residue.sequence_number,
										&record_SLTBRG.partner_atom[0].residue.insertion_code,
										record_SLTBRG.partner_atom[1].atom_name,
										&record_SLTBRG.partner_atom[1].alternate_location_indicator,
										record_SLTBRG.partner_atom[1].residue.residue_name,
										&record_SLTBRG.partner_atom[1].residue.chain_ID,
										&record_SLTBRG.partner_atom[1].residue.sequence_number,
										&record_SLTBRG.partner_atom[1].residue.insertion_code,
										&record_SLTBRG.first_atom,
										&record_SLTBRG.second_atom);

								 return readRecordSLTBRG
									 (record_SLTBRG.partner_atom,
										record_SLTBRG.first_atom,
										record_SLTBRG.second_atom);



			case PDB::RECORD_TYPE__SOURCE:

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_SOURCE.record_name,
										&record_SOURCE.continuation,
										record_SOURCE.sources);

								 return readRecordSOURCE
									 (record_SOURCE.continuation,
										record_SOURCE.sources);



			case PDB::RECORD_TYPE__SSBOND:

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_SSBOND.record_name,
										&record_SSBOND.serial_number,
										record_SSBOND.partner_residue[0].residue_name,
										&record_SSBOND.partner_residue[0].chain_ID,
										&record_SSBOND.partner_residue[0].sequence_number,
										&record_SSBOND.partner_residue[0].insertion_code,
										record_SSBOND.partner_residue[1].residue_name,
										&record_SSBOND.partner_residue[1].chain_ID,
										&record_SSBOND.partner_residue[1].sequence_number,
										&record_SSBOND.partner_residue[1].insertion_code,
										&record_SSBOND.partner_residue[0].symmetry_operator,
										&record_SSBOND.partner_residue[1].symmetry_operator);

								 return readRecordSSBOND
									 (record_SSBOND.serial_number,
										record_SSBOND.partner_residue);



			case PDB::RECORD_TYPE__TER:

								 if ((selected_model_ != 0) && (selected_model_ != current_model_))
								 {
									 return true;
								 }

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_TER.record_name,
										&record_TER.serial_number,
										record_TER.residue_name,
										&record_TER.chain_ID,
										&record_TER.residue_sequence_number,
										&record_TER.insertion_code);

								 return readRecordTER
									 (record_TER.serial_number,
										record_TER.residue_name,
										record_TER.chain_ID,
										record_TER.residue_sequence_number,
										record_TER.insertion_code);



			case PDB::RECORD_TYPE__TITLE:

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_TITLE.record_name,
										&record_TITLE.continuation,
										record_TITLE.title);

								 return readRecordTITLE
									 (record_TITLE.continuation,
										record_TITLE.title);



			case PDB::RECORD_TYPE__TURN:

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_TURN.record_name,
										&record_TURN.sequence_number,
										record_TURN.turn_ID,
										record_TURN.initial_residue.residue_name,
										&record_TURN.initial_residue.chain_ID,
										&record_TURN.initial_residue.sequence_number,
										&record_TURN.initial_residue.insertion_code,
										record_TURN.terminal_residue.residue_name,
										&record_TURN.terminal_residue.chain_ID,
										&record_TURN.terminal_residue.sequence_number,
										&record_TURN.terminal_residue.insertion_code,
										record_TURN.comment);

								 return readRecordTURN
									 (record_TURN.sequence_number,
										record_TURN.turn_ID,
										record_TURN.initial_residue,
										record_TURN.terminal_residue,
										record_TURN.comment);



			case PDB::RECORD_TYPE__TVECT:

								 parseLine
									 (line,
										size, 
										record_type_format->format_string,
										record_TVECT.record_name,
										&record_TVECT.serial_number,
										&record_TVECT.translation_vector[0],
										&record_TVECT.translation_vector[1],
										&record_TVECT.translation_vector[2],
										record_TVECT.comment);

								 return readRecordTVECT
									 (record_TVECT.serial_number,
										record_TVECT.translation_vector,
										record_TVECT.comment);



			default:
				
				return readUnknownRecord(line);
		}
	}

	void PDBFile::clear_()
	{
		current_model_ = INVALID_INDEX;
		current_record_ = INVALID_INDEX;
		record_fields_ = 0;
		PDB_atom_map_.destroy();
		residue_map_.destroy();
		ssbond_list_.clear();
		helix_list_.clear();
		sheet_list_.clear();
		turn_list_.clear();
		new_helix_secstruc_list_.clear();
		new_sheet_secstruc_list_.clear();
		new_turn_secstruc_list_.clear();
		current_protein_ = 0;
		current_chain_ = 0;
		current_residue_ = 0;
		current_PDB_atom_ = 0;
		residue_name_ = "";
		chain_ID_ = 0;
		residue_sequence_number_ = -1;
		insertion_code_ = 0;
	}

	void PDBFile::init_()
		throw()
	{
		// set a default for the current record type
		compare_record_type_format_.record_type = PDB::RECORD_TYPE__UNKNOWN;

		// initialize the buffer for record type comparison
		memcpy(const_cast<char*>(compare_record_type_format_.string),
				const_cast<char*>("      "), 7);
		compare_record_type_format_.format_string = "";

		// initialize buffers with zero
		memset(line_buffer_, 0, sizeof(line_buffer_));
		memset(&record_UNKNOWN, 0, sizeof(PDB::RecordUNKNOWN));

		// set defaults
		options.setDefaultInteger(Option::CHOOSE_MODEL, Default::CHOOSE_MODEL);
	}


	PDBFile::PDBFile(const PDBFile& file)
		throw(Exception::FileNotFound)
		:	File(),
			PropertyManager(file),
			options(file.options),
			info(file.info),
			residue_name_(file.residue_name_),
			residue_sequence_number_(file.residue_sequence_number_),
			current_model_(file.current_model_),
			current_record_(file.current_record_),
			record_fields_(file.record_fields_),
			current_record_type_(file.current_record_type_),

			insertion_code_(file.insertion_code_),
			chain_ID_(file.chain_ID_),
			sequence_number_(file.sequence_number_),
			code_(file.code_),
			current_protein_(0),
			current_const_protein_(0),
			current_chain_(0),
			current_const_chain_(0),
			current_residue_(0),
			current_const_residue_(0),
			current_PDB_atom_(0),
			current_const_atom_(0),
			name_(file.name_),
			verbosity_(file.verbosity_),
			strict_line_checking_(file.strict_line_checking_),
			selected_model_(file.selected_model_),
			store_skipped_records_(file.store_skipped_records_),
			ignore_xplor_pseudo_atoms_(file.ignore_xplor_pseudo_atoms_),
			parse_partial_charges_(file.parse_partial_charges_)
	{
		init_();
		setName(file.getName());
	}

	PDBFile::PDBFile(const String& filename, File::OpenMode open_mode)
		throw(Exception::FileNotFound)
		:	File(),
			residue_name_(""),
			residue_sequence_number_(-1),
			current_model_(INVALID_INDEX),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN),
			insertion_code_(0),
			chain_ID_(0),
			sequence_number_(-1),
			code_(),
			current_protein_(0),
			current_const_protein_(0),
			current_chain_(0),
			current_const_chain_(0),
			current_residue_(0),
			current_const_residue_(0),
			current_PDB_atom_(0),
			current_const_atom_(0),
			name_(),
			verbosity_(0),
			strict_line_checking_(false),
			selected_model_(1),
			store_skipped_records_(true),
			ignore_xplor_pseudo_atoms_(true),
			parse_partial_charges_(false)
	{
		open(filename, open_mode);
	}


	PDBFile::~PDBFile()
		throw()
	{
		clear_();
		close();
	}

	bool PDBFile::readRecordATOM
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
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 element_symbol,
		 PDB::LString2 charge)
	{
		if (chain_ID != chain_ID_)
		{
			current_chain_ = new Chain();
			current_protein_->insert(*current_chain_);
			
			residue_name_ = "";
			chain_ID_ = chain_ID;
			
			current_chain_->setName(chain_ID_);
		}
		
		if (residue_name_ != (const char *)residue_name
				|| residue_sequence_number != residue_sequence_number_
				|| insertion_code != insertion_code_)
		{
			ResidueQuadruple unique_residue(residue_name, chain_ID, residue_sequence_number, insertion_code);

			current_residue_ = new Residue;
			current_chain_->insert(*current_residue_);
			residue_map_[unique_residue] = current_residue_;
			
			residue_name_ = residue_name;
			residue_sequence_number_ = residue_sequence_number;
			insertion_code_ = insertion_code;
			
			if (current_residue_->hasProperty(Residue::PROPERTY__NON_STANDARD) == false)
			{
				current_residue_->setProperty(Residue::PROPERTY__AMINO_ACID);
			}
			String trimmed_name = residue_name_;
			current_residue_->setName(trimmed_name.trim());
			current_residue_->setID(residue_sequence_number_);
			current_residue_->setInsertionCode(insertion_code_);
		}

		// make sure we read only the first location if alternate
		// locations are present to avoid invalid structures due
		// to duplicate atoms
		if ((alternate_location_indicator == ' ' )
				|| (alternate_location_indicator == 'A'))
		{
			current_PDB_atom_ = new PDBAtom;
			current_residue_->insert(*current_PDB_atom_);
			PDB_atom_map_[serial_number] = current_PDB_atom_;
			
			current_PDB_atom_->setName(getAtomName(atom_name));
			current_PDB_atom_->setRemotenessIndicator(getAtomRemotenessIndicator(atom_name));
			current_PDB_atom_->setBranchDesignator(getAtomBranchDesignator(atom_name));
			current_PDB_atom_->setAlternateLocationIndicator(alternate_location_indicator);
			current_PDB_atom_->setOccupancy(occupancy);
			current_PDB_atom_->setTemperatureFactor(temperature_factor);
			current_PDB_atom_->setRadius(current_PDB_atom_->getElement().getVanDerWaalsRadius());
			current_PDB_atom_->setPosition(Vector3(orthogonal_vector[0], orthogonal_vector[1], orthogonal_vector[2]));
			// Figuring out the element is nto entirely trivial: it *should* be in columns 77-78.
			// However, some codes abuse cols. 77-80 for partial charges. We support this format w/ the 
			// option PARTIAL_CHARGES for reading and writing.
			if (parse_partial_charges_)
			{
				// ????
			}
			else
			{
				current_PDB_atom_->setElement(PTE[PDBFile::getAtomElementSymbol(atom_name, element_symbol)]);
				try	
				{
					current_PDB_atom_->setFormalCharge(String(charge).toInt());
				}
				catch (Exception::InvalidFormat)
				{
				}
				current_PDB_atom_->setCharge(current_PDB_atom_->getFormalCharge());
			}
		}
		
		return true;
	}
		
	bool PDBFile::readRecordCONECT
		(PDB::Integer atom_serial_number,
		 PDB::Integer bonded_atom_serial_number[4],
		 PDB::Integer hydrogen_bonded_atom_serial_number[4],
		 PDB::Integer salt_bridged_atom_serial_number[2])
	{
		PDBAtomMap::Iterator	atom_map_it = PDB_atom_map_.find(atom_serial_number);

		PDBAtom*				PDB_atom = 0;
		Bond*						bond = 0;
		unsigned short	i = 0;

		// extract a pointer to the atom, if it was found in the hash map
		if (atom_map_it == PDB_atom_map_.end())
		{
			return false;  // illegal atom number, abort
		}
		else
		{
			PDB_atom = atom_map_it->second; // retrieve the pointer
		}
		
		// read the entries for the bonds
		for (i = 0; i < 4; ++i)
		{
			if (bonded_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(bonded_atom_serial_number[i]);
				
				// retrieve the second atom via the hash table
				if (atom_map_it != PDB_atom_map_.end())
				{
					// create the new bond
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__COVALENT);
						bond->setOrder(Bond::ORDER__SINGLE);
					}
				}
			}
		}
				
		for (i = 0; i < 4; ++i)
		{
			if (hydrogen_bonded_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(hydrogen_bonded_atom_serial_number[i]);
				
				if (atom_map_it != PDB_atom_map_.end())
				{
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__HYDROGEN);
						bond->setOrder(Bond::ORDER__SINGLE);
					}
				}
			}
		}
		
		for (i = 0; i < 2; ++i)
		{
			if (salt_bridged_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(salt_bridged_atom_serial_number[i]);
				
				if (atom_map_it != PDB_atom_map_.end())
				{
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__SALT_BRIDGE);
						bond->setOrder(Bond::ORDER__SINGLE);
					}
				}
			}
		}
		
		return true;
	}

	bool PDBFile::readRecordHEADER
		(PDB::String40 classification,
		 PDB::Date /* deposition_date */,
		 PDB::IDcode ID_code)
	{
		current_protein_->setName(classification);
		current_protein_->setID(ID_code);

		return true;
	}
		
	bool PDBFile::readRecordHELIX
		(PDB::Integer /* serial_number */,
		 PDB::LString3 helix_ID,
		 PDB::RecordHELIX::InitialResidue& initial_residue,
		 PDB::RecordHELIX::TerminalResidue& terminal_residue,
		 PDB::Integer helix_class,
		 PDB::PDBString /* comment */,
		 PDB::Integer /* length */)
	{
		ResidueQuadruple partner_residue
			(initial_residue.residue_name, initial_residue.chain_ID,
			 initial_residue.sequence_number, initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(helix_ID);
		
		new_helix_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::HELIX);
		sec_struc->setProperty("HELIX_CLASS", (unsigned int)helix_class);
		
		helix_list_.push_back(partner_residue);
		
		partner_residue.set(terminal_residue.residue_name, terminal_residue.chain_ID,
												terminal_residue.sequence_number, terminal_residue.insertion_code);
		
		helix_list_.push_back(partner_residue);
		
		return true;
	}

	bool PDBFile::readRecordHETATM
		(PDB::Integer serial_number,
		 PDB::Atom atom_name,
		 PDB::Character alternate_location_indicator,
		 PDB::ResidueName residue_name,
		 PDB::Character chain_ID,
		 PDB::Integer residue_sequence_number,
		 PDB::AChar insertion_code,
		 PDB::Real orthogonal_vector[3],
		 PDB::Real occupancy,
		 PDB::Real temperature,
		 PDB::LString4 segment_ID,
		 PDB::LString2 element_symbol,
		 PDB::LString2 charge)
	{
		readRecordATOM(serial_number, atom_name, alternate_location_indicator, residue_name,
									 chain_ID, residue_sequence_number, insertion_code, orthogonal_vector,
									 occupancy, temperature, segment_ID, element_symbol, charge); 
		
		current_residue_->clearProperty(Residue::PROPERTY__AMINO_ACID);
		current_residue_->setProperty(Residue::PROPERTY__NON_STANDARD);
		
		RegularExpression regular_expression("^OHH|HOH|HHO|H2O|2HO|OH2|SOL|TIP|TIP2|TIP3|TIP4|WAT|D2O$");
		if (regular_expression.match(current_residue_->getName()) == true)
		{
			current_residue_->setProperty(Residue::PROPERTY__WATER);
		}
		
		return true;
	}

	bool PDBFile::readRecordSHEET
		(PDB::Integer /* strand_number */,
		 PDB::LString3 sheet_ID,
		 PDB::Integer /* number_of_strands */,
		 PDB::RecordSHEET::InitialResidue& initial_residue,
		 PDB::RecordSHEET::TerminalResidue& terminal_residue,
		 PDB::Integer sense_of_strand,
		 PDB::Atom /* atom_name_in_current_strand */,
		 PDB::RecordSHEET::ResidueInCurrentStrand & /* residue_in_current_strand */,
		 PDB::Atom /* atom_name_in_previous_strand */,
		 PDB::RecordSHEET::ResidueInPreviousStrand & /* residue_in_previous_strand */)
	{
		ResidueQuadruple partner_residue
			(initial_residue.residue_name, initial_residue.chain_ID,
			 initial_residue.sequence_number, initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(sheet_ID);
		
		new_sheet_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::STRAND);
		sec_struc->setProperty("STRAND_SENSE", (sense_of_strand != 0));
		
		sheet_list_.push_back(partner_residue);
		
		partner_residue.set(terminal_residue.residue_name, terminal_residue.chain_ID,
												terminal_residue.sequence_number, terminal_residue.insertion_code);
		
		sheet_list_.push_back(partner_residue);
		
		return true;
	}

	bool PDBFile::readRecordSSBOND
		(PDB::Integer /* serial_number */,
		 PDB::RecordSSBOND::PartnerResidue partner_residue[2])
	{
		ResidueQuadruple partner(partner_residue[0].residue_name, partner_residue[0].chain_ID,
														 partner_residue[0].sequence_number, partner_residue[0].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		partner.set(partner_residue[1].residue_name, partner_residue[1].chain_ID,
								partner_residue[1].sequence_number,	partner_residue[1].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		return true;
	}

	bool PDBFile::readRecordTER
		(PDB::Integer /* serial_number */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */)
	{
		chain_ID_ = 0;
		
		return true;
	}

	bool PDBFile::readRecordTURN
		(PDB::Integer /* sequence_number */,
		 PDB::LString3 turn_ID,
		 PDB::RecordTURN::InitialResidue &initial_residue,
		 PDB::RecordTURN::TerminalResidue &terminal_residue,
		 PDB::PDBString /* comment */)
	{
		ResidueQuadruple partner_residue(initial_residue.residue_name, initial_residue.chain_ID,
																		 initial_residue.sequence_number,	initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(turn_ID);
		
		new_turn_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::TURN);
		
		turn_list_.push_back(partner_residue);
		
		partner_residue.set(terminal_residue.residue_name, terminal_residue.chain_ID,
												terminal_residue.sequence_number, terminal_residue.insertion_code);
		
		turn_list_.push_back(partner_residue);
		
		return true;
	}

	void PDBFile::read(Protein& protein)
	{
		clear_();
		protein.destroy();
		current_protein_ = &protein;
		readRecords();
		postprocessSSBonds_();
		postprocessHelices_();
		postprocessSheetsTurns_(sheet_list_, new_sheet_secstruc_list_);
		postprocessSheetsTurns_(turn_list_, new_turn_secstruc_list_);
		postprocessRandomCoils_();
	}

	void PDBFile::read(System& system)
	{
		Protein* protein = new Protein();
		read(*protein);
		system.insert(*protein);
	}

	bool PDBFile::write(const System& system)
		throw(File::CannotWrite)
	{
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}

		Size number_of_proteins = system.count(RTTI::getDefault<KernelPredicate<Protein> >());
		if (number_of_proteins > 1)
		{
			Log.error() << "PDBFile::write(): cannot write a system with multiple proteins to a PDB file." << endl;
			return false;
		}
		
		if (number_of_proteins == 0)
		{
			if (system.countMolecules() != 1)
			{
				Log.error() << "PDBFile::write(System): "
										<< "Cannot write empty/multiple molecules to a PDB file." << endl;
				return false;
			}
			else
			{
				// this loop is for future releases which are able to write
				// multiple molecules as different models or chains.
				MoleculeConstIterator it = system.beginMolecule();
				for (; +it; ++it)
				{
					write_(*it);
				}
			}
		} 
		else 
		{
			Composite::CompositeConstIterator it = system.beginComposite();
			for (; +it && !RTTI::isKindOf<Protein>(*it); ++it);
			if (+it)
			{
				write_(*it, true);
			} 
			else 
			{
				Log.error() << "PDBFile::write: cannot find a protein in the current system." << endl;
				return false;
			}
		}

		return true;
	}
	
	void PDBFile::write_(const Composite& composite, bool system)
	{
		const Protein* protein = RTTI::castTo<Protein>(composite);
		const Molecule& molecule = *RTTI::castTo<Molecule>(composite);
  
		char line_buffer[PDB::SIZE_OF_PDB_LINE_BUFFER];
		ChainConstIterator chain_it;
		SecondaryStructureConstIterator sec_struc_it;
		SecondaryStructureConstIterator sheet_it;
		ResidueConstIterator residue_it;
		ResidueConstIterator reverse_residue_it;
		AtomConstIterator atom_iterator;
		AtomConstIterator atom_it;
		Atom::BondConstIterator bond_it;
		unsigned long secstruc_serial_number = 0;
		unsigned long residue_sequence_number = 0;
		char chain_name = BALL_CHAIN_DEFAULT_NAME;
		unsigned long atom_serial_number = 0;
		unsigned long ssbond_serial_number = 0;
		char PDB_code[5] = { 0, 0, 0, 0, 0 };
		char PDB_secstruc_name[4] = { 0, 0, 0, 0 };
		char PDB_residue_name[13][4] = { "", "", "", "", "", "", "", "", "", "", "", "", "" };
		char PDB_atom_name[5] = { 0, 0, 0, 0, 0 };
		char element_symbol[3] = { 0, 0, 0 };
		unsigned long number_of_residues_in_chain = 0;
		unsigned long number_of_remark_records = 0;
		unsigned long number_of_het_records = 0;
		unsigned long number_of_helix_records = 0;
		unsigned long number_of_sheet_records = 0;
		unsigned long number_of_turn_records = 0;
		unsigned long number_of_site_records = 0;
		unsigned long number_of_coordinate_transformation_records = 0;
		const Fragment *current_fragment = 0;
		unsigned long number_of_atomic_coordinate_records = 0;
		unsigned long number_of_ter_records = 0;
		unsigned long number_of_conect_records = 0;
		unsigned long number_of_seqres_records = 0;
		PDB::RecordType record_type = PDB::RECORD_TYPE__UNKNOWN;
		vector<const Residue*> residue(13);
		const SecondaryStructure*		current_sec_struc = 0;
		const Atom*									current_atom = 0;
		PDB::Integer					length_of_secstruc = 0;
		String								temp_string;
		HashMap<void*, PDB::Integer> atom_map;
		vector<const Atom*> covalent_bonded_atom(4);
		vector<const Atom*> hydrogen_bonded_atom(4);
		vector<const Atom*> saltbridge_bonded_atom(2);
		unsigned short covalent_bond = 0;
		unsigned short hydrogen_bond = 0;
		unsigned short saltbridge_bond = 0;

		// --- HEADER ---
		
		/* prepare the current date ... */
		time_t current_time = time(NULL);
		char* date = asctime(localtime(&current_time));
		char PDB_date_format[10];
		
		sprintf(PDB_date_format, "%2.2s-%c%c%c-%2.2s",
						date + 8, toupper(date[4]), toupper(date[5]),
						toupper(date[6]), date + 22);

		if (protein != 0)
		{
			protein->getID().get(PDB_code, 0, 5);
		}
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__HEADER].format_string,
						record_type_format_[PDB::RECORD_TYPE__HEADER].string,
						molecule.getName().c_str(),
						PDB_date_format,
						PDB_code);

		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;

		// --- SEQRES ---

		if (protein != 0)
		{
			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_const_chain_ = &(*chain_it);
				number_of_residues_in_chain = current_const_chain_->countResidues();
				residue_sequence_number = 0;
				
				BALL_FOREACH_RESIDUE((*chain_it), residue_it)
				{
					current_const_residue_ = &(*residue_it);

					if (current_const_residue_->hasProperty(Residue::PROPERTY__NON_STANDARD) == true)
					{
						continue;
					}
				
					residue[residue_sequence_number] = current_const_residue_;
					residue[residue_sequence_number]->getName().get(PDB_residue_name[residue_sequence_number], 0, 4);
			
					if (residue_sequence_number++ == 12)
					{
						chain_name = current_const_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}

		    
						sprintf(line_buffer, 
										record_type_format_[PDB::RECORD_TYPE__SEQRES].format_string,
										record_type_format_[PDB::RECORD_TYPE__SEQRES].string,
										++number_of_seqres_records,
										chain_name,
										number_of_residues_in_chain,
										PDB_residue_name[0],
										PDB_residue_name[1],
										PDB_residue_name[2],
										PDB_residue_name[3],
										PDB_residue_name[4],
										PDB_residue_name[5],
										PDB_residue_name[6],
										PDB_residue_name[7],
										PDB_residue_name[8],
										PDB_residue_name[9],
										PDB_residue_name[10],
										PDB_residue_name[11],
										PDB_residue_name[12]);
	
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;

						residue_sequence_number = 0;

						PDB_residue_name[0][0]
							= PDB_residue_name[1][0]
							= PDB_residue_name[2][0]
							= PDB_residue_name[3][0]
							= PDB_residue_name[4][0]
							= PDB_residue_name[5][0]
							= PDB_residue_name[6][0]
							= PDB_residue_name[7][0]
							= PDB_residue_name[8][0]
							= PDB_residue_name[9][0]
							= PDB_residue_name[10][0]
							= PDB_residue_name[11][0]
							= PDB_residue_name[12][0]
							= '\0';
					}
				}

				if (residue_sequence_number > 0)
				{
					chain_name = current_const_chain_->getName().c_str()[0];
					if (chain_name == 0)
					{
						chain_name = BALL_CHAIN_DEFAULT_NAME;
					}

					sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__SEQRES].format_string,
						record_type_format_[PDB::RECORD_TYPE__SEQRES].string,
						++number_of_seqres_records,
						chain_name,
						number_of_residues_in_chain,
						PDB_residue_name[0],
						PDB_residue_name[1],
						PDB_residue_name[2],
						PDB_residue_name[3],
						PDB_residue_name[4],
						PDB_residue_name[5],
						PDB_residue_name[6],
						PDB_residue_name[7],
						PDB_residue_name[8],
						PDB_residue_name[9],
						PDB_residue_name[10],
						PDB_residue_name[11],
						PDB_residue_name[12]);
			
					line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
					File::getFileStream() << line_buffer << endl;
				}
			}
		

			// --- HELIX ---

			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_const_chain_ = &(*chain_it);
				
				BALL_FOREACH_SECONDARYSTRUCTURE(*chain_it, sec_struc_it)
				{
					current_sec_struc = &(*sec_struc_it);
					
					if (current_sec_struc->getType() == SecondaryStructure::HELIX)
					{
						residue_it = current_sec_struc->beginResidue();
						if (residue_it.isEnd())
						{
							continue;
						}
						reverse_residue_it = --current_sec_struc->endResidue();

						current_sec_struc->getName().get(PDB_secstruc_name, 0, 4);

						residue[0] = &(*residue_it);
						residue[1] = &(*reverse_residue_it);
						residue[0]->getName().get(PDB_residue_name[0], 0, 4);
						residue[1]->getName().get(PDB_residue_name[1], 0, 4);

						for (length_of_secstruc = 1; 
								 !residue_it.isEnd() && residue_it != reverse_residue_it;
								 ++length_of_secstruc, ++residue_it);
			
						chain_name = current_const_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}
			
						sprintf(line_buffer, 
										record_type_format_[PDB::RECORD_TYPE__HELIX].format_string,
										record_type_format_[PDB::RECORD_TYPE__HELIX].string,
										++number_of_helix_records,
										PDB_secstruc_name,
										PDB_residue_name[0],
										chain_name,
										residue[0]->getID().toLong(),
										residue[0]->getInsertionCode(),
										PDB_residue_name[1],
										chain_name,
										residue[1]->getID().toLong(),
										residue[1]->getInsertionCode(),
										(unsigned long)current_sec_struc->getProperty("HELIX_CLASS").getUnsignedInt(),
										"", // comment not supported, yet
										length_of_secstruc);
					
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;
					}
				}
			}
		

			// --- SHEET ---

			temp_string = "12345";
			
			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_const_chain_ = &(*chain_it);
				
				BALL_FOREACH_SECONDARYSTRUCTURE(*chain_it, sec_struc_it)
				{
					current_sec_struc = &(*sec_struc_it);

					if (current_sec_struc->getType() == SecondaryStructure::STRAND)
					{
						residue_it = current_sec_struc->beginResidue();
						if (residue_it.isEnd())
						{
							continue;
						}
						reverse_residue_it = --current_sec_struc->endResidue();

						++number_of_sheet_records;
		
						if (temp_string != current_sec_struc->getName())
						{
							secstruc_serial_number = 0;
							temp_string = current_sec_struc->getName();

							for (length_of_secstruc = 0, sheet_it = sec_struc_it;
									 !sheet_it.isEnd(); ++sheet_it)
							{
								if ((sheet_it->getType() == SecondaryStructure::STRAND)
										&& (sheet_it->getName() == temp_string))
								{
									++length_of_secstruc;
								}
							}
						}
			
						current_sec_struc->getName().get(PDB_secstruc_name, 0, 4);

						residue[0] = &(*residue_it);
						residue[1] = &(*reverse_residue_it);
						residue[0]->getName().get(PDB_residue_name[0], 0, 4);
						residue[1]->getName().get(PDB_residue_name[1], 0, 4);

						chain_name = current_const_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{	
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}
						
						sprintf(line_buffer, 
							record_type_format_[PDB::RECORD_TYPE__SHEET].format_string,
							record_type_format_[PDB::RECORD_TYPE__SHEET].string,
							++secstruc_serial_number,
							PDB_secstruc_name,
							length_of_secstruc,
							PDB_residue_name[0],
							chain_name,
							residue[0]->getID().toLong(),
							residue[0]->getInsertionCode(),
							PDB_residue_name[1],
							chain_name,
							residue[1]->getID().toLong(),
							residue[1]->getInsertionCode(),
							(long)current_sec_struc->getProperty("STRAND_SENSE").getUnsignedInt(),
							"",  // Registration. Atom name in current strand not supported, yet
							"",  // Registration. Residue name in current strand not supported, yet
							' ', // Registration. Chain identifier in current strand not supported, yet
							0L,   // Registration. Residue sequence number in current strand not supported, yet
							' ', // Registration. Insertion code in current strand not supported, yet
							"",  // Registration. Atom name in previous strand not supported, yet
							"",  // Registration. Residue name in previous strand not supported, yet
							' ', // Registration. Chain identifier in previous strand not supported, yet
							0L,   // Registration. Residue sequence number in previous strand not supported, yet
							' ');// Registration. Insertion code in previous strand not supported, yet
							
							
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;
					}
				}
			}

			// --- TURN ---

			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_const_chain_ = &(*chain_it);
				
				BALL_FOREACH_SECONDARYSTRUCTURE(*chain_it, sec_struc_it)
				{
					current_sec_struc = &(*sec_struc_it);
					
					if (current_sec_struc->getType() == SecondaryStructure::TURN)
					{
						residue_it = current_sec_struc->beginResidue();
						if (residue_it.isEnd())
						{
							continue;
						}
						reverse_residue_it = --current_sec_struc->endResidue();

						current_sec_struc->getName().get(PDB_secstruc_name, 0, 4);

						residue[0] = &(*residue_it);
						residue[1] = &(*reverse_residue_it);
						residue[0]->getName().get(PDB_residue_name[0], 0, 4);
						residue[1]->getName().get(PDB_residue_name[1], 0, 4);

						for (length_of_secstruc = 1;
								 !residue_it.isEnd() && residue_it != reverse_residue_it;
								 ++length_of_secstruc, ++residue_it)
						{
						}
						
						chain_name = current_const_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}

						
						sprintf(line_buffer, 
							record_type_format_[PDB::RECORD_TYPE__TURN].format_string,
							record_type_format_[PDB::RECORD_TYPE__TURN].string,
							++number_of_turn_records,
							PDB_secstruc_name,
							PDB_residue_name[0],
							chain_name,
							residue[0]->getID().toLong(),
							residue[0]->getInsertionCode(),
							PDB_residue_name[1],
							chain_name,
							residue[1]->getID().toLong(),
							residue[1]->getInsertionCode(),
							""); // comment not supported, yet
								
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;
					}
				}
			}

			// --- SSBOND ---

			ssbond_serial_number = 0;

			BALL_FOREACH_BOND(*protein, atom_it, bond_it)
			{
				residue[0] = bond_it->getFirstAtom()->getResidue();
				residue[1] = bond_it->getSecondAtom()->getResidue();

				if ((bond_it->getFirstAtom()->getElement() != PTE[Element::S])
						|| (bond_it->getSecondAtom()->getElement() != PTE[Element::S])
						|| (residue[0] == 0) || (residue[1] == 0)
						|| (residue[0] == residue[1])
						|| (residue[0]->hasProperty(Residue::PROPERTY__HAS_SSBOND) == false)
						|| (residue[1]->hasProperty(Residue::PROPERTY__HAS_SSBOND) == false))
				{
					continue;
				}
				
				residue[0]->getName().get(PDB_residue_name[0], 0, 4);
				residue[1]->getName().get(PDB_residue_name[1], 0, 4);

				if (residue[0]->getChain() != 0)
				{
					chain_name = residue[0]->getChain()->getName().c_str()[0];
					if (chain_name == (char)0)
					{
						chain_name = BALL_CHAIN_DEFAULT_NAME;
					}
				} 
				else 
				{
					chain_name = BALL_CHAIN_DEFAULT_NAME;
				}
				char chain_name_B;
				if (residue[1]->getChain() != 0)
				{
					chain_name_B = residue[1]->getChain()->getName().c_str()[0];
					if (chain_name_B == (char)0)
					{
						chain_name_B = BALL_CHAIN_DEFAULT_NAME;
					}
				} 
				else 
				{
					chain_name_B = BALL_CHAIN_DEFAULT_NAME;
				}

				sprintf(line_buffer, 
					record_type_format_[PDB::RECORD_TYPE__SSBOND].format_string,
					record_type_format_[PDB::RECORD_TYPE__SSBOND].string,
					++ssbond_serial_number,
					PDB_residue_name[0],
					chain_name,
					residue[0]->getID().toLong(),
					residue[0]->getInsertionCode(),
					PDB_residue_name[1],
					chain_name_B,
					residue[1]->getID().toLong(),
					residue[1]->getInsertionCode(),
					0L,  // Symmetry operator for 1st residue not supported, yet
					0L); // Symmetry operator for 2nd residue not supported, yet
					
				line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
				File::getFileStream() << line_buffer << endl;
			}
			
			// --- HYDBND ---

			// --- SLTBRG ---

		}

		// --- MODEL ---
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__MODEL].format_string,
						record_type_format_[PDB::RECORD_TYPE__MODEL].string,
						1L);
				
		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;

		// --- ATOM/HETATM ---

		atom_serial_number = 0;
		
		if (protein != 0)
		{
			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_const_chain_ = &(*chain_it);
				current_const_residue_ = 0;
					
				BALL_FOREACH_RESIDUE((*chain_it), residue_it)
				{
					current_const_residue_ = &(*residue_it);
					current_const_residue_->getName().get(PDB_residue_name[0], 0, 4);

					if (current_const_residue_->hasProperty(Residue::PROPERTY__AMINO_ACID) == true)
					{
						record_type = PDB::RECORD_TYPE__ATOM;
					}	
					else 
					{
						record_type = PDB::RECORD_TYPE__HETATM;
					}

					BALL_FOREACH_ATOM(*current_const_residue_, atom_iterator)
					{
						++number_of_atomic_coordinate_records;
						
						current_const_atom_ = &(*atom_iterator);

						// get the element symbol
						strcpy(element_symbol, current_const_atom_->getElement().getSymbol().c_str());

						// normalize the atom name:
						//  if the atom name starts with the element name and the element
						//	name is a single character (e.g. C, N, O, H and the name is not
						//  prefixed by a number) then the name should pe prefixed by
						//  a blank to distinguish CA (carbon alpha) from CA (calcium)
						String name = current_const_atom_->getName();
						name.trim();
						Index offset;
						if ((name.size() < 4) && name.hasPrefix(element_symbol) && (strlen(element_symbol) == 1))
						{
							offset = 1;
							PDB_atom_name[0] = ' ';
						} 
						else 
						{
							offset = 0;
						}

						name.get(&(PDB_atom_name[offset]), 0, 4 - offset);
							
						chain_name = current_const_chain_->getName().c_str()[0];
						if (chain_name == (char)0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}

						// define a few defaults for the case that
						// we don't have a PDBAtom, but only an Atom
						char alternate_location = ' ';
						float occupancy = 1.0;
						float temperature_factor = 20.0;
						if (RTTI::isKindOf<const PDBAtom>(*current_const_atom_))
						{
							const PDBAtom& pdb_atom = *dynamic_cast<const PDBAtom*>(current_const_atom_);
							alternate_location = pdb_atom.getAlternateLocationIndicator();
							occupancy = pdb_atom.getOccupancy();
							temperature_factor = pdb_atom.getTemperatureFactor();
						}
												
						sprintf(line_buffer, 
							record_type_format_[record_type].format_string,
							record_type_format_[record_type].string,
							++atom_serial_number,
							PDB_atom_name,
							alternate_location,
							PDB_residue_name[0],
							chain_name,
							current_const_residue_->getID().toLong(),
							current_const_residue_->getInsertionCode(),
							current_const_atom_->getPosition().x,
							current_const_atom_->getPosition().y,
							current_const_atom_->getPosition().z,
							occupancy,
							temperature_factor,
							PDB_code,
							element_symbol,
							""); // CHARGE NOT YET SUPPORTED
								
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;

						atom_map[(void *)current_const_atom_] =  (long)atom_serial_number;
					}
				}
			

				// --- TER ---

				if (current_const_residue_ != 0)
				{
					++number_of_ter_records;
					
					chain_name = current_const_chain_->getName().c_str()[0];

					if (chain_name == 0)
					{
						chain_name = BALL_CHAIN_DEFAULT_NAME;
					}

					sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__TER].format_string,
						record_type_format_[PDB::RECORD_TYPE__TER].string,
						++atom_serial_number,
						PDB_residue_name[0],
						chain_name,
						current_const_residue_->getID().toLong(),
						current_const_residue_->getInsertionCode());
					
					line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
					File::getFileStream() << line_buffer << endl;
				}

				// ---
				
				if (system == true)
				{
					AtomContainerConstIterator frag_it;
		
					BALL_FOREACH_ATOMCONTAINER(molecule, frag_it)
					{	
						if (RTTI::isKindOf<Molecule>(*frag_it) == false
								|| RTTI::isKindOf<Protein>(*frag_it) == true)
						{
							continue;
						}

						// invariant: *frag_it is molecule
						BALL_FOREACH_ATOM(*frag_it, atom_it)
						{
							++number_of_atomic_coordinate_records;
						
							current_atom = &(*atom_it);
							current_atom->getName().get(PDB_atom_name, 0, 3);
							PDB_atom_name[2] = BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR;
							PDB_atom_name[3] = BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR;
							PDB_atom_name[4] = '\0';
							strcpy(element_symbol, current_atom->getElement().getSymbol().c_str());
						
							current_fragment = current_atom->getAncestor(RTTI::getDefault<Fragment>());
							if (current_fragment != 0)
							{
								current_fragment->getName().get(PDB_residue_name[0], 0, 3);
							} 
							else 
							{
								PDB_residue_name[0][0] = '\0';
							}

							sprintf(line_buffer, 
									record_type_format_[PDB::RECORD_TYPE__HETATM].format_string,
									record_type_format_[PDB::RECORD_TYPE__HETATM].string,
									++atom_serial_number,
									PDB_atom_name,
									BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR,
									PDB_residue_name[0],
									BALL_CHAIN_DEFAULT_NAME,
									0L,
									BALL_RESIDUE_DEFAULT_INSERTION_CODE,
									current_atom->getPosition().x,
									current_atom->getPosition().y,
									current_atom->getPosition().z,
									1.0f,
									20.0f,
									"",
									"",
									""); // CHARGE NOT YET SUPPORTED
								
							line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
							File::getFileStream() << line_buffer << endl;
								
							atom_map[(void *)current_atom] = (long)atom_serial_number;
						}
					}
				
					/** unnecessary TER record!
					// --- TER ---

					++number_of_ter_records;
							
					sprintf(line_buffer, 
									record_type_format_[PDB::RECORD_TYPE__TER].format_string,
									record_type_format_[PDB::RECORD_TYPE__TER].string,
									++atom_serial_number,
									"",
									BALL_CHAIN_DEFAULT_NAME,
									0L,
									' ');
							
					line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
					File::getFileStream() << line_buffer << endl;
					*/
				}
			}
		} 
		else 
		{
			BALL_FOREACH_ATOM(molecule, atom_it)
			{
				++number_of_atomic_coordinate_records;

				current_atom = &(*atom_it);
				current_atom->getName().get(PDB_atom_name, 0, 3);
				PDB_atom_name[2] = BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR;
				PDB_atom_name[3] = BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR;
				PDB_atom_name[4] = '\0';
				strcpy(element_symbol, current_atom->getElement().getSymbol().c_str());

				current_fragment = current_atom->getAncestor(RTTI::getDefault<Fragment>());
				if (current_fragment != 0)
				{
					current_fragment->getName().get(PDB_residue_name[0], 0, 4);
				} 
				else 
				{
					PDB_residue_name[0][0] = '\0';
				}

				sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__HETATM].format_string,
						record_type_format_[PDB::RECORD_TYPE__HETATM].string,
						++atom_serial_number,
						PDB_atom_name,
						BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR,
						PDB_residue_name[0],
						BALL_CHAIN_DEFAULT_NAME,
						0L,
						BALL_RESIDUE_DEFAULT_INSERTION_CODE,
						current_atom->getPosition().x,
						current_atom->getPosition().y,
						current_atom->getPosition().z,
						1.0f,
						20.0f,
						"",
						"",
						""); // CHARGE NOT YET SUPPORTED

				line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
				File::getFileStream() << line_buffer << endl;

				atom_map[(void *)current_atom] = (long)atom_serial_number;
			}
  }

  // --- ENDMDL ---
  
  sprintf(line_buffer, 
					record_type_format_[PDB::RECORD_TYPE__ENDMDL].format_string,
					record_type_format_[PDB::RECORD_TYPE__ENDMDL].string);
				
	line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
	File::getFileStream() << line_buffer << endl;

	// --- CONECT ---

  for (atom_it = molecule.beginAtom(); 
			 !atom_it.isEnd(); ++atom_it)
	{
		current_atom = &(*atom_it);

		covalent_bond
			= hydrogen_bond
			= saltbridge_bond
			= 0;

		covalent_bonded_atom[0]
			= covalent_bonded_atom[1]
			= covalent_bonded_atom[2]
			= covalent_bonded_atom[3]
			= 0;
				
		hydrogen_bonded_atom[0]
			= hydrogen_bonded_atom[1]
			= hydrogen_bonded_atom[2]
			= hydrogen_bonded_atom[3]
			= 0;
				
		saltbridge_bonded_atom[0]
			= saltbridge_bonded_atom[1]
			= 0;

		for (bond_it = current_atom->beginBond();
				 !bond_it.isEnd(); ++bond_it)
		{
			switch ((*bond_it).getType())
			{
				case Bond::TYPE__HYDROGEN:

					if (hydrogen_bond < 4)
					{
						hydrogen_bonded_atom[hydrogen_bond]
							= (*bond_it).getBoundAtom(*current_atom);

						if (*hydrogen_bonded_atom[hydrogen_bond] >= *current_atom)
						{
							hydrogen_bonded_atom[hydrogen_bond] = 0;
						} 
						else 
						{
							++hydrogen_bond;
						}
					}
					
					break;
			
				case Bond::TYPE__SALT_BRIDGE:

					if (saltbridge_bond < 2)
					{
						saltbridge_bonded_atom[saltbridge_bond]
							= (*bond_it).getBoundAtom(*current_atom);

						if (*saltbridge_bonded_atom[saltbridge_bond] >= *current_atom)
						{
							saltbridge_bonded_atom[saltbridge_bond] = 0;
						} 
						else 
						{
							++saltbridge_bond;
						}
					}
					
					break;


				default:

					if (covalent_bond < 4)
					{
						covalent_bonded_atom[covalent_bond]
							= (*bond_it).getBoundAtom(*current_atom);

						if (*covalent_bonded_atom[covalent_bond] >= *current_atom)
						{
							covalent_bonded_atom[covalent_bond] = 0;
						} 
						else 
						{
							++covalent_bond;
						}
					}
			}
		}

		if (covalent_bond == 0 && hydrogen_bond == 0 && saltbridge_bond == 0)
		{
			continue;
		}

		++number_of_conect_records;

		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__CONECT].format_string,
						record_type_format_[PDB::RECORD_TYPE__CONECT].string,
						((*atom_map.find((void *)current_atom)).second),
						(covalent_bonded_atom[0] != 0) ? ((*atom_map.find((void*)covalent_bonded_atom[0])).second) : 0,
						(covalent_bonded_atom[1] != 0) ? ((*atom_map.find((void*)covalent_bonded_atom[1])).second) : 0,
						(covalent_bonded_atom[2] != 0) ? ((*atom_map.find((void*)covalent_bonded_atom[2])).second) : 0,
						(covalent_bonded_atom[3] != 0) ? ((*atom_map.find((void*)covalent_bonded_atom[3])).second) : 0,
						(hydrogen_bonded_atom[0] != 0) ? ((*atom_map.find((void*)hydrogen_bonded_atom[0])).second) : 0,
						(hydrogen_bonded_atom[1] != 0) ? ((*atom_map.find((void*)hydrogen_bonded_atom[1])).second) : 0,
						(saltbridge_bonded_atom[0] != 0) ? ((*atom_map.find((void*)saltbridge_bonded_atom[0])).second) : 0,
						(hydrogen_bonded_atom[2] != 0) ? ((*atom_map.find((void*)hydrogen_bonded_atom[2])).second) : 0,
						(hydrogen_bonded_atom[3] != 0) ? ((*atom_map.find((void*)hydrogen_bonded_atom[3])).second) : 0,
						(saltbridge_bonded_atom[1] != 0) ? ((*atom_map.find((void*)saltbridge_bonded_atom[1])).second) : 0);

		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;
	}
		
		// --- MASTER ---
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__MASTER].format_string,
						record_type_format_[PDB::RECORD_TYPE__MASTER].string,
						number_of_remark_records,
						0L,
						number_of_het_records,
						number_of_helix_records,
						number_of_sheet_records,
						number_of_turn_records,
						number_of_site_records,
						number_of_coordinate_transformation_records,
						number_of_atomic_coordinate_records,
						number_of_ter_records,
						number_of_conect_records,
						number_of_seqres_records);
				
		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;
		
		// --- END ---
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__END].format_string,
						record_type_format_[PDB::RECORD_TYPE__END].string);
				
		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;
	}

	void PDBFile::postprocessSSBonds_()
	{
		ResidueMap::Iterator first;
		ResidueMap::Iterator second;
		
		QuadrupleList::iterator it;
		for (it = ssbond_list_.begin(); it != ssbond_list_.end(); ++it)
		{
			first = residue_map_.find(*it);
			++it;
			second = residue_map_.find(*it);
			
			if (first != residue_map_.end() && second != residue_map_.end())
			{
				first->second->setProperty(Residue::PROPERTY__HAS_SSBOND);
				second->second->setProperty(Residue::PROPERTY__HAS_SSBOND);
			} 
			else 
			{
				Log.warn() << "PDBFile::postprocessSSBonds_: could not assign SSBOND for " 
									 << it->first << ":" << it->third << " - " << it->second << "/" << it->fourth << endl;
			}
		}
	}

	void  PDBFile::postprocessHelices_()
	{
		QuadrupleList::iterator res_it = helix_list_.begin();
		SecStructList::iterator helix_it = new_helix_secstruc_list_.begin();
		
		for (; (res_it != helix_list_.end()) && (helix_it != new_helix_secstruc_list_.end()); 
				 ++res_it, ++helix_it)
		{
			// Check whether the two residues exist in the structure
			// (i.e. a residue has been constructed).
			if (!residue_map_.has(*res_it))
			{
				delete *helix_it;
				++res_it;
				continue;
			}
			Residue* initial(residue_map_[*res_it]);
				
			++res_it;

			// Find the last residue. Caveat: under strange circumstances,
			// res_it mitght be invalid, so test for it first.
			if ((res_it == helix_list_.end()) || !residue_map_.has(*res_it))
			{
				delete *helix_it;
				continue;
			}
			Residue* terminal(residue_map_[*res_it]);

			// Delete the constructed helix object if
			// it was not valid and could not be inserted correctly.
			// Otherwise, responsability for the memory is taken over by
			// the System/Chain/Protein it is contained in.
			bool delete_helix = true;
			if ((initial->getParent() == terminal->getParent())
					&& (initial->getParent() != 0))
			{
				const Residue* ptr = initial;
				while (ptr != 0 && ptr != terminal) 
				{
					ptr = dynamic_cast<const Residue*>(ptr->getNext(RTTI::getDefault<Residue>()));
				}
				if (ptr == 0)
				{
					std::swap(initial, terminal);
				}
				ptr = initial;
				while (ptr != 0 && ptr != terminal)
				{
					ptr = dynamic_cast<const Residue*>(ptr->getNext(RTTI::getDefault<Residue>()));
				}
				if (ptr == 0)
				{
					continue;
				}
				
				delete_helix = !Composite::insertParent(**helix_it, *initial, *terminal, false);
			}
			if (delete_helix == true)
			{
				delete (*helix_it);
			}			
		}
	}

	void  PDBFile::postprocessSheetsTurns_
		(PDBFile::QuadrupleList& secstruc_list,
		 PDBFile::SecStructList& new_secstruc_list)
	{
		ResidueIterator protein_res_it;
		ResidueMap::Iterator initial;
		ResidueMap::Iterator terminal;
		QuadrupleList::iterator res_it = secstruc_list.begin();
		SecStructList::iterator secstruc_it = new_secstruc_list.begin();
		
		for (; res_it != secstruc_list.end(); ++res_it, ++secstruc_it)
		{
			initial = residue_map_.find(*res_it);
			++res_it;
			terminal = residue_map_.find(*res_it);
			
			if (initial != residue_map_.end() && terminal != residue_map_.end())
			{
				Residue* initial_residue = initial->second;
				Residue* terminal_residue = terminal->second;
			
				for (protein_res_it = current_protein_->beginResidue(); !protein_res_it.isEnd();	++protein_res_it)
				{
					if (*protein_res_it == *terminal_residue)
					{
						Residue *temp_residue = terminal_residue;
						terminal_residue = initial_residue;
						initial_residue = temp_residue;
					}
		
					if (*protein_res_it == *initial_residue)
					{
						for (; !protein_res_it.isEnd() && (*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>());
								 ++protein_res_it)
						{	
							if (*protein_res_it == *terminal_residue)
							{
								initial_residue = terminal_residue = 0;
					
								break;
							}
						}

						if (initial_residue == 0 || protein_res_it.isEnd())
						{
							break;
						}

						if (!protein_res_it.isEnd())
						{
							initial_residue = &(*protein_res_it);

							for (; !protein_res_it.isEnd(); ++protein_res_it)
							{
								if ((*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>()))
								{
									--protein_res_it;

									break;
								}

								if (*protein_res_it == *terminal_residue)
								{
									break;
								}
							}

							if (protein_res_it.isValid())
							{
								terminal_residue = &(*protein_res_it);
							}
							else
							{
								initial_residue = terminal_residue = 0;
							}
						}
						else
						{
							initial_residue = terminal_residue = 0;
						}
			
						break;
					}
				}

				if (initial_residue == 0 || terminal_residue == 0
						|| Composite::insertParent(*(*secstruc_it), *initial_residue, *terminal_residue, false) == false)
				{
					delete (*secstruc_it);
				}
			}
			else
			{
				delete (*secstruc_it);
			}
		}
	}

	void PDBFile::postprocessRandomCoils_()
	{
		if (current_protein_ == 0)
		{
			return;
		}

		ResidueIterator protein_res_it;
		Residue* initial_residue = 0;
		Residue* terminal_residue = 0;
		SecondaryStructure* sec_struc = 0;
		
		for (ChainIterator chain_it = current_protein_->beginChain();
				 +chain_it; ++chain_it)
		{
			for (protein_res_it = chain_it->beginResidue();
					 +protein_res_it; ++protein_res_it)
			{
				if ((*protein_res_it).hasProperty(Residue::PROPERTY__AMINO_ACID) == true
						&& (*protein_res_it).Composite::hasAncestor(RTTI::getDefault<SecondaryStructure>()) == false)
				{
					terminal_residue = initial_residue = &(*protein_res_it);
		
					for (; !protein_res_it.isEnd()
							 && (*protein_res_it).hasProperty(Residue::PROPERTY__AMINO_ACID) == true
							 && (*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>()) == false;
							 ++protein_res_it)
					{
						terminal_residue = &(*protein_res_it);
					}
		
					sec_struc = new SecondaryStructure();
					sec_struc->setType(SecondaryStructure::COIL);
		
					Composite::insertParent(*sec_struc, *initial_residue, *terminal_residue, false);
		
					if (protein_res_it.isEnd() == true)
					{
						break;
					}
				}
			}
		}
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL
