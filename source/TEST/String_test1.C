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
CHECK(String() throw())
	s = new String;
	TEST_NOT_EQUAL(0, s)
	TEST_EQUAL(0, strlen(s->c_str()))
RESULT

CHECK(~String() throw())
	delete s;
RESULT

CHECK([EXTRA]size() const throw())
	s = new String();
	TEST_EQUAL(strlen(s->c_str()), 0)
	TEST_EQUAL(s->size(), 0)
	delete s;
	s = new String("ABC");
	TEST_EQUAL(s->size(), 3)
	TEST_EQUAL(strlen(s->c_str()), 3)
	TEST_EQUAL(s->c_str()[0], 'A')
	TEST_EQUAL(s->c_str()[1], 'B')
	TEST_EQUAL(s->c_str()[2], 'C')
	TEST_EQUAL(strcmp("ABC", s->c_str()), 0)
RESULT

String* s2;
CHECK(String(const String& string) throw())
	s2 = new String(*s);
	TEST_EQUAL(s2->size(), 3)
	TEST_EQUAL(strcmp(s2->c_str(), "ABC"), 0)
	delete s2;
	String* s3 = new String;
	s2 = new String(*s3);
	TEST_EQUAL(s2->size(), 0)
	TEST_EQUAL(strcmp(s2->c_str(), ""), 0)
	delete s3;
RESULT
delete s2;
delete s;

CHECK(String(const string& string) throw())
	string sx;
	sx = "ABC";
	s2 = new String(sx);
	TEST_EQUAL(s2->size(), 3)
	TEST_EQUAL(strcmp(s2->c_str(), "ABC"), 0)
	delete s2;
	string* s3 = new string;
	s2 = new String(*s3);
	TEST_EQUAL(s2->size(), 0)
	TEST_EQUAL(strcmp(s2->c_str(), ""), 0)
	delete s3;
	delete s2;
RESULT

String empty;
String non_empty("Hallo");
CHECK(bool operator == (const char* char_ptr) const throw(Exception::NullPointer))
	TEST_EQUAL((empty == ""), true)
	TEST_EQUAL((empty == "Hallo"), false)
	TEST_EQUAL((empty == "Halla"), false)
	TEST_EQUAL((empty == "Halloh"), false)
	TEST_EQUAL((empty == "Hall"), false)
	TEST_EQUAL((empty == empty.c_str()), true)
	TEST_EQUAL((non_empty == ""), false)
	TEST_EQUAL((non_empty == "Hallo"), true)
	TEST_EQUAL((non_empty == "Halloh"), false)
	TEST_EQUAL((non_empty == "Hall"), false)
	TEST_EQUAL((non_empty == "Halla"), false)
	TEST_EQUAL((non_empty == non_empty.c_str()), true)
RESULT

String hall("Hall");
String halloh("Halloh");
CHECK(bool operator == (const String& string) const throw())
	TEST_EQUAL((empty == non_empty), false)
	TEST_EQUAL((empty == empty), true)
	TEST_EQUAL((non_empty == non_empty), true)
	TEST_EQUAL((non_empty == hall), false)
	TEST_EQUAL((non_empty == halloh), false)
RESULT

CHECK(void* create(bool /* deep */ = true, bool empty = false) const throw())
	s2 = (String*)(halloh.create(true, false));
	TEST_EQUAL(*s2, halloh)
	delete s2;
	s2 = (String*)(halloh.create(false, false));
	TEST_EQUAL(*s2, halloh)
	delete s2;
	s2 = (String*)(halloh.create(true, true));
	TEST_EQUAL(*s2, "")
	delete s2;
	s2 = (String*)(halloh.create(false, true));
	TEST_EQUAL(*s2, "")
	delete s2;
RESULT

