// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: GenericMolFile_test.C,v 1.1.2.1 2003/01/07 13:22:26 anker Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/PTE.h>

///////////////////////////

START_TEST(GenericMolFile, "$Id: GenericMolFile_test.C,v 1.1.2.1 2003/01/07 13:22:26 anker Exp $")

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

CHECK(GenericMolFile::GenericMolFile(const String& filename, File::OpenMode open_mode = std::ios::in))
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

}

catch (BALL::Exception::FileNotFound e)
{
	TEST::this_test = false;
	TEST::test = false;
	TEST::all_tests = false;
 	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))
	{
		if (TEST::exception == 1)
			TEST::exception++;
   	std::cout << std::endl << "    (caught exception of type ";
		std::cout << e.getName();
		if ((e.getLine() > 0) && (!(e.getFile() == "")))
			std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();
		std::cout << " while looking for file " << e.getFilename();
		std::cout << " - unexpected!) " << std::endl;
	}
}
catch (BALL::Exception::GeneralException& e)
{
	TEST::this_test = false;
	TEST::test = false;
	TEST::all_tests = false;
 	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))
	{
		if (TEST::exception == 1) TEST::exception++;
   	std::cout << std::endl << "    (caught exception of type ";
		std::cout << e.getName();
		if ((e.getLine() > 0) && (!(e.getFile() == "")))
		{
			std::cout << " outside a subtest, which was thrown in line " << e.getLine() << " of file " << e.getFile();
		}
		std::cout << " - unexpected!) " << std::endl;
		std::cout << "    (message is: " << e.getMessage() << ")" << std::endl;
	}
 }
	
catch (...)
{
	TEST::this_test = false;
	TEST::test = false;
	TEST::all_tests = false;
 	if ((TEST::verbose > 1) || (!TEST::this_test && (TEST::verbose > 0)))
	{
   	std::cout << std::endl << "    (caught unidentified and unexpected exception outside a subtest!) " << std::endl;
	}
}

	
while (TEST::tmp_file_list.size() > 0 && TEST::verbose < 1)
{
	::BALL::File::remove(TEST::tmp_file_list.back());
	TEST::tmp_file_list.pop_back();
}
	
if (!TEST::all_tests)
{
	std::cout << "FAILED" << std::endl;
	return 1;
} 
else 
{
	std::cout << "PASSED" << std::endl;
	return 0;
}
}
