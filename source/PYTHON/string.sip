class String
{
%HeaderCode
#include <BALL/DATATYPE/string.h>
using namespace BALL;
%End

	public:
	String();
  String(const char*);
  void destroy();
  void swap(String&);
  String& reverse(int, int);
  bool has(char) const;
  bool hasSubstring(const String&, int = 0) const;
  bool hasPrefix(const String&) const;
  bool hasSuffix(const String&) const;
  bool isEmpty() const;
  bool isAlpha() const;
  bool isAlnum() const;
  bool isDigit() const;
  bool isSpace() const;
  bool isWhitespace() const;
	const char* c_str() const;
	int size() const;

	operator const char* () const;
%CanConvertToClassCode
	// automatic conversion of Py-Strings to Strings
	return (PyString_Check(sipPy) || sipIsSubClassInstance(sipPy,sipClass_String));
%End
%ConvertToClassCode
  if (PyString_Check(sipPy))
  {
    *sipCppPtr = new String(PyString_AS_STRING(sipPy));

    return 1;
  }

  *sipCppPtr = (String*)sipConvertToCpp(sipPy,sipClass_String,sipIsErr);

  return 0;
%End
%PythonCode
		def __cmp__(self, other):
			return cmp(str(self), str(other))
%End
};
