// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: AtomStaticAtomAttributes_test.C,v 1.2 2003/06/26 12:07:35 amoll Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
///////////////////////////

START_TEST(class_name, "$Id: AtomStaticAtomAttributes_test.C,v 1.2 2003/06/26 12:07:35 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Atom a;
Atom::StaticAtomAttributes sa, sa2;

sa.charge = 1.23;
sa.position = Vector3(1,2,3);
sa.velocity = Vector3(4,5,6);
sa.force = Vector3(7,8,9);
sa.type = 'a';
sa.ptr = &a;

sa2.ptr = 0;
Vector3 e(0,0,0);
CHECK(StaticAtomAttributes& operator = (const StaticAtomAttributes& attr))
	sa2 = sa;
	TEST_REAL_EQUAL(sa2.charge, sa.charge)
	TEST_EQUAL(sa2.position, sa.position)
	TEST_EQUAL(sa2.velocity, sa.velocity)
	TEST_EQUAL(sa2.force, sa.force)
	TEST_EQUAL(sa2.type, sa.type)
	TEST_EQUAL(sa2.ptr, 0)
RESULT

CHECK(void clear())
	sa2.clear();
	TEST_REAL_EQUAL(sa2.charge, 0)
	TEST_EQUAL(sa2.position, e)
	TEST_EQUAL(sa2.velocity, e)
	TEST_EQUAL(sa2.force, e)
	TEST_EQUAL(sa2.type, -1)
	TEST_EQUAL(sa2.ptr, 0)
RESULT

CHECK(void set(StaticAtomAttributes& attr))
	sa2.set(sa);
	TEST_REAL_EQUAL(sa2.charge, sa.charge)
	TEST_EQUAL(sa2.position, sa.position)
	TEST_EQUAL(sa2.velocity, sa.velocity)
	TEST_EQUAL(sa2.force, sa.force)
	TEST_EQUAL(sa2.type, sa.type)
	TEST_EQUAL(sa2.ptr, 0)
RESULT

CHECK(void swap(StaticAtomAttributes& attr))
	sa2.clear();
	sa2.swap(sa);
	TEST_REAL_EQUAL(sa.charge, 0)
	TEST_EQUAL(sa.position, e)
	TEST_EQUAL(sa.velocity, e)
	TEST_EQUAL(sa.force, e)
	TEST_EQUAL(sa.type, -1)
	TEST_EQUAL(sa.ptr, 0)

	TEST_REAL_EQUAL(sa2.charge, 1.23)
	TEST_EQUAL(sa2.position, Vector3(1,2,3))
	TEST_EQUAL(sa2.velocity, Vector3(4,5,6))
	TEST_EQUAL(sa2.force, Vector3(7,8,9))
	TEST_EQUAL(sa2.type, 'a')
	TEST_EQUAL(sa2.ptr, &a)

RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
