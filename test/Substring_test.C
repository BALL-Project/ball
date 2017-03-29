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

// Substring class....
Substring* ptr = 0;
CHECK(Substring() throw())
	ptr = new Substring;
	TEST_NOT_EQUAL(ptr, 0)
RESULT

CHECK(~Substring() throw())
	delete ptr;
RESULT

CHECK(Substring(const Substring& substring, bool deep = true) throw())
	String s = "abcdef";
	Substring sub(s);
	Substring sub2(sub);
	TEST_EQUAL(sub2, s)
RESULT

const char* char1 = "ABCDEF";
const String abcdef = "abcdef";
const String ABCDEF = "ABCDEF";
Substring empty_sub;
const Substring abcdef_sub(abcdef, 0, 6);
const Substring ABCDEF_sub(ABCDEF, 0, 6);
Substring test_sub1(ABCDEF, 0, 6);
Substring test_sub2;
String test_string;
String s5 = "abcdef";
String s4;

CHECK(Substring(const String& string, Index from = 0, Size len = String::EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	Substring sub(s5, 1);
	TEST_EQUAL(sub, "bcdef")
	Substring sub2(s5, 2, 1);
	TEST_EQUAL(sub2, "c")
	Substring sub3(s5, 1, 2);
	TEST_EQUAL(sub3, "bc")
	Substring sub4(s5, -2, 1);
	TEST_EQUAL(sub4, "e")
	Substring* sub5;
	TEST_EXCEPTION(Exception::IndexOverflow, sub5 = new Substring(s5, 6); delete sub5)
	TEST_EXCEPTION(Exception::IndexOverflow, sub5 = new Substring(s5, 0, 7); delete sub5)
	TEST_EXCEPTION(Exception::IndexUnderflow, sub5 = new Substring(s5, -10); delete sub5)
RESULT

CHECK(Substring::~Substring())
	Substring* sub = new Substring(s5);
	delete sub;
RESULT

CHECK(BALL_CREATE_DEEP(Substring))
	Substring sub(s5);
	Substring* sub2 = (Substring*)sub.create(true);
	TEST_EQUAL(sub, *sub2)
	delete sub2;
	sub2 = (Substring*)sub.create(false);
	TEST_EQUAL(sub, *sub2)
	delete sub2;
RESULT

CHECK(void destroy() throw())
	String test_destroy("abcdefghij");
	Substring sub(test_destroy, 3, 3);
	TEST_EQUAL(sub.isBound(), true)
	TEST_EQUAL(sub.toString(), "def")
	sub.destroy();
	TEST_EQUAL(sub.isBound(), false)
	TEST_EQUAL(test_destroy, "abcghij")
RESULT

CHECK(operator String() const throw(Substring::UnboundSubstring))
	Substring sub(s5);
	String s6 = (String)sub;
	TEST_EQUAL(s6, s5)
	Substring empty_sub(s5, 0, 0);
	s6 = (String)empty_sub;
	TEST_EQUAL(s6, "")
RESULT

CHECK(String toString() const throw(Substring::UnboundSubstring))
	Substring sub(s5);
	String s6 = sub.toString();
	TEST_EQUAL(s6, s5)
RESULT

CHECK(Substring& bind(const String& string, Index from = 0, Size len = String::EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	Substring sub;
	sub.bind(ABCDEF, 1, 1);
	TEST_EQUAL(sub, "B")
RESULT

CHECK(Substring& bind(const Substring& substring, Index from = 0, Size len = String::EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	Substring sub;
	sub.bind(ABCDEF, 1, 1);
	TEST_EQUAL(sub, "B")
RESULT

CHECK(void unbind() throw())
	Substring sub;
	sub.bind(test_sub1, 1, 1);
	sub.unbind();
	TEST_EQUAL(sub.getBoundString(), 0)
RESULT

CHECK(String* getBoundString() throw())
	Substring sub;
	sub.bind(test_sub1, 1, 1);
	TEST_EQUAL(sub.getBoundString(), &ABCDEF)
RESULT

CHECK(const String* getBoundString() const throw())
	String* p = test_sub1.getBoundString();
	TEST_EQUAL(p, &ABCDEF)
RESULT

CHECK(void set(const String& string) throw(Substring::UnboundSubstring))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1.set(ABCDEF);
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.set(ABCDEF))	
RESULT

CHECK(void set(const Substring& s) throw(Substring::UnboundSubstring))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub2.bind(ABCDEF);
	test_sub1.set(test_sub2);
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.set(test_sub1))	
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1.set(empty_sub))	
RESULT

CHECK(void set(const char* char_ptr, Size size = String::EndPos) throw(Substring::UnboundSubstring, Exception::NullPointer, Exception::SizeUnderflow))
	String s = "AB12CDEF";
	Substring sub(s, 2, 2);
	TEST_EQUAL(sub, "12")
	sub.set("test");
	TEST_EQUAL(s, "ABtestCDEF")
	TEST_EXCEPTION(Exception::NullPointer, test_sub1.set(0))	
	TEST_EXCEPTION(Exception::SizeUnderflow, test_sub1.set("test", 0))	
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.set(s))	
RESULT

CHECK(bool operator == (const String& string) const throw(Substring::UnboundSubstring))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1 = ABCDEF;
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub = ABCDEF)	
RESULT

CHECK(const Substring& operator = (const Substring& substring) throw(Substring::UnboundSubstring))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1 = ABCDEF_sub;
	TEST_EQUAL(test_string, "ABCDEF")
	test_sub1.unbind();
	test_sub2.bind(ABCDEF);
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 = test_sub2)
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub2 = test_sub1)
	test_sub2.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub2 = test_sub1)
