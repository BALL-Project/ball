// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/autoDeletable.h>

///////////////////////////

using namespace BALL;

class A
	: public AutoDeletable
{
};

START_TEST(AutoDeletable)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


A* a_ptr;
A* array_ptr;

CHECK(void* operator new(size_t size) throw())
	a_ptr = new A;
	array_ptr = new A[12];	
RESULT

A a;
A array[12];

CHECK(~AutoDeletable() throw())
	delete a_ptr;
	delete [] array_ptr;
RESULT

a_ptr = new A;
array_ptr = new A[12];

CHECK(bool isAutoDeletable() const throw())
	TEST_EQUAL(a.isAutoDeletable(), false)
	TEST_EQUAL(a_ptr->isAutoDeletable(), true)
	TEST_EQUAL(array_ptr->isAutoDeletable(), false)
	TEST_EQUAL(array->isAutoDeletable(), false)
RESULT

CHECK(void setAutoDeletable(bool enable) throw())
	TEST_EQUAL(a.isAutoDeletable(), false)
	a.setAutoDeletable(true);
	TEST_EQUAL(a.isAutoDeletable(), true)
	a.setAutoDeletable(false);
	TEST_EQUAL(a.isAutoDeletable(), false)
	TEST_EQUAL(a_ptr->isAutoDeletable(), true)
	a_ptr->setAutoDeletable(false);
	TEST_EQUAL(a_ptr->isAutoDeletable(), false)
	a_ptr->setAutoDeletable(true);
	TEST_EQUAL(a_ptr->isAutoDeletable(), true)
RESULT

delete a_ptr;
delete [] array_ptr;

CHECK(void operator delete(void* ptr) throw())
  // ???
RESULT

CHECK(void operator delete(void* ptr, void*) throw())
  // ???
RESULT

CHECK(void* operator new(size_t size, void* ptr) throw())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
