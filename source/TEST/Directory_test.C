// $Id: Directory_test.C,v 1.4 2000/07/04 08:53:27 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/directory.h>
///////////////////////////

START_TEST(class_name, "$Id: Directory_test.C,v 1.4 2000/07/04 08:53:27 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Directory* dd;
String test_dir;
CHECK(prerequisites)
	test_dir = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
	test_dir += "/data/Directory_test";
	TEST_EQUAL(::chdir(test_dir.c_str()), 0)
	test_dir = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
RESULT

CHECK(Directory::Directory())
	dd = new Directory();
	TEST_NOT_EQUAL(dd, 0)
	TEST_EQUAL(dd->getPath(), String(::getcwd(NULL, Directory::MAX_PATH_LENGTH)))
	TEST_EQUAL(dd->isCurrent(), true)
RESULT

CHECK(Directory::~Directory())
	delete dd;
RESULT

Directory d;

CHECK(Directory::setCurrent(const String& path))
	bool result = d.setCurrent(test_dir);
	TEST_EQUAL(result, true);
	String path  = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
	TEST_EQUAL(path, test_dir)
	result = d.setCurrent("dir_a");
	TEST_EQUAL(result, true);
	path = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
	TEST_EQUAL(path, test_dir + "/dir_a")
	result = d.setCurrent("c");
	TEST_EQUAL(result, false);
	path = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
	TEST_EQUAL(path, test_dir + "/dir_a")
RESULT

CHECK(Directory::Directory(const String& directory_path, bool set_current = false))
	::chdir(test_dir.c_str());
	Directory d("dir_a");
	TEST_EQUAL(d.isValid(), true)
	String s = String(::getcwd(NULL, Directory::MAX_PATH_LENGTH)) + "/dir_a";
	TEST_EQUAL(d.getPath(), s)

	Directory d1("dir_a/", true);
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d.setCurrent(test_dir), true);

	Directory d2("dir_a/dir_c/");
	TEST_EQUAL(d2.isValid(), true)

	Directory d3(test_dir);
	TEST_EQUAL(d3.isValid(), true)

	Directory d5("dir_f");
	TEST_EQUAL(d5.isValid(), false)
RESULT

CHECK(Directory::Directory(const Directory& directory))
	Directory d("dir_a/");
	Directory d1(d);
	TEST_EQUAL(d1 == d, true)
RESULT

CHECK(Directory::clear())
	Directory d("dir_a/");
	d.clear();
	TEST_EQUAL(d.getPath(), "")
RESULT

CHECK(Directory::destroy())
	Directory d1("dir_a/");
	d1.destroy();
RESULT

CHECK(Directory::set(const String& directory_path, bool set_current = false))
	Directory d;
	d.set("dir_a/dir_c/");
	TEST_EQUAL(d.getPath(), test_dir + "/dir_a/dir_c")
	TEST_EQUAL(d.isCurrent(), false)
	d.set("dir_a/dir_c/", true);
	TEST_EQUAL(d.isCurrent(), true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::set(const Directory& directory))
	Directory d("dir_a/");
	Directory d1;
	d1.set(d);
	TEST_EQUAL(d1.getPath(), test_dir + "/dir_a")
RESULT

CHECK(Directory::Directory& operator = (const Directory& directory))
	Directory d("dir_a/");
	Directory d1;
	d1 = d;
	TEST_EQUAL(d1.getPath(), test_dir + "/dir_a")	
RESULT

CHECK(Directory::get(Directory& directory) const )
	Directory d("dir_a/");
	Directory d1;
	d.get(d1);
	TEST_EQUAL(d1.getPath(), test_dir + "/dir_a")
RESULT

CHECK(Directory::swap(Directory& directory))
	Directory d1("dir_a/");
	Directory d2("dir_a/dir_c/", true);
	d1.swap(d2);
	TEST_EQUAL(d1.getPath(), test_dir + "/dir_a/dir_c")
	TEST_EQUAL(d2.getPath(), test_dir + "/dir_a")
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d2.isCurrent(), false)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::getPath() const )
	Directory d1("dir_a/");
	TEST_EQUAL(d1.getPath(), test_dir + "/dir_a")
RESULT

CHECK(Directory::create(String& path, mode_t mode = 0777))
	::chdir(test_dir.c_str());
	Directory d1("dir_a/dir_c/", true);
	TEST_EQUAL(d1.isValid(), true)
	d1.remove("test1");
	d1.remove("test2");
	d1.remove("test3");
	bool result = d1.create("test1");
	TEST_EQUAL(result, true)
	TEST_EQUAL(d.setCurrent(test_dir), true)
	Directory d2;
	result = d2.create("dir_a/dir_c/test2");
	TEST_EQUAL(result, true)
	TEST_EQUAL(d2.isCurrent(), true)
	TEST_EQUAL(d.setCurrent(test_dir), true)
	Directory d3;
	TEST_EQUAL(d3.create(test_dir + "/dir_a/dir_c/test3"), true)
	d3.remove(test_dir + "/dir_a/dir_c/test3");
	d3.remove(test_dir + "/dir_a/dir_c/test2");
	d3.remove(test_dir + "/dir_a/dir_c/test1");
