// $Id: Socket_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
// insert includes here
///////////////////////////

START_TEST(Socket, "$Id: Socket_test.C,v 1.1 1999/08/26 08:02:36 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
///  insert tests for each member function here         
///
	
CHECK("testname")			// a string describing the tested function

TEST_EQUAL(0,0)				// each test should be one of the macros (see above): 
											//   TEST_EQUAL(expression1, expression2)
											//   TEST_REAL_EQUAL(expression1, expression2)
											//   TEST_NOT_EQUAL(expression1, expression2)
                      //   TEST_EXCEPTION(exception_class, expression)

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
