// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/FORMAT/lineBasedFile.h>
///////////////////////////

START_TEST(LineBasedFile)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

String PS = FileSystem::PATH_SEPARATOR;

LineBasedFile* fl;

CHECK(LineBasedFile() )
	fl = new LineBasedFile;
	TEST_NOT_EQUAL(fl, 0)
RESULT

CHECK(~LineBasedFile() )
	delete fl;
RESULT

CHECK(LineBasedFile(const String& filename, File::OpenMode open_mode = std::ios::in)
			throw(Exception::FileNotFound))
	LineBasedFile f1(BALL_TEST_DATA_PATH(LineBasedFile_test.txt));
	TEST_EQUAL(f1.getLineNumber(), 0)
	String line = f1.getLine();
	TEST_EQUAL(line, "")
	f1.readLine();
	TEST_EQUAL(f1.getLineNumber(), 1)
	line = f1.getLine();
	TEST_EQUAL(line, "line1")
	f1.close();

	LineBasedFile* f2 = 0;
	TEST_EXCEPTION(Exception::FileNotFound, f2 = new LineBasedFile("XXXXXXXX.txt"))
	if (f2 != 0)
	{
		delete f2;
	}
RESULT

LineBasedFile fx;

CHECK(LineBasedFile& operator = (const LineBasedFile& file) )
	LineBasedFile f1(BALL_TEST_DATA_PATH(LineBasedFile_test.txt));
	f1.readLine();
	String line = f1.getLine();
	TEST_EQUAL(line, "line1")

	LineBasedFile f2;
	f2 = f1;
	TEST_EQUAL(f2.getLineNumber(), 1)
	line = f2.getLine();
	TEST_EQUAL(line, "line1")
RESULT

CHECK(clear() )
	LineBasedFile f1(BALL_TEST_DATA_PATH(LineBasedFile_test.txt));
	f1.readLine();
	f1.clear();
	TEST_EQUAL(f1.getLineNumber(), 0)
	String line = f1.getLine();
	TEST_EQUAL(line, "")

	fx.clear();
RESULT

CHECK(getLineNumber() const )
	LineBasedFile f1(BALL_TEST_DATA_PATH(LineBasedFile_test.txt));
	TEST_EQUAL(f1.getLineNumber(), 0)
	f1.readLine();
	TEST_EQUAL(f1.getLineNumber(), 1)
	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

LineBasedFile f1(BALL_TEST_DATA_PATH(LineBasedFile_test.txt));

CHECK(getLine() const )
	String line = f1.getLine();
	TEST_EQUAL(line, "")
	f1.readLine();
	line = f1.getLine();
	TEST_EQUAL(line, "line1")

	line = fx.getLine();
	TEST_EQUAL(line, "")
RESULT

CHECK(getLine())	
	String line = f1.getLine();
	TEST_EQUAL(line, "line1")
	f1.getLine() = "test";
	line = f1.getLine();
	
	TEST_EQUAL(line, "test")
	f1.getLine() = "line1";
	line = f1.getLine();
	TEST_EQUAL(line, "line1")
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
	TEST_EQUAL(f1.getField(2, "/"), "2 23")
	TEST_EQUAL(f1.getField(3, "/"), "")

	TEST_EQUAL(fx.getField(), "")
RESULT

CHECK(startsWith(const String& text) const )
	TEST_EQUAL(f1.startsWith("/0/"), true)
	TEST_EQUAL(f1.startsWith("/0/ /1/ /2 2//3/"), true)
	TEST_EQUAL(f1.startsWith("/0/ /1/ /2 2//3/X"), false)

	TEST_EQUAL(fx.startsWith(""), true)
RESULT

CHECK(has(const String& text) const )
	TEST_EQUAL(f1.has("/0/"), true)
	TEST_EQUAL(f1.has("/"), true)
	TEST_EQUAL(f1.has("/1/"), true)
	TEST_EQUAL(f1.has("/3/"), true)
	TEST_EQUAL(f1.has("X"), false)

	TEST_EQUAL(fx.has("X"), false)
	TEST_EQUAL(fx.has(""), true)
RESULT

CHECK(search(const String& text, bool return_to_point) )
	bool result = f1.search("line3");
	TEST_EQUAL(result, true)
	result = f1.search("line4-");
	TEST_EQUAL(result, true)
	result = f1.search("line4-");
	TEST_EQUAL(result, false)
	String line = f1.getLine();
	TEST_EQUAL(line, "line7-")
  f1.rewind();

	f1.skipLines(2);
	line = f1.getLine();
	TEST_EQUAL(line, "line3-")
	result = f1.search("XXX", true);
	TEST_EQUAL(result, false)
	line = f1.getLine();
	TEST_EQUAL(line, "line3-")

  f1.rewind();
	result = f1.search("#");
	TEST_EQUAL(result, true)
	line = f1.getLine();
	TEST_EQUAL(line, "###########")

	TEST_EXCEPTION(Exception::ParseError, fx.search("line4-"))
RESULT

