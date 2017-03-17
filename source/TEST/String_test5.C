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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
