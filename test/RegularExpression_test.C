// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DATATYPE/regularExpression.h>

///////////////////////////

START_TEST(RegularExpression)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

RegularExpression* re = 0;

CHECK(RegularExpression() throw())
	re = new RegularExpression();
	TEST_NOT_EQUAL(re, 0)
RESULT

CHECK(~RegularExpression() throw())
	delete re;
RESULT

CHECK(BALL_CREATE(RegularExpression))
	// ????
RESULT

CHECK(RegularExpression(const String& pattern, bool wildcard_pattern = false) throw())
	re = new RegularExpression("abc");
	TEST_NOT_EQUAL(re, 0)
	if (re != 0)
	{
		TEST_EQUAL((*re).getPattern(), "abc");
	}
	delete re;
RESULT

RegularExpression re1("abc");
RegularExpression re2("abc");

CHECK(RegularExpression(const RegularExpression& regular_expression) throw())
	re = new RegularExpression(re1);
	TEST_NOT_EQUAL(re, 0)
	if (re != 0)
	{
		TEST_EQUAL((*re).getPattern(), "abc");
	}
	delete re;	
RESULT

CHECK(void clear() throw())
	re1.clear();
	TEST_EQUAL(re1.getPattern(), "");
RESULT

CHECK(void destroy() throw())
	re2.clear();
	TEST_EQUAL(re2.getPattern(), "");
RESULT

CHECK(void set(const String& pattern, bool wildcard_pattern = false) throw())
	re1.set("abc*d", true);
	TEST_EQUAL(re1.getPattern(), "abc.*d");
	re1.set("abc", true);
	TEST_EQUAL(re1.getPattern(), "abc");
RESULT

CHECK(void set(const RegularExpression& regular_expression) throw())
	re2.set(re1);
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(void get(RegularExpression& regular_expression) const throw())
	re2.clear();
	re1.get(re2);
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(RegularExpression& operator = (const RegularExpression& expression) throw())
	re2.clear();
	re2 = re1;
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(const String& getPattern() const throw())
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(Size countSubexpressions() const throw())
	TEST_EQUAL(re2.countSubexpressions(), 0);
	TEST_EQUAL(re2.isValid(), true)
	re2.set("([A-Z]+[0-9]+).*([0-9])");
	TEST_EQUAL(re2.isValid(), true)
	TEST_EQUAL(re2.countSubexpressions(), 2);
RESULT

CHECK(static bool match(const char* text, const char* pattern, int compile_flags = 0 | REG_EXTENDED | REG_NOSUB, int execute_flags = 0) throw(Exception::NullPointer))
	char* c = 0;
	TEST_EXCEPTION(Exception::NullPointer, RegularExpression::match(c, "a*bd"))
	TEST_EXCEPTION(Exception::NullPointer, RegularExpression::match("a*bd", c))
	TEST_EQUAL(RegularExpression::match("abbcbbd", ".*bd"), true)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "bd"), true)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "b.a"), false)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "a*d"), true)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "a..d"), false)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "x"), false)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "a[bc]+d"), true)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "a[bc][bc]d"), false)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "a[b]+d"), false)
	TEST_EQUAL(RegularExpression::match("abbcbbd", "a[BC]+d"), false)
RESULT

CHECK(bool match(const String& text, Index from = 0, int execute_flags = 0) const throw(Exception::NullPointer, Exception::IndexUnderflow, Exception::IndexOverflow))
	re2.set("a[bc]+d");
	String s = "abbcbbd";
	TEST_EQUAL(re2.isValid(), true)
	TEST_EQUAL(re2.match(s), true)
	TEST_EQUAL(re2.match(s, 1, 0), false)
	re2.set(".*bd");
	TEST_EQUAL(re2.isValid(), true)
	TEST_EQUAL(re2.match(s), true)
RESULT

CHECK(bool match(const Substring& text, Index from = 0, int execute_flags = 0) const throw(Substring::InvalidSubstring, Exception::IndexUnderflow, Exception::IndexOverflow))
	re2.set("a[bc]+d");
	String s = "abbcbbd";
	Substring ss;
	ss.bind(s);
	TEST_EQUAL(re2.match(ss), true)
	TEST_EQUAL(re2.match(ss, 1, 0), false)
RESULT

