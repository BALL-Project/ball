// $Id: String_test.C,v 1.23 2000/07/18 08:30:02 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/DATATYPE/string.h>
#include <string.h>
#include <string>
///////////////////////////

START_TEST(String,"$Id: String_test.C,v 1.23 2000/07/18 08:30:02 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using std::string;

String* s;	
CHECK(String::String())
	s = new String;
	TEST_NOT_EQUAL(0, s)
	TEST_EQUAL(0, strlen(s->c_str()))
RESULT

CHECK(String::~String())
	delete s;
RESULT

CHECK(String::size()/c_str())
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
CHECK(String::String(String&))
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

CHECK(String::String(string&))
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
RESULT

String empty;
String non_empty("Hallo");
CHECK(String::operator == (char*))
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
CHECK(String::operator == (String&))
	TEST_EQUAL((empty == non_empty), false)
	TEST_EQUAL((empty == empty), true)
	TEST_EQUAL((non_empty == non_empty), true)
	TEST_EQUAL((non_empty == hall), false)
	TEST_EQUAL((non_empty == halloh), false)
RESULT

CHECK(String::create(bool, bool))
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

CHECK(String::String(String&, Index, Size))
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

CHECK(String::String(const char*, Index, Size))
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

CHECK(String::String(Size, char*, ... ))
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

CHECK(String::String(strstream))
	std::strstream instream;
	instream << "ABC" << std::ends << "DEF";
	s2 = new String(instream);
	TEST_NOT_EQUAL(s2, 0)
	TEST_EQUAL(*s2, "ABC")
	delete s2;
	s2 = new String(instream);
	TEST_EQUAL(*s2, "ABC")
	delete s2;
RESULT

CHECK(String::String(char, Size))
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

CHECK(String::String(char))
	s2 = new String('a');
	TEST_EQUAL(*s2, "a")
	delete s2;
RESULT

CHECK(String::String(unsigned char))
	s2 = new String((unsigned char)'b');
	TEST_EQUAL(*s2, "b")
	delete s2;
RESULT

CHECK(String::String(short))
	s2 = new String((short)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((short)-8);
	TEST_EQUAL(*s2, "-8")
	delete s2;
RESULT

CHECK(String::String(unsigned short))
	s2 = new String((unsigned short)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((unsigned short)0);
	TEST_EQUAL(*s2, "0")
	delete s2;
RESULT

CHECK(String::String(int))
	s2 = new String((int)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((int)-8);
	TEST_EQUAL(*s2, "-8")
	delete s2;
RESULT

CHECK(String::String(unsigned int))
	s2 = new String((unsigned int)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((unsigned int)0);
	TEST_EQUAL(*s2, "0")
	delete s2;
RESULT

CHECK(String::String(unsigned long))
	s2 = new String((unsigned long)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((unsigned long)0);
	TEST_EQUAL(*s2, "0")
	delete s2;
RESULT

CHECK(String::String(long))
	s2 = new String((long)7);
	TEST_EQUAL(*s2, "7")
	delete s2;
	s2 = new String((long)-8);
	TEST_EQUAL(*s2, "-8")
	delete s2;
RESULT

CHECK(String::String(float))
	s2 = new String((float)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	delete s2;
	s2 = new String((float)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
	delete s2;
RESULT

CHECK(String::String(double))
	s2 = new String((double)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	delete s2;
	s2 = new String((double)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
	delete s2;
RESULT

CHECK(String::destroy())
	s2 = new String("hallo");
	s2->destroy();
	TEST_EQUAL(*s2, "")
RESULT
delete s2;

const char* char1 = "ABCDEF";
const String abcdef = "abcdef";
const String ABCDEF = "ABCDEF";
Substring empty_sub;
const Substring abcdef_sub(abcdef, 0, 6);
const Substring ABCDEF_sub(ABCDEF, 0, 6);
Substring test_sub1(ABCDEF, 0, 6);
Substring test_sub2;
String test_string;

CHECK(String::set(String&, Index, Size))
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

CHECK(String::set(char*, Index, Size))
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

CHECK(String::set(Size, char*, ...))
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

CHECK(String::set(strstream&))
	std::strstream instream;
	instream << "ABC" << std::ends << "DEF" << std::ends 
		<< "GHI" << std::ends << "jkl" << std::ends << "mno" << std::ends;
	s2 = new String;
	s2->set(instream);
	TEST_EQUAL(*s2, "ABC")
	s2->set(instream);
	TEST_EQUAL(*s2, "ABC")
	delete s2;
RESULT

s2 = new String;
CHECK(String::set(char, Size))
	s2->set('A');
	TEST_EQUAL(*s2, "A")
	s2->set('B', (Size)2);
	TEST_EQUAL(*s2, "BB");
	s2->set('C', (Size)0);
	TEST_EQUAL(*s2, "")
RESULT

CHECK(String::set(unsigned char))
	s2->set((unsigned char)'b');
	TEST_EQUAL(*s2, "b")
RESULT

CHECK(String::set(short))
	s2->set((short)7);
	TEST_EQUAL(*s2, "7")
	s2->set((short)-8);
	TEST_EQUAL(*s2, "-8")
RESULT

CHECK(String::set(unsigned short))
	s2->set((unsigned short)7);
	TEST_EQUAL(*s2, "7")
	s2->set((unsigned short)0);
	TEST_EQUAL(*s2, "0")
RESULT

CHECK(String::set(int))
	s2->set((int)7);
	TEST_EQUAL(*s2, "7")
	s2->set((int)-8);
	TEST_EQUAL(*s2, "-8")
RESULT

CHECK(String::set(unsigned int))
	s2->set((unsigned int)7);
	TEST_EQUAL(*s2, "7")
	s2->set((unsigned int)0);
	TEST_EQUAL(*s2, "0")
RESULT

CHECK(String::set(unsigned long))
	s2->set((unsigned long)7);
	TEST_EQUAL(*s2, "7")
	s2->set((unsigned long)0);
	TEST_EQUAL(*s2, "0")
RESULT

CHECK(String::set(long))
	s2->set((long)7);
	TEST_EQUAL(*s2, "7")
	s2->set((long)-8);
	TEST_EQUAL(*s2, "-8")
RESULT

CHECK(String::set(float))
	s2->set((float)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	s2->set((float)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
RESULT

CHECK(String::set(double))
	s2->set((double)0.0);
	TEST_REAL_EQUAL(atof(s2->c_str()), 0);
	s2->set((double)-1.2);
	TEST_REAL_EQUAL(atof(s2->c_str()), -1.2)
RESULT

CHECK(String::get(char*, Index, Size))
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
CHECK(String::operator = (String&))
	String s5("Hallo");
	s4 = s5;
	TEST_EQUAL(s4, s5)
	s5.set("");
	s4 = s5;
	TEST_EQUAL(s4, s5)
RESULT

CHECK(String::operator = (char*))
	s4 = "Test";
	TEST_EQUAL(s4, "Test")
	s4 = "";
	TEST_EQUAL(s4, "");
RESULT

CHECK(String::operator = (strstream&))
	std::strstream instream;
	instream << "ABC" << std::ends << "DEF" << std::ends 
		<< "GHI" << std::ends << "jkl" << std::ends << "mno" << std::ends;
	s2 = new String;
	*s2 = instream;
	TEST_EQUAL(*s2, "ABC")
	*s2 = instream;
	TEST_EQUAL(*s2, "ABC")
	delete s2;
RESULT

CHECK(String::operator = (char))
	s4 = 'a';
	TEST_EQUAL(s4, "a");
RESULT

CHECK(String::operator = (unsigned char))
	s4 = (unsigned char)'b';
	TEST_EQUAL(s4, "b")
RESULT

CHECK(String::operator = (short))
	s4 = (short)-12;
	TEST_EQUAL(s4, "-12")
RESULT

CHECK(String::operator = (unsigned short))
	s4 = (unsigned short)78;
	TEST_EQUAL(s4, "78")
RESULT

CHECK(String::operator = (int))
	s4 = (int)-19;
	TEST_EQUAL(s4, "-19")
RESULT

CHECK(String::operator = (unsigned int))
	s4 = (unsigned int)123;
	TEST_EQUAL(s4, "123")
RESULT

CHECK(String::operator = (long))
	s4 = (long)-123456789;
	TEST_EQUAL(s4, "-123456789")
RESULT

CHECK(String::operator = (unsigned long))
	s4 = (unsigned long)912345678;
	TEST_EQUAL(s4, "912345678")
RESULT

CHECK(String::operator = (float))
	s4 = (float)-123.456;
	TEST_REAL_EQUAL(atof(s4.c_str()), -123.456)
RESULT

CHECK(String::operator = (double))
	s4 = (double)-456.123;
	TEST_REAL_EQUAL(atof(s4.c_str()), -456.123)
RESULT

CHECK(String::setCompareMode(CompareMode)/getCompareMode())
	String::CompareMode default_mode = String::getCompareMode();
	String::setCompareMode(String::CASE_SENSITIVE);
	TEST_EQUAL(String::getCompareMode(), String::CASE_SENSITIVE)
	String::setCompareMode(String::CASE_INSENSITIVE);
	TEST_EQUAL(String::getCompareMode(), String::CASE_INSENSITIVE)
	String::setCompareMode(default_mode);
RESULT

CHECK(String::toBool())
	s4.set("		false  ");
	TEST_EQUAL(s4.toBool(), false)
	s4.set("FALSE");
	TEST_EQUAL(s4.toBool(), true)
	s4.set(" falsE");
	TEST_EQUAL(s4.toBool(), true)
	s4.set("false");
	TEST_EQUAL(s4.toBool(), false)
	s4.set("fals");
	TEST_EQUAL(s4.toBool(), true)
	s4.set("");
	TEST_EQUAL(s4.toBool(), true)
RESULT

CHECK(String::toChar())
	s4.set("abcdef");
	TEST_EQUAL(s4.toChar(), 'a')
	s4.set("");
	TEST_EQUAL(s4.toChar(), (char)0)
RESULT

CHECK(String::toUnsignedChar())
	s4.set("abcdef");
	TEST_EQUAL(s4.toChar(), 'a')
	s4.set("");
	TEST_EQUAL(s4.toChar(), (unsigned char)0)
RESULT

CHECK(String::toShort())
	s4.set("123");
	TEST_EQUAL(s4.toShort(), (short)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toShort())
	s4.set("");
	//TEST_EQUAL(s4.toShort(), (short)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toShort(), (short)12)
	s4.set("999999999999999999999999");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toShort())
RESULT

CHECK(String::toUnsignedShort())
	s4.set("123");
	TEST_EQUAL(s4.toUnsignedShort(), (unsigned short)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedShort())
	s4.set("");
	//TEST_EQUAL(s4.toUnsignedShort(), (unsigned short)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toUnsignedShort(), (unsigned short)12)
	s4.set("999999999999999999999999");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedShort())
RESULT

CHECK(String::toInt())
	s4.set("123");
	TEST_EQUAL(s4.toInt(), (int)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toInt())
	s4.set("");
	TEST_EQUAL(s4.toInt(), (int)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toInt(), (int)12)
RESULT

CHECK(String::toUnsignedInt())
	s4.set("123");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedInt())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)12.34)
RESULT

CHECK(String::toLong())
	s4.set("123.4");
	TEST_EQUAL(s4.toLong(),(long)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toLong())
	s4.set("");
	TEST_EQUAL(s4.toLong(), (long)0)
	s4.set("-12.34");
	TEST_EQUAL(s4.toLong(), (long)-12.34)
RESULT

CHECK(String::toUnsignedLong())
	s4.set("123.4");
	TEST_EQUAL(s4.toUnsignedLong(),(unsigned long)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedLong())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedLong(), (unsigned long)0)
RESULT

CHECK(String::toFloat())
	s4.set("123.4");
	TEST_REAL_EQUAL(s4.toFloat(),(float)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toFloat())
	s4.set("");
	TEST_EQUAL(s4.toFloat(), (float)0)
	s4.set("-12.34");
	TEST_REAL_EQUAL(s4.toFloat(), (float)-12.34)
	s4 = "";
	for (int i=0; i<999; i++ )
	{
		s4+="9";
	}
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toFloat())
RESULT

CHECK(String::toDouble())
	s4.set("123.4");
	TEST_REAL_EQUAL(s4.toDouble(),(double)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toDouble())
	s4.set("");
	TEST_EQUAL(s4.toDouble(), (double)0)
	s4.set("-12.34");
	TEST_REAL_EQUAL(s4.toDouble(), (double)-12.34)
	s4 = "";
	for (int i=0; i<999; i++ )
	{
		s4+="9";
	}
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toDouble())
RESULT

CHECK(String::toLower())
	s4 = "abCD efGH";
	s4.toLower();
	TEST_EQUAL(s4, "abcd efgh")
	s4 = "ABcd EFgh";
	s4.toLower();
	TEST_EQUAL(s4, "abcd efgh")
	s4 = "";
	s4.toLower();
	TEST_EQUAL(s4, "")
	s4 = "AAAAAA";
	s4.toLower(1);
	TEST_EQUAL(s4, "Aaaaaa")
	s4 = "AAAAAA";
	s4.toLower(-2);
	TEST_EQUAL(s4, "AAAAaa")
	s4 = "AAAAAA";
	s4.toLower(0, 0);
	TEST_EQUAL(s4, "AAAAAA")
	s4 = "AAAAAA";
	TEST_EXCEPTION(Exception::IndexOverflow, s4.toLower(0, 8))
	s4 = "AAAAAA";
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.toLower(-10, 0))
RESULT

CHECK(String::toUpper())
	s4 = "abCD efGH";
	s4.toUpper();
	TEST_EQUAL(s4, "ABCD EFGH")
	s4 = "ABcd EFgh";
	s4.toUpper();
	TEST_EQUAL(s4, "ABCD EFGH")
	s4 = "";
	s4.toUpper();
	TEST_EQUAL(s4, "")
	s4 = "aaaaaa";
	s4.toUpper(1);
	TEST_EQUAL(s4, "aAAAAA")
	s4 = "aaaaaa";
	s4.toUpper(-2);
	TEST_EQUAL(s4, "aaaaAA")
	s4 = "aaaaaa";
	s4.toUpper(0, 0);
	TEST_EQUAL(s4, "aaaaaa")
	s4 = "aaaaaa";
	TEST_EXCEPTION(Exception::IndexOverflow, s4.toUpper(0, 8))
	s4 = "aaaaaa";
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.toUpper(-10, 0))
RESULT

CHECK(String::getSubstring(Index, Size))
	s4 = "abcdefg";
	Substring sub(s4.getSubstring());
	TEST_EQUAL(sub, s4)
	TEST_EQUAL(sub.size(), s4.size())
	Substring sub2(s4.getSubstring(0, 3));
	TEST_EQUAL(sub2, "abc")
	Substring sub3(s4.getSubstring(1, 2));
	TEST_EQUAL(sub3, "bc")
	Substring sub4(s4.getSubstring(1));
	TEST_EQUAL(sub4, "bcdefg")
	TEST_EXCEPTION(Exception::IndexOverflow, Substring sub5(s4.getSubstring(0, 8)))
	TEST_EXCEPTION(Exception::IndexUnderflow, Substring sub6(sub = s4.getSubstring(-10, 0)))
RESULT

CHECK(String::operator () (Index, Size))
	s4 = "abcdefg";
	Substring sub(s4(0, 3));
	TEST_EQUAL(sub, "abc")
	Substring sub2(s4(1, 2));
	TEST_EQUAL(sub2, "bc")
	Substring sub3(s4(1));
	TEST_EQUAL(sub3, "bcdefg")
	TEST_EXCEPTION(Exception::IndexOverflow, Substring sub4(s4(0, 8)))
	TEST_EXCEPTION(Exception::IndexUnderflow, Substring sub5(s4(-10, 0)))
RESULT

CHECK(String::before(String&, Index))
	s4 = "abcdefghabcabcdefghi";
	String res;
	res = s4.before("c");
	TEST_EQUAL(res, "ab")
	res = s4.before("c", 3);
	TEST_EQUAL(res, "abcdefghab")
	res = s4.before("K");
	TEST_EQUAL(res, "")
	res = s4.before("");
	TEST_EQUAL(res, "")
RESULT

CHECK(String::through(String&, Index))
	s4 = "abcdefghabcabcdefghi";
	String res;
	res = s4.through("c");
	TEST_EQUAL(res, "abc")
	res = s4.through("c", 3);
	TEST_EQUAL(res, "abcdefghabc")
	res = s4.through("K");
	TEST_EQUAL(res, "")
	res = s4.through("");
	TEST_EQUAL(res, "")
RESULT

CHECK(String::from(String&, Index))
	s4 = "abcdefghabcabcdefghi";
	String res;
	res = s4.from("c");
	TEST_EQUAL(res, "cdefghabcabcdefghi")
	res = s4.from("c", 3);
	TEST_EQUAL(res, "cabcdefghi")
	res = s4.from("K");
	TEST_EQUAL(res, "")
	res = s4.from("");
	TEST_EQUAL(res, s4)
RESULT

CHECK(String::after(String&, Index))
	s4 = "abcdefghabcabcdefghi";
	String res;
	res = s4.after("c");
	TEST_EQUAL(res, "defghabcabcdefghi")
	res = s4.after("c", 3);
	TEST_EQUAL(res, "abcdefghi")
	res = s4.after("K");
	TEST_EQUAL(res, "")
	TEST_EQUAL(s4.after("i").isValid(), false)
	res = s4.after("");
	TEST_EQUAL(res, s4)
RESULT

CHECK(String::countFields(char*))
	s4 = "aa bb cc";
	TEST_EQUAL(s4.countFields(), 3)
	TEST_EQUAL(s4.countFields("a"), 1)
	TEST_EQUAL(s4.countFields("b"), 2)
	TEST_EQUAL(s4.countFields("c"), 1)
	TEST_EQUAL(s4.countFields(" "), 3)
	s4 = " \t  \t       abc   \t\t   ";
	TEST_EQUAL(s4.countFields(), 1)
	TEST_EQUAL(s4.countFields("\t"), 4)
	s4 = "";
	TEST_EQUAL(s4.countFields(), 0)
	s4 = "    \t   \t  ";
	TEST_EQUAL(s4.countFields(), 0)
	s4 = "    \t   \t  a";
	TEST_EQUAL(s4.countFields(), 1)
RESULT

CHECK(String::getField(Index, char*, Index*))
	s4 = "aa bb cc";
	TEST_EQUAL(s4.getField(0), "aa")
	TEST_EQUAL(s4.getField(1), "bb")
	TEST_EQUAL(s4.getField(2), "cc")
	TEST_EQUAL(s4.getField(3), "")
	TEST_EQUAL(s4.getField(-3), "aa")
	TEST_EQUAL(s4.getField(-2), "bb")
	TEST_EQUAL(s4.getField(-1), "cc")
	TEST_EQUAL(s4.getField(0, "b"), "aa ")
	TEST_EQUAL(s4.getField(1, "b"), " cc")
	s4 = " \t  \t       abc   \t\t   ";
	TEST_EQUAL(s4.getField(0), "abc")
	TEST_EQUAL(s4.getField(1, "\t"), "  ")
	s4 = "";
	TEST_EQUAL(s4.getField(0), "")
	s4 = "    \t   \t  ";
	TEST_EQUAL(s4.getField(0), "")
	s4 = "    \t   \t  a";
	TEST_EQUAL(s4.getField(0), "a")
	Index index = -1;
	Index* i;
	i = &index;
	char* c = 0;
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.getField(0, ",", i))	
	*i = 1;
	TEST_EXCEPTION(Exception::NullPointer, s4.getField(0, c, i))	
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.getField(-99, ","))	
RESULT

