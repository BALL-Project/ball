// $Id: Directory_test.C,v 1.2 2000/06/21 10:35:37 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/directory.h>
///////////////////////////

START_TEST(class_name, "$Id: Directory_test.C,v 1.2 2000/06/21 10:35:37 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Directory* dd;
String testDir = ::getcwd(NULL, 64);

CHECK(Directory::Directory())
	dd = new Directory();
	TEST_NOT_EQUAL(dd, 0)
	TEST_EQUAL(dd->getPath(), String(::getcwd(NULL, 64)))
	TEST_EQUAL(dd->isCurrent(), true)
RESULT

CHECK(Directory::~Directory())
	delete dd;
RESULT

Directory d;

CHECK(Directory::setCurrent(const String& path))
	TEST_EQUAL(d.setCurrent("data"), true);
	TEST_EQUAL(::getcwd(NULL, 64), testDir + "/data")
	TEST_EQUAL(d.setCurrent(testDir), true);
	TEST_EQUAL(::getcwd(NULL, 64), testDir)
RESULT

CHECK(Directory::Directory(const String& directory_path, bool set_current = false))
	Directory d("data");
	TEST_EQUAL(d.isValid(), true)
	String s = String(::getcwd(NULL, 64)) + "/data";
	TEST_EQUAL(d.getPath(), s)

	Directory d1("data/", true);
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d.setCurrent(testDir), true);

	Directory d2("data/Directory_test/");
	TEST_EQUAL(d2.isValid(), true)

	Directory d3(testDir);
	TEST_EQUAL(d3.isValid(), true)

	Directory d5("data/Directory_test1");
	TEST_EQUAL(d5.isValid(), false)

RESULT

CHECK(Directory::Directory(const Directory& directory))
	Directory d("data/");
	Directory d1(d);
	TEST_EQUAL(d1 == d, true)
RESULT

CHECK(Directory::clear())
	Directory d("data/");
	d.clear();
	TEST_EQUAL(d.getPath(), "")
RESULT

CHECK(Directory::destroy())
	Directory d1("data/");
	d1.destroy();
RESULT

CHECK(Directory::set(const String& directory_path, bool set_current = false))
	Directory d;
	d.set("data/Directory_test/");
	TEST_EQUAL(d.getPath(), testDir + "/data/Directory_test")
	TEST_EQUAL(d.isCurrent(), false)
	d.set("data/Directory_test/", true);
	TEST_EQUAL(d.isCurrent(), true)
	d.setCurrent(testDir);
RESULT

CHECK(Directory::set(const Directory& directory))
	Directory d("data/");
	Directory d1;
	d1.set(d);
	TEST_EQUAL(d1.getPath(), testDir + "/data")
RESULT

CHECK(Directory::Directory& operator = (const Directory& directory))
	Directory d("data/");
	Directory d1;
	d1 = d;
	TEST_EQUAL(d1.getPath(), testDir + "/data")	
RESULT

CHECK(Directory::get(Directory& directory) const )
	Directory d("data/");
	Directory d1;
	d.get(d1);
	TEST_EQUAL(d1.getPath(), testDir + "/data")
RESULT

CHECK(Directory::swap(Directory& directory))
	Directory d1("data/");
	Directory d2("data/Directory_test/", true);
	d1.swap(d2);
	TEST_EQUAL(d1.getPath(), testDir + "/data/Directory_test")
	TEST_EQUAL(d2.getPath(), testDir + "/data")
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d2.isCurrent(), false)
	d.setCurrent(testDir);
RESULT

CHECK(Directory::getPath() const )
	Directory d1("data/");
	TEST_EQUAL(d1.getPath(), testDir + "/data")
RESULT

CHECK(Directory::create(String& path, mode_t mode = 0777))
	Directory d1("data/Directory_test/", true);
	TEST_EQUAL(d1.isValid(), true)
	d1.remove("test1");
	d1.remove("test2");
	d1.remove("test3");
	TEST_EQUAL(d1.create("test1"), true)
	TEST_EQUAL(d.setCurrent(testDir), true)
	Directory d2;
	TEST_EQUAL(d2.create("data/Directory_test/test2"), true)
	TEST_EQUAL(d2.isCurrent(), true)
	TEST_EQUAL(d.setCurrent(testDir), true)
