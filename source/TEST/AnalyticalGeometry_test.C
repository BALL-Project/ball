// $Id: AnalyticalGeometry_test.C,v 1.7 2000/03/22 00:44:45 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/common.h>
#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/circle3.h>
#include <BALL/MATHS/line3.h>
#include <BALL/MATHS/plane3.h>
#include <BALL/MATHS/sphere3.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/analyticalGeometry.h>
///////////////////////////

START_TEST(class_name, "$Id: AnalyticalGeometry_test.C,v 1.7 2000/03/22 00:44:45 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Vector3 v1, v2, v3, v4, v5, v6, v7, v8;
Circle3 c1, c2, c3, c4;
Sphere3 s1, s2, s3, s4;
Line3 l1, l2, l3, l4;
Angle a1, a2, a3, a4;
Plane3 p1, p2, p3, p4;

//line54: method GetDeterminant_(const T* m, Size dim)
CHECK(GetDeterminant_(const T* m, Size dim))
  //BAUSTELLE
RESULT


//line114: method GetDeterminant2(const T* m)
CHECK(GetDeterminant2(const T* m))
  //BAUSTELLE
RESULT


//line136: method GetDeterminant3(const T *m)
CHECK(GetDeterminant3(const T *m))
  //BAUSTELLE
RESULT


//line166: method SolveSystem(const T* m, T* x, const Size dim)
CHECK(SolveSystem(const T* m, T* x, const Size dim))
  //BAUSTELLE
RESULT


//line275: method SolveSquaredEquality(const T& a, const T& b, const T &c, T &x1, T &x2)
CHECK(SolveSquaredEquality(const T& a, const T& b, const T &c, T &x1, T &x2))
  //BAUSTELLE
RESULT


//line319: method GetPartition(const TVector3<T>& a, const TVector3<T>& b, const T& r, const T& s)
CHECK(GetPartition(const TVector3<T>& a, const TVector3<T>& b, const T& r, const T& s))
  //BAUSTELLE
RESULT


//line354: method GetDistance(const TLine3<T>& line, const TVector3<T>& point)
CHECK(GetDistance(const TLine3<T>& line, const TVector3<T>& point))
  //BAUSTELLE
RESULT


//line377: method GetDistance(const TLine3<T>& a, const TLine3<T>& b)
CHECK(GetDistance(const TLine3<T>& a, const TLine3<T>& b))
  //BAUSTELLE
RESULT


//line422: method GetDistance(const TPlane3<T>& plane, const TVector3<T>& point)
CHECK(GetDistance(const TPlane3<T>& plane, const TVector3<T>& point))
  //BAUSTELLE
RESULT


//line451: method GetDistance(const TPlane3<T>& plane, const TLine3<T>& line)
CHECK(GetDistance(const TPlane3<T>& plane, const TLine3<T>& line))
  //BAUSTELLE
RESULT


//line481: method GetAngle(const TVector3<T>& a, const TVector3<T>& b, TAngle<T> &intersection_angle)
CHECK(GetAngle(const TVector3<T>& a, const TVector3<T>& b, TAngle<T> &intersection_angle))
  //BAUSTELLE
RESULT


//line517: method GetAngle(const TPlane3<T>& plane, const TVector3<T> &Vector3, TAngle<T> &intersection_angle)
CHECK(GetAngle(const TPlane3<T>& plane, const TVector3<T> &Vector3, TAngle<T> &intersection_angle))
  //BAUSTELLE
RESULT


//line556: method GetAngle(const TPlane3<T>& plane, const TLine3<T>& line, TAngle<T>& intersection_angle)
CHECK(GetAngle(const TPlane3<T>& plane, const TLine3<T>& line, TAngle<T>& intersection_angle))
  //BAUSTELLE
RESULT


//line596: method GetAngle(const TPlane3<T>& a, const TPlane3<T>& b, TAngle<T>& intersection_angle)
CHECK(GetAngle(const TPlane3<T>& a, const TPlane3<T>& b, TAngle<T>& intersection_angle))
  //BAUSTELLE
RESULT


//line651: method GetIntersection(const TPlane3<T>& plane, const TLine3<T>& line, TVector3<T>& intersection_point)
CHECK(GetIntersection(const TPlane3<T>& plane, const TLine3<T>& line, TVector3<T>& intersection_point))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	v2.set(0.0, 0.0, 10.0);
	l1.set(v1, v2);
	v1.set(5.0, 5.0, 0.0);
	TEST_EQUAL(GetIntersection(p1, l1, v2), true)
	TEST_EQUAL(v1, v2);
	v1.set(500.0, 5.0, -5.0);
	v2.set(0.0, 0.0, 10.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetIntersection(p1, l1, v2), false)
RESULT


//line687: method GetIntersection(const TPlane3<T>& a, const TPlane3<T>& b, TLine3<T>& line)
CHECK(GetIntersection(const TPlane3<T>& a, const TPlane3<T>& b, TLine3<T>& line))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 0.0, 10.0);  
	p2 = Plane3(v1, v2, v3);
	v1.set(0.0, 5.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	//??? TEST_EQUAL(GetIntersection(p1, p2, l2), true)
	//TEST_EQUAL(l2, l1);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 0.0, 10.0);  
	p2 = Plane3(v1, v2, v3);
	//TEST_EQUAL(GetIntersection(p1, p1, l1), false)
RESULT


//line765: method GetIntersection(const TSphere3<T> &sphere, const TLine3<T>& line, TVector3<T> &intersection_point1, TVector3<T> &intersection_point2)
CHECK(GetIntersection(const TSphere3<T> &sphere, const TLine3<T>& line, TVector3<T> &intersection_point1, TVector3<T> &intersection_point2))
	v1.set(5.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	v1.set(3.0, 0.0, 0.0);
	v2.set(7.0, 0.0, 0.0);
	//??? TEST_EQUAL(GetIntersection(s1, l1, v3, v4), true);
	//TEST_EQUAL(v1, v3);
	//TEST_EQUAL(v2, v4);
	v1.set(500.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	//TEST_EQUAL(GetIntersection(s1, l1, v3, v4), false);
RESULT


//line809: method GetIntersection(const TSphere3<T>& sphere, const TPlane3<T>& plane, TCircle3<T>& intersectionCircle)
CHECK(GetIntersection(const TSphere3<T>& sphere, const TPlane3<T>& plane, TCircle3<T>& intersectionCircle))
	v1.set(5.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 10.0, 0.0);
	v3.set(10.0, 0.0, 0.0);
	p1.set(v1, v2, v3);
	v1.set(5.0, 0.0, 0.0);
	v2.set(-0.0, -0.0, -100.0);
	c1.set(v1, v2, 2.0);
	TEST_EQUAL(GetIntersection(s1, p1, c2), true)
	TEST_EQUAL(c2, c1)
	v1.set(500.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	TEST_EQUAL(GetIntersection(s1, p1, c1), false)
RESULT


//line861: method GetIntersection(const TSphere3<T>& a, const TSphere3<T>& b, TCircle3<T>& intersection_circle)
CHECK(GetIntersection(const TSphere3<T>& a, const TSphere3<T>& b, TCircle3<T>& intersection_circle))
	v1.set(0.0, 0.0, 0.0);
	s1 = Sphere3(v1, 5);
	v1.set(2.5, 0.0, 0.0);
	s1 = Sphere3(v1, 5);
	//???	TEST_EQUAL(GetIntersection(s1, s2, c1), true)
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 0.0, 0.0);
	c2 = Circle3(v1, v2, 3.0);
	//TEST_EQUAL(c1, c2)
RESULT


//line907: method isComplanar(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c)
CHECK(isComplanar(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c))
	v1.set(1.0, 2.0, 3.0);
	v2.set(2.0, 4.0, 6.0);
	v3.set(3.0, 6.0, 9.0);
	TEST_EQUAL(isComplanar(v1, v2, v3), true)
	v2.set(35.0, -6.1, 9.1);
	v3.set(3.0, -66.0, 19.0);
	TEST_EQUAL(isComplanar(v1, v2, v3), false)
RESULT


//line933: method isOrthogonal(const TVector3<T>& a, const TVector3<T>& b)
CHECK(isOrthogonal(const TVector3<T>& a, const TVector3<T>& b))
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 5.0);
	TEST_EQUAL(isOrthogonal(v2, v3), true)
	TEST_EQUAL(isOrthogonal(v2, v2), false)
RESULT


//line957: method isOrthogonal(const TLine3<T>& line, const TVector3<T>& vector)
CHECK(isOrthogonal(const TLine3<T>& line, const TVector3<T>& vector))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 5.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isOrthogonal(l1, v3), true)
	TEST_EQUAL(isOrthogonal(l1, v1), false)
RESULT


//line981: method isOrthogonal(const TVector3<T>& vector, const TPlane3<T>& plane)
CHECK(isOrthogonal(const TVector3<T>& vector, const TPlane3<T>& plane))
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(5.0, 0.0, 0.0);
	p1 = Plane3(v1, v2, v3);
	v3.set(0.0, 0.0, 5.0);
	v1.set(2.0, 2.0, 2.0);
	TEST_EQUAL(isOrthogonal(v3, p1), true)
	TEST_EQUAL(isOrthogonal(v1, p1), false)
RESULT

CHECK(isOrthogonal(const TPlane3<T>& plane, const TVector3<T>& vector))
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(5.0, 0.0, 0.0);
	p1 = Plane3(v1, v2, v3);
	v3.set(0.0, 0.0, 5.0);
	v1.set(2.0, 2.0, 2.0);
	TEST_EQUAL(isOrthogonal(p1, v3), true)
	TEST_EQUAL(isOrthogonal(p1, v1), false)
RESULT

//line1005: method isOrthogonal(const TPlane3<T>& a, const TPlane3<T>& b)
CHECK(isOrthogonal(const TPlane3<T>& a, const TPlane3<T>& b))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(1.0, 0.0, 0.0);
	v2.set(11.0, 0.0, 0.0);
	v3.set(1.0, 0.0, 10.0);  
	p2 = Plane3(v1, v2, v3);
	TEST_EQUAL(isOrthogonal(p1, p2), true)
	TEST_EQUAL(isOrthogonal(p1, p1), false)
RESULT

CHECK(isIntersecting(const TVector3<T>& point, const TLine3<T>& line))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(1.0, 4.0, 3.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isIntersecting(v3, l1), true)
	v3.set(0.0, 4.0, 3.0);
	TEST_EQUAL(isIntersecting(v3, l1), false)
RESULT

//line1029: method isIntersecting(const TLine3<T>& line, const TVector3<T>& point)
CHECK(isIntersecting(const TLine3<T>& line, const TVector3<T>& point))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(1.0, 4.0, 3.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isIntersecting(l1, v3), true)
	v3.set(0.0, 4.0, 3.0);
	TEST_EQUAL(isIntersecting(l1, v3), false)
RESULT

CHECK(isIntersecting(const TLine3<T>& a, const TLine3<T>& b))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(1.0, 4.0, 3.0);
	l1 = Line3(v1, v2);
	l2 = Line3(v1, v3);
	TEST_EQUAL(isIntersecting(l1, l2), true)
	v1.set(110.0, 4.0, 3.0);
	l2 = Line3(v1, v3);
	TEST_EQUAL(isIntersecting(l1, l2), false)
RESULT


//line1053: method isIntersecting(const TVector3<T>& point, const TPlane3<T>& plane)
CHECK(isIntersecting(const TVector3<T>& point, const TPlane3<T>& plane))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 6.0);  
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(v1, p1), true)
	v1.set(0.0, 0.0, 0.0);
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(v1, p1), true)
RESULT