CHECK(String::split(String[], Size, char*, Index))
//BAUSTELLE: argumente 3 und 4 checken!
	String arr[10];
	s4 = "a b c d e f g";
	TEST_EQUAL(s4.split(arr, 10), 7)
	TEST_EQUAL(arr[0], "a")
	TEST_EQUAL(arr[1], "b")
	TEST_EQUAL(arr[2], "c")
	TEST_EQUAL(arr[3], "d")
	TEST_EQUAL(arr[4], "e")
	TEST_EQUAL(arr[5], "f")
	TEST_EQUAL(arr[6], "g")
	TEST_EQUAL(arr[7], "")
	s4 = "b c d e f g h";
	TEST_EQUAL(s4.split(arr, 5), 5)
	TEST_EQUAL(arr[0], "b")
	TEST_EQUAL(arr[1], "c")
	TEST_EQUAL(arr[2], "d")
	TEST_EQUAL(arr[3], "e")
	TEST_EQUAL(arr[4], "f")
	TEST_EQUAL(arr[5], "f")
	TEST_EQUAL(arr[6], "g")
	TEST_EQUAL(arr[7], "")
	arr[0] = "";
	TEST_EQUAL(s4.split(arr, 0), 0)
	TEST_EQUAL(arr[0], "")
	TEST_EQUAL(arr[1], "c")
	s4= "";
	TEST_EQUAL(s4.split(arr, 10), 0)
	TEST_EQUAL(arr[0], "")

	s4 = "a/b(cd)e*f-g";
	char* c = "/()*-";
	TEST_EQUAL(s4.split(arr, 10, c), 6)
	TEST_EQUAL(arr[0], "a")
	TEST_EQUAL(arr[1], "b")
	TEST_EQUAL(arr[2], "cd")
	TEST_EQUAL(arr[3], "e")
	TEST_EQUAL(arr[4], "f")
	TEST_EQUAL(arr[5], "g")
	TEST_EQUAL(arr[6], "g")

	s4 = "b/c d e/f g h";
	TEST_EQUAL(s4.split(arr, 5, c, 2), 2)
	TEST_EQUAL(arr[0], "c d e")
	TEST_EQUAL(arr[1], "f g h")
	TEST_EQUAL(arr[2], "cd")
