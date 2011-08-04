// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
	BALL_EMBEDDABLE(A, Embeddable)
};

class B
{
	public:
};

class C 
	: public B, 
		public A
{
	public:
	BALL_EMBEDDABLE(C, A)
};

START_TEST(Embeddable)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// avoid nasty error messages when 
// testing multiple registrateion/deregistration
Log.remove(std::cout);

CHECK(~Embeddable() throw())
  A* a_ptr = new A;
	TEST_NOT_EQUAL(a_ptr, 0)
	delete a_ptr;
RESULT

CHECK(Embeddable(const Embeddable& embeddable) throw())
  // ???
RESULT

CHECK(Embeddable(const String& identifier = "<Embeddable>") throw())
  // ???
RESULT

CHECK(void setIdentifier(const String& identifier) throw())
  // ???
RESULT

CHECK(const String& getIdentifier() const throw())
  // ???
RESULT

CHECK([EXTRA] Size countInstances() throw())
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 0)
RESULT											

A* a_ptr = 0;
C* c_ptr = 0;
CHECK(void registerThis() throw())
  a_ptr = new A;
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 0)
	a_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 1)
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
	TEST_EQUAL(A::countInstances(), 2)
	TEST_EQUAL(C::countInstances(), 1)
	c_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 2)
	TEST_EQUAL(C::countInstances(), 1)
	c_ptr->registerThis();
	TEST_EQUAL(A::countInstances(), 2)
	TEST_EQUAL(C::countInstances(), 1)
RESULT

CHECK(void unregisterThis() throw())
	TEST_EQUAL(A::countInstances(), 2)
	TEST_EQUAL(C::countInstances(), 1)
	a_ptr->unregisterThis();
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 1)
	a_ptr->unregisterThis();
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 1)
	delete a_ptr;
	TEST_EQUAL(A::countInstances(), 1)
	TEST_EQUAL(C::countInstances(), 1)
	delete c_ptr;
	TEST_EQUAL(A::countInstances(), 0)
	TEST_EQUAL(C::countInstances(), 0)
RESULT

CHECK([EXTRA] getInstance(Position index))
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

CHECK([EXTRA] getInstance(const String& identifier))
	A a1;
	a1.setIdentifier(String("One"));
	A a2;
	a2.setIdentifier(String("Two"));
	A a3;
	a3.setIdentifier(String("Three"));
	a3.registerThis();
	a2.registerThis();
	a1.registerThis();
	STATUS(" &a1 = " << &a1)
	STATUS(" &a2 = " << &a2)
	STATUS(" &a3 = " << &a3)
	TEST_EQUAL(A::countInstances(), 3)
	TEST_EQUAL(A::getInstance("Three"), &a3)
	TEST_EQUAL(A::getInstance("Two"), &a2)
	TEST_EQUAL(A::getInstance("One"), &a1)
	TEST_EQUAL(A::getInstance("Four"), 0)
RESULT											

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
