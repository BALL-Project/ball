// $Id: analyticalGeometry.sip,v 1.1 2000/07/18 21:27:05 oliver Exp $

%HeaderCode
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
//  (float /a/, float b, const float c, float& x1, float x2);
// Vector3 GetPartition(const Vector3& /a/, const Vector3& b);
// Vector3 GetPartition(const Vector3& /a/, const Vector3& b, float r, float s);
float GetDistance(const Vector3& /a/, const Vector3& /b/);
float GetDistance(const Line3& /line/, const Vector3& /point/);
float GetDistance(const Vector3& /point/, const Line3& /line/);
float GetDistance(const Line3& /a/, const Line3& /b/);
float GetDistance(const Vector3& /point/, const Plane3& /plane/);
float GetDistance(const Plane3& /plane/, const Vector3& /point/);
float GetDistance(const Line3& /line/, const Plane3& /plane/);
float GetDistance(const Plane3& /plane/, const Line3& /line/);
float GetDistance(const Plane3& /a/, const Plane3&/b/);
bool GetAngle(const Vector3& /a/, const Vector3&/b/, Angle& /intersection_angle/);
bool GetAngle(const Line3& /a/, const Line3&/b/, Angle& /intersection_angle/);
bool GetAngle(const Plane3& /plane/, const Vector3& /vector/, Angle& /intersection_angle/);
bool GetAngle(const Vector3& /vector/, const Plane3& /plane/, Angle& /intersection_angle/);
bool GetAngle(const Plane3& /plane/,const Line3& /line/, Angle& /intersection_angle/);
bool GetAngle(const Line3& /line/, const Plane3& /plane/, Angle& /intersection_angle/);
bool GetAngle(const Plane3& /a/, const Plane3&/b/, Angle& /intersection_angle/);
bool GetIntersection(const Line3& /a/, const Line3&/b/, Vector3& /point/);
bool GetIntersection(const Plane3& /plane/, const Line3& /line/, Vector3& /intersection_point/);
bool GetIntersection(const Line3& /line/, const Plane3& /plane/, Vector3& /intersection_point/);
bool GetIntersection(const Plane3& /a/, const Plane3&/b/, Line3& /line/);
bool GetIntersection(const Sphere3& /sphere/, const Line3& /line/, Vector3& /intersection_point1/, Vector3& /intersection_point2/);
bool GetIntersection(const Line3& /line/, const Sphere3& /sphere/, Vector3& /intersection_point1/, Vector3& /intersection_point2/);
bool GetIntersection(const Sphere3& /sphere/, const Plane3& /plane/, Circle3& /intersection_circle/);
bool GetIntersection(const Plane3& /plane/, const Sphere3& /sphere/, Circle3& /intersection_circle/);
bool GetIntersection(const Sphere3& /a/, const Sphere3& /b/,  Circle3& /intersection_circle/);
bool isCollinear(const Vector3& /a/, const Vector3&/b/);
bool isComplanar(const Vector3& /a/, const Vector3&/b/, const Vector3& /c/);
bool isComplanar(const Vector3& /a/, const Vector3&/b/, const Vector3& /c/, const Vector3& /d/);
bool isOrthogonal(const Vector3& /a/, const Vector3&/b/);
bool isOrthogonal(const Vector3& /vector/, const Line3& /line/);
bool isOrthogonal(const Line3& /line/, const Vector3& /vector/);
bool isOrthogonal(const Line3& /a/, const Line3&/b/);
bool isOrthogonal(const Vector3& /vector/, const Plane3& /plane/);
bool isOrthogonal(const Plane3& /plane/, const Vector3& /vector/);
bool isOrthogonal(const Plane3& /a/, const Plane3&/b/);
bool isIntersecting(const Vector3& /point/, const Line3& /line/);
bool isIntersecting(const Line3& /line/, const Vector3& /point/);
bool isIntersecting(const Line3& /a/, const Line3&/b/);
bool isIntersecting(const Vector3& /point/, const Plane3& /plane/);
bool isIntersecting(const Plane3& /plane/, const Vector3& /point/);
bool isIntersecting(const Line3& /line/, const Plane3& /plane/);
bool isIntersecting(const Plane3& /plane/, const Line3& /line/);
bool isIntersecting(const Plane3& /a/, const Plane3&/b/);
bool isParallel(const Line3& /line/, const Plane3& /plane/);
bool isParallel(const Plane3& /plane/, const Line3& /line/);
bool isParallel(const Plane3& /a/, const Plane3& /b/);
Angle getTorsionAngle
  (float /ax/, float /ay/, float /az/,
   float /bx/, float /by/, float /bz/,
   float /cx/, float /cy/, float /cz/, 
   float /dx/, float /dy/, float /dz/);
 