RESULT

CHECK(String::trimLeft(char*))
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

CHECK(String::trimRight(char*))
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

CHECK(String::trim(char*))
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

CHECK(String::truncate(Size))
	s4 = "abcd";
	TEST_EQUAL(s4.truncate(2), "ab")
	TEST_EQUAL(s4.truncate(5), "ab")
	s4 = "";
	TEST_EQUAL(s4.truncate(5), "")
RESULT

CHECK(String::left(Size))
	s4 = "abcdefg";
	TEST_EQUAL(s4.left(4), "abcd")
	TEST_EQUAL(s4.left(20), "abcdefg")
	TEST_EQUAL(s4.left(0), "")
	s4 = "";
	TEST_EQUAL(s4.left(4), "")
	TEST_EQUAL(s4.left(0), "")
RESULT

CHECK(String::right(Size))
	s4 = "abcdefg";
	TEST_EQUAL(s4.right(4), "defg")
	TEST_EQUAL(s4.right(20), "abcdefg")
	TEST_EQUAL(s4.right(0), "")
	s4 = "";
	TEST_EQUAL(s4.right(4), "")
	TEST_EQUAL(s4.right(0), "")
RESULT

CHECK(String::instr(string&, Size))
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

CHECK(String::operator + (string&))
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

CHECK(String::operator + (char*))
	s4 = "abc";
	TEST_EQUAL(s4 + "def", "abcdef")
	TEST_EQUAL(s4 + "", "abc")
	s4 = "";
	TEST_EQUAL(s4 + "def", "def")
	TEST_EQUAL(s4 + "", "")
