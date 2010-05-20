// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MATHS/LINALG/SVDSolver.h>
///////////////////////////

START_TEST(SVDSolver, "")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

SVDSolver<float>* svd_f_ptr;

CHECK(SVDSolver<float>())
	svd_f_ptr = new SVDSolver<float>();
	TEST_NOT_EQUAL(svd_f_ptr, 0)

	TEST_EXCEPTION(Exception::Uninitialized, svd_f_ptr->computeSVD());
RESULT

CHECK(~SVDSolver())
	delete svd_f_ptr;
RESULT
