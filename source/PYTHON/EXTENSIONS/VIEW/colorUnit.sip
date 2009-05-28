// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class NotInHexFormat
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL::Exception;
	using namespace BALL;
	using namespace BALL::VIEW;
	typedef ColorUnit::NotInHexFormat NotInHexFormat;
%End
	public:
	NotInHexFormat(const char* /file/, int /line/, const String& /data/) throw();
};

class ColorUnit
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/colorUnit.h>
	#include <BALL/COMMON/exception.h>
	using namespace BALL::Exception;
	using namespace BALL;
	using namespace BALL::VIEW;
	typedef ColorUnit::NotInHexFormat NotInHexFormat;
%End
  public:
  ColorUnit() throw();
  ColorUnit(const ColorUnit& /color_unit/) throw();
  ColorUnit(const String& /value/) throw(InvalidRange, NotInHexFormat);
  // ColorUnit(const char* /value/) throw(InvalidRange, NotInHexFormat);
  // ColorUnit(const unsigned char /value/) throw();
  // ColorUnit(const short /value/) throw(InvalidRange);
  // ColorUnit(const unsigned short /value/) throw(InvalidRange);
  // ColorUnit(const int /value/) throw(InvalidRange);
  ColorUnit(const unsigned int /value/) throw(InvalidRange);
  // Colorunit(const long /value/) throw(InvalidRange);
  // ColorUnit(const unsigned long /value/) throw(InvalidRange);
  // ColorUnit(const float /value/) throw(InvalidRange);
  // ColorUnit(const double /value/) throw(InvalidRange);
  //virtual ~ColorUnit() throw();
  void clear() throw();
  // operator String () const throw();
  // operator char () const throw();
  // operator unsigned char () const throw();
  // operator short () const throw();
  // operator unsigned short () const throw();
  // operator int () const throw();
  // operator unsigned int () const throw();
  // operator long () const throw();
  // operator unsigned long () const throw();
  // operator float () const throw();
  // operator double () const throw();
  // void set(const ColorUnit& /color_unit/) throw();
  // const ColorUnit& operator = (const ColorUnit& /color_unit/) throw();
  // void swap(ColorUnit& /color_unit/) throw();
  // void set(const char* /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorUnit& operator = (const char* /value/) throw(InvalidRange, NotInHexFormat);
  // void get(char* /value/) const throw();
  // void set(const String& /value/) throw(InvalidRange, NotInHexFormat);
  // const ColorUnit& operator = (const String& /value/) throw(InvalidRange, NotInHexFormat);
  // void get(String& /value/) const throw();
  // void set(const unsigned char /value/) throw();
  // const ColorUnit& operator = (const unsigned char /value/) throw();
  // void get(unsigned char& /value/) const throw();
  // void set(const short /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const short /value/) throw(InvalidRange);
  // void get(short& /value/) const throw();
  // void set(const unsigned short /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned short /value/) throw(InvalidRange);
  // void get(unsigned short& /value/) const throw();
  // void set(const int /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const int /value/) throw(InvalidRange);
  // void get(int& /value/) const throw();
  void set(const unsigned int /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned int /value/) throw(InvalidRange);
  void get(unsigned int& /value/) const throw();
  // void set(const long /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const long /value/) throw(InvalidRange);
  // void get(long& /value/) const throw();
  // void set(const unsigned long /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned long /value/) throw(InvalidRange);
  // void get(unsigned long& /value/) const throw();
  // void set(const float /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const float /value/) throw(InvalidRange);
  // void get(float& /value/) const throw();
  // void set(const double /value/) throw(InvalidRange);
  // const ColorUnit& operator = (const double /value/) throw(InvalidRange);
  // void get(double& /value/) const throw();
  
	
  bool operator == (const ColorUnit& /color_unit/) const throw();
	// bool operator != (const ColorUnit& /color_unit/) const throw();
  // bool operator < (const ColorUnit& /color_unit/) const throw();
  bool operator <= (const ColorUnit& /color_unit/) const throw();
  bool operator > (const ColorUnit& /color_unit/) const throw();
  bool operator >= (const ColorUnit& /color_unit/) const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  // friend std::istream& operator >> (std::istream& s, ColorUnit& /color_unit/);
  // friend std::ostream& operator << (std::ostream& s, const ColorUnit& /color_unit/);
};
