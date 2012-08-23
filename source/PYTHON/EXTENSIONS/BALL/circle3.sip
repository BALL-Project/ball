// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Circle3
{
%TypeHeaderCode
	#include <BALL/MATHS/circle3.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Circle3)
  Circle3();
  Circle3(const Circle3&);
  Circle3(const Vector3&, const Vector3&, float);
  ~Circle3();
  void swap(Circle3&);
  void set(const Circle3&);
  void set(const Vector3&, const Vector3&, float);
  // Circle3& operator = (const Circle3&);
  void get(Circle3&) const;
  void get(Vector3&, Vector3&, float) const;
  bool operator == (const Circle3&) const;
  bool operator != (const Circle3&) const;
  bool has(const Vector3&, bool on_surface = false) const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
};
