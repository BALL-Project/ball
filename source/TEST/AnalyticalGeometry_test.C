// $Id: AnalyticalGeometry_test.C,v 1.20 2000/09/05 09:56:17 oliver Exp $
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

START_TEST(class_name, "$Id: AnalyticalGeometry_test.C,v 1.20 2000/09/05 09:56:17 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Vector3 v1, v2, v3, v4, v5, v6, v7, v8;
Circle3 c1, c2, c3, c4;
Sphere3 s1, s2, s3, s4;
Line3 l1, l2, l3, l4;
Angle a1, a2, a3, a4;
Plane3 p1, p2, p3, p4;

CHECK(GetDeterminant_(const T* m, Size dim))
	float m[16];
	float x, x1;
	for (Size i = 0; i < 16; i++ )
	{
		m[i] = (float) i;
	}
	m[5] = (float)6;
	m[6] = (float)5;
	x = GetDeterminant_(m, 4);

	m[0] = (float)4;
	m[1] = (float)6;
	m[2] = (float)5;
	m[3] = (float)7;
	m[4] = (float)0;
	m[5] = (float)1;
	m[6] = (float)2;
	m[7] = (float)3;
	x1 = GetDeterminant_(m, 4);
	TEST_REAL_EQUAL(-x, x1)
	for (int i = 0; i < 16 ; i++ )
	{
		m[i] = (float) i * 2;
	}
	x = GetDeterminant_(m, 4);
	TEST_REAL_EQUAL(x * 2, x1)
RESULT

CHECK(GetDeterminant(const T* m, Size dim))
	float m[16];
	float x, x1;
	for (Size i = 0; i < 16; i++ )
	{
		m[i] = (float) i;
	}
	m[5] = (float)6;
	m[6] = (float)5;
	x = GetDeterminant(m, 4);

	m[0] = (float)4;
	m[1] = (float)6;
	m[2] = (float)5;
	m[3] = (float)7;
	m[4] = (float)0;
	m[5] = (float)1;
	m[6] = (float)2;
	m[7] = (float)3;
	x1 = GetDeterminant(m, 4);
	TEST_REAL_EQUAL(-x, x1)
	for (int i = 0; i < 16 ; i++ )
	{
		m[i] = (float) i * 2;
	}
	x = GetDeterminant(m, 4);
	TEST_REAL_EQUAL(x * 2, x1)
RESULT

CHECK(GetDeterminant2(const T* m))
	float m[16], x, x1;
	m[0] = 0;
	m[1] = 1;
	m[2] = 2;
	m[3] = 3;
	x1 = GetDeterminant2(m);
	m[0] = 2;
	m[1] = 3;
	m[2] = 0;
	m[3] = 1;
	x = GetDeterminant2(m);
	TEST_REAL_EQUAL(-x, x1)
	m[0] = 4;
	m[1] = 6;
	m[2] = 0;
	m[3] = 2;
	x1 = GetDeterminant2(m);
	TEST_REAL_EQUAL(x * 4, x1)
RESULT

CHECK(GetDeterminant2(const T& m00, const T& m01, const T& m10, const T& m11))
	float x, x1;
	x = GetDeterminant2(0, 1, 2, 3);
	x1 = GetDeterminant2(2, 3, 0, 1);
	TEST_REAL_EQUAL(-x, x1)
RESULT

CHECK(GetDeterminant3(const T *m))
	float m[9], x, x1;
	for (int i = 0; i < 9 ; i++ )
	{
		m[i] = (float) i;
	}
	x = GetDeterminant3(m);
	m[0] = 3;
	m[1] = 4;
	m[2] = 5;
	m[3] = 0;
	m[4] = 1;
	m[5] = 2;
	x1 = GetDeterminant3(m);
	TEST_REAL_EQUAL(-x, x1)
	for (int i = 0; i < 9 ; i++ )
	{
		m[i] = (float) i * 2;
	}
	x = GetDeterminant3(m);
	TEST_REAL_EQUAL(x * 2, x1)
RESULT

CHECK(GetDeterminant3(const T& m00, const T& m01, const T& m02,
								  		const T& m10, const T& m11, const T& m12,
											const T& m20, const T& m21, const T& m22))
	float x = GetDeterminant3(0, 1, 2, 3, 4, 5, 6, 7, 8);
	float x1 = GetDeterminant3(3, 4, 5, 0, 1, 2, 6, 7, 8);
	TEST_REAL_EQUAL(x, -x1)
RESULT

CHECK(SolveSystem(const T* m, T* x, const Size dim))
	float m[6], x[2];
	m[0] = 1.0;
	m[1] = 1.0;
	m[2] = 2.0;
	m[3] = 2.0;
	m[4] = 2.0;
	m[5] = 4.0;
	TEST_EQUAL(SolveSystem(m, x, 2), false)

	m[0] = 2.0;
	m[1] = 2.0;
	m[2] = 6.0;
	m[3] = 3.0;
	m[4] = 4.0;
	m[5] = 11.0;
	TEST_EQUAL(SolveSystem(m, x, 2), true)
	TEST_REAL_EQUAL(x[0], 1.0)
	TEST_REAL_EQUAL(x[1], 2.0)
RESULT

CHECK(SolveSystem2(const T& a1, const T& b1, const T& c1,
									const T& a2, const T& b2, const T& c2,
									T& x1, T& x2))
	float x1, x2;
  //2*1 + 2*2 = 6
	//3*1 + 4*2 = 11
	TEST_EQUAL(SolveSystem2((float)2.0, (float)2.0, (float)6.0, (float)3.0, (float)4.0, (float)11.0, x1, x2), true)
	TEST_REAL_EQUAL(x1, 1.0)
	TEST_REAL_EQUAL(x2, 2.0)
RESULT

CHECK(SolveSquaredEquality(const T& a, const T& b, const T &c, T &x1, T &x2))
	float x1, x2;
	// 0 + 0 = 0
	// -1^2 - 1 = 0 
	TEST_EQUAL(SolveQuadraticEquation( (float)-1.0, (float)-1.0, (float)0.0, x1, x2), 2)
	TEST_REAL_EQUAL(x1, -1.0)
	TEST_REAL_EQUAL(x2, 0.0)
	// 2*2^2 + 2*2 = 0
	// 2*0^2 + 2*0 = 0
	TEST_EQUAL(SolveQuadraticEquation((float)2.0, (float)-4.0, (float)0.0, x1, x2), 2)
	TEST_REAL_EQUAL(x1, 2.0)
	TEST_REAL_EQUAL(x2, 0.0)
	// 2*2^2  + 4*2  - 16 = 0
	// 2*-4^2 + 4*-4 - 16 = 0
	TEST_EQUAL(SolveQuadraticEquation((float)2.0, (float)4.0, (float)-16.0, x1, x2), 2)
	TEST_REAL_EQUAL(x1, 2.0)
	TEST_REAL_EQUAL(x2, -4.0)
	// 0*x^2 + 0*x + 1 != 0 
	TEST_EQUAL(SolveQuadraticEquation((float)0.0, (float)0.0, (float)1.0, x1, x2), 0)
	TEST_REAL_EQUAL(x1, 2.0)
	// 0*x^2 + 1*1 + 1 = 0
	TEST_EQUAL(SolveQuadraticEquation((float)0.0, (float)1.0, (float)1.0, x1, x2), 1)
	TEST_REAL_EQUAL(x1, 1.0)
	TEST_REAL_EQUAL(x2, 1.0)
RESULT

CHECK(GetPartition(const TVector3<T>& a, const TVector3<T>& b))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(5.0, 0.0, 0.0);
	TEST_EQUAL(GetPartition(v1, v2), v3)
RESULT

CHECK(GetPartition(const TVector3<T>& a, const TVector3<T>& b, const T& r, const T& s))
	v1.set(0.0, 10.0, 10.0);
	v2.set(20.0, 0.0, 0.0);
	v3.set(10.0, 5.0, 5.0);
	TEST_EQUAL(GetPartition(v1, v2,(float)2.0, (float)2.0), v3)
	TEST_EXCEPTION(Exception::DivisionByZero, GetPartition(v1, v2, (float)0.0, (float)0.0))
RESULT

CHECK(GetDistance(const TVector3<T>& a, const TVector3<T>& b))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	TEST_REAL_EQUAL(GetDistance(v1, v2), 10.0)
	v2.set(0.0, 0.0, 0.0);
	TEST_REAL_EQUAL(GetDistance(v1, v2), 0.0)
RESULT

CHECK(GetDistance(const TLine3<T>& line, const TVector3<T>& point))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1 = Line3(v1, v2);
	v1.set(0.0, 0.0, -5.0);
	TEST_REAL_EQUAL(GetDistance(l1, v1), 5.0)
	v1.set(0.0, 0.0, 0.0);
	TEST_REAL_EQUAL(GetDistance(l1, v1), 0.0)
	l1 = Line3(v1, v1);
	TEST_EXCEPTION(Exception::DivisionByZero, GetDistance(l1, v1))
RESULT

CHECK(GetDistance(const TVector3<T>& point, const TLine3<T>& line))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1 = Line3(v1, v2);
	v1.set(0.0, 0.0, -5.0);
	TEST_REAL_EQUAL(GetDistance(v1, l1), 5.0)
	v1.set(0.0, 0.0, 0.0);
	TEST_REAL_EQUAL(GetDistance(v1, l1), 0.0)
RESULT

CHECK(GetDistance(const TLine3<T>& a, const TLine3<T>& b))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1 = Line3(v1, v2);
	v1.set(0.0, 0.0, -5.0);
	v2.set(10.0, 0.0, 0.0);
	l2.set(v1, v2);
	TEST_REAL_EQUAL(GetDistance(l1, l2), 5.0)
	v1.set(0.0, 0.0, -5.0);
	v2.set(0.0, 10.0, 0.0);
	l2.set(v1, v2);
	TEST_REAL_EQUAL(GetDistance(l1, l2), 5.0)
	l2 = l1;
	TEST_REAL_EQUAL(GetDistance(l1, l2), 0.0)
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 0.0, 0.0);
	l1 = Line3(v1, v2);
	l2 = l1;
	TEST_EXCEPTION(Exception::DivisionByZero, GetDistance(l1, l2))
