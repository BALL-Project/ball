// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/chain.h>
///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(Composite)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Composite a, b, c, d, e, f;
a.appendChild(b);
b.appendChild(c);
b.appendChild(d);
c.appendChild(e);

const Composite& c_a = a;
const Composite& c_b = b;
const Composite& c_c = c;
const Composite& c_d = d;
const Composite& c_e = e;
const Composite& c_f = f;
CHECK(bool isDescendantOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isDescendantOf(a), false)
	TEST_EQUAL(b.isDescendantOf(a), true)
	TEST_EQUAL(c.isDescendantOf(a), true)
	TEST_EQUAL(d.isDescendantOf(a), true)
	TEST_EQUAL(e.isDescendantOf(a), true)
	TEST_EQUAL(f.isDescendantOf(a), false)
	TEST_EQUAL(a.isDescendantOf(b), false)
	TEST_EQUAL(b.isDescendantOf(b), false)
	TEST_EQUAL(c.isDescendantOf(b), true)
	TEST_EQUAL(d.isDescendantOf(b), true)
	TEST_EQUAL(e.isDescendantOf(b), true)
	TEST_EQUAL(f.isDescendantOf(b), false)
	TEST_EQUAL(a.isDescendantOf(c), false)
	TEST_EQUAL(b.isDescendantOf(c), false)
	TEST_EQUAL(c.isDescendantOf(c), false)
	TEST_EQUAL(d.isDescendantOf(c), false)
	TEST_EQUAL(e.isDescendantOf(c), true)
	TEST_EQUAL(f.isDescendantOf(c), false)
	TEST_EQUAL(a.isDescendantOf(d), false)
	TEST_EQUAL(b.isDescendantOf(d), false)
	TEST_EQUAL(c.isDescendantOf(d), false)
	TEST_EQUAL(d.isDescendantOf(d), false)
	TEST_EQUAL(e.isDescendantOf(d), false)
	TEST_EQUAL(f.isDescendantOf(d), false)
RESULT

CHECK(Composite* getLowestCommonAncestor(const Composite& composite) throw())
	TEST_EQUAL(d.getLowestCommonAncestor(e), &b)
	TEST_EQUAL(d.getLowestCommonAncestor(f), 0)
	TEST_EQUAL(f.getLowestCommonAncestor(f), &f)
RESULT

CHECK(const Composite* getLowestCommonAncestor(const Composite& composite) const throw())
	TEST_EQUAL(c_d.getLowestCommonAncestor(e), &b)
	TEST_EQUAL(c_d.getLowestCommonAncestor(f), 0)
	TEST_EQUAL(c_f.getLowestCommonAncestor(f), &f)
RESULT

CHECK(template<typename T> T* getAncestor(const T& /* dummy */) throw())
	TEST_EQUAL(a.getAncestor(b), 0)
	TEST_EQUAL(f.getAncestor(b), 0)
	TEST_EQUAL(e.getAncestor(b), &c)

	Chain chain;
	Protein protein, protein2;
	System system, system2;

	protein.insert(chain);
	TEST_EQUAL(chain.getAncestor(protein2), &protein)
	TEST_EQUAL(chain.getAncestor(system2), 0)

	system.insert(protein);
	TEST_EQUAL(chain.getAncestor(protein2), &protein)
	TEST_EQUAL(chain.getAncestor(system2), &system)
	TEST_EQUAL(protein.getAncestor(protein2), 0)
	TEST_EQUAL(protein.getAncestor(system2), &system)
RESULT

CHECK(template<typename T> const T* getAncestor(const T& /* dummy */) const throw())
	TEST_EQUAL(c_a.getAncestor(b), 0)
	TEST_EQUAL(c_f.getAncestor(b), 0)
	TEST_EQUAL(c_e.getAncestor(b), &c)
RESULT

