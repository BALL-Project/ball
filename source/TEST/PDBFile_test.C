// $Id: PDBFile_test.C,v 1.3 2000/07/12 19:36:47 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(PDBFile, "$Id: PDBFile_test.C,v 1.3 2000/07/12 19:36:47 oliver Exp $")

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

	TEST_FILE(tmp_filename.c_str(), "data/PDBFile_test2.txt", true)

	f.open(tmp_filename, File::OUT);
	f.write(S);
	f.close();

	TEST_FILE(tmp_filename.c_str(), "data/PDBFile_test2.txt", true)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
