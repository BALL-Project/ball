// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Plane3
{
%TypeHeaderCode
	#include <BALL/MATHS/plane3.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Plane3)
  Plane3();
  Plane3(const Plane3&);
  Plane3(const Vector3&, const Vector3&);
  Plane3(const Vector3&, const Vector3&, const Vector3&);
  Plane3(float, float, float, float);
	~Plane3();
  void swap(Plane3&);
  void set(const Plane3&);
  void set(const Vector3&, const Vector3&);
  void set(const Vector3&, const Vector3&, const Vector3&);
	//  Plane3& operator = (const Plane3 &/plane/);
  void get(Plane3&) const;
  void get(Vector3&, Vector3&) const;
  void normalize();
  void hessify();
  bool operator == (const Plane3&) const;
  bool operator != (const Plane3&) const;
  bool has(const Vector3&) const;
  bool has(const Line3&) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const
};
