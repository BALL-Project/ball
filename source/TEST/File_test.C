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

#ifdef BALL_COMPILER_MSVC
#	define sleep(a) _sleep(1000 * a)
#endif

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

CHECK(File(const String& name, OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
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

CHECK(bool open(const String& name, File::OpenMode open_mode = std::ios::in) throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	file.open(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(f.isOpen(), true)
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(bool reopen() throw(Exception::FileNotFound))
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

CHECK(static Size getSize(String name) throw(Exception::FileNotFound))
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

CHECK(static Type getType(String name, bool trace_link) throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(file.getType(BALL_TEST_DATA_PATH(File_test.txt), false), 4)
	TEST_EQUAL(file.getSize(), 100)
	TEST_EXCEPTION(Exception::FileNotFound, File::getType("this_file_should_not_exists", true))
RESULT

CHECK(Type getType(bool trace_link) const throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.getType(false), 4)
	TEST_EQUAL(f.getType(true), 4)
	TEST_EQUAL(file.getSize(), 100)
	File x;
	TEST_EXCEPTION(Exception::FileNotFound, x.getType(true))
RESULT

CHECK(static bool copy(String source_name, String destination_name, Size buffer_size = 4096) throw(Exception::FileNotFound))
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

CHECK(bool copyTo(const String& destination_name, Size buffer_size = 4096) throw(Exception::FileNotFound))
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

CHECK(static bool move(const String& source_name, const String& destination_name) throw(Exception::FileNotFound))
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

CHECK(bool moveTo(const String& destination_name) throw(Exception::FileNotFound))
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

CHECK(static bool rename(String old_path, String new_path) throw(Exception::FileNotFound))
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

CHECK(bool renameTo(const String& new_path) throw(Exception::FileNotFound))
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

CHECK(static bool truncate(String path, Size size = 0) throw(Exception::FileNotFound))
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

CHECK(bool truncate(Size size = 0) throw(Exception::FileNotFound))
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

CHECK(bool operator == (const File& file) const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
/* TODO: we can't copy files!
	File f1(f);
	TEST_EQUAL(f1 == f, true)	
*/
	file.copyTo("XXX");
	File f2("XXX");
	TEST_EQUAL(f2 == f, false)	
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(bool operator != (const File& file) const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	/* TODO: we can't copy files!
	File f1(f);
	TEST_EQUAL(f1 != f, false)	
	*/
	file.copyTo("XXX");
	File f2("XXX");
	TEST_EQUAL(f2 != f, true)	
	TEST_EQUAL(file.getSize(), 100)
RESULT

CHECK(static bool isAccessible(String name) throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.isAccessible(BALL_TEST_DATA_PATH(File_test.txt)), true)
	f.remove("XXX");
	TEST_EQUAL(f.isAccessible("XXX"), false)
RESULT

CHECK(bool isAccessible() const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.isAccessible(), true)
	file.copyTo("XXZ");

	File f1("XXZ");
	f1.remove();
	TEST_EQUAL(f1.isAccessible(), false)
RESULT

CHECK(bool isCanonized() const throw(Exception::FileNotFound))
	File f0(BALL_TEST_DATA_PATH(../../TEST/data/File_test.txt));
	TEST_EQUAL(f0.isValid(), true)
	TEST_EQUAL(f0.isCanonized(), true)

	File f2(BALL_TEST_DATA_PATH(/File_test.txt));
	TEST_EQUAL(f2.isValid(), true)
	TEST_EQUAL(f2.isCanonized(), true)

	File f4(BALL_TEST_DATA_PATH(../data/File_test.txt));
	TEST_EQUAL(f4.isValid(), true)
	TEST_EQUAL(f4.isCanonized(), true)

	File f5(BALL_TEST_DATA_PATH(.././data/File_test.txt));
	TEST_EQUAL(f5.isValid(), true)
	TEST_EQUAL(f5.isCanonized(), true)

	File f6(BALL_TEST_DATA_PATH(File_test.txt));
	TEST_EQUAL(f6.isValid(), true)
	TEST_EQUAL(f6.isCanonized(), true)

	File x;
	x.setName("this_file_should_not_exists");
	TEST_EXCEPTION(Exception::FileNotFound, x.isCanonized())
RESULT

CHECK(static bool isReadable(String name) throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.isReadable(source_name), true)	

	TEST_EXCEPTION(Exception::FileNotFound, File::isReadable("this_file_should_not_exists"))
RESULT

CHECK(bool isReadable() const throw(Exception::FileNotFound))
	File f2(source_name);
	TEST_EQUAL(f2.isReadable(), true)

	File x;
	TEST_EXCEPTION(Exception::FileNotFound, x.isReadable())
RESULT

CHECK(static bool isWritable(String name) throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.isWritable(source_name), true)	

	TEST_EXCEPTION(Exception::FileNotFound, File::isWritable("this_file_should_not_exists"))
RESULT

CHECK(bool isWritable() const throw(Exception::FileNotFound))
	File f2(source_name);
	TEST_EQUAL(f2.isWritable(), true)	

	File x;
	TEST_EXCEPTION(Exception::FileNotFound, x.isWritable())
RESULT

CHECK(static bool isExecutable(String name) throw(Exception::FileNotFound))
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
#ifndef BALL_COMPILER_MSVC
	TEST_EQUAL(f.isExecutable(BALL_TEST_DATA_PATH(File_test2.txt)), true)
#endif
	TEST_EQUAL(f.isExecutable(source_name), false)	

	TEST_EXCEPTION(Exception::FileNotFound, File::isExecutable("this_file_should_not_exists"))
RESULT

CHECK(bool isExecutable() const throw(Exception::FileNotFound))
#ifndef BALL_COMPILER_MSVC
	File f1(BALL_TEST_DATA_PATH(File_test2.txt));
	TEST_EQUAL(f1.isExecutable(), true)	
#endif
	File f2(source_name);
	TEST_EQUAL(f2.isExecutable(), false)	
RESULT

CHECK(bool isValid() const throw())
	File f;
	TEST_EQUAL(f.isValid(), false)	

	File f1;
	TEST_EXCEPTION(Exception::FileNotFound, f1.open("XXY"))
	TEST_EQUAL(f1.isValid(), false)	

	File f2(source_name);
	TEST_EQUAL(f2.isValid(), true)	
	f2.close();
	TEST_EQUAL(f2.isValid(), true)	
RESULT

CHECK(bool isOpen() const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.isOpen(), true)	
	File f2;
	TEST_EQUAL(f2.isOpen(), false)	
	f2.open(source_name);
	TEST_EQUAL(f2.isOpen(), true)	
	f2.close();
	TEST_EQUAL(f2.isOpen(), false)	
RESULT

CHECK(bool isClosed() const throw())
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	const File& f  = file;
	TEST_EQUAL(f.isClosed(), false)	
	File f2;
	TEST_EQUAL(f2.isClosed(), true)	
	f2.open(source_name);
	TEST_EQUAL(f2.isClosed(), false)	
	f2.close();
	TEST_EQUAL(f2.isClosed(), true)	
RESULT

String filename;
CHECK(std::fstream& getFileStream())
	NEW_TMP_FILE(filename);
	File new_file(filename, std::ios::out);
	File  file(BALL_TEST_DATA_PATH(File_test.txt));
	new_file.getFileStream() << "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
	new_file.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(File_test.txt))

	File x;
	x.getFileStream();
RESULT

CHECK(void clear() throw())
	File file(filename);
	file.clear();
	TEST_EQUAL(file.getName(), "")
RESULT

CHECK(bool reopen(File::OpenMode open_mode) throw(Exception::FileNotFound))
	File file(filename);
	TEST_EQUAL(file.reopen(), true)
	file.setName("this_file_should_not_exists");
	TEST_EQUAL(file.getName(), "this_file_should_not_exists")
	TEST_EXCEPTION(Exception::FileNotFound, file.reopen())
RESULT

CHECK(void setName(const String& name) throw())
	File file;
	file.setName(filename);
	file.setName("this_file_should_not_exists");
RESULT

CHECK(const String& getOriginalName() const)
	File file;
	TEST_EQUAL(file.getOriginalName(), "")
	File file2(String("file:")+argv[0]);
	TEST_EQUAL(file2.getOriginalName(), String("file:")+argv[0])
	TEST_EQUAL(file2.getName(), argv[0])
RESULT

CHECK(TransformationManager& getTransformationManager())
	File file;
	TEST_EQUAL(file.getTransformationManager().findTransformation("ftp://"), "")
RESULT

CHECK(const TransformationManager& getTransformationManager() const)
	File file;
	file.getTransformationManager().registerTransformation("xxx://", "ls");
RESULT

CHECK(static void disableTransformation(Transformation transformation))
	File::disableTransformation(File::TRANSFORMATION__URL);
	TEST_EQUAL(File::isTransformationEnabled(File::TRANSFORMATION__URL), false)
RESULT

CHECK(static void enableTransformation(Transformation transformation))
	File::enableTransformation(File::TRANSFORMATION__URL);
	TEST_EQUAL(File::isTransformationEnabled(File::TRANSFORMATION__URL), true)
RESULT

CHECK(static void registerTransformation(const String& pattern, const String& exec))
	File::registerTransformation("yyy://", "ls");
RESULT

CHECK(static void unregisterTransformation(const String& pattern))
	File::unregisterTransformation("yyy://");
	File::unregisterTransformation("xxx://");
	File::unregisterTransformation("zzz://");
RESULT

CHECK(static bool isTransformationEnabled(Transformation transformation))
	TEST_EQUAL(File::isTransformationEnabled(File::TRANSFORMATION__URL), true)
RESULT

// ============================================================
// other classes
// ============================================================

// TransformationManager

TransformationManager* m_ptr = 0;
CHECK(TransformationManager())
  m_ptr = new TransformationManager;
RESULT

CHECK(~TransformationManager())
  delete m_ptr;
RESULT

CHECK(String findTransformation(const String& name) const)
	TransformationManager f;
	TEST_EQUAL(f.findTransformation(""), "")
	TEST_EQUAL(f.findTransformation("asddasd"), "")
	TEST_EQUAL(f.findTransformation("ftp://"), "")
RESULT

CHECK(void registerTransformation(const String& pattern, const String& command))
	TransformationManager f;
	f.registerTransformation("asdddasd", "addd");
	TEST_EQUAL(f.findTransformation("asdddasd"), "addd")
RESULT

CHECK(String transform(const String& name))
	TransformationManager f;
	TEST_EQUAL(f.transform(""), "")
	TEST_EQUAL(f.transform("asdd"), "")
	f.registerTransformation("blabla:", "asd");
	TEST_EQUAL(f.transform("blabla"), "")
	TEST_EQUAL(f.transform("blabla:"), "asd");
	f.registerTransformation("my1:", "%s");
	TEST_EQUAL(f.transform("my1:filename.ext"), "my1:filename.ext")
	f.registerTransformation("my2:%s", "name.txt");
	TEST_EQUAL(f.transform("my2:%s"), "name.txt")
	f.registerTransformation("my3:", "%f");
	TEST_EQUAL(f.transform("my3:filename.ext"), "my3:filename")
	f.registerTransformation("my3:", "%f[bla]");
	TEST_EQUAL(f.transform("my3:filename.ext"), "my3:filename")
	TEST_EQUAL(f.transform("my3:filename.bla"), "my3:filename")
	String PS = FileSystem::PATH_SEPARATOR;
	f.registerTransformation("my4:", "%b");
	TEST_EQUAL(f.transform("my4:"+PS+"some_path"+PS+"filename.ext"), "filename")
	f.registerTransformation("my5:", "%p");
	TEST_EQUAL(f.transform("my5:"+PS+"some_path"+PS+"filename.ext"), "my5:"+PS+"some_path"+PS)
	f.registerTransformation("my6:", "%t");
	String result = f.transform("my6:asddasd");
	TEST_EQUAL(result !="", true )
	TEST_EQUAL(result.size(), 12 )
	TEST_EQUAL(result[0], '_')
	TEST_EQUAL(File::isAccessible(result), false)
RESULT

CHECK(void unregisterTransformation(const String& pattern))
	TransformationManager f;
	f.registerTransformation("asdddasd", "addd");
	f.unregisterTransformation("asdddasd");
	TEST_EQUAL(f.findTransformation("asdddasd"), "")
RESULT

File::CannotWrite* cw_ptr = 0;
CHECK(CannotWrite(const char* file, int line, const String& filename) throw())	
	cw_ptr = new File::CannotWrite("asdf", 1234, "filename");
	TEST_NOT_EQUAL(cw_ptr, 0)
RESULT

CHECK(String getFilename() const throw())
	TEST_EQUAL(cw_ptr->getFilename(), "filename")
RESULT

CHECK(~CannotWrite() throw())
	delete cw_ptr;
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
