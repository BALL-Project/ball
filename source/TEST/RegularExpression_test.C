// $Id: RegularExpression_test.C,v 1.1 2000/05/23 14:19:52 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/regExp.h>

///////////////////////////

START_TEST(RegularExpression, "$Id: RegularExpression_test.C,v 1.1 2000/05/23 14:19:52 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///

CHECK(RegularExpression::set(const RegularExpression& regular_expression, bool deep = true))
  //BAUSTELLE
RESULT


CHECK(RegularExpression::set(const String& pattern, bool wildcard_pattern = false))
  //BAUSTELLE
RESULT


CHECK(RegularExpression::get(RegularExpression& regular_expression, bool deep = true) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::getPattern() const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::countSubexpressions() const )
  //BAUSTELLE
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
  //BAUSTELLE
RESULT


CHECK(RegularExpression::bool operator == (const RegularExpression& regular_expression) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::bool operator != (const RegularExpression& regular_expression) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::bool operator < (const RegularExpression& regular_expression) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::bool operator <= (const RegularExpression& regular_expression) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::bool operator >= (const RegularExpression& regular_expression) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::bool operator > (const RegularExpression& regular_expression) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::isValid() const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::dump(::std::ostream& s = ::std::cout, Size depth = 0) const )
  //BAUSTELLE
RESULT


CHECK(RegularExpression::friend::std::ostream& operator << (::std::ostream& s, const RegularExpression& regular_expression))
  //BAUSTELLE
RESULT


CHECK(RegularExpression::friend::std::istream& operator >> (::std::istream& s, RegularExpression& regular_expression))
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
