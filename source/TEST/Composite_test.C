// $Id: Composite_test.C,v 1.14 2000/08/24 20:26:08 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
#include <../source/TEST/ItemCollector.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/chain.h>
///////////////////////////

using namespace BALL;
using namespace std;

START_TEST(Composite, "$Id: Composite_test.C,v 1.14 2000/08/24 20:26:08 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

Composite* c_ptr;
CHECK(Composite())
	c_ptr = new Composite;
	TEST_NOT_EQUAL(c_ptr, 0)
RESULT

CHECK(isValid())
	TEST_EQUAL(c_ptr->isValid(), true)
RESULT

CHECK(Composite::~Composite())
	delete c_ptr;
RESULT

CHECK(getDegree() const)
	Composite a;
	TEST_EQUAL(a.getDegree(), 0)
RESULT

CHECK(prependChild(Composite&))
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

CHECK(appendChild(Composite&))
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

CHECK(countDescendants() const)
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

CHECK(Composite::clear())
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

CHECK(Composite::destroy())
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

CHECK(Composite::destroy(bool))
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

CHECK(Composite(const Composite&, bool))
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

CHECK(Composite::clone(Composite&, KernelPredicateType&) const)
	Composite a, b, c, d, e;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	Composite f;
	a.clone(f, Composite::DEFAULT_UNARY_PREDICATE);
	TEST_EQUAL(a.getDegree(), 1)
	TEST_EQUAL(b.getDegree(), 2)
	TEST_EQUAL(c.getDegree(), 1)
	TEST_EQUAL(d.getDegree(), 0)
	TEST_EQUAL(e.getDegree(), 0)
	TEST_EQUAL(f.getDegree(), 1)
	TEST_EQUAL(f.countDescendants(), 4)
RESULT

CHECK(isSelected())
	Composite a;
	TEST_EQUAL(a.isSelected(), false)
RESULT

CHECK(select())
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

CHECK(deselect())
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


TextPersistenceManager  pm;
Composite composite;
composite.select();
String filename;

CHECK(persistentWrite(TextPersistenceManager&, String&, bool))
	NEW_TMP_FILE(filename)
	std::ofstream  ofile(filename.c_str(), File::OUT);
	pm.setOstream(ofile);
	using namespace RTTI;
	pm.registerClass(getStreamName<Composite>(), getNew<Composite>);
	composite >> pm;
	ofile.close();
RESULT

CHECK(persistentRead(TextPersistenceManager()))
	using namespace RTTI;
	std::ifstream  ifile(filename.c_str());
	pm.setIstream(ifile);
	PersistentObject* ptr;
	ptr = pm.readObject();
	ifile.close();
	TEST_NOT_EQUAL(ptr, 0)
	if (ptr != 0)
	{
		TEST_EQUAL(isKindOf<Composite>(*ptr), true)
		if (isKindOf<Composite>(*ptr))
		{
			Composite* pers_a = castTo<Composite>(*ptr);
			TEST_EQUAL(pers_a->isSelected(), true)
		}
	}
RESULT

CHECK(containsSelection() const)
	Composite a, b, c, d, e;
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

CHECK(getPathLength(const Composite&, const Composite&) const)
	Composite a, b, c, d, e, f;
	a.appendChild(b);
	b.appendChild(c);
	b.appendChild(d);
	c.appendChild(e);
	TEST_EQUAL(Composite::getPathLength(a, a), 0)
	TEST_EQUAL(Composite::getPathLength(a, b), 1)
	TEST_EQUAL(Composite::getPathLength(b, a), 1)
	TEST_EQUAL(Composite::getPathLength(b, c), 1)
	TEST_EQUAL(Composite::getPathLength(c, b), 1)
	TEST_EQUAL(Composite::getPathLength(c, d), INVALID_SIZE)
	TEST_EQUAL(Composite::getPathLength(d, c), INVALID_SIZE)
	TEST_EQUAL(Composite::getPathLength(d, a), 2)
	TEST_EQUAL(Composite::getPathLength(a, d), 2)
	TEST_EQUAL(Composite::getPathLength(c, a), 2)
	TEST_EQUAL(Composite::getPathLength(a, c), 2)
	TEST_EQUAL(Composite::getPathLength(e, a), 3)
	TEST_EQUAL(Composite::getPathLength(a, e), 3)
	TEST_EQUAL(Composite::getPathLength(b, e), 2)
	TEST_EQUAL(Composite::getPathLength(e, b), 2)
	TEST_EQUAL(Composite::getPathLength(a, f), INVALID_SIZE)
	TEST_EQUAL(Composite::getPathLength(f, a), INVALID_SIZE)
RESULT

Composite a, b, c, d, e, f;
a.appendChild(b);
b.appendChild(c);
b.appendChild(d);
c.appendChild(e);

CHECK(getDepth(const Composite&))
	TEST_EQUAL(a.getDepth(), 0)
	TEST_EQUAL(b.getDepth(), 1)
	TEST_EQUAL(c.getDepth(), 2)
	TEST_EQUAL(d.getDepth(), 2)
	TEST_EQUAL(e.getDepth(), 3)
	TEST_EQUAL(f.getDepth(), 0)
RESULT

const Composite& c_a = a;
const Composite& c_b = b;
const Composite& c_c = c;
const Composite& c_d = d;
const Composite& c_e = e;
const Composite& c_f = f;

CHECK(getDepth() const)
	TEST_EQUAL(c_a.getDepth(), 0)
	TEST_EQUAL(c_b.getDepth(), 1)
	TEST_EQUAL(c_c.getDepth(), 2)
	TEST_EQUAL(c_d.getDepth(), 2)
	TEST_EQUAL(c_e.getDepth(), 3)
	TEST_EQUAL(c_f.getDepth(), 0)
RESULT

CHECK(getHeight() const)
	TEST_EQUAL(a.getHeight(), 3)
	TEST_EQUAL(b.getHeight(), 2)
	TEST_EQUAL(c.getHeight(), 1)
	TEST_EQUAL(d.getHeight(), 0)
	TEST_EQUAL(e.getHeight(), 0)
	TEST_EQUAL(f.getHeight(), 0)
RESULT

CHECK(getRoot())
	TEST_EQUAL(&a.getRoot(), &a)
	TEST_EQUAL(&b.getRoot(), &a)
	TEST_EQUAL(&c.getRoot(), &a)
	TEST_EQUAL(&d.getRoot(), &a)
	TEST_EQUAL(&e.getRoot(), &a)
	TEST_EQUAL(&f.getRoot(), &f)
RESULT

CHECK(getRoot() const)
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


CHECK(isEmpty() const)
	TEST_EQUAL(a.isEmpty(), false)
	TEST_EQUAL(b.isEmpty(), false)
	TEST_EQUAL(c.isEmpty(), false)
	TEST_EQUAL(d.isEmpty(), true)
	TEST_EQUAL(e.isEmpty(), true)
	TEST_EQUAL(f.isEmpty(), true)
RESULT

CHECK(isRoot() const)
	TEST_EQUAL(a.isRoot(), true)
	TEST_EQUAL(b.isRoot(), false)
	TEST_EQUAL(c.isRoot(), false)
	TEST_EQUAL(d.isRoot(), false)
	TEST_EQUAL(e.isRoot(), false)
	TEST_EQUAL(f.isRoot(), true)
RESULT

CHECK(isRootOf() const)
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

CHECK(isInterior() const)
	TEST_EQUAL(a.isInterior(), false)
	TEST_EQUAL(b.isInterior(), true)
	TEST_EQUAL(c.isInterior(), true)
	TEST_EQUAL(d.isInterior(), false)
	TEST_EQUAL(e.isInterior(), false)
	TEST_EQUAL(f.isInterior(), false)
RESULT

CHECK(hasChild() const)
	TEST_EQUAL(a.hasChild(), true)
	TEST_EQUAL(b.hasChild(), true)
	TEST_EQUAL(c.hasChild(), true)
	TEST_EQUAL(d.hasChild(), false)
	TEST_EQUAL(e.hasChild(), false)
	TEST_EQUAL(f.hasChild(), false)
RESULT

CHECK(isChild() const)
	TEST_EQUAL(a.isChild(), false)
	TEST_EQUAL(b.isChild(), true)
	TEST_EQUAL(c.isChild(), true)
	TEST_EQUAL(d.isChild(), true)
	TEST_EQUAL(e.isChild(), true)
	TEST_EQUAL(f.isChild(), false)
RESULT

CHECK(isChildOf() const)
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

CHECK(isFirstChild() const)
	TEST_EQUAL(a.isFirstChild(), false)
	TEST_EQUAL(b.isFirstChild(), true)
	TEST_EQUAL(c.isFirstChild(), true)
	TEST_EQUAL(d.isFirstChild(), false)
	TEST_EQUAL(e.isFirstChild(), true)
	TEST_EQUAL(f.isFirstChild(), false)
RESULT

CHECK(isFirstChildOf(const Composite&) const)
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

CHECK(isLastChild() const)
	TEST_EQUAL(a.isLastChild(), false)
	TEST_EQUAL(b.isLastChild(), true)
	TEST_EQUAL(c.isLastChild(), false)
	TEST_EQUAL(d.isLastChild(), true)
	TEST_EQUAL(e.isLastChild(), true)
	TEST_EQUAL(f.isLastChild(), false)
RESULT

CHECK(isLastChildOf(const Composite&) const)
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

CHECK(hasParent() const)
	TEST_EQUAL(a.hasParent(), false)
	TEST_EQUAL(b.hasParent(), true)
	TEST_EQUAL(c.hasParent(), true)
	TEST_EQUAL(d.hasParent(), true)
	TEST_EQUAL(e.hasParent(), true)
	TEST_EQUAL(f.hasParent(), false)
RESULT

CHECK(isParentOf() const)
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

CHECK(isParent() const)
	TEST_EQUAL(a.isParent(), true)
	TEST_EQUAL(b.isParent(), true)
	TEST_EQUAL(c.isParent(), true)
	TEST_EQUAL(d.isParent(), false)
	TEST_EQUAL(e.isParent(), false)
	TEST_EQUAL(f.isParent(), false)
RESULT

CHECK(hasSibling() const)
	TEST_EQUAL(a.hasSibling(), false)
	TEST_EQUAL(b.hasSibling(), false)
	TEST_EQUAL(c.hasSibling(), true)
	TEST_EQUAL(d.hasSibling(), true)
	TEST_EQUAL(e.hasSibling(), false)
	TEST_EQUAL(f.hasSibling(), false)
RESULT

CHECK(isSiblingOf() const)
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

CHECK(hasPreviousSibling() const)
	TEST_EQUAL(a.hasPreviousSibling(), false)
	TEST_EQUAL(b.hasPreviousSibling(), false)
	TEST_EQUAL(c.hasPreviousSibling(), false)
	TEST_EQUAL(d.hasPreviousSibling(), true)
	TEST_EQUAL(e.hasPreviousSibling(), false)
	TEST_EQUAL(f.hasPreviousSibling(), false)
RESULT

CHECK(hasNextSibling() const)
	TEST_EQUAL(a.hasNextSibling(), false)
	TEST_EQUAL(b.hasNextSibling(), false)
	TEST_EQUAL(c.hasNextSibling(), true)
	TEST_EQUAL(d.hasNextSibling(), false)
	TEST_EQUAL(e.hasNextSibling(), false)
	TEST_EQUAL(f.hasNextSibling(), false)
RESULT

CHECK(isPreviousSiblingOf() const)
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

CHECK(isNextSiblingOf() const)
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

CHECK(hasDescendant() const)
	TEST_EQUAL(a.hasDescendant(), true)
	TEST_EQUAL(b.hasDescendant(), true)
	TEST_EQUAL(c.hasDescendant(), true)
	TEST_EQUAL(d.hasDescendant(), false)
	TEST_EQUAL(e.hasDescendant(), false)
	TEST_EQUAL(f.hasDescendant(), false)
RESULT

CHECK(isDescendant() const)
	TEST_EQUAL(a.isDescendant(), false)
	TEST_EQUAL(b.isDescendant(), true)
	TEST_EQUAL(c.isDescendant(), true)
	TEST_EQUAL(d.isDescendant(), true)
	TEST_EQUAL(e.isDescendant(), true)
	TEST_EQUAL(f.isDescendant(), false)
RESULT

CHECK(isDescendantOf() const)
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

cout << endl;
cout << "a " << &a <<endl;
cout << "b " << &b <<endl;
cout << "c " << &c <<endl;
cout << "d " << &d <<endl;
cout << "e " << &e <<endl;
cout << "f " << &f <<endl;
cout << endl;

CHECK(getLowestCommonAncestor(Composite& composite))
	TEST_EQUAL(d.getLowestCommonAncestor(e), &b)
	TEST_EQUAL(d.getLowestCommonAncestor(f), 0)
	TEST_EQUAL(f.getLowestCommonAncestor(f), &f)
RESULT

CHECK(getLowestCommonAncestor(Composite& composite) const)
	TEST_EQUAL(c_d.getLowestCommonAncestor(e), &b)
	TEST_EQUAL(c_d.getLowestCommonAncestor(f), 0)
	TEST_EQUAL(c_f.getLowestCommonAncestor(f), &f)
RESULT

CHECK(getAncestor(const T&))
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

CHECK(getAncestor(const T& t) const)
	TEST_EQUAL(c_a.getAncestor(b), 0)
	TEST_EQUAL(c_f.getAncestor(b), 0)
	TEST_EQUAL(c_e.getAncestor(b), &c)
RESULT

CHECK(template<T> getPrevious(const T&))
	// a single composite should return zero
	Composite* ptr = f.getPrevious(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(a.getPrevious(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(b.getPrevious(RTTI::getDefault<Composite>()), &a)
	TEST_EQUAL(c.getPrevious(RTTI::getDefault<Composite>()), &b)
	TEST_EQUAL(d.getPrevious(RTTI::getDefault<Composite>()), &e)
	TEST_EQUAL(e.getPrevious(RTTI::getDefault<Composite>()), &c)
RESULT

CHECK(template<T> getPrevious(const T&) const)
	// a single composite should return zero
	const Composite* ptr = c_f.getPrevious(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(c_a.getPrevious(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(c_b.getPrevious(RTTI::getDefault<Composite>()), &c_a)
	TEST_EQUAL(c_c.getPrevious(RTTI::getDefault<Composite>()), &c_b)
	TEST_EQUAL(c_d.getPrevious(RTTI::getDefault<Composite>()), &c_e)
	TEST_EQUAL(c_e.getPrevious(RTTI::getDefault<Composite>()), &c_c)
RESULT

CHECK(template<T> getNext(const T&))
	// a single composite should return zero
	Composite* ptr = f.getNext(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(a.getNext(RTTI::getDefault<Composite>()), &c_b)
	TEST_EQUAL(b.getNext(RTTI::getDefault<Composite>()), &c_c)
	TEST_EQUAL(c.getNext(RTTI::getDefault<Composite>()), &c_e)
	TEST_EQUAL(d.getNext(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(e.getNext(RTTI::getDefault<Composite>()), &c_d)
RESULT

CHECK(template<T> getNext(const T&) const)
	// a single composite should return zero
	const Composite* ptr = c_f.getNext(RTTI::getDefault<Composite>());
	TEST_EQUAL(ptr, 0)

	TEST_EQUAL(c_a.getNext(RTTI::getDefault<Composite>()), &b)
	TEST_EQUAL(c_b.getNext(RTTI::getDefault<Composite>()), &c)
	TEST_EQUAL(c_c.getNext(RTTI::getDefault<Composite>()), &e)
	TEST_EQUAL(c_d.getNext(RTTI::getDefault<Composite>()), 0)
	TEST_EQUAL(c_e.getNext(RTTI::getDefault<Composite>()), &d)
RESULT

CHECK(getParent())
	TEST_EQUAL(a.getParent(), 0)
	TEST_EQUAL(e.getParent(), &c)
RESULT

CHECK(getParent() const)
	TEST_EQUAL(c_a.getParent(), 0)
	TEST_EQUAL(c_e.getParent(), &c)
RESULT

CHECK(getChild(Index index))
	TEST_EQUAL(a.getChild(1), 0)
	TEST_EQUAL(a.getChild(0), &b)
	TEST_EQUAL(b.getChild(1), &d)
	TEST_EQUAL(e.getChild(0), 0)
RESULT

CHECK(getChild(Index index) const)
	TEST_EQUAL(c_a.getChild(1), 0)
	TEST_EQUAL(c_a.getChild(0), &b)
	TEST_EQUAL(c_b.getChild(1), &d)
	TEST_EQUAL(c_e.getChild(0), 0)
RESULT

CHECK(getSibling(Index index))
	TEST_EQUAL(a.getSibling(0), &a)
	TEST_EQUAL(c.getSibling(-1), 0)
	TEST_EQUAL(c.getSibling(0), &c)
	TEST_EQUAL(c.getSibling(1), &d)
	TEST_EQUAL(d.getSibling(-1), &c)
	TEST_EQUAL(d.getSibling(0), &d)
	TEST_EQUAL(d.getSibling(1), 0)
	TEST_EQUAL(e.getSibling(0), &e)
RESULT

CHECK(getSibling(Index index) const)
	TEST_EQUAL(c_a.getSibling(0), &a)
	TEST_EQUAL(c_c.getSibling(-1), 0)
	TEST_EQUAL(c_c.getSibling(0), &c)
	TEST_EQUAL(c_c.getSibling(1), &d)
	TEST_EQUAL(c_d.getSibling(-1), &c)
	TEST_EQUAL(c_d.getSibling(0), &d)
	TEST_EQUAL(c_d.getSibling(1), 0)
	TEST_EQUAL(c_e.getSibling(0), &e)
RESULT

CHECK(getFirstChild())
	TEST_EQUAL(a.getFirstChild(), &b)
	TEST_EQUAL(b.getFirstChild(), &c)
	TEST_EQUAL(e.getFirstChild(), 0)
RESULT

CHECK(getFirstChild() const)
	TEST_EQUAL(c_a.getFirstChild(), &b)
	TEST_EQUAL(c_b.getFirstChild(), &c)
	TEST_EQUAL(c_e.getFirstChild(), 0)
RESULT

CHECK(getLastChild())
	TEST_EQUAL(a.getLastChild(), &b)
	TEST_EQUAL(b.getLastChild(), &d)
	TEST_EQUAL(e.getLastChild(), 0)
RESULT

CHECK(getLastChild() const)
	TEST_EQUAL(c_a.getLastChild(), &b)
	TEST_EQUAL(c_b.getLastChild(), &d)
	TEST_EQUAL(c_e.getLastChild(), 0)
RESULT

CHECK(expand() collapse() )
	a.expand();
	TEST_EQUAL(a.isExpanded(), true)
	a.collapse();
	TEST_EQUAL(a.isCollapsed(), true)
	a.expand();
	TEST_EQUAL(a.isExpanded(), true)
RESULT

CHECK(static bool insertParent(Composite& parent, Composite& first, 
															 Composite& last, bool destroy_parent = true))
//
RESULT

CHECK(insertBefore(Composite& composite))
	e.insertBefore(f);
	TEST_EQUAL(c.getFirstChild(), &f)
	TEST_EQUAL(c.getLastChild(), &e)
	TEST_EQUAL(f.getParent(), &c)
	c.removeChild(f);
RESULT

CHECK(insertAfter(Composite& composite))
	e.insertAfter(f);
	TEST_EQUAL(c.getFirstChild(), &e)
	TEST_EQUAL(c.getLastChild(), &f)
	TEST_EQUAL(f.getParent(), &c)
	c.removeChild(f);
RESULT

CHECK(spliceBefore(Composite& composite))
//
RESULT

CHECK(spliceAfter(Composite& composite))
//
RESULT

CHECK(splice(Composite& composite))
//
RESULT

CHECK(bool removeChild(Composite& child))
	TEST_EQUAL(b.removeChild(d), true)
	TEST_EQUAL(d.getParent(), 0)
	TEST_EQUAL(b.getChild(1), 0)
	b.appendChild(d);
	f.clear();
	TEST_EQUAL(f.getFirstChild(), 0)
	TEST_EQUAL(f.removeChild(d), false)
RESULT

CHECK(replace(Composite& composite))
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

CHECK(swap(Composite& composite))/*
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

	cout <<endl;
	cout << "a " << &a <<endl;
	cout << "b " << &b <<endl;
	cout << "c " << &c <<endl;
	cout << "d " << &d <<endl;
	cout << "e " << &e <<endl;
	cout << endl;
	cout << "a1 " << &a1 <<endl;
	cout << "b1 " << &b1 <<endl;
	cout << "c1 " << &c1 <<endl;
	cout << "d1 " << &d1 <<endl;
	cout << "e1 " << &e1 <<endl;
	cout << endl;

	a.swap(e);
	TEST_EQUAL(b.getParent(), &a)
	TEST_EQUAL(c.getParent(), &b)
	TEST_EQUAL(e.getParent(), &c)

	b1.swap(b);
	cout << "-----------1";
	TEST_EQUAL(a.getFirstChild(), &b1)
	cout << "-----------2";
	TEST_EQUAL(a1.getFirstChild(), &b)
	cout << "-----------3";
	TEST_EQUAL(b.getParent(), &a1)
	cout << "-----------4";
	TEST_EQUAL(b1.getParent(), &a)
	cout << "-----------5";

	TEST_EQUAL(c.getParent(), &b1)
	TEST_EQUAL(c1.getParent(), &b)*/
RESULT

CHECK(bool isExpanded() const)
	a.expand();
	TEST_EQUAL(a.isExpanded(), true)
	a.collapse();
	TEST_EQUAL(a.isExpanded(), false)
RESULT

CHECK(bool isCollapsed() const)
	a.expand();
	TEST_EQUAL(a.isCollapsed(), false)
	a.collapse();
	TEST_EQUAL(a.isCollapsed(), true)
	a.expand();
RESULT

CHECK(bool hasAnyAncestor() const)
	TEST_EQUAL(a.hasAnyAncestor(), false)
	TEST_EQUAL(b.hasAnyAncestor(), true)
	TEST_EQUAL(f.hasAnyAncestor(), false)
RESULT

CHECK(bool hasAncestor(const T& t) const)
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

CHECK(isAncestor())
	TEST_EQUAL(a.isAncestor(), true)
	TEST_EQUAL(b.isAncestor(), true)
	TEST_EQUAL(e.isAncestor(), false)
	TEST_EQUAL(e.getChild(0), 0)
	TEST_EQUAL(e.getFirstChild(), 0)
RESULT

CHECK(isAncestorOf(const Composite& composite) const)
	TEST_EQUAL(a.isAncestorOf(b), true)
	TEST_EQUAL(a.isAncestorOf(d), true)
	TEST_EQUAL(a.isAncestorOf(e), true)
	TEST_EQUAL(b.isAncestorOf(b), false)
	TEST_EQUAL(e.isAncestorOf(b), false)
RESULT

CHECK(isRelatedWith(const Composite& composite))
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

CHECK(isHomomorph(const Composite& composite))
	f.set(a);
	TEST_EQUAL(f.isHomomorph(a), true) // ???
	TEST_EQUAL(a.isHomomorph(f), true) // ???
	TEST_EQUAL(a.isHomomorph(a), true)
	TEST_EQUAL(a.isHomomorph(b), false)
	f.clear();
	TEST_EQUAL(f.isHomomorph(a), false)
RESULT

CHECK(dump())
	TEST_EQUAL(a.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
  String filename;
	NEW_TMP_FILE(filename)
	std::ofstream outfile(filename.c_str(), File::OUT);
	a.dump(outfile);
	outfile.close();
	TEST_FILE(filename.c_str(), "data/Composite_test.txt", true)
RESULT

CHECK(host(Visitor<Composite>& visitor))
//
RESULT

ItemCollector<Composite> myproc;

CHECK(bool applyAncestor(UnaryProcessor<Composite>& processor))
	myproc.start();
	a.applyAncestor(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)

	myproc.start();
	e.applyAncestor(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 3)
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyChild(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.applyChild(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)

	myproc.start();
	a.applyChild(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 1)
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)

	myproc.start();
	b.applyChild(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 2)
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyDescendantPreorder(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.applyDescendantPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)

	myproc.start();
	a.applyDescendantPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyDescendantPostorder(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.applyDescendantPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)

	myproc.start();
	a.applyDescendantPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyDescendant(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.applyDescendant(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)

	myproc.start();
	a.applyDescendant(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyPreorder(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.applyPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 1)

	myproc.start();
	a.applyPreorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 5)
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyPostorder(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.applyPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 0)

	myproc.start();
	a.applyPostorder(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 4)
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();// ???
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();// ???
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool apply(UnaryProcessor<Composite>& processor))
	myproc.start();
	e.apply(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 1)

	myproc.start();
	a.apply(myproc);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 5)
	TEST_EQUAL(myproc.getPointer(), &a) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &b) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(bool applyLevel(UnaryProcessor<Composite>& processor, long level))
	myproc.start();
	e.applyLevel(myproc,0 );
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 1)
	TEST_EQUAL(myproc.getPointer(), &e) myproc.forward();

	myproc.start();
	c.applyLevel(myproc, 0);
	myproc.reset();
	TEST_EQUAL(myproc.getSize(), 2)
	TEST_EQUAL(myproc.getPointer(), &c) myproc.forward();
	TEST_EQUAL(myproc.getPointer(), &d) myproc.forward();// ???
	TEST_EQUAL(myproc.getPointer(), 0)
RESULT

CHECK(set(const Composite&, KernelPredicateType&))
	Composite x;
	x.set(a, Composite::DEFAULT_UNARY_PREDICATE);
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(set(const Composite&, bool))
	Composite x;
	x.set(a, true);
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(operator = (const Composite&))
	Composite x;
	x = a;
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(get(Composite&, KernelPredicateType&) const)
	Composite x;
	a.get(x, Composite::DEFAULT_UNARY_PREDICATE);
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(get(Composite&, bool) const)
	Composite x;
	a.get(x, true);
	TEST_EQUAL(x.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_NOT_EQUAL(x.getFirstChild()->getChild(1), 0)
	TEST_NOT_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild(), 0)
	TEST_EQUAL(x.getFirstChild()->getFirstChild()->getFirstChild()->getFirstChild(), 0)
RESULT

CHECK(count(const KernelPredicateType&) const)
	TEST_EQUAL(a.count(Composite::DEFAULT_UNARY_PREDICATE), 5)
	TEST_EQUAL(e.count(Composite::DEFAULT_UNARY_PREDICATE), 1)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
