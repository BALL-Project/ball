
// $Id: plane3.sip,v 1.3 2001/12/27 11:47:18 oliver Exp $
class Plane3
{
%HeaderCode
	#include <BALL/MATHS/plane3.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Plane3)
  Plane3();
  Plane3(const Plane3& /plane/);
  Plane3(const Vector3& /point/, const Vector3& /normal/);
  Plane3(const Vector3& /a/, const Vector3& /b/, const Vector3& /c/);
  Plane3(float /a/, float /b/, float /c/, float /d/);
	~Plane3();
  void swap(Plane3& /plane/);
  void set(const Plane3& /plane/);
  void set(const Vector3& /point/, const Vector3& /normal/);
  void set(const Vector3& /a/, const Vector3& /b/, const Vector3& /c/);
	//  Plane3& operator = (const Plane3 &/plane/);
  void get(Plane3& /plane/) const;
  void get(Vector3& /point/, Vector3& /normal/) const;
  void normalize();
  void hessify();
  bool operator == (const Plane3& /plane/) const;
  bool operator != (const Plane3& /plane/) const;
  bool has(const Vector3& /point/) const;
  bool has(const Line3& /line/) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const
};