RESULT

CHECK(GetDistance(const TVector3<T>& point, const TPlane3<T>& plane))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	TEST_REAL_EQUAL(GetDistance(v1, p1), 5.0)
	v1.set(5.0, 5.0, 0.0);
	TEST_REAL_EQUAL(GetDistance(v1, p1), 0.0)
	v1.set(0.0, 0.0, 0.0);
	p1 = Plane3(v1, v1, v1);
	TEST_EXCEPTION(Exception::DivisionByZero, GetDistance(v1, p1))
RESULT

CHECK(GetDistance(const TPlane3<T>& plane, const TVector3<T>& point))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	TEST_REAL_EQUAL(GetDistance(p1, v1), 5.0)
	v1.set(5.0, 5.0, 0.0);
	TEST_REAL_EQUAL(GetDistance(p1, v1), 0.0)
RESULT

CHECK(GetDistance(const TLine3<T>& line, const TPlane3<T>& plane))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	v2.set(0.0, 10.0, 0.0);
	l1.set(v1, v2);
	TEST_REAL_EQUAL(GetDistance(l1, p1), 5.0)
	v1.set(5.0, 5.0, 0.0);
	v2.set(0.0, 10.0, 0.0);
	l1.set(v1, v2);
	TEST_REAL_EQUAL(GetDistance(l1, p1), 0.0)
	p1.n.set(0.0, 0.0, 0.0);
	TEST_EXCEPTION(Exception::DivisionByZero, GetDistance(l1, p1))
