class Sphere3
{
%HeaderCode
	#include <BALL/MATHS/sphere3.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Sphere3)
  Sphere3();
  Sphere3(const Sphere3& /sphere3/);
  Sphere3(const Vector3& /point/, float /radius/);
  ~Sphere3();
  void swap(Sphere3& /sphere3/);
  void set(const Sphere3& /sphere/);
  void set(const Vector3& /point/, float /rhs/);
  // Sphere3& operator = (const Sphere3& /sphere3/);
  void get(Sphere3& /sphere3/) const;
  void get(Vector3& /point/, float /rhs/) const;
  // bool operator == (const Sphere3& sphere3) const;
  // bool operator != (const Sphere3& sphere3) const;
  bool has(const Vector3& /point/, bool /on_surface/ = false) const;
  bool isEmpty() const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;

  Vector3 p;
  float radius;
};
