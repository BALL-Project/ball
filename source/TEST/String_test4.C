// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/DATATYPE/string.h>
#include <cstring>
#include <string>
///////////////////////////

START_TEST(String)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using std::string;

String* s2;
String hall("Hall");
String halloh("Halloh");
String s4;

CHECK(int compare(const char* char_ptr, Index from = 0) const throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abc";
	TEST_EQUAL(s4.compare("abc"), 0)
	TEST_EQUAL(s4.compare("abc", -3), 0)
	TEST_EQUAL((s4.compare("abd") < 0), true)
	TEST_EQUAL((s4.compare("abb") > 0), true)
	TEST_EQUAL((s4.compare("abcd") < 0), true)
	TEST_EQUAL((s4.compare("ab") > 0), true)
	s4 = "";
	TEST_EQUAL(s4.compare(""), 0)
	TEST_EQUAL((s4.compare("a") < 0), true)
	TEST_EQUAL((s4.compare("b") < 0), true)
	s4 = "abc";
	TEST_EQUAL(s4.compare("bc", 1), 0)
	TEST_EQUAL(s4.compare("bc", -2), 0)
	TEST_EQUAL(s4.compare("c", 2), 0)
	TEST_EQUAL(s4.compare("c", -1), 0)

	char* c = 0;
	TEST_EXCEPTION(Exception::NullPointer, s4.compare(c, 0))
RESULT

CHECK(int compare(const char* char_ptr, Index from, Size len) const throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abc";
	TEST_EQUAL(s4.compare("abc", 0, 1), 0)
	TEST_EQUAL(s4.compare("abc", 0, 2), 0)
	TEST_EQUAL(s4.compare("abc", 0, 3), 0)
	TEST_EXCEPTION(Exception::IndexOverflow, s4.compare("abc", 0, 4))
	TEST_EQUAL(s4.compare("axxx", 0, 1), 0)
	TEST_EQUAL(s4.compare("abxx", 0, 2), 0)
	TEST_EQUAL(s4.compare("abcx", 0, 3), 0)
	TEST_EQUAL(s4.compare("axxx", -3, 1), 0)
	TEST_EQUAL(s4.compare("abxx", -3, 2), 0)
	TEST_EQUAL(s4.compare("abcx", -3, 3), 0)
	TEST_EQUAL(s4.compare("bxxx", 1, 1), 0)
	TEST_EQUAL(s4.compare("bcxx", 1, 2), 0)
	TEST_EQUAL(s4.compare("bxxx", -2, 1), 0)
	TEST_EQUAL(s4.compare("bcxx", -2, 2), 0)
	TEST_EQUAL(s4.compare("cxxx", 2, 1), 0)
	TEST_EQUAL(s4.compare("cxxx", -1, 1), 0)
	TEST_NOT_EQUAL(s4.compare("bd", 1, 2), 0) 
	TEST_EQUAL(s4.compare("abcd", 0, 3), 0) 
	TEST_EQUAL(s4.compare("abd", 0, 2), 0) 
	TEST_NOT_EQUAL(s4.compare("abd", 0, 3), 0) 

	char* c = 0;
	TEST_EXCEPTION(Exception::NullPointer, s4.compare(c, 0, 1))
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.compare("a", -99, 0))
	TEST_EXCEPTION(Exception::IndexOverflow, s4.compare("a", 0, 99))
RESULT

CHECK(int compare(char c, Index from = 0) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abcd";
	TEST_EQUAL(s4.compare('a'), 0)
	TEST_EQUAL(s4.compare('a', 0), 0)
	TEST_EQUAL(s4.compare('a', -4), 0)
	TEST_EQUAL(s4.compare('b', 1), 0)
	TEST_EQUAL(s4.compare('b', -3), 0)
	TEST_EQUAL(s4.compare('c', 2), 0)
	TEST_EQUAL(s4.compare('c', -2), 0)
	TEST_EQUAL(s4.compare('d', 3), 0)
	TEST_EQUAL(s4.compare('d', -1), 0)
	TEST_NOT_EQUAL(s4.compare('a'), 1)
	TEST_EXCEPTION(Exception::IndexOverflow, s4.compare('a', 5))
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.compare('a', -10))
RESULT

String s5;
CHECK([EXTRA]bool operator == (const String& string) const throw())
	s4 = "abcd";
	s5 = "abcd";
	TEST_EQUAL(s4, s5)
	s5 = "abcD";
	TEST_NOT_EQUAL(s4, s5)
	s4 = "";
	s5 = "";
	TEST_EQUAL(s4, s5)
	s4 = "";
	s5 = "a";
	TEST_NOT_EQUAL(s4, s5)
	s4 = "c";
	s5 = "";
	TEST_NOT_EQUAL(s4, s5)
RESULT

CHECK(bool operator != (const String& string) const throw())
	s4 = "abcd";
	s5 = "abcd";
	TEST_EQUAL((s4 != s5), false)
	s5 = "abcD";
	TEST_EQUAL((s4 != s5), true)
	s4 = "";
	s5 = "";
	TEST_EQUAL((s4 != s5), false)
	s4 = "";
	s5 = "a";
	TEST_EQUAL((s4 != s5), true)
	s4 = "c";
	s5 = "";
	TEST_EQUAL((s4 != s5), true)
RESULT

CHECK(bool operator < (const String& string) const throw())
	s4 = "abc";
	s5 = "abd";
	TEST_EQUAL((s4 < s5), true)
	s4 = "abd";
	s5 = "abc";
	TEST_EQUAL((s4 < s5), false)
	s4 = "abc";
	s5 = "abc";
	TEST_EQUAL((s4 < s5), false)
	s4 = "a";
	s5 = "a";
	TEST_EQUAL((s4 < s5), false)
	s4 = "ab";
	s5 = "abc";
	TEST_EQUAL((s4 < s5), true)
	s4 = "";
	s5 = "a";
	TEST_EQUAL((s4 < s5), true)
RESULT

CHECK(bool operator <= (const String& string) const throw())
	s4 = "abc";
	s5 = "abd";
	TEST_EQUAL((s4 <= s5), true)
	s4 = "abd";
	s5 = "abc";
	TEST_EQUAL((s4 <= s5), false)
	s4 = "abc";
	s5 = "abc";
	TEST_EQUAL((s4 <= s5), true)
	s4 = "a";
	s5 = "a";
	TEST_EQUAL((s4 <= s5), true)
	s4 = "ab";
	s5 = "abc";
	TEST_EQUAL((s4 <= s5), true)
	s4 = "";
	s5 = "a";
	TEST_EQUAL((s4 <= s5), true)
RESULT

CHECK(bool operator > (const String& string) const throw())
	s4 = "abc";
	s5 = "abd";
	TEST_EQUAL((s4 > s5), false)
	s4 = "abd";
	s5 = "abc";
	TEST_EQUAL((s4 > s5), true)
	s4 = "abc";
	s5 = "abc";
	TEST_EQUAL((s4 > s5), false)
	s4 = "a";
	s5 = "a";
	TEST_EQUAL((s4 > s5), false)
	s4 = "ab";
	s5 = "abc";
	TEST_EQUAL((s4 > s5), false)
	s4 = "";
	s5 = "a";
	TEST_EQUAL((s4 > s5), false)
	s4 = "";
	s5 = "";
	TEST_EQUAL((s4 > s5), false)
RESULT

CHECK(bool operator >= (const String& string) const throw())
	s4 = "abc";
	s5 = "abd";
	TEST_EQUAL((s4 >= s5), false)
	s4 = "abd";
	s5 = "abc";
	TEST_EQUAL((s4 >= s5), true)
	s4 = "abc";
	s5 = "abc";
	TEST_EQUAL((s4 >= s5), true)
	s4 = "a";
	s5 = "a";
	TEST_EQUAL((s4 >= s5), true)
	s4 = "";
	s5 = "";
	TEST_EQUAL((s4 >= s5), true)
	s4 = "ab";
	s5 = "abc";
	TEST_EQUAL((s4 >= s5), false)
	s4 = "";
	s5 = "a";
	TEST_EQUAL((s4 >= s5), false)
RESULT

CHECK(friend bool operator == (const char* char_ptr, const String& string) throw(Exception::NullPointer))
	s5 = "abd";
	TEST_EQUAL(("abc" == s5), false)
	s5 = "abc";
	TEST_EQUAL(("abc" == s5), true)
	s5 = "a";
	TEST_EQUAL(("a" == s5), true)
	s5 = "";
	TEST_EQUAL(("" == s5), true)
	s5 = "abc";
	TEST_EQUAL(("ab" == s5), false)
	s5 = "a";
	TEST_EQUAL(("" == s5), false)
	s5 = "";
	TEST_EQUAL(("a" == s5), false)
RESULT

CHECK(friend bool operator != (const char* char_ptr, const String& string) throw(Exception::NullPointer))
	s5 = "abd";
	TEST_EQUAL(("abc" != s5), true)
	s5 = "abc";
	TEST_EQUAL(("abc" != s5), false)
	s5 = "a";
	TEST_EQUAL(("a" != s5), false)
	s5 = "";
	TEST_EQUAL(("" != s5), false)
	s5 = "abc";
	TEST_EQUAL(("ab" != s5), true)
	s5 = "a";
	TEST_EQUAL(("" != s5), true)
	s5 = "";
	TEST_EQUAL(("a" != s5), true)
RESULT

CHECK(friend bool operator < (const char* char_ptr, const String& string) throw(Exception::NullPointer))
	s5 = "abd";
	TEST_EQUAL(("abc" < s5), true)
	s5 = "abc";
	TEST_EQUAL(("abc" < s5), false)
	s5 = "a";
	TEST_EQUAL(("a" < s5), false)
	s5 = "";
	TEST_EQUAL(("" < s5), false)
	s5 = "abc";
	TEST_EQUAL(("ab" < s5), true)
	s5 = "a";
	TEST_EQUAL(("" < s5), true)
	s5 = "";
	TEST_EQUAL(("a" < s5), false)
RESULT

CHECK(friend bool operator <= (const char* char_ptr, const String& string) throw(Exception::NullPointer))
	s5 = "abd";
	TEST_EQUAL(("abc" <= s5), true)
	s5 = "abc";
	TEST_EQUAL(("abc" <= s5), true)
	s5 = "a";
	TEST_EQUAL(("a" <= s5), true)
	s5 = "";
	TEST_EQUAL(("" <= s5), true)
	s5 = "abc";
	TEST_EQUAL(("ab" <= s5), true)
	s5 = "a";
	TEST_EQUAL(("" <= s5), true)
	s5 = "";
	TEST_EQUAL(("a" <= s5), false)
RESULT

CHECK(friend bool operator > (const char* char_ptr, const String& string) throw(Exception::NullPointer))
	s5 = "abd";
	TEST_EQUAL(("abc" > s5), false)
	s5 = "abc";
	TEST_EQUAL(("abc" > s5), false)
	s5 = "a";
	TEST_EQUAL(("a" > s5), false)
	s5 = "";
	TEST_EQUAL(("" > s5), false)
	s5 = "abc";
	TEST_EQUAL(("ab" > s5), false)
	s5 = "a";
	TEST_EQUAL(("" > s5), false)
	s5 = "";
	TEST_EQUAL(("a" > s5), true)
RESULT

CHECK(friend bool operator >= (const char* char_ptr, const String& string) throw(Exception::NullPointer))
	s5 = "abd";
	TEST_EQUAL(("abc" >= s5), false)
	s5 = "abc";
	TEST_EQUAL(("abc" >= s5), true)
	s5 = "a";
	TEST_EQUAL(("a" >= s5), true)
	s5 = "";
	TEST_EQUAL(("" >= s5), true)
	s5 = "abc";
	TEST_EQUAL(("ab" >= s5), false)
	s5 = "a";
	TEST_EQUAL(("" >= s5), false)
	s5 = "";
	TEST_EQUAL(("a" >= s5), true)
RESULT

CHECK([EXTRA]bool operator == (const char* char_ptr) const throw(Exception::NullPointer))
	s5 = "abc";
	TEST_EQUAL((s5 == "abc"), true)
	TEST_EQUAL((s5 == "abd"), false)
	TEST_EQUAL((s5 == "ab"), false)
	TEST_EQUAL((s5 == "abcd"), false)
	s5 = "abd";
	TEST_EQUAL((s5 == "abc"), false)
	s5 = "a";
	TEST_EQUAL((s5 == "a"), true)
	TEST_EQUAL((s5 == ""), false)
	s5 = "";
	TEST_EQUAL((s5 == "a"), false)
	TEST_EQUAL((s5 == ""), true)
RESULT

CHECK(bool operator != (const char* char_ptr) const throw(Exception::NullPointer))
	s5 = "abc";
	TEST_EQUAL((s5 != "abc"), false)
	TEST_EQUAL((s5 != "abd"), true)
	TEST_EQUAL((s5 != "ab"), true)
	TEST_EQUAL((s5 != "abcd"), true)
	s5 = "abd";
	TEST_EQUAL((s5 != "abc"), true)
	s5 = "a";
	TEST_EQUAL((s5 != "a"), false)
	TEST_EQUAL((s5 != ""), true)
	s5 = "";
	TEST_EQUAL((s5 != "a"), true)
	TEST_EQUAL((s5 != ""), false)
RESULT

CHECK(bool operator < (const char* char_ptr) const throw(Exception::NullPointer))
	s5 = "abc";
	TEST_EQUAL((s5 < "abc"), false)
	TEST_EQUAL((s5 < "abd"), true)
	TEST_EQUAL((s5 < "ab"), false)
	TEST_EQUAL((s5 < "abcd"), true)
	s5 = "abd";
	TEST_EQUAL((s5 < "abc"), false)
	s5 = "a";
	TEST_EQUAL((s5 < "a"), false)
	TEST_EQUAL((s5 < ""), false)
	s5 = "";
	TEST_EQUAL((s5 < "a"), true)
	TEST_EQUAL((s5 < ""), false)
RESULT

CHECK(bool operator <= (const char* char_ptr) const throw(Exception::NullPointer))
	s5 = "abc";
	TEST_EQUAL((s5 <= "abc"), true)
	TEST_EQUAL((s5 <= "abd"), true)
	TEST_EQUAL((s5 <= "ab"), false)
	TEST_EQUAL((s5 <= "abcd"), true)
	s5 = "abd";
	TEST_EQUAL((s5 <= "abc"), false)
	s5 = "a";
	TEST_EQUAL((s5 <= "a"), true)
	TEST_EQUAL((s5 <= ""), false)
	s5 = "";
	TEST_EQUAL((s5 <= "a"), true)
	TEST_EQUAL((s5 <= ""), true)
RESULT

CHECK(bool operator > (const char* char_ptr) const throw(Exception::NullPointer))
	s5 = "abc";
	TEST_EQUAL((s5 > "abc"), false)
	TEST_EQUAL((s5 > "abd"), false)
	TEST_EQUAL((s5 > "ab"), true)
	TEST_EQUAL((s5 > "abcd"), false)
	s5 = "abd";
	TEST_EQUAL((s5 > "abc"), true)
	s5 = "a";
	TEST_EQUAL((s5 > "a"), false)
	TEST_EQUAL((s5 > ""), true)
	s5 = "";
	TEST_EQUAL((s5 > "a"), false)
	TEST_EQUAL((s5 > ""), false)
RESULT

CHECK(bool operator >= (const char* char_ptr) const throw(Exception::NullPointer))
	s5 = "abc";
	TEST_EQUAL((s5 >= "abc"), true)
	TEST_EQUAL((s5 >= "abd"), false)
	TEST_EQUAL((s5 >= "ab"), true)
	TEST_EQUAL((s5 >= "abcd"), false)
	s5 = "abd";
	TEST_EQUAL((s5 >= "abc"), true)
	s5 = "a";
	TEST_EQUAL((s5 >= "a"), true)
	TEST_EQUAL((s5 >= ""), true)
	s5 = "";
	TEST_EQUAL((s5 >= "a"), false)
	TEST_EQUAL((s5 >= ""), true)
RESULT

CHECK(friend bool operator == (char c, const String& string) throw())
	s5 = "bc";
	TEST_EQUAL(('a' == s5), false)
	TEST_EQUAL(('b' == s5), true)
	TEST_EQUAL(('c' == s5), false)
	s5 = "";
	TEST_EQUAL(('a' == s5), false)
RESULT

CHECK(friend bool operator != (char c, const String& string) throw())
	s5 = "bc";
	TEST_EQUAL(('a' != s5), true)
	TEST_EQUAL(('b' != s5), false)
	TEST_EQUAL(('c' != s5), true)
	s5 = "";
	TEST_EQUAL(('a' != s5), true)
RESULT

CHECK(friend bool operator < (char c, const String& string) throw())
	s5 = "bc";
	TEST_EQUAL(('a' < s5), true)
	TEST_EQUAL(('b' < s5), false)
	TEST_EQUAL(('c' < s5), false)
	s5 = "";
	TEST_EQUAL(('a' < s5), false)
RESULT

CHECK(friend bool operator <= (char c, const String& string) throw())
	s5 = "bc";
	TEST_EQUAL(('a' <= s5), true)
	TEST_EQUAL(('b' <= s5), true)
	TEST_EQUAL(('c' <= s5), false)
	s5 = "";
	TEST_EQUAL(('a' <= s5), false)
RESULT

CHECK(friend bool operator > (char c, const String& string) throw())
	s5 = "bc";
	TEST_EQUAL(('a' > s5), false)
	TEST_EQUAL(('b' > s5), false)
	TEST_EQUAL(('c' > s5), true)
	s5 = "";
	TEST_EQUAL(('a' > s5), true)
RESULT

CHECK(friend bool operator >= (char c, const String& string) throw())
	s5 = "bc";
	TEST_EQUAL(('a' >= s5), false)
	TEST_EQUAL(('b' >= s5), true)
	TEST_EQUAL(('c' >= s5), true)
	s5 = "";
	TEST_EQUAL(('a' >= s5), true)
RESULT

CHECK(bool operator == (char c) const throw())
	s5 = "bc";
	TEST_EQUAL((s5 == 'a'), false)
	TEST_EQUAL((s5 == 'b'), true)
	TEST_EQUAL((s5 == 'c'), false)
	s5 = "";
	TEST_EQUAL((s5 == 'a'), false)
RESULT

CHECK(bool operator != (char c) const throw())
	s5 = "bc";
	TEST_EQUAL((s5 != 'a'), true)
	TEST_EQUAL((s5 != 'b'), false)
	TEST_EQUAL((s5 != 'c'), true)
	s5 = "";
	TEST_EQUAL((s5 != 'a'), true)
RESULT

CHECK(bool operator < (char c) const throw())
	s5 = "bc";
	TEST_EQUAL((s5 < 'a'), false)
	TEST_EQUAL((s5 < 'b'), false)
	TEST_EQUAL((s5 < 'c'), true)
	s5 = "";
	TEST_EQUAL((s5 < 'a'), true)
RESULT

CHECK(bool operator <= (char c) const throw())
	s5 = "bc";
	TEST_EQUAL((s5 <= 'a'), false)
	TEST_EQUAL((s5 <= 'b'), true)
	TEST_EQUAL((s5 <= 'c'), true)
	s5 = "";
	TEST_EQUAL((s5 <= 'a'), true)
RESULT

CHECK(bool operator > (char c) const throw())
	s5 = "bc";
	TEST_EQUAL((s5 > 'a'), true)
	TEST_EQUAL((s5 > 'b'), false)
	TEST_EQUAL((s5 > 'c'), false)
	s5 = "";
	TEST_EQUAL((s5 > 'a'), false)
RESULT

CHECK(bool operator >= (char c) const throw())
	s5 = "bc";
	TEST_EQUAL((s5 >= 'a'), true)
	TEST_EQUAL((s5 >= 'b'), true)
	TEST_EQUAL((s5 >= 'c'), false)
	s5 = "";
	TEST_EQUAL((s5 >= 'a'), false)
RESULT

CHECK(bool isValid() const throw())
	s2 = new String;
	TEST_EQUAL(s2->isValid(), true)
	delete s2;
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	std::ofstream dump_stream(tmp_filename.c_str(), std::ios::out);
	s2 = new String("abcdefghijklm");
	s2->dump(dump_stream, 0);
	dump_stream.clear();
	dump_stream.close();
	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(string_test_dump0.txt))

	NEW_TMP_FILE(tmp_filename)
	std::ofstream dump_stream2(tmp_filename.c_str(), std::ios::out);
	s2->dump(dump_stream2, 4);
	dump_stream2.close();
	TEST_FILE_REGEXP(tmp_filename.c_str(), BALL_TEST_DATA_PATH(string_test_dump4.txt))
	delete s2;
