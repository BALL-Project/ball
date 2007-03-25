// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
<<<<<<< Skeleton_test.C
// $Id: Skeleton_test.C,v 1.5.14.1 2007/03/25 21:48:59 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
=======
// $Id: Skeleton_test.C,v 1.5.14.1 2007/03/25 21:48:59 oliver Exp $
//
// Author:
//   Oliver Kohlbacher
>>>>>>> 1.4.10.1
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here

///////////////////////////

<<<<<<< Skeleton_test.C
START_TEST(class_name, "$Id: Skeleton_test.C,v 1.5.14.1 2007/03/25 21:48:59 oliver Exp $")
=======
START_TEST(class_name, "$Id: Skeleton_test.C,v 1.5.14.1 2007/03/25 21:48:59 oliver Exp $")
>>>>>>> 1.4.10.1

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
CHECK("testname")			// a string describing the tested function

TEST_EQUAL(0,0)				// each test should be one of the macros (see above): 
											//   TEST_EQUAL(expression1, expression2)
											//   TEST_REAL_EQUAL(expression1, expression2)
											//   TEST_NOT_EQUAL(expression1, expression2)
                      //   TEST_EXCEPTION(exception_class, expression)
                      //   TEST_FILE(infile, templatefile, use_regexps)

RESULT								// prints the result for the test
											// and updates the global variable
											// OK. If any test fails, OK is set
											// to false and the whole test suite
											// will terminate with FAILURE and exit
											// code 1. Otherwise it terminates
											// with OK and 0.
											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
