// $Id: string.sip,v 1.4 2001/12/27 11:47:18 oliver Exp $

class Substring
{
%HeaderCode
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
  Substring(const Substring& /substring/, bool /deep/ = true);
  Substring(const String& /string/, Index /from/ = 0, Size /len/ = String::EndPos);
  ~Substring();
  void destroy();
  // operator String() const;
  String toString() const;
  Substring& bind(const String& /string/, Index /from/ = 0, Size /len/ = String::EndPos);
  Substring& bind(const Substring& /substring/, Index /from/ = 0, Size /len/ = String::EndPos);
  void unbind();
  String* getBoundString();
  const String* getBoundString() const;
  void set(const String& /string/);
  void set(const Substring& /s/);
  void set(const char* /char_ptr/, Size /size/ = String::EndPos);
  // Substring& operator = (const String& string);
  // Substring& operator = (const Substring& substring);
  // Substring& operator = (const char* char_ptr);
  char* c_str();
  const char* c_str() const;
  Index getFirstIndex() const;
  Index getLastIndex() const;
  Size size() const;
  // char& operator [] (Index /index/);
  char operator [] (Index /index/) const;
  Substring& toLower();
  Substring& toUpper();
  bool isBound() const;
  bool isEmpty() const;
  bool operator == (const Substring& /substring/) const;
  bool operator != (const Substring& /substring/) const;
  bool operator == (const String& /string/) const;
  bool operator != (const String& /string/) const;
  // friend bool operator == (const String& string, const Substring& substring);
  // friend bool operator != (const String& string, const Substring& substring);
  bool operator == (const char* /char_ptr/) const;
  bool operator != (const char* /char_ptr/) const;
  bool operator == (char /c/) const;
  bool operator != (char /c/) const;
  // friend ::std::ostream& operator << (::std::ostream& s, const Substring& substring);
  bool isValid() const;
  // void dump(::std::ostream& s = ::std::cout, Size depth = 0) const;
};


