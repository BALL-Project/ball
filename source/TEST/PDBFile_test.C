// $Id: PDBFile_test.C,v 1.5.4.1 2002/11/08 18:52:16 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>

///////////////////////////

START_TEST(PDBFile, "$Id: PDBFile_test.C,v 1.5.4.1 2002/11/08 18:52:16 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PDBFile* pdb_file;
CHECK(PDBFile::PDBFile())											
pdb_file = new PDBFile;
TEST_NOT_EQUAL(pdb_file, 0)
RESULT

CHECK(PDBFile::~PDBFile())
delete pdb_file;
RESULT

CHECK(PDBFile::read(System&))
RESULT

CHECK(PDBFile::write(System&))
	PDBFile f;
	f.open("data/PDBFile_test2.pdb");
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	System S;
	f.read(S);
	TEST_EQUAL(S.countAtoms(), 892);
	f.close();

	f.open(tmp_filename, File::OUT);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/PDBFile_test2.txt")

	f.open(tmp_filename, File::OUT);
	f.write(S);
	f.close();

	TEST_FILE_REGEXP(tmp_filename.c_str(), "data/PDBFile_test2.txt")
RESULT


CHECK(writing of Systems containing Atoms instead of PDBAtoms)
	FragmentDB db;
	System* system = new System;
	Protein* protein = new Protein;
	Chain* chain = new Chain;
	system->insert(*protein);
	protein->insert(*chain);
	chain->insert(*db.getResidueCopy("ALA"));
	TEST_EQUAL(system->countAtoms(), 10)
	String filename;
	NEW_TMP_FILE(filename)
	STATUS(system->countBonds())
	PDBFile outfile(filename, File::OUT);
	outfile << *system;
	outfile.close();
	delete system;
	TEST_FILE_REGEXP(filename.c_str(), "data/PDBFile_test3.txt")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
