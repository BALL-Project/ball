// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: DCDFile_test.C,v 1.8.2.1 2003/01/07 13:22:21 anker Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/FORMAT/DCDFile.h>

///////////////////////////

START_TEST(DCDFile, "$Id: DCDFile_test.C,v 1.8.2.1 2003/01/07 13:22:21 anker Exp $")

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


CHECK(DCDFile::DCDFile& operator = (const DCDFile& file) throw())
  DCDFile one(dcd_test_file, std::ios::in);
	DCDFile two;
	two = one;
	bool test = (one == two);
	TEST_EQUAL(test, true)
RESULT


CHECK(DCDFile::clear() throw())
	DCDFile file(dcd_test_file, std::ios::in);
	file.clear();
	DCDFile empty;
	bool test = (file == empty);
	TEST_EQUAL(test, true)
RESULT


CHECK(DCDFile::bool operator == (const DCDFile& file) throw())
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

CHECK(DCDFile::readHeader() throw())
  DCDFile one(dcd_test_file, std::ios::in);
	bool test = one.readHeader();
	TEST_EQUAL(test, false)
	one.close();
	DCDFile two("data/INIFile_test.ini", std::ios::in);
	test = two.readHeader();
	TEST_EQUAL(test, false)
RESULT


CHECK(DCDFile::writeHeader() throw())
	String temporary;
	NEW_TMP_FILE(temporary)
	DCDFile one(temporary, std::ios::out);
	one.writeHeader();
	one.close();
	DCDFile two(temporary, std::ios::in);
	bool test = two.readHeader();
	TEST_EQUAL(test, true);
RESULT


CHECK(DCDFile::append(const SnapShot& snapshot) throw())
	Atom atom;
	::std::vector<Vector3> atom_positions;
	atom_positions.push_back(atom.getPosition());
	SnapShot snap;
	snap.setAtomPositions(atom_positions);
	String temporary;
	NEW_TMP_FILE(temporary)
	DCDFile one(temporary, File::out);
	one.writeHeader();
	for (Size i = 0; i < 100; i++) one.append(snap);
	// one.updateHeader();
	one.close();
	DCDFile two(temporary, std::ios::in);
	SnapShot snap2;
	two.readHeader();
	two.read(snap2);
	// ?????
	// DCDFile lacks something like updateHeader(). As long as such a
	// function doesn't exist, appending cannot work.
	// bool test = (snap == snap2);
	// TEST_EQUAL(test, true);
RESULT


CHECK(DCDFile::read(SnapShot& snapshot) throw())
  //?????
RESULT


CHECK(DCDFile::flushToDisk(const ::std::vector<SnapShot> buffer) throw())
  //?????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