RESULT

CHECK(String::operator + (char))
	s4 = "abc";
	TEST_EQUAL(s4 + 'd', "abcd")
	s4 = "";
	TEST_EQUAL(s4 + 'd', "d")
RESULT

CHECK(String::operator + (char*, String&))
	s4 = "abc";
	TEST_EQUAL("def" + s4, "defabc")
	TEST_EQUAL("" + s4, "abc")
	s4 = "";
	TEST_EQUAL("def" + s4, "def")
	TEST_EQUAL("" + s4, "")
RESULT

CHECK(String::operator + (char*, String&))
	s4 = "abc";
	TEST_EQUAL('d' + s4, "dabc")
	s4 = "";
	TEST_EQUAL('d' + s4, "d")
RESULT

CHECK(String::swap(String&))
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

CHECK(String::reverse(Index, Size))
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

CHECK(String::substitute(String&, String&))
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

CHECK(String::has(char))
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

CHECK(String::hasSubstring(String&, Index))
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

CHECK(String::hasPrefix(String&))
	s4 = "abcdefgh";
	TEST_EQUAL(s4.hasPrefix(""), true)
	TEST_EQUAL(s4.hasPrefix("abcd"), true)
	TEST_EQUAL(s4.hasPrefix("abce"), false)
	s4 = "";
	TEST_EQUAL(s4.hasPrefix(""), true)
	TEST_EQUAL(s4.hasPrefix("a"), false)
