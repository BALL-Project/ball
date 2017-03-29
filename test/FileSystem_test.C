// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/SYSTEM/fileSystem.h>

///////////////////////////

START_TEST(FileSystem)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

const String& PS = FileSystem::PATH_SEPARATOR;
CHECK(canonizePath(String& path) -- expansion of tilde)
	String path("~/test.dat");
	FileSystem::canonizePath(path);
	TEST_EQUAL(path.hasSuffix("/test.dat"), true)
#ifdef BALL_COMPILER_MSVC
	TEST_EQUAL(path[0],'C')
#else
	TEST_EQUAL(path[0], FileSystem::PATH_SEPARATOR)
#endif
RESULT

CHECK(canonizePath(String& path) -- removal of duplicate path separators)
	String path("//");
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "//");
	path = "/usr//local";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "/usr/local");
	path = PS + "usr" + PS + "local" + PS + PS;
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "/usr/local/")
RESULT

CHECK(canonizePath(String& path) -- expansion of relative paths)
	String path(PS + "usr" + PS + "local" + PS + ".." + PS + "bin" + PS);
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "/usr/bin/")
	path = String("..") + PS + ".." + PS;
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, String("../../"))
	path = "..";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, "..")
	path = String("..") + PS + "..";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, String("../.."))
	path = String("..") + PS + "." + PS + "..";
	FileSystem::canonizePath(path);
	TEST_EQUAL(path, String("../.."))
RESULT

CHECK(baseName(const String& filename))
	String filename = PS + "test" + PS + PS + "TEST" + PS + "basename.sfx";
	TEST_EQUAL(FileSystem::baseName(filename), "basename.sfx")
	TEST_EQUAL(FileSystem::baseName(PS), "")
	TEST_EQUAL(FileSystem::baseName(PS + "a"), "a")
	TEST_EQUAL(FileSystem::baseName(""), "")
	TEST_EQUAL(FileSystem::baseName("test"), "test")
RESULT

CHECK(fileExtension(const String& filename))
	String filename = "test" + PS + "basename.sfx";
	TEST_EQUAL(FileSystem::fileExtension(filename), "sfx")
	TEST_EQUAL(FileSystem::fileExtension(PS), "")
	TEST_EQUAL(FileSystem::fileExtension(PS + "."), "")
	TEST_EQUAL(FileSystem::fileExtension(""), "")
	TEST_EQUAL(FileSystem::fileExtension("test"), "")
RESULT

CHECK(path(const String& filename))
	String filename = PS + "test" + PS + PS + "TEST" + PS + "basename.sfx";
	TEST_EQUAL(FileSystem::path(filename), PS + "test" + PS + PS + "TEST" + PS)
	TEST_EQUAL(FileSystem::path(PS), PS)
	TEST_EQUAL(FileSystem::path(""), "")
	TEST_EQUAL(FileSystem::path("test"), "")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
