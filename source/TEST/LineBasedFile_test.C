// $Id: LineBasedFile_test.C,v 1.3 2000/10/14 16:40:26 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/lineBasedFile.h>
///////////////////////////

START_TEST(class_name, "$Id: LineBasedFile_test.C,v 1.3 2000/10/14 16:40:26 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


CHECK(BALL_CREATE(LineBasedFile))
  //BAUSTELLE
RESULT

LineBasedFile* fl;

CHECK(LineBasedFile() throw())
	fl = new LineBasedFile;
	TEST_NOT_EQUAL(fl, 0)
RESULT

CHECK(~LineBasedFile() throw())
	delete fl;
RESULT

CHECK(LineBasedFile(const String& filename, File::OpenMode open_mode = File::IN)
			throw(Exception::FileNotFound))
	LineBasedFile f1("data/LineBasedFile_test.txt");
	TEST_EQUAL(f1.getLineNumber(), 0)
	TEST_EQUAL(f1.getLine(), "")
	f1.readLine();
	TEST_EQUAL(f1.getLineNumber(), 1)
	TEST_EQUAL(f1.getLine(), "line1")

	TEST_EXCEPTION(Exception::FileNotFound, LineBasedFile f2("XXXXXXXX.txt"))
RESULT

LineBasedFile fx;

CHECK(LineBasedFile(const LineBasedFile& f) throw())
	LineBasedFile f1("data/LineBasedFile_test.txt");
	f1.readLine();
	LineBasedFile f2(f1);

	TEST_EQUAL(f2.getLineNumber(), 1)
	TEST_EQUAL(f2.getLine(), "line1")
RESULT

CHECK(LineBasedFile& operator = (const LineBasedFile& file) throw())
	LineBasedFile f1("data/LineBasedFile_test.txt");
	f1.readLine();
	LineBasedFile f2(f1);
	TEST_EQUAL(f2.getLineNumber(), 1)
	TEST_EQUAL(f2.getLine(), "line1")
RESULT

CHECK(clear() throw())
	LineBasedFile f1("data/LineBasedFile_test.txt");
	f1.readLine();
	f1.clear();
	TEST_EQUAL(f1.getLineNumber(), 0)
	TEST_EQUAL(f1.getLine(), "")

	fx.clear();
RESULT

CHECK(getLineNumber() const  throw())
	LineBasedFile f1("data/LineBasedFile_test.txt");
	TEST_EQUAL(f1.getLineNumber(), 0)
	f1.readLine();
	TEST_EQUAL(f1.getLineNumber(), 1)

	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

LineBasedFile f1("data/LineBasedFile_test.txt");

CHECK(getLine() const  throw())
	TEST_EQUAL(f1.getLine(), "")
	f1.readLine();
	TEST_EQUAL(f1.getLine(), "line1")

	TEST_EQUAL(fx.getLine(), "")
RESULT


CHECK(getField(Position pos = 0, const String& quotes = "", 
			const String& delimiters = String::CHARACTER_CLASS__WHITESPACE) 
			const  throw(Exception::IndexUnderflow))

	TEST_EQUAL(f1.getField(), "line1")
	TEST_EQUAL(f1.getField(1), "")
	TEST_EXCEPTION(Exception::IndexUnderflow, f1.getField(-99))
	f1.readLine();
	TEST_EQUAL(f1.getField(), "/0/")
	TEST_EQUAL(f1.getField(1), "/1/")
	TEST_EQUAL(f1.getField(2), "/2")
	TEST_EQUAL(f1.getField(0, "/"), "0")
	TEST_EQUAL(f1.getField(1, "/"), "1")
	TEST_EQUAL(f1.getField(2, "/"), "2 2")
	TEST_EQUAL(f1.getField(3, "/"), "3")

	TEST_EQUAL(fx.getField(), "")
RESULT

CHECK(copyString(Position start, Position end = 0) 
			const throw(Exception::IndexUnderflow, Exception::NullPointer))
	TEST_EQUAL(f1.copyString(0), "/0/ /1/ /2 2//3/")
	TEST_EQUAL(f1.copyString(1, 15), "0/ /1/ /2 2//3")
	TEST_EXCEPTION(Exception::IndexUnderflow, f1.copyString(-1, 15))
	TEST_EQUAL(f1.copyString(0, 150), "0/ /1/ /2 2//3")
	TEST_EQUAL(f1.copyString(30, 150), "")

	TEST_EQUAL(fx.copyString(0), "")
RESULT

CHECK(startsWith(const String& text) const  throw())
	TEST_EQUAL(f1.startsWith("/0/"), true)
	TEST_EQUAL(f1.startsWith("/0/ /1/ /2 2//3/"), true)
	TEST_EQUAL(f1.startsWith("/0/ /1/ /2 2//3/X"), false)

	TEST_EQUAL(fx.startsWith(""), false)
RESULT

CHECK(has(const String& text) const  throw())
	TEST_EQUAL(f1.has("/0/"), true)
	TEST_EQUAL(f1.has("/1/"), true)
	TEST_EQUAL(f1.has("/3/"), true)
	TEST_EQUAL(f1.has("X"), false)

	TEST_EQUAL(fx.has("X"), false)
	TEST_EQUAL(fx.has(""), false)
RESULT

CHECK(search(const String& text) throw())
	TEST_EQUAL(f1.search("line3"), true)
	TEST_EQUAL(f1.search("line4-"), true)
	TEST_EQUAL(f1.search("line4-"), false)

	TEST_EQUAL(fx.search("line4-"), false)
RESULT

CHECK(search(const String& text, const String& stop) throw())
  f1.rewind();
	TEST_EQUAL(f1.search("line3", "line4"), true)
  f1.rewind();
	TEST_EQUAL(f1.search("line4", "line3"), false)

	TEST_EQUAL(fx.search("line4", "line3"), false)
RESULT

CHECK(switchString(const std::vector<String>& data) const  throw())
  f1.rewind();
	f1.readLine();

	vector<String> vec;
	vec.push_back("line");
	vec.push_back("line1");
	vec.push_back("line2");
	vec.push_back("");

	TEST_EQUAL(f1.switchString(vec), 1)
	f1.readLine();
	TEST_EQUAL(f1.switchString(vec), -1)

	TEST_EQUAL(fx.switchString(vec), 3)
RESULT

CHECK(test(const char* file, int line, bool condition, const String& msg)
			const  throw(LineBasedFileError))
	f1.test(__FILE__, __LINE__, true, "test");
	TEST_EXCEPTION(LineBasedFile::LineBasedFileError, f1.test(__FILE__, __LINE__, false, "test") )
	fx.test(__FILE__, __LINE__, true, "test");
RESULT

CHECK(readLine() throw())
  f1.rewind();
	f1.readLine();

	TEST_EQUAL(f1.getLine(), "line1")

	fx.readLine();
	TEST_EQUAL(fx.getLine(), "")
RESULT

CHECK(skipLines(Size number = 1) throw(Exception::IndexUnderflow))
  f1.rewind();
	f1.skipLines(2);
	TEST_EQUAL(f1.getLine(), "line3")
	TEST_EQUAL(f1.getLineNumber(), 3)

	fx.skipLines(2);
	TEST_EQUAL(fx.getLine(), "")
	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

CHECK(rewind() throw())
  f1.rewind();
	TEST_EQUAL(f1.getLine(), "")
	TEST_EQUAL(f1.getLineNumber(), 0)

  fx.rewind();
	TEST_EQUAL(fx.getLine(), "")
	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
