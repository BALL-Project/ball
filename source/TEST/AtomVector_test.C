// $Id: AtomVector_test.C,v 1.2 2001/07/14 20:14:52 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
///////////////////////////

START_TEST(class_name, "$Id: AtomVector_test.C,v 1.2 2001/07/14 20:14:52 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

AtomVector* ptr; 
CHECK(Cstr)
  ptr = new AtomVector;
  TEST_NOT_EQUAL(ptr, 0)
RESULT


CHECK(Dstr)
  delete ptr; 
RESULT

Atom a, b;
a.setName("a");
b.setName("b");
Vector3 v3(1.0, 2.0, 3.0);
a.setPosition(v3);
v3.set(10.0, 20.0, 30.0);
b.setPosition(v3);
AtomVector av;
Molecule mol;
mol.insert(a);
mol.insert(b);

CHECK(CpyCstr)
	av.push_back(&a);
	av.push_back(&b);
	AtomVector av2(av);
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")
RESULT


CHECK(AtomVector::BALL_CREATE_DEEP(AtomVector))
	AtomVector* v_ptr = (AtomVector*) av.create(false, true);
	TEST_EQUAL(v_ptr->size(), 0)
	delete v_ptr;
	v_ptr = (AtomVector*) av.create();
	TEST_EQUAL(v_ptr->size(), 2)
	TEST_EQUAL((*v_ptr)[0]->getName(), "a")
	delete v_ptr;
RESULT


CHECK(AtomVector::AtomVector& operator = (const AtomVector& rhs))
	AtomVector av2;
	av2 = av;
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")
RESULT


CHECK(AtomVector::set(const AtomVector& atoms))
	AtomVector av2;
	av2.set(av);
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")
RESULT


CHECK(AtomVector::AtomVector& operator = (const Composite& rhs))
	AtomVector av2;
	av2 = mol;
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")

	b.select();
	av2 = mol;
	TEST_EQUAL(av2.size(), 1)
	TEST_EQUAL(av2[0]->getName(), "b")

	b.deselect();
	av2 = mol;
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")
RESULT


CHECK(AtomVector::set(const Composite& composite, bool selected_only = false))
 	AtomVector av2;
	b.select();
	av2.set(mol, false);
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")

	av2.set(mol, true);
	TEST_EQUAL(av2.size(), 1)
	TEST_EQUAL(av2[0]->getName(), "b")

	b.deselect();
	av2.set(mol, false);
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")
RESULT


CHECK(size)
	TEST_EQUAL(av.size(), 2)
	AtomVector av2;
	TEST_EQUAL(av2.size(), 0)
RESULT


CHECK(AtomVector::const& operator [] (int i) const 1/2)
	TEST_EQUAL(av[0]->getName(), "a")
	TEST_EQUAL(av[1]->getName(), "b")
RESULT


CHECK(AtomVector::savePositions())
	av.savePositions();
RESULT


CHECK(AtomVector::resetPositions())
	v3.set(1111.11, 111.1, 11.1);
	a.setPosition(v3);
	b.setPosition(v3);
	TEST_EQUAL(a.getPosition(), v3);
	av.resetPositions();
	v3.set(1.0, 2.0, 3.0);
	TEST_EQUAL(a.getPosition(), v3);
	v3.set(10.0, 20.0, 30.0);
	TEST_EQUAL(b.getPosition(), v3);
RESULT


CHECK(AtomVector::moveTo(const Gradient& direction, float step = 1.0))
  Gradient g(av);
	Vector3 v3(-1.0, -2.0, -3.0);
	a.setPosition(v3);
	TEST_EQUAL(av[0]->getPosition(), v3)
	av.moveTo(g);
	v3.set(1.0, 2.0, 3.0);
	TEST_EQUAL(av[0]->getPosition(), v3)
	// BAUSTELLE
RESULT

Atom c;
c.setName("c");
CHECK(push_back)
	av.push_back(&c);
	TEST_EQUAL(av.size(), 3)
RESULT


CHECK(resize)
	av.resize(2);
	TEST_EQUAL(av.size(), 2)
	av.resize(99);
	TEST_EQUAL(av.size(), 99)
	TEST_EQUAL(av[98], 0)
	av.resetPositions();	
	av.resize(2);
	av.push_back(&c);
	TEST_EQUAL(av[2], &c)
RESULT


CHECK(AtomVector::begin())
	TEST_NOT_EQUAL(av.begin(), 0)
	TEST_NOT_EQUAL(*av.begin(), 0)
	TEST_EQUAL(*av.begin(), &a)
	TEST_EQUAL((*av.begin())->getName(), "a")
	AtomVector av2;
	TEST_EQUAL(av2.begin(), 0)
RESULT


CHECK(AtomVector::end())
	AtomVector::Iterator it;
	TEST_NOT_EQUAL(av.end(), 0)
	it = av.end();
	TEST_NOT_EQUAL(it, 0)
	TEST_EQUAL(*it, 0)
	--it;
	TEST_EQUAL(*it, &c)
RESULT

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
END_TEST
