// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%ModuleCode
	#include <BALL/DATATYPE/regularExpression.h>
%End

class RegularExpression
{
%TypeHeaderCode
	#include <BALL/DATATYPE/regularExpression.h>
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
  RegularExpression(const RegularExpression&);
  RegularExpression(const String&, bool wildcard_pattern = false);
  ~RegularExpression() throw();
  virtual void clear() throw();
  void destroy();
  void set(const RegularExpression&);
  void set(const String&, bool wildcard_pattern = false);
  void get(RegularExpression&) const;
  const String& getPattern() const;
  Size countSubexpressions() const;
  bool match(const char*, const char*, int compile_flags = REG_EXTENDED | REG_NOSUB, int execute_flags = 0 );
  bool match(const String&, int from = 0, int execute_flags = 0) const throw(NullPointer, IndexUnderflow, IndexOverflow);
  bool match(const Substring&, int from = 0, int execute_flags = 0) const;
  bool match(const char*, int execute_flags = 0) const;

  // static bool find
  //  (const char*, const char*,
  //   const char** found_substring_from = 0,
  //   const char** found_substring_to = 0,
  //  int compile_flags = 0 | 1 ,
  //  int execute_flags = 0);
  bool find
   (const String&, Substring&,
    int from = 0, int execute_flags = 0) const;
  // bool find
  // (const String&, Substring /found_subexpression[],
  //  int number_of_subexpressions, int from = 0, int execute_flags = 0) const;
  bool find
   (const Substring&, Substring&,
    int from = 0, int execute_flags = 0) const;
  // bool find
  // (const Substring& text, Substring found_subexpressions[],
  //  Size number_of_subexpressions, int from = 0, int execute_flags = 0) const;
  //bool find
  // (const char*, const char** found_substrings_from = 0,
  //  const char** found_substring_to = 0, int execute_flags = 0) const;
  bool isEmpty() const;
  bool operator == (const RegularExpression&) const;
  bool operator != (const RegularExpression&) const;
  bool operator < (const RegularExpression&) const;
  bool operator <= (const RegularExpression&) const;
  bool operator >= (const RegularExpression&) const;
  bool operator > (const RegularExpression&) const;
  virtual bool isValid() const throw();
  // virtual void dump(::std::ostream& s = ::std::cout, unsigned int depth = 0) const;
  // friend ::std::ostream& operator << (::std::ostream& s, const RegularExpression&);
  // friend ::std::istream& operator >> (::std::istream& s, RegularExpression&);
};
