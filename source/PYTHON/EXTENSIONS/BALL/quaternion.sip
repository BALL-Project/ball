// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class Quaternion
{
%TypeHeaderCode
	#include <BALL/MATHS/quaternion.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE_NODEEP(Quaternion)
  Quaternion();
  Quaternion(const Quaternion&);
};
/*
  Quaternion(const Vector3&, float);
  Quaternion(float, float, float, float);
  ~Quaternion();
  void set(const Quaternion&);
  void set(const Vector3&, float);
  void set(float, float, float, float);
  // Quaternion& operator = (const Quaternion& q);
  void get(Quaternion&) const;
  void setIdentity();
  void swap(Quaternion&);
  float getAngle() const;
  Vector3 getAxis();
  Matrix4x4& getRotationMatrix(Matrix4x4&) const;
  // ???? Quaternion operator - () const;
  Quaternion getInverse() const;
  Quaternion getConjugate() const;
  Quaternion& operator += (const Quaternion&);
  Quaternion& operator -= (const Quaternion&);
  bool operator == (const Quaternion&) const;
  bool operator != (const Quaternion&) const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  float i;
  float j;
  float k;
  float angle;
};
*/