RESULT

CHECK(GetDistance(const TPlane3<T>& plane, const TLine3<T>& line))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	v2.set(0.0, 10.0, 0.0);
	l1.set(v1, v2);
	TEST_REAL_EQUAL(GetDistance(p1, l1), 5.0)
	v1.set(5.0, 5.0, 0.0);
	v2.set(0.0, 10.0, 0.0);
	l1.set(v1, v2);
	TEST_REAL_EQUAL(GetDistance(p1, l1), 0.0)
RESULT

CHECK(GetDistance(const TPlane3<T>& a, const TPlane3<T>& b))
	v1.set(0.0, 0.0, 5.0);
	v2.set(-10.0, 0.0, 5.0);
	v3.set(0.0, -10.0, 5.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(10.0, 0.0, 0.0);
	v2.set(20.0, 0.0, 0.0);
	v3.set(10.0, 10.0, 0.0);  
	p2 = Plane3(v1, v2, v3);
	TEST_REAL_EQUAL(GetDistance(p1, p2), 5.0)

	p1 = p2;
	TEST_REAL_EQUAL(GetDistance(p1, p2), 0.0)

	v1.set(10.0, 10.0, 0.0);
	v2.set(20.0, 0.0, 0.0);
	v3.set(10.0, 10.0, 0.0);  
	p2 = Plane3(v1, v2, v3);
	TEST_REAL_EQUAL(GetDistance(p1, p2), 0.0)

	v3.set(0.0, 0.0, 0.0);  
	p1 = Plane3(v1, v1, v1);
	TEST_EXCEPTION(Exception::DivisionByZero, GetDistance(p1, p2))
RESULT

CHECK(GetAngle(const TVector3<T>& a, const TVector3<T>& b, TAngle<T> &intersection_angle))
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	TEST_EQUAL(GetAngle(v2, v3, a1), true)
	a2.set(90, false);
	TEST_EQUAL(a1, a2)
	v2.set(0.0, 0.0, 0.0);
	TEST_EQUAL(GetAngle(v2, v3, a1), false)
RESULT

CHECK(GetAngle(const TLine3<T>& a, const TLine3<T>& b, TAngle<T>& intersection_angle))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 10.0, 0.0);
	l2.set(v1, v2);
	a1.set(90, false);
	TEST_EQUAL(GetAngle(l1, l2, a2), true)
	TEST_EQUAL(a2, a1)
	v2.set(0.0, 0.0, 0.0);
	l2.set(v1, v2);
	TEST_EQUAL(GetAngle(l1, l2, a1), false)