RESULT

CHECK(const Substring& operator = (const char* char_ptr) throw(Substring::UnboundSubstring, Exception::NullPointer))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1 = char1;
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub = char1)	
	TEST_EXCEPTION(Exception::NullPointer, test_sub1 = 0)	
RESULT

CHECK(char* c_str() throw(Substring::UnboundSubstring))
	test_string = "abcdef";
	test_sub1.bind(test_string);
	test_sub1.c_str()[0] = 'A';
	TEST_EQUAL(test_string, "Abcdef")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.c_str()[0] = 'A')	
RESULT

CHECK(const char* c_str() const throw(Substring::UnboundSubstring))
	const char*	c1 = ABCDEF_sub.c_str();
	const char*	c2 = "ABCDEF";
	TEST_EQUAL(strcmp(c1, c2), 0)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.c_str())	
RESULT

CHECK(Index getFirstIndex() const throw(Substring::UnboundSubstring))
	String temp = "AAAA";
	test_sub1.bind(temp, 2, 1);
	TEST_EQUAL(test_sub1.getFirstIndex(), 2)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.getFirstIndex())	
RESULT

CHECK(Index getLastIndex() const throw(Substring::UnboundSubstring))
	String temp = "AAAA";
	test_sub1.bind(temp, 1, 2);
	TEST_EQUAL(test_sub1.getLastIndex(), 2)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.getLastIndex())	
RESULT

CHECK(Size size() const throw())
	TEST_EQUAL(ABCDEF_sub.size(), 6)
	TEST_EQUAL(empty_sub.size(), 0)
RESULT

CHECK(char& operator [] (Index index) throw(Substring::UnboundSubstring, Exception::IndexUnderflow, Exception::IndexOverflow))
	test_string = "ABCDEF";
	test_sub1.bind(test_string);
	test_sub1[0] = 'a';
	test_sub1[5] = 'f';
	TEST_EQUAL(test_string, "aBCDEf")
	test_sub1[-1] = 'F';
	TEST_EQUAL(test_string, "aBCDEF")
	TEST_EXCEPTION(Exception::IndexOverflow, test_sub1[ 6] = 'F')
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub[0] = 'F')	
RESULT

CHECK(char operator [] (Index index) const throw(Substring::UnboundSubstring, Exception::IndexUnderflow, Exception::IndexOverflow))
	const char c = abcdef_sub[0];
	TEST_EQUAL(c, 'a')
	TEST_EQUAL(abcdef_sub[-1], 'f')
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub[0])	
RESULT

CHECK(Substring& toLower() throw(Substring::UnboundSubstring))
	test_string = abcdef + ABCDEF + "1";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1.toLower(), "abcdefabcdef1")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.toLower())	
RESULT

CHECK(Substring& toUpper() throw(Substring::UnboundSubstring))
	test_string = abcdef + ABCDEF + "1";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1.toUpper(), "ABCDEFABCDEF1")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.toUpper())	
RESULT

CHECK(bool isBound() const throw())
	test_sub1.unbind();
	TEST_EQUAL(test_sub1.isBound(), false)
	test_sub1.bind(ABCDEF, 1, 1);
	TEST_EQUAL(test_sub1.isBound(), true)
RESULT

CHECK(bool isEmpty() const throw())
	test_string = "";
	test_sub1.bind(test_string);
	test_string = "A";
	TEST_EQUAL(test_sub1.isEmpty(), false)
	test_sub1.unbind();
	TEST_EQUAL(test_sub1.isEmpty(), true)
RESULT

