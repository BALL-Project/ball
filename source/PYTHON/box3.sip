class Box3
{
%HeaderCode
#include <BALL/MATHS/box3.h>
using namespace BALL;
%End
	public:
  Box3();
  Box3(Box3&);
  Box3(Vector3&, Vector3&);
  Box3
   (float, float, float,
    float, float, float);

  void set(Box3&);
  void set(Vector3&, Vector3&);
  void set
   (float, float, float,
    float, float, float);
  // Box3& operator = (Box3& box);
  void get(Box3&) const;
  void get(Vector3&, Vector3&) const;
  void get(float, float, float, float, float, float) const;
  void swap(Box3&);
  float getSurface() const;
  float getVolume() const;
  float getWidth() const;
  float getHeight() const;
  float getDepth() const;
  void join(Box3&);
  // bool operator == (Box3& box) const;
  // bool operator != (Box3& box) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  Vector3 a;
  Vector3 b;
};
