// $Id: BitVector_test.C,v 1.1 1999/12/17 11:16:50 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/bitVector.h>

///////////////////////////

START_TEST(BitVector, "$Id: BitVector_test.C,v 1.1 1999/12/17 11:16:50 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(BitVector::setSize())
BitVector b(8);
b.setBit(3);
TEST_EQUAL(b.getSize(), 8)
b.setSize(24, true);
TEST_EQUAL(b.getSize(), 24)
b.setSize(24, false);
TEST_EQUAL(b.getSize(), 24)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