CHECK(bool operator == (const Substring& substring) const throw(Substring::UnboundSubstring))
	test_sub1.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 == test_sub2)	
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub2 == test_sub1)	
	test_sub1.bind(ABCDEF, 0, 5);
	test_sub2.bind(ABCDEF, 0, 5);
	TEST_EQUAL(test_sub1 == test_sub2, true)
	test_sub1.bind(ABCDEF, 0, 3);
	TEST_EQUAL(test_sub1 == test_sub2, false)
	test_string = "XXX";
	test_sub2.bind(test_string);
	TEST_EQUAL(test_sub1 == test_sub2, false)
RESULT

CHECK(bool operator != (const Substring& substring) const throw(Substring::UnboundSubstring))
	test_sub1.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 != test_sub2)	
	TEST_EXCEPTION(Substring::UnboundSubstring,  test_sub2 != test_sub1)	
	test_sub1.bind(ABCDEF, 0, 5);
	test_sub2.bind(ABCDEF, 0, 5);
	TEST_EQUAL(test_sub1 != test_sub2, false)
	test_sub1.bind(ABCDEF, 0, 3);
	TEST_EQUAL(test_sub1 != test_sub2, true)
	test_string = "XXX";
	test_sub2.bind(test_string);
	TEST_EQUAL(test_sub1 != test_sub2, true)
RESULT

CHECK(const Substring& operator = (const String& string) throw(Substring::UnboundSubstring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(test_sub1 == ABCDEF, true)
	test_string = "ABCDE";
	TEST_EQUAL(test_sub1 == ABCDEF, false)
	test_sub1.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 == ABCDEF)	
RESULT

CHECK(friend bool operator != (const String& string, const Substring& substring) throw(Substring::UnboundSubstring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(test_sub1 != ABCDEF, false)
	test_string = "ABCDE";
	TEST_EQUAL(test_sub1 != ABCDEF, true)
	test_sub1.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 != ABCDEF)	
RESULT

CHECK(friend bool operator == (const String& string, const Substring& substring) throw(Substring::UnboundSubstring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(ABCDEF == test_sub1, true)
	test_string = "ABCDE";
	TEST_EQUAL(ABCDEF == test_sub1, false)
	TEST_EXCEPTION(Substring::UnboundSubstring, ABCDEF == empty_sub)	
RESULT

CHECK(bool operator != (const String& string) const throw(Substring::UnboundSubstring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(ABCDEF != test_sub1, false)
	test_string = "ABCDE";
	TEST_EQUAL(ABCDEF != test_sub1, true)
	TEST_EXCEPTION(Substring::UnboundSubstring, ABCDEF != empty_sub)	
RESULT

CHECK(bool operator == (const char* char_ptr) const throw(Substring::UnboundSubstring, Exception::NullPointer))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(test_sub1 == char1, true)
	test_string = "ABCDE";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1 == char1, false)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub == char1)	
	char* c = 0;
	TEST_EXCEPTION(Exception::NullPointer, test_sub1 == c)	
RESULT

CHECK(bool operator != (const char* char_ptr) const throw(Substring::UnboundSubstring, Exception::NullPointer))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(test_sub1 != char1, false)
	test_string = "ABCDE";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1 != char1, true)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub != char1)	
	char* c = 0;
	TEST_EXCEPTION(Exception::NullPointer, test_sub1 != c)	
RESULT

CHECK(bool operator == (char c) const throw(Substring::UnboundSubstring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 1);
	char c = 'A';
	TEST_EQUAL(test_sub1 == c, true)
	test_string = "ABCDE";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1 == c, false)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub == c)	
RESULT

CHECK(bool operator != (char c) const throw(Substring::UnboundSubstring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 1);
	char c = 'A';
	TEST_EQUAL(test_sub1 != c, false)
	test_string = "ABCDE";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1 != c, true)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub != c)	
RESULT

String filename;
using std::ofstream;
using std::ios;

CHECK(friend std::ostream& operator << (std::ostream& s, const Substring& substring) throw())
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	test_sub1.bind(ABCDEF, 1, 4);
	outstr << test_sub1;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Substring_test2.txt))
	std::ofstream outstr2(filename.c_str(), std::ios::out);
	outstr2 << empty_sub;
	outstr2.close();
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(test_sub1.isValid(), true)
	test_sub1.unbind();
	TEST_EQUAL(test_sub1.isValid(), false)
	test_sub1.bind(test_string, -1, 1);
	TEST_EQUAL(test_sub1.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw(Substring::UnboundSubstring))
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	test_sub1.bind(ABCDEF, 1, 4);
	test_sub1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Substring_test.txt))
	std::ofstream outfile2(filename.c_str(), ios::out);
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.dump(outfile2))
	outfile2.close();
RESULT

CHECK(void clear() throw())
	Substring sub(s5, 1);
	TEST_EQUAL(sub, "bcdef")
	sub.clear();
	TEST_EQUAL(sub.isBound(), false)
	TEST_EXCEPTION(Substring::UnboundSubstring, sub.c_str())
RESULT
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
