class Angle
{
%HeaderCode
#include <BALL/MATHS/angle.h>
using namespace BALL;
%End
  public:
  Angle();
  Angle(Angle&, bool  = true);
  Angle(float, bool = true);
  void swap(Angle&);
  void set(Angle&, bool   = true);
  void set(float, bool = true);
//  void get(Angle&, bool = true) const;
//  void get(float& , bool = true) const;
//  void setNull();
//  static Angle getNull();
  float toRadian() const;
//  static float toRadian(float);
  float toDegree() const;
//  static float toDegree(float);
//  static Angle getTorsionTAngle
//   (float, float, float,
//    float, float, float,
//    float, float, float, 
//    float, float, float);
//  void normalize(Range);
  void negate();
  bool isEquivalent(Angle) const;
  bool isValid () const;
 };
