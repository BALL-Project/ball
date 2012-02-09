// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

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

String hall("Hall");
String halloh("Halloh");
String s4;

CHECK(Size split(std::vector<String>& strings, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index from = 0) const throw(Exception::IndexUnderflow, Exception::NullPointer))
	vector<String> arr;
	s4 = "a b c d e f g";
	TEST_EQUAL(s4.split(arr), 7)
	TEST_EQUAL(arr[0], "a")
	TEST_EQUAL(arr[1], "b")
	TEST_EQUAL(arr[2], "c")
	TEST_EQUAL(arr[3], "d")
	TEST_EQUAL(arr[4], "e")
	TEST_EQUAL(arr[5], "f")
	TEST_EQUAL(arr[6], "g")
	TEST_EQUAL(arr.size(), 7)
	s4 = "b c d e f g h";
	TEST_EQUAL(s4.split(arr), 7)
	TEST_EQUAL(arr.size(), 7)
	TEST_EQUAL(arr[0], "b")
	TEST_EQUAL(arr[1], "c")
	TEST_EQUAL(arr[2], "d")
	TEST_EQUAL(arr[3], "e")
	TEST_EQUAL(arr[4], "f")
	TEST_EQUAL(arr[5], "g")
	TEST_EQUAL(arr[6], "h")
	s4= "";
	TEST_EQUAL(s4.split(arr), 0)
	TEST_EQUAL(arr.size(), 0)

	s4 = "a/b(cd)e*f-g";
	const char* c = "/()*-";
	TEST_EQUAL(s4.split(arr, c), 6)
	TEST_EQUAL(arr.size(), 6)
	TEST_EQUAL(arr[0], "a")
	TEST_EQUAL(arr[1], "b")
	TEST_EQUAL(arr[2], "cd")
	TEST_EQUAL(arr[3], "e")
	TEST_EQUAL(arr[4], "f")
	TEST_EQUAL(arr[5], "g")

	s4 = "b/c d e/f g h";
	TEST_EQUAL(s4.split(arr, c, 2), 2)
	TEST_EQUAL(arr[0], "c d e")
	TEST_EQUAL(arr[1], "f g h")
	TEST_EQUAL(arr.size(), 2)
RESULT

CHECK(Size splitQuoted(std::vector<String>& strings, const char* delimiters = CHARACTER_CLASS__WHITESPACE, const char* quotes = CHARACTER_CLASS__QUOTES, Index from = 0) const throw(Exception::IndexUnderflow, Exception::NullPointer))
	vector<String> arr;
	s4 = "a b c d e f g";
	TEST_EQUAL(s4.splitQuoted(arr), 7)
	TEST_EQUAL(arr.size(), 7)
	if (arr.size() > 0)	
	{
		TEST_EQUAL(arr[0], "a")
	}
	if (arr.size() > 1)	TEST_EQUAL(arr[1], "b")	
	if (arr.size() > 2)	TEST_EQUAL(arr[2], "c")
	if (arr.size() > 3)	TEST_EQUAL(arr[3], "d")
	if (arr.size() > 4)	TEST_EQUAL(arr[4], "e")
	if (arr.size() > 5)	TEST_EQUAL(arr[5], "f")
	if (arr.size() > 6)	TEST_EQUAL(arr[6], "g")
	s4 = "b c d e f g h";
	TEST_EQUAL(s4.splitQuoted(arr), 7)
	TEST_EQUAL(arr.size(), 7)
	if (arr.size() > 0) TEST_EQUAL(arr[0], "b")
	if (arr.size() > 1) TEST_EQUAL(arr[1], "c")
	if (arr.size() > 2) TEST_EQUAL(arr[2], "d")
	if (arr.size() > 3) TEST_EQUAL(arr[3], "e")
	if (arr.size() > 4) TEST_EQUAL(arr[4], "f")
	if (arr.size() > 5) TEST_EQUAL(arr[5], "g")
	if (arr.size() > 6) TEST_EQUAL(arr[6], "h")
	s4= "";
	TEST_EQUAL(s4.splitQuoted(arr), 0)
	TEST_EQUAL(arr.size(), 0)

	s4 = "a/b(cd)e*f-g";
	const char* c = "/()*-";
	TEST_EQUAL(s4.splitQuoted(arr, c), 6)
	TEST_EQUAL(arr.size(), 6)
	if (arr.size() > 0) TEST_EQUAL(arr[0], "a")
	if (arr.size() > 1) TEST_EQUAL(arr[1], "b")
	if (arr.size() > 2) TEST_EQUAL(arr[2], "cd")
	if (arr.size() > 3) TEST_EQUAL(arr[3], "e")
	if (arr.size() > 4) TEST_EQUAL(arr[4], "f")
	if (arr.size() > 5) TEST_EQUAL(arr[5], "g")

	s4 = "b/c d e/f g h";
	TEST_EQUAL(s4.splitQuoted(arr, c, String::CHARACTER_CLASS__QUOTES, 2), 2)
	if (arr.size() > 0) TEST_EQUAL(arr[0], "c d e")
	if (arr.size() > 1) TEST_EQUAL(arr[1], "f g h")
	TEST_EQUAL(arr.size(), 2)
