# 1 "vector.h"
namespace BALL {
 template <class T>
 class TVector_
 {
  public:
   TVector_(const T& vx, const T& vy, const T& vz)
    : x(x),
     y(y),
     z(z),
     h((T)0)
   {
   }
   TVector_(const T& vx, const T& vy, const T& vz, const T& h)
    : x(x),
     y(y),
     z(z),
     h(h)
   {
   }
   T x;
   T y;
   T z;
   T h;
  protected:
  private:
 };
}  
