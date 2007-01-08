#! python
from ClassTest import *

START_TEST(String,"$Id: String_test1.py,v 1.3 2003/08/26 09:18:25 oliver Exp $")

CHECK('String::String()')
s = String()
TEST_EQUAL(s is None, false)
RESULT()

CHECK('String::size()/c_str()')
s = String()
TEST_EQUAL(len(s.c_str()), 0)
TEST_EQUAL(s.size(), 0)

s = String("ABC")
TEST_EQUAL(s.size(), 3)
TEST_EQUAL(len(s.c_str()), 3)
TEST_EQUAL(s.c_str()[0], 'A')
TEST_EQUAL(s.c_str()[1], 'B')
TEST_EQUAL(s.c_str()[2], 'C')
RESULT()

CHECK('String::String(String&)')
s2 = String(s)
TEST_EQUAL(s2.size(), 3)

s3 = String()
s2 = String(s3)
TEST_EQUAL(s2.size(), 0)
RESULT()

empty = String()
non_empty = String("Hallo")
CHECK('String::operator == (char*)')
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
RESULT()

hall = String("Hall")
halloh = String("Halloh")
CHECK('String::operator == (String&)')
TEST_EQUAL((empty == non_empty), false)
TEST_EQUAL((empty == empty), true)
TEST_EQUAL((non_empty == non_empty), true)
TEST_EQUAL((non_empty == hall), false)
TEST_EQUAL((non_empty == halloh), false)
RESULT()

CHECK('String::String(String&, Index, Size)')
s2 = String(halloh, 2, 4)
TEST_EQUAL(s2, "lloh")

s2 = String(halloh, 4, 2)
TEST_EQUAL(s2, "oh")

s2 = String(halloh, 3)
TEST_EQUAL(s2, "loh")

s2 = String(halloh, -2)
TEST_EQUAL(s2, "oh")

s2 = String(halloh, 0, 0)
TEST_EQUAL(s2, "")

s2 = String(halloh, -1)
TEST_EQUAL(s2, "h")

try:
	s2 = String(halloh, 1, 7)
	FAIL()
except:
	pass
RESULT()

CHECK('String::String(const char*, Index, Size)')
s2 = String("halloh")
TEST_EQUAL(s2, "halloh")
s2 = String("halloh", 1)
TEST_EQUAL(s2, "alloh")
s2 = String("halloh", 1, 2)
TEST_EQUAL(s2, "al")
s2 = String("halloh", -1, 0)
TEST_EQUAL(s2, "")
s2 = String("halloh", -5)
TEST_EQUAL(s2, "alloh")
s2 = String("halloh", 2, 3)
TEST_EQUAL(s2, "llo")
try:
	s2 = String("halloh", 0, 8)
	FAIL()
except:
	pass
try:
	s2 = String("halloh", -12)
	FAIL()
except:
	pass
RESULT()