RESULT

CHECK(String& trimLeft(const char* trimmed = CHARACTER_CLASS__WHITESPACE) throw())
	s4 = "\t     abc  \t";
	TEST_EQUAL(s4.trimLeft(), "abc  \t")
	s4 = "    ";
	TEST_EQUAL(s4.trimLeft(), "")
	s4 = "abba";
	TEST_EQUAL(s4.trimLeft("a"), "bba")
	s4 = "abba";
	TEST_EQUAL(s4.trimLeft("c"), "abba")
	s4 = "";
	TEST_EQUAL(s4.trimLeft(), "")
RESULT

CHECK(String& trimRight(const char* trimmed = CHARACTER_CLASS__WHITESPACE) throw())
	s4 = "\t     abc  \t";
	TEST_EQUAL(s4.trimRight(), "\t     abc")
	s4 = "    ";
	TEST_EQUAL(s4.trimRight(), "")
	s4 = "abba";
	TEST_EQUAL(s4.trimRight("a"), "abb")
	s4 = "abba";
	TEST_EQUAL(s4.trimRight("c"), "abba")
	s4 = "";
	TEST_EQUAL(s4.trimRight(), "")
RESULT

CHECK(String trim(const char* trimmed = CHARACTER_CLASS__WHITESPACE) const throw())
	s4 = "\t     a b\tc  \t";
	TEST_EQUAL(s4.trim(), "a b\tc")
	s4 = "    ";
	TEST_EQUAL(s4.trim(), "")
	s4 = "abba";
	TEST_EQUAL(s4.trim("a"), "bb")
	s4 = "abba";
	TEST_EQUAL(s4.trim("c"), "abba")
	s4 = "";
	TEST_EQUAL(s4.trim(), "")
RESULT

CHECK(String& trim(const char* trimmed = CHARACTER_CLASS__WHITESPACE) throw())
	s4 = "\t     a b\tc  \t";
	const String& c = s4;
	TEST_EQUAL(c.trim(), "a b\tc")
	s4 = "    ";
	TEST_EQUAL(c.trim(), "")
	s4 = "abba";
	TEST_EQUAL(c.trim("a"), "bb")
	s4 = "abba";
	TEST_EQUAL(c.trim("c"), "abba")
	s4 = "";
	TEST_EQUAL(c.trim(), "")
RESULT

CHECK(String& truncate(Size size) throw())
	s4 = "abcd";
	TEST_EQUAL(s4.truncate(2), "ab")
	TEST_EQUAL(s4.truncate(5), "ab")
	s4 = "";
	TEST_EQUAL(s4.truncate(5), "")
RESULT

CHECK(Substring left(Size len) const throw())
	s4 = "abcdefg";
	TEST_EQUAL(s4.left(4), "abcd")
	TEST_EQUAL(s4.left(20), "abcdefg")
	TEST_EQUAL(s4.left(0), "")
	s4 = "";
	TEST_EQUAL(s4.left(4), "")
	TEST_EQUAL(s4.left(0), "")
