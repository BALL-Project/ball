// $Id: File_test.C,v 1.13 2000/10/21 14:10:54 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SYSTEM/file.h>
#include <sys/types.h>
#include <sys/stat.h>
///////////////////////////

START_TEST(class_name, "$Id: File_test.C,v 1.13 2000/10/21 14:10:54 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;
	
File* f1;

CHECK(File())
	f1 = new File();
	TEST_NOT_EQUAL(f1, 0)
RESULT

CHECK(~File())
	delete f1;
RESULT

CHECK(File::OpenMode)
	TEST_EQUAL(File::IN, std::ios::in)
	TEST_EQUAL(File::APP, std::ios::app)
	TEST_EQUAL(File::OUT, std::ios::out)
	TEST_EQUAL(File::ATE, std::ios::ate)
	TEST_EQUAL(File::TRUNC, std::ios::trunc)
	TEST_EQUAL(File::BINARY, std::ios::binary)
RESULT

CHECK(File(const String& name, OpenMode open_mode = File::IN))
	File f("data/File_test.txt");
	TEST_EQUAL(f.getSize(), 100)

	TEST_EXCEPTION(Exception::FileNotFound, File f2(""))
	TEST_EXCEPTION(Exception::FileNotFound, File f2("sdffsdf"))
RESULT

File  file("data/File_test.txt");
const File& f  = file;

CHECK(File(const File& file))
	File f1(f);
	TEST_EQUAL(f1 == f, true)

	File f2;
	TEST_EXCEPTION(Exception::FileNotFound, File f3(f2))
RESULT

CHECK(close())
	TEST_EQUAL(file.getSize(), 100)
	file.close();
	TEST_EQUAL(f.isClosed(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(open(const String& name, OpenMode open_mode = File::IN))
	file.open("data/File_test.txt");
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)

	File f1(f);
	TEST_EXCEPTION(Exception::FileNotFound, f1.open(""))
RESULT

CHECK(reopen())
	file.close();
	file.reopen();
	file.reopen();
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(getName())
	TEST_EQUAL(f.getName(), "data/File_test.txt")
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(getSize())
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static getSize(String filename))
	TEST_EQUAL(f.getSize("data/File_test.txt"), 100)
	TEST_EXCEPTION(Exception::FileNotFound, f.getSize("XXX"))
RESULT

CHECK(int getOpenMode() const)
	TEST_EQUAL(f.getOpenMode(), File::IN)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static Type getType(String name, bool trace_link))
	TEST_EQUAL(f.getType("data/File_test.txt", false), 4)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(Type getType(bool trace_link) const;)
	TEST_EQUAL(f.getType(false), 4)
	TEST_EQUAL(f.getType(true), 4)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(copy(String source_name, String destination_name, Size buffer_size = 4096))
	TEST_EQUAL(file.copy("data/File_test.txt", "data/File_test.txt"), false)
	TEST_EQUAL(file.copy("", "data/File_test.txt"), false)
	TEST_EQUAL(file.copy("data/File_test.txt", ""), false)
	TEST_EQUAL(file.copy("", ""), false)
	TEST_EXCEPTION(Exception::FileNotFound, file.copy("ZZZZZZZZZZ", "XXX"))
	TEST_EQUAL(file.copy("data/File_test.txt", "XXX"), true)
	TEST_EQUAL(file.copy("data/File_test.txt", "XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(f.getSize("XXX"), 100)
	f.remove("XXX");

	TEST_EQUAL(file.copy("", "X"), false)
	TEST_EQUAL(file.copy("data/File_test.txt", ""), false)
RESULT

CHECK(copyTo(const String& destination_name, Size buffer_size = 4096))
	TEST_EQUAL(file.copyTo("data/File_test.txt"), false)
	TEST_EQUAL(file.copyTo(""), false)
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(f.getSize("XXX"), 100)
	f.remove("XXX");

	TEST_EQUAL(file.copyTo(""), false)
RESULT

CHECK(move(const String& source_name, const String& destination_name))
	TEST_EQUAL(file.copyTo("XXX"), true)
	TEST_EQUAL(file.copyTo("YYY"), true)
	TEST_EQUAL(f.move("XXX", "XXX"), false)
	TEST_EQUAL(f.move("", "XXX"), false)
	TEST_EQUAL(f.move("XXX", ""), false)

	TEST_EQUAL(f.move("XXX", "YYY"), true)
	TEST_EQUAL(f.isAccessible("XXX"), false)
	TEST_EQUAL(f.getSize("YYY"), 100)

	file.copyTo("XXX");
	TEST_EQUAL(f.move("XXX", "YYY"), true)
	TEST_EQUAL(file.getSize(), 100)

	TEST_EQUAL(f.move("YYY", ""), false)
	TEST_EQUAL(f.move("", "XXX"), false)

	f.remove("XXX");
	f.remove("YYY");

	TEST_EXCEPTION(Exception::FileNotFound, f.move("ZZZZZZZZZZ", "XXX"))
RESULT

CHECK(moveTo(const String& destination_name))
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.moveTo("XXX"), false)
	TEST_EQUAL(f1.moveTo(""), false)
	TEST_EQUAL(f1.moveTo("YYY"), true)
	TEST_EQUAL(f1.isAccessible(), true)
	TEST_EQUAL(f1.moveTo("YYY"), false)
	TEST_EQUAL(f1.isAccessible(), true)
	TEST_EQUAL(f.getSize("YYY"), 100)

	TEST_EQUAL(f1.moveTo(""), false)
	TEST_EQUAL(f.remove("YYY"), true)
	TEST_EXCEPTION(Exception::FileNotFound, f1.moveTo("XXX"))
RESULT

CHECK(remove(String name))
	file.copyTo("XXX");
	TEST_EQUAL(f.remove("XXX"), true)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EQUAL(f.remove("XXX"), false)
RESULT

CHECK(remove())
	file.copyTo("XXX");
	File f1 = File("XXX");
	TEST_EQUAL(f1.remove(), true)
	TEST_EQUAL(f1.remove(), false)
	TEST_EQUAL(f1.isAccessible(), false)
RESULT

CHECK(rename(String old_path, String new_path))
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.rename("XXX", "XXX"), true)
	TEST_EQUAL(f1.rename("XXX", "YYY"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), false)
	TEST_EQUAL(f1.isAccessible("YYY"), true)
	f1.remove();
	TEST_EQUAL(file.getSize(), 100)
	f.remove("YYY");

	TEST_EXCEPTION(Exception::FileNotFound, f1.rename("", "XXX"))
	TEST_EXCEPTION(Exception::FileNotFound, f1.rename("XXX", ""))
