// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/iterator.h>
#include <BALL/CONCEPT/predicate.h>
#include <BALL/KERNEL/standardPredicates.h>

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

class True
	:	public UnaryPredicate<Composite>
{
	public:
	virtual bool operator () (const Composite&) const throw() { return true; }
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
	TEST_EQUAL(t.isEnd(), true)
RESULT

CHECK(Composite& getData() throw())
	CompositeIteratorTraits cit(a);
	Composite* composite = &cit.getData();
	TEST_EQUAL((composite == &a), true)
RESULT

CHECK(Composite::CompositeIterator& getPosition() throw())
	CompositeIteratorTraits cit(a);
	Composite* composite = &cit.getData();
	TEST_EQUAL((composite == &a), true)
RESULT

CHECK(CompositeIteratorTraits& operator = (const CompositeIteratorTraits& traits) throw())
	CompositeIteratorTraits cit1(a);
	CompositeIteratorTraits cit2;
	cit2 = cit1;
	TEST_EQUAL((cit1 == cit2), true)
RESULT

CHECK(CompositeIteratorTraits(const CompositeIteratorTraits& traits) throw())
	CompositeIteratorTraits cit1(a);
	CompositeIteratorTraits cit2(cit1);
	TEST_EQUAL((cit1 == cit2), true)
RESULT

CHECK(void toBegin() throw(Exception::Precondition))
	CompositeIteratorTraits cit(a);
	MyPred p(c);
	cit.setPredicate(p);
	cit.forward();
	TEST_EQUAL((&cit.getData() == &c), true)
	cit.toBegin();
	TEST_EQUAL((&cit.getData() == &c), true)
RESULT

CHECK(void toEnd() throw(Exception::Precondition))
	CompositeIteratorTraits cit(a);
	cit.toEnd();
	// TEST_EQUAL((BLUBB == &f), true)
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
	TEST_EQUAL(t.isBegin(), true)
	t.toEnd();
	TEST_EQUAL(t.isEnd(), true)
	TEST_EQUAL(t.isBegin(), false)

	True my_true;
	t.setPredicate(my_true);
	t.toBegin();
	TEST_EQUAL(t.isEnd(), false)
	TEST_EQUAL(t.isBegin(), true)
	t.toEnd();
	TEST_EQUAL(t.isEnd(), true)
	TEST_EQUAL(t.isBegin(), false)

	Composite empty;
	t = CompositeIteratorTraits(empty);
	t.setPredicate(my_true);
	t.toBegin();
	TEST_EQUAL(t.isEnd(), false)
	TEST_EQUAL(t.isBegin(), true)
	t.forward();
	TEST_EQUAL(t.isEnd(), true)
	TEST_EQUAL(t.isBegin(), false)
	t.backward();
	TEST_EQUAL(t.isEnd(), false)
	TEST_EQUAL(t.isBegin(), true)
RESULT

CHECK(bool isRBegin() const throw())
	CompositeIteratorTraits cit(a);
	MyPred p(c);
	cit.setPredicate(p);
	TEST_EQUAL(cit.isRBegin(), false)
	cit.toRBegin();
	TEST_EQUAL(cit.isRBegin(), true)
RESULT

CHECK(bool isREnd() const throw())
	CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	cit.forward();
	TEST_EQUAL(cit.isREnd(), false)
	cit.toREnd();
	TEST_EQUAL(cit.isREnd(), true)
RESULT

CHECK(bool isSingular() const throw())
	CompositeIteratorTraits cit1;
	TEST_EQUAL(cit1.isSingular(), true)
	CompositeIteratorTraits cit2(a);
	TEST_EQUAL(cit2.isSingular(), false)
RESULT

CHECK(bool isValid() const throw())
	CompositeIteratorTraits cit1;
	TEST_EQUAL(cit1.isValid(), false)
	CompositeIteratorTraits cit2(a);
	TEST_EQUAL(cit2.isValid(), true)
RESULT

CHECK(bool operator != (const CompositeIteratorTraits& traits) const throw())
	CompositeIteratorTraits cit1(a);
	True tp;
	cit1.setPredicate(tp);
	CompositeIteratorTraits cit2;
	cit2.setPredicate(tp);
	TEST_EQUAL((cit1 != cit2), true)
	CompositeIteratorTraits cit3(a);
	cit3.setPredicate(tp);
	TEST_EQUAL((cit1 != cit3), false)
RESULT

CHECK(bool operator == (const CompositeIteratorTraits& traits) const throw())
	CompositeIteratorTraits cit1(a);
	True tp;
	cit1.setPredicate(tp);
	CompositeIteratorTraits cit2;
	cit2.setPredicate(tp);
	TEST_EQUAL((cit1 == cit2), false)
	CompositeIteratorTraits cit3(a);
	cit3.setPredicate(tp);
	TEST_EQUAL((cit1 == cit3), true)
RESULT

CHECK(const Composite& getData() const throw())
	CompositeIteratorTraits cit(a);
	const Composite& composite = cit.getData();
	TEST_EQUAL((&composite == &a), true)
RESULT

CHECK(const Composite* getContainer() const throw())
	CompositeIteratorTraits t;
	const Composite* composite_const_ptr = t.getContainer();
	TEST_EQUAL(composite_const_ptr, 0)
RESULT

CHECK(const Composite::CompositeIterator& getPosition() const throw())
	CompositeIteratorTraits cit(a);
	const Composite* composite_const_ptr = &cit.getData();
	TEST_EQUAL((composite_const_ptr == &a), true)
RESULT

CHECK(const UnaryPredicate<Composite>* getPredicate() const throw())
	CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	const UnaryPredicate<Composite>* predicate_const_ptr = cit.getPredicate();
	TEST_EQUAL(predicate_const_ptr, &tp)
RESULT

CHECK(void backward() throw())
	CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	TEST_EQUAL((&cit.getData() == &a), true)
	cit.toRBegin();
	cit.backward();
	TEST_EQUAL((&cit.getData() == &f), true)
RESULT

CHECK(void forward() throw())
	CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	TEST_EQUAL((&cit.getData() == &a), true)
	cit.forward();
	TEST_EQUAL((&cit.getData() == &b), true)
	MyPred f_predicate(f);
	cit.setPredicate(f_predicate);
	cit.forward();
	TEST_EQUAL((&cit.getData() == &f), true)
RESULT

CHECK(void invalidate() throw())
	CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	TEST_EQUAL(cit.isValid(), true)
	cit.invalidate();
	TEST_EQUAL(cit.isValid(), false)
RESULT

CHECK(void setPredicate(const UnaryPredicate<Composite>& predicate) throw())
	CompositeIteratorTraits cit(a);
	MyPred c_predicate(c);
	cit.setPredicate(c_predicate);
	TEST_EQUAL(cit.getPredicate(), &c_predicate)
RESULT

CHECK(void toRBegin() throw(Exception::Precondition))
  CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	TEST_EQUAL(cit.isRBegin(), false)
	cit.toRBegin();
	TEST_EQUAL(cit.isRBegin(), true)
RESULT

CHECK(void toREnd() throw(Exception::Precondition))
  CompositeIteratorTraits cit(a);
	True tp;
	cit.setPredicate(tp);
	TEST_EQUAL(cit.isREnd(), true)
	cit.forward();
	TEST_EQUAL(cit.isREnd(), false)
	cit.toREnd();
	TEST_EQUAL(cit.isREnd(), true)
RESULT


CHECK([EXTRA] Iteration)
	Composite ac1;
	Composite ac2;
	ac1.appendChild(ac2);
	Composite a1;
	Composite a2;
	ac2.appendChild(a1);
	ac1.appendChild(a2);
	True my_true;

	STATUS(" ac1 = " << &ac1)
	STATUS(" ac2 = " << &ac2)
	STATUS(" a1  = " << &a1)
	STATUS(" a2  = " << &a2)
	
	CompositeIteratorTraits t1(ac1);
	t1.setPredicate(my_true);
	t1.toBegin();
	while (!t1.isEnd())
	{
		TEST_EQUAL((ac1.isAncestorOf(t1.getData()) || (&t1.getData() == &ac1)), true)
		STATUS(" - " << &t1.getData())
		t1.forward();
	}

	CompositeIteratorTraits t2(ac2);
	t2.setPredicate(my_true);
	t2.toBegin();
	while (!t2.isEnd())
	{
		STATUS(" - " << &t2.getData())
		TEST_EQUAL((ac2.isAncestorOf(t2.getData()) || (&t2.getData() == &ac2)), true)
		t2.forward();
	}

	Composite single;
	CompositeIteratorTraits t3(single);
	t3.setPredicate(my_true);
	t3.toBegin();
	TEST_EQUAL(t3.isBegin(), true)
	TEST_EQUAL(t3.isEnd(), false)
	TEST_EQUAL(t3.isValid(), true)
	TEST_EQUAL(t3.isSingular(), false)
	TEST_EQUAL(&t3.getData(), &single)
	t3.forward();
	TEST_EQUAL(t3.isEnd(), true)
	TEST_EQUAL(t3.isBegin(), false)
	TEST_EQUAL(t3.isValid(), false)
	TEST_EQUAL(t3.isSingular(), false)
	TEST_EQUAL(&t3.getData(), 0)
	
	Composite root;
	root.appendChild(single);
	CompositeIteratorTraits t4(single);
	t4.setPredicate(my_true);
	t4.toBegin();
	TEST_EQUAL(t4.isBegin(), true)
	TEST_EQUAL(t4.isEnd(), false)
	TEST_EQUAL(t4.isValid(), true)
	TEST_EQUAL(t4.isSingular(), false)
	TEST_EQUAL(&t4.getData(), &single)
	t4.forward();
	TEST_EQUAL(t4.isEnd(), true)
	TEST_EQUAL(t4.isBegin(), false)
	TEST_EQUAL(t4.isValid(), false)
	TEST_EQUAL(t4.isSingular(), false)
	TEST_EQUAL(&t4.getData(), 0)	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST

