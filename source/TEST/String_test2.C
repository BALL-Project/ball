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

CHECK(short toShort() const throw(Exception::InvalidFormat))
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

CHECK(unsigned short toUnsignedShort() const throw(Exception::InvalidFormat))
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

CHECK(int toInt() const throw(Exception::InvalidFormat))
	s4.set("123");
	TEST_EQUAL(s4.toInt(), (int)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toInt())
	s4.set("");
	TEST_EQUAL(s4.toInt(), (int)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toInt(), (int)12)
RESULT

CHECK(unsigned int toUnsignedInt() const throw(Exception::InvalidFormat))
	s4.set("123");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)123)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedInt())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)0)
	s4.set("12.34");
	TEST_EQUAL(s4.toUnsignedInt(), (unsigned int)12.34)
RESULT

CHECK(long toLong() const throw(Exception::InvalidFormat))
	s4.set("123.4");
	TEST_EQUAL(s4.toLong(),(long)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toLong())
	s4.set("");
	TEST_EQUAL(s4.toLong(), (long)0)
	s4.set("-12.34");
	TEST_EQUAL(s4.toLong(), (long)-12.34)
RESULT

CHECK(unsigned long toUnsignedLong() const throw(Exception::InvalidFormat))
	s4.set("123.4");
	TEST_EQUAL(s4.toUnsignedLong(),(unsigned long)123.4)
	s4.set("abc");
	TEST_EXCEPTION(Exception::InvalidFormat, s4.toUnsignedLong())
	s4.set("");
	TEST_EQUAL(s4.toUnsignedLong(), (unsigned long)0)
RESULT

CHECK(float toFloat() const throw(Exception::InvalidFormat))
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

CHECK(double toDouble() const throw(Exception::InvalidFormat))
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