RESULT

CHECK(GetAngle(const TPlane3<T>& plane, const TVector3<T> &Vector3, TAngle<T> &intersection_angle))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v2.set(0.0, 0.0, 10.0);
	a2.set(90, false);
	TEST_EQUAL(GetAngle(p1, v2, a1), true)
	TEST_EQUAL(a1, a2)
	v2.set(0.0, 0.0, 0.0);
	TEST_EQUAL(GetAngle(p1, v2, a1), false)
RESULT

CHECK(GetAngle(const TVector3<T> &Vector3,const TPlane3<T>& plane, TAngle<T> &intersection_angle))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v2.set(0.0, 0.0, 10.0);
	a2.set(90, false);
	TEST_EQUAL(GetAngle(v2, p1, a1), true)
	TEST_EQUAL(a1, a2)
	v2.set(0.0, 0.0, 0.0);
	TEST_EQUAL(GetAngle(v2, p1, a1), false)
RESULT

CHECK(GetAngle(const TPlane3<T>& plane, const TLine3<T>& line, TAngle<T>& intersection_angle))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	v2.set(0.0, 0.0, 10.0);
	l1.set(v1, v2);
	a2.set(90, false);
	TEST_EQUAL(GetAngle(p1, l1, a1), true)
	TEST_EQUAL(a1, a2)
	v2.set(0.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetAngle(p1, l1, a1), false)
RESULT

CHECK(GetAngle(const TLine3<T>& line, const TPlane3<T>&, plane TAngle<T>& intersection_angle))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	v2.set(0.0, 0.0, 10.0);
	l1.set(v1, v2);
	a2.set(90, false);
	TEST_EQUAL(GetAngle(l1, p1, a1), true)
	TEST_EQUAL(a1, a2)
	v2.set(0.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetAngle(l1, p1, a1), false)
RESULT

CHECK(GetAngle(const TPlane3<T>& a, const TPlane3<T>& b, TAngle<T>& intersection_angle))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 0.0, 10.0);  
	p2 = Plane3(v1, v2, v3);
	a2.set(90, false);
	TEST_EQUAL(GetAngle(p1, p2, a1), true)
	TEST_EQUAL(a1, a2);
	v2.set(0.0, 0.0, 0.0);
	v3.set(0.0, 0.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(GetAngle(p1, p2, a1), false)
RESULT

CHECK(GetIntersection(const TLine3<T>& a, const TLine3<T>& b, TVector3<T>& point))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 10.0, 0.0);
	l2.set(v1, v2);
	v2.set(0.0, 0.0, 0.0);
	TEST_EQUAL(GetIntersection(l1, l2, v1), true)
	TEST_EQUAL(v1, v2);
	v1.set(100.0, 0.0, 0.0);
	v1.set(1.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetIntersection(l1, l2, v1), false)
RESULT

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
	v1.set(0.0, 0.0, 10.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(l1.d * p1.n, 0.0)
	TEST_EQUAL(GetIntersection(p1, l1, v2), false)
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 0.0, 10.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetIntersection(p1, l1, v2), true)
	v1.set(0.0, 0.0, 0.0);
	TEST_EQUAL(v2, v1)
RESULT

