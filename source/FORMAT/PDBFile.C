// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile.C,v 1.49 2005/02/13 21:58:13 amoll Exp $
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


	const PDB::RecordTypeFormat
	PDBFile::record_type_format_[] =
	{
		{ PDB::RECORD_TYPE__UNKNOWN,"      ", PDB::FORMAT_UNKNOWN },
		{ PDB::RECORD_TYPE__ANISOU, "ANISOU", PDB::FORMAT_ANISOU },
		{ PDB::RECORD_TYPE__ATOM,   "ATOM  ", PDB::FORMAT_ATOM   },
		{ PDB::RECORD_TYPE__AUTHOR, "AUTHOR", PDB::FORMAT_AUTHOR },
		{ PDB::RECORD_TYPE__CAVEAT, "CAVEAT", PDB::FORMAT_CAVEAT },
		{ PDB::RECORD_TYPE__CISPEP, "CISPEP", PDB::FORMAT_CISPEP },
		{ PDB::RECORD_TYPE__COMPND, "COMPND", PDB::FORMAT_COMPND },
		{ PDB::RECORD_TYPE__CONECT, "CONECT", PDB::FORMAT_CONECT },
		{ PDB::RECORD_TYPE__CRYST1, "CRYST1", PDB::FORMAT_CRYST1 },
		{ PDB::RECORD_TYPE__DBREF,  "DBREF ", PDB::FORMAT_DBREF  },
		{ PDB::RECORD_TYPE__END,    "END   ", PDB::FORMAT_END    },
		{ PDB::RECORD_TYPE__ENDMDL, "ENDMDL", PDB::FORMAT_ENDMDL },
		{ PDB::RECORD_TYPE__EXPDTA, "EXPDTA", PDB::FORMAT_EXPDTA },
		{ PDB::RECORD_TYPE__FORMUL, "FORMUL", PDB::FORMAT_FORMUL },
		{ PDB::RECORD_TYPE__FTNOTE, "FTNOTE", PDB::FORMAT_FTNOTE },
		{ PDB::RECORD_TYPE__HEADER, "HEADER", PDB::FORMAT_HEADER },
		{ PDB::RECORD_TYPE__HELIX,  "HELIX ", PDB::FORMAT_HELIX  },
		{ PDB::RECORD_TYPE__HET,    "HET   ", PDB::FORMAT_HET    },
		{ PDB::RECORD_TYPE__HETATM, "HETATM", PDB::FORMAT_HETATM },
		{ PDB::RECORD_TYPE__HETNAM, "HETNAM", PDB::FORMAT_HETNAM },
		{ PDB::RECORD_TYPE__HYDBND, "HYDBND", PDB::FORMAT_HYDBND },
		{ PDB::RECORD_TYPE__JRNL,   "JRNL  ", PDB::FORMAT_JRNL   },
		{ PDB::RECORD_TYPE__KEYWDS, "KEYWDS", PDB::FORMAT_KEYWDS },
		{ PDB::RECORD_TYPE__LINK,   "LINK  ", PDB::FORMAT_LINK   },
		{ PDB::RECORD_TYPE__MASTER, "MASTER", PDB::FORMAT_MASTER },
		{ PDB::RECORD_TYPE__MODEL,  "MODEL ", PDB::FORMAT_MODEL  },
		{ PDB::RECORD_TYPE__MODRES, "MODRES", PDB::FORMAT_MODRES },
		{ PDB::RECORD_TYPE__MTRIX1, "MTRIX1", PDB::FORMAT_MTRIX1 },
		{ PDB::RECORD_TYPE__MTRIX2, "MTRIX2", PDB::FORMAT_MTRIX2 },
		{ PDB::RECORD_TYPE__MTRIX3, "MTRIX3", PDB::FORMAT_MTRIX3 },
		{ PDB::RECORD_TYPE__OBSLTE, "OBSLTE", PDB::FORMAT_OBSLTE },
		{ PDB::RECORD_TYPE__ORIGX1, "ORIGX1", PDB::FORMAT_ORIGX1 },
		{ PDB::RECORD_TYPE__ORIGX2, "ORIGX2", PDB::FORMAT_ORIGX2 },
		{ PDB::RECORD_TYPE__ORIGX3, "ORIGX3", PDB::FORMAT_ORIGX3 },
		{ PDB::RECORD_TYPE__REMARK, "REMARK", PDB::FORMAT_REMARK },
		{ PDB::RECORD_TYPE__REVDAT, "REVDAT", PDB::FORMAT_REVDAT },
		{ PDB::RECORD_TYPE__SCALE1, "SCALE1", PDB::FORMAT_SCALE1 },
		{ PDB::RECORD_TYPE__SCALE2, "SCALE2", PDB::FORMAT_SCALE2 },
		{ PDB::RECORD_TYPE__SCALE3, "SCALE3", PDB::FORMAT_SCALE3 },
		{ PDB::RECORD_TYPE__SEQRES, "SEQRES", PDB::FORMAT_SEQRES },
		{ PDB::RECORD_TYPE__SHEET,  "SHEET ", PDB::FORMAT_SHEET },
		{ PDB::RECORD_TYPE__SIGATM, "SIGATM", PDB::FORMAT_SIGATM },
		{ PDB::RECORD_TYPE__SIGUIJ, "SIGUIJ", PDB::FORMAT_SIGUIJ },
		{ PDB::RECORD_TYPE__SITE,   "SITE  ", PDB::FORMAT_SITE },
		{ PDB::RECORD_TYPE__SLTBRG, "SLTBRG", PDB::FORMAT_SLTBRG },
		{ PDB::RECORD_TYPE__SOURCE, "SOURCE", PDB::FORMAT_SOURCE },
		{ PDB::RECORD_TYPE__SSBOND, "SSBOND", PDB::FORMAT_SSBOND },
		{ PDB::RECORD_TYPE__TER,    "TER   ", PDB::FORMAT_TER },
		{ PDB::RECORD_TYPE__TITLE,  "TITLE ", PDB::FORMAT_TITLE },
		{ PDB::RECORD_TYPE__TURN,   "TURN  ", PDB::FORMAT_TURN },
		{ PDB::RECORD_TYPE__TVECT,  "TVECT ", PDB::FORMAT_TVECT },
	};


	// options and defaults for the PDBFile class
	const char* PDBFile::Option::VERBOSITY = "verbosity";
	const char* PDBFile::Option::STRICT_LINE_CHECKING = "strict_line_checking";
	const char* PDBFile::Option::CHOOSE_MODEL = "choose_model";
	const char* PDBFile::Option::STORE_SKIPPED_RECORDS = "store_skipped_records";
	const char* PDBFile::Option::IGNORE_XPLOR_PSEUDO_ATOMS = "ignore_xplor_pseudo_atoms";
	const char* PDBFile::Option::PARSE_PARTIAL_CHARGES = "parse_partial_charges";

	const Index PDBFile::Default::VERBOSITY = 0;
	const bool  PDBFile::Default::STRICT_LINE_CHECKING = false;
	const Index PDBFile::Default::CHOOSE_MODEL = 1;
	const bool  PDBFile::Default::STORE_SKIPPED_RECORDS = true;
	const bool  PDBFile::Default::IGNORE_XPLOR_PSEUDO_ATOMS = true;
	const bool  PDBFile::Default::PARSE_PARTIAL_CHARGES = false;

	PDBFile::PDBFile()
		:	GenericMolFile(),
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
		:	GenericMolFile(),
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
		// if the element symbol starts with a blank,
		// drop the blank
		if (element_symbol[0] == ' ')
		{
			element_symbol[0] = element_symbol[1];
			element_symbol[1] = '\0';
		}
		// If the element_symbol entry is valid, it has precedence,
		// otherwise, we try to extract it from the atom name.
		Element e(PTE[element_symbol]);
		if (e == Element::UNKNOWN)
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
		parse_partial_charges_ = options.setDefaultBool(Option::PARSE_PARTIAL_CHARGES, Default::PARSE_PARTIAL_CHARGES);
													 
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

		if ((record_type_format == 0)
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
		:	GenericMolFile(),
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
		:	GenericMolFile(),
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


	PDBFile::~PDBFile() throw()
	{
		clear_();
		close();
	}

	bool PDBFile::read(Protein& protein)
		throw(Exception::ParseError)
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
		
		return true;
	}

	bool PDBFile::read(Molecule& molecule)
		throw(Exception::ParseError)
	{
		// ????
		
		return true;
	}

	bool PDBFile::read(System& system)
		throw(Exception::ParseError)
	{
		Protein* protein = new Protein;
		bool result = read(*protein);
		if (result == false)
		{
			delete protein;
		}
		else
		{	
			system.insert(*protein);
		}

		return result;
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
			return false;
		}
		
		if (number_of_proteins == 0)
		{
			if (system.countMolecules() != 1)
			{
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


	bool PDBFile::parseRecordANISOU(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}


	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordANISOU& record)
	{
		return parseLine(line, size,PDB::FORMAT_ANISOU,
										 record.record_name, &record.serial_number,
										 record.atom_name, &record.alternate_location_indicator,
										 record.residue_name, &record.chain_ID, &record.residue_sequence_number,
										 &record.insertion_code, &record.u11, &record.u22, &record.u33,
										 &record.u12, &record.u13, &record.u23, record.segment_ID,
										 record.element_symbol, record.charge);
	}

	bool PDBFile::interpretRecord(const PDB::RecordANISOU& /* record */)
	{
		return true;
	}
					
	bool PDBFile::interpretRecord(const PDB::RecordATOM& record)
	{
		if (record.chain_ID != chain_ID_)
		{
			current_chain_ = new Chain;
			current_protein_->insert(*current_chain_);
			
			residue_name_ = "";
			chain_ID_ = record.chain_ID;
			
			current_chain_->setName(chain_ID_);
		}
		
		if (residue_name_ != (const char*)record.residue_name
				|| (record.residue_sequence_number != residue_sequence_number_)
				|| (record.insertion_code != insertion_code_))
		{
			ResidueQuadruple unique_residue(record.residue_name, record.chain_ID, 
																			record.residue_sequence_number, record.insertion_code);

			current_residue_ = new Residue;
			current_chain_->insert(*current_residue_);
			residue_map_[unique_residue] = current_residue_;
			
			residue_name_ = record.residue_name;
			residue_sequence_number_ = record.residue_sequence_number;
			insertion_code_ = record.insertion_code;
			
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
		if ((record.alternate_location_indicator == ' ' )
				|| (record.alternate_location_indicator == 'A'))
		{
			current_PDB_atom_ = new PDBAtom;
			current_residue_->insert(*current_PDB_atom_);
			PDB_atom_map_[record.serial_number] = current_PDB_atom_;
			
			current_PDB_atom_->setName(getAtomName(record.atom_name));
			current_PDB_atom_->setRemotenessIndicator(getAtomRemotenessIndicator(record.atom_name));
			current_PDB_atom_->setBranchDesignator(getAtomBranchDesignator(record.atom_name));
			current_PDB_atom_->setAlternateLocationIndicator(record.alternate_location_indicator);
			current_PDB_atom_->setOccupancy(record.occupancy);
			current_PDB_atom_->setTemperatureFactor(record.temperature_factor);
			current_PDB_atom_->setRadius(current_PDB_atom_->getElement().getVanDerWaalsRadius());
			current_PDB_atom_->setPosition(Vector3(record.orthogonal_vector[0], record.orthogonal_vector[1], record.orthogonal_vector[2]));
			// Figuring out the element is nto entirely trivial: it *should* be in columns 77-78.
			// However, some codes abuse cols. 77-80 for partial charges. We support this format w/ the 
			// option PARTIAL_CHARGES for reading and writing.
			if (parse_partial_charges_)
			{
				// ????
			}
			else
			{
				char element_symbol[3];
				element_symbol[0] = record.element_symbol[0];
				element_symbol[1] = record.element_symbol[1];
				element_symbol[2] = '\0';
				current_PDB_atom_->setElement(PTE[PDBFile::getAtomElementSymbol(record.atom_name, element_symbol)]);
				try	
				{
					current_PDB_atom_->setFormalCharge(String(record.charge).toInt());
				}
				catch (Exception::InvalidFormat)
				{
				}
				current_PDB_atom_->setCharge(current_PDB_atom_->getFormalCharge());
			}
		}
		
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordATOM& record)
	{
		record.element_symbol[0] = '\0';
		record.occupancy = 1.0;
		record.temperature_factor = 0.0;
		record.segment_ID[0] = '\0';
		record.charge[0] = '\0';
		record.partial_charge[0] = '\0';

		if (parse_partial_charges_ == true)
		{
			return parseLine
				(line, size, 
				 PDB::FORMAT_ATOM_PARTIAL_CRG,
				 record.record_name,
				 &record.serial_number,
				 record.atom_name,
				 &record.alternate_location_indicator,
				 record.residue_name,
				 &record.chain_ID,
				 &record.residue_sequence_number,
				 &record.insertion_code,
				 &record.orthogonal_vector[0],
				 &record.orthogonal_vector[1],
				 &record.orthogonal_vector[2],
				 &record.occupancy,
				 &record.temperature_factor,
				 record.segment_ID,
				 record.partial_charge);			
		}
		else	
		{
			return parseLine
				(line, size, 
				 PDB::FORMAT_ATOM,
				 record.record_name,
				 &record.serial_number,
				 record.atom_name,
				 &record.alternate_location_indicator,
				 record.residue_name,
				 &record.chain_ID,
				 &record.residue_sequence_number,
				 &record.insertion_code,
				 &record.orthogonal_vector[0],
				 &record.orthogonal_vector[1],
				 &record.orthogonal_vector[2],
				 &record.occupancy,
				 &record.temperature_factor,
				 record.segment_ID,
				 record.element_symbol,
				 record.charge,
				 record.partial_charge);
		}
	}
		
	bool PDBFile::parseRecordATOM(const char* line, Size size)
	{
		// Parse only those records belonging to the correct model!
		if ((selected_model_ != 0) && (selected_model_ != current_model_))
		{
			return true;
		}
		static PDB::RecordATOM record;
		if (!fillRecord(line, size, record))
		{
			return false;
		}
		if ((ignore_xplor_pseudo_atoms_ == true)
				&& record.orthogonal_vector[0] >= 9998.0
				&& record.orthogonal_vector[1] >= 9998.0
				&& record.orthogonal_vector[2] >= 9998.0)
		{ // ignore XPLOR pseudo atoms (see Rasmol2.6 source 'molecule.c/ReadPDBAtom')
			return true;
		}
		return interpretRecord(record);
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
					 record.cis_peptide[0].residue_name,
					 &record.cis_peptide[0].chain_ID,
					 &record.cis_peptide[0].residue_sequence_number,
					 &record.cis_peptide[0].insertion_code,
					 record.cis_peptide[1].residue_name,
					 &record.cis_peptide[1].chain_ID,
					 &record.cis_peptide[1].residue_sequence_number,
					 &record.cis_peptide[1].insertion_code,
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


	bool PDBFile::parseRecordCONECT(const char* line, Size size)
	{
		static PDB::RecordCONECT record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCONECT& record)
	{
		record.bonded_atom_serial_number[0] = 0;
		record.bonded_atom_serial_number[1] = 0;
		record.bonded_atom_serial_number[2] = 0;
		record.bonded_atom_serial_number[3] = 0;
		record.hydrogen_bonded_atom_serial_number[0] = 0;
		record.hydrogen_bonded_atom_serial_number[1] = 0;
		record.hydrogen_bonded_atom_serial_number[2] = 0;
		record.hydrogen_bonded_atom_serial_number[3] = 0;
		record.salt_bridged_atom_serial_number[0] = 0;
		record.salt_bridged_atom_serial_number[1] = 0;

		return parseLine(line, size, PDB::FORMAT_CONECT,
					 record.record_name, &record.atom_serial_number,
					 &record.bonded_atom_serial_number[0], &record.bonded_atom_serial_number[1],
					 &record.bonded_atom_serial_number[2], &record.bonded_atom_serial_number[3],
					 &record.hydrogen_bonded_atom_serial_number[0],
					 &record.hydrogen_bonded_atom_serial_number[1],
					 &record.salt_bridged_atom_serial_number[0],
					 &record.hydrogen_bonded_atom_serial_number[2],
					 &record.hydrogen_bonded_atom_serial_number[3],
					 &record.salt_bridged_atom_serial_number[1]);
	}

	bool PDBFile::interpretRecord(const PDB::RecordCONECT& record)
	{
		PDBAtom* PDB_atom = 0;
		Bond*	bond = 0;
		unsigned short i = 0;

		// extract a pointer to the atom, if it was found in the hash map
		PDBAtomMap::Iterator atom_map_it = PDB_atom_map_.find(record.atom_serial_number);
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
			if (record.bonded_atom_serial_number[i] != 0)
			{
				// retrieve the second atom via the hash table
				atom_map_it = PDB_atom_map_.find(record.bonded_atom_serial_number[i]);				
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
			if (record.hydrogen_bonded_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(record.hydrogen_bonded_atom_serial_number[i]);
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
			if (record.salt_bridged_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(record.salt_bridged_atom_serial_number[i]);
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


	bool PDBFile::parseRecordCRYST1(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCRYST1& record)
	{
		return parseLine(line, size, PDB::FORMAT_CRYST1,
					 record.record_name, &record.unit_cell.a,
					 &record.unit_cell.b, &record.unit_cell.c,
					 &record.unit_cell.alpha, &record.unit_cell.beta,
					 &record.unit_cell.gamma, record.unit_cell.space_group,
					 &record.unit_cell.z_value);
	}
		
	bool PDBFile::interpretRecord(const PDB::RecordCRYST1& /* record */)
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


	bool PDBFile::parseRecordENDMDL(const char* line, Size size)
	{
		static PDB::RecordENDMDL record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordENDMDL& record)
	{
		return parseLine(line, size, PDB::FORMAT_ENDMDL,record.record_name);
	}

	bool PDBFile::interpretRecord(const PDB::RecordENDMDL& /* record */)
	{
		// We are now outside a model...
		current_model_ = INVALID_INDEX;
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

	bool PDBFile::parseRecordHEADER(const char* line, Size size)
	{
		static PDB::RecordHEADER record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHEADER& record)
	{
		return parseLine(line, size, PDB::FORMAT_HEADER,
										 record.record_name, record.classification, record.deposition_date, record.ID_code);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHEADER& record)
	{
		current_protein_->setName(record.classification);
		current_protein_->setID(record.ID_code);

		return true;
	}

	bool PDBFile::parseRecordHELIX(const char* line, Size size)
	{
		static PDB::RecordHELIX record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordHELIX& record)
	{
		ResidueQuadruple partner_residue
			(record.initial_residue.residue_name, record.initial_residue.chain_ID,
			 record.initial_residue.sequence_number, record.initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(record.helix_ID);
		
		new_helix_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::HELIX);
		sec_struc->setProperty("HELIX_CLASS", (unsigned int)record.helix_class);
		
		helix_list_.push_back(partner_residue);
		
		partner_residue.set(record.terminal_residue.residue_name, record.terminal_residue.chain_ID,
												record.terminal_residue.sequence_number, record.terminal_residue.insertion_code);
		
		helix_list_.push_back(partner_residue);
		
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHELIX& record)
	{
		return parseLine(line, size, PDB::FORMAT_HELIX,
										 record.record_name, &record.serial_number,
										 record.helix_ID, record.initial_residue.residue_name,
										 &record.initial_residue.chain_ID, &record.initial_residue.sequence_number,
										 &record.initial_residue.insertion_code, record.terminal_residue.residue_name,
										 &record.terminal_residue.chain_ID, &record.terminal_residue.sequence_number,
										 &record.terminal_residue.insertion_code, &record.helix_class,
										 record.comment, &record.length);
	}

	bool PDBFile::parseRecordHET(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHET& record)
	{
		return parseLine(line, size, PDB::FORMAT_HET,
										 record.record_name, record.het_ID,
										 &record.chain_ID, &record.sequence_number,
										 &record.insertion_code, &record.number_of_HETATM_records,
										 record.text);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHET& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordHETATM(const char* line, Size size)
	{
		if ((selected_model_ != 0) && (selected_model_ != current_model_))
		{
			return true;
		}
		parseRecordATOM(line, size); 
		
		current_residue_->clearProperty(Residue::PROPERTY__AMINO_ACID);
		current_residue_->setProperty(Residue::PROPERTY__NON_STANDARD);
		
		RegularExpression regular_expression("^OHH|HOH|HHO|H2O|2HO|OH2|SOL|TIP|TIP2|TIP3|TIP4|WAT|D2O$");
		if (regular_expression.match(current_residue_->getName()) == true)
		{
			current_residue_->setProperty(Residue::PROPERTY__WATER);
		}
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHETATM& record)
	{
		return parseLine(line, size, PDB::FORMAT_HETATM,
										 record.record_name, &record.serial_number,
										 record.atom_name, &record.alternate_location_indicator,
										 record.residue_name, &record.chain_ID, &record.residue_sequence_number,
										 &record.insertion_code, &record.orthogonal_vector[0],
										 &record.orthogonal_vector[1], &record.orthogonal_vector[2],
										 &record.occupancy, &record.temperature_factor,
										 record.segment_ID, record.element_symbol, record.charge);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHETATM& /* record */)
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
		
	bool PDBFile::parseRecordHYDBND(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHYDBND& record)
	{
		return parseLine(line, size, PDB::FORMAT_HYDBND,
										 record.record_name, record.hydrogen_partner_atom[0].atom_name,
										 &record.hydrogen_partner_atom[0].alternate_location_indicator,
										 record.hydrogen_partner_atom[0].residue.residue_name,
										 &record.hydrogen_partner_atom[0].residue.chain_ID,
										 &record.hydrogen_partner_atom[0].residue.sequence_number,
										 &record.hydrogen_partner_atom[0].residue.insertion_code,
										 record.hydrogen_atom.atom_name,
										 &record.hydrogen_atom.alternate_location_indicator,
										 &record.hydrogen_atom.residue.chain_ID,
										 &record.hydrogen_atom.residue.sequence_number,
										 &record.hydrogen_atom.residue.insertion_code,
										 record.hydrogen_partner_atom[1].atom_name,
										 &record.hydrogen_partner_atom[1].alternate_location_indicator,
										 record.hydrogen_partner_atom[1].residue.residue_name,
										 &record.hydrogen_partner_atom[1].residue.chain_ID,
										 &record.hydrogen_partner_atom[1].residue.sequence_number,
										 &record.hydrogen_partner_atom[1].residue.insertion_code,
										 &record.first_non_hydrogen_atom,
										 &record.second_non_hydrogen_atom);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHYDBND& /* record */)
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
										record.link_partner[0].residue.residue_name,
										&record.link_partner[0].residue.chain_ID,
										&record.link_partner[0].residue.sequence_number,
										&record.link_partner[0].residue.insertion_code,
										record.link_partner[1].atom_name,
										&record.link_partner[1].alternate_location_indicator,
										record.link_partner[1].residue.residue_name,
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


	bool PDBFile::parseRecordMODEL(const char* line, Size size)
	{
		static PDB::RecordMODEL record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMODEL& record)
	{
		return parseLine(line, size, PDB::FORMAT_MODEL,
										 record.record_name,	&record.model_serial_number);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMODEL& record)
	{
		current_model_ = (Index)record.model_serial_number;
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
										 record.residue_name,	&record.chain_ID,
										 &record.sequence_number,	&record.insertion_code,
										 record.standard_residue_name,record.comment);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMODRES& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordMTRIX1(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMTRIX1& record)
	{
		return parseLine(line, size, PDB::FORMAT_MTRIX1,
										 record.record_name, &record.serial_number,
										 &record.transformation_matrix[0],
										 &record.transformation_matrix[1],
										 &record.transformation_matrix[2],
										 &record.transformation_matrix[3],
										 &record.is_given);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMTRIX1& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordMTRIX2(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMTRIX2& record)
	{
		return parseLine(line, size, PDB::FORMAT_MTRIX2,
										 record.record_name, &record.serial_number,
										 &record.transformation_matrix[0],
										 &record.transformation_matrix[1],
										 &record.transformation_matrix[2],
										 &record.transformation_matrix[3],
										 &record.is_given);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMTRIX2& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordMTRIX3(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMTRIX3& record)
	{
		return parseLine(line, size, PDB::FORMAT_MTRIX3,
										 record.record_name, &record.serial_number,
										 &record.transformation_matrix[0],
										 &record.transformation_matrix[1],
										 &record.transformation_matrix[2],
										 &record.transformation_matrix[3],
										 &record.is_given);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMTRIX3& /* record */)
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

	bool PDBFile::parseRecordSHEET(const char* line, Size size)
	{
		static PDB::RecordSHEET record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSHEET& record)
	{
		return parseLine(line, size, PDB::FORMAT_SHEET,
										 record.record_name, &record.strand_number,
										 record.sheet_ID, &record.number_of_strands,
										 record.initial_residue.residue_name,
										 &record.initial_residue.chain_ID,
										 &record.initial_residue.sequence_number,
										 &record.initial_residue.insertion_code,
										 record.terminal_residue.residue_name,
										 &record.terminal_residue.chain_ID,
										 &record.terminal_residue.sequence_number,
										 &record.terminal_residue.insertion_code,
										 &record.sense_of_strand,
										 record.atom_name_in_current_strand,
										 record.residue_in_current_strand.residue_name,
										 &record.residue_in_current_strand.chain_ID,
										 &record.residue_in_current_strand.sequence_number,
										 &record.residue_in_current_strand.insertion_code,
										 record.atom_name_in_previous_strand,
										 record.residue_in_previous_strand.residue_name,
										 &record.residue_in_previous_strand.chain_ID,
										 &record.residue_in_previous_strand.sequence_number,
										 &record.residue_in_previous_strand.insertion_code);
	}

	bool PDBFile::interpretRecord(const PDB::RecordSHEET& record)
	{
    ResidueQuadruple partner_residue
      (record.initial_residue.residue_name, record.initial_residue.chain_ID,
       record.initial_residue.sequence_number, record.initial_residue.insertion_code);

    SecondaryStructure* sec_struc = new SecondaryStructure(record.sheet_ID);

    new_sheet_secstruc_list_.push_back(sec_struc);
    sec_struc->setType(SecondaryStructure::STRAND);
    sec_struc->setProperty("STRAND_SENSE", (record.sense_of_strand != 0));

    sheet_list_.push_back(partner_residue);

    partner_residue.set(record.terminal_residue.residue_name, record.terminal_residue.chain_ID,
                        record.terminal_residue.sequence_number, record.terminal_residue.insertion_code);

    sheet_list_.push_back(partner_residue);

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
								 record.residue_name,
								 &record.chain_ID,
								 &record.residue_sequence_number,
								 &record.insertion_code,
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
		if ((selected_model_ != 0) && (selected_model_ != current_model_))
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
								 record.residue_name,
								 &record.chain_ID,
								 &record.residue_sequence_number,
								 &record.insertion_code,
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
		if ((selected_model_ != 0) && (selected_model_ != current_model_))
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
										 record.residue[0].residue_name,
										 &record.residue[0].chain_ID,
										 &record.residue[0].sequence_number,
										 &record.residue[0].insertion_code,
										 record.residue[1].residue_name,
										 &record.residue[1].chain_ID,
										 &record.residue[1].sequence_number,
										 &record.residue[1].insertion_code,
										 record.residue[2].residue_name,
										 &record.residue[2].chain_ID,
										 &record.residue[2].sequence_number,
										 &record.residue[2].insertion_code,
										 record.residue[3].residue_name,
										 &record.residue[3].chain_ID,
										 &record.residue[3].sequence_number,
										 &record.residue[3].insertion_code);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSITE& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordSLTBRG(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSLTBRG& record)
	{
		return parseLine(line, size, PDB::FORMAT_SLTBRG,
										 record.record_name,
										 record.partner_atom[0].atom_name,
										 &record.partner_atom[0].alternate_location_indicator,
										 record.partner_atom[0].residue.residue_name,
										 &record.partner_atom[0].residue.chain_ID,
										 &record.partner_atom[0].residue.sequence_number,
										 &record.partner_atom[0].residue.insertion_code,
										 record.partner_atom[1].atom_name,
										 &record.partner_atom[1].alternate_location_indicator,
										 record.partner_atom[1].residue.residue_name,
										 &record.partner_atom[1].residue.chain_ID,
										 &record.partner_atom[1].residue.sequence_number,
										 &record.partner_atom[1].residue.insertion_code,
										 &record.first_atom,
										 &record.second_atom);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSLTBRG&  /* record */)
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


	bool PDBFile::parseRecordSSBOND(const char* line, Size size)
	{
		static PDB::RecordSSBOND record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSSBOND& record)
	{
		return parseLine(line, size, PDB::FORMAT_SSBOND,
										 record.record_name,
                    &record.serial_number,
                    record.partner_residue[0].residue_name,
                    &record.partner_residue[0].chain_ID,
                    &record.partner_residue[0].sequence_number,
                    &record.partner_residue[0].insertion_code,
                    record.partner_residue[1].residue_name,
                    &record.partner_residue[1].chain_ID,
                    &record.partner_residue[1].sequence_number,
                    &record.partner_residue[1].insertion_code,
                    &record.partner_residue[0].symmetry_operator,
                    &record.partner_residue[1].symmetry_operator);
	}

	bool PDBFile::interpretRecord(const PDB::RecordSSBOND& record)
	{
		ResidueQuadruple partner(record.partner_residue[0].residue_name, record.partner_residue[0].chain_ID,
														 record.partner_residue[0].sequence_number, record.partner_residue[0].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		partner.set(record.partner_residue[1].residue_name, record.partner_residue[1].chain_ID,
								record.partner_residue[1].sequence_number,	record.partner_residue[1].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		return true;
	}


	bool PDBFile::parseRecordTER(const char* line, Size size)
	{
		if ((selected_model_ != 0) && (selected_model_ != current_model_))
		{
			return true;
		}
		static PDB::RecordTER record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordTER& record)
	{
		return parseLine(line, size, PDB::FORMAT_TER, 
										 record.record_name,
										 &record.serial_number,
										 record.residue_name,
										 &record.chain_ID,
										 &record.residue_sequence_number,
										 &record.insertion_code);

	}
	
	bool PDBFile::interpretRecord(const PDB::RecordTER& /* record */)
	{
		chain_ID_ = 0;
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

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordTURN& record)
	{
		return parseLine(line, size, PDB::FORMAT_TURN,
										 record.record_name, &record.sequence_number,
										 record.turn_ID, record.initial_residue.residue_name,
										 &record.initial_residue.chain_ID,
										 &record.initial_residue.sequence_number,
										 &record.initial_residue.insertion_code,
										 record.terminal_residue.residue_name,
										 &record.terminal_residue.chain_ID,
										 &record.terminal_residue.sequence_number,
										 &record.terminal_residue.insertion_code,
										 record.comment);
	}

	bool PDBFile::parseRecordTURN(const char* line, Size size)
	{
		static PDB::RecordTURN record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordTURN& record)
	{
		ResidueQuadruple partner_residue(record.initial_residue.residue_name, record.initial_residue.chain_ID,
																		 record.initial_residue.sequence_number, record.initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(record.turn_ID);
		
		new_turn_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::TURN);
		
		turn_list_.push_back(partner_residue);
		
		partner_residue.set(record.terminal_residue.residue_name, record.terminal_residue.chain_ID,
												record.terminal_residue.sequence_number, record.terminal_residue.insertion_code);
		
		turn_list_.push_back(partner_residue);
		
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


		

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL
