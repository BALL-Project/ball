// $Id: Directory_test.C,v 1.5.4.3 2002/12/11 10:32:27 crauser Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/directory.h>
///////////////////////////



START_TEST(Directory, "$Id: Directory_test.C,v 1.5.4.3 2002/12/11 10:32:27 crauser Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String PS = FileSystem::PATH_SEPARATOR;
Directory* dd;
String test_dir;
CHECK(prerequisites)
#ifdef BALL_COMPILER_MSVC
	test_dir = ::_getcwd(NULL, Directory::MAX_PATH_LENGTH);
	test_dir += "\\data\\Directory_test";
	TEST_EQUAL(::_chdir(test_dir.c_str()), 0)
	test_dir = ::_getcwd(NULL, Directory::MAX_PATH_LENGTH);
#else
	test_dir = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
	test_dir += "/data/Directory_test";
	TEST_EQUAL(::chdir(test_dir.c_str()), 0)
	test_dir = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
#endif
RESULT

CHECK(Directory::Directory())
	dd = new Directory();
	TEST_NOT_EQUAL(dd, 0)
#ifdef BALL_COMPILER_MSVC
	TEST_EQUAL(dd->getPath(), String(::_getcwd(NULL, Directory::MAX_PATH_LENGTH)))
#else
	TEST_EQUAL(dd->getPath(), String(::getcwd(NULL, Directory::MAX_PATH_LENGTH)))
#endif
	TEST_EQUAL(dd->isCurrent(), true)
RESULT

CHECK(Directory::~Directory())
	delete dd;
RESULT


CHECK(Directory::setCurrent(const String& path))
	Directory d;
	bool result = d.setCurrent(test_dir);
	TEST_EQUAL(result, true);
#ifdef BALL_COMPILER_MSVC
	String path  = ::_getcwd(NULL, Directory::MAX_PATH_LENGTH);
#else
	String path  = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
#endif
	TEST_EQUAL(path, test_dir)
	result = d.setCurrent("dir_a");
	TEST_EQUAL(result, true);
#ifdef BALL_COMPILER_MSVC
	path = ::_getcwd(NULL, Directory::MAX_PATH_LENGTH);
#else
	path = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
#endif
	const String& PS = FileSystem::PATH_SEPARATOR;
	TEST_EQUAL(path, test_dir + PS+"dir_a")
	result = d.setCurrent("c");
	TEST_EQUAL(result, false);
#ifdef BALL_COMPILER_MSVC
	path = ::_getcwd(NULL, Directory::MAX_PATH_LENGTH);
#else
	path = ::getcwd(NULL, Directory::MAX_PATH_LENGTH);
#endif
	TEST_EQUAL(path, test_dir + PS+ "dir_a")
RESULT

CHECK(Directory::Directory(const String& directory_path, bool set_current = false))
#ifdef BALL_COMPILER_MSVC
	::_chdir(test_dir.c_str());
#else
	::chdir(test_dir.c_str());
#endif
	Directory d("dir_a");
	TEST_EQUAL(d.isValid(), true)
	
#ifdef BALL_COMPILER_MSVC
	String s = String(::_getcwd(NULL, Directory::MAX_PATH_LENGTH)) + PS + "dir_a";
#else
	String s = String(::getcwd(NULL, Directory::MAX_PATH_LENGTH)) + PS + "dir_a";
#endif
	TEST_EQUAL(d.getPath(), s)

	Directory d1("dir_a"+PS, true);
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d1.isValid(), true)
	TEST_EQUAL(d.setCurrent(test_dir), true);


	Directory d2("dir_a" + PS + "dir_c"+PS);
	TEST_EQUAL(d2.isValid(), true)

	
	Directory d3(test_dir);
	TEST_EQUAL(d3.isValid(), true)

	Directory d5("dir_f");
	TEST_EQUAL(d5.isValid(), false)
RESULT

CHECK(Directory::Directory(const Directory& directory))

	Directory d("dir_a"+PS);
	Directory d1(d);
	TEST_EQUAL(d1 == d, true)
RESULT

CHECK(Directory::clear())

	Directory d("dir_a"+PS);
	d.clear();
	TEST_EQUAL(d.getPath(), "")
RESULT

CHECK(Directory::destroy())
	Directory d1("dir_a"+PS);

	d1.destroy();
