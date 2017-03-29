// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
#include "ItemCollector.h"
///////////////////////////

using namespace BALL;
using namespace std;

class myVisitor
	: public  Visitor<class Composite>
{
	public:
	Composite* c_ptr;
	void visit(Composite& composite)
	{
		c_ptr = &composite;
	}
};


START_TEST(Composite)
Composite a, b, c, d, e, f;
a.appendChild(b);
b.appendChild(c);
b.appendChild(d);
c.appendChild(e);

CHECK(bool removeChild(Composite& child) throw())
	TEST_EQUAL(b.removeChild(d), true)
	TEST_EQUAL(d.getParent(), 0)
	TEST_EQUAL(b.getChild(1), 0)
	b.appendChild(d);
	f.clear();
	TEST_EQUAL(f.getFirstChild(), 0)
	TEST_EQUAL(f.removeChild(d), false)
RESULT

CHECK(void replace(Composite& composite) throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);

	b.replace(b);

	Composite a1, b1, c1, d1, e1;
	a1.appendChild(b1);
	b1.appendChild(c1);
	b1.appendChild(d1);
	c1.appendChild(e1);
	b1.replace(b);

	TEST_EQUAL(a.getFirstChild(), 0)
	TEST_EQUAL(b.getParent(), &a1)
	TEST_EQUAL(c.getParent(), &b)

	TEST_EQUAL(a1.getFirstChild(), &b)
	TEST_EQUAL(b1.getParent(), 0)
	TEST_EQUAL(c1.getParent(), &b1)
RESULT

CHECK(void swap(Composite& composite) throw())
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);

	Composite a1, b1, c1, d1, e1;
	a1.appendChild(b1);
	b1.appendChild(c1);
	b1.appendChild(d1);
	c1.appendChild(e1);

	a.swap(e);
	TEST_EQUAL(b.getParent(), &a)
	TEST_EQUAL(c.getParent(), &b)
	TEST_EQUAL(e.getParent(), &c)

	b1.swap(b);
	TEST_EQUAL(a.getFirstChild(), &b1)
	TEST_EQUAL(a1.getFirstChild(), &b)
	TEST_EQUAL(b.getParent(), &a1)
	TEST_EQUAL(b1.getParent(), &a)

	TEST_EQUAL(c.getParent(), &b1)
	TEST_EQUAL(c1.getParent(), &b)
RESULT

CHECK(template<typename T> bool hasAncestor(const T& dummy ) const throw())
	TEST_EQUAL(a.hasAncestor(b), false)
	TEST_EQUAL(b.hasAncestor(c), true)

	Chain chain;
	Protein protein, protein2;
	System system, system2;

	protein.insert(chain);
	TEST_EQUAL(chain.hasAncestor(protein2), true)
	TEST_EQUAL(chain.hasAncestor(system2), false)

	system.insert(protein);
	TEST_EQUAL(chain.hasAncestor(protein2), true)
	TEST_EQUAL(chain.hasAncestor(system2), true)
	TEST_EQUAL(protein.hasAncestor(protein2), false)
	TEST_EQUAL(protein.hasAncestor(system2), true)
RESULT

CHECK(bool isAncestorOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isAncestorOf(b), true)
	TEST_EQUAL(a.isAncestorOf(d), true)
	TEST_EQUAL(a.isAncestorOf(e), true)
	TEST_EQUAL(b.isAncestorOf(b), false)
	TEST_EQUAL(e.isAncestorOf(b), false)
RESULT

CHECK(bool isRelatedWith(const Composite& composite) const throw())
	TEST_EQUAL(a.isRelatedWith(a), true)
	TEST_EQUAL(a.isRelatedWith(b), true)
	TEST_EQUAL(b.isRelatedWith(a), true)
	TEST_EQUAL(a.isRelatedWith(d), true)
	TEST_EQUAL(d.isRelatedWith(a), true)
	TEST_EQUAL(a.isRelatedWith(e), true)
	TEST_EQUAL(e.isRelatedWith(a), true)
	TEST_EQUAL(c.isRelatedWith(d), false)
	TEST_EQUAL(d.isRelatedWith(c), false)
	TEST_EQUAL(d.isRelatedWith(f), false)
	TEST_EQUAL(f.isRelatedWith(c), false)
RESULT

CHECK(bool isHomomorph(const Composite& composite) const throw())
	f.set(a);
	TEST_EQUAL(f.isHomomorph(a), true)
	TEST_EQUAL(a.isHomomorph(f), true)
	TEST_EQUAL(a.isHomomorph(a), true)
	TEST_EQUAL(a.isHomomorph(b), false)
	f.clear();
	TEST_EQUAL(f.isHomomorph(a), false)