CHECK(bool match(const char* text, int execute_flags = 0) const throw(Exception::NullPointer))
	const char* s = "abbcbbd";
	TEST_EQUAL(re2.match(s), true)
RESULT

CHECK(bool find(const String& text, Substring& found, Index from = 0, int execute_flags = 0) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	Substring sub;
	String text("1234ABC123");
	RegularExpression re("[A-Z]+");
	bool result = re.find(text, sub);
	TEST_EQUAL(result, true)
	TEST_EQUAL(sub.isBound(), true)
	TEST_EQUAL(sub.size(), 3)
	TEST_EQUAL(sub.getFirstIndex(), 4)
	sub.set("");
	TEST_EQUAL(text, "1234123")
RESULT

CHECK(bool find(const String& text, vector<Substring>& subexpressions, Index from = 0, int execute_flags = 0) const throw(Exception::IndexUnderflow, Exception::IndexOverflow))
	re2.set("(bb).*(bd)");
	String s = "abbcbbd";
	vector<Substring> ss;
	TEST_EQUAL(re2.find(s, ss, 0), true)
	TEST_EQUAL(ss.size(), 3)
	TEST_EQUAL(ss[0].isBound(), true)
	TEST_EQUAL(ss[0].size(), 6)
	TEST_EQUAL(ss[0], "bbcbbd")
	TEST_EQUAL(ss[1].isBound(), true)
	TEST_EQUAL(ss[1].size(), 2)
	TEST_EQUAL(ss[1], "bb")
	TEST_EQUAL(ss[2].isBound(), true)
	TEST_EQUAL(ss[2].size(), 2)
	TEST_EQUAL(ss[2], "bd")
	TEST_EQUAL(re2.find(s, ss, 5), false)
RESULT


CHECK(bool isEmpty() const throw())
	re2.set("abc");
	TEST_EQUAL(re2.isEmpty(), false)
	re2.set("");
	TEST_EQUAL(re2.isEmpty(), true)
RESULT

CHECK(bool operator == (const RegularExpression& regular_expression) const throw())
	re2.set("abc");
	re1.set("abc");
	TEST_EQUAL(re2 == re1, true)
	re2.set("");
	TEST_EQUAL(re2 == re1, false)
RESULT

CHECK(bool operator != (const RegularExpression& regular_expression) const throw())
	TEST_EQUAL(re2 != re1, true)
	re2.set("abc");
	TEST_EQUAL(re2 != re1, false)
RESULT

CHECK(bool operator < (const RegularExpression& regular_expression) const throw())
	TEST_EQUAL(re2 < re1, false)
	re2.set("abb");
	TEST_EQUAL(re2 < re1, true)
RESULT

CHECK(bool operator <= (const RegularExpression& regular_expression) const throw())
	TEST_EQUAL(re2 <= re1, true)
	re2.set(re1);
	TEST_EQUAL(re2 <= re1, true)
	re2.set("abd");
	TEST_EQUAL(re2 <= re1, false)
RESULT

CHECK(bool operator >= (const RegularExpression& regular_expression) const throw())
	TEST_EQUAL(re2 >= re1, true)
	re2.set(re1);
	TEST_EQUAL(re2 >= re1, true)
	re2.set("abb");
	TEST_EQUAL(re2 >= re1, false)
RESULT

CHECK(bool operator > (const RegularExpression& regular_expression) const throw())
	TEST_EQUAL(re2 > re1, false)
	re2.set("abd");
	TEST_EQUAL(re2 > re1, true)
RESULT

CHECK(bool isValid() const throw())
	re2.set("abc");
	TEST_EQUAL(re2.isValid(), true)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	re2.set("[A-Z]+");
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	re2.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Regular_Expression.txt))
RESULT

CHECK(friend std::ostream& operator << (std::ostream& s, const RegularExpression& regular_expression) throw())
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), std::ios::out);
	outstr << re2;
	outstr.close();
	TEST_FILE(filename.c_str(), BALL_TEST_DATA_PATH(Regular_Expression2.txt))
RESULT

CHECK(friend std::istream& operator >> (std::istream& s, RegularExpression& regular_expression) throw())
	std::ifstream instr(BALL_TEST_DATA_PATH(Regular_Expression2.txt));
	re2.clear();
	instr >> re2;
	instr.close();
	TEST_EQUAL(re2.getPattern(), "[A-Z]+")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