RESULT

CHECK(String::hasSuffix(String&))
	s4 = "abcdefgh";
	TEST_EQUAL(s4.hasSuffix(""), true)
	TEST_EQUAL(s4.hasSuffix("fgh"), true)
	TEST_EQUAL(s4.hasSuffix("fhh"), false)
	s4 = "";
	TEST_EQUAL(s4.hasSuffix(""), true)
	TEST_EQUAL(s4.hasSuffix("h"), false)
RESULT

CHECK(String::isEmpty())
	s4 = "a";
	TEST_EQUAL(s4.isEmpty(), false)
	s4 = "ab";
	TEST_EQUAL(s4.isEmpty(), false)
	s4 = "";
	TEST_EQUAL(s4.isEmpty(), true)
RESULT

CHECK(String::isAlpha())
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

CHECK(String::isAlnum())
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

CHECK(String::isDigit())
	s4 = "";
	TEST_EQUAL(s4.isDigit(), true)
	s4 = "1";
	TEST_EQUAL(s4.isDigit(), true)
	s4 = "1234567890";
	TEST_EQUAL(s4.isDigit(), true)
	s4 = "1234a67890";
	TEST_EQUAL(s4.isDigit(), false)
RESULT

CHECK(String::isSpace())
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

CHECK(String::isWhitespace())
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

