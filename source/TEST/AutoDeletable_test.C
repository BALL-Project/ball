// $Id: AutoDeletable_test.C,v 1.2 2001/07/15 15:30:41 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/autoDeletable.h>

///////////////////////////

using namespace BALL;

class A
	: public AutoDeletable
{
};

START_TEST(AutoDeletable, "$Id: AutoDeletable_test.C,v 1.2 2001/07/15 15:30:41 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


A* a_ptr;
A* array_ptr;

CHECK(AutoDeletable::void* operator new(size_t size) throw())
	a_ptr = new A;
	array_ptr = new A[12];	
RESULT

A a;
a_ptr = new A;
A array[12];
array_ptr = new A[12];

CHECK(AutoDeletable::~AutoDeletable())
	delete a_ptr;
	delete [] array_ptr;
RESULT

a_ptr = new A;
array_ptr = new A[12];

CHECK(AutoDeletable::isAutoDeletable() const )
	TEST_EQUAL(a.isAutoDeletable(), false)
	TEST_EQUAL(a_ptr->isAutoDeletable(), true)
	TEST_EQUAL(array_ptr->isAutoDeletable(), false)
	TEST_EQUAL(array->isAutoDeletable(), false)
RESULT

CHECK(AutoDeletable::setAutoDeletable(bool enable))
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