CHECK(GetIntersection(const TLine3<T>& line, const TPlane3<T>& plane, TVector3<T>& intersection_point))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(5.0, 5.0, -5.0);
	v2.set(0.0, 0.0, 10.0);
	l1.set(v1, v2);
	v1.set(5.0, 5.0, 0.0);
	TEST_EQUAL(GetIntersection(l1, p1, v2), true)
	TEST_EQUAL(v1, v2);
	v1.set(0.0, 5.0, 10.0);
	v2.set(1.0, 2.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetIntersection(l1, p1, v2), false)
RESULT

CHECK(GetIntersection(const TPlane3<T>& a, const TPlane3<T>& b, TLine3<T>& line))
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 10.0, 0.0);  
	p1 = Plane3(v1, v2, v3);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	v3.set(0.0, 0.0, 10.0);  
	p2 = Plane3(v1, v2, v3);
	v1.set(0.0, 0.0, 0.0);
	v2.set(1.0, 0.0, 0.0);
	l1.set(v1, v2);
	TEST_EQUAL(GetIntersection(p1, p2, l2), true)
	l2.normalize();
	TEST_EQUAL(l2, l1);
	v1.set(0.0, 0.0, 5.0);
	v2.set(10.0, 0.0, 5.0);
	v3.set(0.0, 0.0, 15.0);  
	p2 = Plane3(v1, v2, v3);
	TEST_EQUAL(GetIntersection(p1, p1, l1), false)
RESULT

CHECK(GetIntersection(const TSphere3<T> &sphere, const TLine3<T>& line, TVector3<T> &intersection_point1, TVector3<T> &intersection_point2))
	v1.set(0.0, 0.0, 0.0);
	s1 = Sphere3(v1, sqrt(3.0));
	v1.set(-10.0, -10.0, -10.0);
	v2.set(10.0, 10.0, 10.0);
	l1.set(v1, v2);
	v1.set(1.0, 1.0, 1.0);
	v2.set(-1.0, -1.0, -1.0);
	TEST_EQUAL(GetIntersection(s1, l1, v3, v4), true);
	TEST_EQUAL(v1, v3)	
	TEST_EQUAL(v2, v4)
	v1.set(500.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	TEST_EQUAL(GetIntersection(s1, l1, v3, v4), false);
RESULT

CHECK(GetIntersection(const TLine3<T>& line, const TSphere3<T> &sphere, TVector3<T> &intersection_point1, TVector3<T> &intersection_point2))
	v1.set(5.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	v1.set(0.0, 0.0, 0.0);
	v2.set(10.0, 0.0, 0.0);
	l1.set(v1, v2);
	v1.set(3.0, 0.0, 0.0);
	v2.set(7.0, 0.0, 0.0);
	TEST_EQUAL(GetIntersection(l1, s1, v3, v4), true);
	TEST_EQUAL(v1, v4);
	TEST_EQUAL(v2, v3);
	v1.set(500.0, -500.0, 1000.0);
	s1 = Sphere3(v1, 2);
	TEST_EQUAL(GetIntersection(l1, s1, v3, v4), false);
RESULT

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
	TEST_EQUAL(c1, c2);

	v1.set(-5.0, 0.0, 4.0);
	s1.set(v1, 5.0);
	v1.set(-5.0, 0.0, 0.0);
	v2.set(-0.0, -0.0, -100.0);
	c1.set(v1, v2, 3.0);
	TEST_EQUAL(GetIntersection(s1, p1, c2), true)
	TEST_EQUAL(c2, c1)

	v1.set(0.0, 0.0, 500.0);
	s1 = Sphere3(v1, 2);
	TEST_EQUAL(GetIntersection(s1, p1, c1), false)
RESULT

CHECK(GetIntersection(const TPlane3<T>& plane, const TSphere3<T>& sphere TCircle3<T>& intersectionCircle))
	v1.set(5.0, 0.0, 0.0);
	s1 = Sphere3(v1, 2);
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 10.0, 0.0);
	v3.set(10.0, 0.0, 0.0);
	p1.set(v1, v2, v3);
	v1.set(5.0, 0.0, 0.0);
	v2.set(-0.0, -0.0, -100.0);
	c1.set(v1, v2, 2.0);
	TEST_EQUAL(GetIntersection(p1, s1, c2), true)
	TEST_EQUAL(c2, c1)
	v1.set(0.0, 0.0, 500.0);
	s1 = Sphere3(v1, 2);
	TEST_EQUAL(GetIntersection(p1, s1, c1), false)
RESULT

CHECK(GetIntersection(const TSphere3<T>& a, const TSphere3<T>& b, TCircle3<T>& intersection_circle))
	v1.set(0.0, 0.0, 0.0);
	s1 = Sphere3(v1, 5);
	v1.set(5.0, 0.0, 0.0);
	s2 = Sphere3(v1, 5);
	TEST_EQUAL(GetIntersection(s1, s2, c1), true)
	v1.set(2.5, 0.0, 0.0);
	v2.set(1.0, 0.0, 0.0);
	c2 = Circle3(v1, v2, sqrt(25.0 - 6.25));
	TEST_EQUAL(c1, c2);
RESULT

CHECK(isCollinear(const TVector3<T>& a, const TVector3<T>& b))
	v1.set(1.0, 2.0, 3.0);
	v2.set(2.0, 4.0, 6.0);
	TEST_EQUAL(isCollinear(v1, v2), true)
	v2.set(12.0, 4.0, 6.0);
	TEST_EQUAL(isCollinear(v1, v2), false)
RESULT

CHECK(isComplanar(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c))
	v1.set(1.0, 2.0, 3.0);
	v2.set(2.0, 4.0, 6.0);
	v3.set(3.0, 6.0, 9.0);
	TEST_EQUAL(isComplanar(v1, v2, v3), true)
	v2.set(35.0, -6.1, 9.1);
	v3.set(3.0, -66.0, 19.0);
	TEST_EQUAL(isComplanar(v1, v2, v3), false)
RESULT

CHECK(isComplanar(const TVector3<T>& a, const TVector3<T>& b, const TVector3<T>& c, const TVector3<T>& d))
	v1.set(1.0, 2.0, 3.0);
	v2.set(2.0, 4.0, 6.0);
	v3.set(3.0, 6.0, 9.0);
	v4.set(4.0, 8.0, 12.0);
	TEST_EQUAL(isComplanar(v1, v2, v3, v4), true)
	v2.set(35.0, -6.1, 9.1);
	v3.set(351.0, -6.1, 9.1);
	v4.set(35.0, -61.1, 91.1);
	TEST_EQUAL(isComplanar(v1, v2, v3, v4), false)
RESULT

CHECK(isOrthogonal(const TVector3<T>& a, const TVector3<T>& b))
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 5.0);
	TEST_EQUAL(isOrthogonal(v2, v3), true)
	TEST_EQUAL(isOrthogonal(v2, v2), false)