CHECK(search(const String& text, const String& stop, bool return_to_point) )
  f1.rewind();
	bool result = f1.search("line3", "line4", true);
	TEST_EQUAL(result, true)
	String line = f1.getLine();
	TEST_EQUAL(line, "line3-")
	TEST_EQUAL(f1.getLineNumber(), 3)

  f1.rewind();
	result = f1.search("line4", "line3", false);
	TEST_EQUAL(result, false)
	line = f1.getLine();
	TEST_EQUAL(line, "line3-")
	TEST_EQUAL(f1.getLineNumber(), 3)

  f1.rewind();
	result = f1.search("/", "l", false);
	TEST_EQUAL(result, false)
	line = f1.getLine();
	TEST_EQUAL(line, "line1")
	TEST_EQUAL(f1.getLineNumber(), 1)

  f1.rewind();
	f1.readLine();
	f1.readLine();
	line = f1.getLine();
	TEST_EQUAL(line, "/0/ /1/ /2 2//3/")
	result = f1.search("line4", "line3", true);
	TEST_EQUAL(result, false)
	line = f1.getLine();
	TEST_EQUAL(line, "/0/ /1/ /2 2//3/")
	TEST_EQUAL(f1.getLineNumber(), 2)

  f1.rewind();
	f1.skipLines(2);
	line = f1.getLine();
	TEST_EQUAL(line, "line3-")
	result = f1.search("XXX", "ZZZZZ", true);
	TEST_EQUAL(result, false)
	line = f1.getLine(); 
	TEST_EQUAL(line, "line3-")

  f1.rewind();
	result = f1.search("#", "l", false);
	TEST_EQUAL(result, false)
	line = f1.getLine();
	TEST_EQUAL(line, "line1")

  f1.rewind();
	result = f1.search("#", "l", true);
	TEST_EQUAL(result, false)
	TEST_EQUAL(f1.getLineNumber(), 0)

  f1.rewind();
	result = f1.search("line7", "#", false);
	TEST_EQUAL(result, false)
	line = f1.getLine();
	TEST_EQUAL(line, "###########")


	TEST_EXCEPTION(Exception::ParseError, fx.search("line4", "line3"))
RESULT

CHECK(switchString(const std::vector<String>& data) const )
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
	TEST_EXCEPTION(Exception::ParseError, f1.test(__FILE__, __LINE__, false, "test") )
	fx.test(__FILE__, __LINE__, true, "test");
RESULT

CHECK(readLine() throw(LineBasedFileError))
  f1.rewind();
	f1.readLine();

	TEST_EQUAL(f1.getLine(), "line1")

	TEST_EXCEPTION(Exception::ParseError, fx.readLine())
	TEST_EQUAL(fx.getLine(), "")
RESULT

CHECK(skipLines(Size number = 1) throw(Exception::IndexUnderflow, LineBasedFileError))
  f1.rewind();
	bool result = f1.skipLines(2);
	TEST_EQUAL(result, true)
	String line = f1.getLine();
	TEST_EQUAL(line, "line3-")
	TEST_EQUAL(f1.getLineNumber(), 3)
	result = f1.skipLines(5);
	TEST_EQUAL(result, false)

	TEST_EXCEPTION(Exception::ParseError, fx.skipLines(2))
	line = fx.getLine();
	TEST_EQUAL(line, "")
	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

CHECK(rewind() throw(LineBasedFileError))
  f1.rewind();
	String line = f1.getLine();
	TEST_EQUAL(line, "")
	TEST_EQUAL(f1.getLineNumber(), 0)

  TEST_EXCEPTION(Exception::ParseError,fx.rewind())
	line = fx.getLine();
	TEST_EQUAL(line, "")
	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

CHECK(gotoLine(Position line_number) throw(LineBasedFileError))
  f1.rewind();
	f1.skipLines(4);
	bool result = f1.gotoLine(3);
	TEST_EQUAL(result, true)
	String line = f1.getLine();
	TEST_EQUAL(line, "line3-" )
	TEST_EQUAL(f1.getLineNumber(), 3)

	result = f1.gotoLine(5);
	TEST_EQUAL(result, true)
	line = f1.getLine();
	TEST_EQUAL(line, "line5-" )
	TEST_EQUAL(f1.getLineNumber(), 5)

	result = f1.gotoLine(8);
	TEST_EQUAL(result, false)
	line = f1.getLine(); 
	TEST_EQUAL(line, "line7-" )
	TEST_EQUAL(f1.getLineNumber(), 7)

  TEST_EXCEPTION(Exception::ParseError,fx.gotoLine(2))
	line = fx.getLine();
	TEST_EQUAL(line, "")
	TEST_EQUAL(fx.getLineNumber(), 0)
RESULT

CHECK([EXTRA] triming whitespaces)
	LineBasedFile f1(BALL_TEST_DATA_PATH(LineBasedFile_test2.txt), File::MODE_IN, true);
	f1.readLine();
	String line = f1.getLine();
	TEST_EQUAL(line, "line1")
	f1.readLine();
	line = f1.getLine();
	TEST_EQUAL(line, "line2")
	f1.readLine();
	line = f1.getLine();
	TEST_EQUAL(line, "line 3")
RESULT

CHECK(bool trimWhiteSpacesEnabled() const )
	LineBasedFile f1;
	TEST_EQUAL(f1.trimWhiteSpacesEnabled(), false)
RESULT

CHECK(void enableTrimWhitespaces(bool state) )
	LineBasedFile f1;
	TEST_EQUAL(f1.trimWhiteSpacesEnabled(), false)
	f1.enableTrimWhitespaces(true);
	TEST_EQUAL(f1.trimWhiteSpacesEnabled(), true)
	f1.enableTrimWhitespaces(false);
	TEST_EQUAL(f1.trimWhiteSpacesEnabled(), false)
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
