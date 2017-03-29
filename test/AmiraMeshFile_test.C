// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/amiraMeshFile.h>
#include <BALL/DATATYPE/regularData3D.h>

///////////////////////////

START_TEST(AmiraMeshFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

AmiraMeshFile* cf = 0;
CHECK(AmiraMeshFile() throw())
	cf = new AmiraMeshFile;
	TEST_NOT_EQUAL(cf, 0)
RESULT


CHECK(~AmiraMeshFile() throw())
	delete cf;
RESULT


CHECK(AmiraMeshFile(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	AmiraMeshFile f(BALL_TEST_DATA_PATH(test_ascii.am), std::ios::in);
	RegularData3D map;
	f.read(map);
	TEST_EXCEPTION(Exception::FileNotFound, AmiraMeshFile f2("test_ascii.am"))
RESULT


CHECK(bool open(const String& name, File::OpenMode open_mode) throw(Exception::FileNotFound))
	AmiraMeshFile f5;
	bool f5_result = f5.open(BALL_TEST_DATA_PATH(test_ascii.am), std::ios::in | std::ios::binary);
	TEST_EQUAL(f5_result, true)
	AmiraMeshFile f6;
	TEST_EXCEPTION(Exception::FileNotFound, f6.open(BALL_TEST_DATA_PATH(file_does_not.exist), std::ios::in | std::ios::binary));
	AmiraMeshFile f7;
	bool f7_result = f7.open(BALL_TEST_DATA_PATH(AG.pdb), std::ios::binary);
	TEST_EQUAL(f7_result, false)
RESULT

CHECK(bool readHeader())
	AmiraMeshFile f3(BALL_TEST_DATA_PATH(test_ascii.am));
	bool f3_result = f3.readHeader();
	TEST_EQUAL(f3_result, true)
RESULT

CHECK(bool read(RegularData3D& map) throw(Exception::ParseError))
	AmiraMeshFile f(BALL_TEST_DATA_PATH(test_ascii.am));
	RegularData3D map;
	f.read(map);
	TEST_EQUAL(map.getSize().x, 78)
	TEST_EQUAL(map.getSize().y, 38)
	TEST_EQUAL(map.getSize().z, 38)
	AmiraMeshFile f2(BALL_TEST_DATA_PATH(AG.pdb));
	bool f2_result = f2.read(map);
	TEST_EQUAL(f2_result, false)
RESULT

//
//CHECK(bool writeHeader() throw())
//RESULT
//
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
