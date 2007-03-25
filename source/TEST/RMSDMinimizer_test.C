// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RMSDMinimizer_test.C,v 1.1.18.1 2007/03/25 21:47:36 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/RMSDMinimizer.h>

///////////////////////////

START_TEST(RMSDMinimizer, "$Id: RMSDMinimizer_test.C,v 1.1.18.1 2007/03/25 21:47:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

typedef RMSDMinimizer::PointVector Points;

CHECK(RMSDMinimizer::Result RMSDMinimizer::computeTransformation(const AtomBijection& ab)
    throw(RMSDMinimizer::IncompatibleCoordinateSets, RMSDMinimizer::TooFewCoordinates))
	// ????
RESULT

CHECK(RMSDMinimizer::Reszlt RMSDMinimizer::computeTransformation
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
	PRECISION(0.0002)
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
