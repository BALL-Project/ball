// $Id: GlobalTypes_test.C,v 1.1 2000/12/13 16:44:38 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/common.h>

///////////////////////////


// Verify that the globally defined data types (from
// COMMON/global.h) have the correct size on all platforms.
// This is required for portable persistence.

START_TEST(sizes of the global data types, "$Id: GlobalTypes_test.C,v 1.1 2000/12/13 16:44:38 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(size of Distance)
	TEST_EQUAL(sizeof(Distance), 4)
RESULT											

CHECK(size of Handle)
	TEST_EQUAL(sizeof(Handle), 4)
RESULT											

CHECK(size of Index)
	TEST_EQUAL(sizeof(Index), 4)
RESULT											

CHECK(size of Size)
	TEST_EQUAL(sizeof(Size), 4)
RESULT											

CHECK(size of Time)
	TEST_EQUAL(sizeof(Size), sizeof(time_t))
RESULT											

CHECK(size of HashIndex)
	TEST_EQUAL(sizeof(HashIndex), 4)
RESULT											

CHECK(size of Position)
	TEST_EQUAL(sizeof(Position), 4)
RESULT											

CHECK(size of Real)
	TEST_EQUAL(sizeof(Real), 4)
RESULT											

CHECK(size of DoubleReal)
	TEST_EQUAL(sizeof(DoubleReal), 8)
RESULT											

CHECK(size of Property)
	TEST_EQUAL(sizeof(Property), 4)
RESULT											

CHECK(size of ErrorCode)
	TEST_EQUAL(sizeof(ErrorCode), 4)
RESULT											

CHECK(size of Byte)
	TEST_EQUAL(sizeof(Byte), 1)
RESULT											

CHECK(size of PointerSizeInt)
	TEST_EQUAL(sizeof(PointerSizeInt), 8)
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