RESULT

CHECK(renameTo(const String& new_path))
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.renameTo("XXX"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), true)
	TEST_EQUAL(f1.renameTo("YYY"), true)
	TEST_EQUAL(f1.isAccessible("XXX"), false)
	TEST_EQUAL(f1.isAccessible("YYY"), true)
	f1.remove();
RESULT

CHECK(truncate(String path, Size size = 0))
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.truncate("XXX", 50), true)
	TEST_EQUAL(f1.getSize(), 50)
	TEST_EQUAL(f1.truncate("XXX", 0), true)
	TEST_EQUAL(f1.getSize(), 0)
	f1.remove();

	TEST_EXCEPTION(Exception::FileNotFound, f1.truncate("", 50))
RESULT

CHECK(truncate(Size size = 0))
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.truncate(50), true)
	TEST_EQUAL(f1.getSize(), 50)
	TEST_EQUAL(f1.truncate(0), true)
	TEST_EQUAL(f1.getSize(), 0)
	f1.remove();
RESULT

CHECK(createTemporaryFilename(String& temporary))
	String s;
	TEST_EQUAL(f.createTemporaryFilename(s), true)
	TEST_NOT_EQUAL(s, "")
RESULT

CHECK(operator == (const File& file))
	File f1(f);
	TEST_EQUAL(f1 == f, true)	
	file.copyTo("XXX");
	File f2("XXX");
	TEST_EQUAL(f2 == f, false)	
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(operator != (const File& file))
	File f1(f);
	TEST_EQUAL(f1 != f, false)	
	file.copyTo("XXX");
	File f2("XXX");
	TEST_EQUAL(f2 != f, true)	
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(isOpen())
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.isOpen(), true)	
	f1.close();
	TEST_EQUAL(f1.isOpen(), false)	
