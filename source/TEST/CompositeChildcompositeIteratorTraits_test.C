// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/iterator.h>
#include <BALL/CONCEPT/predicate.h>

///////////////////////////

using namespace BALL;

class MyPred
	:	public UnaryPredicate<Composite>
{
	public:
	MyPred(const Composite& my_comp)
		:	composite_(&my_comp)
	{
	}

	virtual bool operator () (const Composite& composite) const throw()
	{
		return (&composite == composite_);
	}
	
	const Composite* composite_;
};

START_TEST(CompositeIteratorTraits)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CompositeIteratorTraits* cit_ptr = 0;
CHECK(CompositeIteratorTraits() throw())
	cit_ptr = new CompositeIteratorTraits;
	TEST_NOT_EQUAL(cit_ptr, 0)
RESULT

CHECK(~CompositeIteratorTraits() throw())
	delete cit_ptr;
RESULT

CHECK(Composite* getContainer() throw())
	CompositeIteratorTraits t;
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
	CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)
	MyPred p(c);

	TEST_EQUAL(p(a), false)
	TEST_EQUAL(p(b), false)
	TEST_EQUAL(p(c), true)
	TEST_EQUAL(p(d), false)
	TEST_EQUAL(p(e), false)
	TEST_EQUAL(p(f), false)

	STATUS(" &a = " << (void*)&a)
	STATUS(" &b = " << (void*)&b)
	STATUS(" &c = " << (void*)&c)
	STATUS(" &d = " << (void*)&d)
	STATUS(" &e = " << (void*)&e)
	STATUS(" &f = " << (void*)&f)
	
	t.setPredicate(p);
	t.toBegin();
	TEST_EQUAL(t.isValid(), true)
	TEST_EQUAL(&t.getData(), &c)
	t.forward();
	TEST_EQUAL(t.isValid(), false)
	TEST_EQUAL(&t.getData(), 0)
	TEST_EQUAL(t.getPredicate(), &p)
RESULT

CHECK(Composite& getData() throw())
  // ???
RESULT

CHECK(Composite::SubcompositeIterator& getPosition() throw())
  // ???
RESULT

CHECK(CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) throw())
  // ???
RESULT

CHECK(CompositeIteratorTraits(const CompositeIteratorTraits& traits) throw())
  // ???
RESULT

CHECK(void toBegin() throw(Exception::Precondition))
  // ???
RESULT

CHECK(void toEnd() throw(Exception::Precondition))
  // ???
RESULT

CHECK(bool isBegin() const throw())
	CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)
	MyPred p(c);
	t.setPredicate(p);

	t.toBegin();
	TEST_EQUAL(t.isBegin(), true)
	t.forward();
	TEST_EQUAL(t.isBegin(), false)
RESULT

CHECK(bool isEnd() const throw())
	CompositeIteratorTraits t(a);
	TEST_EQUAL(t.getContainer(), &a)
	MyPred p(c);
	t.setPredicate(p);

	t.toBegin();
	TEST_EQUAL(t.isEnd(), false)
	t.toEnd();
	TEST_EQUAL(t.isEnd(), true)
RESULT

CHECK(bool isRBegin() const throw())
  // ???
RESULT

CHECK(bool isREnd() const throw())
  // ???
RESULT

CHECK(bool isSingular() const throw())
  // ???
RESULT

CHECK(bool isValid() const throw())
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

CHECK(const Composite::SubcompositeIterator& getPosition() const throw())
  // ???
RESULT

CHECK(const UnaryPredicate<Composite>* getPredicate() const throw())
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

CHECK(void setPredicate(const UnaryPredicate<Composite>& predicate) throw())
  // ???
RESULT

CHECK(void toRBegin() throw(Exception::Precondition))
  // ???
RESULT

CHECK(void toREnd() throw(Exception::Precondition))
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

