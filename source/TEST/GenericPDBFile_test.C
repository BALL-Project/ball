// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GenericPDBFile_test.C,v 1.5 2003/09/03 12:31:44 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/genericPDBFile.h>
#include <BALL/DATATYPE/options.h>
///////////////////////////

START_TEST(GenericPDBFile, "$Id: GenericPDBFile_test.C,v 1.5 2003/09/03 12:31:44 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

GenericPDBFile* ptr;

CHECK(GenericPDBFile())
	ptr = new GenericPDBFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(~GenericPDBFile() throw())
  delete ptr;
RESULT

String filename;
NEW_TMP_FILE(filename);
GenericPDBFile empty;
Options options;

CHECK(GenericPDBFile(const GenericPDBFile& file) throw())
	GenericPDBFile pdb(empty);
	TEST_EQUAL(pdb.getName(), empty.getName())
	TEST_EQUAL(pdb.getOpenMode(), empty.getOpenMode())
RESULT

CHECK(GenericPDBFile(const Options& new_options))
	GenericPDBFile pdb(options);
RESULT

CHECK(Index getCurrentModel() const)
	empty.getCurrentModel();
RESULT

CHECK(Index getRecordNumber() const)
	empty.getRecordNumber();
RESULT

CHECK(Index getSelectedModel() const)
  empty.getSelectedModel();
RESULT

CHECK(PDB::RecordType getRecordType() const)
  empty.getRecordType();
RESULT

CHECK(Size countRecord(PDB::RecordType record_type, bool from_begin_of_file = true))
  TEST_EQUAL(empty.countRecord(PDB::RECORD_TYPE__ATOM), 0)
RESULT

CHECK(Size countRecordFields() const)
	TEST_EQUAL(empty.countRecordFields(), 0)
RESULT

CHECK(Size countRecords(bool from_begin_of_file = true))
  TEST_EQUAL(empty.countRecords(), 0)
RESULT

CHECK(bool hasFormat())
	const GenericPDBFile& c_p(empty);
  TEST_EQUAL(c_p.hasFormat(), false)
RESULT

CHECK(bool hasFormat() const)
  TEST_EQUAL(empty.hasFormat(), false)
RESULT

CHECK(bool hasFormat(const String& s) const)
	TEST_EQUAL(empty.hasFormat("asdasd"), false)
RESULT

CHECK(bool parseLine(const char* line, Size size, const char* format_string, ...))
	// ???
RESULT

CHECK(bool readFirstRecord(bool read_values = true))
	TEST_EQUAL(empty.readFirstRecord(), true)
	TEST_EQUAL(empty.readFirstRecord(false), true)
RESULT

CHECK(bool readInvalidRecord(const char* line))
	TEST_EQUAL(empty.readInvalidRecord("asddasdd"), true)
RESULT

CHECK(bool readLine(char* line, Size size, bool extract_values))
 TEST_EQUAL(empty.readLine("addcasdasdasdqawe", 12, true), true) 
 TEST_EQUAL(empty.readLine("addcasda", 1, false), true) 
RESULT

CHECK(bool readNextRecord(bool read_values = true))
	TEST_EQUAL(empty.readNextRecord(), true)
	TEST_EQUAL(empty.readNextRecord(false), true)
RESULT

PDB::Integer my_int;
PDB::Atom my_atom;
PDB::Character my_character;
PDB::ResidueName my_residue_name;
PDB::AChar my_achar;
PDB::LString4 my_lstring4;
PDB::LString2 my_lstring2;
PDB::Real my_real;
PDB::Continuation my_cont;
PDB::PDBList my_list;

CHECK(bool readRecordANISOU(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Integer u11, PDB::Integer u22, PDB::Integer u33, PDB::Integer u12, PDB::Integer u13, PDB::Integer u23, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
	TEST_EQUAL(empty.readRecordANISOU(my_int, my_atom, my_character, my_residue_name, my_character, my_int, my_achar, my_int, my_int, my_int, my_int, my_int, my_int, my_lstring4, my_lstring2, my_lstring2), true)
RESULT

PDB::Real my_real3;
CHECK(bool readRecordATOM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real orthogonal_vector[3], PDB::Real occupancy, PDB::Real temperature_factor, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
	TEST_EQUAL(empty.readRecordATOM(my_int, my_atom, my_character, my_residue_name, my_character, my_int, my_achar, &my_real3, my_real, my_real, my_lstring4, my_lstring2, my_lstring2), true)
RESULT

CHECK(bool readRecordAUTHOR(PDB::Continuation continuation, PDB::PDBList authors))
  TEST_EQUAL(empty.readRecordAUTHOR(my_cont, my_list), true)
RESULT

PDB::IDcode my_id;
PDB::PDBString my_string;

CHECK(bool readRecordCAVEAT(PDB::Continuation continuation, PDB::IDcode entry_code, PDB::PDBString comment))
  TEST_EQUAL(empty.readRecordCAVEAT(my_cont, my_id, my_string), true)
RESULT

PDB::RecordCISPEP::CisPeptide my_cisp[2];
CHECK(bool readRecordCISPEP(PDB::Integer record_serial_number, PDB::RecordCISPEP::CisPeptide cis_peptide[2], PDB::Integer specific_model_ID, PDB::Real angle_measure))
  TEST_EQUAL(empty.readRecordCISPEP(my_int, my_cisp, my_int, my_real), true)
RESULT

PDB::SpecificationList my_speci;
CHECK(bool readRecordCOMPND(PDB::Continuation continuation, PDB::SpecificationList component_description))
  TEST_EQUAL(empty.readRecordCOMPND(my_cont, my_speci), true)
RESULT

PDB::Integer my_int4[4];
CHECK(bool readRecordCONECT(PDB::Integer atom_serial_number, PDB::Integer bonded_atom_serial_number[4], PDB::Integer hydrogen_bonded_atom_serial_number[4], PDB::Integer salt_bridged_atom_serial_number[2]))
  TEST_EQUAL(empty.readRecordCONECT(my_int, my_int4, my_int4, my_int4), true)
RESULT

PDB::RecordCRYST1::UnitCell my_cell;
CHECK(bool readRecordCRYST1(PDB::RecordCRYST1::UnitCell& unit_cell))
  TEST_EQUAL(empty.readRecordCRYST1(my_cell), true)
RESULT

PDB::RecordDBREF::InitialSequence my_init;
PDB::RecordDBREF::EndingSequence my_end;
PDB::LString6 my_string6;
PDB::LString8 my_string8;
PDB::LString12 my_string12;
PDB::RecordDBREF::InitialDatabaseSegment my_init_segment;
PDB::RecordDBREF::EndingDatabaseSegment my_end_segment;

CHECK(bool readRecordDBREF(PDB::IDcode entry_code, PDB::Character chain_ID, PDB::RecordDBREF::InitialSequence& initial_sequence, PDB::RecordDBREF::EndingSequence& ending_sequence, PDB::LString6 sequence_database_name, PDB::LString8 sequence_database_accession_code, PDB::LString12 sequence_database_ID_code, PDB::RecordDBREF::InitialDatabaseSegment& initial_database_segment, PDB::RecordDBREF::EndingDatabaseSegment& ending_database_segment))
  TEST_EQUAL(empty.readRecordDBREF(my_id, my_character, my_init, my_end, my_string6, my_string8, my_string12, my_init_segment, my_end_segment), true)
RESULT

CHECK(bool readRecordEND())
  TEST_EQUAL(empty.readRecordEND(), true)
RESULT

CHECK(bool readRecordENDMDL())
	TEST_EQUAL(empty.readRecordENDMDL(), true)
RESULT

PDB::SList my_slist;
CHECK(bool readRecordEXPDTA(PDB::Continuation continuation, PDB::SList technique))
  TEST_EQUAL(empty.readRecordEXPDTA(my_cont, my_slist), true)
RESULT

CHECK(bool readRecordFORMUL(PDB::Integer component_number, PDB::LString3 het_ID, PDB::Integer continuation_number, PDB::Character is_water, PDB::PDBString chemical_formula))
  TEST_EQUAL(empty.readRecordFORMUL(my_int, my_string, my_int, my_character, my_string), true)
RESULT

CHECK(bool readRecordFTNOTE(PDB::Integer number, PDB::PDBString text))
  TEST_EQUAL(empty.readRecordFTNOTE(my_int, my_string), true)
RESULT

PDB::String40 my_string40;
PDB::Date my_date;
CHECK(bool readRecordHEADER(PDB::String40 classification, PDB::Date deposition_date, PDB::IDcode ID_code))
  TEST_EQUAL(empty.readRecordHEADER(my_string40, my_date, my_id), true)
RESULT

PDB::RecordHELIX::InitialResidue my_init_residue;
PDB::RecordHELIX::TerminalResidue my_end_residue;
PDB::LString3 my_string3;
CHECK(bool readRecordHELIX(PDB::Integer serial_number, PDB::LString3 helix_ID, PDB::RecordHELIX::InitialResidue& initial_residue, PDB::RecordHELIX::TerminalResidue& terminal_residue, PDB::Integer helix_class, PDB::PDBString comment, PDB::Integer length))
  TEST_EQUAL(empty.readRecordHELIX(my_int, my_string3, my_init_residue, my_end_residue, my_int, my_string, my_int), true)
RESULT

CHECK(bool readRecordHET(PDB::LString3 het_ID, PDB::Character chain_ID, PDB::Integer sequence_number, PDB::AChar insertion_code, PDB::Integer number_of_HETATM_records, PDB::PDBString text))
  TEST_EQUAL(empty.readRecordHET(my_string3, my_character, my_int, my_achar, my_int, my_string), true)
RESULT

PDB::ResidueName my_name;
PDB::LString4 my_string4;
PDB::LString2 my_string2;
CHECK(bool readRecordHETATM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real orthogonal_vector[3], PDB::Real occupancy, PDB::Real temperature_factor, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
  TEST_EQUAL(empty.readRecordHETATM(my_int, my_atom, my_character, my_name, my_character, my_int, my_achar, &my_real3, my_real, my_real, my_string4, my_string2, my_string2), true)
RESULT

CHECK(bool readRecordHETNAM(PDB::Continuation continuation, PDB::LString3 het_ID, PDB::PDBString chemical_name))
	TEST_EQUAL(empty.readRecordHETNAM(my_cont, my_string3, my_string), true)
RESULT

PDB::SymmetryOperator my_sym;
PDB::RecordHYDBND::HydrogenPartnerAtom my_atom2[2];
PDB::RecordHYDBND::HydrogenAtom my_atom3;
CHECK(bool readRecordHYDBND(PDB::RecordHYDBND::HydrogenPartnerAtom hydrogen_partner_atom[2], PDB::RecordHYDBND::HydrogenAtom& hydrogen_atom, PDB::SymmetryOperator first_non_hydrogen_atom, PDB::SymmetryOperator second_non_hydrogen_atom))
  TEST_EQUAL(empty.readRecordHYDBND(my_atom2, my_atom3, my_sym, my_sym), true)
RESULT

PDB::LString lstring;
CHECK(bool readRecordJRNL(PDB::LString text))
  TEST_EQUAL(empty.readRecordJRNL(lstring), true)
RESULT

CHECK(bool readRecordKEYWDS(PDB::Continuation continuation, PDB::PDBList keywords))
	TEST_EQUAL(empty.readRecordKEYWDS(my_cont, my_list), true)
RESULT

PDB::RecordLINK::LinkPartner my_link[2];
CHECK(bool readRecordLINK(PDB::RecordLINK::LinkPartner link_partner[2], PDB::SymmetryOperator first_atom, PDB::SymmetryOperator second_atom))
  TEST_EQUAL(empty.readRecordLINK(my_link, my_sym, my_sym), true)
RESULT

CHECK(bool readRecordMASTER(PDB::Integer number_of_REMARK_records, PDB::Integer zero, PDB::Integer number_of_HET_records, PDB::Integer number_of_HELIX_records, PDB::Integer number_of_SHEET_records, PDB::Integer number_of_TURN_records, PDB::Integer number_of_SITE_records, PDB::Integer number_of_ORIGX_SCALE_MTRIX_records, PDB::Integer number_of_ATOM_HETATM_records, PDB::Integer number_of_TER_records, PDB::Integer number_of_CONECT_records, PDB::Integer number_of_SEQRES_records))
  TEST_EQUAL(empty.readRecordMASTER(my_int, my_int, my_int, my_int, my_int, my_int, my_int, my_int, my_int, my_int, my_int, my_int), true)
RESULT

CHECK(bool readRecordMODEL(PDB::Integer model_serial_number))
	TEST_EQUAL(empty.readRecordMODEL(my_int), true)
RESULT

CHECK(bool readRecordMODRES(PDB::IDcode entry_code, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer sequence_number, PDB::AChar insertion_code, PDB::ResidueName standardresidue_name, PDB::PDBString comment))
  TEST_EQUAL(empty.readRecordMODRES(my_id, my_name, my_character, my_int, my_achar, my_name, my_string), true)
RESULT

PDB::Real my_real4[4];
CHECK(bool readRecordMTRIX1(PDB::Integer serial_number, PDB::Real transformation_matrix[4], PDB::Integer is_given))
	TEST_EQUAL(empty.readRecordMTRIX1(my_int, my_real4, my_int), true)
RESULT

CHECK(bool readRecordMTRIX2(PDB::Integer serial_number, PDB::Real transformation_matrix[4], PDB::Integer is_given))
	TEST_EQUAL(empty.readRecordMTRIX2(my_int, my_real4, my_int), true)
RESULT

CHECK(bool readRecordMTRIX3(PDB::Integer serial_number, PDB::Real transformation_matrix[4], PDB::Integer is_given))
	TEST_EQUAL(empty.readRecordMTRIX3(my_int, my_real4, my_int), true)
RESULT

PDB::IDcode my_id8[8];
CHECK(bool readRecordOBSLTE(PDB::Continuation continuation, PDB::Date entry_replaced_date, PDB::IDcode entry_code, PDB::IDcode replacing_entry_code[8]))
	TEST_EQUAL(empty.readRecordOBSLTE(my_cont, my_date, my_id, my_id8), true)
RESULT

CHECK(bool readRecordORIGX1(PDB::Real transformation_matrix[4]))
	TEST_EQUAL(empty.readRecordORIGX1(my_real4), true)
RESULT

CHECK(bool readRecordORIGX2(PDB::Real transformation_matrix[4]))
	TEST_EQUAL(empty.readRecordORIGX2(my_real4), true)
RESULT

CHECK(bool readRecordORIGX3(PDB::Real transformation_matrix[4]))
	TEST_EQUAL(empty.readRecordORIGX3(my_real4), true)
RESULT

PDB::LString my_lstring;
CHECK(bool readRecordREMARK(PDB::Integer remark_number, PDB::LString text))
	TEST_EQUAL(empty.readRecordREMARK(my_int, my_lstring), true)
RESULT

PDB::String5 my_string5;
PDB::LString6 my_string6_4[4];
CHECK(bool readRecordREVDAT(PDB::Integer modification_number, PDB::Continuation continuation, PDB::Date modification_date, PDB::String5 modification_ID, PDB::Integer modification_type, PDB::LString6 name_of_modified_record[4]))
	TEST_EQUAL(empty.readRecordREVDAT(my_int, my_cont, my_date, my_string5, my_int, my_string6_4), true)
RESULT

CHECK(bool readRecordSCALE1(PDB::Real transformation_matrix[4]))
	TEST_EQUAL(empty.readRecordSCALE1(my_real4), true)
RESULT

CHECK(bool readRecordSCALE2(PDB::Real transformation_matrix[4]))
	TEST_EQUAL(empty.readRecordSCALE2(my_real4), true)
RESULT

CHECK(bool readRecordSCALE3(PDB::Real transformation_matrix[4]))
	TEST_EQUAL(empty.readRecordSCALE3(my_real4), true)
RESULT

PDB::ResidueName my_name13[13];
CHECK(bool readRecordSEQRES(PDB::Integer serial_number, PDB::Character chain_ID, PDB::Integer number_of_residues_in_chain, PDB::ResidueName residue_name[13]))
	TEST_EQUAL(empty.readRecordSEQRES(my_int, my_character, my_int, my_name13), true)
RESULT

CHECK(bool readRecordSHEET(PDB::Integer strand_number, PDB::LString3 sheet_ID, PDB::Integer number_of_strands, PDB::RecordSHEET::InitialResidue& initial_residue, PDB::RecordSHEET::TerminalResidue& terminal_residue, PDB::Integer sense_of_strand, PDB::Atom atom_name_in_current_strand, PDB::RecordSHEET::ResidueInCurrentStrand& residue_in_current_strand, PDB::Atom atom_name_in_previous_strand, PDB::RecordSHEET::ResidueInPreviousStrand& residue_in_previous_strand))
RESULT

CHECK(bool readRecordSIGATM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real standard_vector_deviation[3], PDB::Real standard_occupancy_deviation, PDB::Real standard_temperature_deviation, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
RESULT

CHECK(bool readRecordSIGUIJ(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Integer sig11, PDB::Integer sig22, PDB::Integer sig33, PDB::Integer sig12, PDB::Integer sig13, PDB::Integer sig23, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
RESULT

PDB::RecordSITE::Residue my_residue4[4];
CHECK(bool readRecordSITE(PDB::Integer sequence_number, PDB::LString3 name, PDB::Integer number_of_residues, PDB::RecordSITE::Residue residue[4]))
	TEST_EQUAL(empty.readRecordSITE(my_int, my_string3, my_int, my_residue4), true)
RESULT

CHECK(bool readRecordSLTBRG(PDB::RecordSLTBRG::PartnerAtom partner_atom[2], PDB::SymmetryOperator first_atom, PDB::SymmetryOperator second_atom))
	PDB::RecordSLTBRG::PartnerAtom partner_atom[2];
	TEST_EQUAL(empty.readRecordSLTBRG(partner_atom, my_sym, my_sym), true)
RESULT

CHECK(bool readRecordSOURCE(PDB::Continuation continuation, PDB::SpecificationList sources))
	TEST_EQUAL(empty.readRecordSOURCE(my_cont, my_speci), true)
RESULT

CHECK(bool readRecordSSBOND(PDB::Integer serial_number, PDB::RecordSSBOND::PartnerResidue partner_residue[2]))
	// ??? PDB::RecordSSBOND::PartnerResidue partner_residue[2];
	// TEST_EQUAL(empty.RecordSSBOND(my_int, partner_residue), true)
RESULT

CHECK(bool readRecordTER(PDB::Integer serial_number, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code))
	TEST_EQUAL(empty.readRecordTER(my_int, my_name, my_character, my_int, my_achar), true)
RESULT

CHECK(bool readRecordTITLE(PDB::Continuation continuation, PDB::PDBString title))
	TEST_EQUAL(empty.readRecordTITLE(my_cont, my_string), true)
RESULT

CHECK(bool readRecordTURN(PDB::Integer sequence_number, PDB::LString3 turn_ID, PDB::RecordTURN::InitialResidue& initial_residue, PDB::RecordTURN::TerminalResidue& terminal_residue, PDB::PDBString comment))
	// ??? bool result = empty.readRecordTURN(my_int, my_string3, my_init_residue, my_end_residue, my_string);
	// TEST_EQUAL(result, true)
RESULT

CHECK(bool readRecordTVECT(PDB::Integer serial_number, PDB::Real translation_vector[3], PDB::PDBString comment))
	// bool result = empty.readRecordTVECT(my_int, my_real3, my_string);
  // TEST_EQUAL(result, true)
RESULT

CHECK(bool readRecords())
  TEST_EQUAL(empty.readRecords(), true)
RESULT

const char* c_ptr = 0;
CHECK(bool readUnknownRecord(const char* line))
  empty.readUnknownRecord(c_ptr);
RESULT

CHECK(char getAtomRemotenessIndicator(const PDB::Atom atom_name))
  empty.getAtomRemotenessIndicator(my_atom);
RESULT

CHECK(char* getRecordString())
  empty.getRecordString();
RESULT

CHECK(const char* getAtomElementSymbol(const PDB::Atom atom_name, PDB::Atom element_symbol))
  empty.getAtomElementSymbol(my_atom, my_atom);
RESULT

CHECK(const char* getAtomName(const PDB::Atom atom_name))
  empty.getAtomName(my_atom);
RESULT

CHECK(const char* getRecordString() const)
  empty.getRecordString();
RESULT

CHECK(float getVersion() const)
  empty.getVersion();
RESULT

CHECK(short getAtomBranchDesignator(const PDB::Atom atom_name))
  empty.getAtomBranchDesignator(my_atom);
RESULT

CHECK(void clear(int state = 0))
  empty.clear(29);
RESULT

CHECK(void selectAllModels())
  empty.selectAllModels();
RESULT

CHECK(void selectModel(Index index))
	empty.selectModel(2);
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
