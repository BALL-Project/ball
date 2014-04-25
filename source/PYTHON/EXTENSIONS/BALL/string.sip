// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Substring
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	using namespace BALL;
	using namespace BALL::Exception;
%End
  // friend class String;
  public:
  // BALL_CREATE(Substring)
  // class UnboundSubstring
  //  : public Exception::GeneralException
  // {
  //  public:
  //  UnboundSubstring(const char* file, int line); 
  // };
  // class InvalidSubstring
  //  : public Exception::GeneralException
  // {
  //  public:
  //  InvalidSubstring(const char* file, int line); 
  // };
  Substring();
  Substring(const Substring&, bool deep = true);
  Substring(const String&, Index from = 0, Size len = String::EndPos);
  ~Substring();
  void destroy();
  // operator String() const;
  String toString() const;
  Substring& bind(const String&, int from = 0, int len = String::EndPos);
  Substring& bind(const Substring&, int from = 0, int len = String::EndPos);
  void unbind();

  String* getBoundString();
  // const String* getBoundString() const;

  void set(const String&);
  void set(const Substring&);
  void set(const char*, Size size = String::EndPos);
  // Substring& operator = (const String& string);
  // Substring& operator = (const Substring& substring);
  // Substring& operator = (const char* char_ptr);
  Index getFirstIndex() const;
  Index getLastIndex() const;
  Size size() const;
  // char& operator [] (Index);
  char operator [] (Index) const;
%MethodCode
	sipRes = sipCpp->operator [] (*a0);
%End

  Substring& toLower();
  Substring& toUpper();
  bool isBound() const;
  bool isEmpty() const;
  bool operator == (const Substring&) const;
  bool operator != (const Substring&) const;
  bool operator == (const String&) const;
  bool operator != (const String&) const;
  // friend bool operator == (const String& string, const Substring& substring);
  // friend bool operator != (const String& string, const Substring& substring);
  bool operator == (const char*) const;
  bool operator != (const char*) const;
  bool operator == (char) const;
  bool operator != (char) const;
  // friend ::std::ostream& operator << (::std::ostream& s, const Substring& substring);
  bool isValid() const;
  // void dump(::std::ostream& s = ::std::cout, int depth = 0) const;
};


class String
// : public string
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/DATATYPE/hashMap.h>
	using namespace BALL;
	using namespace std;
%End

  // friend class Substring;
  public:
  // virtual void* create(bool   = true, bool empty = false) const;
  enum CompareMode
  { 
   CASE_SENSITIVE   = 0, 
   CASE_INSENSITIVE = 1 
  };
  const char* CHARACTER_CLASS__ASCII_ALPHA;
  const char* CHARACTER_CLASS__ASCII_ALPHANUMERIC;
  const char* CHARACTER_CLASS__ASCII_LOWER;
  const char* CHARACTER_CLASS__ASCII_UPPER;
  const char* CHARACTER_CLASS__ASCII_NUMERIC;
  const char* CHARACTER_CLASS__WHITESPACE;

  String();

  String(const String&);
  // String(const string& string);

  String(const String&, Index, Size len = String::EndPos)
		throw(NullPointer, IndexOverflow, IndexUnderflow);

	String(const char*)
		throw(NullPointer, IndexOverflow, IndexUnderflow);
  String(const char*, Index) 
		throw(NullPointer, IndexOverflow, IndexUnderflow);
  String(const char*, Index, Size) 
		throw(NullPointer, IndexOverflow, IndexUnderflow);

  // String(Size buffer_size, const char* format, ... );
  // String(std::strstream& s);
  // String(char, Size size = 1);
  // String(const unsigned char uc);
  //String(short);
  // String(unsigned short us);
  //String(int);
  // String(unsigned int ui);
  //String(long);
  // String(unsigned long );
  //String(float);
  String(double);
  ~String();
  void destroy();
  void set(const String&, int from = 0, int len = String::EndPos);
  void set(const char*, int from = 0, int len = String::EndPos);
  // void set(Size, const char *format, ...);
  // void set(std::strstream& s);
  // void set(char, Size len = 1);
  // void set(unsigned char uc);
  //void set(short);
  // void set(unsigned short us);
  //void set(int);
  // void set(unsigned int ui);
  void set(long);
  // void set(unsigned long ul);
  //void set(float);
  //void set(double);
  // void get(char* char_ptr, int from = 0, int len = String::EndPos) const;
  // String& operator = (const String& s);
  // String& operator = (const char* pc);
  // String& operator = (std::strstream& s);
  // String& operator = (char c);
  // String& operator = (unsigned char uc);
  // String& operator = (short s);
  // String& operator = (unsigned short us);
  // String& operator = (int i);
  // String& operator = (unsigned int ui);
  // String& operator = (long l);
  // String& operator = (unsigned long );
  // String& operator = (float f);
  // String& operator = (double d);
  static void setCompareMode(CompareMode);
  static CompareMode getCompareMode();
  bool toBool() const;
  char toChar() const;
  // unsigned char toUnsignedChar() const;
  short toShort() const;
  // unsigned short toUnsignedShort() const;
  int toInt() const;
  // unsigned int toUnsignedInt() const;
  long toLong() const;
  // unsigned long toUnsignedLong() const;
  float toFloat() const;
  double toDouble() const;
  void toLower(int from = 0, int len = String::EndPos);
  void toUpper(int from = 0, int len = String::EndPos);
  Substring getSubstring(int from = 0, int len = String::EndPos) const; 
  Substring operator () (int, int len = String::EndPos) const; 
