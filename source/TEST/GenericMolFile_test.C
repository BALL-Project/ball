// $Id: GenericMolFile_test.C,v 1.1 2001/12/17 11:29:33 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(GenericMolFile, "$Id: GenericMolFile_test.C,v 1.1 2001/12/17 11:29:33 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

GenericMolFile* ptr;

CHECK(GenericMolFile::GenericMolFile())
	ptr = new GenericMolFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(GenericMolFile::~GenericMolFile())
  delete ptr;
RESULT

GenericMolFile mol;

CHECK(GenericMolFile::GenericMolFile(const String& filename, File::OpenMode open_mode = File::IN))
  mol = GenericMolFile("data/GenericMolFile_test.dat");
  TEST_EQUAL(mol.isValid(), true)
RESULT

System system;
CHECK(GenericMolFile::read(System& system))
  mol.read(system);
RESULT

CHECK(GenericMolFile::write(const System& system))
  String filename;
  NEW_TMP_FILE(filename)
  GenericMolFile mol2(filename, std::ios::out);
	mol2.write(system);
RESULT

CHECK(GenericMolFile::GenericMolFile& operator >> (System& system))
	System system2;
  mol >> system2;
RESULT


CHECK(GenericMolFile::GenericMolFile& operator << (const System& system))
  String filename;
  NEW_TMP_FILE(filename)
  GenericMolFile mol2(filename, std::ios::out);
  mol2 << system;
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
