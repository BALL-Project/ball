// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(Composite)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK([EXTRA] Composite forward const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeConstIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = const_cast<const Composite&>(a).beginComposite();
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &a)
	++sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &d)
	sub_it++;
  TEST_EQUAL(+sub_it, false)
  TEST_EQUAL(sub_it == a.endComposite(), true)

	// Iteration over an empty container
	Composite f;
	sub_it = const_cast<const Composite&>(f).beginComposite();
	TEST_EQUAL(sub_it.isValid(), true)
	TEST_EQUAL(&*sub_it, &f)
	++sub_it;
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == const_cast<const Composite&>(f).endComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)

	Composite g;
	Composite h;
	g.appendChild(h);
	sub_it = const_cast<const Composite&>(h).beginComposite();
	TEST_EQUAL(sub_it.isValid(), true)
	TEST_EQUAL(&*sub_it, &h)
	++sub_it;
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == const_cast<const Composite&>(h).endComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)
RESULT

CHECK([EXTRA] Composite backward iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = a.endComposite();
  TEST_EQUAL(sub_it.isValid(), false)
	--sub_it;
	TEST_EQUAL(sub_it.isValid(), true)
  TEST_EQUAL(&*sub_it, &d)
	--sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &a)
	sub_it--;
  TEST_EQUAL(+sub_it, false)
RESULT

CHECK([EXTRA] Composite backward const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeConstIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = const_cast<const Composite&>(a).endComposite();
  TEST_EQUAL(sub_it.isValid(), false)
	--sub_it;
	TEST_EQUAL(sub_it.isValid(), true)
  TEST_EQUAL(&*sub_it, &d)
	--sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &a)
	sub_it--;
  TEST_EQUAL(+sub_it, false)
RESULT

CHECK([EXTRA] Composite forward reverse iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = a.rbeginComposite();
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	++sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &a)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(sub_it == a.rendComposite(), true)
RESULT

CHECK([EXTRA] Composite forward reverse const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeConstReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = const_cast<const Composite&>(a).rbeginComposite();
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	++sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &a)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(sub_it == const_cast<const Composite&>(a).rendComposite(), true)
RESULT

CHECK([EXTRA] Composite backward reverse iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = a.rendComposite();
  TEST_EQUAL(sub_it.base().isValid(), true)
	--sub_it;
	TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &a)
	--sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	TEST_EQUAL(sub_it == a.rbeginComposite(), true)
RESULT

CHECK([EXTRA] Composite backward reverse const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeConstReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = const_cast<const Composite&>(a).rendComposite();
  TEST_EQUAL(sub_it.base().isValid(), true)
	--sub_it;
	TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &a)
	--sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &e)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	TEST_EQUAL(sub_it == const_cast<const Composite&>(a).rbeginComposite(), true)
RESULT

CHECK([EXTRA] Ancestor forward iteration)
  Composite a, b, c, d, e;
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::AncestorIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = e.beginAncestor();
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	++sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &a)
	sub_it++;
  TEST_EQUAL(sub_it == a.endAncestor(), true)
RESULT

CHECK([EXTRA] Ancestor forward const iteration)
  Composite a, b, c, d, e;
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::AncestorConstIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = const_cast<const Composite&>(e).beginAncestor();
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	++sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &a)
	sub_it++;
  TEST_EQUAL(+sub_it, false)
  TEST_EQUAL(sub_it == a.endAncestor(), true)
RESULT

CHECK([EXTRA] ChildComposite forward iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = a.beginChildComposite();
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	++sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &d)
	sub_it++;
  TEST_EQUAL(+sub_it, false)
  TEST_EQUAL(sub_it == a.endChildComposite(), true)

	// Iteration over an empty container
	Composite f;
	sub_it = f.beginChildComposite();
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == f.endChildComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)

	Composite g;
	Composite h;
	g.appendChild(h);
	sub_it = h.beginChildComposite();
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == h.endChildComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)
RESULT

CHECK([EXTRA] ChildComposite forward const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeConstIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = const_cast<const Composite&>(a).beginChildComposite();
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	++sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &d)
	sub_it++;
  TEST_EQUAL(+sub_it, false)
  TEST_EQUAL(sub_it == a.endChildComposite(), true)

	// Iteration over an empty container
	Composite f;
	sub_it = const_cast<const Composite&>(f).beginChildComposite();
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == const_cast<const Composite&>(f).endChildComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)

	Composite g;
	Composite h;
	g.appendChild(h);
	sub_it = const_cast<const Composite&>(h).beginChildComposite();
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == const_cast<const Composite&>(h).endChildComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