%MethodCode
	sipRes = new Substring(sipCpp->operator () (a0, a1));
%End

  Substring before(const String&, int from = 0) const;
  Substring through(const String&, int from = 0) const;
  Substring from(const String&, int from = 0) const;
  Substring after(const String&, int from = 0) const;
  Size countFields(const char* delimiters = String::CHARACTER_CLASS__WHITESPACE) const;
  String getField(Index, const char* delimiters = String::CHARACTER_CLASS__WHITESPACE) const;
  // Size split(String string_array[], Size array_size, const char* delimiters = CHARACTER_CLASS__WHITESPACE, int from = 0) const;
  String& trimLeft(const char* trimmed = String::CHARACTER_CLASS__WHITESPACE);
  String& trimRight(const char* trimmed_chars = String::CHARACTER_CLASS__WHITESPACE);
  String& trim(const char* trimmed_chars = String::CHARACTER_CLASS__WHITESPACE);
  String& truncate(Size);
  Substring left(Size) const;
  Substring right(Size) const;
  Substring instr(const String&, int from = 0) const;
  // String operator + (const string&) const;
	String operator + (const String&) const;
  String operator + (const char*) const;
  String operator + (char) const;
  // friend String operator + (const char* char_ptr, const String& s);
  // friend String operator + (char c, const String& s);
  void swap(String&);
  String& reverse(int from = 0, int len = String::EndPos);
  Index substitute(const String&, const String&);
  bool has(char) const;
  bool hasSubstring(const String&, int from = 0) const;
  bool hasPrefix(const String&) const;
  bool hasSuffix(const String&) const;
  bool isEmpty() const;
  bool isAlpha() const;
  bool isAlnum() const;
  bool isDigit() const;
  bool isSpace() const;
  bool isWhitespace() const;
  bool isAlpha(char);
  bool isAlnum(char);
  bool isDigit(char);
  bool isSpace(char);
  bool isWhitespace(char);
  int compare(const String&, int from = 0) const;
  int compare(const String&, int, int) const;
  int compare(const char*, int from = 0) const;
  int compare(const char*, int, int) const;
  int compare(char, int from = 0) const;
  // friend bool operator == (const char* char_ptr, const String&);
  // friend bool operator != (const char* char_ptr, const String&);
  // friend bool operator < (const char* char_ptr, const String&);
  // friend bool operator <= (const char* char_ptr, const String&);
  // friend bool operator > (const char* char_ptr, const String&);
  // friend bool operator >= (const char* char_ptr, const String&);
  bool operator == (const char*) const;
  bool operator != (const char*) const;
  bool operator < (const char*) const;
  bool operator <= (const char*) const;
  bool operator > (const char*) const;
  bool operator >= (const char*) const;
  // friend bool operator == (char c, const String&);
  // friend bool operator != (char c, const String&);
  // friend bool operator < (char c, const String&);
  // friend bool operator <= (char c, const String&);
  // friend bool operator > (char c, const String&);
  // friend bool operator >= (char c, const String&);
  bool operator == (char) const;
  bool operator != (char) const;
  bool operator < (char) const;
  bool operator <= (char) const;
  bool operator > (char) const;
  bool operator >= (char) const;
  bool isValid() const;
  // void dump(::std::ostream& s = ::std::cout, int depth = 0) const;
  // ::std::istream& getline(::std::istream& s = ::std::cin, char delimiter = '\n');
  // friend ::std::istream& getline(::std::istream& s,  String& string,  char delimiter = '\n');
	
	// std::string compatibility methods
	int size() const;
  const char* c_str() const;


	long __hash__();
%MethodCode
	boost::hash<String> h;
	sipRes = h(*sipCpp);
%End


  SIP_PYOBJECT __str__();
%MethodCode
	if (sipCpp == 0)
	{
		sipRes = PyString_FromString("");
	}
	else
	{
		sipRes = PyString_FromString(sipCpp->c_str());
	}
%End

SIP_PYOBJECT __repr__();
%MethodCode
	if (sipCpp == 0)
	{
		sipRes = PyString_FromString("");
	}
	else
	{
		sipRes = PyString_FromString(sipCpp->c_str());
	}
%End

%ConvertToTypeCode
  if (sipIsErr == NULL)
    return (PyString_Check(sipPy) || BALL_IS_SUBCLASS_INSTANCE(sipPy, String));

  if (sipPy == Py_None)
  {
    *sipCppPtr = new String;

    return 1;
  }
  if (PyString_Check(sipPy))
  {
    *sipCppPtr = new String(PyString_AS_STRING(sipPy));

    return 1;
  }

  *sipCppPtr = BALL_CONVERT_TO_CPP(String);

  return 0;
%End
};

bool operator == (const String& s1, const String& s2);
bool operator != (const String& s1, const String& s2);
bool operator <  (const String& s1, const String& s2);
bool operator <= (const String& s1, const String& s2);
bool operator >= (const String& s1, const String& s2);
bool operator >  (const String& s1, const String& s2);
