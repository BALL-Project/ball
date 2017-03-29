// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/RMSDMinimizer.h>

///////////////////////////

START_TEST(RMSDMinimizer)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

typedef RMSDMinimizer::PointVector Points;

CHECK(RMSDMinimizer::Result RMSDMinimizer::computeTransformation(const AtomBijection& ab)
    throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates))
	// ????
RESULT

CHECK(RMSDMinimizer::Result RMSDMinimizer::computeTransformation
    (const RMSDMinimizer::PointVector& X, const RMSDMinimizer::PointVector& Y)
    throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates))
	Points A;
	Points B;

	TEST_EXCEPTION(RMSDMinimizer::TooFewCoordinates, RMSDMinimizer::computeTransformation(A, B))

	A.push_back(Vector3(0.0, 0.0, 0.0));
	TEST_EXCEPTION(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::computeTransformation(A, B))
	B.push_back(Vector3(0.0, 2.0, 0.0));
	TEST_EXCEPTION(RMSDMinimizer::TooFewCoordinates, RMSDMinimizer::computeTransformation(A, B))
	
	A.push_back(Vector3(0.0, 0.0, 1.0));
	TEST_EXCEPTION(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::computeTransformation(A, B))
	B.push_back(Vector3(0.0, 2.0, 1.0));
	TEST_EXCEPTION(RMSDMinimizer::TooFewCoordinates, RMSDMinimizer::computeTransformation(A, B))

	A.push_back(Vector3(3.0, 0.0, 1.0));
	TEST_EXCEPTION(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::computeTransformation(A, B))
	B.push_back(Vector3(3.0, 2.0, 1.0));
	RMSDMinimizer::Result r = RMSDMinimizer::computeTransformation(A, B);
	PRECISION(1e-3)
	TEST_REAL_EQUAL(r.second, 0.0)
	STATUS("RMSD: " << r.second)
	STATUS("T:\n" << r.first)
	TEST_REAL_EQUAL(r.first.m11, 1.0)
	TEST_REAL_EQUAL(r.first.m12, 0.0)
	TEST_REAL_EQUAL(r.first.m13, 0.0)
	TEST_REAL_EQUAL(r.first.m14, 0.0)
	TEST_REAL_EQUAL(r.first.m21, 0.0)
	TEST_REAL_EQUAL(r.first.m22, 1.0)
	TEST_REAL_EQUAL(r.first.m23, 0.0)
	TEST_REAL_EQUAL(r.first.m24, 2.0)
	TEST_REAL_EQUAL(r.first.m31, 0.0)
	TEST_REAL_EQUAL(r.first.m32, 0.0)
	TEST_REAL_EQUAL(r.first.m33, 1.0)
	TEST_REAL_EQUAL(r.first.m34, 0.0)
	TEST_REAL_EQUAL(r.first.m41, 0.0)
	TEST_REAL_EQUAL(r.first.m42, 0.0)
	TEST_REAL_EQUAL(r.first.m43, 0.0)
	TEST_REAL_EQUAL(r.first.m44, 1.0)
RESULT

CHECK(RMSDMinimizer::Reszlt RMSDMinimizer::computeTransformation
    (const RMSDMinimizer::PointVector& X, const RMSDMinimizer::PointVector& Y)
    throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates))
	vector<Vector3> vs1, vs2;
	
	vs1.push_back(Vector3(25.861,	3.886,	34.880));
	vs1.push_back(Vector3(27.128,	3.019,	34.851));
	vs1.push_back(Vector3(27.781,	3.086,	36.096));
	vs1.push_back(Vector3(30.318,   3.114,  35.247));
	vs1.push_back(Vector3(31.422,   4.794,  34.532));
	vs1.push_back(Vector3(30.314,   4.429,  35.143));		
	vs1.push_back(Vector3(32.569,	7.979,	33.549));
	vs1.push_back(Vector3(32.867,	7.345,	32.521));
	vs1.push_back(Vector3(31.964,	7.456,	34.484));
	
	vs2.push_back(Vector3(25.861,   3.886,  34.880));
	vs2.push_back(Vector3(27.126,   3.020,  34.851));
	vs2.push_back(Vector3(27.526,   2.665,  36.160));
	vs2.push_back(Vector3(30.319,   3.546,  35.391));
	vs2.push_back(Vector3(31.422,   5.105,  34.317));
	vs2.push_back(Vector3(30.304,   4.842,  35.018));
	vs2.push_back(Vector3(32.521,   8.000,  33.476));
	vs2.push_back(Vector3(32.575,   7.569,  32.307));
	vs2.push_back(Vector3(32.080,   7.387,  34.471));
		
	RMSDMinimizer::Result r = RMSDMinimizer::computeTransformation(vs1, vs2);	
	Matrix4x4 m = r.first;
	
	double rmsd = 0;
	for (unsigned int i = 0; i < 9; ++i)
	{
			Vector3 v1 = m * vs1[i];
			Vector3 v2 = vs2[i];
			rmsd += v1.getSquareDistance(v2);
	}	

	rmsd = sqrt(rmsd/9);		
	TEST_REAL_EQUAL(rmsd, r.second)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
