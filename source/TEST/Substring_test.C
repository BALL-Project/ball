// $Id: Substring_test.C,v 1.2 2002/12/12 11:34:45 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/string.h>
#include <string.h>
#include <string>
///////////////////////////

START_TEST(String,"$Id: Substring_test.C,v 1.2 2002/12/12 11:34:45 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using std::string;

// Substring class....
Substring* ptr = 0;
CHECK(Substring::Substring())
	ptr = new Substring;
	TEST_NOT_EQUAL(ptr, 0)
RESULT

CHECK(Substring::~Substring())
	delete ptr;
RESULT

CHECK(Substring::Substring(String&))
	String s("abcdef");
	Substring sub(s);
	TEST_EQUAL(sub, s)
RESULT

CHECK(Substring::Substring(Subtring&))
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

CHECK(Substring::Substring(String&, Index, Size))
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

CHECK(Substring::create(bool))
	Substring sub(s5);
	Substring* sub2 = (Substring*)sub.create(true);
	TEST_EQUAL(sub, *sub2)
	delete sub2;
	sub2 = (Substring*)sub.create(false);
	TEST_EQUAL(sub, *sub2)
	delete sub2;
RESULT

CHECK(Substring::destroy())
	String test_destroy("abcdefghij");
	Substring sub(test_destroy, 3, 3);
	TEST_EQUAL(sub.isBound(), true)
	TEST_EQUAL(sub.toString(), "def")
	sub.destroy();
	TEST_EQUAL(sub.isBound(), false)
	TEST_EQUAL(test_destroy, "abcghij")
RESULT

CHECK(Substring::operator String ())
	Substring sub(s5);
	String s6 = (String)sub;
	TEST_EQUAL(s6, s5)
	Substring empty_sub(s5, 0, 0);
	s6 = (String)empty_sub;
	TEST_EQUAL(s6, "")
RESULT

CHECK(Substring::toString())
	Substring sub(s5);
	String s6 = sub.toString();
	TEST_EQUAL(s6, s5)
RESULT

CHECK(Substring::bind(const String& string, Index from = 0, Size len = string::npos))
	Substring sub;
	sub.bind(ABCDEF, 1, 1);
	TEST_EQUAL(sub, "B")
RESULT

CHECK(Substring::bind(const Substring& substring, Index from = 0, Size len = string::npos))
	Substring sub;
	sub.bind(ABCDEF, 1, 1);
	TEST_EQUAL(sub, "B")
RESULT

CHECK(Substring::unbind())
	Substring sub;
	sub.bind(test_sub1, 1, 1);
	sub.unbind();
	TEST_EQUAL(sub.getBoundString(), 0)
RESULT

CHECK(Substring::getBoundString())
	Substring sub;
	sub.bind(test_sub1, 1, 1);
	TEST_EQUAL(sub.getBoundString(), &ABCDEF)
RESULT

CHECK(Substring::getBoundString() const )
	String* p = test_sub1.getBoundString();
	TEST_EQUAL(p, &ABCDEF)
RESULT

CHECK(Substring::set(const String& string))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1.set(ABCDEF);
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.set(ABCDEF))	
RESULT

CHECK(Substring::set(const Substring& s))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub2.bind(ABCDEF);
	test_sub1.set(test_sub2);
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.set(test_sub1))	
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1.set(empty_sub))	
RESULT

CHECK(Substring::set(const char* char_ptr, Size size = string::npos))
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

CHECK(Substring::Substring& operator = (const String& string))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1 = ABCDEF;
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub = ABCDEF)	
RESULT

CHECK(Substring::Substring& operator = (const Substring& substring))
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

CHECK(Substring::Substring& operator = (const char* char_ptr))
	test_string = "";
	test_sub1.bind(test_string);
	test_sub1 = char1;
	TEST_EQUAL(test_string, "ABCDEF")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub = char1)	
	TEST_EXCEPTION(Exception::NullPointer, test_sub1 = 0)	
RESULT

CHECK(Substring::c_str())
	test_string = "abcdef";
	test_sub1.bind(test_string);
	test_sub1.c_str()[0] = 'A';
	TEST_EQUAL(test_string, "Abcdef")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.c_str()[0] = 'A')	
RESULT

CHECK(Substring::c_str() const )
	const char*	c1 = ABCDEF_sub.c_str();
	const char*	c2 = "ABCDEF";
	TEST_EQUAL(strcmp(c1, c2), 0)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.c_str())	
RESULT

CHECK(Substring::getFirstIndex() const )
	String temp = "AAAA";
	test_sub1.bind(temp, 2, 1);
	TEST_EQUAL(test_sub1.getFirstIndex(), 2)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.getFirstIndex())	
RESULT

