// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/FORMAT/CCP4File.h>
#include <BALL/DATATYPE/regularData3D.h>

///////////////////////////

START_TEST(CCP4File, "$Id: CCP4File_test.C")

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
	CCP4File f("data/2ovc.ccp4", std::ios::in);
	RegularData3D map;
	f.read(map);
	TEST_EXCEPTION(Exception::FileNotFound, CCP4File f2("2ovc.ccp4"))
RESULT


CHECK(bool read(RegularData3D& map) throw(Exception::ParseError))
	CCP4File f("data/2ovc.ccp4");
	RegularData3D map;
	f.read(map);
	TEST_EQUAL(map.getSize().x, 42)
	TEST_EQUAL(map.getSize().y, 43)
	TEST_EQUAL(map.getSize().z, 78)
	CCP4File f2("CCP4File_test.C");
	bool f2_result = f2.read(map);
	TEST_EQUAL(f2_result, false)
RESULT

//CHECK(bool readHeader() throw())
//RESULT
//
//CHECK(bool readSymmetryRecords() throw())
//RESULT
//
//CHECK(bool writeHeader() throw())
//RESULT
//
//CHECK(bool open(const String& name, File::OpenMode open_mode) throw(Exception::FileNotFound))
//RESULT
//
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