RESULT

CHECK(isClosed())
	file.copyTo("XXX");
	File f1("XXX");
	TEST_EQUAL(f1.isClosed(), false)	
	f1.close();
	TEST_EQUAL(f1.isClosed(), true)	
RESULT

CHECK(isAccessible(String name))
	TEST_EQUAL(f.isAccessible("data/File_test.txt"), true)
	f.remove("XXX");
	TEST_EQUAL(f.isAccessible("XXX"), false)
RESULT

CHECK(isAccessible())
	TEST_EQUAL(f.isAccessible(), true)
	file.copyTo("XXX");
	File f1("XXX");
	f1.remove();
	TEST_EQUAL(f1.isAccessible(), false)
RESULT

CHECK(isCanonized())
	File f1("../TEST/data/File_test.txt");
	TEST_EQUAL(f1.isValid(), true)
	TEST_EQUAL(f1.isCanonized(), true)

	File f2("data//File_test.txt");
	TEST_EQUAL(f2.isValid(), true)
	TEST_EQUAL(f2.isCanonized(), true)

	File f4("data/../data/File_test.txt");
	TEST_EQUAL(f4.isValid(), true)
	TEST_EQUAL(f4.isCanonized(), true)

	File f5("./data/File_test.txt");
	TEST_EQUAL(f5.isValid(), true)
	TEST_EQUAL(f5.isCanonized(), true)

	File f6("data/File_test.txt");
	TEST_EQUAL(f6.isValid(), true)
	TEST_EQUAL(f6.isCanonized(), true)

	File f7("~/BALL/source/TEST/data/File_test.txt");
	TEST_EQUAL(f7.isCanonized(), true)
RESULT

CHECK(isReadable(String name))
	TEST_EQUAL(f.isReadable("File_test.C"), true)	
RESULT

CHECK(isReadable())
	File f2("File_test.C");
	TEST_EQUAL(f2.isReadable(), true)
RESULT

CHECK(isWritable(String name))
	TEST_EQUAL(f.isWritable("File_test.C"), true)	
RESULT

CHECK(isWritable())
	File f2("File_test.C");
	TEST_EQUAL(f2.isWritable(), true)	
RESULT

CHECK(isExecutable(String name))
	TEST_EQUAL(f.isExecutable(BALL_PATH "/source/configure"), true)	
	TEST_EQUAL(f.isExecutable("File_test.C"), false)	
RESULT

CHECK(isExecutable())
	File f1(BALL_PATH "/source/configure");
	TEST_EQUAL(f1.isExecutable(), true)	
	File f2("File_test.C");
	TEST_EQUAL(f2.isExecutable(), false)	
RESULT

CHECK(isValid())
	TEST_EQUAL(f.isValid(), true)	

	File f1;
	TEST_EXCEPTION(Exception::FileNotFound, f1 = File("XXX"))
	TEST_EQUAL(f1.isValid(), true)	
RESULT

CHECK(isOpen())
	TEST_EQUAL(f.isOpen(), true)	
	File f2;
	TEST_EQUAL(f2.isOpen(), false)	
	f2.open("File_test.C");
	TEST_EQUAL(f2.isOpen(), true)	
	f2.close();
	TEST_EQUAL(f2.isOpen(), false)	
RESULT


CHECK(isClosed())
	TEST_EQUAL(f.isClosed(), false)	
	File f2;
	TEST_EQUAL(f2.isClosed(), true)	
	f2.open("File_test.C");
	TEST_EQUAL(f2.isClosed(), false)	
	f2.close();
	TEST_EQUAL(f2.isClosed(), true)	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