CHECK(Substring::getLastIndex() const )
	String temp = "AAAA";
	test_sub1.bind(temp, 1, 2);
	TEST_EQUAL(test_sub1.getLastIndex(), 2)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.getLastIndex())	
RESULT

CHECK(Substring::size() const )
	TEST_EQUAL(ABCDEF_sub.size(), 6)
	TEST_EQUAL(empty_sub.size(), 0)
RESULT

CHECK(Substring::char& operator [] (Index index))
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

CHECK(Substring::char operator [] (Index index) const )
	const char c = abcdef_sub[0];
	TEST_EQUAL(c, 'a')
	TEST_EQUAL(abcdef_sub[-1], 'f')
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub[0])	
RESULT

CHECK(Substring::toLower())
	test_string = abcdef + ABCDEF + "1";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1.toLower(), "abcdefabcdef1")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.toLower())	
RESULT

CHECK(Substring::toUpper())
	test_string = abcdef + ABCDEF + "1";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1.toUpper(), "ABCDEFABCDEF1")
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.toUpper())	
RESULT

CHECK(Substring::isBound() const )
	test_sub1.unbind();
	TEST_EQUAL(test_sub1.isBound(), false)
	test_sub1.bind(ABCDEF, 1, 1);
	TEST_EQUAL(test_sub1.isBound(), true)
RESULT

CHECK(Substring::isEmpty() const )
	test_string = "";
	test_sub1.bind(test_string);
	test_string = "A";
	TEST_EQUAL(test_sub1.isEmpty(), false)
	test_sub1.unbind();
	TEST_EQUAL(test_sub1.isEmpty(), true)
RESULT

CHECK(Substring::bool operator == (const Substring& substring) const )
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

CHECK(Substring::bool operator != (const Substring& substring) const )
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

CHECK(Substring::bool operator == (const String& string) const )
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(test_sub1 == ABCDEF, true)
	test_string = "ABCDE";
	TEST_EQUAL(test_sub1 == ABCDEF, false)
	test_sub1.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 == ABCDEF)	
RESULT

CHECK(Substring::bool operator != (const String& string) const )
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(test_sub1 != ABCDEF, false)
	test_string = "ABCDE";
	TEST_EQUAL(test_sub1 != ABCDEF, true)
	test_sub1.unbind();
	TEST_EXCEPTION(Substring::UnboundSubstring, test_sub1 != ABCDEF)	
RESULT

CHECK(Substring::bool operator == (const String& string, const Substring& substring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(ABCDEF == test_sub1, true)
	test_string = "ABCDE";
	TEST_EQUAL(ABCDEF == test_sub1, false)
	TEST_EXCEPTION(Substring::UnboundSubstring, ABCDEF == empty_sub)	
RESULT

CHECK(Substring::bool operator != (const String& string, const Substring& substring))
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 6);
	TEST_EQUAL(ABCDEF != test_sub1, false)
	test_string = "ABCDE";
	TEST_EQUAL(ABCDEF != test_sub1, true)
	TEST_EXCEPTION(Substring::UnboundSubstring, ABCDEF != empty_sub)	
RESULT

CHECK(Substring::bool operator == (const char* char_ptr) const )
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

CHECK(Substring::bool operator != (const char* char_ptr) const )
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

CHECK(Substring::bool operator == (char c) const )
	test_string = "XABCDEFG";
	test_sub1.bind(test_string, 1, 1);
	char c = 'A';
	TEST_EQUAL(test_sub1 == c, true)
	test_string = "ABCDE";
	test_sub1.bind(test_string);
	TEST_EQUAL(test_sub1 == c, false)
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub == c)	
RESULT

CHECK(Substring::bool operator != (char c) const )
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

CHECK(std::ostream& operator << (std::ostream& s, const Substring& substring))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	test_sub1.bind(ABCDEF, 1, 4);
	outstr << test_sub1;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Substring_test2.txt")
	std::ofstream outstr2(filename.c_str(), std::ios::out);
	outstr2 << empty_sub;
	outstr2.close();
RESULT

CHECK(Substring::isValid() const )
	TEST_EQUAL(test_sub1.isValid(), true)
	test_sub1.unbind();
	TEST_EQUAL(test_sub1.isValid(), false)
	test_sub1.bind(test_string, -1, 1);
	TEST_EQUAL(test_sub1.isValid(), true)
RESULT

CHECK(Substring::dump(std::ostream& s = std::cout, Size depth = 0) const )
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	test_sub1.bind(ABCDEF, 1, 4);
	test_sub1.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), "data/Substring_test.txt")
	std::ofstream outfile2(filename.c_str(), ios::out);
	TEST_EXCEPTION(Substring::UnboundSubstring, empty_sub.dump(outfile2))
	outfile2.close();
RESULT

CHECK(String::operator Substring())
	s4 = "abcdef";
	Substring sub((Substring)s4);
	TEST_EQUAL(sub.toString(), s4)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