CHECK(void toLower(Index from = 0, Size len = EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
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

CHECK(void toUpper(Index from = 0, Size len = EndPos) throw(Exception::IndexUnderflow, Exception::IndexOverflow))
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

CHECK(Substring getSubstring(Index from = 0, Size len = EndPos) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
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

CHECK(Substring operator () (Index from, Size len = EndPos) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
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

CHECK(Substring before(const String& s, Index from = 0) const throw())
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

CHECK(Substring through(const String& s, Index from = 0) const throw())
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

CHECK(Substring from(const String& s, Index from = 0) const throw())
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

CHECK(Substring after(const String& s, Index from = 0) const throw())
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

CHECK(Size countFields(const char* delimiters = CHARACTER_CLASS__WHITESPACE) const throw(Exception::NullPointer))
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

CHECK(Size countFieldsQuoted(const char* delimiters = CHARACTER_CLASS__WHITESPACE, const char* quotes = CHARACTER_CLASS__QUOTES) const throw(Exception::NullPointer))
	s4 = "aa b cc b";
	TEST_EQUAL(s4.countFieldsQuoted(), 4)
	TEST_EQUAL(s4.countFieldsQuoted("a"), 1)
	TEST_EQUAL(s4.countFieldsQuoted(" ", "b"), 2)
	TEST_EQUAL(s4.countFieldsQuoted("c"), 2)
	s4 = " \t  \t  \"  bb   abc \"  \t\t   ";
	TEST_EQUAL(s4.countFieldsQuoted(), 1)
	TEST_EQUAL(s4.countFieldsQuoted("\t"), 4)
	s4 = "";
	TEST_EQUAL(s4.countFieldsQuoted(), 0)
	s4 = "    \t   \t  ";
	TEST_EQUAL(s4.countFieldsQuoted(), 0)
	s4 = "  \"  \t \"  \t  a";
	TEST_EQUAL(s4.countFieldsQuoted(), 2)
RESULT

CHECK(String getField(Index index, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index* from = 0) const throw(Exception::IndexUnderflow, Exception::NullPointer))
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

CHECK(String getFieldQuoted(Index index, const char* delimiters = CHARACTER_CLASS__WHITESPACE, const char* quotes = CHARACTER_CLASS__QUOTES, Index* from = 0) const throw(Exception::IndexUnderflow, Exception::NullPointer))
	// test the default (unquoted behviour)
	s4 = "aa bb cc";
	TEST_EQUAL(s4.getFieldQuoted(0), "aa")
	TEST_EQUAL(s4.getFieldQuoted(1), "bb")
	TEST_EQUAL(s4.getFieldQuoted(2), "cc")
	TEST_EQUAL(s4.getFieldQuoted(3), "")
	s4 = " aa   bb    cc    ";
	TEST_EQUAL(s4.getFieldQuoted(0), "aa")
	TEST_EQUAL(s4.getFieldQuoted(1), "bb")
	TEST_EQUAL(s4.getFieldQuoted(2), "cc")
	TEST_EQUAL(s4.getFieldQuoted(3), "")

	// simple quotes
	s4 = "aa \"bb cc\"";
	TEST_EQUAL(s4.getFieldQuoted(0), "aa")
	TEST_EQUAL(s4.getFieldQuoted(1), "bb cc")
	TEST_EQUAL(s4.getFieldQuoted(2), "")
	TEST_EQUAL(s4.getFieldQuoted(-2), "aa")
	TEST_EQUAL(s4.getFieldQuoted(-1), "bb cc")
	TEST_EQUAL(s4.getFieldQuoted(0, "b"), "aa bb cc")
	TEST_EQUAL(s4.getFieldQuoted(1, "b"), "")
	s4 = " \t  \t       abc   \t\t   ";
	TEST_EQUAL(s4.getFieldQuoted(0), "abc")
	TEST_EQUAL(s4.getFieldQuoted(1, "\t"), "  ")
	s4 = "";
	TEST_EQUAL(s4.getFieldQuoted(0), "")
	s4 = "    \t   \t  ";
	TEST_EQUAL(s4.getFieldQuoted(0), "")
	s4 = "    \t   \t  a";
	TEST_EQUAL(s4.getFieldQuoted(0), "a")
	Index index = -1;
	Index* i;
	i = &index;
	char* c = 0;
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.getFieldQuoted(0, ",", "'", i))	
	*i = 1;
	TEST_EXCEPTION(Exception::NullPointer, s4.getFieldQuoted(0, c, "'", i))	
	TEST_EXCEPTION(Exception::NullPointer, s4.getFieldQuoted(0, " ", c, i))	
	TEST_EXCEPTION(Exception::IndexUnderflow, s4.getFieldQuoted(-99, ","))	
	s4 = "abcd efg \"jkl\" mno pqr stu vwx";
	TEST_EQUAL(s4.getFieldQuoted(0), "abcd")
	TEST_EQUAL(s4.getFieldQuoted(1), "efg")
	TEST_EQUAL(s4.getFieldQuoted(2), "jkl")
	TEST_EQUAL(s4.getFieldQuoted(3), "mno")
	TEST_EQUAL(s4.getFieldQuoted(4), "pqr")
	TEST_EQUAL(s4.getFieldQuoted(5), "stu")
	TEST_EQUAL(s4.getFieldQuoted(6), "vwx")
	s4 = "abcd efg \"jkl 'mno pqr' stu\" vwx";
	TEST_EQUAL(s4.getFieldQuoted(0), "abcd")
	TEST_EQUAL(s4.getFieldQuoted(1), "efg")
	TEST_EQUAL(s4.getFieldQuoted(2), "jkl 'mno pqr' stu")
	TEST_EQUAL(s4.getFieldQuoted(3), "vwx")
	TEST_EQUAL(s4.getFieldQuoted(4), "")
	s4 = "a b c d e f g";
	TEST_EQUAL(s4.getFieldQuoted(0), "a")
	TEST_EQUAL(s4.getFieldQuoted(1), "b")
	TEST_EQUAL(s4.getFieldQuoted(2), "c")
	TEST_EQUAL(s4.getFieldQuoted(3), "d")
	TEST_EQUAL(s4.getFieldQuoted(4), "e")
	TEST_EQUAL(s4.getFieldQuoted(5), "f")
	TEST_EQUAL(s4.getFieldQuoted(6), "g")
	TEST_EQUAL(s4.getFieldQuoted(7), "")
RESULT

CHECK(Size split(String string_array[], Size array_size, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index from = 0) const throw(Exception::IndexUnderflow, Exception::NullPointer))
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
	const char* c = "/()*-";
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
