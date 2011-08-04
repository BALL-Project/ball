// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/factory.h>

///////////////////////////

START_TEST(Factory)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(static const T& getDefault())
	Size& def(const_cast<Size&>(Factory<Size>::getDefault()));
	def = 1234;
	TEST_EQUAL(Factory<Size>::getDefault(), def);
	def = 3456;
	TEST_EQUAL(Factory<Size>::getDefault(), def);
RESULT											

CHECK(static T* create())
	Size* ptr = Factory<Size>::create();
	TEST_NOT_EQUAL(ptr, 0)
	Size* ptr2 = Factory<Size>::create();
	TEST_NOT_EQUAL(ptr, 0)
	TEST_NOT_EQUAL(ptr, ptr2)
	delete ptr;
	delete ptr2;
RESULT											

CHECK(static void* createVoid())
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