RESULT

CHECK(Directory::set(const String& directory_path, bool set_current = false))
	Directory d;
	d.set("dir_a" + PS + "dir_c" + PS);
	TEST_EQUAL(d.getPath(), test_dir + PS + "dir_a" + PS + "dir_c")
	TEST_EQUAL(d.isCurrent(), false)
	d.set("dir_a" + PS + "dir_c"+ PS, true);
	TEST_EQUAL(d.isCurrent(), true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::set(const Directory& directory))
	Directory d("dir_a" + PS);
	Directory d1;
	d1.set(d);
	TEST_EQUAL(d1.getPath(), test_dir + PS + "dir_a")
RESULT

CHECK(Directory::Directory& operator = (const Directory& directory))
	Directory d("dir_a" + PS);
	Directory d1;
	d1 = d;
	TEST_EQUAL(d1.getPath(), test_dir + PS + "dir_a")	
RESULT

CHECK(Directory::get(Directory& directory) const )
	Directory d("dir_a" + PS);
	Directory d1;
	d.get(d1);
	TEST_EQUAL(d1.getPath(), test_dir + PS + "dir_a")
RESULT

CHECK(Directory::swap(Directory& directory))
	Directory d;
	Directory d1("dir_a" + PS);
	Directory d2("dir_a" + PS + "dir_c" + PS, true);
	d1.swap(d2);
	TEST_EQUAL(d1.getPath(), test_dir + PS +"dir_a" + PS + "dir_c")
	TEST_EQUAL(d2.getPath(), test_dir + PS + "dir_a")
	TEST_EQUAL(d1.isCurrent(), true)
	TEST_EQUAL(d2.isCurrent(), false)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::getPath() const )
	Directory d1("dir_a" + PS);
	TEST_EQUAL(d1.getPath(), test_dir + PS + "dir_a")
RESULT

CHECK(Directory::create(String& path, mode_t mode = 0777))
	Directory d;
#ifdef BALL_COMPILER_MSVC
	::_chdir(test_dir.c_str());
#else
	::chdir(test_dir.c_str());
#endif
	Directory d1("dir_a" + PS + "dir_c"+ PS, true);
	TEST_EQUAL(d1.isValid(), true)
	d1.remove("test1");
	d1.remove("test2");
	d1.remove("test3");
	bool result = d1.create("test1");
	TEST_EQUAL(result, true)
	TEST_EQUAL(d.setCurrent(test_dir), true)
	Directory d2;
	result = d2.create("dir_a" + PS + "dir_c" + PS + "test2");
	TEST_EQUAL(result, true)
	TEST_EQUAL(d2.isCurrent(), true)
	TEST_EQUAL(d.setCurrent(test_dir), true)
	Directory d3;
	TEST_EQUAL(d3.create(test_dir + PS +"dir_a" + PS + "dir_c" + PS + "test3"), true)
	d3.remove(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "test3");
	d3.remove(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "test2");
	d3.remove(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "test1");
RESULT

CHECK(Directory::getNextEntry(String& entry))
#ifdef BALL_COMPILER_MSVC
	::_chdir(String(test_dir + PS + "dir_a" + PS + "dir_c").c_str());
#else
	::chdir(String(test_dir + PS + "dir_a" + PS + "dir_c").c_str());
#endif
	Directory d1(".", true);
	TEST_EQUAL(d1.isValid(), true)
	d1.create("test1");
	d1.create("test1" + PS + "test2");

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
	d1.remove("test1" + PS + "test2");
	d1.remove("test1");
RESULT



CHECK(Directory::countItems() const)
	Directory d0(test_dir + PS + "dir_a" + PS + "dir_c");
	d0.create("test1");
	Directory d1(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "test1");
	d1.create("test1");
	d1.create("test2");
	d1.create("test3");
	d1.create("test4");
	TEST_EQUAL(d1.countItems(), 4)
RESULT

CHECK(Directory::countFiles() const)
	Directory d1(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "test1");
	TEST_EQUAL(d1.countFiles(), 0)
RESULT

CHECK(Directory::countDirectories() const )
	Directory d0(test_dir + PS + "dir_a" + PS + "dir_c");
	Directory d1(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "test1");
	TEST_EQUAL(d1.countDirectories(), 4)d1.remove("test1");
	d1.remove("test2");
	d1.remove("test3");
	d1.remove("test4");
	d0.remove("test1");
