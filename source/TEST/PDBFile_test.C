// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

namespace BALL
{
	class TestPDBFile
		:	public PDBFile
	{
		public:
		TestPDBFile(const String& filename, File::OpenMode open_mode)
			:	PDBFile(filename, open_mode)
		{
		}

		// make protected members public for testing
		using PDBFile::writeRawRecord_;
		using PDBFile::writeRecord_;
	};
	

}

START_TEST(PDBFile)

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
	f.open(BALL_TEST_DATA_PATH(PDBFile_test2.pdb));
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
	PDBFile f(BALL_TEST_DATA_PATH(PDBFile_test_models.pdb));
	System s;
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 1)
	TEST_EQUAL(f.options.getInteger(PDBFile::Option::CHOOSE_MODEL), 1)
	s.clear();
	f.selectModel(2);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
	TEST_EQUAL(f.options.getInteger(PDBFile::Option::CHOOSE_MODEL), 2)
	s.clear();
	f.selectModel(3);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	TEST_EQUAL(f.options.getInteger(PDBFile::Option::CHOOSE_MODEL), 3)
	s.clear();
	f.selectModel(4);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 0)
	TEST_EQUAL(f.options.getInteger(PDBFile::Option::CHOOSE_MODEL), 4)
	s.clear();
	f.selectModel(-1);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 6)
	TEST_EQUAL(f.options.getInteger(PDBFile::Option::CHOOSE_MODEL), -1)
	s.clear();
	f.options.setInteger(PDBFile::Option::CHOOSE_MODEL, -1);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 6)
	s.clear();
	f.options.setInteger(PDBFile::Option::CHOOSE_MODEL, 1);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 1)
	s.clear();
	f.options.setInteger(PDBFile::Option::CHOOSE_MODEL, 2);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
	s.clear();
	f.options.setInteger(PDBFile::Option::CHOOSE_MODEL, 3);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.options.setInteger(PDBFile::Option::CHOOSE_MODEL, 4);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 0)

	PDBFile g(BALL_TEST_DATA_PATH(PDBFile_test_no_model.pdb));
	s.clear();
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(1);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(-1);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	g.selectModel(2);
	g.read(s);
	TEST_EQUAL(s.countAtoms(), 0)
RESULT

CHECK([EXTRA]PDBFile strict line checking)
	PDBFile f(BALL_TEST_DATA_PATH(PDBFile_test_line_checking.pdb));
	System s;
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 3)
	s.clear();
	f.options.setBool(PDBFile::Option::STRICT_LINE_CHECKING, true);
	f.read(s);
	TEST_EQUAL(s.countAtoms(), 2)
RESULT

CHECK(void writeRawRecord_(const char* format, const char* tag, ...))
	String filename;
	NEW_TMP_FILE(filename);
	TestPDBFile f(filename, std::ios::out);
	f.writeRawRecord_("%s-%d", "HEADER", "55", 6);
	f.close();
RESULT

CHECK(void writeRecord_(PDB::RecordType record, ...))
	String filename;
	NEW_TMP_FILE(filename);
	TestPDBFile f(filename, std::ios::out);
	f.writeRecord_(PDB::RECORD_TYPE__ENDMDL);
	f.writeRecord_(PDB::RECORD_TYPE__AUTHOR, "AA", "BB", "CC", 1L, 2L, 3L);
	f.close();
RESULT

CHECK(void writeRecord_(const PDB::RecordSEQRES& seqres))
	PDB::RecordSEQRES sr;
	String filename;
	NEW_TMP_FILE(filename);
	TestPDBFile f(filename, std::ios::out);
	f.writeRecord_(sr);
	f.close();
RESULT


CHECK(bool write(const System& system) throw(File::CannotWrite))
	PDBFile f;
	f.open(BALL_TEST_DATA_PATH(PDBFile_test2.pdb));
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test2_2006.txt))

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test2_2006.txt))

	PDBFile out(tmp_filename);
	TEST_EXCEPTION(File::CannotWrite, out.write(S))
RESULT


CHECK(bool write(const System& system) throw(File::CannotWrite) - PDBFormat 1996)
	PDBFile f;
	f.options[PDBFile::Option::WRITE_PDBFORMAT_1996] = true;
	
	f.open(BALL_TEST_DATA_PATH(PDBFile_test2.pdb));
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test2.txt))

	f.open(tmp_filename, std::ios::out);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test2.txt))

	PDBFile out(tmp_filename);
	TEST_EXCEPTION(File::CannotWrite, out.write(S))
RESULT


CHECK([EXTRA]writing of Systems containing Atoms instead of PDBAtoms - PDBFormat 2006)
	FragmentDB db("fragments/Fragments.db");
	System* system = new System;
	String filename;
	NEW_TMP_FILE(filename)
	PDBFile outfile(filename, std::ios::out);
	outfile.options[PDBFile::Option::WRITE_PDBFORMAT_1996] = true;
	
	HINFile methane(BALL_TEST_DATA_PATH(methane.hin));
	system->clear();
	methane.read(*system);
	TEST_EQUAL(system->countAtoms(), 5)
	NEW_TMP_FILE(filename)
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test_write_methane.txt))
	
	delete system;
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
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test_write_empty.txt))
	
	system->insert(*protein);
	protein->insert(*chain);
	Residue* res = db.getResidueCopy("ALA");
	res->setID(1);
	STATUS("Residue ID: " << res->getID())
	chain->insert(*res);
	TEST_EQUAL(system->countAtoms(), 10)
	
	NEW_TMP_FILE(filename)
	STATUS(system->countBonds())
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test3.txt))

	HINFile methane(BALL_TEST_DATA_PATH(methane.hin));
	system->clear();
	methane.read(*system);
	TEST_EQUAL(system->countAtoms(), 5)
	NEW_TMP_FILE(filename)
	outfile.open(filename, std::ios::out);
	outfile << *system;
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(PDBFile_test_write_methane_2006.txt))
	
	delete system;
RESULT

CHECK(PDBFile sets HETATM property)
	PDBFile pdb_file(BALL_TEST_DATA_PATH(PDBFile_test_write_methane.txt));

	System sys;
	pdb_file >> sys;
	for(AtomIterator it = sys.beginAtom(); +it; ++it)
	{
		TEST_EQUAL(it->hasProperty(PDBAtom::PROPERTY__HETATM), true)
	}
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
	PDBFile f(name, std::ios::out);
	f << s;
RESULT

PDBFile* ptr;

CHECK(PDBFile())
	ptr = new PDBFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(~PDBFile() throw())
  delete ptr;
RESULT

String filename;
NEW_TMP_FILE(filename);
PDBFile empty;
Options options;

CHECK(PDBFile(const Options& new_options))
	PDBFile pdb(options);
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
  TEST_EQUAL(empty.hasFormat(), false)
	PDBFile f(BALL_TEST_DATA_PATH(PDBFile_test2.pdb));
	TEST_EQUAL(f.hasFormat(), true)
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

// this is just a test if the types compile
PDB::Integer my_int;
PDB::Atom my_atom;
PDB::Character my_character;
PDB::ResidueName my_residue_name;
PDB::AChar my_achar;
PDB::LString4 my_lstring4;
PDB::LString2 my_lstring2 = "  ";
PDB::Real my_real = 0;
PDB::Continuation my_cont = 0;
PDB::PDBList my_list;

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