#CHECK('String::String(stringstream)')
##ifdef BALL_HAS_SSTREAM
#	std::stringstream instream("ABC DEF")
##else
#	std::strstream instream("ABC DEF", len("ABC DEF"), ios::app)
##endif
#	s2 = String(instream)
#	TEST_NOT_EQUAL(s2, 0)
#	TEST_EQUAL(*s2, "ABC")
#	delete s2
#	s2 = String(instream)
#	TEST_EQUAL(*s2, "DEF")
#	delete s2
#RESULT()
#
#CHECK('String::String(char, Size)')
#	s2 = String('a', (Size)1)
#	TEST_EQUAL(*s2, "a")
#	delete s2
#	s2 = String('a', (Size)2)
#	TEST_EQUAL(*s2, "aa")
#	delete s2
#	s2 = String('a', (Size)0)
#	TEST_EQUAL(*s2, "")
#	delete s2
#RESULT()
#
#CHECK('String::String(char)')
#	s2 = String('a')
#	TEST_EQUAL(*s2, "a")
#	delete s2
#RESULT()
#
#CHECK('String::String(unsigned char)')
#	s2 = String((unsigned char)'b')
#	TEST_EQUAL(*s2, "b")
#	delete s2
#RESULT()
#
#CHECK('String::String(short)')
#	s2 = String((short)7)
#	TEST_EQUAL(*s2, "7")
#	delete s2
#	s2 = String((short)-8)
#	TEST_EQUAL(*s2, "-8")
#	delete s2
#RESULT()
#
#CHECK('String::String(unsigned short)')
#	s2 = String((unsigned short)7)
#	TEST_EQUAL(*s2, "7")
#	delete s2
#	s2 = String((unsigned short)0)
#	TEST_EQUAL(*s2, "0")
#	delete s2
#RESULT()
#
#CHECK('String::String(int)')
#	s2 = String((int)7)
#	TEST_EQUAL(*s2, "7")
#	delete s2
#	s2 = String((int)-8)
#	TEST_EQUAL(*s2, "-8")
#	delete s2
#RESULT()
#
#CHECK('String::String(unsigned int)')
#	s2 = String((unsigned int)7)
#	TEST_EQUAL(*s2, "7")
#	delete s2
#	s2 = String((unsigned int)0)
#	TEST_EQUAL(*s2, "0")
#	delete s2
#RESULT()
#
#CHECK('String::String(unsigned long)')
#	s2 = String((unsigned long)7)
#	TEST_EQUAL(*s2, "7")
#	delete s2
#	s2 = String((unsigned long)0)
#	TEST_EQUAL(*s2, "0")
#	delete s2
#RESULT()
#
#CHECK('String::String(long)')
#	s2 = String((long)7)
#	TEST_EQUAL(*s2, "7")
#	delete s2
#	s2 = String((long)-8)
#	TEST_EQUAL(*s2, "-8")
#	delete s2
#RESULT()
#
#CHECK('String::String(float)')
#	s2 = String((float)0.0)
#	TEST_REAL_EQUAL(atof(s2.c_str()), 0)
#	delete s2
#	s2 = String((float)-1.2)
#	TEST_REAL_EQUAL(atof(s2.c_str()), -1.2)
#	delete s2
#RESULT()
#
#CHECK('String::String(double)')
#	s2 = String((double)0.0)
#	TEST_REAL_EQUAL(atof(s2.c_str()), 0)
#	delete s2
#	s2 = String((double)-1.2)
#	TEST_REAL_EQUAL(atof(s2.c_str()), -1.2)
#	delete s2
#RESULT()
#
#CHECK('String::destroy()')
#	s2 = String("hallo")
#	s2.destroy()
#	TEST_EQUAL(*s2, "")
#RESULT()
#delete s2
#
#const String abcdef = "abcdef"
#const String ABCDEF = "ABCDEF"
#Substring empty_sub
#const Substring abcdef_sub(abcdef, 0, 6)
#const Substring ABCDEF_sub(ABCDEF, 0, 6)
#Substring test_sub1(ABCDEF, 0, 6)
#Substring test_sub2
#String test_string
#
#CHECK('String::set(String&, Index, Size)')
#	s2 = String("AAAA")
#	s2.set(halloh)
#	TEST_EQUAL(*s2, halloh)
#	s2.set(halloh, 1)
#	TEST_EQUAL(*s2, "alloh")
#	s2.set(halloh, -2)
#	TEST_EQUAL(*s2, "oh")
#	s2.set(halloh, -2, 0)
#	TEST_EQUAL(*s2, "")
#	s2.set(halloh, 1, 2)
#	TEST_EQUAL(*s2, "al")
#	TEST_EXCEPTION(Exception::IndexUnderflow, s2.set(halloh, -10, 3))
#	TEST_EXCEPTION(Exception::IndexOverflow, s2.set(halloh, 0, 10))
#RESULT()
#delete s2
#
#CHECK('String::set(char*, Index, Size)')
#	s2 = String("AAAA")
#	s2.set("halloh")
#	TEST_EQUAL(*s2, "halloh")
#	s2.set("halloh", 1)
#	TEST_EQUAL(*s2, "alloh")
#	s2.set("halloh", -2)
#	TEST_EQUAL(*s2, "oh")
#	s2.set("halloh", -2, 0)
#	TEST_EQUAL(*s2, "")
#	s2.set("halloh", 1, 2)
#	TEST_EQUAL(*s2, "al")
#	TEST_EXCEPTION(Exception::IndexUnderflow, s2.set("halloh", -10, 3))
#	TEST_EXCEPTION(Exception::IndexOverflow, s2.set("halloh", 0, 10))
#RESULT()
#delete s2
#
#CHECK('String::set(Size, char*, ...)')
#	s2 = String
#	s2.set(15, "%s", "halloh")
#	TEST_EQUAL(*s2, "halloh")
#	s2.set(2, "%s", "halloh")
#	TEST_EQUAL(*s2, "h")
#	s2.set(15, "%3.1f", 1.2)
#	TEST_EQUAL(*s2, "1.2")
#	TEST_EXCEPTION(Exception::IndexUnderflow, s2.set(0, "%s", "Halloh"))
#	TEST_EXCEPTION(Exception::NullPointer, s2.set(15, (char*)0, "Halloh", 1.5, 1.2))
#RESULT()
#delete s2
#
#CHECK('String::set(stringstream&)')
##ifdef BALL_HAS_SSTREAM
#	std::stringstream instream("ABC DEF GHI jkl mno")
##else
#	std::strstream instream("ABC DEF GHI jkl mno", strlen("ABC DEF GHI jkl mno"), ios::app)
##endif
#
#	s2 = String
#	s2.set(instream)
#	TEST_EQUAL(*s2, "ABC")
#	s2.set(instream)
#	TEST_EQUAL(*s2, "DEF")
#	delete s2
#RESULT()
#
#s2 = String
#CHECK('String::set(char, Size)')
#	s2.set('A')
#	TEST_EQUAL(*s2, "A")
#	s2.set('B', (Size)2)
#	TEST_EQUAL(*s2, "BB")
#	s2.set('C', (Size)0)
#	TEST_EQUAL(*s2, "")
#RESULT()
#
#CHECK('String::set(unsigned char)')
#	s2.set((unsigned char)'b')
#	TEST_EQUAL(*s2, "b")
#RESULT()
#
#CHECK('String::set(short)')
#	s2.set((short)7)
#	TEST_EQUAL(*s2, "7")
#	s2.set((short)-8)
#	TEST_EQUAL(*s2, "-8")
#RESULT()
#
#CHECK('String::set(unsigned short)')
#	s2.set((unsigned short)7)
#	TEST_EQUAL(*s2, "7")
#	s2.set((unsigned short)0)
#	TEST_EQUAL(*s2, "0")
#RESULT()
#
#CHECK('String::set(int)')
#	s2.set((int)7)
#	TEST_EQUAL(*s2, "7")
#	s2.set((int)-8)
#	TEST_EQUAL(*s2, "-8")
#RESULT()
#
#CHECK('String::set(unsigned int)')
#	s2.set((unsigned int)7)
#	TEST_EQUAL(*s2, "7")
#	s2.set((unsigned int)0)
#	TEST_EQUAL(*s2, "0")
#RESULT()
#
#CHECK('String::set(unsigned long)')
#	s2.set((unsigned long)7)
#	TEST_EQUAL(*s2, "7")
#	s2.set((unsigned long)0)
#	TEST_EQUAL(*s2, "0")
#RESULT()
#
#CHECK('String::set(long)')
#	s2.set((long)7)
#	TEST_EQUAL(*s2, "7")
#	s2.set((long)-8)
#	TEST_EQUAL(*s2, "-8")
#RESULT()
#
#CHECK('String::set(float)')
#	s2.set((float)0.0)
#	TEST_REAL_EQUAL(atof(s2.c_str()), 0)
#	s2.set((float)-1.2)
#	TEST_REAL_EQUAL(atof(s2.c_str()), -1.2)
#RESULT()
#
#CHECK('String::set(double)')
#	s2.set((double)0.0)
#	TEST_REAL_EQUAL(atof(s2.c_str()), 0)
#	s2.set((double)-1.2)
#	TEST_REAL_EQUAL(atof(s2.c_str()), -1.2)
#RESULT()
#
#CHECK('String::get(char*, Index, Size)')
#	char test_string[15]
#	s2.set("TEST")
#	s2.get(test_string)
#	TEST_EQUAL(strlen(test_string), 4)
#	TEST_EQUAL(strcmp(test_string, "TEST"), 0)
#	s2.get(test_string, 1)
#	TEST_EQUAL(strcmp(test_string, "EST"), 0)
#	s2.get(test_string, -2)
#	TEST_EQUAL(strcmp(test_string, "ST"), 0)
#	s2.get(test_string, 0, 1)
#	TEST_EQUAL(strcmp(test_string, "T"), 0)
#	s2.get(test_string, 1, 1)
#	TEST_EQUAL(strcmp(test_string, "E"), 0)
#	s2.get(test_string, 2, 2)
#	TEST_EQUAL(strcmp(test_string, "ST"), 0)
#RESULT()
#delete s2
#
#String s4
#
#CHECK('String::operator = (String&)')
#	String s5("Hallo")
#	s4 = s5
#	TEST_EQUAL(s4, s5)
#	s5.set("")
#	s4 = s5
#	TEST_EQUAL(s4, s5)
#	String s6
#	s4 = s6
#	TEST_EQUAL(s4, s6)
#RESULT()
#
#CHECK('String::operator = (char*)')
#	s4 = "TestTestTestTest"
#	TEST_EQUAL(s4, "TestTestTestTest")
#	s4 = ""
#	TEST_EQUAL(s4, "")
#RESULT()
#
#CHECK('String::operator = (stringstream&)')
##ifdef BALL_HAS_SSTREAM
#	std::stringstream instream("ABC DEF GHI jkl mni")
##else
#	std::strstream instream("ABC DEF GHI jkl mni", strlen("ABC DEF GHI jkl mni"),ios::app)
##endif
#
#	s2 = String
#	*s2 = instream
#	TEST_EQUAL(*s2, "ABC")
#	*s2 = instream
#	TEST_EQUAL(*s2, "DEF")
#	delete s2
#RESULT()
#
#CHECK('String::operator = (char)')
#	s4 = 'a'
#	TEST_EQUAL(s4, "a")
#RESULT()
#
#CHECK('String::operator = (unsigned char)')
#	s4 = (unsigned char)'b'
#	TEST_EQUAL(s4, "b")
#RESULT()

END_TEST()
