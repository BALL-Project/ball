// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: colorUnit.sip,v 1.1 2003/11/15 10:59:05 oliver Exp $
//

class NotInHexFormat
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:
	NotInHexFormat(const char* /file/, int /line/, const String& /data/) throw();
};

class ColorUnit
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  ColorUnit() throw();
  ColorUnit(const ColorUnit& /color_unit/) throw();
  ColorUnit(const String& /value/) throw(InvalidRange, NotInHexFormat);
  ColorUnit(const char* /value/) throw(InvalidRange, NotInHexFormat);
  ColorUnit(const double /value/) throw(InvalidRange);
  // ColorUnit(const float /value/) throw(InvalidRange);
  // ColorUnit(const int /value/) throw(InvalidRange);
  // Colorunit(const long /value/) throw(InvalidRange);
  // ColorUnit(const short /value/) throw(InvalidRange);
  // ColorUnit(const unsigned char /value/) throw();
  // ColorUnit(const unsigned int /value/) throw(InvalidRange);
  // ColorUnit(const unsigned long /value/) throw(InvalidRange);
  // ColorUnit(const unsigned short /value/) throw(InvalidRange);
  // bool operator != (const ColorUnit& /color_unit/) const throw();
  // bool operator < (const ColorUnit& /color_unit/) const throw();
  bool operator <= (const ColorUnit& /color_unit/) const throw();
  bool operator == (const ColorUnit& /color_unit/) const throw();
  bool operator > (const ColorUnit& /color_unit/) const throw();
  bool operator >= (const ColorUnit& /color_unit/) const throw();
  // const ColorUnit& operator = (const ColorUnit& /color_unit/) throw();
  // const ColorUnit& operator = (const String& /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorUnit& operator = (const char* /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorUnit& operator = (const double /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const float /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const int /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const long double /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const long /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const short /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned char /value/) throw();
  // const ColorUnit& operator = (const unsigned int /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned long /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned short /value/) throw(InvalidRange);
  // friend std::istream& operator >> (std::istream& s, ColorUnit& /color_unit/);
  // friend std::ostream& operator << (std::ostream& s, const ColorUnit& /color_unit/);
  // operator String () const throw();
  // operator char () const throw();
  // operator double () const throw();
  // operator float () const throw();
  // operator int () const throw();
  // operator long () const throw();
  // operator long double () const throw();
  // operator short () const throw();
  // operator unsigned char () const throw();
  // operator unsigned int () const throw();
  // operator unsigned long () const throw();
  // operator unsigned short () const throw();
  void clear() throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  // void get(String& /value/) const throw();
  // void get(char* /value/) const throw();
  // void get(double& /value/) const throw();
  // void get(float& /value/) const throw();
  // void get(int& /value/) const throw();
  // void get(long double& /value/) const throw();
  // void get(long& /value/) const throw();
  // void get(short& /value/) const throw();
  // void get(unsigned char& /value/) const throw();
  // void get(unsigned int& /value/) const throw();
  // void get(unsigned long& /value/) const throw();
  // void get(unsigned short& /value/) const throw();
  // void set(const ColorUnit& /color_unit/) throw();
  // void set(const String& /value/) throw(InvalidRange, NotInHexFormat);
  // void set(const char* /value/) throw(InvalidRange, NotInHexFormat);
  void set(const double /value/) throw(InvalidRange);
  // void set(const float /value/) throw(InvalidRange);
  // void set(const int /value/) throw(InvalidRange);
  // void set(const long double /value/) throw(InvalidRange);
  // void set(const long /value/) throw(InvalidRange);
  // void set(const short /value/) throw(InvalidRange);
  // void set(const unsigned char /value/) throw();
  // void set(const unsigned int /value/) throw(InvalidRange);
  // void set(const unsigned long /value/) throw(InvalidRange);
  // void set(const unsigned short /value/) throw(InvalidRange);
  // void swap(ColorUnit& /color_unit/) throw();
  ~ColorUnit() throw();
};
