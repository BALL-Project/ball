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

String s4;

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
