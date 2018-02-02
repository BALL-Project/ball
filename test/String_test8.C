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
String s4;
String s5;

CHECK([EXTRA]bool operator == (const char* char_ptr) const)
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

CHECK(bool operator != (const char* char_ptr) const)
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

CHECK(bool operator < (const char* char_ptr) const)
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

CHECK(bool operator <= (const char* char_ptr) const)
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

CHECK(bool operator > (const char* char_ptr) const)
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

CHECK(bool operator >= (const char* char_ptr) const)
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

CHECK(Substring operator () (Index from, Size len = EndPos) const)
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
