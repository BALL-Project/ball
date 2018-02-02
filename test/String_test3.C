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

String empty;
String non_empty("Hallo");
String hall("Hall");
String halloh("Halloh");
const String abcdef = "abcdef";
const String ABCDEF = "ABCDEF";
Substring empty_sub;
const Substring abcdef_sub(abcdef, 0, 6);
const Substring ABCDEF_sub(ABCDEF, 0, 6);
Substring test_sub1(ABCDEF, 0, 6);
Substring test_sub2;
String test_string;
String s4;

CHECK(const String& operator = (unsigned char uc) throw())
	s4 = (unsigned char)'b';
	TEST_EQUAL(s4, "b")
RESULT

CHECK(const String& operator = (short s) throw())
	s4 = (short)-12;
	TEST_EQUAL(s4, "-12")
RESULT

CHECK(const String& operator = (unsigned short us) throw())
	s4 = (unsigned short)78;
	TEST_EQUAL(s4, "78")
RESULT

CHECK(const String& operator = (int i) throw())
	s4 = (int)-19;
	TEST_EQUAL(s4, "-19")
RESULT

CHECK(const String& operator = (unsigned int ui) throw())
	s4 = (unsigned int)123;
	TEST_EQUAL(s4, "123")
RESULT

CHECK(const String& operator = (long l) throw())
	s4 = (long)-123456789;
	TEST_EQUAL(s4, "-123456789")
RESULT

CHECK(const String& operator = (unsigned long ) throw())
	s4 = (unsigned long)912345678;
	TEST_EQUAL(s4, "912345678")
RESULT

CHECK(const String& operator = (float f) throw())
	s4 = (float)-123.456;
	TEST_REAL_EQUAL(atof(s4.c_str()), -123.456)
RESULT

CHECK(const String& operator = (double d) throw())
	s4 = (double)-456.123;
	TEST_REAL_EQUAL(atof(s4.c_str()), -456.123)
RESULT

CHECK(static void setCompareMode(CompareMode compare_mode) throw())
	String::CompareMode default_mode = String::getCompareMode();
	String::setCompareMode(String::CASE_SENSITIVE);
	TEST_EQUAL(String::getCompareMode(), String::CASE_SENSITIVE)
	String::setCompareMode(String::CASE_INSENSITIVE);
	TEST_EQUAL(String::getCompareMode(), String::CASE_INSENSITIVE)
	String::setCompareMode(default_mode);
RESULT

CHECK(static CompareMode getCompareMode() throw())
	String::CompareMode default_mode = String::getCompareMode();
	String::setCompareMode(String::CASE_SENSITIVE);
	TEST_EQUAL(String::getCompareMode(), String::CASE_SENSITIVE)
	String::setCompareMode(default_mode);
RESULT

CHECK(bool toBool() const throw())
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

CHECK(char toChar() const throw())
	s4.set("abcdef");
	TEST_EQUAL(s4.toChar(), 'a')
	s4.set("");
	TEST_EQUAL(s4.toChar(), (char)0)
RESULT

CHECK(unsigned char toUnsignedChar() const throw())
	s4.set("abcdef");
	TEST_EQUAL(s4.toChar(), 'a')
	s4.set("");
	TEST_EQUAL(s4.toChar(), (unsigned char)0)
RESULT

CHECK(short toShort() const)
	s4.set("123");
	TEST_EQUAL(s4.toShort(), (short)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toShort())
	s4.set("");
	TEST_EQUAL(s4.toShort(), (short)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toShort(), (short)12)
	s4.set("9999999");
	STATUS(s4)

	short x = 0;

  try
  {
		x = s4.toShort();
	}
	catch (Exception::InvalidFormat&)
	{
		STATUS("Caught the right one!")
	}
	catch (Exception::GeneralException e)
	{
		STATUS(e)
	}
	catch (...)
	{
		STATUS("caught something!")
	}
	TEST_EXCEPTION(Exception::InvalidFormat, x = s4.toShort())
RESULT

CHECK(unsigned short toUnsignedShort() const)
	s4.set("123");
	TEST_EQUAL(s4.toUnsignedShort(), (unsigned short)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedShort())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedShort(), (unsigned short)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toUnsignedShort(), (unsigned short)12)
	s4.set("99999999");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedShort())
RESULT

CHECK(int toInt() const)
	s4.set("123");
	TEST_EQUAL(s4.toInt(), (int)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toInt())
	s4.set("");
	TEST_EQUAL(s4.toInt(), (int)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toInt(), (int)12)
RESULT

CHECK(unsigned int toUnsignedInt() const)
	s4.set("123");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedInt())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)12.34)
RESULT

CHECK(long toLong() const)
	s4.set("123.4");
	TEST_EQUAL(s4.toLong(),(long)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toLong())
	s4.set("");
	TEST_EQUAL(s4.toLong(), (long)0)
	s4.set("-12.34");
	TEST_EQUAL(s4.toLong(), (long)-12.34)
RESULT

CHECK(unsigned long toUnsignedLong() const)
	s4.set("123.4");
	TEST_EQUAL(s4.toUnsignedLong(),(unsigned long)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedLong())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedLong(), (unsigned long)0)
RESULT

CHECK(float toFloat() const)
	PRECISION(0.01)
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

CHECK(double toDouble() const)
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

CHECK(void toLower(Index from = 0, Size len = EndPos))
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
