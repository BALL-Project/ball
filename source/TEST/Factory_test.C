// $Id: Factory_test.C,v 1.1.2.1 2002/11/30 16:27:21 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/factory.h>

///////////////////////////

START_TEST(Factory, "$Id: Factory_test.C,v 1.1.2.1 2002/11/30 16:27:21 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(Factory::getDefault())
	const Size& c_def = Factory<int>::getDefault();
	Size& def = const_cast<Size&>(c_def);
	def = 1234;
	TEST_EQUAL(1234, Factory<int>::getDefault());
	def = 3456;
	TEST_EQUAL(3456, Factory<int>::getDefault());
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
