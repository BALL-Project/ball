// $Id: LineBasedFile_test.C,v 1.1 2000/10/12 23:30:35 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/lineBasedFile.h>
///////////////////////////

START_TEST(class_name, "$Id: LineBasedFile_test.C,v 1.1 2000/10/12 23:30:35 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


CHECK(BALL_CREATE(LineBasedFile))
  //BAUSTELLE
RESULT

LineBasedFile* f;

CHECK(LineBasedFile() throw())
	f = new LineBasedFile;
	TEST_NOT_EQUAL(f, 0)
RESULT

CHECK(~LineBasedFile() throw())
	delete f;
RESULT

CHECK(LineBasedFile(const String& filename, File::OpenMode open_mode = File::IN) throw())
	LineBasedFile f("test/LineBasedFile_test.txt");
	TEST_EQUAL(f.getLineNumber(), 0)
	TEST_EQUAL(f.getLine, "line0")
RESULT

CHECK(LineBasedFile& operator = (const LineBasedFile& file) throw())
  //BAUSTELLE
RESULT

CHECK(clear() throw())
  //BAUSTELLE
RESULT

CHECK(getLineNumber() const  throw())
  //BAUSTELLE
RESULT

CHECK(getLine() const  throw())
  //BAUSTELLE
RESULT

CHECK(getField(Position pos = 0, const String& quotes = "", 
			const String& delimiters = String::CHARACTER_CLASS__WHITESPACE) 
			const  throw(Exception::IndexUnderflow, Exception::NullPointer))
  //BAUSTELLE
RESULT

CHECK(copyString(Position start, Position end = 0) 
			const throw(Exception::IndexUnderflow, Exception::NullPointer))
  //BAUSTELLE
RESULT

CHECK(startsWith(const String& text) const  throw())
  //BAUSTELLE
RESULT

CHECK(has(const String& text) const  throw())
  //BAUSTELLE
RESULT

CHECK(search(const String& text) throw())
  //BAUSTELLE
RESULT

CHECK(search(const String& text, const String& stop) throw())
  //BAUSTELLE
RESULT

CHECK(switchString(const std::vector<String>& data) const  throw())
  //BAUSTELLE
RESULT

CHECK(test(const char* file, int line, bool condition, const String& msg)
			const  throw(LineBasedFileError))
  //BAUSTELLE
RESULT

CHECK(readLine() throw())
  //BAUSTELLE
RESULT

CHECK(skipLines(Size number = 1) throw(Exception::IndexUnderflow))
  //BAUSTELLE
RESULT

CHECK(rewind() throw())
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
