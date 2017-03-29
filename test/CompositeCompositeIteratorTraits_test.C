// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>

///////////////////////////

START_TEST(class_name)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	
Composite::CompositeIteratorTraits* ccitp = 0;

CHECK(CompositeIteratorTraits() throw())
	ccitp = new Composite::CompositeIteratorTraits;
	TEST_NOT_EQUAL(ccitp, 0)
RESULT

CHECK(~CompositeIteratorTraits() throw())
	delete ccitp;
RESULT

bool test;

CHECK(CompositeIteratorTraits(const Composite& composite) throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits(composite);
	test = (traits.getContainer() == &composite);
	TEST_EQUAL(test, true)
RESULT

CHECK(CompositeIteratorTraits(const CompositeIteratorTraits& traits) throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits1(composite);
	Composite::CompositeIteratorTraits traits2(traits1);
	test = (traits1.getContainer() == traits2.getContainer());
	TEST_EQUAL(test, true)
RESULT

CHECK(bool isValid() const throw())
	Composite::CompositeIteratorTraits traits;
	test = traits.isValid();
	TEST_EQUAL(test, false)
RESULT

CHECK(CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits1(composite);
	Composite::CompositeIteratorTraits traits2 = traits1;
	test = (traits1.getContainer() == traits2.getContainer());
	TEST_EQUAL(test, true)
RESULT

CHECK(Composite* getContainer() throw())
	// Partially tested above.
	Composite::CompositeIteratorTraits traits;
	test = (traits.getContainer() == 0);
	TEST_EQUAL(test, true)
RESULT

CHECK(const Composite* getContainer() const throw())
	// Same implementation as the non-const version.
RESULT

CHECK(bool isSingular() const throw())
	Composite::CompositeIteratorTraits traits;
	test = traits.isSingular();
	TEST_EQUAL(test, true)
RESULT

CHECK(Composite* getPosition() throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits(composite);
	test = (traits.getPosition() == 0);
	TEST_EQUAL(test, true)
	traits.toBegin();
	test = (traits.getPosition() == &composite);
	TEST_EQUAL(test, true)
RESULT

CHECK(const Composite* getPosition() const throw())
	// Same implementation as non-const version.
RESULT

CHECK(void setPosition(Composite* position) throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits(composite);
	traits.setPosition(&composite);
	test = (traits.getPosition() == &composite);
	TEST_EQUAL(test, true)
RESULT

CHECK(Composite& getData() throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits(composite);
	traits.toBegin();
	test = (&traits.getData() == &composite);
	TEST_EQUAL(test, true)
RESULT

CHECK(const Composite& getData() const throw())
	// Same implementation as non-const version.
RESULT

CHECK(bool operator == (const CompositeIteratorTraits& traits) const throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits1(composite);
	Composite::CompositeIteratorTraits traits2(composite);
	test = (traits1 == traits2);
	TEST_EQUAL(test, true)
	traits1.toBegin();
	test = (traits1 == traits2);
	TEST_NOT_EQUAL(test, true)
RESULT

CHECK(bool operator != (const CompositeIteratorTraits& traits) const throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits1(composite);
	Composite::CompositeIteratorTraits traits2(composite);
	test = (traits1 != traits2);
	TEST_NOT_EQUAL(test, true)
	traits1.toBegin();
	test = (traits1 != traits2);
	TEST_EQUAL(test, true)
RESULT

CHECK(void invalidate() throw())
	Composite composite;
	Composite::CompositeIteratorTraits traits(composite);
	traits.toBegin();
	traits.invalidate();
	test = traits.isValid();
	TEST_EQUAL(test, false)
RESULT

// Build a composite tree for testing purposes.
/*

            0
            |
            |
            |
            1
           /|\
          / | \
         /  |  \
        2   4   5
       /        |\
      /         | \
     /          |  \
    3           6   7
*/


std::vector<Composite*> tree;

for (Size i = 0; i <= 7; ++i) 
{
	Composite* composite = new Composite;
	tree.push_back(composite);
}

tree[0]->appendChild(*tree[1]);
tree[1]->appendChild(*tree[2]);
tree[1]->appendChild(*tree[4]);
tree[1]->appendChild(*tree[5]);
tree[2]->appendChild(*tree[3]);
tree[5]->appendChild(*tree[6]);
tree[5]->appendChild(*tree[7]);

CHECK(void toBegin() throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toBegin();
	test = (traits.getPosition() == tree[1]);
	TEST_EQUAL(test, true)
RESULT

CHECK(bool isBegin() const throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toBegin();
	test = traits.isBegin();
	TEST_EQUAL(test, true)
RESULT

CHECK(void toEnd() throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toBegin();
	traits.toEnd();
	test = (traits.getPosition() == 0);
	TEST_EQUAL(test, true)
RESULT

CHECK(bool isEnd() const throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toBegin();
	traits.toEnd();
	test = traits.isEnd();
	TEST_EQUAL(test, true)
RESULT

CHECK(void toRBegin() throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toRBegin();
	test = (traits.getPosition() == tree[7]);
	TEST_EQUAL(test, true)
RESULT

CHECK(bool isRBegin() const throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toRBegin();
	test = traits.isRBegin();
	TEST_EQUAL(test, true)
RESULT

CHECK(void toREnd() throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toREnd();
	test = (traits.getPosition() == tree[1]);
	TEST_EQUAL(test, true)
RESULT

CHECK(bool isREnd() const throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toREnd();
	test = traits.isREnd();
	TEST_EQUAL(test, true)
RESULT

CHECK(void forward() throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toBegin();
	
	for (Size i = 1; i < 7; ++i)
	{
		test = (traits.getPosition() == tree[i]);
		TEST_EQUAL(test, true)
		traits.forward();
		test = (traits.getPosition() == tree[i+1]);
		TEST_EQUAL(test, true)
	}
RESULT

CHECK(void backward() throw())
	Composite::CompositeIteratorTraits traits(*tree[1]);
	traits.toEnd();
	traits.backward();
	
	for (Size i = 7; i > 1; --i)
	{
		test = (traits.getPosition() == tree[i]);
		TEST_EQUAL(test, true)
		traits.backward();
		test = (traits.getPosition() == tree[i-1]);
		TEST_EQUAL(test, true)
	}
RESULT

delete tree[0];

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
