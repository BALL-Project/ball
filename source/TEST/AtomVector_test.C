// $Id: AtomVector_test.C,v 1.4 2001/07/15 11:11:13 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/COMMON/atomVector.h>
#include <BALL/MOLMEC/COMMON/gradient.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
///////////////////////////

START_TEST(class_name, "$Id: AtomVector_test.C,v 1.4 2001/07/15 11:11:13 amoll Exp $")

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
Atom c;
c.setName("c");

CHECK(AtomVector::AtomVector(const AtomVector& atom_vector))
	av.push_back(&a);
	av.push_back(&b);
	AtomVector av2(av);
	TEST_EQUAL(av2.size(), 2)
	TEST_EQUAL(av2[0]->getName(), "a")
	TEST_EQUAL(av2[1]->getName(), "b")
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

	Atom a1(a);
	Atom b1(b);
	AtomVector av3;
	av3.push_back(&a1);
	av3.push_back(&b1);
	av3.savePositions();

	// test for saved positons
	av2.set(av3);
	v3.set(-99, -99, -99);
	a1.setPosition(v3);
	av2.resetPositions();
	v3.set(1.0, 2.0, 3.0);
	TEST_EQUAL(av2[0]->getPosition(), v3)
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
	// no step lengt, using saved position
  Gradient g(av);
	Vector3 v3(2.2, 3.3, 4.4);
	g[0] = v3;
	v3.set(5.5, 6.6, 7.7);
	g[1] = v3;
	//v3.set(8.8, 9.9, 10.1);
	v3.set(-1.0, -2.0, -3.0);
	a.setPosition(v3);
	TEST_EQUAL(av[0]->getPosition(), v3)
	av.moveTo(g);
	v3.set(3.2, 5.3, 7.4);
	TEST_EQUAL(av[0]->getPosition(), v3)
	v3.set(15.5, 26.6, 37.7);
	TEST_EQUAL(av[1]->getPosition(), v3)
	
	// step length set
	av.moveTo(g, 2.0);
	v3.set(5.4, 8.6, 11.8);
	TEST_EQUAL(av[0]->getPosition(), v3)
	v3.set(1.0, 2.0, 3.0);
	a.setPosition(v3);

	// no saved positon, so using the actual position
	AtomVector av2;
	av2.push_back(&a);
	av2.push_back(&b);
	av2.moveTo(g);
	v3.set(3.2, 5.3, 7.4);
	TEST_EQUAL(av2[0]->getPosition(), v3)
	
	// if different sIze in gradient, nothing happens:
	av2.push_back(&c);
	v3 = a.getPosition();
	av2.moveTo(g);
	TEST_EQUAL(av2[0]->getPosition(), v3)
RESULT

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


CHECK(AtomVector::begin()/end())
	TEST_EQUAL(*av.begin(), &a)
	TEST_EQUAL((*av.begin())->getName(), "a")
	AtomVector av2;
	bool result = (av2.begin() == av2.end());
	TEST_EQUAL(result, true)
RESULT


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
END_TEST