CHECK(isIntersecting(const TPlane3<T>& plane, const TVector3<T>& point))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 6.0);  
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(p1, v1), true)
	v1.set(0.0, 0.0, 0.0);
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(p1, v1), true)
RESULT

//line1077: method isIntersecting(const TLine3<T>& line, const TPlane3<T>& plane)
CHECK(isIntersecting(const TLine3<T>& line, const TPlane3<T>& plane))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 0.0, 3.0);
	v3.set(0.0, 3.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 0.0, 2.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isIntersecting(l1, p1), true)
	v1.set(100.0, 200.0, 300.0);
	v2.set(1.0, 2.0, 3.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isIntersecting(l1, p1), false)
RESULT


//line1101: method isIntersecting(const TPlane3<T>& a, const TPlane3<T>& b)
CHECK(isIntersecting(const TPlane3<T>& a, const TPlane3<T>& b))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 0.0, 3.0);
	v3.set(0.0, 3.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 0.0, 2.0);
	v3.set(0.0, 2.0, 0.0);  
	p2 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(p1, p2), true)
	v1.set(100.0, 200.0, 300.0);
	p2 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(p1, p2), false)
RESULT


//line1125: method isParallel(const TPlane3<T>& plane, const TLine3<T>& line)
CHECK(isParallel(const TPlane3<T>& plane, const TLine3<T>& line))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(isParallel(p1, l1), true)
	v2.set(103.1, 113.1, 204.1);  
	l1.set(v1, v2);
	TEST_EQUAL(isParallel(p1, l1), false)	
