// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GenericPDBFile_test.C,v 1.2 2003/07/07 15:46:52 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/genericPDBFile.h>
#include <BALL/DATATYPE/options.h>
///////////////////////////

START_TEST(GenericPDBFile, "$Id: GenericPDBFile_test.C,v 1.2 2003/07/07 15:46:52 amoll Exp $")

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
  // TEST_EQUAL(empty.parseLine("asddasddcasdasd", 2, "dacasd"), false) ??? Segfault
  TEST_EQUAL(empty.parseLine("", 1, ""), true)
RESULT

CHECK(bool readFirstRecord(bool read_values = true))
	TEST_EQUAL(empty.readFirstRecord(), false)
	TEST_EQUAL(empty.readFirstRecord(false), false)
RESULT

CHECK(bool readInvalidRecord(const char* line))
	TEST_EQUAL(empty.readInvalidRecord("asddasdd"), true)
RESULT

CHECK(bool readLine(char* line, Size size, bool extract_values))
 TEST_EQUAL(empty.readLine("addcasdasdasdqawe", 12, true), false) 
 TEST_EQUAL(empty.readLine("addcasda", 1, false), false) 
RESULT

CHECK(bool readNextRecord(bool read_values = true))
	TEST_EQUAL(empty.readNextRecord(), false)
	TEST_EQUAL(empty.readNextRecord(false), false)
RESULT

PDB::Integer my_int;
PDB::Atom my_atom;
PDB::Character my_character;
PDB::ResidueName my_residue_name;
PDB::AChar my_achar;
PDB::LString4 my_lstring4;
PDB::LString2 my_lstring2;

