// $Id: Composite_test.C,v 1.1 1999/09/06 22:22:30 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/CONCEPT/composite.h>
#include <BALL/CONCEPT/textPersistenceManager.h>
using namespace BALL;
///////////////////////////

START_TEST(Composite, "$Id: Composite_test.C,v 1.1 1999/09/06 22:22:30 oliver Exp $")

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
Composite a;
a.select();
String filename;
CHECK(persistentWrite(TextPersistenceManager&, String&, bool))
NEW_TMP_FILE(filename)
ofstream  ofile(filename.c_str(), ios::out);
pm.setOstream(ofile);
pm.registerClass(RTTI<Composite>::getStreamName(), RTTI<Composite>::getNew);
a >> pm;
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

CHECK(set(const Composite&, UnaryPredicate<Composite>&))
RESULT

CHECK(set(const Composite&, bool))
RESULT

CHECK(operator = (const Composite&))
RESULT

CHECK(get(Composite&, UnaryPredicate<Composite>&) const)
RESULT

CHECK(get(Composite&, bool) const)
RESULT

CHECK(count(const KernelPredicateType&) const)
RESULT


CHECK(getPathLength(const Composite&, const Composite&) const)
RESULT

CHECK(getDepth(const Composite&))
RESULT

CHECK(getDepth() const)
RESULT

CHECK(getHeight(const Composite&))
RESULT

CHECK(getHeight() const)
RESULT

CHECK(getRoot())
RESULT

CHECK(getRoot() const)
RESULT

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
// 		bool isEmpty() const;
// 		bool isFree() const;
// 		bool isRoot() const;
// 		bool isRootOf(const Composite& composite) const;
// 		bool isInterior() const;
// 		bool isInteriorOf(const Composite& composite) const;
// 		bool isLeaf() const;
// 		bool isLeafOf(const Composite& composite) const;
// 		bool hasChild() const;
// 		bool isChild() const;
// 		bool isChildOf(const Composite& composite) const;
// 		bool isFirstChild() const;
// 		bool isFirstChildOf(const Composite& composite) const;
// 		bool isLastChild() const;
// 		bool isLastChildOf(const Composite& composite) const;
// 		bool hasParent() const;
// 		bool isParent() const;
// 		bool isParentOf(const Composite& composite) const;
// 		bool hasSibling() const;
// 		bool isSibling() const;
// 		bool isSiblingOf(const Composite& composite) const;
// 		bool hasPreviousSibling() const;
// 		bool isPreviousSibling() const;
// 		bool isPreviousSiblingOf(const Composite& composite) const;
// 		bool hasNextSibling() const;
// 		bool isNextSibling() const;
// 		bool isNextSiblingOf(const Composite& composite) const;
// 		bool hasPreceedingSibling() const;
// 		bool isPreceedingSibling() const;
// 		bool isPreceedingSiblingOf(const Composite& composite) const;
// 		bool hasSucceedingSibling() const;
// 		bool isSucceedingSibling() const;
// 		bool isSucceedingSiblingOf(const Composite& composite) const;
// 		bool hasDescendant() const;
// 		bool isDescendant() const;
// 		bool isDescendantOf(const Composite& composite) const;
// 		bool hasAnyAncestor() const;
// 		template <class T> bool hasAncestor(const T& t) const 
// 		bool isAncestor() const;
// 		bool isAncestorOf(const Composite& composite) const
// 		bool isRelatedWith(const Composite& composite) const;
// 		bool isHomomorph(const Composite& composite) const;
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