RESULT

CHECK(isParallel(const TLine3<T>& line, const TPlane3<T>& plane))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(1.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(isParallel(l1, p1), true)
	v2.set(103.1, 113.1, 204.1);  
	l1.set(v1, v2);
	TEST_EQUAL(isParallel(l1, p1), false)	
RESULT


CHECK(isParallel(const TPlane3<T>& a, const TPlane3<T>& b))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 0.0, 10.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(1.0, 0.0, 0.0);
	v2.set(11.0, 0.0, 0.0);
	v3.set(1.0, 0.0, 10.0);  
	p2 = Plane3(v1, v2, v3);
	TEST_EQUAL(isParallel(p1, p2), true)
	v2.set(100.0, 1110.0, 210.0);
	p2 = Plane3(v1, v2, v3);
	TEST_EQUAL(isParallel(p1, p2), false)
RESULT

CHECK(TAngle<T> getTorsionAngle
		(const T& ax, const T& ay, const T& az,
		 const T& bx, const T& by, const T& bz,
		 const T& cx, const T& cy, const T& cz, 
		 const T& dx, const T& dy, const T& dz))
	a1 = getTorsionAngle((float)0.0, (float)0.0,  (float)0.0,   (float)10.0,
											 (float)0.0, (float)0.0,  (float)0.0,   (float)-10.0,
											 (float)0.0, (float)10.0, (float)-10.0, (float)0.0);
	Angle res(-180, false);
	TEST_EQUAL(a1, res)

	a1 = getTorsionAngle((float)0.0, (float)0.0, (float)0.0,  (float)10.0,
											 (float)0.0, (float)0.0, (float)0.0,  (float)-10.0,
											 (float)0.0, (float)5.0, (float)-5.0, (float)10.0);
	res.set(-90, false);
	TEST_EQUAL(a1, res)

	a1 = getTorsionAngle((float)0.0, (float)0.0, (float)0.0, (float)10.0,
											 (float)0.0, (float)0.0, (float)0.0, (float)-10.0,
											 (float)0.0, (float)0.0, (float)0.0, (float)0.0);
	res.set(0, false);
	TEST_EQUAL(a1, res)
	a1 = getTorsionAngle((float)0.0, (float)0.0,  (float)0.0,   (float)10.0,
											 (float)0.0, (float)0.0,  (float)0.0,   (float)-10.0,
											 (float)0.0, (float)10.0, (float)-10.0, (float)0.0);
	TEST_EXCEPTION(Exception::DivisionByZero, 
		getTorsionAngle((float)0, (float)0, (float)0, (float)0, (float)0, (float)0, 
										(float)0, (float)0, (float)0, (float)0, (float)0, (float)0))
RESULT

END_TEST