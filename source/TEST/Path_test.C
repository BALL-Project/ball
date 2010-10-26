// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: Path_test.C,v 1.12.30.3 2007/06/21 19:44:53 oliver Exp $

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/SYSTEM/path.h>

///////////////////////////

START_TEST(Path, "$Id: Path_test.C,v 1.12.30.3 2007/06/21 19:44:53 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

#ifdef BALL_COMPILER_MSVC
# define chdir _chdir
#endif

::chdir(BALL_TEST_DATA_PATH(..));

CHECK(Path())
	Path* p = new Path();
	TEST_NOT_EQUAL(p, 0)
	delete p;
RESULT

Path p;
String data_suffix1("/data/");
String data_suffix2("/data/");
data_suffix2[0] = FileSystem::PATH_SEPARATOR;
data_suffix2[5] = FileSystem::PATH_SEPARATOR;

String x_test("XXXXX");
x_test += FileSystem::PATH_SEPARATOR + "XXXXX";

String x_test_quoted(x_test + FileSystem::PATH_SEPARATOR);

CHECK(string getDataPath())
	STATUS(p.getDataPath())
	TEST_EQUAL(String(p.getDataPath()).hasSuffix(data_suffix1)
	 		|| String(p.getDataPath()).hasSuffix(data_suffix2), true)
RESULT

CHECK(void setDataPath(const string& path))
	p.setDataPath(x_test);
	TEST_EQUAL(p.getDataPath(), x_test_quoted);
RESULT

CHECK(void addDataPath(const string& path))
	p.addDataPath(x_test);
	STATUS(p.getDataPath())
	TEST_EQUAL(String(p.getDataPath()).hasSubstring(x_test_quoted), true)
	TEST_EQUAL(String(p.getDataPath()).hasSuffix(x_test_quoted), true)
RESULT

CHECK(string getDefaultDataPath())
	TEST_EQUAL(String(p.getDefaultDataPath()).hasSuffix(data_suffix1)
			|| String(p.getDefaultDataPath()).hasSuffix(data_suffix2), true)
RESULT

CHECK(string find(const string& name))
	Path p1;
	p1.reset();
	String file = String("fragments") + FileSystem::PATH_SEPARATOR + "Fragments.db";
	TEST_NOT_EQUAL(p1.find(file), "")
	file = "Fragments.db";
	TEST_EQUAL(p1.find(file), "")
	file = "Path_test.C";
	TEST_EQUAL(p1.find(file), "Path_test.C");
	file = String("TEST") + FileSystem::PATH_SEPARATOR + "Path_test.C";
	TEST_EQUAL(p1.find(file), "Path_test.C");	
	file = String("xxx") + FileSystem::PATH_SEPARATOR + "Path_test.C";
	TEST_EQUAL(p1.find(file), "Path_test.C");
	file = "Path_testX.C";
	TEST_EQUAL(p1.find(file), "");
RESULT

CHECK(string findStrict(const string& name))
	Path p1;
	TEST_NOT_EQUAL(p1.find("fragments/Fragments.db"), "")
	TEST_EQUAL(p1.find("Fragments.db"), "")
	TEST_EQUAL(String(p1.getDataPath()+"TEST/Path_test.C"), String(p1.getDataPath()+"TEST/Path_test.C"))
	TEST_EQUAL(p1.findStrict("Path_test.C"), "Path_test.C")
	TEST_EQUAL(p1.findStrict("/TEST/Path_test.C"), "");
	TEST_EQUAL(p1.findStrict("/xxx/Path_test.C"), "");
RESULT

CHECK([extra]Singleton)
	Path p1;
	Path p2;
	p1.reset();
	STATUS(p1.getDataPath())
	TEST_EQUAL(String(p1.getDataPath()).hasSuffix(data_suffix1)
	 		|| String(p1.getDataPath()).hasSuffix(data_suffix2), true)
	p1.setDataPath(x_test);
	TEST_EQUAL(p1.getDataPath(), x_test_quoted)
	TEST_EQUAL(p2.getDataPath(), x_test_quoted)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
