// $Id: DCDFile_test.C,v 1.6 2001/07/15 17:32:38 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/FORMAT/DCDFile.h>

///////////////////////////

START_TEST(DCDFile, "$Id: DCDFile_test.C,v 1.6 2001/07/15 17:32:38 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         

String dcd_test_file("data/DCD_test.dcd");

CHECK(DCDFile::DCDHeader::DCDHeader() throw())
  DCDFile::DCDHeader* p = new DCDFile::DCDHeader;
	TEST_NOT_EQUAL(p, 0)
RESULT

DCDFile* p = new DCDFile;

CHECK(DCDFile::DCDFile() throw())
  p = new DCDFile;
	TEST_NOT_EQUAL(p, 0)
RESULT


CHECK(DCDFile::~DCDFile() throw())
	delete p;
RESULT


CHECK(DCDFile::DCDFile(const DCDFile& file) throw())
  DCDFile first_file(dcd_test_file);
	DCDFile second_file(first_file);
	bool test = (first_file == second_file);
	TEST_EQUAL(test, true)
RESULT


CHECK(DCDFile::DCDFile(const String& name, File::OpenMode open_mode) throw())
  //BAUSTELLE
  DCDFile test_file(dcd_test_file, File::IN);
	TEST_EQUAL(test_file.isOpen(), true)
	TEST_EQUAL(test_file.getOpenMode(), File::IN)
	test_file.close();
	TEST_EQUAL(test_file.isOpen(), false)

	DCDFile test_file2("test.out.dcd", File::OUT);
	TEST_EQUAL(test_file2.isOpen(), true)
	TEST_EQUAL(test_file2.getOpenMode(), File::OUT)
	test_file2.close();
	TEST_EQUAL(test_file2.isOpen(), false)
RESULT


CHECK(DCDFile::DCDFile& operator = (const DCDFile& file) throw())
  DCDFile one(dcd_test_file, File::IN);
	DCDFile two;
	two = one;
	bool test = (one == two);
	TEST_EQUAL(test, true)
RESULT


CHECK(DCDFile::clear() throw())
	DCDFile file(dcd_test_file, File::IN);
	file.clear();
	DCDFile empty;
	bool test = (file == empty);
	TEST_EQUAL(test, true)
RESULT


CHECK(DCDFile::bool operator == (const DCDFile& file) throw())
  DCDFile one(dcd_test_file, File::IN);
  DCDFile two(dcd_test_file, File::IN);
	bool test = (one == two);
	TEST_EQUAL(test, true)
	DCDFile three;
	test = (one == three);
	TEST_EQUAL(test, false)
RESULT

// init() is called every time an object is constructed, so we won't have a
// dedicated test here. There is no reasonable way to test it, anyway.

CHECK(DCDFile::readHeader() throw())
  DCDFile one(dcd_test_file, File::IN);
	bool test = one.readHeader();
	TEST_EQUAL(test, false)
	one.close();
	DCDFile two("data/INIFile_test.ini", File::IN);
	test = two.readHeader();
	TEST_EQUAL(test, false)
RESULT


CHECK(DCDFile::writeHeader() throw())
	// BAUSTELLE:
	// Oh, how did this temporary file thing work again?
	// DCDFile one(temporary, File::OUT);
	// one.writeHeader();
	// one.close();
	// DCDFile two(temporary, File::IN);
	// bool test = two.readHeader();
	// TEST_EQUAL(test, true);
RESULT


CHECK(DCDFile::append(const SnapShot& snapshot) throw())
  // BAUSTELLE
	// SnapShot snap;
	// DCDFile one(temporary, File::out);
	// one.append(snap);
	// one.close();
	// DCDFile two(temporary, File::IN);
	// SnapShot snap2;
	// two.readHeader();
	// two.read(snap2);
	// bool test = (snap == snap2);
	// TEST_EQUAL(test, true);
RESULT


CHECK(DCDFile::read(SnapShot& snapshot) throw())
  //BAUSTELLE
RESULT


CHECK(DCDFile::flushToDisk(const ::std::vector<SnapShot> buffer) throw())
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