CHECK(bool readRecordANISOU(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Integer u11, PDB::Integer u22, PDB::Integer u33, PDB::Integer u12, PDB::Integer u13, PDB::Integer u23, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
	TEST_EQUAL(empty.readRecordANISOU(my_int, my_atom, my_character, my_residue_name, my_character, my_int, my_achar, my_int, my_int, my_int, my_int, my_int, my_int, my_lstring4, my_lstring2, my_lstring2), false)
RESULT

CHECK(bool readRecordATOM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real orthogonal_vector[3], PDB::Real occupancy, PDB::Real temperature_factor, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
  // ???
RESULT

CHECK(bool readRecordAUTHOR(PDB::Continuation continuation, PDB::PDBList authors))
  // ???
RESULT

CHECK(bool readRecordCAVEAT(PDB::Continuation continuation, PDB::IDcode entry_code, PDB::PDBString comment))
  // ???
RESULT

CHECK(bool readRecordCISPEP(PDB::Integer record_serial_number, PDB::RecordCISPEP::CisPeptide cis_peptide[2], PDB::Integer specific_model_ID, PDB::Real angle_measure))
  // ???
RESULT

CHECK(bool readRecordCOMPND(PDB::Continuation continuation, PDB::SpecificationList component_description))
  // ???
RESULT

CHECK(bool readRecordCONECT(PDB::Integer atom_serial_number, PDB::Integer bonded_atom_serial_number[4], PDB::Integer hydrogen_bonded_atom_serial_number[4], PDB::Integer salt_bridged_atom_serial_number[2]))
  // ???
RESULT

CHECK(bool readRecordCRYST1(PDB::RecordCRYST1::UnitCell& unit_cell))
  // ???
RESULT

CHECK(bool readRecordDBREF(PDB::IDcode entry_code, PDB::Character chain_ID, PDB::RecordDBREF::InitialSequence& initial_sequence, PDB::RecordDBREF::EndingSequence& ending_sequence, PDB::LString6 sequence_database_name, PDB::LString8 sequence_database_accession_code, PDB::LString12 sequence_database_ID_code, PDB::RecordDBREF::InitialDatabaseSegment& initial_database_segment, PDB::RecordDBREF::EndingDatabaseSegment& ending_database_segment))
  // ???
RESULT

CHECK(bool readRecordEND())
  // ???
RESULT

CHECK(bool readRecordENDMDL())
  // ???
RESULT

CHECK(bool readRecordEXPDTA(PDB::Continuation continuation, PDB::SList technique))
  // ???
RESULT

CHECK(bool readRecordFORMUL(PDB::Integer component_number, PDB::LString3 het_ID, PDB::Integer continuation_number, PDB::Character is_water, PDB::PDBString chemical_formula))
  // ???
RESULT

CHECK(bool readRecordFTNOTE(PDB::Integer number, PDB::PDBString text))
  // ???
RESULT

CHECK(bool readRecordHEADER(PDB::String40 classification, PDB::Date deposition_date, PDB::IDcode ID_code))
  // ???
RESULT

CHECK(bool readRecordHELIX(PDB::Integer serial_number, PDB::LString3 helix_ID, PDB::RecordHELIX::InitialResidue& initial_residue, PDB::RecordHELIX::TerminalResidue& terminal_residue, PDB::Integer helix_class, PDB::PDBString comment, PDB::Integer length))
  // ???
RESULT

CHECK(bool readRecordHET(PDB::LString3 het_ID, PDB::Character chain_ID, PDB::Integer sequence_number, PDB::AChar insertion_code, PDB::Integer number_of_HETATM_records, PDB::PDBString text))
  // ???
RESULT

CHECK(bool readRecordHETATM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real orthogonal_vector[3], PDB::Real occupancy, PDB::Real temperature_factor, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
  // ???
RESULT

CHECK(bool readRecordHETNAM(PDB::Continuation continuation, PDB::LString3 het_ID, PDB::PDBString chemical_name))
  // ???
RESULT

CHECK(bool readRecordHYDBND(PDB::RecordHYDBND::HydrogenPartnerAtom hydrogen_partner_atom[2], PDB::RecordHYDBND::HydrogenAtom& hydrogen_atom, PDB::SymmetryOperator first_non_hydrogen_atom, PDB::SymmetryOperator second_non_hydrogen_atom))
  // ???
RESULT

CHECK(bool readRecordJRNL(PDB::LString text))
  // ???
RESULT

CHECK(bool readRecordKEYWDS(PDB::Continuation continuation, PDB::PDBList keywords))
  // ???
RESULT

CHECK(bool readRecordLINK(PDB::RecordLINK::LinkPartner link_partner[2], PDB::SymmetryOperator first_atom, PDB::SymmetryOperator second_atom))
  // ???
RESULT

CHECK(bool readRecordMASTER(PDB::Integer number_of_REMARK_records, PDB::Integer zero, PDB::Integer number_of_HET_records, PDB::Integer number_of_HELIX_records, PDB::Integer number_of_SHEET_records, PDB::Integer number_of_TURN_records, PDB::Integer number_of_SITE_records, PDB::Integer number_of_ORIGX_SCALE_MTRIX_records, PDB::Integer number_of_ATOM_HETATM_records, PDB::Integer number_of_TER_records, PDB::Integer number_of_CONECT_records, PDB::Integer number_of_SEQRES_records))
  // ???
RESULT

CHECK(bool readRecordMODEL(PDB::Integer model_serial_number))
  // ???
RESULT

CHECK(bool readRecordMODRES(PDB::IDcode entry_code, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer sequence_number, PDB::AChar insertion_code, PDB::ResidueName standardresidue_name, PDB::PDBString comment))
  // ???
RESULT

CHECK(bool readRecordMTRIX1(PDB::Integer serial_number, PDB::Real transformation_matrix[4], PDB::Integer is_given))
  // ???
RESULT

CHECK(bool readRecordMTRIX2(PDB::Integer serial_number, PDB::Real transformation_matrix[4], PDB::Integer is_given))
  // ???
RESULT

CHECK(bool readRecordMTRIX3(PDB::Integer serial_number, PDB::Real transformation_matrix[4], PDB::Integer is_given))
  // ???
RESULT

CHECK(bool readRecordOBSLTE(PDB::Continuation continuation, PDB::Date entry_replaced_date, PDB::IDcode entry_code, PDB::IDcode replacing_entry_code[8]))
  // ???
RESULT

CHECK(bool readRecordORIGX1(PDB::Real transformation_matrix[4]))
  // ???
RESULT

CHECK(bool readRecordORIGX2(PDB::Real transformation_matrix[4]))
  // ???
RESULT

CHECK(bool readRecordORIGX3(PDB::Real transformation_matrix[4]))
  // ???
RESULT

CHECK(bool readRecordREMARK(PDB::Integer remark_number, PDB::LString text))
  // ???
RESULT

CHECK(bool readRecordREVDAT(PDB::Integer modification_number, PDB::Continuation continuation, PDB::Date modification_date, PDB::String5 modification_ID, PDB::Integer modification_type, PDB::LString6 name_of_modified_record[4]))
  // ???
RESULT

CHECK(bool readRecordSCALE1(PDB::Real transformation_matrix[4]))
  // ???
RESULT

CHECK(bool readRecordSCALE2(PDB::Real transformation_matrix[4]))
  // ???
RESULT

CHECK(bool readRecordSCALE3(PDB::Real transformation_matrix[4]))
  // ???
RESULT

CHECK(bool readRecordSEQRES(PDB::Integer serial_number, PDB::Character chain_ID, PDB::Integer number_of_residues_in_chain, PDB::ResidueName residue_name[13]))
  // ???
RESULT

CHECK(bool readRecordSHEET(PDB::Integer strand_number, PDB::LString3 sheet_ID, PDB::Integer number_of_strands, PDB::RecordSHEET::InitialResidue& initial_residue, PDB::RecordSHEET::TerminalResidue& terminal_residue, PDB::Integer sense_of_strand, PDB::Atom atom_name_in_current_strand, PDB::RecordSHEET::ResidueInCurrentStrand& residue_in_current_strand, PDB::Atom atom_name_in_previous_strand, PDB::RecordSHEET::ResidueInPreviousStrand& residue_in_previous_strand))
  // ???
RESULT

CHECK(bool readRecordSIGATM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real standard_vector_deviation[3], PDB::Real standard_occupancy_deviation, PDB::Real standard_temperature_deviation, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
  // ???
RESULT

CHECK(bool readRecordSIGUIJ(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Integer sig11, PDB::Integer sig22, PDB::Integer sig33, PDB::Integer sig12, PDB::Integer sig13, PDB::Integer sig23, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
  // ???
RESULT

CHECK(bool readRecordSITE(PDB::Integer sequence_number, PDB::LString3 name, PDB::Integer number_of_residues, PDB::RecordSITE::Residue residue[4]))
  // ???
RESULT

CHECK(bool readRecordSLTBRG(PDB::RecordSLTBRG::PartnerAtom partner_atom[2], PDB::SymmetryOperator first_atom, PDB::SymmetryOperator second_atom))
  // ???
RESULT

CHECK(bool readRecordSOURCE(PDB::Continuation continuation, PDB::SpecificationList sources))
  // ???
RESULT

CHECK(bool readRecordSSBOND(PDB::Integer serial_number, PDB::RecordSSBOND::PartnerResidue partner_residue[2]))
  // ???
RESULT

CHECK(bool readRecordTER(PDB::Integer serial_number, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code))
  // ???
RESULT

CHECK(bool readRecordTITLE(PDB::Continuation continuation, PDB::PDBString title))
  // ???
RESULT

CHECK(bool readRecordTURN(PDB::Integer sequence_number, PDB::LString3 turn_ID, PDB::RecordTURN::InitialResidue& initial_residue, PDB::RecordTURN::TerminalResidue& terminal_residue, PDB::PDBString comment))
  // ???
RESULT

CHECK(bool readRecordTVECT(PDB::Integer serial_number, PDB::Real translation_vector[3], PDB::PDBString comment))
  // ???
RESULT

CHECK(bool readRecords())
  // ???
RESULT

CHECK(bool readUnknownRecord(const char* line))
  // ???
RESULT

CHECK(char getAtomRemotenessIndicator(const PDB::Atom atom_name))
  // ???
RESULT

CHECK(char* getRecordString())
  // ???
RESULT

CHECK(const char* getAtomElementSymbol(const PDB::Atom atom_name, PDB::Atom element_symbol))
  // ???
RESULT

CHECK(const char* getAtomName(const PDB::Atom atom_name))
  // ???
RESULT

CHECK(const char* getRecordString() const)
  // ???
RESULT

CHECK(float getVersion() const)
  // ???
RESULT

CHECK(short getAtomBranchDesignator(const PDB::Atom atom_name))
  // ???
RESULT

CHECK(void clear(int state = 0))
  // ???
RESULT

CHECK(void selectAllModels())
  // ???
RESULT

CHECK(void selectModel(Index index))
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
