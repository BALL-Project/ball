// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: CompositeCompositeIteratorTraits_test.C,v 1.1 2003/06/19 10:45:51 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>

///////////////////////////

using namespace BALL;

START_TEST(Composite::CompositeIteratorTraits, "$Id: CompositeCompositeIteratorTraits_test.C,v 1.1 2003/06/19 10:45:51 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Composite::CompositeIteratorTraits* cit_ptr = 0;
CHECK(CompositeIteratorTraits() throw())
	cit_ptr = new Composite::CompositeIteratorTraits;
	TEST_NOT_EQUAL(cit_ptr, 0)
RESULT

CHECK(~CompositeIteratorTraits() throw())
	delete cit_ptr;
RESULT

CHECK(Composite* getContainer() throw())
	Composite::CompositeIteratorTraits t;
	TEST_EQUAL(t.getContainer(), 0)
RESULT

Composite a;
Composite b;
Composite c;
Composite d;
Composite e;
Composite f;
a.appendChild(b);
a.appendChild(c);
a.appendChild(d);
c.appendChild(e);
c.appendChild(f);

CHECK(CompositeIteratorTraits(const Composite& composite) throw())
	Composite::CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)
	STATUS(" &a = " << (void*)&a)
	STATUS(" &b = " << (void*)&b)
	STATUS(" &c = " << (void*)&c)
	STATUS(" &d = " << (void*)&d)
	STATUS(" &e = " << (void*)&e)
	STATUS(" &f = " << (void*)&f)
	
	t.toBegin();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &a)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &b)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &c)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &e)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &f)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &d)
	t.forward();
	TEST_EQUAL(t.isValid(), false)
	TEST_EQUAL(t.isEnd(), true)
RESULT

CHECK(Composite& getData() throw())
	Composite::CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)
	TEST_EQUAL(a.isSelected(), false)
	t.toBegin();
	t.getData().select();
	TEST_EQUAL(a.isSelected(), true)
	a.deselect();
RESULT

CHECK(CompositeIteratorPosition& getPosition() throw())
  // ???
RESULT

CHECK(CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) throw())
  // ???
RESULT

CHECK(CompositeIteratorTraits(const CompositeIteratorTraits& traits) throw())
	Composite::CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)
	STATUS(" &a = " << (void*)&a)
	STATUS(" &b = " << (void*)&b)
	STATUS(" &c = " << (void*)&c)
	STATUS(" &d = " << (void*)&d)
	STATUS(" &e = " << (void*)&e)
	STATUS(" &f = " << (void*)&f)
	
	t.toBegin();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &a)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &b)
	t.forward();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &c)

	Composite::CompositeIteratorTraits t2(t);
	TEST_EQUAL(t2.isValid(), true)
	TEST_EQUAL(&t2.getData(), &c)
	t2.forward();
	TEST_EQUAL(t2.isValid(), true)
	TEST_EQUAL(&t2.getData(), &e)
	t2.forward();
	TEST_EQUAL(t2.isValid(), true)
	TEST_EQUAL(&t2.getData(), &f)
	t2.forward();
	TEST_EQUAL(t2.isValid(), true)
	TEST_EQUAL(&t2.getData(), &d)
	t2.forward();
	TEST_EQUAL(t2.isValid(), false)
	TEST_EQUAL(t2.isEnd(), true)

	// make sure the two traits are still
	// independent
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &c)
RESULT

CHECK(void toBegin() throw())
  // ???
RESULT

CHECK(void toEnd() throw())
  // ???
RESULT

CHECK(bool isBegin() const throw())
	Composite::CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)

	t.toBegin();
	TEST_EQUAL(t.isBegin(), true)
	t.forward();
	TEST_EQUAL(t.isBegin(), false)
	t.backward();
	TEST_EQUAL(t.isBegin(), true)
	t.toEnd();
	TEST_EQUAL(t.isBegin(), false)
RESULT

CHECK(bool isEnd() const throw())
	Composite::CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)

	t.toBegin();
	TEST_EQUAL(t.isEnd(), false)
	t.forward();
	TEST_EQUAL(t.isEnd(), false)
	t.toEnd();
	TEST_EQUAL(t.isEnd(), true)
	t.backward();
	TEST_EQUAL(t.isEnd(), false)
RESULT

CHECK(bool isRBegin() const throw())
  // ???
RESULT

CHECK(bool isREnd() const throw())
  // ???
RESULT

CHECK(bool isSingular() const throw())
	Composite::CompositeIteratorTraits t1(a);
	TEST_EQUAL(t1.isSingular(), false);
	Composite::CompositeIteratorTraits t2;
	TEST_EQUAL(t2.isSingular(), true);
	t2 = t1;
	TEST_EQUAL(t2.isSingular(), false);
RESULT

CHECK(bool isValid() const throw())
	Composite::CompositeIteratorTraits t1;
	TEST_EQUAL(t1.isValid(), false)
	// ???
RESULT

CHECK(bool operator != (const CompositeIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(bool operator == (const CompositeIteratorTraits& traits) const throw())
  // ???
RESULT

CHECK(const Composite& getData() const throw())
  // ???
RESULT

CHECK(const Composite* getContainer() const throw())
  // ???
RESULT

CHECK(const CompositeIteratorPosition& getPosition() const throw())
  // ???
RESULT

CHECK(void backward() throw())
  // ???
RESULT

CHECK(void forward() throw())
  // ???
RESULT

CHECK(void invalidate() throw())
  // ???
RESULT

CHECK(void toRBegin() throw())
  // ???
RESULT

CHECK(void toREnd() throw())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

