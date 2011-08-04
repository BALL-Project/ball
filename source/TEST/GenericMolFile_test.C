// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>

///////////////////////////

START_TEST(GenericMolFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

GenericMolFile* ptr;

CHECK(GenericMolFile() throw())
	ptr = new GenericMolFile;
	TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(~GenericMolFile() throw())
  delete ptr;
RESULT


CHECK(GenericMolFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
  GenericMolFile mol(BALL_TEST_DATA_PATH(GenericMolFile_test.dat));
  TEST_EQUAL(mol.isValid(), true)
RESULT

GenericMolFile mol(BALL_TEST_DATA_PATH(GenericMolFile_test.dat));
System system;
CHECK(bool read(System& system) throw(Exception::ParseError))
  TEST_EQUAL(mol.read(system), false)
  String filename;
  NEW_TMP_FILE(filename)
  GenericMolFile mol2(filename, std::ios::out);
  TEST_EQUAL(mol2.read(system), false)
RESULT

CHECK(bool write(const System& system) throw(File::CannotWrite))
  String filename;
  NEW_TMP_FILE(filename)
  GenericMolFile mol2(filename, std::ios::out);
	mol2.write(system);
RESULT

CHECK(GenericMolFile& operator >> (System& system) throw(Exception::ParseError))
	System system2;
	mol >> system2;
RESULT


CHECK(GenericMolFile& operator << (const System& system) throw(File::CannotWrite))
  String filename;
  NEW_TMP_FILE(filename)
  GenericMolFile mol2(filename, std::ios::out);
	 mol2 << system;
  GenericMolFile mol3;
  TEST_EXCEPTION(File::CannotWrite, mol3 << system)
RESULT

Molecule m;

CHECK(GenericMolFile& operator << (const Molecule& molecule) throw(File::CannotWrite))
	TEST_EXCEPTION(File::CannotWrite, mol << m)
RESULT

CHECK(GenericMolFile& operator >> (Molecule& molecule) throw(Exception::ParseError))
	mol >> m;
RESULT

CHECK(Molecule* read() throw(Exception::ParseError))
	TEST_EQUAL(mol.read(), 0)
RESULT

CHECK(const GenericMolFile& operator = (const GenericMolFile& rhs) throw(Exception::FileNotFound))
  String filename;
  NEW_TMP_FILE(filename)
  GenericMolFile mol(filename, std::ios::out);
	GenericMolFile mol2;
	mol2 = mol;
	TEST_EQUAL(mol2.getName(), filename)
RESULT

CHECK(bool write(const Molecule& molecule) throw(File::CannotWrite))
	TEST_EXCEPTION(File::CannotWrite, mol.write(m))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
