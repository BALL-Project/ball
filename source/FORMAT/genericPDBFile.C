// $Id: genericPDBFile.C,v 1.8 2000/10/23 23:31:09 amoll Exp $

#include <BALL/FORMAT/genericPDBFile.h>

#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

using namespace std;

namespace BALL 
{

	extern "C" int GenericPDBFileRecordNameComparator_
		(const void* a_ptr, const void* b_ptr)
	{
		return memcmp
			(((PDB::RecordTypeFormat*)a_ptr)->string, 
			 ((PDB::RecordTypeFormat*)b_ptr)->string, 6);
	}


	const PDB::RecordTypeFormat
	GenericPDBFile::record_type_format_[] =
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


	GenericPDBFile::GenericPDBFile()
		:	current_model_(INVALID_INDEX),
			selected_model_(0),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN)
	{
		compare_record_type_format_.record_type = PDB::RECORD_TYPE__UNKNOWN;
		memcpy(const_cast<char*>(compare_record_type_format_.string), const_cast<char*>("      "), 7);
		compare_record_type_format_.format_string = "";

		memset(line_buffer_, 0, sizeof(line_buffer_));
		memset(&record_UNKNOWN, 0, sizeof(PDB::RecordUNKNOWN));
	}

	GenericPDBFile::~GenericPDBFile()
	{
		close();
	}

	const char* GenericPDBFile::getAtomElementSymbol
		(const PDB::Atom atom_name, PDB::Atom element_symbol)
	{
		// If the element_symbol entry is valid, it has precedence
		if (((element_symbol[0] == ' ') && (element_symbol[1] == ' '))
				|| (element_symbol[0] == '\0') || (element_symbol[0] == '\0'))
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

	Size GenericPDBFile::countRecord
		(PDB::RecordType record_type,
		 bool from_begin_of_file)
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

	Size GenericPDBFile::countRecords(bool from_begin_of_file)
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

	bool GenericPDBFile::readFirstRecord(bool read_values)
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

		return readNextRecord(read_values);
	}
		
	bool GenericPDBFile::readNextRecord(bool read_values)
	{
		getline(line_buffer_, sizeof(line_buffer_) - 1, '\n');
		line_buffer_[sizeof(line_buffer_) - 1] = '\0';
		
		if (eof())
		{
			return false;
		}

		Size size = gcount();
		
#		ifdef BALL_STRICT_PDB_LINE_IMPORT
		
		if (size <= PDB::SIZE_OF_PDB_RECORD_LINE)
		{ 
			return readInvalidRecord(line_buffer_);
		}
			
#	endif

		++current_record_;

		return readLine
			(line_buffer_,
			 size,
			 read_values);
	}

	bool GenericPDBFile::readRecords()
	{
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

	bool GenericPDBFile::readUnknownRecord(const char* /* line */)
	{
		return true;
	}

	bool GenericPDBFile::readInvalidRecord(const char* /* line */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordANISOU
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
		return true;
	}

	bool GenericPDBFile::readRecordATOM
		(PDB::Integer /* serial_number */,
		 PDB::Atom /* atom_name */,
		 PDB::Character /* alternate_location_indicator */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Real* /* orthogonal_vector[3] */,
		 PDB::Real /* occupancy */,
		 PDB::Real /* temperature_factor */,
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 /* element_symbol */,
		 PDB::LString2 /* charge */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordAUTHOR
		(PDB::Continuation /* continuation */,
		 PDB::PDBList /* authors */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordCAVEAT
		(PDB::Continuation  /* continuation */,
		 PDB::IDcode /* entry_code */,
		 PDB::PDBString /* comment */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordCISPEP
		(PDB::Integer /* record_serial_number */,
		 PDB::RecordCISPEP::CisPeptide* /* cis_peptide[2] */,
		 PDB::Integer /*  specific_model_ID */,
		 PDB::Real /* angle_measure */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordCOMPND
		(PDB::Continuation /* continuation */,
		 PDB::SpecificationList /* component_description */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordCONECT
		(PDB::Integer /* atom_serial_number */,
		 PDB::Integer* /*  bonded_atom_serial_number[4] */,
		 PDB::Integer* /*  hydrogen_bonded_atom_serial_number[4] */,
		 PDB::Integer* /*  salt_bridged_atom_serial_number[2] */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordCRYST1
		(PDB::RecordCRYST1::UnitCell& /* unit_cell */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordDBREF
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
		return true;
	}

	bool GenericPDBFile::readRecordEND()
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordENDMDL()
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordEXPDTA
		(PDB::Continuation /* continuation */,
		 PDB::SList /* technique */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordFORMUL
		(PDB::Integer /* component_number */,
		 PDB::LString3 /* het_ID */,
		 PDB::Integer /* continuation_number */,
		 PDB::Character /* is_water */,
		 PDB::PDBString /* chemical_formula */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordFTNOTE
		(PDB::Integer /* number */,
		 PDB::PDBString /* text */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordHEADER
		(PDB::String40 /* classification */,
		 PDB::Date /* deposition_date */,
		 PDB::IDcode /* ID_code */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordHELIX
		(PDB::Integer /* serial_number */,
		 PDB::LString3 /* helix_ID */,
		 PDB::RecordHELIX::InitialResidue& /* initial_residue */,
		 PDB::RecordHELIX::TerminalResidue& /* terminal_residue */,
		 PDB::Integer /* class */,
		 PDB::PDBString /* comment */,
		 PDB::Integer /* length */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordHET
		(PDB::LString3 /* het_ID */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Integer /* number_of_HETATM_records */,
		 PDB::PDBString /* text */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordHETATM
		(PDB::Integer /* serial_number */,
		 PDB::Atom /* atom_name */,
		 PDB::Character /* alternate_location_indicator */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::Real /* orthogonal_vector */[3],
		 PDB::Real /* occupancy */,
		 PDB::Real /* temperature_factor */,
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 /* element_symbol */,
		 PDB::LString2 /* charge */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordHETNAM
		(PDB::Continuation /* continuation */,
		 PDB::LString3 /* het_ID */,
		 PDB::PDBString /* chemical_name */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordHYDBND
		(PDB::RecordHYDBND::HydrogenPartnerAtom /* hydrogen_partner */[2],
		 PDB::RecordHYDBND::HydrogenAtom& /* hydrogen_atom */,
		 PDB::SymmetryOperator /* first_non_hydrogen_atom */,
		 PDB::SymmetryOperator /* second_non_hydrogen_atom */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordJRNL
		(PDB::LString /* text */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordKEYWDS
		(PDB::Continuation /* continuation */,
		 PDB::PDBList /* keywords */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordLINK
		(PDB::RecordLINK::LinkPartner /* link_partner */[2],
		 PDB::SymmetryOperator /* first_atom */,
		 PDB::SymmetryOperator /* second_atom */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordMASTER
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
		return true;
	}
		
	bool GenericPDBFile::readRecordMODEL
		(PDB::Integer /* model_serial_number */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordMODRES
		(PDB::IDcode /* entry_code */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* sequence_number */,
		 PDB::AChar /* insertion_code */,
		 PDB::ResidueName /* standard_residue_name */,
		 PDB::PDBString /* comment */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordMTRIX1
		(PDB::Integer /* serial_number */,
		 PDB::Real /* transformation_matrix */[4],
		 PDB::Integer /* is_given */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordMTRIX2
		(PDB::Integer /* serial_number */,
		 PDB::Real /* transformation_matrix */[4],
		 PDB::Integer /* is_given */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordMTRIX3
		(PDB::Integer /* serial_number */,
		 PDB::Real /* transformation_matrix */[4],
		 PDB::Integer /* is_given */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordOBSLTE
		(PDB::Continuation /* continuation */,
		 PDB::Date /* entry_replaced_date */,
		 PDB::IDcode /* entry_code */,
		 PDB::IDcode /* replacing_entry */[8])
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordORIGX1
		(PDB::Real /* transformation_matrix */[4])
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordORIGX2
		(PDB::Real /* transformation_matrix */[4])
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordORIGX3
		(PDB::Real /* transformation_matrix */[4])
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordREMARK
		(PDB::Integer /* remark_number */,
		 PDB::LString /* text */)
	{
		return true;
	}
		
	bool GenericPDBFile::readRecordREVDAT
		(PDB::Integer /* modification_number */,
		 PDB::Continuation /* continuation */,
		 PDB::Date /* modification_date */,
		 PDB::String5 /* modification_ID */,
		 PDB::Integer /* modification_type */,
		 PDB::LString6 /* name_of_modified_record */[4])
	{
		return true;
	}

	bool GenericPDBFile::readRecordSCALE1
		(PDB::Real /* transformation_matrix */[4])
	{
		return true;
	}

	bool GenericPDBFile::readRecordSCALE2
		(PDB::Real /* transformation_matrix */[4])
	{
		return true;
	}

	bool GenericPDBFile::readRecordSCALE3
		(PDB::Real /* transformation_matrix */[4])
	{
		return true;
	}

	bool GenericPDBFile::readRecordSEQRES
		(PDB::Integer /* serial_number */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* number_of_residues_in_chain */,
		 PDB::ResidueName /* residue_name */[13])
	{
		return true;
	}

	bool GenericPDBFile::readRecordSHEET
		(PDB::Integer /* strand_number */,
		 PDB::LString3 /* sheet_ID */,
		 PDB::Integer /* number_of_strands */,
		 PDB::RecordSHEET::InitialResidue& /* initial_residue */,
		 PDB::RecordSHEET::TerminalResidue& /* terminal_residue */,
		 PDB::Integer /* sense_of_strand */,
		 PDB::Atom /* atom_name_in_current_strand */,
		 PDB::RecordSHEET::ResidueInCurrentStrand& /* residue_in_current_strand */,
		 PDB::Atom /* atom_name_in_previous_strand */,
		 PDB::RecordSHEET::ResidueInPreviousStrand& /* residue_in_previous_strand */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordSIGATM
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
		return true;
	}

	bool GenericPDBFile::readRecordSIGUIJ
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
		return true;
	}

	bool GenericPDBFile::readRecordSITE
		(PDB::Integer /* sequence_number */,
		 PDB::LString3 /* name */,
		 PDB::Integer /* number_of_residues */,
		 PDB::RecordSITE::Residue /* residue */[4])
	{
		return true;
	}

	bool GenericPDBFile::readRecordSLTBRG
		(PDB::RecordSLTBRG::PartnerAtom /* partner_atom */[2],
		 PDB::SymmetryOperator /* first_atom */,
		 PDB::SymmetryOperator /* second_atom */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordSOURCE
		(PDB::Continuation /* continuation */,
		 PDB::SpecificationList /*sources */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordSSBOND
		(PDB::Integer /* serial_number */,
		 PDB::RecordSSBOND::PartnerResidue /* partner_residue */[2])
	{
		return true;
	}

	bool GenericPDBFile::readRecordTER
		(PDB::Integer /* serial_number */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordTITLE
		(PDB::Continuation /* continuation */,
		 PDB::PDBString /* title */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordTURN
		(PDB::Integer /* sequence_number */,
		 PDB::LString3 /* turn_ID */,
		 PDB::RecordTURN::InitialResidue& /* initial_residue */,
		 PDB::RecordTURN::TerminalResidue& /* terminal_residue */,
		 PDB::PDBString /* comment */)
	{
		return true;
	}

	bool GenericPDBFile::readRecordTVECT
		(PDB::Integer /* serial_number */,
		 PDB::Real /* translation_vector */[3],
		 PDB::PDBString /*comment */)
	{
		return true;
	}

	bool GenericPDBFile::hasFormat()
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

	bool GenericPDBFile::parseLine
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
				for (; isdigit(*format_column); ++format_column)
				{
				}

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
					*var_arg_double = atof(temp);
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

	bool GenericPDBFile::readLine(char* line, Size size, bool extract_values)
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
				 GenericPDBFileRecordNameComparator_);
			
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

				if (selected_model_ != 0 && selected_model_ != current_model_)
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
				
				if (selected_model_ != 0 && selected_model_ != current_model_)
				{
					return true;
				}

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

				if (hasProperty(PDB::PROPERTY__PSEUDO_XPLOR_ATOM_IMPORT) == false
						&& record_ATOM.orthogonal_vector[0] == 9999.000
						&& record_ATOM.orthogonal_vector[1] == 9999.000
						&& record_ATOM.orthogonal_vector[2] == 9999.000)
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
				
				return readRecordAUTHOR
		(record_AUTHOR.continuation,
		 record_AUTHOR.authors);


				
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

				if (selected_model_ != 0
			&& selected_model_ != current_model_)
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

				if (selected_model_ != 0
						&& selected_model_ != current_model_)
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

				if (selected_model_ != 0
						&& selected_model_ != current_model_)
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

				if (selected_model_ != 0
						&& selected_model_ != current_model_)
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

	void GenericPDBFile::clear_()
	{
		current_model_ = INVALID_INDEX;
		current_record_ = INVALID_INDEX;
		record_fields_ = 0;
		//  current_record_type_ = PDB::RECORD_TYPE__UNKNOWN;
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/genericPDBFile.iC>
#	endif

} // namespace BALL
