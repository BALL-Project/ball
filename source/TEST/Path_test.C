// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Path_test.C,v 1.6 2002/12/12 11:34:43 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/path.h>
///////////////////////////

START_TEST(Path, "$Id: Path_test.C,v 1.6 2002/12/12 11:34:43 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Path())
	Path* p = new Path();
	TEST_NOT_EQUAL(p, 0)
RESULT

Path p = Path();
string data_suffix(" data ");
data_suffix[0] = FileSystem::PATH_SEPARATOR;
data_suffix[5] = FileSystem::PATH_SEPARATOR;

CHECK(getDataPath())
	STATUS(p.getDataPath())
	TEST_EQUAL(String(p.getDataPath()).hasSuffix(data_suffix), true)
RESULT

CHECK(setDataPath(const string& path))
	p.setDataPath("XXXXX/XXXXX");
	TEST_EQUAL(p.getDataPath(),"XXXXX/XXXXX")
RESULT

CHECK(addDataPath(const string& path))
	p.addDataPath("XXXXX/XXXXX");
	STATUS(p.getDataPath())
	TEST_EQUAL(String(p.getDataPath()).hasSubstring("XXXXX/XXXXX"), true)
	TEST_EQUAL(String(p.getDataPath()).hasSuffix("XXXXX/XXXXX"), true)
RESULT

CHECK(getDefaultDataPath())
	TEST_EQUAL(String(p.getDefaultDataPath()).hasSuffix(data_suffix), true)
RESULT

CHECK(find(const string& name))
	Path p1;
	TEST_NOT_EQUAL(p1.find("fragments/Fragments.db"), "")
	TEST_EQUAL(p1.find("Fragments.db"), "")
	TEST_EQUAL(p1.find("Path_test.C"), "Path_test.C");
	TEST_EQUAL(p1.find("/TEST/Path_test.C"), "Path_test.C");	
	TEST_EQUAL(p1.find("/xxx/Path_test.C"), "Path_test.C");
	TEST_EQUAL(p1.find("Path_testX.C"), "");
RESULT

CHECK(findStrict(const string& name))
	Path p1;
	TEST_NOT_EQUAL(p1.find("fragments/Fragments.db"), "")
	TEST_EQUAL(p1.find("Fragments.db"), "")
	TEST_EQUAL(String(p1.getDataPath()+"TEST/Path_test.C"), String(p1.getDataPath()+"TEST/Path_test.C"))
	TEST_EQUAL(p1.findStrict("Path_test.C"), "Path_test.C")
	TEST_EQUAL(p1.findStrict("/TEST/Path_test.C"), "");
	TEST_EQUAL(p1.findStrict("/xxx/Path_test.C"), "");
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
