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

Composite* c_ptr = 0;
CHECK(Composite() throw())
	c_ptr = new Composite;
	TEST_NOT_EQUAL(c_ptr, 0)
RESULT

CHECK(bool isValid() const throw())
	TEST_EQUAL(c_ptr->isValid(), true)
RESULT

CHECK(~Composite() throw())
	delete c_ptr;
RESULT

CHECK(Size getDegree() const throw())
	Composite a;
	TEST_EQUAL(a.getDegree(), 0)
RESULT

CHECK(void prependChild(Composite& composite) throw())
	Composite a, b, c, d;
	TEST_EQUAL(a.getDegree(), 0)
	TEST_EQUAL(b.getDegree(), 0)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
	a.prependChild(b);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 0)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
	b.prependChild(c);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 1)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
	b.prependChild(d);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
RESULT

CHECK(void appendChild(Composite& composite) throw())
	Composite a, b, c, d;
	TEST_EQUAL(a.getDegree(), 0)
	TEST_EQUAL(b.getDegree(), 0)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
	a.appendChild(b);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 0)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
	b.appendChild(c);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 1)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
	b.appendChild(d);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(d.getDegree(), 0)
RESULT

CHECK(Size countDescendants() const throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	TEST_EQUAL(a.countDescendants(), 4)
	TEST_EQUAL(b.countDescendants(), 3)
	TEST_EQUAL(c.countDescendants(), 1)
	TEST_EQUAL(d.countDescendants(), 0)
	TEST_EQUAL(e.countDescendants(), 0)
RESULT

CHECK(void clear() throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	c.clear();
	TEST_EQUAL(a.countDescendants(), 3)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(a.getDegree(), 1)
RESULT

CHECK(void destroy() throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	c.destroy();
	TEST_EQUAL(a.countDescendants(), 2)
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(b.getDegree(), 1)
	TEST_EQUAL(a.getDegree(), 1)
RESULT

CHECK(void destroy(bool virtual_destroy) throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	c.destroy(true);
	TEST_EQUAL(c.getDegree(), 0)
	TEST_EQUAL(b.getDegree(), 1)
	TEST_EQUAL(a.getDegree(), 1)
RESULT

CHECK(Composite(const Composite& composite, bool deep = true) throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	Composite f(a);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(c.getDegree(), 1)
	TEST_EQUAL(d.getDegree(), 0)
	TEST_EQUAL(e.getDegree(), 0)
	TEST_EQUAL(f.getDegree(), 1)
	TEST_EQUAL(f.countDescendants(), 4)
	Composite g(a, false);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(c.getDegree(), 1)
	TEST_EQUAL(d.getDegree(), 0)
	TEST_EQUAL(e.getDegree(), 0)
	TEST_EQUAL(g.getDegree(), 0)
	TEST_EQUAL(g.countDescendants(), 0)
RESULT

CHECK(void* clone(Composite& root) const throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	Composite f;
	a.clone(f);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(c.getDegree(), 1)
	TEST_EQUAL(d.getDegree(), 0)
	TEST_EQUAL(e.getDegree(), 0)
	TEST_EQUAL(f.getDegree(), 1)
	TEST_EQUAL(f.countDescendants(), 4)
RESULT

// Inherited from Selectable  - just to make sure it's there!
CHECK([EXTRA] bool isSelected() throw())
	Composite a;
	TEST_EQUAL(a.isSelected(), false)
RESULT

CHECK(void select() throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	TEST_EQUAL(a.isSelected(), false);
	TEST_EQUAL(b.isSelected(), false);
	TEST_EQUAL(c.isSelected(), false);
	TEST_EQUAL(d.isSelected(), false);
	TEST_EQUAL(e.isSelected(), false);
	e.select();
	TEST_EQUAL(a.isSelected(), false);
	TEST_EQUAL(b.isSelected(), false);
	TEST_EQUAL(c.isSelected(), true);
	TEST_EQUAL(d.isSelected(), false);
	TEST_EQUAL(e.isSelected(), true);
	a.select();
	TEST_EQUAL(a.isSelected(), true);
	TEST_EQUAL(b.isSelected(), true);
	TEST_EQUAL(c.isSelected(), true);
	TEST_EQUAL(d.isSelected(), true);
	TEST_EQUAL(e.isSelected(), true);
RESULT

CHECK(void deselect() throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	TEST_EQUAL(a.isSelected(), false);
	TEST_EQUAL(b.isSelected(), false);
	TEST_EQUAL(c.isSelected(), false);
	TEST_EQUAL(d.isSelected(), false);
	TEST_EQUAL(e.isSelected(), false);
	a.select();
	TEST_EQUAL(a.isSelected(), true);
	TEST_EQUAL(b.isSelected(), true);
	TEST_EQUAL(c.isSelected(), true);
	TEST_EQUAL(d.isSelected(), true);
	TEST_EQUAL(e.isSelected(), true);
	a.deselect();
	TEST_EQUAL(a.isSelected(), false);
	TEST_EQUAL(b.isSelected(), false);
	TEST_EQUAL(c.isSelected(), false);
	TEST_EQUAL(d.isSelected(), false);
	TEST_EQUAL(e.isSelected(), false);
	a.select();
	e.deselect();
	TEST_EQUAL(a.isSelected(), false);
	TEST_EQUAL(b.isSelected(), false);
	TEST_EQUAL(c.isSelected(), false);
	TEST_EQUAL(d.isSelected(), true);
	TEST_EQUAL(e.isSelected(), false);
RESULT

CHECK([EXTRA] Composite forward iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  b.appendChild(c);
  b.appendChild(d);
  c.appendChild(e);
	Composite::CompositeIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = a.beginComposite();
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
	sub_it = f.beginComposite();
	TEST_EQUAL(sub_it.isValid(), true)
	TEST_EQUAL(&*sub_it, &f)
	++sub_it;
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == f.endComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)

	Composite g;
	Composite h;
	g.appendChild(h);
	sub_it = h.beginComposite();
	TEST_EQUAL(sub_it.isValid(), true)
	TEST_EQUAL(&*sub_it, &h)
	++sub_it;
	TEST_EQUAL(sub_it.isValid(), false)
	TEST_EQUAL(sub_it == h.endComposite(), true)
	TEST_PRECONDITION_EXCEPTION(++sub_it)
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