RESULT

CHECK(isOrthogonal(const TVector3<T>& vector, const TLine3<T>& line))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 5.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isOrthogonal(v3, l1), true)
	TEST_EQUAL(isOrthogonal(v1, l1), false)
RESULT

CHECK(isOrthogonal(const TLine3<T>& line, const TVector3<T>& vector))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 5.0);
	l1 = Line3(v1, v2);
	TEST_EQUAL(isOrthogonal(l1, v3), true)
	TEST_EQUAL(isOrthogonal(l1, v1), false)
RESULT

CHECK(isOrthogonal(const TLine3<T>& a, const TLine3<T>& b))
	v1.set(0.0, 0.0, 0.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 5.0);
	l1 = Line3(v1, v2);
	l2 = Line3(v1, v3);
	TEST_EQUAL(isOrthogonal(l1, l2), true)
	v3.set(0.0, 120.0, 5.0);
	l2 = Line3(v1, v3);
	TEST_EQUAL(isOrthogonal(l1, l2), false)
RESULT

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

CHECK(isIntersecting(const TVector3<T>& point, const TPlane3<T>& plane))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 6.0);  
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(v1, p1), true)
	v1.set(0.0, 0.0, 0.0);
	TEST_EQUAL(isIntersecting(v1, p1), false)
RESULT

CHECK(isIntersecting(const TPlane3<T>& plane, const TVector3<T>& point))
	v1.set(1.0, 2.0, 3.0);
	v2.set(0.0, 5.0, 0.0);
	v3.set(0.0, 0.0, 6.0);  
	p1 = Plane3(v1, v2, v3);
	TEST_EQUAL(isIntersecting(p1, v1), true)
	v1.set(0.0, 0.0, 0.0);
	TEST_EQUAL(isIntersecting(p1, v1), false)
RESULT

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

CHECK(TAngle<T> getOrientedAngle<T>(T ax, T ay, T az, T bx, T by, T bz, T nx, T ny, T nz))
	// BAUSTELLE
RESULT

CHECK(TAngle<T> getOrientedAngle<T>(const TVector<T>& a, const TVector3<T>& b, const TVector3<T>& normal))
	// BAUSTELLE
RESULT

END_TEST
