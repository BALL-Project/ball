// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Angle
{
%TypeHeaderCode
	#include <BALL/DATATYPE/string.h>
	#include <BALL/MATHS/angle.h>
	using namespace BALL;
%End
  public:
  //BALL_CREATE_NODEEP(Angle)
  enum Range
  {
   RANGE__UNLIMITED = 0, 
   RANGE__UNSIGNED  = 1, 
   RANGE__SIGNED    = 2 
  };
  Angle();
  Angle(const Angle&);
  Angle(float, bool radian = true);
  ~Angle();
  void swap(Angle&);
  void set(float, bool radian = true);
  void set(const Angle&);
  // const Angle& operator = (const Angle& angle);
  // const Angle& operator = (const T& new_value);
  // void get(Angle& angle) const;
  // void get(T& val, bool radian = true) const;
  // operator T () const;
  float toRadian() const;
  // static float toRadian(const T& degree);
  float toDegree() const;
  // static T toDegree(const T& radian);
  void normalize(Range);
  void negate();
  // ???? Angle operator + () const;
  // ???? Angle operator - () const;
  Angle& operator += (const Angle&);
  Angle& operator += (float);
  Angle operator + (const Angle&);
  Angle& operator -= (const Angle&);
  Angle& operator -= (float);
  Angle operator - (const Angle&);
  Angle& operator *= (const Angle&);
  Angle& operator *= (float);
  Angle& operator /= (const Angle&);
  Angle& operator /= (float);
  Angle operator / (const Angle&);
  bool operator == (const Angle&) const;
  bool operator != (const Angle&) const;
  bool operator < (const Angle&) const;
  bool operator < (float) const;
  bool operator <= (const Angle&) const;
  bool operator >= (const Angle&) const;
  bool operator > (const Angle&) const;
  bool isEquivalent(Angle) const;
  bool isValid () const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;

  SIP_PYOBJECT __str__();
%MethodCode
	sipRes = PyString_FromString(String(sipCpp->toRadian()).c_str());
%End
};