class String
// : public string
{
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

  String(const String& /string/);
  // String(const string& string);

  String(const String& /s/, Index /from/, Size /len/ = String::EndPos)
		throw(NullPointer, IndexOverflow, IndexUnderflow);

	String(const char* /char_ptr/)
		throw(NullPointer, IndexOverflow, IndexUnderflow);
  String(const char* /char_ptr/, Index /from/) 
		throw(NullPointer, IndexOverflow, IndexUnderflow);
  String(const char* /char_ptr/, Index /from/, Size /len/) 
		throw(NullPointer, IndexOverflow, IndexUnderflow);

  // String(Size buffer_size, const char* format, ... );
  // String(std::strstream& s);
  // String(char /c/, Size /size/ = 1);
  // String(const unsigned char uc);
  //String(short /s/);
  // String(unsigned short us);
  //String(int /i/);
  // String(unsigned int ui);
  //String(long /l/);
  // String(unsigned long );
  //String(float /f/);
  String(double /d/);
  ~String();
  void destroy();
  void set(const String& /string/, Index /from/ = 0, Size /len/ = String::EndPos);
  void set(const char* /char_ptr/, Index /from/ = 0, Size /len/ = String::EndPos);
  // void set(Size /buffer_size/, const char *format, ...);
  // void set(std::strstream& s);
  // void set(char /c/, Size /len/ = 1);
  // void set(unsigned char uc);
  //void set(short /s/);
  // void set(unsigned short us);
  //void set(int /i/);
  // void set(unsigned int ui);
  void set(long /l/);
  // void set(unsigned long ul);
  //void set(float /f/);
  //void set(double /d/);
  // void get(char* char_ptr, Index from = 0, Size len = String::EndPos) const;
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
  static void setCompareMode(CompareMode /compare_mode/);
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
  void toLower(Index /from/ = 0, Size /len/ = String::EndPos);
  void toUpper(Index /from/ = 0, Size /len/ = String::EndPos);
  Substring getSubstring(Index /from/ = 0, Size /len/ = String::EndPos) const; 
  Substring operator () (Index /from/, Size /len/ = String::EndPos) const; 
  Substring before(const String& /s/, Index /from/ = 0) const;
  Substring through(const String& /s/, Index /from/ = 0) const;
  Substring from(const String& /s/, Index /from/ = 0) const;
  Substring after(const String& /s/, Index /from/ = 0) const;
  Size countFields(const char* /delimiters/ = String::CHARACTER_CLASS__WHITESPACE) const;
  String getField(Index /index/, const char* /delimiters/ = String::CHARACTER_CLASS__WHITESPACE) const;
  // Size split(String string_array[], Size array_size, const char* delimiters = CHARACTER_CLASS__WHITESPACE, Index from = 0) const;
  String& trimLeft(const char* /trimmed/ = String::CHARACTER_CLASS__WHITESPACE);
  String& trimRight(const char* /trimmed_chars/ = String::CHARACTER_CLASS__WHITESPACE);
  String& trim(const char* /trimmed_chars/ = String::CHARACTER_CLASS__WHITESPACE);
  String& truncate(Size /size/);
  Substring left(Size /len/) const;
  Substring right(Size /len/) const;
  Substring instr(const String& /pattern/, Index /from/ = 0) const;
  // String operator + (const string& /string/) const;
  String operator + (const char* /char_ptr/) const;
  String operator + (char /c/) const;
  // friend String operator + (const char* char_ptr, const String& s);
  // friend String operator + (char c, const String& s);
  void swap(String& /s/);
  String& reverse(Index /from/ = 0, Size /len/ = String::EndPos);
  Index substitute(const String& /to_replace/, const String& /replacing/);
  bool has(char /c/) const;
  bool hasSubstring(const String& /s/, Index /from/ = 0) const;
  bool hasPrefix(const String& /s/) const;
  bool hasSuffix(const String& /s/) const;
  bool isEmpty() const;
  bool isAlpha() const;
  bool isAlnum() const;
  bool isDigit() const;
  bool isSpace() const;
  bool isWhitespace() const;
  bool isAlpha(char /c/);
  bool isAlnum(char /c/);
  bool isDigit(char /c/);
  bool isSpace(char /c/);
  bool isWhitespace(char /c/);
  int compare(const String& /string/, Index /from/ = 0) const;
  int compare(const String& /string/, Index /from/, Size /len/) const;
  int compare(const char* /char_ptr/, Index /from/ = 0) const;
  int compare(const char* /char_ptr/, Index /from/, Size /len/) const;
  int compare(char /c/, Index /from/ = 0) const;
  bool operator == (const String& /string/) const;
  bool operator != (const String& /string/) const;
  bool operator < (const String& /string/) const;
  bool operator <= (const String& /string/) const;
  bool operator >= (const String& /string/) const;
  bool operator > (const String& /string/) const;
  // friend bool operator == (const char* char_ptr, const String& /string/);
  // friend bool operator != (const char* char_ptr, const String& /string/);
  // friend bool operator < (const char* char_ptr, const String& /string/);
  // friend bool operator <= (const char* char_ptr, const String& /string/);
  // friend bool operator > (const char* char_ptr, const String& /string/);
  // friend bool operator >= (const char* char_ptr, const String& /string/);
  bool operator == (const char* /char_ptr/) const;
  bool operator != (const char* /char_ptr/) const;
  bool operator < (const char* /char_ptr/) const;
  bool operator <= (const char* /char_ptr/) const;
  bool operator > (const char* /char_ptr/) const;
  bool operator >= (const char* /char_ptr/) const;
  // friend bool operator == (char c, const String& /string/);
  // friend bool operator != (char c, const String& /string/);
  // friend bool operator < (char c, const String& /string/);
  // friend bool operator <= (char c, const String& /string/);
  // friend bool operator > (char c, const String& /string/);
  // friend bool operator >= (char c, const String& /string/);
  bool operator == (char /c/) const;
  bool operator != (char /c/) const;
  bool operator < (char /c/) const;
  bool operator <= (char /c/) const;
  bool operator > (char /c/) const;
  bool operator >= (char /c/) const;
  bool isValid() const;
  // void dump(::std::ostream& s = ::std::cout, Size depth = 0) const;
  // ::std::istream& getline(::std::istream& s = ::std::cin, char delimiter = '\n');
  // friend ::std::istream& getline(::std::istream& s,  String& string,  char delimiter = '\n');
	int size() const;

	PyMethod __str__
%MemberCode
  const char *s;
  String *ptr;

  if ((ptr = (String *)sipGetCppPtr((sipThisType*)a0,sipClass_String)) == NULL)
    return NULL;

  if ((s = ptr->c_str()) == NULL)
  {
    Py_INCREF(Py_None);
    return Py_None;
  }

  return PyString_FromString(s);
%End

 
%ConvertToTypeCode
  if (PyString_Check(sipPy))
  {
    *sipCppPtr = new String(PyString_AS_STRING(sipPy));

    return 1;
  }

  *sipCppPtr = (String*)sipConvertToCpp(sipPy,sipClass_String,sipIsErr);

  return 0;
%End
};