RESULT

CHECK(Directory::getNextEntry(String& entry))
	::chdir(String(test_dir + "/dir_a/dir_c").c_str());
	Directory d1(".", true);
	TEST_EQUAL(d1.isValid(), true)
	d1.create("test1");
	d1.create("test1/test2");

	Directory d2("test1");
	String s;	
	bool result = d2.getNextEntry(s);
	TEST_EQUAL(result, true)
	TEST_EQUAL(s, ".");
	result = d2.getNextEntry(s);
	TEST_EQUAL(result, true)
 	TEST_EQUAL(s, "..")
	result = d2.getNextEntry(s);
	TEST_EQUAL(s, "test2");
	TEST_EQUAL(result, true)
	result = d2.getNextEntry(s);
	TEST_EQUAL(result, false)
	d1.remove("test1/test2");
	d1.remove("test1");
RESULT


Directory d0(test_dir + "/dir_a/dir_c");
d0.create("test1");
Directory d1(test_dir + "/dir_a/dir_c/test1");
d1.create("test1");
d1.create("test2");
d1.create("test3");
d1.create("test4");
CHECK(Directory::countItems() const)
	TEST_EQUAL(d1.countItems(), 4)
RESULT

CHECK(Directory::countFiles() const)
	TEST_EQUAL(d1.countFiles(), 0)
RESULT

CHECK(Directory::countDirectories() const )
	TEST_EQUAL(d1.countDirectories(), 4)
RESULT
d1.remove("test1");
d1.remove("test2");
d1.remove("test3");
d1.remove("test4");
d0.remove("test1");


CHECK(Directory::remove(String old_path))
	::chdir(test_dir.c_str());
	Directory d1("dir_a/dir_c/", true);
	d1.create("test1");
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d1.remove("test1"), true)
	TEST_EQUAL(d1.remove("xxxx"), false)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::rename(String old_path, String new_path))
	Directory d1("dir_a/dir_c", true);
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d1.create("test1"), true)
	TEST_EQUAL(d1.rename("test1", "test2"), true)	  
	TEST_EQUAL(d1.rename("test2", "test3"), true)	  
	TEST_EQUAL(d1.rename("testz", "test"), false)	  
	TEST_EQUAL(d1.remove("test3"), true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::renameTo(const String& new_path))
	Directory d1("dir_a/dir_c");
	TEST_EQUAL(d1.create("test1"), true)
	d.setCurrent(test_dir);
	Directory d2("dir_a/dir_c/test1", true);
	TEST_EQUAL(d2.isValid(), true)
	bool result = d2.renameTo("test2");
	TEST_EQUAL(result, true)	  
	result = d1.remove("test2");
	TEST_EQUAL(result, true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::getFirstEntry(String& entry) const )
	String s;
	d0.create("test1");
	TEST_EQUAL(d1.getFirstEntry(s), true)
	TEST_EQUAL(s, ".");
	TEST_EQUAL(d1.getFirstEntry(s), true)
	TEST_EQUAL(s, ".");
	d0.remove("test1");
RESULT

CHECK(Directory::find(const String& filename, String& filepath))
	Directory d1(test_dir + "/dir_a");
	String s;
	bool result = d1.find("dir_c", s);
	TEST_EQUAL(result, true)
	TEST_EQUAL(s, test_dir + "/dir_a");
	d1.set(test_dir);
	result = d1.find("a", s);
	TEST_EQUAL(result, true)
	TEST_EQUAL(s, test_dir)
	result = d1.find("f", s);
	TEST_EQUAL(result, false)
RESULT

CHECK(Directory::has(const String& filename, bool recursive = false) const )
	Directory d1("dir_a/dir_c/");
	TEST_EQUAL(d1.has("a"), true)
	TEST_EQUAL(d1.has("x"), false)
RESULT

CHECK(Directory::isCurrent() const )
	Directory d1("dir_a/dir_c/");
	TEST_EQUAL(d1.isCurrent(), false)
	d1.setCurrent();
	TEST_EQUAL(d1.isCurrent(), true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::isEmpty() const )
	Directory d1(test_dir + "/dir_a/dir_c");
	d1.create("xxx");
	Directory d2(test_dir + "/dir_a/dir_c/xxx");
	TEST_EQUAL(d2.isEmpty(), true)
	d1.remove("xxx");
	TEST_EQUAL(d1.isEmpty(), false)
RESULT

CHECK(Directory::bool operator == (const Directory& directory) const )
	Directory d("data/");
	Directory d1(d);
	TEST_EQUAL(d == d1, true)
	d1.set("dir_a/dir_c/");
	TEST_EQUAL(d == d1, false)
RESULT

CHECK(Directory::bool operator != (const Directory& directory) const )
	Directory d("data/");
	Directory d1(d);
	TEST_EQUAL(d != d1, false)
	d1.set("dir_a/dir_c/");
	TEST_EQUAL(d != d1, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