CHECK(String::isFloat())
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

CHECK(String::isAlpha(char))
	String char_class = String::CHARACTER_CLASS__ASCII_ALPHA;
	for (char c = CHAR_MIN; c != CHAR_MAX; c++)
	{
		TEST_EQUAL(String::isAlpha(c), char_class.has(c))
	}
RESULT

CHECK(String::isAlnum(char))
	String char_class = String::CHARACTER_CLASS__ASCII_ALPHANUMERIC;
	for (char c = CHAR_MIN; c != CHAR_MAX; c++)
	{
		TEST_EQUAL(String::isAlnum(c), char_class.has(c))
	}
RESULT

CHECK(String::isDigit(char))
	String char_class = String::CHARACTER_CLASS__ASCII_NUMERIC;
	for (char c = CHAR_MIN; c != CHAR_MAX; c++)
	{
		TEST_EQUAL(String::isDigit(c), char_class.has(c))
	}
RESULT

CHECK(String::isWhitespace(char))
	String char_class = String::CHARACTER_CLASS__WHITESPACE;
	for (char c = '\0'; c != (char)255; c++)
	{
		TEST_EQUAL(String::isWhitespace(c), char_class.has(c))
	}
RESULT

CHECK(String::isSpace(char))
	for (char c = '\0'; c != (char)255; c++)
	{
		TEST_EQUAL(String::isSpace(c), (c == ' '))
	}