RESULT

CHECK(void dump(std::ostream& s = std::cout, Size depth = 0) const throw())
	TEST_EQUAL(a.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), std::ios::out);
	a.dump(outfile);
	outfile.close();
	TEST_FILE_REGEXP(filename.c_str(), BALL_TEST_DATA_PATH(Composite_test.txt))
RESULT

CHECK(void host(Visitor<Composite>& visitor) throw(Exception::GeneralException))
	myVisitor mv;
	a.host(mv);
	TEST_EQUAL(mv.c_ptr, &a)
RESULT

ItemCollector<Composite> myproc;

CHECK(template<typename T> bool applyAncestor(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	a.applyAncestor(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)
	e.applyAncestor(myproc);
	TEST_EQUAL(myproc.getSize(), 3)
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyChild(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.applyChild(myproc);
	TEST_EQUAL(myproc.getSize(), 0)

	a.applyChild(myproc);
	TEST_EQUAL(myproc.getSize(), 1)
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)

	b.applyChild(myproc);
	TEST_EQUAL(myproc.getSize(), 2)
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyDescendantPreorder(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.applyDescendantPreorder(myproc);
	TEST_EQUAL(myproc.getSize(), 0)

	a.applyDescendantPreorder(myproc);
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyDescendantPostorder(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.applyDescendantPostorder(myproc);
	TEST_EQUAL(myproc.getSize(), 0)

	a.applyDescendantPostorder(myproc);
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyDescendant(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.applyDescendant(myproc);
	TEST_EQUAL(myproc.getSize(), 0)

	a.applyDescendant(myproc);
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyPreorder(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.applyPreorder(myproc);
	TEST_EQUAL(myproc.getSize(), 1)

	a.applyPreorder(myproc);
	TEST_EQUAL(myproc.getSize(), 5)
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyPostorder(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.applyPostorder(myproc);
	TEST_EQUAL(myproc.getSize(), 1)
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();

	a.applyPostorder(myproc);
	TEST_EQUAL(myproc.getSize(), 5)

	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool apply(UnaryProcessor<T>& processor) throw(Exception::GeneralException))
	e.apply(myproc);
	TEST_EQUAL(myproc.getSize(), 1)

	a.apply(myproc);
	TEST_EQUAL(myproc.getSize(), 5)
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(template<typename T> bool applyLevel(UnaryProcessor<T>& processor, long level) throw(Exception::GeneralException))
	e.applyLevel(myproc,0 );
	TEST_EQUAL(myproc.getSize(), 1)
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();

	b.applyLevel(myproc, 1);
	TEST_EQUAL(myproc.getSize(), 2)
	TEST_EQUAL(myproc.getPointer(), &c)
	myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d)
	myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)

	b.applyLevel(myproc, 2);
	TEST_EQUAL(myproc.getSize(), 1)
	TEST_EQUAL(myproc.getPointer(), &e)
	myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(void set(const Composite& composite, bool deep = true) throw())
	Composite x;
	x.set(a, true);
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(Composite& operator = (const Composite& composite) throw())
	Composite x;
	x = a;
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(void get(Composite& composite, bool deep = true) const throw())
	Composite x;
	a.get(x);
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(Size count(const KernelPredicateType& predicate) const throw())
	TEST_EQUAL(a.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_EQUAL(e.count(Composite::DEFAULT_UNARY_PREDICATE), 1)
RESULT

CHECK(bool operator == (const Composite& composite) const throw())
	Composite a, e;
	TEST_EQUAL(a == e, false)
	e = a;
	TEST_EQUAL(a == e, false)
	TEST_EQUAL(a == a, true)
RESULT

CHECK(bool operator != (const Composite& composite) const throw())
	Composite a, e;
	TEST_EQUAL(a != e, true)
	e = a;
	TEST_EQUAL(a != e, true)
	TEST_EQUAL(a != a, false)
RESULT

CHECK([EXTRA] memory leaks...)
	Composite* ptr = 0;
	for (Size i = 0; i < 1e6; i++)
	{
		ptr = new Composite;
		if (!(i % 1000)) { STATUS("constructed " << i  << " composites.") }
		delete ptr;
	}
RESULT

CHECK(Size removeSelected() throw())
	Composite a, b, c, d, e, f;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);

	TEST_EQUAL(a.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	c.select();
	a.removeSelected();
	TEST_EQUAL(a.count(Composite::DEFAULT_UNARY_PREDICATE), 3)
	TEST_EQUAL(e.getParent(), 0)
RESULT

END_TEST