CHECK(template<typename T> T* getPrevious(const T& /* dummy */) throw())
	// a single composite should return zero
	Composite* ptr = f.getPrevious(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(a.getPrevious(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(b.getPrevious(RTTI::getDefault<Composite>()), &a)
	TEST_EQUAL(c.getPrevious(RTTI::getDefault<Composite>()), &b)
	TEST_EQUAL(d.getPrevious(RTTI::getDefault<Composite>()), &e)
	TEST_EQUAL(e.getPrevious(RTTI::getDefault<Composite>()), &c)
RESULT

CHECK(template<typename T> const T* getPrevious(const T& dummy) const throw())
	// a single composite should return zero
	const Composite* ptr = c_f.getPrevious(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(c_a.getPrevious(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(c_b.getPrevious(RTTI::getDefault<Composite>()), &c_a)
	TEST_EQUAL(c_c.getPrevious(RTTI::getDefault<Composite>()), &c_b)
	TEST_EQUAL(c_d.getPrevious(RTTI::getDefault<Composite>()), &c_e)
	TEST_EQUAL(c_e.getPrevious(RTTI::getDefault<Composite>()), &c_c)
RESULT

CHECK(template<typename T> T* getNext(const T& /* dummy */) throw())
	// a single composite should return zero
	Composite* ptr = f.getNext(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(a.getNext(RTTI::getDefault<Composite>()), &c_b)
	TEST_EQUAL(b.getNext(RTTI::getDefault<Composite>()), &c_c)
	TEST_EQUAL(c.getNext(RTTI::getDefault<Composite>()), &c_e)
	TEST_EQUAL(d.getNext(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(e.getNext(RTTI::getDefault<Composite>()), &c_d)
RESULT

CHECK(template<typename T> const T* getNext(const T& dummy) const throw())
	// a single composite should return zero
	const Composite* ptr = c_f.getNext(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(c_a.getNext(RTTI::getDefault<Composite>()), &b)
	TEST_EQUAL(c_b.getNext(RTTI::getDefault<Composite>()), &c)
	TEST_EQUAL(c_c.getNext(RTTI::getDefault<Composite>()), &e)
	TEST_EQUAL(c_d.getNext(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(c_e.getNext(RTTI::getDefault<Composite>()), &d)
RESULT

CHECK(Composite* getParent() throw())
	TEST_EQUAL(a.getParent(), 0)
	TEST_EQUAL(e.getParent(), &c)
RESULT

CHECK(const Composite* getParent() const throw())
	TEST_EQUAL(c_a.getParent(), 0)
	TEST_EQUAL(c_e.getParent(), &c)
RESULT

CHECK(Composite* getChild(Index index) throw())
	TEST_EQUAL(a.getChild(1), 0)
	TEST_EQUAL(a.getChild(0), &b)
	TEST_EQUAL(b.getChild(1), &d)
	TEST_EQUAL(e.getChild(0), 0)
RESULT

CHECK(const Composite* getChild(Index index) const throw())
	TEST_EQUAL(c_a.getChild(1), 0)
	TEST_EQUAL(c_a.getChild(0), &b)
	TEST_EQUAL(c_b.getChild(1), &d)
	TEST_EQUAL(c_e.getChild(0), 0)
RESULT

CHECK(Composite* getSibling(Index index) throw())
	TEST_EQUAL(a.getSibling(0), &a)
	TEST_EQUAL(c.getSibling(-1), 0)
	TEST_EQUAL(c.getSibling(0), &c)
	TEST_EQUAL(c.getSibling(1), &d)
	TEST_EQUAL(d.getSibling(-1), &c)
	TEST_EQUAL(d.getSibling(0), &d)
	TEST_EQUAL(d.getSibling(1), 0)
	TEST_EQUAL(e.getSibling(0), &e)
RESULT

CHECK(const Composite* getSibling(Index index) const throw())
	TEST_EQUAL(c_a.getSibling(0), &a)
	TEST_EQUAL(c_c.getSibling(-1), 0)
	TEST_EQUAL(c_c.getSibling(0), &c)
	TEST_EQUAL(c_c.getSibling(1), &d)
	TEST_EQUAL(c_d.getSibling(-1), &c)
	TEST_EQUAL(c_d.getSibling(0), &d)
	TEST_EQUAL(c_d.getSibling(1), 0)
	TEST_EQUAL(c_e.getSibling(0), &e)
RESULT

CHECK(Composite* getFirstChild() throw())
	TEST_EQUAL(a.getFirstChild(), &b)
	TEST_EQUAL(b.getFirstChild(), &c)
	TEST_EQUAL(e.getFirstChild(), 0)
RESULT

CHECK(const Composite* getFirstChild() const throw())
	TEST_EQUAL(c_a.getFirstChild(), &b)
	TEST_EQUAL(c_b.getFirstChild(), &c)
	TEST_EQUAL(c_e.getFirstChild(), 0)
RESULT

CHECK(Composite* getLastChild() throw())
	TEST_EQUAL(a.getLastChild(), &b)
	TEST_EQUAL(b.getLastChild(), &d)
	TEST_EQUAL(e.getLastChild(), 0)
RESULT

CHECK(const Composite* getLastChild() const throw())
	TEST_EQUAL(c_a.getLastChild(), &b)
	TEST_EQUAL(c_b.getLastChild(), &d)
	TEST_EQUAL(c_e.getLastChild(), 0)
RESULT

CHECK(static bool insertParent(Composite& parent, Composite& first, Composite& last, bool destroy_parent = true) throw())
	{
		Composite a, b, c, d, e, f, x;
		a.appendChild(b);
		a.appendChild(c);
		a.appendChild(d);
		a.appendChild(e);
		d.appendChild(f);
		TEST_EQUAL(Composite::insertParent(x, c, d, true), true)
		TEST_EQUAL(a.getChild(0), &b)
		TEST_EQUAL(a.getChild(1), &x)
		TEST_EQUAL(a.getChild(2), &e)
		TEST_EQUAL(a.getChild(3), 0)

		TEST_EQUAL(x.getChild(0), &c)
		TEST_EQUAL(x.getChild(1), &d)
		TEST_EQUAL(x.getChild(2), 0)

		TEST_EQUAL(d.getChild(0), &f)
	}
	{
		Composite a, b, c, d, e, f, x;
		a.appendChild(b);
		a.appendChild(c);
		a.appendChild(d);
		a.appendChild(e);
		d.appendChild(f);
		TEST_EQUAL(Composite::insertParent(x, a, e, true), false)
		TEST_EQUAL(Composite::insertParent(x, e, x, true), false)
		TEST_EQUAL(Composite::insertParent(x, b, e, true), true)
		TEST_EQUAL(a.getChild(0), &x)
		TEST_EQUAL(a.getChild(1), 0)
		TEST_EQUAL(x.getChild(0), &b)
		TEST_EQUAL(x.getChild(1), &c)
		TEST_EQUAL(x.getChild(2), &d)
		TEST_EQUAL(x.getChild(3), &e)
		TEST_EQUAL(d.getChild(0), &f)
	}
RESULT

CHECK(void insertBefore(Composite& composite) throw())
	e.insertBefore(f);
	TEST_EQUAL(c.getFirstChild(), &f)
	TEST_EQUAL(c.getLastChild(), &e)
	TEST_EQUAL(f.getParent(), &c)
	c.removeChild(f);
RESULT

CHECK(void insertAfter(Composite& composite) throw())
	e.insertAfter(f);
	TEST_EQUAL(c.getFirstChild(), &e)
	TEST_EQUAL(c.getLastChild(), &f)
	TEST_EQUAL(f.getParent(), &c)
	c.removeChild(f);
RESULT

CHECK(void spliceBefore(Composite& composite) throw())
	Composite a, b, c, d, e, f;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);

	e.appendChild(f);

	e.spliceBefore(b);
	TEST_EQUAL(e.getChild(0), &c)
	TEST_EQUAL(e.getChild(1), &d)
	TEST_EQUAL(e.getChild(2), &f)
	TEST_EQUAL(b.getChild(0), 0)

	a.spliceBefore(a);
RESULT

CHECK(void spliceAfter(Composite& composite) throw())
	Composite a, b, c, d, e, f;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);

	e.appendChild(f);

	e.spliceAfter(b);
	TEST_EQUAL(e.getChild(0), &f)
	TEST_EQUAL(e.getChild(1), &c)
	TEST_EQUAL(e.getChild(2), &d)
	TEST_EQUAL(b.getChild(0), 0)

	a.spliceAfter(a);
RESULT

CHECK(void splice(Composite& composite) throw())
	{
		Composite a, b, c, d, e, f;
		a.appendChild(b);
		b.appendChild(c);
		b.appendChild(d);

		e.appendChild(f);

		e.splice(b);
		TEST_EQUAL(e.getChild(0), &c)
		TEST_EQUAL(e.getChild(1), &d)
		TEST_EQUAL(e.getChild(2), &f)
		TEST_EQUAL(b.getChild(0), 0)
	}
	{
		Composite a, b, c, d, e, f;
		a.appendChild(b);
		b.appendChild(c);
		b.appendChild(d);
		c.appendChild(e);
		b.splice(c);
		TEST_EQUAL(b.getChild(0), &e)
		TEST_EQUAL(b.getChild(1), &d)
	}

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