RESULT

CHECK(String::compare(String&, Index))
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

CHECK(String::compare(String&, Index, Size))
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

CHECK(String::compare(char*, Index))
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

CHECK(String::compare(char*, Index, Size))
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

CHECK(String::compare(char, Index))
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
CHECK(String::operator == (String&))
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

CHECK(String::operator != (String&))
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

CHECK(String::operator < (String&))
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

CHECK(String::operator <= (String&))
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

CHECK(String::operator > (String&))
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

CHECK(String::operator >= (String&))
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

CHECK(operator == (char*, String&))
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

CHECK(operator != (char*, String&))
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

CHECK(operator < (char*, String&))
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

CHECK(operator <= (char*, String&))
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

CHECK(operator > (char*, String&))
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

CHECK(operator >= (char*, String&))
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

CHECK(String::operator == (char*))
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

CHECK(String::operator != (char*))
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

CHECK(String::operator < (char*))
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

CHECK(String::operator <= (char*))
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

CHECK(String::operator > (char*))
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

CHECK(String::operator >= (char*))
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

CHECK(operator == (char, String&))
	s5 = "bc";
	TEST_EQUAL(('a' == s5), false)
	TEST_EQUAL(('b' == s5), true)
	TEST_EQUAL(('c' == s5), false)
	s5 = "";
	TEST_EQUAL(('a' == s5), false)
RESULT

CHECK(operator != (char, String&))
	s5 = "bc";
	TEST_EQUAL(('a' != s5), true)
	TEST_EQUAL(('b' != s5), false)
	TEST_EQUAL(('c' != s5), true)
	s5 = "";
	TEST_EQUAL(('a' != s5), true)
RESULT

CHECK(operator < (char, String&))
	s5 = "bc";
	TEST_EQUAL(('a' < s5), true)
	TEST_EQUAL(('b' < s5), false)
	TEST_EQUAL(('c' < s5), false)
	s5 = "";
	TEST_EQUAL(('a' < s5), false)
RESULT

CHECK(operator <= (char, String&))
	s5 = "bc";
	TEST_EQUAL(('a' <= s5), true)
	TEST_EQUAL(('b' <= s5), true)
	TEST_EQUAL(('c' <= s5), false)
	s5 = "";
	TEST_EQUAL(('a' <= s5), false)
RESULT

CHECK(operator > (char, String&))
	s5 = "bc";
	TEST_EQUAL(('a' > s5), false)
	TEST_EQUAL(('b' > s5), false)
	TEST_EQUAL(('c' > s5), true)
	s5 = "";
	TEST_EQUAL(('a' > s5), true)
RESULT

CHECK(operator >= (char, String&))
	s5 = "bc";
	TEST_EQUAL(('a' >= s5), false)
	TEST_EQUAL(('b' >= s5), true)
	TEST_EQUAL(('c' >= s5), true)
	s5 = "";
	TEST_EQUAL(('a' >= s5), true)
RESULT

CHECK(operator == (char))
	s5 = "bc";
	TEST_EQUAL((s5 == 'a'), false)
	TEST_EQUAL((s5 == 'b'), true)
	TEST_EQUAL((s5 == 'c'), false)
	s5 = "";
	TEST_EQUAL((s5 == 'a'), false)