RESULT

CHECK(Substring right(Size len) const throw())
	s4 = "abcdefg";
	TEST_EQUAL(s4.right(4), "defg")
	TEST_EQUAL(s4.right(20), "abcdefg")
	TEST_EQUAL(s4.right(0), "")
	s4 = "";
	TEST_EQUAL(s4.right(4), "")
	TEST_EQUAL(s4.right(0), "")
RESULT

CHECK(Substring instr(const String& pattern, Index from = 0) const throw())
	s4 = "abcdefghabcdefg";
	TEST_EQUAL(s4.instr("defg"), "defg")
	TEST_EQUAL(s4.instr("defgh"), "defgh")
	TEST_EQUAL(s4.instr("defg", 5), "defg")
	TEST_EQUAL(s4.instr("defgh", 5), "")
	TEST_EQUAL(s4.instr("degf"), "")
	TEST_EQUAL(s4.instr(""), "")
	s4 = "";
	TEST_EQUAL(s4.instr("a"), "")
	TEST_EQUAL(s4.instr(""), "")
RESULT

CHECK(String operator + (const string& string) const throw())
	string stl_str("def");
	s4 = "abc";
	TEST_EQUAL(s4 + stl_str, "abcdef")
	stl_str.assign("");
	s4 = "abc";
	TEST_EQUAL(s4 + stl_str, "abc")
	stl_str.assign("def");
	s4 = "";
	TEST_EQUAL(s4 + stl_str, "def")
	stl_str.assign("");
	s4 = "";
	TEST_EQUAL(s4 + stl_str, "")
RESULT

CHECK(friend String operator + (const char* char_ptr, const String& s) throw())
	s4 = "abc";
	TEST_EQUAL(s4 + "def", "abcdef")
	TEST_EQUAL(s4 + "", "abc")
	s4 = "";
	TEST_EQUAL(s4 + "def", "def")
	TEST_EQUAL(s4 + "", "")
RESULT

CHECK(String operator + (char c) const throw())
	s4 = "abc";
	TEST_EQUAL(s4 + 'd', "abcd")
	s4 = "";
	TEST_EQUAL(s4 + 'd', "d")
RESULT

CHECK(String operator + (const char* char_ptr) const throw())
	s4 = "abc";
	TEST_EQUAL("def" + s4, "defabc")
	TEST_EQUAL("" + s4, "abc")
	s4 = "";
	TEST_EQUAL("def" + s4, "def")
	TEST_EQUAL("" + s4, "")
RESULT

CHECK(friend String operator + (char c, const String& s) throw())
	s4 = "abc";
	TEST_EQUAL('d' + s4, "dabc")
	s4 = "";
	TEST_EQUAL('d' + s4, "d")
RESULT

CHECK(void swap(String& s) throw())
	s4 = "abc";
	String s5 = "def";
	s4.swap(s5);
	TEST_EQUAL(s4, "def")
	TEST_EQUAL(s5, "abc")
	s4 = "a";
	s5 = "bcdefgh";
	s4.swap(s5);
	TEST_EQUAL(s4, "bcdefgh")
	TEST_EQUAL(s5, "a")
	s4 = "";
	s5 = "bcd";
	s4.swap(s5);
	TEST_EQUAL(s4, "bcd")
	TEST_EQUAL(s5, "")
	s4 = "abc";
	s5 = "";
	s4.swap(s5);
	TEST_EQUAL(s4, "")
	TEST_EQUAL(s5, "abc")
RESULT

