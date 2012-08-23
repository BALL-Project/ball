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
	NotInHexFormat(const char*, int, const String&) throw();
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
  ColorUnit(const ColorUnit&) throw();
  ColorUnit(const String&) throw(InvalidRange, NotInHexFormat);
  // ColorUnit(const char*) throw(InvalidRange, NotInHexFormat);
  // ColorUnit(const unsigned char) throw();
  // ColorUnit(const short) throw(InvalidRange);
  // ColorUnit(const unsigned short) throw(InvalidRange);
  // ColorUnit(const int) throw(InvalidRange);
  ColorUnit(const unsigned int) throw(InvalidRange);
  // Colorunit(const long) throw(InvalidRange);
  // ColorUnit(const unsigned long) throw(InvalidRange);
  // ColorUnit(const float) throw(InvalidRange);
  // ColorUnit(const double) throw(InvalidRange);
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
  // void set(const ColorUnit&) throw();
  // const ColorUnit& operator = (const ColorUnit&) throw();
  // void swap(ColorUnit&) throw();
  // void set(const char*) throw(InvalidRange, NotInHexFormat);
  // const ColorUnit& operator = (const char*) throw(InvalidRange, NotInHexFormat);
  // void get(char*) const throw();
  // void set(const String&) throw(InvalidRange, NotInHexFormat);
  // const ColorUnit& operator = (const String&) throw(InvalidRange, NotInHexFormat);
  // void get(String&) const throw();
  // void set(const unsigned char) throw();
  // const ColorUnit& operator = (const unsigned char) throw();
  // void get(unsigned char&) const throw();
  // void set(const short) throw(InvalidRange);
  // const ColorUnit& operator = (const short) throw(InvalidRange);
  // void get(short&) const throw();
  // void set(const unsigned short) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned short) throw(InvalidRange);
  // void get(unsigned short&) const throw();
  // void set(const int) throw(InvalidRange);
  // const ColorUnit& operator = (const int) throw(InvalidRange);
  // void get(int&) const throw();
  void set(const unsigned int) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned int) throw(InvalidRange);
  void get(unsigned int&) const throw();
  // void set(const long) throw(InvalidRange);
  // const ColorUnit& operator = (const long) throw(InvalidRange);
  // void get(long&) const throw();
  // void set(const unsigned long) throw(InvalidRange);
  // const ColorUnit& operator = (const unsigned long) throw(InvalidRange);
  // void get(unsigned long&) const throw();
  // void set(const float) throw(InvalidRange);
  // const ColorUnit& operator = (const float) throw(InvalidRange);
  // void get(float&) const throw();
  // void set(const double) throw(InvalidRange);
  // const ColorUnit& operator = (const double) throw(InvalidRange);
  // void get(double&) const throw();
  
	
  bool operator == (const ColorUnit&) const throw();
	// bool operator != (const ColorUnit&) const throw();
  // bool operator < (const ColorUnit&) const throw();
  bool operator <= (const ColorUnit&) const throw();
  bool operator > (const ColorUnit&) const throw();
  bool operator >= (const ColorUnit&) const throw();
  // void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  // friend std::istream& operator >> (std::istream& s, ColorUnit&);
  // friend std::ostream& operator << (std::ostream& s, const ColorUnit&);
};
