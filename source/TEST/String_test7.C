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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
