// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PrimitiveManager_test.C,v 1.3 2002/12/18 16:00:40 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/VIEW/GUI/KERNEL/primitiveManager.h>
///////////////////////////

/////////////////////////////////////////////////////////////
START_TEST(PrimitiveManager, "$Id: PrimitiveManager_test.v")
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::VIEW;
using namespace std;

Composite c,d,e,f;

GeometricObject g,h,i,j, x;

CHECK(PrimitiveManager())
	PrimitiveManager* pm = new PrimitiveManager;
	TEST_NOT_EQUAL(pm, 0)
RESULT

PrimitiveManager pm;

CHECK(insertGeometricObject(..))
	TEST_EQUAL(pm.insertGeometricObject(&g, &c), true)
	TEST_EQUAL(pm.insertGeometricObject(&h, &c), true)
	TEST_EQUAL(pm.insertGeometricObject(&i, &d), true)
	TEST_EQUAL(pm.insertGeometricObject(&j, 0), true)
	TEST_EQUAL(pm.insertGeometricObject(&i, 0), false)
RESULT

CHECK(getFirstIterator(Composite* composite))
	TEST_EQUAL(*(pm.getFirstIterator(&c)), &g)
	TEST_EQUAL(*(pm.getFirstIterator(&d)), &i)
	TEST_EQUAL(*(pm.getFirstIterator(0)), &j)
	TEST_EQUAL(pm.getFirstIterator(&e) ==  pm.getEndIterator(), true)
RESULT

CHECK(getSecondIterator(..))
	TEST_EQUAL(*(pm.getSecondIterator(&c)), &h)
	TEST_EQUAL(*(pm.getSecondIterator(&d)), &i)
	TEST_EQUAL(*(pm.getSecondIterator(0)), &j)
	TEST_EQUAL(pm.getSecondIterator(&e) == pm.getEndIterator(), true)
RESULT

CHECK(hasComposite(Composite))
	TEST_EQUAL(pm.hasComposite(&c), true)
	TEST_EQUAL(pm.hasComposite(&d), true)
	TEST_EQUAL(pm.hasComposite(0), true)
	TEST_EQUAL(pm.hasComposite(&e), false)
RESULT

CHECK(hasGeometricObject(const GeometricObject* object) const)
	TEST_EQUAL(pm.hasGeometricObject(&g), true)
	TEST_EQUAL(pm.hasGeometricObject(&h), true)
	TEST_EQUAL(pm.hasGeometricObject(&i), true)
	TEST_EQUAL(pm.hasGeometricObject(&x), false)
RESULT

CHECK(countGeometricObjects(Composite* composite = 0) const)
	TEST_EQUAL(pm.countGeometricObjects(), 1)
	TEST_EQUAL(pm.countGeometricObjects(&c), 2)
	TEST_EQUAL(pm.countGeometricObjects(&d), 1)
	TEST_EQUAL(pm.countGeometricObjects(&e), 0)
RESULT

CHECK(getNumberOfGeometricObjects() const)
	TEST_EQUAL(pm.getNumberOfGeometricObjects(), 4)
RESULT

CHECK(dump)
	cout << endl;
	pm.dump();
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

