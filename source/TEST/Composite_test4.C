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
	TEST_EQUAL(c.getPathLength(d), INVALID_Size)
	TEST_EQUAL(d.getPathLength(c), INVALID_Size)
	TEST_EQUAL(d.getPathLength(a), 2)
	TEST_EQUAL(a.getPathLength(d), 2)
	TEST_EQUAL(c.getPathLength(a), 2)
	TEST_EQUAL(a.getPathLength(c), 2)
	TEST_EQUAL(e.getPathLength(a), 3)
	TEST_EQUAL(a.getPathLength(e), 3)
	TEST_EQUAL(b.getPathLength(e), 2)
	TEST_EQUAL(e.getPathLength(b), 2)
	TEST_EQUAL(a.getPathLength(f), INVALID_Size)
	TEST_EQUAL(f.getPathLength(a), INVALID_Size)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
