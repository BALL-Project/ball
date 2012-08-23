// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

%ModuleCode
	#include <BALL/MATHS/analyticalGeometry.h>
	using namespace BALL;
%End

// float GetDeterminant_(const T* m, Size dim);
// float GetDeterminant(const T* m, Size dim);
// float GetDeterminant2(const T* m);
// float GetDeterminant2(float m00, float m01, float m10, float m11);
// float GetDeterminant3(const T *m);
// float GetDeterminant3
//   (float m00, float m01, float m02,
//    float m10, float m11, float m12,
//   float m20, float m21, float m22);
// bool SolveSystem(const T* m, T* x, const Size dim);
// bool SolveSystem2
// short SolveQuadraticEquation
//  (float, float b, const float c, float& x1, float x2);
// Vector3 GetPartition(const Vector3&, const Vector3& b);
// Vector3 GetPartition(const Vector3&, const Vector3& b, float r, float s);
float GetDistance(const Vector3&, const Vector3&);
float GetDistance(const Line3&, const Vector3&);
float GetDistance(const Vector3&, const Line3&);
float GetDistance(const Line3&, const Line3&);
float GetDistance(const Vector3&, const Plane3&);
float GetDistance(const Plane3&, const Vector3&);
float GetDistance(const Line3&, const Plane3&);
float GetDistance(const Plane3&, const Line3&);
float GetDistance(const Plane3&, const Plane3&);
bool GetAngle(const Vector3&, const Vector3&, Angle&);
bool GetAngle(const Line3&, const Line3&, Angle&);
bool GetAngle(const Plane3&, const Vector3&, Angle&);
bool GetAngle(const Vector3&, const Plane3&, Angle&);
bool GetAngle(const Plane3&,const Line3&, Angle&);
bool GetAngle(const Line3&, const Plane3&, Angle&);
bool GetAngle(const Plane3&, const Plane3&, Angle&);
bool GetIntersection(const Line3&, const Line3&, Vector3&);
bool GetIntersection(const Plane3&, const Line3&, Vector3&);
bool GetIntersection(const Line3&, const Plane3&, Vector3&);
bool GetIntersection(const Plane3&, const Plane3&, Line3&);
bool GetIntersection(const Sphere3&, const Line3&, Vector3&, Vector3&);
bool GetIntersection(const Line3&, const Sphere3&, Vector3&, Vector3&);
bool GetIntersection(const Sphere3&, const Plane3&, Circle3&);
bool GetIntersection(const Plane3&, const Sphere3&, Circle3&);
bool GetIntersection(const Sphere3&, const Sphere3&,  Circle3&);
bool isCollinear(const Vector3&, const Vector3&);
bool isComplanar(const Vector3&, const Vector3&, const Vector3&);
bool isComplanar(const Vector3&, const Vector3&, const Vector3&, const Vector3&);
bool isOrthogonal(const Vector3&, const Vector3&);
bool isOrthogonal(const Vector3&, const Line3&);
bool isOrthogonal(const Line3&, const Vector3&);
bool isOrthogonal(const Line3&, const Line3&);
bool isOrthogonal(const Vector3&, const Plane3&);
bool isOrthogonal(const Plane3&, const Vector3&);
bool isOrthogonal(const Plane3&, const Plane3&);
bool isIntersecting(const Vector3&, const Line3&);
bool isIntersecting(const Line3&, const Vector3&);
bool isIntersecting(const Line3&, const Line3&);
bool isIntersecting(const Vector3&, const Plane3&);
bool isIntersecting(const Plane3&, const Vector3&);
bool isIntersecting(const Line3&, const Plane3&);
bool isIntersecting(const Plane3&, const Line3&);
bool isIntersecting(const Plane3&, const Plane3&);
bool isParallel(const Line3&, const Plane3&);
bool isParallel(const Plane3&, const Line3&);
bool isParallel(const Plane3&, const Plane3&);
Angle getTorsionAngle
  (float, float, float,
   float, float, float,
   float, float, float, 
   float, float, float);
 