RESULT



CHECK(Directory::remove(String old_path))
	Directory d;
#ifdef BALL_COMPILER_MSVC
	::_chdir(test_dir.c_str());
#else
	::chdir(test_dir.c_str());
#endif
	Directory d1("dir_a" + PS + "dir_c" + PS, true);
	d1.create("test1");
	TEST_EQUAL(d1.isValid(), true)
	bool result = d1.remove("test1");
	TEST_EQUAL(result, true)
	TEST_EQUAL(d1.remove("xxxx"), false)

	d.setCurrent(test_dir);
RESULT

CHECK(Directory::rename(String old_path, String new_path))
	Directory d;
	Directory d1("dir_a" + PS + "dir_c", true);
	TEST_EQUAL(d1.isValid(), true)
	bool result = d1.create("test1");
	TEST_EQUAL(result, true)	
	result = d1.rename("test1", "test2");
	TEST_EQUAL(result, true)  
	result = d1.rename("test2", "test3");
	TEST_EQUAL(result, true)	  
	TEST_EQUAL(d1.rename("testz", "test"), false)	 
	result = d1.remove("test3");
	TEST_EQUAL(result, true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::renameTo(const String& new_path))
	Directory d;
	Directory d1("dir_a" + PS + "dir_c");
	bool result = d1.create("test1");
	TEST_EQUAL(result, true)
	d.setCurrent(test_dir);
	Directory d2("dir_a" + PS + "dir_c" + PS + "test1", true);
	TEST_EQUAL(d2.isValid(), true)
  result = d2.renameTo("test2");
	TEST_EQUAL(result, true)	  
	result = d1.remove("test2");
	TEST_EQUAL(result, true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::getFirstEntry(String& entry) const )
	String s;
	Directory d0(test_dir + PS + "dir_a" + PS + "dir_c");

	d0.create("test1");

	TEST_EQUAL(d0.getFirstEntry(s), true)
	TEST_EQUAL(s, ".");
	TEST_EQUAL(d0.getFirstEntry(s), true)
	TEST_EQUAL(s, ".");
	d0.remove("test1");
RESULT

CHECK(Directory::find(const String& filename, String& filepath))
	Directory d1(test_dir + PS + "dir_a");
	String s;
	bool result = d1.find("dir_c", s);
	TEST_EQUAL(result, true)
	TEST_EQUAL(s, test_dir + PS + "dir_a");
		d1.set(test_dir);
	result = d1.find("a", s);
	TEST_EQUAL(result, true)
	TEST_EQUAL(s, test_dir)
	result = d1.find("f", s);
	TEST_EQUAL(result, false)
RESULT

CHECK(Directory::has(const String& filename, bool recursive = false) const )
	Directory d1("dir_a" + PS + "dir_c"+ PS);
	TEST_EQUAL(d1.has("a"), true)
	TEST_EQUAL(d1.has("x"), false)
RESULT

CHECK(Directory::isCurrent() const )
	Directory d;
	Directory d1("dir_a" + PS + "dir_c"+ PS);
	TEST_EQUAL(d1.isCurrent(), false)
	d1.setCurrent();
	TEST_EQUAL(d1.isCurrent(), true)
	d.setCurrent(test_dir);
RESULT

CHECK(Directory::isEmpty() const )
	Directory d1(test_dir + PS + "dir_a" + PS + "dir_c");
	d1.create("xxx");
	Directory d2(test_dir + PS + "dir_a" + PS + "dir_c" + PS + "xxx");
	TEST_EQUAL(d2.isEmpty(), true)
	d1.remove("xxx");
	TEST_EQUAL(d1.isEmpty(), false)
RESULT

CHECK(Directory::bool operator == (const Directory& directory) const )
	Directory d("data" + PS);
	Directory d1(d);
	TEST_EQUAL(d == d1, true)
	d1.set("dir_a"+ PS + "dir_c"+ PS);
	TEST_EQUAL(d == d1, false)
RESULT

CHECK(Directory::bool operator != (const Directory& directory) const )
	Directory d("data" + PS);
	Directory d1(d);
	TEST_EQUAL(d != d1, false)
	d1.set("dir_a" + PS + "dir_c" + PS);
	TEST_EQUAL(d != d1, true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
