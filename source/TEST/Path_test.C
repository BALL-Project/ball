// $Id: Path_test.C,v 1.4 2001/07/15 17:32:40 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/path.h>
///////////////////////////

START_TEST(Path, "$Id: Path_test.C,v 1.4 2001/07/15 17:32:40 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Path())
	Path* p = new Path();
	TEST_NOT_EQUAL(p, 0)
RESULT

Path p = Path();

CHECK(getDataPath())
	TEST_EQUAL(String(p.getDataPath()).hasSuffix("/BALL/data/"), true)
RESULT

CHECK(setDataPath(const string& path))
	p.setDataPath("XXXXX/XXXXX");
	TEST_EQUAL(p.getDataPath(),"XXXXX/XXXXX")
RESULT

CHECK(addDataPath(const string& path))
	p.setDataPath("XXXXX/XXXXX");
	TEST_EQUAL(String(p.getDataPath()).hasSubstring("XXXXX/XXXXX"), true)
	TEST_EQUAL(String(p.getDataPath()).hasSuffix("XXXXX/XXXXX"), true)
RESULT

CHECK(getDefaultDataPath())
	TEST_EQUAL(String(p.getDefaultDataPath()).hasSuffix("/BALL/data/"), true)
RESULT

CHECK(find(const string& name))
	TEST_EQUAL(p.find("Path_test.C"), "Path_test.C");	// <<<<<<<<<
	TEST_EQUAL(p.find("/TEST/Path_test.C"), "Path_test.C");	// <<<<<<<<<
	TEST_EQUAL(p.find("/xxx/Path_test.C"), "Path_test.C");	// <<<<<<<<<
	TEST_EQUAL(p.find("Path_testX.C"), "");
RESULT

CHECK(findStrict(const string& name))
	TEST_EQUAL(String(p.getDataPath()+"TEST/Path_test.C"), String(p.getDataPath()+"TEST/Path_test.C"))
	TEST_EQUAL(p.findStrict("Path_test.C"), "Path_test.C")
	TEST_EQUAL(p.findStrict("/TEST/Path_test.C"), "");
	TEST_EQUAL(p.findStrict("/xxx/Path_test.C"), "");
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
