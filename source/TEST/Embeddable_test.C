// $Id: Embeddable_test.C,v 1.1 2000/07/16 16:07:55 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/embeddable.h>
#include <BALL/common.h>

using namespace BALL;

///////////////////////////

class A 
	: public Embeddable
{
	public:
	BALL_EMBEDDABLE(A)

};

class B
{
	public:
};

class C : public B, public A
{
	public:
	BALL_EMBEDDABLE(C)

};

START_TEST(Embeddable, "$Id")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// avoid nasty error messages when 
// testing multiple registrateion/deregistration
Log.remove(std::cout);

// tests for class Embeddable::

CHECK(Embeddable::~Embeddable())
  A* a_ptr = new A;
	TEST_NOT_EQUAL(a_ptr, 0)
	delete a_ptr;
RESULT

CHECK(Embeddable::countInstances())
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 0)
RESULT											

A* a_ptr = 0;
C* c_ptr = 0;
CHECK(Embeddable::registerThis() throw())
  a_ptr = new A;
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 0)
	a_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 0)
	a_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 0)
	c_ptr = new C;
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 0)
	c_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 1)
	c_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 1)
RESULT

CHECK(Embeddable::unregisterThis() throw())
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 1)
	a_ptr->unregisterThis();
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 1)
	a_ptr->unregisterThis();
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 1)
	delete a_ptr;
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 1)
	delete c_ptr;
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 0)
RESULT

CHECK(Embeddable::getInstance(Position index))
	A a1;
	A a2;
	A a3;
	a3.registerThis();
	a2.registerThis();
	a1.registerThis();
	TEST_EQUAL(A::countInstances(), 3)
	TEST_EQUAL(A::getInstance(0), &a3)
	TEST_EQUAL(A::getInstance(1), &a2)
	TEST_EQUAL(A::getInstance(2), &a1)
	TEST_EQUAL(A::getInstance(3), 0)
	TEST_EQUAL(A::getInstance(1234), 0)
RESULT											





/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