CHECK(String(const String& s, Index from, Size len = EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	s2 = new String(halloh, 2, 4);
	TEST_EQUAL(*s2, "lloh")
	delete s2;
	s2 = new String(halloh, 4, 2);
	TEST_EQUAL(*s2, "oh")
	delete s2;
	s2 = new String(halloh, 3);
	TEST_EQUAL(*s2, "loh")
	delete s2;
	s2 = new String(halloh, -2);
	TEST_EQUAL(*s2, "oh")
	delete s2;
	s2 = new String(halloh, 0, 0);
	TEST_EQUAL(*s2, "")
	delete s2;
	s2 = new String(halloh, -1);
	TEST_EQUAL(*s2, "h")
	delete s2;
	s2 = 0;
	TEST_EXCEPTION(Exception::IndexOverflow, s2 = new String(halloh, 1, 7))
	if (s2 != 0)
		delete s2;
RESULT

CHECK(String(const char* char_ptr, Index from = 0, Size len = EndPos) throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow))
	s2 = new String("halloh");
	TEST_EQUAL(*s2, "halloh")
	delete s2;
	s2 = new String("halloh", 1);
	TEST_EQUAL(*s2, "alloh")
	delete s2;
	s2 = new String("halloh", 1, 2);
	TEST_EQUAL(*s2, "al")
	delete s2;
	s2 = new String("halloh", -1, 0);
	TEST_EQUAL(*s2, "")
	delete s2;
	s2 = new String("halloh", -5);
	TEST_EQUAL(*s2, "alloh")
	delete s2;
	s2 = new String("halloh", 2, 3);
	TEST_EQUAL(*s2, "llo")
	delete s2;
	s2 = 0;
	TEST_EXCEPTION(Exception::IndexOverflow, s2 = new String("halloh", 0, 8))
	if (s2 != 0)
		delete s2;
	s2 = 0;
	TEST_EXCEPTION(Exception::IndexUnderflow, s2 = new String("halloh", -12))
	if (s2 != 0)
		delete s2;
RESULT

CHECK(String(Size buffer_size, const char* format, ... ) throw(Exception::IndexUnderflow, Exception::NullPointer))
	s2 = new String(15, "%s", "halloh");
	TEST_EQUAL(*s2, "halloh")
	delete s2;
	s2 = new String(2, "%s", "halloh");
	TEST_EQUAL(*s2, "h")
	delete s2;
	s2 = new String(15, "%3.1f", 1.2);
	TEST_EQUAL(*s2, "1.2")
	delete s2;
	s2 = 0;
	TEST_EXCEPTION(Exception::IndexUnderflow, s2 = new String(0, "%s", "Halloh"))
	if (s2 != 0)
	{
		delete s2;
	}
	s2 = 0;
	TEST_EXCEPTION(Exception::NullPointer, s2 = new String(15, (char*)0, "Halloh", 1.5, 1.2))
	if (s2 != 0)
	{
		delete s2;
	}
RESULT

CHECK(String(std::stringstream& s) throw())
#ifdef BALL_HAS_SSTREAM
	std::stringstream instream("ABC DEF");
#else
	std::strstream instream("ABC DEF", strlen("ABC DEF"), ios::app);
#endif
	s2 = new String(instream);
	TEST_NOT_EQUAL(s2, 0)
	TEST_EQUAL(*s2, "ABC")
	delete s2;
	s2 = new String(instream);
	TEST_EQUAL(*s2, "DEF")
	delete s2;
RESULT

CHECK(String(std::strstream& s) throw())
	// tested above
RESULT

CHECK(String(const char c, Size len = 1) throw())
	s2 = new String('a', (Size)1);
	TEST_EQUAL(*s2, "a")
	delete s2;
	s2 = new String('a', (Size)2);
	TEST_EQUAL(*s2, "aa")
	delete s2;
	s2 = new String('a', (Size)0);
	TEST_EQUAL(*s2, "")
	delete s2;
RESULT

CHECK(String(const unsigned char uc) throw())
	s2 = new String((unsigned char)'b');
	TEST_EQUAL(*s2, "b")
	delete s2;
RESULT

CHECK(String(short s) throw())
	s2 = new String((short)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((short)-8);
	TEST_EQUAL(*s2, "-8")
	delete s2;
RESULT

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
#ifdef BALL_HAS_SSTREAM
	std::stringstream instream("ABC DEF GHI jkl mno");
#else
	std::strstream instream("ABC DEF GHI jkl mno", strlen("ABC DEF GHI jkl mno"), ios::app);
#endif

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
#ifdef BALL_HAS_SSTREAM
	std::stringstream instream("ABC DEF GHI jkl mni");
#else
	std::strstream instream("ABC DEF GHI jkl mni", strlen("ABC DEF GHI jkl mni"),ios::app);
#endif

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
