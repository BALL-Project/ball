// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/SYSTEM/file.h>
#include <sys/types.h>
#include <sys/stat.h>
///////////////////////////

using namespace BALL;
using namespace std;


START_TEST(File)


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

File* f1 = 0;

String source_name(BALL_TEST_DATA_PATH(../File_test.C));

CHECK(File() throw())
	f1 = new File();
	TEST_NOT_EQUAL(f1, 0)
RESULT

CHECK(~File() throw())
	delete f1;
RESULT

CHECK(File(const String& name, OpenMode open_mode = std::ios::in))
	File f(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(f.getSize(), 100)

	File* f2;
	TEST_EXCEPTION(Exception::FileNotFound, f2 = new File("x"))
	TEST_EXCEPTION(Exception::FileNotFound, f2 = new File("sdffsdf"))
RESULT

CHECK(void close() throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(file.getSize(), 100)
	file.close();
	TEST_EQUAL(f.isClosed(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(bool open(const String& name, File::OpenMode open_mode = std::ios::in))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	file.open(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(bool reopen())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	file.close();
	file.reopen();
	file.reopen();
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)
	File x;
	x.setName("this_file_should_not_exists");
	TEST_EXCEPTION(Exception::FileNotFound, x.reopen())
RESULT

CHECK(const String& getName() const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.getName(), BALL_TEST_DATA_PATH(File_test.txt))
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(Size getSize() throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static Size getSize(String name))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(file.getSize(BALL_TEST_DATA_PATH(File_test.txt)), 100)
	TEST_EXCEPTION(Exception::FileNotFound, file.getSize("XXX"))
RESULT

CHECK(File::OpenMode getOpenMode() const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.getOpenMode(), std::ios::in)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static Type getType(String name, bool trace_link))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(file.getType(BALL_TEST_DATA_PATH(File_test.txt), false), 4)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EXCEPTION(Exception::FileNotFound, File::getType("this_file_should_not_exists", true))
RESULT

CHECK(Type getType(bool trace_link) const)
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.getType(false), 4)
	TEST_EQUAL(f.getType(true), 4)
	TEST_EQUAL(file.getSize(), 100)
	File x;
	TEST_EXCEPTION(Exception::FileNotFound, x.getType(true))
RESULT

CHECK(static bool copy(String source_name, String destination_name, Size buffer_size = 4096))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.copy(BALL_TEST_DATA_PATH(File_test.txt), BALL_TEST_DATA_PATH(File_test.txt)), false)
	TEST_EQUAL(f.copy("", BALL_TEST_DATA_PATH(File_test.txt)), false)
	TEST_EQUAL(f.copy(BALL_TEST_DATA_PATH(File_test.txt), ""), false)
	TEST_EQUAL(f.copy("", ""), false)
	TEST_EXCEPTION(Exception::FileNotFound, f.copy("ZZZZZZZZZZ", "XXX"))
	TEST_EQUAL(f.copy(BALL_TEST_DATA_PATH(File_test.txt), "XXX"), true)
	TEST_EQUAL(f.copy(BALL_TEST_DATA_PATH(File_test.txt), "XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(file.getSize("XXX"), 100)
	f.remove("XXX");

	TEST_EQUAL(f.copy("", "X"), false)
	TEST_EQUAL(f.copy(BALL_TEST_DATA_PATH(File_test.txt), ""), false)
RESULT

CHECK(bool copyTo(const String& destination_name, Size buffer_size = 4096))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(file.copyTo(BALL_TEST_DATA_PATH(File_test.txt)), false)
	TEST_EQUAL(file.copyTo(""), false)
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(file.getSize("XXX"), 100)
	f.remove("XXX");

	TEST_EQUAL(file.copyTo(""), false)

	File x;
	x.setName("this_file_should_not_exists");
	TEST_EXCEPTION(Exception::FileNotFound, x.copyTo("asdaddasdasd"))
RESULT

CHECK(static bool move(const String& source_name, const String& destination_name))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.copyTo("YYY"), true)
	TEST_EQUAL(f.move("XXX", "XXX"), false)
	TEST_EQUAL(f.move("", "XXX"), false)
	TEST_EQUAL(f.move("XXX", ""), false)

	TEST_EQUAL(f.move("XXX", "YYY") && f.move("YYY", "XXX"), true)
	TEST_EQUAL(f.isAccessible("YYY"), false)
	TEST_EQUAL(file.getSize("XXX"), 100)

	file.copyTo("XXX");
	TEST_EQUAL(f.move("XXX", "YYY") && f.move("YYY", "XXX"), true)
	TEST_EQUAL(file.getSize(), 100)

	TEST_EQUAL(f.move("YYY", ""), false)
	TEST_EQUAL(f.move("", "XXX"), false)

	f.remove("XXX");
	f.remove("YYY");

	TEST_EXCEPTION(Exception::FileNotFound, f.move("ZZZZZZZZZZ", "XXX"))
RESULT

CHECK(bool moveTo(const String& destination_name))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.moveTo("XXX"), false)
	TEST_EQUAL(f1.moveTo(""), false)
	TEST_EQUAL(f1.moveTo("YYY"), true)
	TEST_EQUAL(f1.isAccessible(), true)
	TEST_EQUAL(f1.moveTo("YYY"), false)
	TEST_EQUAL(f1.isAccessible(), true)
	TEST_EQUAL(file.getSize("YYY"), 100)

	TEST_EQUAL(f1.moveTo(""), false)
	TEST_EQUAL(f.remove("YYY"), true)
	TEST_EXCEPTION(Exception::FileNotFound, f1.moveTo("XXX"))
RESULT

CHECK(static bool remove(String name) throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	file.copyTo("XXX");
	TEST_EQUAL(f.remove("XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(f.remove("XXX"), false)
RESULT

CHECK(bool remove() throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.remove(), true)
	TEST_EQUAL(f1.remove(), false)
	TEST_EQUAL(f1.isAccessible(), false)
RESULT

CHECK(static bool rename(String old_path, String new_path))
	File  filex(BALL_TEST_DATA_PATH(File_test.txt));
	filex.copyTo("XXX");

	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	File f1("XXX");
	TEST_EQUAL(f1.rename("XXX", "XXX"), true)
	f1.close();
	TEST_EQUAL(f1.rename("XXX", "YYY") && f1.rename("YYY", "XXX"), true)

	TEST_EQUAL(f1.isAccessible("YYY"), false)
	TEST_EQUAL(f1.isAccessible("XXX"), true)
	f1.remove();

	TEST_EQUAL(file.getSize(), 100)
	f.remove("YYY");

	TEST_EXCEPTION(Exception::FileNotFound, f1.rename("", "XXX"))
	TEST_EXCEPTION(Exception::FileNotFound, f1.rename("XXX", ""))
RESULT

CHECK(bool renameTo(const String& new_path))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	file.copyTo("XXX");

	File f1("XXX");
	TEST_EQUAL(f1.renameTo("XXX"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), true)
	TEST_EQUAL(f1.renameTo("YYY"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), false)
	TEST_EQUAL(f1.isAccessible("YYY"), true)
	f1.remove();

	File x;
	TEST_EXCEPTION(Exception::FileNotFound, x.renameTo("dddddddd"))
RESULT

CHECK(static bool truncate(String path, Size size = 0))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.truncate("XXX", 50), true)
	TEST_EQUAL(f1.getSize(), 50)
	TEST_EQUAL(f1.truncate("XXX", 0), true)
	TEST_EQUAL(f1.getSize(), 0)
	f1.remove();

	TEST_EXCEPTION(Exception::FileNotFound, f1.truncate("", 50))
RESULT

CHECK(bool truncate(Size size = 0))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.truncate(50), true)
	TEST_EQUAL(f1.getSize(), 50)
	TEST_EQUAL(f1.truncate(0), true)
	TEST_EQUAL(f1.getSize(), 0)
	f1.remove();

	File x;
	TEST_EXCEPTION(Exception::FileNotFound, x.truncate())
RESULT

CHECK(static bool createTemporaryFilename(String& temporary) throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	String s;
	TEST_EQUAL(f.createTemporaryFilename(s), true)
	TEST_NOT_EQUAL(s, "")
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
