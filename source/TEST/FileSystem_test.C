// $Id: FileSystem_test.C,v 1.2 2001/07/31 00:49:56 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/fileSystem.h>

///////////////////////////

START_TEST(FileSystem, "$Id: FileSystem_test.C,v 1.2 2001/07/31 00:49:56 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(canonizePath(String& path) -- expansion of tilde)
	String path("~/test.dat");
	FileSystem::canonizePath(path);
	TEST_EQUAL(path.hasSuffix("/test.dat"), true)
	TEST_EQUAL(path[0], FileSystem::PATH_SEPARATOR)
RESULT

CHECK(canonizePath(String& path) -- removal of duplicate path separators)
	String path("//");
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "/")
	path = "/usr//local//";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "/usr/local/")
RESULT

CHECK(canonizePath(String& path) -- expansion of relative paths)
	String path("/usr/local/../bin/");
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "/usr/bin/")
	path = "../../";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "../../")
	path = "..";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "..")
	path = "../..";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "../..")
	path = ".././..";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "../..")
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
