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

String* s;
String* s2;
String halloh("Halloh");

CHECK(String(unsigned short us) throw())
	s2 = new String((unsigned short)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((unsigned short)0);
	TEST_EQUAL(*s2, "0")
	delete s2;
RESULT

CHECK(String(int i) throw())
	s2 = new String((int)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((int)-8);
	TEST_EQUAL(*s2, "-8")
	delete s2;
RESULT

CHECK(String(unsigned int ui) throw())
	s2 = new String((unsigned int)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((unsigned int)0);
	TEST_EQUAL(*s2, "0")
	delete s2;
RESULT

CHECK(String(unsigned long) throw())
	s2 = new String((unsigned long)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((unsigned long)0);
	TEST_EQUAL(*s2, "0")
	delete s2;
RESULT

CHECK(String(long l) throw())
	s2 = new String((long)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((long)-8);
	TEST_EQUAL(*s2, "-8")
	delete s2;
RESULT

CHECK(String(float f) throw())
	s2 = new String((float)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	delete s2;
	s2 = new String((float)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
	delete s2;
RESULT

CHECK(String(double d) throw())
	s2 = new String((double)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	delete s2;
	s2 = new String((double)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
	delete s2;
RESULT

CHECK(void destroy() throw())
	s2 = new String("hallo");
	s2->destroy();
	TEST_EQUAL(*s2, "")
RESULT
delete s2;

const String abcdef = "abcdef";
const String ABCDEF = "ABCDEF";
Substring empty_sub;
const Substring abcdef_sub(abcdef, 0, 6);
const Substring ABCDEF_sub(ABCDEF, 0, 6);
Substring test_sub1(ABCDEF, 0, 6);
Substring test_sub2;
String test_string;

CHECK(void set(const String& string, Index from = 0, Size len = EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s2 = new String("AAAA");
	s2->set(halloh);
	TEST_EQUAL(*s2, halloh)
	s2->set(halloh, 1);
	TEST_EQUAL(*s2, "alloh")
	s2->set(halloh, -2);
	TEST_EQUAL(*s2, "oh")
	s2->set(halloh, -2, 0);
	TEST_EQUAL(*s2, "");
	s2->set(halloh, 1, 2);
	TEST_EQUAL(*s2, "al")
	TEST_EXCEPTION(Exception::IndexUnderflow, s2->set(halloh, -10, 3))
	TEST_EXCEPTION(Exception::IndexOverflow, s2->set(halloh, 0, 10))
RESULT
delete s2;

CHECK(void set(const char* char_ptr, Index from = 0, Size len = EndPos) throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow))
	s2 = new String("AAAA");
	s2->set("halloh");
	TEST_EQUAL(*s2, "halloh")
	s2->set("halloh", 1);
	TEST_EQUAL(*s2, "alloh")
	s2->set("halloh", -2);
	TEST_EQUAL(*s2, "oh")
	s2->set("halloh", -2, 0);
	TEST_EQUAL(*s2, "");
	s2->set("halloh", 1, 2);
	TEST_EQUAL(*s2, "al")
	TEST_EXCEPTION(Exception::IndexUnderflow, s2->set("halloh", -10, 3))
	TEST_EXCEPTION(Exception::IndexOverflow, s2->set("halloh", 0, 10))
RESULT
delete s2;

CHECK(void set(Size buffer_size, const char *format, ...) throw(Exception::IndexUnderflow, Exception::NullPointer))
	s2 = new String;
	s2->set(15, "%s", "halloh");
	TEST_EQUAL(*s2, "halloh")
	s2->set(2, "%s", "halloh");
	TEST_EQUAL(*s2, "h")
	s2->set(15, "%3.1f", 1.2);
	TEST_EQUAL(*s2, "1.2")
	TEST_EXCEPTION(Exception::IndexUnderflow, s2->set(0, "%s", "Halloh"))
	TEST_EXCEPTION(Exception::NullPointer, s2->set(15, (char*)0, "Halloh", 1.5, 1.2))
RESULT
delete s2;

CHECK(void set(std::stringstream& s) throw())
	std::stringstream instream("ABC DEF GHI jkl mno");

	s2 = new String;
	s2->set(instream);
	TEST_EQUAL(*s2, "ABC")
	s2->set(instream);
	TEST_EQUAL(*s2, "DEF")
	delete s2;
RESULT

CHECK(void set(std::strstream& s) throw())
	// tested above
RESULT

s2 = new String;
CHECK(void set(char c, Size len = 1) throw())
	s2->set('A');
	TEST_EQUAL(*s2, "A")
	s2->set('B', (Size)2);
	TEST_EQUAL(*s2, "BB");
	s2->set('C', (Size)0);
	TEST_EQUAL(*s2, "")
RESULT

CHECK(void set(unsigned char uc) throw())
	s2->set((unsigned char)'b');
	TEST_EQUAL(*s2, "b")
RESULT

CHECK(void set(short s) throw())
	s2->set((short)7);
	TEST_EQUAL(*s2, "7")
	s2->set((short)-8);
	TEST_EQUAL(*s2, "-8")
RESULT

CHECK(void set(unsigned short us) throw())
	s2->set((unsigned short)7);
	TEST_EQUAL(*s2, "7")
	s2->set((unsigned short)0);
	TEST_EQUAL(*s2, "0")
RESULT

CHECK(void set(int i) throw())
	s2->set((int)7);
	TEST_EQUAL(*s2, "7")
	s2->set((int)-8);
	TEST_EQUAL(*s2, "-8")
RESULT

CHECK(void set(unsigned int ui) throw())
	s2->set((unsigned int)7);
	TEST_EQUAL(*s2, "7")
	s2->set((unsigned int)0);
	TEST_EQUAL(*s2, "0")
RESULT

CHECK(void set(unsigned long ul) throw())
	s2->set((unsigned long)7);
	TEST_EQUAL(*s2, "7")
	s2->set((unsigned long)0);
	TEST_EQUAL(*s2, "0")
RESULT

CHECK(void set(long l) throw())
	s2->set((long)7);
	TEST_EQUAL(*s2, "7")
	s2->set((long)-8);
	TEST_EQUAL(*s2, "-8")
RESULT

CHECK(void set(float f) throw())
	s2->set((float)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	s2->set((float)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
RESULT

CHECK(void set(double d) throw())
	s2->set((double)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	s2->set((double)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
RESULT

CHECK(void get(char* char_ptr, Index from = 0, Size len = EndPos) const throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow))
	char test_string[15];
	s2->set("TEST");
	s2->get(test_string);
	TEST_EQUAL(strlen(test_string), 4);
	TEST_EQUAL(strcmp(test_string, "TEST"), 0)
	s2->get(test_string, 1);
	TEST_EQUAL(strcmp(test_string, "EST"), 0)
	s2->get(test_string, -2);
	TEST_EQUAL(strcmp(test_string, "ST"), 0)
	s2->get(test_string, 0, 1);
	TEST_EQUAL(strcmp(test_string, "T"), 0)
	s2->get(test_string, 1, 1);
	TEST_EQUAL(strcmp(test_string, "E"), 0)
	s2->get(test_string, 2, 2);
	TEST_EQUAL(strcmp(test_string, "ST"), 0)
RESULT
delete s2;

String s4;

CHECK(const String& operator = (const String& s) throw())
	String s5("Hallo");
	s4 = s5;
	TEST_EQUAL(s4, s5)
	s5.set("");
	s4 = s5;
	TEST_EQUAL(s4, s5)
	String s6;
	s4 = s6;
	TEST_EQUAL(s4, s6)
RESULT

CHECK(const String& operator = (const char* pc) throw(Exception::NullPointer))
	s4 = "TestTestTestTest";
	TEST_EQUAL(s4, "TestTestTestTest")
	s4 = "";
	TEST_EQUAL(s4, "");
RESULT

CHECK(const String& operator = (std::stringstream& s) throw())
	std::stringstream instream("ABC DEF GHI jkl mni");

	s2 = new String;
	*s2 = instream;
	TEST_EQUAL(*s2, "ABC")
	*s2 = instream;
	TEST_EQUAL(*s2, "DEF")
	delete s2;
RESULT

CHECK(const String& operator = (std::strstream& s) throw())
	// tested above
RESULT

CHECK(const String& operator = (char c) throw())
	s4 = 'a';
	TEST_EQUAL(s4, "a");
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