CHECK(String& reverse(Index from = 0, Size len = EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abcd";
	s4.reverse();
	TEST_EQUAL(s4, "dcba")
	s4.reverse();
	TEST_EQUAL(s4, "abcd")
	s4 = "abcde";
	s4.reverse();
	TEST_EQUAL(s4, "edcba")
	s4.reverse();
	TEST_EQUAL(s4, "abcde")
	s4 = "a";
	s4.reverse();
	TEST_EQUAL(s4, "a")
	s4.reverse();
	TEST_EQUAL(s4, "a")
	s4 = "";
	s4.reverse();
	TEST_EQUAL(s4, "")
	s4 = "abcde";
	s4.reverse(1);
	TEST_EQUAL(s4, "aedcb")
	s4 = "abcde";
	s4.reverse(1, 2);
	TEST_EQUAL(s4, "acbde")
	s4 = "abcde";
	s4.reverse(-2, 2);
	TEST_EQUAL(s4, "abced")
	s4 = "abcde";
	s4.reverse(0, 0);
	TEST_EQUAL(s4, "abcde")
RESULT

CHECK(Size substitute(const String& to_replace, const String& replacing) throw())
	s4 = "abcdef";
	s4.substitute("ab", "XX");
	TEST_EQUAL(s4, "XXcdef")
	s4 = "";
	s4.substitute("ab", "XX");
	TEST_EQUAL(s4, "")
	s4 = "abcdefabcdef";
	s4.substitute("ab", "XX");
	TEST_EQUAL(s4, "XXcdefabcdef")
	s4 = "abcdefabcdef";
	s4.substitute("ij", "XX");
	TEST_EQUAL(s4, "abcdefabcdef")
	s4 = "abcdefabcdef";
	s4.substitute("", "XX");
	TEST_EQUAL(s4, "XXabcdefabcdef")
RESULT

CHECK(bool has(char c) const throw())
	s4 = "abcdef";
	TEST_EQUAL(s4.has('a'), true)
	TEST_EQUAL(s4.has('b'), true)
	TEST_EQUAL(s4.has('c'), true)
	TEST_EQUAL(s4.has('d'), true)
	TEST_EQUAL(s4.has('e'), true)
	TEST_EQUAL(s4.has('f'), true)
	TEST_EQUAL(s4.has('g'), false)
	s4 = "";
	TEST_EQUAL(s4.has('a'), false)
RESULT

CHECK(bool hasSubstring(const String& s, Index from = 0) const throw())
	s4 = "abcdefghabcdef";
	TEST_EQUAL(s4.hasSubstring("abc"), true)
	TEST_EQUAL(s4.hasSubstring(""), true)
	TEST_EQUAL(s4.hasSubstring("xyz"), false)
	TEST_EQUAL(s4.hasSubstring("fg"), true)
	TEST_EQUAL(s4.hasSubstring("fg", 7), false)
	TEST_EQUAL(s4.hasSubstring("ef", -3), false)
	TEST_EQUAL(s4.hasSubstring("ef", -3), false)
	s4 = "";
	TEST_EQUAL(s4.hasSubstring(""), true)
	TEST_EQUAL(s4.hasSubstring("a"), false)
RESULT

CHECK(bool hasPrefix(const String& s) const throw())
	s4 = "abcdefgh";
	TEST_EQUAL(s4.hasPrefix(""), true)
	TEST_EQUAL(s4.hasPrefix("abcd"), true)
	TEST_EQUAL(s4.hasPrefix("abce"), false)
	s4 = "";
	TEST_EQUAL(s4.hasPrefix(""), true)
	TEST_EQUAL(s4.hasPrefix("a"), false)
RESULT

CHECK(bool hasSuffix(const String& s) const throw())
	s4 = "abcdefgh";
	TEST_EQUAL(s4.hasSuffix(""), true)
	TEST_EQUAL(s4.hasSuffix("fgh"), true)
	TEST_EQUAL(s4.hasSuffix("fhh"), false)
	s4 = "";
	TEST_EQUAL(s4.hasSuffix(""), true)
	TEST_EQUAL(s4.hasSuffix("h"), false)
RESULT

CHECK(bool isEmpty() const throw())
	s4 = "a";
	TEST_EQUAL(s4.isEmpty(), false)
	s4 = "ab";
	TEST_EQUAL(s4.isEmpty(), false)
	s4 = "";
	TEST_EQUAL(s4.isEmpty(), true)
RESULT

CHECK(static bool isAlpha(char c) throw())
	s4 = "abc";
	TEST_EQUAL(s4.isAlpha(), true)
	s4 = "a";
	TEST_EQUAL(s4.isAlpha(), true)
	s4 = "ab c";
	TEST_EQUAL(s4.isAlpha(), false)
	s4 = "abc.";
	TEST_EQUAL(s4.isAlpha(), false)
	s4 = "abc9";
	TEST_EQUAL(s4.isAlpha(), false)
	s4 = "";
	TEST_EQUAL(s4.isAlpha(), true)
RESULT

CHECK(bool isAlnum() const throw())
	s4 = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	TEST_EQUAL(s4.isAlnum(), true)
	s4 = "123";
	TEST_EQUAL(s4.isAlnum(), true)
	s4 = "a";
	TEST_EQUAL(s4.isAlnum(), true)
	s4 = "0";
	TEST_EQUAL(s4.isAlnum(), true)
	s4 = "ab c";
	TEST_EQUAL(s4.isAlnum(), false)
	s4 = "abc.";
	TEST_EQUAL(s4.isAlnum(), false)
	s4 = "abc9";
	TEST_EQUAL(s4.isAlnum(), true)
	s4 = "";
	TEST_EQUAL(s4.isAlnum(), true)
RESULT

CHECK(bool isDigit() const throw())
	s4 = "";
	TEST_EQUAL(s4.isDigit(), true)
	s4 = "1";
	TEST_EQUAL(s4.isDigit(), true)
	s4 = "1234567890";
	TEST_EQUAL(s4.isDigit(), true)
	s4 = "1234a67890";
	TEST_EQUAL(s4.isDigit(), false)
RESULT

CHECK(bool isSpace() const throw())
	s4 = "";
	TEST_EQUAL(s4.isSpace(), true)
	s4 = " ";
	TEST_EQUAL(s4.isSpace(), true)
	s4 = "         ";
	TEST_EQUAL(s4.isSpace(), true)
	s4 = "\t";
	TEST_EQUAL(s4.isSpace(), false)
	s4 = " \t\r\n ";
	TEST_EQUAL(s4.isSpace(), false)
	s4 = "      \t\ta";
	TEST_EQUAL(s4.isSpace(), false)
RESULT

CHECK(bool isWhitespace() const throw())
	s4 = "";
	TEST_EQUAL(s4.isWhitespace(), true)
	s4 = " ";
	TEST_EQUAL(s4.isWhitespace(), true)
	s4 = "\t";
	TEST_EQUAL(s4.isWhitespace(), true)
	s4 = " \t\r\n ";
	TEST_EQUAL(s4.isWhitespace(), true)
	s4 = "      \t\ta";
	TEST_EQUAL(s4.isWhitespace(), false)
RESULT

CHECK(bool isFloat() const throw())
	s4 = "";
	TEST_EQUAL(s4.isFloat(), true)
	s4 = "z";
	TEST_EQUAL(s4.isFloat(), false)
	s4 = "1";
	TEST_EQUAL(s4.isFloat(), true)
	s4 = "1.2";
	TEST_EQUAL(s4.isFloat(), true)
	s4 = "1..2";
	TEST_EQUAL(s4.isFloat(), false)
	s4 = ".2";
	TEST_EQUAL(s4.isFloat(), true)
	s4 = "99999999999999999999999999999999999999999999999999999999";
	TEST_EQUAL(s4.isFloat(), true)
	s4 ="-1.244e8";
	TEST_EQUAL(s4.isFloat(), true)
	s4 ="-1.244e890";
	TEST_EQUAL(s4.isFloat(), false)
RESULT

CHECK(bool isAlpha() const throw())
	String char_class = String::CHARACTER_CLASS__ASCII_ALPHA;
	for (char c = CHAR_MIN; c != CHAR_MAX; c++)
	{
		TEST_EQUAL(String::isAlpha(c), char_class.has(c))
	}
RESULT

CHECK(static bool isAlnum(char c) throw())
	String char_class = String::CHARACTER_CLASS__ASCII_ALPHANUMERIC;
	for (char c = CHAR_MIN; c != CHAR_MAX; c++)
	{
		TEST_EQUAL(String::isAlnum(c), char_class.has(c))
	}
RESULT

CHECK(static bool isDigit(char c) throw())
	String char_class = String::CHARACTER_CLASS__ASCII_NUMERIC;
	for (char c = CHAR_MIN; c != CHAR_MAX; c++)
	{
		TEST_EQUAL(String::isDigit(c), char_class.has(c))
	}
RESULT

CHECK(static bool isWhitespace(char c) throw())
	String char_class = String::CHARACTER_CLASS__WHITESPACE;
	for (unsigned char c = '\0'; c != (unsigned char)255; c++)
	{
		TEST_EQUAL(String::isWhitespace((char)c), char_class.has((char)c))
	}
RESULT

CHECK(static bool isSpace(char c) throw())
	for (unsigned char c = '\0'; c != (unsigned char)255; c++)
	{
		TEST_EQUAL(String::isSpace((char)c), ((char)c == ' '))
	}
RESULT

CHECK(int compare(const String& string, Index from = 0) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abc";
	String s5 = "abc";
	TEST_EQUAL(s4.compare(s5), 0)
	s5 = "abcd";
	TEST_EQUAL((s4.compare(s5) < 0), true)
	s5 = "ab";
	TEST_EQUAL((s4.compare(s5) > 0), true)
	s5 = "xbcd";
	TEST_EQUAL((s4.compare(s5) < 0), true)
	s5 = "xb";
	TEST_EQUAL((s4.compare(s5) > 0), false)
	s4 = "";
	s5 = "";
	TEST_EQUAL(s4.compare(s5), 0)
	s5 = "a";
	TEST_EQUAL((s4.compare(s5) < 0), true)
	s4 = "abc";
	s5 = "abc";
	TEST_EQUAL(s4.compare(s5, 0), 0)
	TEST_EQUAL(s4.compare(s5, -3), 0)
	s5 = "bc";
	TEST_EQUAL(s4.compare(s5, 1), 0)
	TEST_EQUAL(s4.compare(s5, -2), 0)
	s5 = "c";
	TEST_EQUAL(s4.compare(s5, 2), 0)
	TEST_EQUAL(s4.compare(s5, -1), 0)
	s5 = "abd";
	TEST_NOT_EQUAL(s4.compare(s5, 0), 0)
RESULT

CHECK(int compare(const String& string, Index from, Size len) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s4 = "abc";
	String s5 = "abc";
	TEST_EQUAL(s4.compare(s5, 0, 1), 0)
	TEST_EQUAL(s4.compare(s5, 0, 2), 0)
	TEST_EQUAL(s4.compare(s5, 0, 3), 0)
	TEST_EXCEPTION(Exception::IndexOverflow, s4.compare(s5, 0, 4))
	s5 = "axxx";
	TEST_EQUAL(s4.compare(s5, 0, 1), 0)
	TEST_EQUAL(s4.compare("axxx", -3, 1), 0)
	s5 = "abxx";
	TEST_EQUAL(s4.compare(s5, 0, 2), 0)
	TEST_EQUAL(s4.compare(s5, -3, 2), 0)
	s5 = "abcx";
	TEST_EQUAL(s4.compare(s5, 0, 3), 0)
	TEST_EQUAL(s4.compare(s5, -3, 3), 0)
	s5 = "bxxx";
	TEST_EQUAL(s4.compare(s5, 1, 1), 0)
	TEST_EQUAL(s4.compare(s5, -2, 1), 0)
	s5 = "bcxx";
	TEST_EQUAL(s4.compare(s5, 1, 2), 0)
	TEST_EQUAL(s4.compare(s5, -2, 2), 0)
	s5 = "cxxx";
	TEST_EQUAL(s4.compare(s5, 2, 1), 0)
	TEST_EQUAL(s4.compare(s5, -1, 1), 0)
	s5 = "bd";
	TEST_NOT_EQUAL(s4.compare(s5, 1, 2), 0) 
	s5 = "abcd";
	TEST_EQUAL(s4.compare(s5, 0, 3), 0) 
	s5 = "abd";
	TEST_EQUAL(s4.compare(s5, 0, 2), 0) 
	TEST_NOT_EQUAL(s4.compare(s5, 0, 3), 0) 
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