RESULT

CHECK(std::istream& getline(std::istream& s = std::cin, char delimiter = '\n') throw())
	String line;
	std::ifstream instream(BALL_TEST_DATA_PATH(String_test2.txt));
	line.getline(instream);
	TEST_EQUAL(line, "ABC DEF")
	line.getline(instream);
	TEST_EQUAL(line, "GHI jkl")
	line.getline(instream);
	TEST_EQUAL(line, "mno")
RESULT

CHECK(friend std::istream& getline(std::istream& s, String& string, char delimiter = '\n') throw())
	std::ifstream test_stream(BALL_TEST_DATA_PATH(String_test2.txt));
	if (!test_stream.good())
	{
		throw Exception::FileNotFound(__FILE__, __LINE__, BALL_TEST_DATA_PATH(String_test2.txt));
	}
	String s;
	s.getline(test_stream);
	TEST_EQUAL(s, "ABC DEF")
	s.getline(test_stream);
	TEST_EQUAL(s, "GHI jkl")
	s.getline(test_stream);
	TEST_EQUAL(s, "mno")
	s.getline(test_stream);
	TEST_EQUAL(s, "")
RESULT

CHECK(Substring operator () (Index from, Size len = EndPos) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abcdef";
	Substring sub((Substring)s4);
	TEST_EQUAL(sub.toString(), s4)
RESULT

CHECK(String encodeBase64() throw())
	String test("abcdefghi");
	String erg = test.encodeBase64();
RESULT

CHECK(String decodeBase64() throw())
	String test("abcdefghi");
	String erg = test.encodeBase64();
	TEST_EQUAL(erg, "YWJjZGVmZ2hp")
	erg = erg.decodeBase64();
	TEST_EQUAL(erg, test)

	test="";
	erg = test.decodeBase64();
	TEST_EQUAL(erg, "")
RESULT


CHECK(void clear() throw())
	String test("abcdefghi");
	test.clear();
	TEST_EQUAL(test.size(), 0)
	TEST_EQUAL(test, "")
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
