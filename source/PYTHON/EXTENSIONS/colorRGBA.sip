// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorRGBA.sip,v 1.1 2003/11/15 10:59:05 oliver Exp $
//

class ColorRGBA
{
%TypeHeaderCode
	#include <BALL/.h>
	using namespace BALL;
%End
  public:
  ColorRGBA() throw();
  // ColorRGBA(const ColorHSV& /color/) throw();
  ColorRGBA(const ColorRGBA& /color/) throw();
  ColorRGBA(const ColorUnit& /red_value/, const ColorUnit& /green_value/, const ColorUnit& /blue_value/, const ColorUnit& /alpha_value/ = 1.0) throw();
  ColorRGBA(const String& /value/) throw(InvalidRange, NotInHexFormat);
  ColorRGBA(const char* /value/) throw(InvalidRange, NotInHexFormat);
  ColorUnit& getAlpha() throw();
  ColorUnit& getBlue() throw();
  ColorUnit& getGreen() throw();
  ColorUnit& getRed() throw();
  bool operator !=  (const ColorRGBA& /color/) const throw();
  bool operator < (const ColorRGBA& /color/) const throw();
  bool operator <= (const ColorRGBA& /color/) const throw();
  bool operator == (const ColorRGBA& /color/) const throw();
  bool operator > (const ColorRGBA& /color/) const throw();
  bool operator >= (const ColorRGBA& /color/) const throw();
  // const ColorRGBA& operator = (const ColorHSV& /color/) throw();
  // const ColorRGBA& operator = (const ColorRGBA& /color/) throw();
  // const ColorRGBA& operator = (const String& /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorRGBA& operator = (const char* /value/) throw(InvalidRange, NotInHexFormat);
  const ColorUnit& getAlpha() const throw();
  const ColorUnit& getBlue() const throw();
  const ColorUnit& getGreen() const throw();
  const ColorUnit& getRed() const throw();
  // friend std::istream& operator >> (std::istream& s, ColorRGBA& /color/);
  // friend std::ostream& operator << (std::ostream& s, const ColorRGBA& /color/);
  // operator String() const throw();
  void clear() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  // void get(ColorHSV& /color/) const throw();
  void get(ColorUnit& /red_value/, ColorUnit& /green_value/, ColorUnit& /blue_value/) const throw();
  void get(ColorUnit& /red_value/, ColorUnit& /green_value/, ColorUnit& /blue_value/, ColorUnit& /alpha_value/) const throw();
  void get(String& /value/) const throw();
  void get(char* /value/) const throw();
  // void set(const ColorHSV& /color/) throw();
  void set(const ColorRGBA& /color/) throw();
  void set(const ColorUnit& /red_value/, const ColorUnit& /green_value/, const ColorUnit& /blue_value/, const ColorUnit& /alpha_value/ = 1.0) throw();
  void set(const String& /value/) throw(InvalidRange, NotInHexFormat);
  void set(const char* /value/) throw(InvalidRange, NotInHexFormat);
  void setAlpha(const ColorUnit& /alpha_value/) throw();
  void setBlue(const ColorUnit& /blue_value/) throw();
  void setGreen(const ColorUnit& /green_value/) throw();
  void setRed(const ColorUnit& /red_value/) throw();
  void swap(ColorRGBA& /color/) throw();
  ~ColorRGBA() throw();
};
