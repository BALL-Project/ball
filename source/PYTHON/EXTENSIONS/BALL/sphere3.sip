// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Sphere3
{
%TypeHeaderCode
	#include <BALL/MATHS/sphere3.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Sphere3)
  Sphere3();
  Sphere3(const Sphere3&);
  Sphere3(const Vector3&, float);
  ~Sphere3();
  void swap(Sphere3&);
  void set(const Sphere3&);
  void set(const Vector3&, float);
  // Sphere3& operator = (const Sphere3&);
  void get(Sphere3&) const;
  void get(Vector3&, float) const;
  bool operator == (const Sphere3&) const;
  bool operator != (const Sphere3&) const;
  bool has(const Vector3&, bool on_surface = false) const;
  bool isEmpty() const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;

  Vector3 p;
  float radius;
};
