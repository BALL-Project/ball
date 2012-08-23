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
  ColorRGBA(const ColorRGBA&) throw();
  // ColorRGBA(const ColorHSV&) throw();
  ColorRGBA(const char*) throw(InvalidRange, NotInHexFormat);
  ColorRGBA(const String&) throw(InvalidRange, NotInHexFormat);
	//  ColorRGBA(const ColorUnit&, const ColorUnit&, const ColorUnit&, const ColorUnit& alpha = 1.0) throw();
  ColorRGBA(float, float, float, float alpha = 1.0) throw();
	//ColorRGBA(const QColor&, const ColorUnit& = 1.0) throw();
  //virtual ~ColorRGBA() throw();
  void clear() throw();
  // operator String() const throw();
  void set(const ColorRGBA&) throw();
  // const ColorRGBA& operator = (const ColorRGBA&) throw();
  // void set(const ColorHSV&) throw();
  // const ColorRGBA& operator = (const ColorHSV&) throw();
  // void get(ColorHSV&) const throw();
  void set(const char*) throw(InvalidRange, NotInHexFormat);
  // const ColorRGBA& operator = (const char*) throw(InvalidRange, NotInHexFormat);
  void get(char*) const throw();
  void set(const String&) throw(InvalidRange, NotInHexFormat);
  // const ColorRGBA& operator = (const String&) throw(InvalidRange, NotInHexFormat);
  void get(String&) const throw();
	//void get(QColor&) const throw();
	//bool set(const QColor& color) throw();
	//QColor getQColor() const throw();
  void swap(ColorRGBA&) throw();
  void setRed(const ColorUnit&) throw();
  ColorUnit& getRed() throw();
  // const ColorUnit& getRed() const throw();

  void setGreen(const ColorUnit&) throw();
  ColorUnit& getGreen() throw();
  // const ColorUnit& getGreen() const throw();

  void setBlue(const ColorUnit&) throw();
  ColorUnit& getBlue() throw();
  // const ColorUnit& getBlue() const throw();

  void setAlpha(const ColorUnit&) throw();
  ColorUnit& getAlpha() throw();
  // const ColorUnit& getAlpha() const throw();

  void set(const ColorUnit&, const ColorUnit&, const ColorUnit&, const ColorUnit& alpha_value = 1.0) throw();
  void get(ColorUnit&, ColorUnit&, ColorUnit&, ColorUnit&) const throw();
  void get(ColorUnit&, ColorUnit&, ColorUnit&) const throw();
  bool operator == (const ColorRGBA&) const throw();
  bool operator !=  (const ColorRGBA&) const throw();
  bool operator < (const ColorRGBA&) const throw();
  bool operator <= (const ColorRGBA&) const throw();
  bool operator > (const ColorRGBA&) const throw();
  bool operator >= (const ColorRGBA&) const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
	// std::istream& operator >> (std::istream& s, ColorRGBA& color);
  //  std::ostream& operator << (std::ostream& s, const ColorRGBA&);
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