RESULT

CHECK(Directory::remove(String old_path))
	Directory d1("data/Directory_test/", true);
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d1.remove("test1"), true)
	TEST_EQUAL(d1.remove("test2"), true)
	d.setCurrent(testDir);
RESULT

CHECK(Directory::rename(String old_path, String new_path))
	Directory d1("data/Directory_test", true);
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d1.create("test1"), true)
	TEST_EQUAL(d1.rename("test1", "test2"), true)	  
	TEST_EQUAL(d1.rename("test2", "test3"), true)	  
	TEST_EQUAL(d1.rename("testz", "test"), false)	  
	TEST_EQUAL(d1.remove("test3"), true)
	d.setCurrent(testDir);
RESULT

CHECK(Directory::renameTo(const String& new_path))
	Directory d1("data/Directory_test");
	TEST_EQUAL(d1.create("test1"), true)
	d.setCurrent(testDir);
	Directory d2("data/Directory_test/test1", true);
	TEST_EQUAL(d2.isValid(), true)
	TEST_EQUAL(d2.renameTo("test2"), true)	  
	TEST_EQUAL(d1.remove("test2"), true)
	d.setCurrent(testDir);
RESULT

Directory d1("data/Directory_test/");

CHECK(Directory::getFirstEntry(String& entry) const )
	String s;
	TEST_EQUAL(d1.getFirstEntry(s), true)
	TEST_EQUAL(s, ".");
	TEST_EQUAL(d1.getFirstEntry(s), true)
	TEST_EQUAL(s, ".");
RESULT

CHECK(Directory::getNextEntry(String& entry))
	String s;
	TEST_EQUAL(d1.getNextEntry(s), true)
	TEST_EQUAL(s, "..");
	TEST_EQUAL(d1.getNextEntry(s), true)
	TEST_EQUAL(s, "CVS");
	TEST_EQUAL(d1.getNextEntry(s), true)
	TEST_EQUAL(s, "a");
RESULT
 
CHECK(Directory::countItems() const )
	TEST_EQUAL(d1.countItems(), 3)
RESULT

CHECK(Directory::countFiles() const )
	TEST_EQUAL(d1.countFiles(), 2)
RESULT

CHECK(Directory::countDirectories() const )
	TEST_EQUAL(d1.countDirectories(), 1)
RESULT

CHECK(Directory::find(const String& filename, String& filepath))
	Directory d1("data/");
	String s;
	TEST_EQUAL(d1.find("Directory_test", s), true)
	TEST_EQUAL(s, testDir + "/data");
	d1.set(testDir);
	TEST_EQUAL(d1.find("a", s), true)
	TEST_EQUAL(s, testDir + "/data/Directory_test")
RESULT

CHECK(Directory::has(const String& filename, bool recursive = false) const )
	Directory d1("data/Directory_test/");
	TEST_EQUAL(d1.has("a"), true)
	TEST_EQUAL(d1.has("x"), false)
RESULT

CHECK(Directory::isCurrent() const )
	Directory d1("data/Directory_test/");
	TEST_EQUAL(d1.isCurrent(), false)
	d1.setCurrent();
	TEST_EQUAL(d1.isCurrent(), true)
	d.setCurrent(testDir);
RESULT

CHECK(Directory::isEmpty() const )
	Directory d1("data/Directory_test/");
	d1.create("xxx");
	Directory d2("data/Directory_test/xxx");
	TEST_EQUAL(d2.isEmpty(), true)
	d1.remove("xxx");
	TEST_EQUAL(d1.isEmpty(), false)
RESULT

CHECK(Directory::bool operator == (const Directory& directory) const )
	Directory d("data/");
	Directory d1(d);
	TEST_EQUAL(d == d1, true)
	d1.set("data/Directory_test/");
	TEST_EQUAL(d == d1, false)
RESULT

CHECK(Directory::bool operator != (const Directory& directory) const )
	Directory d("data/");
	Directory d1(d);
	TEST_EQUAL(d != d1, false)
	d1.set("data/Directory_test/");
	TEST_EQUAL(d != d1, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
