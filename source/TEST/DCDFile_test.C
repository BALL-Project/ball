// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DCDFile_test.C,v 1.14 2003/07/03 13:20:04 amoll Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/DCDFile.h>
///////////////////////////

START_TEST(DCDFile, "$Id: DCDFile_test.C,v 1.14 2003/07/03 13:20:04 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String dcd_test_file("data/DCD_test.dcd");

DCDFile* p = new DCDFile;

CHECK(DCDFile() throw())
  p = new DCDFile;
	TEST_NOT_EQUAL(p, 0)
RESULT


CHECK(~DCDFile() throw())
	delete p;
RESULT


CHECK(DCDFile(const DCDFile& file) throw(Exception::FileNotFound))
  DCDFile first_file(dcd_test_file);
	DCDFile second_file(first_file);
	bool test = (first_file == second_file);
	TEST_EQUAL(test, true)
RESULT


CHECK(DCDFile(const String& name, File::OpenMode open_mode = std::ios::in) throw())
  DCDFile test_file(dcd_test_file, std::ios::in);
	TEST_EQUAL(test_file.isOpen(), true)
	TEST_EQUAL(test_file.getOpenMode(), std::ios::in)
	test_file.close();
	TEST_EQUAL(test_file.isOpen(), false)

	String filename;
	NEW_TMP_FILE(filename)
	DCDFile test_file2(filename, std::ios::out);
	TEST_EQUAL(test_file2.isOpen(), true)
	TEST_EQUAL(test_file2.getOpenMode(), std::ios::out)
	test_file2.close();
	TEST_EQUAL(test_file2.isOpen(), false)
RESULT


CHECK(const DCDFile& operator = (const DCDFile& file) throw())
  DCDFile one(dcd_test_file, std::ios::in);
	DCDFile two;
	two = one;
	bool test = (one == two);
	TEST_EQUAL(test, true)
RESULT


CHECK(void clear() throw())
	DCDFile file(dcd_test_file, std::ios::in);
	file.clear();
	DCDFile empty;
	bool test = (file == empty);
	TEST_EQUAL(test, true)
RESULT


CHECK(bool operator == (const DCDFile& file) const throw())
  DCDFile one(dcd_test_file, std::ios::in);
  DCDFile two(dcd_test_file, std::ios::in);
	bool test = (one == two);
	TEST_EQUAL(test, true)
	DCDFile three;
	test = (one == three);
	TEST_EQUAL(test, false)
RESULT

// init() is called every time an object is constructed, so we won't have a
// dedicated test here. There is no reasonable way to test it, anyway.

CHECK(bool readHeader() throw())
  DCDFile one(dcd_test_file, std::ios::in);
	bool test = one.readHeader();
	TEST_EQUAL(test, false)
	one.close();
	DCDFile two("data/INIFile_test.ini", std::ios::in);
	test = two.readHeader();
	TEST_EQUAL(test, false)
RESULT


CHECK(bool writeHeader() throw())
	String temporary;
	NEW_TMP_FILE(temporary)
	DCDFile one(temporary, std::ios::out);
	one.writeHeader();
	one.close();
	DCDFile two(temporary, std::ios::in);
	bool test = two.readHeader();
	TEST_EQUAL(test, true);
RESULT


CHECK(bool append(const SnapShot& snapshot) throw())
	System system;
	Molecule mol;
	Atom atom;

	atom.setPosition(Vector3(1.0, 2.0, 3.0));

	mol.insert(atom);
	system.insert(mol);

	SnapShot snap;
	snap.takeSnapShot(system);

	String temporary;
	NEW_TMP_FILE(temporary)
	DCDFile one(temporary, std::ios::out);
	for (Size i = 0; i < 100; i++) one.append(snap);
	one.close();
	DCDFile two(temporary, std::ios::in);
	SnapShot snap2;
	two.read(snap2);
	// ?????
	// DCDFile lacks something like updateHeader(). As long as such a
	// function doesn't exist, appending cannot work.
	// bool test = (snap == snap2);
	// TEST_EQUAL(test, true);
RESULT


CHECK(bool read(SnapShot& snapshot) throw())
  //?????
RESULT


CHECK(bool flushToDisk(const std::vector<SnapShot>& buffer) throw())
  //?????
RESULT

CHECK(BALL_CREATE(DCDFile))
RESULT

CHECK(bool hasVelocities() const throw())
RESULT

CHECK(bool init() throw())
RESULT

CHECK(bool isSwappingBytes() const throw())
RESULT

CHECK(bool open(const String& name, File::OpenMode open_mode = std::ios::in) throw())
RESULT

CHECK(bool seekAndWriteHeader() throw())
RESULT

CHECK(void disableVelocityStorage() throw())
RESULT

CHECK(void enableVelocityStorage() throw())
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
