// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBFile_test.C,v 1.21 2004/11/09 15:34:09 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

START_TEST(PDBFile, "$Id: PDBFile_test.C,v 1.21 2004/11/09 15:34:09 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PDBFile* pdb_file;
CHECK(PDBFile())
	pdb_file = new PDBFile;
	TEST_NOT_EQUAL(pdb_file, 0)
RESULT

CHECK(~PDBFile() throw())
	delete pdb_file;
RESULT

CHECK(void read(System& system))
	PDBFile f;
	f.open("data/PDBFile_test2.pdb");
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();
	
	TEST_EQUAL(S.countResidues(), 58)
	TEST_EQUAL(S.countChains(), 1)
	TEST_EQUAL(S.countSecondaryStructures(), 7)

	ABORT_IF(S.countSecondaryStructures() != 7)
	Position i = 0;
	SecondaryStructureIterator it(S.beginSecondaryStructure());
	for (; +it; ++it)
	{
		STATUS(i << " / " << it->getType())

		// Check the correct scondary structure types
		switch (i)
		{
			case 0:
			case 2:
			case 4:
			case 6:
				TEST_EQUAL(it->getType(), SecondaryStructure::COIL)
				break;
			
			case 5:
				TEST_EQUAL(it->getType(), SecondaryStructure::HELIX)
				break;

			case 1:
			case 3:
				TEST_EQUAL(it->getType(), SecondaryStructure::STRAND)
				break;

			// This should not happen
			default:
				TEST_EQUAL(1, 0)
		}
		++i;
	}
RESULT

CHECK([EXTRA]PDBFile::selectModel())
	PDBFile f("data/PDBFile_test_models.pdb");
	System s;
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 1)
	s.clear();
	f.selectModel(2);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
	s.clear();
	f.selectModel(3);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.selectModel(4);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 0)
	s.clear();
	f.selectModel(0);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 6)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 0);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 6)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 1);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 1)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 2);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 3);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.options.setInteger(GenericPDBFile::Option::CHOOSE_MODEL, 4);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 0)

	PDBFile g("data/PDBFile_test_no_model.pdb");
	s.clear();
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(1);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(0);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(2);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK([EXTRA]PDBFile strict line checking)
	PDBFile f("data/PDBFile_test_line_checking.pdb");
	System s;
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.options.setBool(GenericPDBFile::Option::STRICT_LINE_CHECKING, true);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
RESULT

CHECK(bool write(const System& system) throw(File::CannotWrite))
	PDBFile f;
	f.open("data/PDBFile_test2.pdb");
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/PDBFile_test2.txt")

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/PDBFile_test2.txt")

	PDBFile out(tmp_filename);
	TEST_EXCEPTION(File::CannotWrite, out.write(S))
RESULT


CHECK([EXTRA]writing of Systems containing Atoms instead of PDBAtoms)
	FragmentDB db("fragments/Fragments.db");
	System* system = new System;
	Protein* protein = new Protein;
	Chain* chain = new Chain;

	String filename;
	NEW_TMP_FILE(filename)
	PDBFile outfile(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test_write_empty.txt")

	system->insert(*protein);
	protein->insert(*chain);
	chain->insert(*db.getResidueCopy("ALA"));
	TEST_EQUAL(system->countAtoms(), 10)
	NEW_TMP_FILE(filename)
	STATUS(system->countBonds())
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test3.txt")

	HINFile methane("data/methane.hin");
	system->clear();
	methane.read(*system);
	TEST_EQUAL(system->countAtoms(), 5)
	NEW_TMP_FILE(filename)
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test_write_methane.txt")

	delete system;
RESULT

CHECK(PDBFile& operator << (const Molecule& molecule))
RESULT

CHECK(PDBFile& operator << (const Protein& protein))
RESULT

CHECK(PDBFile& operator << (const System& system))
RESULT

CHECK(PDBFile& operator >> (Protein& protein))
RESULT

CHECK(PDBFile& operator >> (System& system))
RESULT

CHECK(PDBFile(const PDBFile& PDB_file) throw(Exception::FileNotFound))
RESULT

CHECK(PDBFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
RESULT

CHECK(bool readInvalidRecord(const char* line))
RESULT

CHECK(bool readRecordATOM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real orthogonal_vector[3], PDB::Real occupancy, PDB::Real temperature_factor, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
RESULT

CHECK(bool readRecordCONECT(PDB::Integer atom_serial_number, PDB::Integer bonded_atom_serial_number[4], PDB::Integer hydrogen_bonded_atom_serial_number[4], PDB::Integer salt_bridged_atom_serial_number[2]))
RESULT

CHECK(bool readRecordEND())
RESULT

CHECK(bool readRecordENDMDL())
RESULT

CHECK(bool readRecordHEADER(PDB::String40 classification, PDB::Date deposition_date, PDB::IDcode ID_code))
RESULT

CHECK(bool readRecordHELIX(PDB::Integer serial_number, PDB::LString3 helix_ID, PDB::RecordHELIX::InitialResidue& initial_residue, PDB::RecordHELIX::TerminalResidue& terminal_residue, PDB::Integer helix_class, PDB::PDBString comment, PDB::Integer length))
RESULT

CHECK(bool readRecordHETATM(PDB::Integer serial_number, PDB::Atom atom_name, PDB::Character alternate_location_indicator, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code, PDB::Real orthogonal_vector[3], PDB::Real occupancy, PDB::Real temperature, PDB::LString4 segment_ID, PDB::LString2 element_symbol, PDB::LString2 charge))
RESULT

CHECK(bool readRecordMODEL(PDB::Integer model_serial_number))
RESULT

CHECK(bool readRecordSHEET(PDB::Integer strand_number, PDB::LString3 sheet_ID, PDB::Integer number_of_strands, PDB::RecordSHEET::InitialResidue& initial_residue, PDB::RecordSHEET::TerminalResidue& terminal_residue, PDB::Integer sense_of_strand, PDB::Atom atom_name_in_current_strand, PDB::RecordSHEET::ResidueInCurrentStrand& residue_in_current_strand, PDB::Atom atom_name_in_previous_strand, PDB::RecordSHEET::ResidueInPreviousStrand& residue_in_previous_strand))
RESULT

CHECK(bool readRecordSSBOND(PDB::Integer serial_number, PDB::RecordSSBOND::PartnerResidue partner_residue[2]))
RESULT

CHECK(bool readRecordTER(PDB::Integer serial_number, PDB::ResidueName residue_name, PDB::Character chain_ID, PDB::Integer residue_sequence_number, PDB::AChar insertion_code))
RESULT

CHECK(bool readRecordTURN(PDB::Integer sequence_number, PDB::LString3 turn_ID, PDB::RecordTURN::InitialResidue& initial_residue, PDB::RecordTURN::TerminalResidue& terminal_residue, PDB::PDBString comment))
RESULT

CHECK(bool readUnknownRecord(const char* line))
RESULT

CHECK(bool write(const Protein& protein) throw(File::CannotWrite))
RESULT

CHECK(void read(Protein& protein))
RESULT

CHECK(void write(const Molecule& molecule))
RESULT


CHECK([EXTRA] strange_things)
	System s;
	Protein p;
	Residue r;
	Atom a;
	((AtomContainer*)&s)->insert(a);
	String name;
	NEW_TMP_FILE(name);
	PDBFile f(name, File::OUT);
	f << s;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
