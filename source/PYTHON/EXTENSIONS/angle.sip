// $Id: angle.sip,v 1.2 2001/12/22 16:39:14 oliver Exp $

class Angle
{
%HeaderCode
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
  Angle(const Angle& /angle/);
  Angle(float /new_value/, bool /radian/ = true);
  ~Angle();
  void swap(Angle& /angle/);
  void set(float /new_value/, bool /radian/ = true);
  void set(const Angle& /angle/);
  // const Angle& operator = (const Angle& angle);
  // const Angle& operator = (const T& new_value);
  // void get(Angle& angle) const;
  // void get(T& val, bool radian = true) const;
  // operator T () const;
  float toRadian() const;
  // static float toRadian(const T& degree);
  float toDegree() const;
  // static T toDegree(const T& radian);
  void normalize(Range /range/);
  void negate();
  Angle operator + () const;
  Angle operator - () const;
  Angle& operator += (const Angle& /angle/);
  Angle& operator += (float /val/);
  Angle operator + (const Angle& /angle/);
  Angle& operator -= (const Angle& /angle/);
  Angle& operator -= (float /val/);
  Angle operator - (const Angle& /angle/);
  Angle& operator *= (const Angle& /angle/);
  Angle& operator *= (float /val/);
  Angle& operator /= (const Angle& /angle/);
  Angle& operator /= (float /val/);
  Angle operator / (const Angle& /val/);
  bool operator == (const Angle& /angle/) const;
  // bool operator != (const Angle& /angle/) const;
  // bool operator < (const Angle& /angle/) const;
  // bool operator < (float /val/) const;
  // bool operator <= (const Angle& /angle/) const;
  // bool operator >= (const Angle& /angle/) const;
  // bool operator > (const Angle& /angle/) const;
  bool isEquivalent(Angle /angle/) const;
  bool isValid () const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
	PyMethod	__str__
%MemberCode
  Angle* ptr;
  if ((ptr = (Angle*)sipGetCppPtr((sipThisType*)a0 ,sipClass_Angle)) == NULL)
    return NULL;

  return PyString_FromString(String(ptr->toRadian()).c_str());
%End
};
