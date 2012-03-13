// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/FORMAT/CCP4File.h>
#include <BALL/DATATYPE/regularData3D.h>

///////////////////////////

START_TEST(CCP4File)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CCP4File* cf = 0;
CHECK(CCP4File() throw())
	cf = new CCP4File;
	TEST_NOT_EQUAL(cf, 0)
RESULT


CHECK(~CCP4File() throw())
	delete cf;
RESULT


CHECK(CCP4File(const String& filename, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	CCP4File f(BALL_TEST_DATA_PATH(2ovc.ccp4), std::ios::in);
	RegularData3D map;
	f.read(map);
	TEST_EXCEPTION(Exception::FileNotFound, CCP4File f2("2ovc.ccp4"))
RESULT


CHECK(bool open(const String& name, File::OpenMode open_mode) throw(Exception::FileNotFound))
	CCP4File f5;
	bool f5_result = f5.open(BALL_TEST_DATA_PATH(2ovc.ccp4), std::ios::in | std::ios::binary);
	TEST_EQUAL(f5_result, true)
	CCP4File f6;
	TEST_EXCEPTION(Exception::FileNotFound, f6.open(BALL_TEST_DATA_PATH(file_does_not.exist), std::ios::in | std::ios::binary));
	CCP4File f7;
	bool f7_result = f7.open(BALL_TEST_DATA_PATH(AG.pdb), std::ios::binary);
	TEST_EQUAL(f7_result, false)
RESULT

CHECK(bool readHeader())
	CCP4File f3(BALL_TEST_DATA_PATH(2ovc.ccp4));
	bool f3_result = f3.readHeader();
	TEST_EQUAL(f3_result, true)
RESULT

CHECK(bool readSymmetryRecords())
	CCP4File f4(BALL_TEST_DATA_PATH(2ovc.ccp4));
	bool f4_result = f4.readSymmetryRecords();
	TEST_EQUAL(f4_result, true)
RESULT

CHECK(bool read(RegularData3D& map) throw(Exception::ParseError))
	CCP4File f(BALL_TEST_DATA_PATH(2ovc.ccp4));
	RegularData3D map;
	f.read(map);
	TEST_EQUAL(map.getSize().x, 42)
	TEST_EQUAL(map.getSize().y, 43)
	TEST_EQUAL(map.getSize().z, 78)
	CCP4File f2(BALL_TEST_DATA_PATH(AG.pdb));
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
