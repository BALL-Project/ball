// $Id: AutoDeletable_test.C,v 1.1 2001/07/05 17:48:12 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/autoDeletable.h>

///////////////////////////

using namespace BALL;

class A: public AutoDeletable
{
};

START_TEST(class_name, "$Id: AutoDeletable_test.C,v 1.1 2001/07/05 17:48:12 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


A* a_ptr;
A* array_ptr;

CHECK(AutoDeletable::void* operator new(size_t size) throw())
	a_ptr = new A;
	array_ptr = new A[12];	
RESULT

CHECK(AutoDeletable::void* operator new(size_t size, void* ptr) throw())
	// BAUSTELLE
RESULT

CHECK(AutoDeletable::void operator delete(void* ptr) throw())
	// BAUSTELLE
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
  //BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
