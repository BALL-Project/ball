// $Id: Composite_test.C,v 1.5 1999/09/08 18:07:56 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
using namespace BALL;
///////////////////////////

START_TEST(Composite, "$Id: Composite_test.C,v 1.5 1999/09/08 18:07:56 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

CHECK(Composite())
Composite c;
RESULT

CHECK(isValid())
Composite* c;
c = new Composite;
TEST_NOT_EQUAL(c, 0)
TEST_EQUAL(c->isValid(), true)
RESULT

CHECK(Composite::~Composite())
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

CHECK(Composite::clone(Composite&, UnaryPredicate<Composite>&) const)
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
TEST_EQUAL(a.isSelected(), true);
TEST_EQUAL(b.isSelected(), true);
TEST_EQUAL(c.isSelected(), true);
TEST_EQUAL(d.isSelected(), true);
TEST_EQUAL(e.isSelected(), false);
RESULT


TextPersistenceManager  pm;
Composite composite;
composite.select();
String filename;
CHECK(persistentWrite(TextPersistenceManager&, String&, bool))
NEW_TMP_FILE(filename)
ofstream  ofile(filename.c_str(), ios::out);
pm.setOstream(ofile);
pm.registerClass(RTTI<Composite>::getStreamName(), RTTI<Composite>::getNew);
composite >> pm;
ofile.close();
RESULT

CHECK(persistentRead(TextPersistenceManager()))
ifstream  ifile(filename.c_str());
pm.setIstream(ifile);
PersistentObject* ptr;
ptr = pm.readObject();
ifile.close();
TEST_NOT_EQUAL(ptr, 0)
if (ptr != 0)
{
  TEST_EQUAL(RTTI<Composite>::isKindOf(*ptr), true)
  if (RTTI<Composite>::isKindOf(*ptr))
  {
    Composite* pers_a = RTTI<Composite>::castTo(*ptr);
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

CHECK(getDepth() const)
const Composite& c_a = a;
const Composite& c_b = b;
const Composite& c_c = c;
const Composite& c_d = d;
const Composite& c_e = e;
const Composite& c_f = f;
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

// 		bool hasAnyAncestor() const;
// 		template <class T> bool hasAncestor(const T& t) const 
// 		bool isAncestor() const;
// 		bool isAncestorOf(const Composite& composite) const
// 		bool isRelatedWith(const Composite& composite) const;
// 		bool isHomomorph(const Composite& composite) const;


// 		Composite* getLowestCommonAncestor(Composite& composite);
// 		const Composite* getLowestCommonAncestor(const Composite& composite) const;
// 
// 		template <class T> Composite* getAncestor(const T& /* t */)
// 		template <class T> const Composite* getAncestor(const T& t) const
// 		Composite* getParent();
// 		const Composite* getParent() const;
// 		Composite* getChild(Index index);
// 		const Composite* getChild(Index index) const;
// 		Composite* getSibling(Index index);
// 		const Composite* getSibling(Index index) const;
// 		Composite* getFirstChild();
// 		const Composite* getFirstChild() const;
// 		Composite* getLastChild();
// 		const Composite* getLastChild() const;
// 		void expand();
// 		void collapse();
// 		static bool insertParent
// 			(Composite& parent, Composite& first, 
// 			 Composite& last, bool destroy_parent = true);
// 
// 		void insertBefore(Composite& composite);
// 		void insertAfter(Composite& composite);
// 		void spliceBefore(Composite& composite);
// 		void spliceAfter(Composite& composite);
// 		void splice(Composite& composite);
// 		bool removeChild(Composite& child);
// 		void replace(Composite& composite);
// 		void swap(Composite& composite);
// 
// 		bool isExpanded() const;
// 		bool isCollapsed() const;
// 
// 		void host(Visitor<Composite>& visitor);
// 		bool applyAncestor(UnaryProcessor<Composite>& processor);
// 		bool applyChild(UnaryProcessor<Composite>& processor);
// 		bool applyDescendantPreorder(UnaryProcessor<Composite>& processor);
// 		bool applyDescendantPostorder(UnaryProcessor<Composite>& processor);
// 		bool applyDescendant(UnaryProcessor<Composite>& processor);
// 		bool applyPreorder(UnaryProcessor<Composite>& processor);
// 		bool applyPostorder(UnaryProcessor<Composite>& processor);
// 		bool apply(UnaryProcessor<Composite>& processor);
// 		bool applyLevel(UnaryProcessor<Composite>& processor, long level);

CHECK(set(const Composite&, UnaryPredicate<Composite>&))
//BAUSTELLE
RESULT

CHECK(set(const Composite&, bool))
//BAUSTELLE
RESULT

CHECK(operator = (const Composite&))
//BAUSTELLE
RESULT

CHECK(get(Composite&, UnaryPredicate<Composite>&) const)
//BAUSTELLE
RESULT

CHECK(get(Composite&, bool) const)
//BAUSTELLE
RESULT

CHECK(count(const KernelPredicateType&) const)
//BAUSTELLE
RESULT


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
