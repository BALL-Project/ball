// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ColorRGBA
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorRGBA.h>
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	#include <BALL/COMMON/exception.h>

	using namespace BALL;
	using namespace BALL::VIEW;
	using namespace BALL::Exception;
	typedef ColorUnit::NotInHexFormat NotInHexFormat;
%End
  public:
  ColorRGBA() throw();
  ColorRGBA(const ColorRGBA& /color/) throw();
  // ColorRGBA(const ColorHSV& /color/) throw();
  ColorRGBA(const char* /value/) throw(InvalidRange, NotInHexFormat);
  ColorRGBA(const String& /value/) throw(InvalidRange, NotInHexFormat);
	//  ColorRGBA(const ColorUnit& /red/, const ColorUnit& /green/, const ColorUnit& /blue/, const ColorUnit& /alpha/ = 1.0) throw();
  ColorRGBA(float /red/, float /green/, float /blue/, float /alpha/ = 1.0) throw();
	//ColorRGBA(const QColor&, const ColorUnit& = 1.0) throw();
  //virtual ~ColorRGBA() throw();
  void clear() throw();
  // operator String() const throw();
  void set(const ColorRGBA& /color/) throw();
  // const ColorRGBA& operator = (const ColorRGBA& /color/) throw();
  // void set(const ColorHSV& /color/) throw();
  // const ColorRGBA& operator = (const ColorHSV& /color/) throw();
  // void get(ColorHSV& /color/) const throw();
  void set(const char* /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorRGBA& operator = (const char* /value/) throw(InvalidRange, NotInHexFormat);
  void get(char* /value/) const throw();
  void set(const String& /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorRGBA& operator = (const String& /value/) throw(InvalidRange, NotInHexFormat);
  void get(String& /value/) const throw();
	//void get(QColor&) const throw();
	//bool set(const QColor& color) throw();
	//QColor getQColor() const throw();
  void swap(ColorRGBA& /color/) throw();
  void setRed(const ColorUnit& /red_value/) throw();
  ColorUnit& getRed() throw();
  // const ColorUnit& getRed() const throw();

  void setGreen(const ColorUnit& /green_value/) throw();
  ColorUnit& getGreen() throw();
  // const ColorUnit& getGreen() const throw();

  void setBlue(const ColorUnit& /blue_value/) throw();
  ColorUnit& getBlue() throw();
  // const ColorUnit& getBlue() const throw();

  void setAlpha(const ColorUnit& /alpha_value/) throw();
  ColorUnit& getAlpha() throw();
  // const ColorUnit& getAlpha() const throw();

  void set(const ColorUnit& /red_value/, const ColorUnit& /green_value/, const ColorUnit& /blue_value/, const ColorUnit& /alpha_value/ = 1.0) throw();
  void get(ColorUnit& /red_value/, ColorUnit& /green_value/, ColorUnit& /blue_value/, ColorUnit& /alpha_value/) const throw();
  void get(ColorUnit& /red_value/, ColorUnit& /green_value/, ColorUnit& /blue_value/) const throw();
  bool operator == (const ColorRGBA& /color/) const throw();
  bool operator !=  (const ColorRGBA& /color/) const throw();
  bool operator < (const ColorRGBA& /color/) const throw();
  bool operator <= (const ColorRGBA& /color/) const throw();
  bool operator > (const ColorRGBA& /color/) const throw();
  bool operator >= (const ColorRGBA& /color/) const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
	// std::istream& operator >> (std::istream& s, ColorRGBA& color);
  //  std::ostream& operator << (std::ostream& s, const ColorRGBA& /color/);
	SIP_PYOBJECT __str__();
%MethodCode
	String s("ColorRGBA {");
	s += (String) *sipCpp;
	s += " }";
	sipRes = PyString_FromString(s.c_str());
%End

	SIP_PYOBJECT __repr__();
%MethodCode
	String s("ColorRGBA {");
	s += (String) *sipCpp;
	s += " }";
	sipRes = PyString_FromString(s.c_str());
%End

};
