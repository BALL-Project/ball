// $Id: Factory_test.C,v 1.2 2002/12/12 11:34:40 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/factory.h>

///////////////////////////

START_TEST(Factory, "$Id: Factory_test.C,v 1.2 2002/12/12 11:34:40 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Factory::getDefault())
	Size& def(const_cast<Size&>(Factory<Size>::getDefault()));
	def = 1234;
	TEST_EQUAL(Factory<Size>::getDefault(), def);
	def = 3456;
	TEST_EQUAL(Factory<Size>::getDefault(), def);
RESULT											

CHECK(Factory::create())
	Size* ptr = Factory<Size>::create();
	TEST_NOT_EQUAL(ptr, 0)
	Size* ptr2 = Factory<Size>::create();
	TEST_NOT_EQUAL(ptr, 0)
	TEST_NOT_EQUAL(ptr, ptr2)
	delete ptr;
	delete ptr2;
RESULT											

CHECK(Factory::createVoid())
	Size* ptr = (Size*)Factory<Size>::create();
	TEST_NOT_EQUAL(ptr, 0)
	Size* ptr2 = (Size*)Factory<Size>::create();
	TEST_NOT_EQUAL(ptr, 0)
	TEST_NOT_EQUAL(ptr, ptr2)
	delete ptr;
	delete ptr2;	
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
