// $Id: RegularExpression_test.C,v 1.4 2000/07/12 19:36:48 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/regExp.h>

///////////////////////////

START_TEST(RegularExpression, "$Id: RegularExpression_test.C,v 1.4 2000/07/12 19:36:48 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
String filename;
using std::ofstream;
using std::ios;

RegularExpression* re;

CHECK(RegularExpression())
	re = new RegularExpression();
	TEST_NOT_EQUAL(re, 0)
RESULT

CHECK(~RegularExpression())
	delete re;
RESULT

CHECK(RegularExpression(const String& pattern, bool wildcard_pattern = false))
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

CHECK(RegularExpression(const RegularExpression& regular_expression, bool deep = true))
	re = new RegularExpression(re1);
	TEST_NOT_EQUAL(re, 0)
	if (re != 0)
	{
		TEST_EQUAL((*re).getPattern(), "abc");
	}
	delete re;	
RESULT

CHECK(clear)
	re1.clear();
	TEST_EQUAL(re1.getPattern(), "");
RESULT

CHECK(destroy)
	re2.clear();
	TEST_EQUAL(re2.getPattern(), "");
RESULT

CHECK(RegularExpression::set(const String& pattern, bool wildcard_pattern = false))
	re1.set("abc*d", true);
	TEST_EQUAL(re1.getPattern(), "abc.*d");
	re1.set("abc", true);
	TEST_EQUAL(re1.getPattern(), "abc");
RESULT

CHECK(RegularExpression::set(const RegularExpression& regular_expression, bool deep = true))
	re2.set(re1);
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(RegularExpression::get(RegularExpression& regular_expression, bool deep = true) const )
	re2.clear();
	re1.get(re2);
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(RegularExpression::getPattern() const )
	TEST_EQUAL(re2.getPattern(), "abc");
RESULT

CHECK(RegularExpression::countSubexpressions() const )////////////////////////
	TEST_EQUAL(re2.countSubexpressions(), 0);
	re2.set("[A-Z]+[0-9]+");
	TEST_EQUAL(re2.countSubexpressions(), 0);
RESULT

CHECK(RegularExpression::match(const char* text, const char* pattern, int compile_flags = 0 | REG_EXTENDED | REG_NOSUB, int execute_flags = 0 ))
  //BAUSTELLE
RESULT

CHECK(RegularExpression::match(const String& text, Index from = 0, int execute_flags = 0 ) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::match(const Substring& text, Index from = 0, int execute_flags = 0) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::match(const char* text, int execute_flags = 0) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::find(const char* text, const char* pattern, const char** found_substring_from = 0, const char** found_substring_to = 0, int compile_flags = 0 | REG_EXTENDED, int execute_flags = 0))
  //BAUSTELLE
RESULT

CHECK(RegularExpression::find(const String& text, Substring& found, Index from = 0, int execute_flags = 0) const )
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

CHECK(RegularExpression::find(const String& text, Substring found_subexpression[], Size number_of_subexpressions, Index from = 0, int execute_flags = 0) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::find(const Substring& text, Substring& found, Index from = 0, int execute_flags = 0) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::find(const Substring& text, Substring found_subexpressions[], Size number_of_subexpressions, Index from = 0, int execute_flags = 0) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::find(const char* text, const char** found_substrings_from = 0, const char** found_substring_to = 0, int execute_flags = 0) const )
  //BAUSTELLE
RESULT

CHECK(RegularExpression::isEmpty() const )
	re2.set("abc");
	TEST_EQUAL(re2.isEmpty(), false)
	re2.set("");
	TEST_EQUAL(re2.isEmpty(), true)
RESULT

CHECK(RegularExpression::bool operator == (const RegularExpression& regular_expression) const )
	re2.set("abc");
	re1.set("abc");
	TEST_EQUAL(re2 == re1, true)
	re2.set("");
	TEST_EQUAL(re2 == re1, false)
RESULT

CHECK(RegularExpression::bool operator != (const RegularExpression& regular_expression) const )
	TEST_EQUAL(re2 != re1, true)
	re2.set("abc");
	TEST_EQUAL(re2 != re1, false)
RESULT

CHECK(RegularExpression::bool operator < (const RegularExpression& regular_expression) const )
	TEST_EQUAL(re2 < re1, false)
	re2.set("abb");
	TEST_EQUAL(re2 < re1, true)
RESULT

CHECK(RegularExpression::bool operator <= (const RegularExpression& regular_expression) const )
	TEST_EQUAL(re2 <= re1, true)
	re2.set(re1);
	TEST_EQUAL(re2 <= re1, true)
	re2.set("abd");
	TEST_EQUAL(re2 <= re1, false)
RESULT

CHECK(RegularExpression::bool operator >= (const RegularExpression& regular_expression) const )
	TEST_EQUAL(re2 >= re1, true)
	re2.set(re1);
	TEST_EQUAL(re2 >= re1, true)
	re2.set("abb");
	TEST_EQUAL(re2 >= re1, false)
RESULT

CHECK(RegularExpression::bool operator > (const RegularExpression& regular_expression) const )
	TEST_EQUAL(re2 > re1, false)
	re2.set("abd");
	TEST_EQUAL(re2 > re1, true)
RESULT

CHECK(RegularExpression::isValid() const )
	re2.set("abc");
	TEST_EQUAL(re2.isValid(), true)
RESULT

CHECK(RegularExpression::dump(::std::ostream& s = ::std::cout, Size depth = 0) const )
	re2.set("[A-Z]+");
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	re2.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Regular_Expression.txt", false)
RESULT

CHECK(RegularExpression::friend::std::ostream& operator << (::std::ostream& s, const RegularExpression& regular_expression))
	NEW_TMP_FILE(filename)
	std::ofstream outstr(filename.c_str(), File::OUT);
	outstr << re2;
	outstr.close();
	TEST_FILE(filename.c_str(), "data/Regular_Expression2.txt", false)
RESULT

CHECK(RegularExpression::friend::std::istream& operator >> (::std::istream& s, RegularExpression& regular_expression))
	std::ifstream instr("data/Regular_Expression2.txt");
	re2.clear();
	instr >> re2;
	instr.close();
	TEST_EQUAL(re2.getPattern(), "[A-Z]+")
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
