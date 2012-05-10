// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// This file contains the more or less "general" portion of PDBFile.
// It covers the methods doing the generic parsing of records.
// Specifics, in particular those records which are truly interpreted,
// are handled in PDBFileDetails.

#include <BALL/FORMAT/PDBFile.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/DATATYPE/regularExpression.h>

#include <cctype>
#include <cstdarg>
#include <cstdlib>

using std::streampos;
using std::ios;
using std::endl;

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

	// options and defaults for the PDBFile class
	const char* PDBFile::Option::VERBOSITY = "verbosity";
	const char* PDBFile::Option::STRICT_LINE_CHECKING = "strict_line_checking";
	const char* PDBFile::Option::CHOOSE_MODEL = "choose_model";
	const char* PDBFile::Option::STORE_SKIPPED_RECORDS = "store_skipped_records";
	const char* PDBFile::Option::IGNORE_XPLOR_PSEUDO_ATOMS = "ignore_xplor_pseudo_atoms";
	const char* PDBFile::Option::PARSE_PARTIAL_CHARGES = "parse_partial_charges";
	const char* PDBFile::Option::WRITE_PDBFORMAT_1996 = "write_pdbformat_1996";

	const Index PDBFile::Default::VERBOSITY = 0;
	const bool  PDBFile::Default::STRICT_LINE_CHECKING = false;
	const Index PDBFile::Default::CHOOSE_MODEL = 1;
	const bool  PDBFile::Default::STORE_SKIPPED_RECORDS = true;
	const bool  PDBFile::Default::IGNORE_XPLOR_PSEUDO_ATOMS = true;
	const bool  PDBFile::Default::PARSE_PARTIAL_CHARGES = false;
	const bool  PDBFile::Default::WRITE_PDBFORMAT_1996 = false;

	PDBFile::PDBFile()
		:	GenericMolFile(),
			residue_name_(""),
			residue_sequence_number_(-1),
			current_model_(INVALID_INDEX),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN),
			alternate_location_indicator_('A'),
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
			parse_partial_charges_(false),
			write_pdbformat_1996_(false),
			read_done_(0)
	{
		init_();
	}

	PDBFile::PDBFile(const Options& new_options)
		:	GenericMolFile(),
			options(new_options),
			residue_name_(""),
			residue_sequence_number_(-1),
			current_model_(INVALID_INDEX),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN),

			alternate_location_indicator_('A'),
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
			parse_partial_charges_(false),	
			write_pdbformat_1996_(false),
			read_done_(0)
	{
		init_();
	}

	PDBFile::PDBFile(const String& filename, File::OpenMode open_mode)
		:	GenericMolFile(),
			residue_name_(""),
			residue_sequence_number_(-1),
			current_model_(INVALID_INDEX),
			current_record_(INVALID_INDEX),
			record_fields_(0),
			current_record_type_(PDB::RECORD_TYPE__UNKNOWN),
			alternate_location_indicator_('A'),
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
			parse_partial_charges_(false),
			write_pdbformat_1996_(false),
			read_done_(0)
	{
		open(filename, open_mode);
	}


	PDBFile::~PDBFile() 
	{
		clear();
		close();
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
		verbosity_             = options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		strict_line_checking_  = options.setDefaultBool(Option::STRICT_LINE_CHECKING, Default::STRICT_LINE_CHECKING);
		selected_model_        = options.setDefaultInteger(Option::CHOOSE_MODEL, Default::CHOOSE_MODEL);
		ignore_xplor_pseudo_atoms_ = options.setDefaultBool(Option::IGNORE_XPLOR_PSEUDO_ATOMS, Default::IGNORE_XPLOR_PSEUDO_ATOMS);
		store_skipped_records_ = options.setDefaultBool(Option::STORE_SKIPPED_RECORDS, Default::STORE_SKIPPED_RECORDS);
		parse_partial_charges_ = options.setDefaultBool(Option::PARSE_PARTIAL_CHARGES, Default::PARSE_PARTIAL_CHARGES);
		write_pdbformat_1996_  = options.getBool(Option::WRITE_PDBFORMAT_1996);
													 
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

	/** Check whether the current file is in PDB format. 
      We just open the file and check that it contains a valid
      PDB record in the first line.
	*/
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

		strncpy(formats, "%.6s", 4);
		strncpy(&(formats[4]), format_string, sizeof(formats)-4);
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

				for (; isdigit(*format_column); ++format_column) {};

				if (*format_column == '.')
				{
					++format_column;

					temp2 = format_column;
					for (; isdigit(*format_column); ++format_column) {};
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
		va_end(var_args);
		
		return true;
	}

	bool PDBFile::readLine(char* line, Size size, bool extract_values)
	{
		// Look up the type of record by comparing the first six letters.
		PDB::RecordTypeFormat* record_type_format = 0;
		memcpy(const_cast<char*>(compare_record_type_format_.string), line, 6);
		record_type_format = (PDB::RecordTypeFormat*)::bsearch 
			((const void*)&compare_record_type_format_, 
			 (const void*)PDB::RECORD_TYPE_FORMAT, 
			 (Size)PDB::NUMBER_OF_REGISTERED_RECORD_TYPES, 
			 (Size)sizeof(PDB::RecordTypeFormat), 
			 PDBFileRecordNameComparator_);
			
		// If nothing appropriate was found, this record is unknown!
		if (record_type_format == 0)
		{
			return readUnknownRecord(line);
		}
		current_record_type_ = record_type_format->record_type;

		// If we do not want to extract the values, it is sufficient
		// to know that the record type is something we know how to
		// handle, so we are done now.
		if (extract_values == false)
		{
			return true;
		}

		// Otherwise, we parse the record with the appropriate method
		// for the format identified.
		switch(current_record_type_)
		{
			case PDB::RECORD_TYPE__ANISOU:
				return parseRecordANISOU(line, size);

			case PDB::RECORD_TYPE__ATOM:
				return parseRecordATOM(line, size);

			case PDB::RECORD_TYPE__AUTHOR:
				return parseRecordAUTHOR(line, size);

			case PDB::RECORD_TYPE__CAVEAT:
				return parseRecordCAVEAT(line, size);
				
			case PDB::RECORD_TYPE__CISPEP:
				return parseRecordCISPEP(line, size);

			case PDB::RECORD_TYPE__COMPND:
				return parseRecordCOMPND(line, size);

			case PDB::RECORD_TYPE__CONECT:
			case PDB::RECORD_TYPE__CON06:
			case PDB::RECORD_TYPE__CON061:
			case PDB::RECORD_TYPE__CON062:
			case PDB::RECORD_TYPE__CON063:
			case PDB::RECORD_TYPE__CON064:
				return parseRecordCONECT(line, size);

			case PDB::RECORD_TYPE__CRYST1:
				return parseRecordCRYST1(line, size);

			case PDB::RECORD_TYPE__DBREF:
				return parseRecordDBREF(line, size);

			case PDB::RECORD_TYPE__END:
				return parseRecordEND(line, size);

			case PDB::RECORD_TYPE__ENDMDL:
				return parseRecordENDMDL(line, size);

			case PDB::RECORD_TYPE__EXPDTA:
				return parseRecordEXPDTA(line, size);

			case PDB::RECORD_TYPE__FORMUL:
				return parseRecordFORMUL(line, size);

			case PDB::RECORD_TYPE__FTNOTE:
				return parseRecordFTNOTE(line, size);

			case PDB::RECORD_TYPE__HEADER:
				return parseRecordHEADER(line, size);

			case PDB::RECORD_TYPE__HELIX:
				return parseRecordHELIX(line, size);

			case PDB::RECORD_TYPE__HET:
				return parseRecordHELIX(line, size);

			case PDB::RECORD_TYPE__HETATM:
				return parseRecordHETATM(line, size);

			case PDB::RECORD_TYPE__HETNAM:
				return parseRecordHETNAM(line, size);

			case PDB::RECORD_TYPE__HETSYN:
				return parseRecordHETSYN(line, size);

			case PDB::RECORD_TYPE__HYDBND:
				return parseRecordHYDBND(line, size);

			case PDB::RECORD_TYPE__JRNL:
				return parseRecordJRNL(line, size);

			case PDB::RECORD_TYPE__KEYWDS:
				return parseRecordKEYWDS(line, size);

			case PDB::RECORD_TYPE__LINK:
				return parseRecordLINK(line, size);

			case PDB::RECORD_TYPE__MASTER:
				return parseRecordMASTER(line, size);

			case PDB::RECORD_TYPE__MODEL:
				return parseRecordMODEL(line, size);

			case PDB::RECORD_TYPE__MODRES:
				return parseRecordMODRES(line, size);

			case PDB::RECORD_TYPE__MTRIX1:
				return parseRecordMTRIX1(line, size);

			case PDB::RECORD_TYPE__MTRIX2:
				return parseRecordMTRIX2(line, size);

			case PDB::RECORD_TYPE__MTRIX3:
				return parseRecordMTRIX3(line, size);

			case PDB::RECORD_TYPE__OBSLTE:
				return parseRecordOBSLTE(line, size);

			case PDB::RECORD_TYPE__ORIGX1:
				return parseRecordORIGX1(line, size);

			case PDB::RECORD_TYPE__ORIGX2:
				return parseRecordORIGX2(line, size);

			case PDB::RECORD_TYPE__ORIGX3:
				return parseRecordORIGX3(line, size);

			case PDB::RECORD_TYPE__REMARK:
				return parseRecordREMARK(line, size);

			case PDB::RECORD_TYPE__REVDAT:
				return parseRecordREVDAT(line, size);

			case PDB::RECORD_TYPE__SCALE1:
				return parseRecordSCALE1(line, size);

			case PDB::RECORD_TYPE__SCALE2:
				return parseRecordSCALE2(line, size);

			case PDB::RECORD_TYPE__SCALE3:
				return parseRecordSCALE3(line, size);

			case PDB::RECORD_TYPE__SEQADV:
				return parseRecordSEQADV(line, size);

			case PDB::RECORD_TYPE__SEQRES:
				return parseRecordSEQRES(line, size);

			case PDB::RECORD_TYPE__SHEET:
				return parseRecordSHEET(line, size);

			case PDB::RECORD_TYPE__SIGATM:
				return parseRecordSIGATM(line, size);

			case PDB::RECORD_TYPE__SIGUIJ:
				return parseRecordSIGUIJ(line, size);

			case PDB::RECORD_TYPE__SITE:
				return parseRecordSITE(line, size);

			case PDB::RECORD_TYPE__SLTBRG:
				return parseRecordSLTBRG(line, size);

			case PDB::RECORD_TYPE__SOURCE:
				return parseRecordSOURCE(line, size);

			case PDB::RECORD_TYPE__SPRSDE:
				return parseRecordSPRSDE(line, size);

			case PDB::RECORD_TYPE__SSBOND:
				return parseRecordSSBOND(line, size);

			case PDB::RECORD_TYPE__TER:
				return parseRecordTER(line, size);

			case PDB::RECORD_TYPE__TITLE:
				return parseRecordTITLE(line, size);

			case PDB::RECORD_TYPE__TURN:
				return parseRecordTURN(line, size);

			case PDB::RECORD_TYPE__TVECT:
				return parseRecordTVECT(line, size);

			default:				
				return readUnknownRecord(line);
		}
	}

	void PDBFile::clear()
	{
		current_model_ = INVALID_INDEX;
		current_record_ = INVALID_INDEX;
		record_fields_ = 0;
		PDB_atom_map_.clear();
		residue_map_.clear();
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
		
	{
		// set a default for the current record type
		compare_record_type_format_.record_type = PDB::RECORD_TYPE__UNKNOWN;

		// initialize the buffer for record type comparison
		memcpy(const_cast<char*>(compare_record_type_format_.string),
				const_cast<char*>("      "), 7);
		compare_record_type_format_.format_string = "";

		// initialize buffers with zero
		memset(line_buffer_, 0, sizeof(line_buffer_));

		// set defaults
		options.setDefaultInteger(Option::CHOOSE_MODEL, Default::CHOOSE_MODEL);
	}


	bool PDBFile::parseRecordANISOU(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}


	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordANISOU& record)
	{
		return parseLine(line, size,PDB::FORMAT_ANISOU,
										 record.record_name, &record.serial_number,
										 record.atom_name, &record.alternate_location_indicator,
										 record.residue.name, &record.residue.chain_ID, &record.residue.sequence_number,
										 &record.residue.insertion_code, &record.u11, &record.u22, &record.u33,
										 &record.u12, &record.u13, &record.u23, record.segment_ID,
										 record.element_symbol, record.charge);
	}

	bool PDBFile::interpretRecord(const PDB::RecordANISOU& /* record */)
	{
		return true;
	}
					
	bool PDBFile::parseRecordAUTHOR(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordAUTHOR& record)
	{
		return parseLine(line, size, PDB::FORMAT_AUTHOR,
										 record.record_name, &record.continuation,
										 record.authors);
	}

	bool PDBFile::interpretRecord(const PDB::RecordAUTHOR& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordCAVEAT(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCAVEAT& record)
	{
		return parseLine(line, size, PDB::FORMAT_CAVEAT,
										 record.record_name, &record.continuation, 
										 record.entry_code, record.comment);
	}
				
	bool PDBFile::interpretRecord(const PDB::RecordCAVEAT& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordCISPEP(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCISPEP& record)
	{	
		return parseLine(line, size, PDB::FORMAT_CISPEP,
					 record.record_name, &record.record_serial_number,
					 record.residue[0].name,
					 &record.residue[0].chain_ID,
					 &record.residue[0].sequence_number,
					 &record.residue[0].insertion_code,
					 record.residue[1].name,
					 &record.residue[1].chain_ID,
					 &record.residue[1].sequence_number,
					 &record.residue[1].insertion_code,
					 &record.specific_model_ID,
					 &record.angle_measure);
	}
		
	bool PDBFile::interpretRecord(const PDB::RecordCISPEP& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordCOMPND(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCOMPND& record)
	{
		return parseLine(line, size, PDB::FORMAT_COMPND,
										 record.record_name, &record.continuation,
										 record.component_description);
	}

	bool PDBFile::interpretRecord(const PDB::RecordCOMPND& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordDBREF(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordDBREF& record)
	{
		return parseLine(line, size, PDB::FORMAT_DBREF, 
										 record.record_name, record.entry_code,
										 &record.chain_ID, &record.initial_sequence.number,
										 &record.initial_sequence.insertion_code,
										 &record.ending_sequence.number,
										 &record.ending_sequence.insertion_code,
										 record.sequence_database_name,
										 record.sequence_database_accession_code,
										 record.sequence_database_ID_code,
										 &record.chain_ID,
										 &record.initial_database_segment.number,
										 &record.initial_database_segment.insertion_code,
										 &record.ending_database_segment.number,
										 &record.ending_database_segment.insertion_code);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordDBREF& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordEND(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordEND& record)
	{
		return parseLine(line, size, PDB::FORMAT_END, record.record_name);
	}

	bool PDBFile::interpretRecord(const PDB::RecordEND& /* record */)	
	{
		return true;
	}


	bool PDBFile::parseRecordEXPDTA(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordEXPDTA& record)
	{
		return parseLine(line, size, PDB::FORMAT_EXPDTA,
										 record.record_name, &record.continuation,record.technique);

	}
	
	bool PDBFile::interpretRecord(const PDB::RecordEXPDTA& /* record */)
	{
		return true;
	}
															
	
	bool PDBFile::parseRecordFORMUL(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordFORMUL& record)
	{
		return parseLine(line, size, PDB::FORMAT_FORMUL,
					 record.record_name, &record.component_number,
					 record.het_ID, &record.continuation_number,
					 &record.is_water, record.chemical_formula);
	}

	bool PDBFile::interpretRecord(const PDB::RecordFORMUL& /* record */)
	{
		return true;
	}
		
	bool PDBFile::parseRecordFTNOTE(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordFTNOTE& record)
	{
		return parseLine(line, size, PDB::FORMAT_FTNOTE,
										 record.record_name,&record.number, record.text);
	}

	bool PDBFile::interpretRecord(const PDB::RecordFTNOTE& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordHET(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHET& record)
	{
		return parseLine(line, size, PDB::FORMAT_HET,
										 record.record_name, record.residue.name,
										 &record.residue.chain_ID, &record.residue.sequence_number,
										 &record.residue.insertion_code, &record.number_of_HETATM_records,
										 record.text);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHET& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordHETNAM(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHETNAM& record)
	{
		return parseLine(line, size, PDB::FORMAT_HETNAM,
										 record.record_name, &record.continuation,
										 record.het_ID, record.chemical_name);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHETNAM& /* record */)
	{
		return true;
	}
		
	bool PDBFile::parseRecordHETSYN(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHETSYN& record)
	{
		return parseLine(line, size, PDB::FORMAT_HETSYN,
										 record.record_name, &record.continuation,
										 record.het_ID, record.het_synonyms);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHETSYN& /* record */)
	{
		return true;
	}
		
	bool PDBFile::parseRecordJRNL(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordJRNL& record)
	{
		return parseLine(line,size, PDB::FORMAT_JRNL, record.record_name, record.text);
	}

	bool PDBFile::interpretRecord(const PDB::RecordJRNL& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordKEYWDS(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordKEYWDS& record)
	{
		return parseLine(line, size, PDB::FORMAT_KEYWDS,
										 record.record_name, &record.continuation, record.keywords);
	}

	bool PDBFile::interpretRecord(const PDB::RecordKEYWDS& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordLINK(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordLINK& record)
	{
		return parseLine(line,	size, PDB::FORMAT_LINK,
										record.record_name,	record.link_partner[0].atom_name,
										&record.link_partner[0].alternate_location_indicator,
										record.link_partner[0].residue.name,
										&record.link_partner[0].residue.chain_ID,
										&record.link_partner[0].residue.sequence_number,
										&record.link_partner[0].residue.insertion_code,
										record.link_partner[1].atom_name,
										&record.link_partner[1].alternate_location_indicator,
										record.link_partner[1].residue.name,
										&record.link_partner[1].residue.chain_ID,
										&record.link_partner[1].residue.sequence_number,
										&record.link_partner[1].residue.insertion_code,
										&record.first_atom, &record.second_atom);
	}

	bool PDBFile::interpretRecord(const PDB::RecordLINK& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordMASTER(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMASTER& record)
	{
		return parseLine(line, size, PDB::FORMAT_MASTER,
							record.record_name,
							&record.number_of_REMARK_records,
							&record.zero,
							&record.number_of_HET_records,
							&record.number_of_HELIX_records,
							&record.number_of_SHEET_records,
							&record.number_of_TURN_records,
							&record.number_of_SITE_records,
							&record.number_of_ORIGX_SCALE_MTRIX_records,
							&record.number_of_ATOM_HETATM_records,
							&record.number_of_TER_records,
							&record.number_of_CONECT_records,
							&record.number_of_SEQRES_records);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMASTER& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordMODRES(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMODRES& record)
	{
		return parseLine(line, size, PDB::FORMAT_MODRES,
										 record.record_name, record.entry_code,
										 record.residue.name,	&record.residue.chain_ID,
										 &record.residue.sequence_number,	&record.residue.insertion_code,
										 record.standard_residue_name, record.comment);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMODRES& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordOBSLTE(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordOBSLTE& record)
	{
		return parseLine(line, size, PDB::FORMAT_OBSLTE,
										 record.record_name, &record.continuation,
										 record.entry_replaced_date,	record.entry_code,
										 record.replacing_entry_code[0],
										 record.replacing_entry_code[1],
										 record.replacing_entry_code[2],
										 record.replacing_entry_code[3],
										 record.replacing_entry_code[4],
										 record.replacing_entry_code[5],
										 record.replacing_entry_code[6],
										 record.replacing_entry_code[7]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordOBSLTE& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordORIGX1(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordORIGX1& record)
	{
		return parseLine(line, size, PDB::FORMAT_ORIGX1,
										 record.record_name, &record.transformation_matrix[0],
										 &record.transformation_matrix[1], &record.transformation_matrix[2],
										 &record.transformation_matrix[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordORIGX1& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordORIGX2(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordORIGX2& record)
	{
		return parseLine(line, size, PDB::FORMAT_ORIGX2,
										 record.record_name, &record.transformation_matrix[0],
										 &record.transformation_matrix[1], &record.transformation_matrix[2],
										 &record.transformation_matrix[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordORIGX2&  /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordORIGX3(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordORIGX3& record)
	{
		return parseLine(line, size, PDB::FORMAT_ORIGX2,
										 record.record_name, &record.transformation_matrix[0],
										 &record.transformation_matrix[1], &record.transformation_matrix[2],
										 &record.transformation_matrix[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordORIGX3& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordREMARK(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordREMARK& record)
	{
		return parseLine(line, size, PDB::FORMAT_REMARK,
										 record.record_name, &record.remark_number,	record.text);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordREMARK& /* record */) 
	{
		return true;
	}


	bool PDBFile::parseRecordREVDAT(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordREVDAT& record)
	{
		return parseLine(line, size, PDB::FORMAT_REVDAT,
										 record.record_name, &record.modification_number,
										 &record.continuation, record.modification_date,
										 record.modification_ID, &record.modification_type,
										 record.name_of_modified_record[0], record.name_of_modified_record[1],
										 record.name_of_modified_record[2],	record.name_of_modified_record[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordREVDAT& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSCALE1(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSCALE1& record)
	{
		return parseLine(line, size, PDB::FORMAT_SCALE1,
							record.record_name,	&record.transformation_matrix[0],
							&record.transformation_matrix[1],	&record.transformation_matrix[2],
							&record.transformation_matrix[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSCALE1& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSCALE2(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSCALE2& record)
	{
		return parseLine(line, size, PDB::FORMAT_SCALE2,
							record.record_name,	&record.transformation_matrix[0],
							&record.transformation_matrix[1],	&record.transformation_matrix[2],
							&record.transformation_matrix[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSCALE2& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSCALE3(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSCALE3& record)
	{
		return parseLine(line, size, PDB::FORMAT_SCALE2,
							record.record_name,	&record.transformation_matrix[0],
							&record.transformation_matrix[1],	&record.transformation_matrix[2],
							&record.transformation_matrix[3]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSCALE3& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSEQADV(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSEQADV& record)
	{
		return parseLine(line, size, PDB::FORMAT_SEQADV,
										 record.record_name,	
										 record.residue.name, record.residue.chain_ID,
										 record.residue.sequence_number, record.residue.insertion_code,
										 record.database, record.db_id_code,
										 record.db_res, record.db_seq, record.comment);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSEQADV& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSEQRES(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSEQRES& record)
	{
		return parseLine(line, size, PDB::FORMAT_SEQRES,
							record.record_name,	&record.serial_number,
							&record.chain_ID,	&record.number_of_residues_in_chain,
							record.residue_name[0], record.residue_name[1],
							record.residue_name[2],	record.residue_name[3],
							record.residue_name[4], record.residue_name[5],
							record.residue_name[6],	record.residue_name[7],
							record.residue_name[8],	record.residue_name[9],
							record.residue_name[10], record.residue_name[11],
							record.residue_name[12]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSEQRES& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordSIGATM(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSIGATM& record)
	{
		return parseLine(line, size, PDB::FORMAT_SIGATM,
								 record.record_name,
								 &record.serial_number,
								 record.atom_name,
								 &record.alternate_location_indicator,
								 record.residue.name,
								 &record.residue.chain_ID,
								 &record.residue.sequence_number,
								 &record.residue.insertion_code,
								 &record.standard_vector_deviation[0],
								 &record.standard_vector_deviation[1],
								 &record.standard_vector_deviation[2],
								 &record.standard_occupancy_deviation,
								 &record.standard_temperature_deviation,
								 record.segment_ID,
								 record.element_symbol,
								 record.charge);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSIGATM& /* record */)
	{
		if ((selected_model_ != -1) && (selected_model_ != current_model_))
		{
			return true;
		}
		return true;
	}


	bool PDBFile::parseRecordSIGUIJ(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSIGUIJ& record)
	{
		return parseLine(line, size, PDB::FORMAT_SIGUIJ,
										 record.record_name, &record.serial_number,
								 record.atom_name,
								 &record.alternate_location_indicator,
								 record.residue.name,
								 &record.residue.chain_ID,
								 &record.residue.sequence_number,
								 &record.residue.insertion_code,
								 &record.sig11,
								 &record.sig22,
								 &record.sig33,
								 &record.sig12,
								 &record.sig13,
								 &record.sig23,
								 record.segment_ID,
								 record.element_symbol,
								 record.charge);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSIGUIJ& /* record */)
	{
		if ((selected_model_ != -1) && (selected_model_ != current_model_))
		{
			return true;
		}
		return true;
	}


	bool PDBFile::parseRecordSITE(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSITE& record)
	{
		return parseLine(line, size, PDB::FORMAT_SITE,
										 record.record_name, &record.sequence_number,
										 record.name, &record.number_of_residues,
										 record.residue[0].name,
										 &record.residue[0].chain_ID,
										 &record.residue[0].sequence_number,
										 &record.residue[0].insertion_code,
										 record.residue[1].name,
										 &record.residue[1].chain_ID,
										 &record.residue[1].sequence_number,
										 &record.residue[1].insertion_code,
										 record.residue[2].name,
										 &record.residue[2].chain_ID,
										 &record.residue[2].sequence_number,
										 &record.residue[2].insertion_code,
										 record.residue[3].name,
										 &record.residue[3].chain_ID,
										 &record.residue[3].sequence_number,
										 &record.residue[3].insertion_code);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSITE& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordSOURCE(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSOURCE& record)
	{
		return parseLine(line, size, PDB::FORMAT_SOURCE,
										 record.record_name, &record.continuation, record.sources);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSOURCE& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSPRSDE(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSPRSDE& record)
	{
		return parseLine(line, size, PDB::FORMAT_SPRSDE,
										 record.record_name, &record.continuation, record.id_code,
										 record.old_codes[0], record.old_codes[1],
										 record.old_codes[2], record.old_codes[3],
										 record.old_codes[4], record.old_codes[5],
										 record.old_codes[6], record.old_codes[7]);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSPRSDE& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordTITLE(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordTITLE& record)
	{
		return parseLine(line, size, PDB::FORMAT_TITLE,
										record.record_name, &record.continuation, record.title);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordTITLE& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordTVECT(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordTVECT& record)
	{
		return parseLine(line, size, PDB::FORMAT_TVECT,
										 record.record_name, &record.serial_number,
										 &record.translation_vector[0],
										 &record.translation_vector[1],
										 &record.translation_vector[2],
										 record.comment);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordTVECT& /* record */)
	{
		return true;
	}


  void PDBFile::writeRawRecord_(const char* format, const char* tag, ...)
  {
    static char line_buffer[PDB::SIZE_OF_PDB_LINE_BUFFER];

		va_list args;
		va_start(args, tag);
		vsprintf(line_buffer, format, args);
		va_end(args);

    // Terminate each line with a line break and a zero to indicate
    // the string end.
    line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1 - 6] = '\0';

    // Write the line to the stream. Don't flush it (yet).
    File::getFileStream() << tag << line_buffer << '\n';
	}

  void PDBFile::writeRecord_(PDB::RecordType record, ...)
  {

		// Update book keeping records.
		switch (record)
		{
			case PDB::RECORD_TYPE__REMARK:
				book_keeping_.remark_records++; break;

			case PDB::RECORD_TYPE__HET:
				book_keeping_.het_records++; break;

			case PDB::RECORD_TYPE__HELIX:
				book_keeping_.helix_records++; break;

			case PDB::RECORD_TYPE__SHEET:
				book_keeping_.sheet_records++; break;

			case PDB::RECORD_TYPE__TURN:
				book_keeping_.turn_records++; break;

			case PDB::RECORD_TYPE__SITE:
				book_keeping_.site_records++; break;

			case PDB::RECORD_TYPE__ORIGX1:
			case PDB::RECORD_TYPE__ORIGX2:
			case PDB::RECORD_TYPE__ORIGX3:
			case PDB::RECORD_TYPE__SCALE1:
			case PDB::RECORD_TYPE__SCALE2:
			case PDB::RECORD_TYPE__SCALE3:
			case PDB::RECORD_TYPE__MTRIX1:
			case PDB::RECORD_TYPE__MTRIX2:
			case PDB::RECORD_TYPE__MTRIX3:
				book_keeping_.coordinate_transformation_records++; break;

			case PDB::RECORD_TYPE__ATOM:
			case PDB::RECORD_TYPE__HETATM:
				book_keeping_.atomic_coordinate_records++; break;

			case PDB::RECORD_TYPE__TER:
				book_keeping_.ter_records++; break;

			case PDB::RECORD_TYPE__CONECT:
			case PDB::RECORD_TYPE__CON06:
			case PDB::RECORD_TYPE__CON061:
			case PDB::RECORD_TYPE__CON062:
			case PDB::RECORD_TYPE__CON063:
			case PDB::RECORD_TYPE__CON064:
				book_keeping_.conect_records++; break;

			case PDB::RECORD_TYPE__SEQRES:
				book_keeping_.seqres_records++; break;

			default:
				//  no need to account for the remaining records...
				break;
		}

		// Write the record using the appropriate format definitions
		va_list var_args;
;
		va_start(var_args, record);		

    static char line_buffer[PDB::SIZE_OF_PDB_LINE_BUFFER];
		vsprintf(line_buffer, PDB::RECORD_TYPE_FORMAT[record].format_string, var_args);

		va_end(var_args);

    // Terminate each line with a line break and a zero to indicate
    // the string end.
    line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1 - 6] = '\0';
    

		// Write the line to the stream. Don't flush it (yet).
    File::getFileStream() << PDB::RECORD_TYPE_FORMAT[record].string << line_buffer << '\n';
	}

	void PDBFile::addAllRecords_(const PDBInfo& info, PDB::RecordType type)
	{
		std::list<Position> all_lines = info.getRecordIndices(type);
		for (std::list<Position>::const_iterator it = all_lines.begin();
				 it != all_lines.end(); ++it)
		{
			File::getFileStream() << info.getSkippedRecords()[*it] << '\n';
		}

		// Update book keeping records.
		Size number = all_lines.size();
		switch (type)
		{
			case PDB::RECORD_TYPE__REMARK:
				book_keeping_.remark_records += number; break;

			case PDB::RECORD_TYPE__HET:
				book_keeping_.het_records += number; break;

			case PDB::RECORD_TYPE__HELIX:
				book_keeping_.helix_records += number; break;

			case PDB::RECORD_TYPE__SHEET:
				book_keeping_.sheet_records += number; break;

			case PDB::RECORD_TYPE__TURN:
				book_keeping_.turn_records += number; break;

			case PDB::RECORD_TYPE__SITE:
				book_keeping_.site_records += number; break;

			case PDB::RECORD_TYPE__ORIGX1:
			case PDB::RECORD_TYPE__ORIGX2:
			case PDB::RECORD_TYPE__ORIGX3:
			case PDB::RECORD_TYPE__SCALE1:
			case PDB::RECORD_TYPE__SCALE2:
			case PDB::RECORD_TYPE__SCALE3:
			case PDB::RECORD_TYPE__MTRIX1:
			case PDB::RECORD_TYPE__MTRIX2:
			case PDB::RECORD_TYPE__MTRIX3:
				book_keeping_.coordinate_transformation_records += number; break;

			case PDB::RECORD_TYPE__ATOM:
			case PDB::RECORD_TYPE__HETATM:
				book_keeping_.atomic_coordinate_records += number; break;

			case PDB::RECORD_TYPE__TER:
				book_keeping_.ter_records += number; break;

			case PDB::RECORD_TYPE__CONECT:
				book_keeping_.conect_records += number; break;

			case PDB::RECORD_TYPE__CON06:
				book_keeping_.conect_records++; break;

			case PDB::RECORD_TYPE__SEQRES:
				book_keeping_.seqres_records += number; break;

			default:
				//  no need to account for the remaining records...
				break;
		}

	}

	void PDBFile::open(const String& name, File::OpenMode open_mode)
	{
		read_done_ = 0;
		File::open(name,open_mode);
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL
