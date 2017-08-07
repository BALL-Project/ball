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
	std::stringstream instream("ABC DEF");
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
