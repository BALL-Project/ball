// $Id: Directory_test.C,v 1.1 2000/06/15 19:14:00 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/directory.h>
///////////////////////////

START_TEST(class_name, "$Id: Directory_test.C,v 1.1 2000/06/15 19:14:00 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Directory* d1;

CHECK(Directory::Directory())
	d1 = new Directory();
	TEST_NOT_EQUAL(d1, 0)
RESULT

CHECK(Directory::~Directory())
	delete d1;
RESULT

CHECK(Directory::Directory(const String& directory_path, bool set_current = false))
	Directory d("data/");
	TEST_EQUAL(d.getPath(), "data/")
	d = Directory("data/", true);
	TEST_EQUAL(d.isCurrent(), true)
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
	TEST_EQUAL(d.getPath(), "data/Directory_test/")
	TEST_EQUAL(d.isCurrent(), false)
	d.set("data/Directory_test/", true);
	TEST_EQUAL(d.isCurrent(), true)
RESULT

CHECK(Directory::set(const Directory& directory))
	Directory d("data/");
	Directory d1;
	d1.set(d);
	TEST_EQUAL(d1.getPath(), "data/")
RESULT

CHECK(Directory::Directory& operator = (const Directory& directory))
	Directory d("data/");
	Directory d1;
	d1 = d;
	TEST_EQUAL(d1.getPath(), "data/")	
RESULT

CHECK(Directory::get(Directory& directory) const )
	Directory d("data/");
	Directory d1;
	d.get(d1);
	TEST_EQUAL(d1.getPath(), "data/")
RESULT

CHECK(Directory::swap(Directory& directory))
	Directory d1("data/");
	Directory d2("data/Directory_test/", true);
	d1.swap(d2);
	TEST_EQUAL(d1.getPath(), "data/Directory_test/")
	TEST_EQUAL(d2.getPath(), "data/")
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d2.isCurrent(), false)
RESULT

CHECK(Directory::getPath() const )
	Directory d1("data/");
	TEST_EQUAL(d1.getPath(), "data/")
RESULT

CHECK(Directory::create(String& path, mode_t mode = 0777))
	Directory d1("data/");
	TEST_EQUAL(d1.create("Directory_test/test1"), true)
RESULT

CHECK(Directory::rename(String old_path, String new_path))
	Directory d1("data/");
	TEST_EQUAL(d1.rename("Directory_test/test1", "Directory_test/test2"), true)	  
RESULT

CHECK(Directory::renameTo(const String& new_path))
	Directory d1("data/Directory_test/test2");
	TEST_EQUAL(d1.renameTo("Directory_test/test1"), true)	  
RESULT

CHECK(Directory::setCurrent(String& directory_path))
	Directory d1("data/Directory_test/test1");
	TEST_EQUAL(d1.setCurrent("Directory_test/test1"), true)
	TEST_EQUAL(d1.isCurrent(), true)	
RESULT

CHECK(Directory::setCurrent())
	Directory d1("data/Directory_test/test1");
	TEST_EQUAL(d1.setCurrent("Directory_test/"), true)
	TEST_EQUAL(d1.setCurrent(), true)
	TEST_EQUAL(d1.isCurrent(), true)	
RESULT

CHECK(Directory::remove(String old_path))
	Directory d1("data/Directory_test/");
	TEST_EQUAL(d1.remove("test1"), true)
RESULT

CHECK(Directory::getFirstEntry(String& entry))
	Directory d1("data/Directory_test/");
	String s;
	TEST_EQUAL(d1.getFirstEntry(s), true)
	s = "aa";
	TEST_EQUAL(d1.has("aa"), true)
	s = "a";
RESULT

CHECK(Directory::getFirstEntry(String& entry) const )
	Directory d1("data/Directory_test/");
	String s;
	TEST_EQUAL(d1.getFirstEntry(s), true)
	TEST_EQUAL(s, "a");
	TEST_EQUAL(d1.getFirstEntry(s), true)
	TEST_EQUAL(s, "a");
RESULT

CHECK(Directory::getNextEntry(String& entry))
	Directory d1("data/Directory_test/");
	String s;
	TEST_EQUAL(d1.getNextEntry(s), true)
	s = "bb";
	TEST_EQUAL(d1.has("bb"), true)
	s = "b";
RESULT

CHECK(Directory::getNextEntry(String& entry) const )
	Directory d1("data/Directory_test/");
	String s;
	TEST_EQUAL(d1.getNextEntry(s), true)
	TEST_EQUAL(s, "b");
	TEST_EQUAL(d1.getNextEntry(s), true)
	TEST_EQUAL(s, "b");
	TEST_EQUAL(d1.getNextEntry(s), true)
	TEST_EQUAL(s, "CVS");
RESULT
 
CHECK(Directory::countItems() const )
	Directory d1("data/Directory_test/");
	TEST_EQUAL(d1.countItems(), 3)
RESULT

CHECK(Directory::countFiles() const )
	Directory d1("data/Directory_test/");
	TEST_EQUAL(d1.countFiles(), 2)
RESULT

CHECK(Directory::countDirectories() const )
	Directory d1("data/Directory_test/");
	TEST_EQUAL(d1.countDirectories(), 1)
RESULT

CHECK(Directory::find(const String& filename, String& filepath, bool recursive = false))
	Directory d1("data/");
	String s;
	TEST_EQUAL(d1.find("a", s, true ), true)
	s = "Directory_test1";
	TEST_EQUAL(d1.rename(s, "Directory_test"), true)
RESULT

CHECK(Directory::find(const String& filename, String& filepath, bool recursive = false) const )
	Directory d1("data/");
	String s;
	TEST_EQUAL(d1.find("a", s, true ), true)
	TEST_EQUAL(s, "Directory_test");
	TEST_EQUAL(d1.find("a", s, false ), false)
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

CHECK(Directory::apply(DirectoryProcessor& processor))
  //BAUSTELLE
RESULT

CHECK(Directory::apply(DirectoryRecursiveProcessor& processor))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
