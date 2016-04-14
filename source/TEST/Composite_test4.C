// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <BALL/CONCEPT/visitor.h>
///////////////////////////

using namespace BALL;
using namespace std;

class myVisitor
	: public Visitor<class Composite>
{
	public:
	Composite* c_ptr;
	void visit(Composite& composite)
	{
		c_ptr = &composite;
	}
};

START_TEST(Composite)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK([EXTRA] ChildComposite backward iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = a.endChildComposite();
  TEST_EQUAL(sub_it.isValid(), false)
	--sub_it;
	TEST_EQUAL(sub_it.isValid(), true)
  TEST_EQUAL(&*sub_it, &d)
	--sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it--;
  TEST_EQUAL(+sub_it, false)
RESULT

CHECK([EXTRA] ChildComposite backward const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeConstIterator sub_it;
  TEST_EQUAL(+sub_it, false)
  sub_it = const_cast<const Composite&>(a).endChildComposite();
  TEST_EQUAL(sub_it.isValid(), false)
	--sub_it;
	TEST_EQUAL(sub_it.isValid(), true)
  TEST_EQUAL(&*sub_it, &d)
	--sub_it;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(+sub_it, true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it--;
  TEST_EQUAL(+sub_it, false)
RESULT

CHECK([EXTRA] ChildComposite forward reverse iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = a.rbeginChildComposite();
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	++sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(sub_it == a.rendChildComposite(), true)
RESULT

CHECK([EXTRA] ChildComposite forward reverse const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeConstReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = const_cast<const Composite&>(a).rbeginChildComposite();
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	++sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	sub_it++;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(sub_it == const_cast<const Composite&>(a).rendChildComposite(), true)
RESULT

CHECK([EXTRA] ChildComposite backward reverse iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = a.rendChildComposite();
  TEST_EQUAL(sub_it.base().isValid(), true)
	--sub_it;
	TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	--sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	TEST_EQUAL(sub_it == a.rbeginChildComposite(), true)
RESULT

CHECK([EXTRA] ChildComposite backward reverse const iteration)
  Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
  a.appendChild(b);
  a.appendChild(c);
  a.appendChild(d);
  c.appendChild(e);
	Composite::ChildCompositeConstReverseIterator sub_it;
  TEST_EQUAL(sub_it.base().isValid(), false)
  sub_it = const_cast<const Composite&>(a).rendChildComposite();
  TEST_EQUAL(sub_it.base().isValid(), true)
	--sub_it;
	TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &b)
	--sub_it;
  TEST_EQUAL(sub_it.base().isValid(), true)
  TEST_EQUAL(&*sub_it, &c)
	sub_it--;
  TEST_EQUAL(sub_it.base().isValid(), false)
  TEST_EQUAL(&*sub_it, &d)
	TEST_EQUAL(sub_it == const_cast<const Composite&>(a).rbeginChildComposite(), true)
RESULT

TextPersistenceManager  pm;
Composite composite;
composite.select();
String filename;

CHECK(void persistentWrite(PersistenceManager& pm, const char* name = 0) const throw(Exception::GeneralException))
	NEW_TMP_FILE(filename)
	std::ofstream  ofile(filename.c_str(), std::ios::out);
	pm.setOstream(ofile);
	using namespace RTTI;
	pm.registerClass(getStreamName<Composite>(), getNew<Composite>);
	composite >> pm;
	ofile.close();
RESULT

CHECK(void persistentRead(PersistenceManager& pm) throw(Exception::GeneralException))
	using namespace RTTI;
	std::ifstream  ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject* ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
        TEST_EQUAL(isKindOf<Composite>(ptr), true)
        if (isKindOf<Composite>(ptr))
		{
			Composite* pers_a = castTo<Composite>(*ptr);
			TEST_EQUAL(pers_a->isSelected(), true)
		}
		delete ptr;
	}
RESULT

CHECK(bool containsSelection() const throw())
	Composite a, b, c, d, e;
	Composite root;
	root.appendChild(a);
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	TEST_EQUAL(a.containsSelection(), false);
	TEST_EQUAL(b.containsSelection(), false);
	TEST_EQUAL(c.containsSelection(), false);
	TEST_EQUAL(d.containsSelection(), false);
	TEST_EQUAL(e.containsSelection(), false);
	e.select();
	TEST_EQUAL(a.containsSelection(), true);
	TEST_EQUAL(b.containsSelection(), true);
	TEST_EQUAL(c.containsSelection(), true);
	TEST_EQUAL(d.containsSelection(), false);
	TEST_EQUAL(e.containsSelection(), true);
RESULT

CHECK(Size getPathLength(const Composite& composite) const throw())
	Composite a, b, c, d, e, f;
	Composite root;
	root.appendChild(a);
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	TEST_EQUAL(a.getPathLength(a), 0)
	TEST_EQUAL(a.getPathLength(b), 1)
	TEST_EQUAL(b.getPathLength(a), 1)
	TEST_EQUAL(b.getPathLength(c), 1)
	TEST_EQUAL(c.getPathLength(b), 1)
	TEST_EQUAL(c.getPathLength(d), INVALID_SIZE)
	TEST_EQUAL(d.getPathLength(c), INVALID_SIZE)
	TEST_EQUAL(d.getPathLength(a), 2)
	TEST_EQUAL(a.getPathLength(d), 2)
	TEST_EQUAL(c.getPathLength(a), 2)
	TEST_EQUAL(a.getPathLength(c), 2)
	TEST_EQUAL(e.getPathLength(a), 3)
	TEST_EQUAL(a.getPathLength(e), 3)
	TEST_EQUAL(b.getPathLength(e), 2)
	TEST_EQUAL(e.getPathLength(b), 2)
	TEST_EQUAL(a.getPathLength(f), INVALID_SIZE)
	TEST_EQUAL(f.getPathLength(a), INVALID_SIZE)
RESULT

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

CHECK(Size getDepth() const throw())
	TEST_EQUAL(c_a.getDepth(), 0)
	TEST_EQUAL(c_b.getDepth(), 1)
	TEST_EQUAL(c_c.getDepth(), 2)
	TEST_EQUAL(c_d.getDepth(), 2)
	TEST_EQUAL(c_e.getDepth(), 3)
	TEST_EQUAL(c_f.getDepth(), 0)
RESULT

CHECK(Size getHeight() const throw())
	TEST_EQUAL(a.getHeight(), 3)
	TEST_EQUAL(b.getHeight(), 2)
	TEST_EQUAL(c.getHeight(), 1)
	TEST_EQUAL(d.getHeight(), 0)
	TEST_EQUAL(e.getHeight(), 0)
	TEST_EQUAL(f.getHeight(), 0)
RESULT

CHECK(Composite& getRoot() throw())
	TEST_EQUAL(&a.getRoot(), &a)
	TEST_EQUAL(&b.getRoot(), &a)
	TEST_EQUAL(&c.getRoot(), &a)
	TEST_EQUAL(&d.getRoot(), &a)
	TEST_EQUAL(&e.getRoot(), &a)
	TEST_EQUAL(&f.getRoot(), &f)
RESULT

CHECK(const Composite& getRoot() const throw())
	const Composite& c_a = a;
	const Composite& c_b = b;
	const Composite& c_c = c;
	const Composite& c_d = d;
	const Composite& c_e = e;
	const Composite& c_f = f;
	TEST_EQUAL(&c_a.getRoot(), &a)
	TEST_EQUAL(&c_b.getRoot(), &a)
	TEST_EQUAL(&c_c.getRoot(), &a)
	TEST_EQUAL(&c_d.getRoot(), &a)
	TEST_EQUAL(&c_e.getRoot(), &a)
	TEST_EQUAL(&c_f.getRoot(), &f)
RESULT

CHECK(bool isEmpty() const throw())
	TEST_EQUAL(a.isEmpty(), false)
	TEST_EQUAL(b.isEmpty(), false)
	TEST_EQUAL(c.isEmpty(), false)
	TEST_EQUAL(d.isEmpty(), true)
	TEST_EQUAL(e.isEmpty(), true)
	TEST_EQUAL(f.isEmpty(), true)
RESULT

CHECK(bool isRoot() const throw())
	TEST_EQUAL(a.isRoot(), true)
	TEST_EQUAL(b.isRoot(), false)
	TEST_EQUAL(c.isRoot(), false)
	TEST_EQUAL(d.isRoot(), false)
	TEST_EQUAL(e.isRoot(), false)
	TEST_EQUAL(f.isRoot(), true)
RESULT

CHECK(bool isRootOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isRootOf(a), true)
	TEST_EQUAL(a.isRootOf(b), true)
	TEST_EQUAL(b.isRootOf(a), false)
	TEST_EQUAL(c.isRootOf(a), false)
	TEST_EQUAL(d.isRootOf(a), false)
	TEST_EQUAL(e.isRootOf(a), false)
	TEST_EQUAL(f.isRootOf(a), false)
	TEST_EQUAL(f.isRootOf(f), true)
	TEST_EQUAL(a.isRootOf(b), true)
	TEST_EQUAL(a.isRootOf(c), true)
	TEST_EQUAL(a.isRootOf(d), true)
	TEST_EQUAL(a.isRootOf(e), true)
	TEST_EQUAL(a.isRootOf(f), false)
RESULT

CHECK(bool isInterior() const throw())
	TEST_EQUAL(a.isInterior(), false)
	TEST_EQUAL(b.isInterior(), true)
	TEST_EQUAL(c.isInterior(), true)
	TEST_EQUAL(d.isInterior(), false)
	TEST_EQUAL(e.isInterior(), false)
	TEST_EQUAL(f.isInterior(), false)
RESULT

CHECK(bool hasChild() const throw())
	TEST_EQUAL(a.hasChild(), true)
	TEST_EQUAL(b.hasChild(), true)
	TEST_EQUAL(c.hasChild(), true)
	TEST_EQUAL(d.hasChild(), false)
	TEST_EQUAL(e.hasChild(), false)
	TEST_EQUAL(f.hasChild(), false)
RESULT

CHECK(bool isChildOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isChildOf(a), false)
	TEST_EQUAL(b.isChildOf(a), true)
	TEST_EQUAL(c.isChildOf(a), false)
	TEST_EQUAL(d.isChildOf(a), false)
	TEST_EQUAL(e.isChildOf(a), false)
	TEST_EQUAL(f.isChildOf(a), false)
	TEST_EQUAL(a.isChildOf(b), false)
	TEST_EQUAL(b.isChildOf(b), false)
	TEST_EQUAL(c.isChildOf(b), true)
	TEST_EQUAL(d.isChildOf(b), true)
	TEST_EQUAL(e.isChildOf(b), false)
	TEST_EQUAL(f.isChildOf(b), false)
RESULT

CHECK(bool isFirstChild() const throw())
	TEST_EQUAL(a.isFirstChild(), false)
	TEST_EQUAL(b.isFirstChild(), true)
	TEST_EQUAL(c.isFirstChild(), true)
	TEST_EQUAL(d.isFirstChild(), false)
	TEST_EQUAL(e.isFirstChild(), true)
	TEST_EQUAL(f.isFirstChild(), false)
RESULT

CHECK(bool isFirstChildOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isFirstChildOf(a), false)
	TEST_EQUAL(b.isFirstChildOf(a), true)
	TEST_EQUAL(c.isFirstChildOf(a), false)
	TEST_EQUAL(d.isFirstChildOf(a), false)
	TEST_EQUAL(e.isFirstChildOf(a), false)
	TEST_EQUAL(f.isFirstChildOf(a), false)
	TEST_EQUAL(a.isFirstChildOf(b), false)
	TEST_EQUAL(b.isFirstChildOf(b), false)
	TEST_EQUAL(c.isFirstChildOf(b), true)
	TEST_EQUAL(d.isFirstChildOf(b), false)
	TEST_EQUAL(e.isFirstChildOf(b), false)
	TEST_EQUAL(f.isFirstChildOf(b), false)
RESULT

CHECK(bool isLastChild() const throw())
	TEST_EQUAL(a.isLastChild(), false)
	TEST_EQUAL(b.isLastChild(), true)
	TEST_EQUAL(c.isLastChild(), false)
	TEST_EQUAL(d.isLastChild(), true)
	TEST_EQUAL(e.isLastChild(), true)
	TEST_EQUAL(f.isLastChild(), false)
RESULT

CHECK(bool isLastChildOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isLastChildOf(a), false)
	TEST_EQUAL(b.isLastChildOf(a), true)
	TEST_EQUAL(c.isLastChildOf(a), false)
	TEST_EQUAL(d.isLastChildOf(a), false)
	TEST_EQUAL(e.isLastChildOf(a), false)
	TEST_EQUAL(f.isLastChildOf(a), false)
	TEST_EQUAL(a.isLastChildOf(b), false)
	TEST_EQUAL(b.isLastChildOf(b), false)
	TEST_EQUAL(c.isLastChildOf(b), false)
	TEST_EQUAL(d.isLastChildOf(b), true)
	TEST_EQUAL(e.isLastChildOf(b), false)
	TEST_EQUAL(f.isLastChildOf(b), false)
RESULT

CHECK(bool hasParent() const throw())
	TEST_EQUAL(a.hasParent(), false)
	TEST_EQUAL(b.hasParent(), true)
	TEST_EQUAL(c.hasParent(), true)
	TEST_EQUAL(d.hasParent(), true)
	TEST_EQUAL(e.hasParent(), true)
	TEST_EQUAL(f.hasParent(), false)
RESULT

CHECK(bool isParentOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isParentOf(a), false)
	TEST_EQUAL(a.isParentOf(b), true)
	TEST_EQUAL(b.isParentOf(a), false)
	TEST_EQUAL(c.isParentOf(a), false)
	TEST_EQUAL(d.isParentOf(a), false)
	TEST_EQUAL(e.isParentOf(a), false)
	TEST_EQUAL(f.isParentOf(a), false)
	TEST_EQUAL(f.isParentOf(f), false)
	TEST_EQUAL(a.isParentOf(c), false)
	TEST_EQUAL(a.isParentOf(d), false)
	TEST_EQUAL(a.isParentOf(e), false)
	TEST_EQUAL(a.isParentOf(f), false)
RESULT

CHECK(bool hasSibling() const throw())
	TEST_EQUAL(a.hasSibling(), false)
	TEST_EQUAL(b.hasSibling(), false)
	TEST_EQUAL(c.hasSibling(), true)
	TEST_EQUAL(d.hasSibling(), true)
	TEST_EQUAL(e.hasSibling(), false)
	TEST_EQUAL(f.hasSibling(), false)
RESULT

CHECK(bool isSiblingOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isSiblingOf(a), false)
	TEST_EQUAL(b.isSiblingOf(a), false)
	TEST_EQUAL(c.isSiblingOf(a), false)
	TEST_EQUAL(d.isSiblingOf(a), false)
	TEST_EQUAL(e.isSiblingOf(a), false)
	TEST_EQUAL(f.isSiblingOf(a), false)
	TEST_EQUAL(a.isSiblingOf(b), false)
	TEST_EQUAL(b.isSiblingOf(b), false)
	TEST_EQUAL(c.isSiblingOf(b), false)
	TEST_EQUAL(d.isSiblingOf(b), false)
	TEST_EQUAL(e.isSiblingOf(b), false)
	TEST_EQUAL(f.isSiblingOf(b), false)
	TEST_EQUAL(a.isSiblingOf(c), false)
	TEST_EQUAL(b.isSiblingOf(c), false)
	TEST_EQUAL(c.isSiblingOf(c), false)
	TEST_EQUAL(d.isSiblingOf(c), true)
	TEST_EQUAL(e.isSiblingOf(c), false)
	TEST_EQUAL(f.isSiblingOf(c), false)
	TEST_EQUAL(a.isSiblingOf(d), false)
	TEST_EQUAL(b.isSiblingOf(d), false)
	TEST_EQUAL(c.isSiblingOf(d), true)
	TEST_EQUAL(d.isSiblingOf(d), false)
	TEST_EQUAL(e.isSiblingOf(d), false)
	TEST_EQUAL(f.isSiblingOf(d), false)
RESULT

CHECK(bool hasPreviousSibling() const throw())
	TEST_EQUAL(a.hasPreviousSibling(), false)
	TEST_EQUAL(b.hasPreviousSibling(), false)
	TEST_EQUAL(c.hasPreviousSibling(), false)
	TEST_EQUAL(d.hasPreviousSibling(), true)
	TEST_EQUAL(e.hasPreviousSibling(), false)
	TEST_EQUAL(f.hasPreviousSibling(), false)
RESULT

CHECK(bool hasNextSibling() const throw())
	TEST_EQUAL(a.hasNextSibling(), false)
	TEST_EQUAL(b.hasNextSibling(), false)
	TEST_EQUAL(c.hasNextSibling(), true)
	TEST_EQUAL(d.hasNextSibling(), false)
	TEST_EQUAL(e.hasNextSibling(), false)
	TEST_EQUAL(f.hasNextSibling(), false)
RESULT

CHECK(bool isPreviousSiblingOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isPreviousSiblingOf(a), false)
	TEST_EQUAL(b.isPreviousSiblingOf(a), false)
	TEST_EQUAL(c.isPreviousSiblingOf(a), false)
	TEST_EQUAL(d.isPreviousSiblingOf(a), false)
	TEST_EQUAL(e.isPreviousSiblingOf(a), false)
	TEST_EQUAL(f.isPreviousSiblingOf(a), false)
	TEST_EQUAL(a.isPreviousSiblingOf(b), false)
	TEST_EQUAL(b.isPreviousSiblingOf(b), false)
	TEST_EQUAL(c.isPreviousSiblingOf(b), false)
	TEST_EQUAL(d.isPreviousSiblingOf(b), false)
	TEST_EQUAL(e.isPreviousSiblingOf(b), false)
	TEST_EQUAL(f.isPreviousSiblingOf(b), false)
	TEST_EQUAL(a.isPreviousSiblingOf(c), false)
	TEST_EQUAL(b.isPreviousSiblingOf(c), false)
	TEST_EQUAL(c.isPreviousSiblingOf(c), false)
	TEST_EQUAL(d.isPreviousSiblingOf(c), false)
	TEST_EQUAL(e.isPreviousSiblingOf(c), false)
	TEST_EQUAL(f.isPreviousSiblingOf(c), false)
	TEST_EQUAL(a.isPreviousSiblingOf(d), false)
	TEST_EQUAL(b.isPreviousSiblingOf(d), false)
	TEST_EQUAL(c.isPreviousSiblingOf(d), true)
	TEST_EQUAL(d.isPreviousSiblingOf(d), false)
	TEST_EQUAL(e.isPreviousSiblingOf(d), false)
	TEST_EQUAL(f.isPreviousSiblingOf(d), false)
RESULT

CHECK(bool isNextSiblingOf(const Composite& composite) const throw())
	TEST_EQUAL(a.isNextSiblingOf(a), false)
	TEST_EQUAL(b.isNextSiblingOf(a), false)
	TEST_EQUAL(c.isNextSiblingOf(a), false)
	TEST_EQUAL(d.isNextSiblingOf(a), false)
	TEST_EQUAL(e.isNextSiblingOf(a), false)
	TEST_EQUAL(f.isNextSiblingOf(a), false)
	TEST_EQUAL(a.isNextSiblingOf(b), false)
	TEST_EQUAL(b.isNextSiblingOf(b), false)
	TEST_EQUAL(c.isNextSiblingOf(b), false)
	TEST_EQUAL(d.isNextSiblingOf(b), false)
	TEST_EQUAL(e.isNextSiblingOf(b), false)
	TEST_EQUAL(f.isNextSiblingOf(b), false)
	TEST_EQUAL(a.isNextSiblingOf(c), false)
	TEST_EQUAL(b.isNextSiblingOf(c), false)
	TEST_EQUAL(c.isNextSiblingOf(c), false)
	TEST_EQUAL(d.isNextSiblingOf(c), true)
	TEST_EQUAL(e.isNextSiblingOf(c), false)
	TEST_EQUAL(f.isNextSiblingOf(c), false)
	TEST_EQUAL(a.isNextSiblingOf(d), false)
	TEST_EQUAL(b.isNextSiblingOf(d), false)
	TEST_EQUAL(c.isNextSiblingOf(d), false)
	TEST_EQUAL(d.isNextSiblingOf(d), false)
	TEST_EQUAL(e.isNextSiblingOf(d), false)
	TEST_EQUAL(f.isNextSiblingOf(d), false)
RESULT

CHECK(BALL_CREATE_DEEP(Composite))
  // ???
RESULT

CHECK(const PreciseTime& getModificationTime() const throw())
  // ???
RESULT

CHECK(const PreciseTime& getSelectionTime() const throw())
  // ???
RESULT

CHECK(void stamp(StampType stamp = BOTH) throw())
  // ???
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
