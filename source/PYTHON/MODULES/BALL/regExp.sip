// $Id: regExp.sip,v 1.1 2000/07/18 17:02:52 oliver Exp $

class RegularExpression
{
%HeaderCode
	#include <BALL/DATATYPE/regExp.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(RegularExpression)
  static const String ALPHA;
  static const String ALPHANUMERIC;
  static const String REAL;
  static const String IDENTIFIER;
  static const String INTEGER;
  static const String HEXADECIMAL_INTEGER;
  static const String LOWERCASE;
  static const String NON_ALPHA;
  static const String NON_ALPHANUMERIC;
  static const String NON_NUMERIC;
  static const String NON_WHITESPACE;
  static const String UPPERCASE;
  static const String WHITESPACE;
  RegularExpression();
  RegularExpression(const RegularExpression& /regular_expression/, bool /deep/ = true);
  RegularExpression(const String& /pattern/, bool /wildcard_pattern/ = false);
  ~RegularExpression();
  virtual void clear();
  void destroy();
  void set(const RegularExpression& /regular_expression/, bool /deep/ = true);
  void set(const String& /pattern/, bool /wildcard_pattern/ = false);
  void get(RegularExpression& /regular_expression/, bool /deep/ = true) const;
  const String& getPattern() const;
  Size countSubexpressions() const;
  //static bool match
  // (const char* /text/, const char* /pattern/,
  //  int /compile_flags/ = 0, int /execute_flags/ = 0 );
  bool match(const String& /text/, Index /from/ = 0, int /execute_flags/ = 0 ) const;
  bool match(const Substring& /text/, Index /from/ = 0, int /execute_flags/ = 0) const;
  bool match(const char* /text/, int /execute_flags/ = 0) const;
  // static bool find
  //  (const char* /text/, const char* /pattern/,
  //   const char** found_substring_from = 0,
  //   const char** found_substring_to = 0,
  //  int compile_flags = 0 | 1 ,
  //  int execute_flags = 0);
  bool find
   (const String& /text/, Substring& /found/,
    Index /from/ = 0, int /execute_flags/ = 0) const;
  // bool find
  // (const String& /text/, Substring /found_subexpression[],
  //  Size number_of_subexpressions, Index from = 0, int execute_flags = 0) const;
  bool find
   (const Substring& /text/, Substring& /found/,
    Index /from/ = 0, int /execute_flags/ = 0) const;
  // bool find
  // (const Substring& text, Substring found_subexpressions[],
  //  Size number_of_subexpressions, Index from = 0, int execute_flags = 0) const;
  //bool find
  // (const char* /text/, const char** found_substrings_from = 0,
  //  const char** found_substring_to = 0, int execute_flags = 0) const;
  bool isEmpty() const;
  //bool operator == (const RegularExpression& regular_expression) const;
  //bool operator != (const RegularExpression& regular_expression) const;
  //bool operator < (const RegularExpression& regular_expression) const;
  //bool operator <= (const RegularExpression& regular_expression) const;
  //bool operator >= (const RegularExpression& regular_expression) const;
  //bool operator > (const RegularExpression& regular_expression) const;
  virtual bool isValid() const;
  // virtual void dump(::std::ostream& s = ::std::cout, Size depth = 0) const;
  // friend ::std::ostream& operator << (::std::ostream& s, const RegularExpression& regular_expression);
  // friend ::std::istream& operator >> (::std::istream& s, RegularExpression& regular_expression);
};