RESULT

CHECK(operator != (char))
	s5 = "bc";
	TEST_EQUAL((s5 != 'a'), true)
	TEST_EQUAL((s5 != 'b'), false)
	TEST_EQUAL((s5 != 'c'), true)
	s5 = "";
	TEST_EQUAL((s5 != 'a'), true)
RESULT

CHECK(operator < (char))
	s5 = "bc";
	TEST_EQUAL((s5 < 'a'), false)
	TEST_EQUAL((s5 < 'b'), false)
	TEST_EQUAL((s5 < 'c'), true)
	s5 = "";
	TEST_EQUAL((s5 < 'a'), true)
RESULT

CHECK(operator <= (char))
	s5 = "bc";
	TEST_EQUAL((s5 <= 'a'), false)
	TEST_EQUAL((s5 <= 'b'), true)
	TEST_EQUAL((s5 <= 'c'), true)
	s5 = "";
	TEST_EQUAL((s5 <= 'a'), true)
RESULT

CHECK(operator > (char))
	s5 = "bc";
	TEST_EQUAL((s5 > 'a'), true)
	TEST_EQUAL((s5 > 'b'), false)
	TEST_EQUAL((s5 > 'c'), false)
	s5 = "";
	TEST_EQUAL((s5 > 'a'), false)
RESULT

CHECK(operator >= (char))
	s5 = "bc";
	TEST_EQUAL((s5 >= 'a'), true)
	TEST_EQUAL((s5 >= 'b'), true)
	TEST_EQUAL((s5 >= 'c'), false)
	s5 = "";
	TEST_EQUAL((s5 >= 'a'), false)
RESULT

CHECK(String::isValid())
	s2 = new String;
	TEST_EQUAL(s2->isValid(), true)
RESULT

CHECK(String::dump(ostream&, Size))
	String tmp_filename;
	NEW_TMP_FILE(tmp_filename)
	std::ofstream dump_stream(tmp_filename.c_str(), std::ios::out);
	s2 = new String("abcdefghijklm");
	s2->dump(dump_stream, 0);
	dump_stream.clear();
	dump_stream.close();
	TEST_FILE(tmp_filename.c_str(), "data/string_test_dump0.txt", true)

	NEW_TMP_FILE(tmp_filename)
	std::ofstream dump_stream2(tmp_filename.c_str(), std::ios::out);
	s2->dump(dump_stream2, 4);
	dump_stream2.close();
	TEST_FILE(tmp_filename.c_str(), "data/string_test_dump4.txt", true)
RESULT

CHECK(String::getline(istream&, char*))
	String line;
	std::ifstream instream("data/string_test.txt");
	line.getline(instream);
	TEST_EQUAL(line, "ABC DEF")
	line.getline(instream);
	TEST_EQUAL(line, "GHI jkl")
	line.getline(instream);
	TEST_EQUAL(line, "mno")
RESULT

CHECK(String::getline(istream&, String&, char*))
	std::ifstream test_stream("data/string_test.txt");
	if (!test_stream.good())
	{
		throw Exception::FileNotFound(__FILE__, __LINE__, "data/string_test.txt");
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


// Substring class....

CHECK(Substring::Substring())
	Substring sub;
RESULT

CHECK(Substring::Substring(String&))
	String s5("abcdef");
	Substring sub(s5);
	TEST_EQUAL(sub, s5)
RESULT

CHECK(Substring::Substring(Subtring&))
	s5 ="abcdef";
	Substring sub(s5);
	Substring sub2(sub);
	TEST_EQUAL(sub2, s5)
RESULT

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
	char * c3;
	TEST_EXCEPTION(Substring::UnboundSubstring, c3 = empty_sub.c_str())	
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
	char c2;
	TEST_EQUAL(c, 'a')
	TEST_EQUAL(abcdef_sub[-1], 'f')
	TEST_EXCEPTION(Substring::UnboundSubstring, c2 = empty_sub[0])	
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

CHECK(Substring::friend::std::ostream& operator << (::std::ostream& s, const Substring& substring))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	test_sub1.bind(ABCDEF, 1, 4);
	outstr << test_sub1;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/String_test2.txt", false)
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

CHECK(Substring::dump(::std::ostream& s = ::std::cout, Size depth = 0) const )
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), ios::out);
	test_sub1.bind(ABCDEF, 1, 4);
	test_sub1.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/String_test.txt", true)
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
