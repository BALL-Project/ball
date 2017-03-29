// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/residue.h>
#include <BALL/KERNEL/secondaryStructure.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/CONCEPT/textPersistenceManager.h>

#include "ItemCollector.h"
///////////////////////////

START_TEST(Atom)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

Atom* atom = 0;
CHECK(Atom() throw())
	atom = new Atom;
	TEST_NOT_EQUAL(atom, 0)
RESULT

CHECK(void setCharge(float charge) throw())
	atom->setCharge(1.23456);
	TEST_REAL_EQUAL(atom->getCharge(), 1.23456);
RESULT

CHECK(float getCharge() const throw())
	Atom a;
	TEST_REAL_EQUAL(a.getCharge(), 0.0);
RESULT

const Atom ac;

CHECK(void setName(const String& name) throw())
	TEST_EQUAL(atom->getName(), "")
	atom->setName("ATOMNAME");
	TEST_EQUAL(atom->getName(), "ATOMNAME");
RESULT
			
CHECK(const String& getName() const throw())
	TEST_EQUAL(ac.getName(), BALL_ATOM_DEFAULT_NAME)
RESULT

CHECK(void setElement(const Element& element) throw())
	TEST_EQUAL(atom->getElement(), Element::UNKNOWN)
	atom->setElement(PTE.getElement(1));
	TEST_EQUAL(atom->getElement(), PTE.getElement(1))
RESULT
			
CHECK(const Element& getElement() const throw())
	TEST_EQUAL(ac.getElement(), *BALL_ATOM_DEFAULT_ELEMENT)
RESULT

Vector3	null_vector(0, 0, 0);
Vector3	test_vector(1, 2, 3);
CHECK(void setPosition(const Vector3& position) throw())
	TEST_EQUAL(atom->getPosition(), null_vector)
	atom->setPosition(test_vector);
	TEST_EQUAL(atom->getPosition(), test_vector)
RESULT

CHECK(const Vector3& getPosition() const throw())
	TEST_EQUAL(ac.getPosition(), Vector3(BALL_ATOM_DEFAULT_POSITION))
RESULT

CHECK(Vector3& getPosition() throw())
	Atom a;
	a.getPosition().set(1,2,3);
	TEST_EQUAL(a.getPosition(), Vector3(1,2,3))
RESULT

CHECK(float getDistance(const Vector3&) const)
	Atom a, b;
	a.getPosition().set(1,2,3);
	b.getPosition().set(0, 0, 0);
	TEST_REAL_EQUAL(a.getDistance(b), 3.741657387)
RESULT

CHECK(void setRadius(float radius) throw())
	TEST_REAL_EQUAL(atom->getRadius(), 0.0)
	atom->setRadius(1.23456);
	TEST_REAL_EQUAL(atom->getRadius(), 1.23456)
RESULT

CHECK(float getRadius() const throw())
	TEST_EQUAL(ac.getRadius(), BALL_ATOM_DEFAULT_RADIUS)
RESULT

CHECK(void setType(Type atom_type) throw())
	TEST_EQUAL(atom->getType(), Atom::UNKNOWN_TYPE)
	atom->setType(27);
	TEST_EQUAL(atom->getType(), 27);
RESULT
		
CHECK(Type getType() const throw())
	TEST_EQUAL(ac.getType(), BALL_ATOM_DEFAULT_TYPE)
RESULT

CHECK(void setVelocity(const Vector3& velocity) throw())
	TEST_EQUAL(atom->getVelocity(), null_vector)
	atom->setVelocity(test_vector);
	TEST_EQUAL(atom->getVelocity(), test_vector)
RESULT
			
CHECK(const Vector3& getVelocity() const throw())
	TEST_EQUAL(ac.getVelocity(), Vector3(BALL_ATOM_DEFAULT_VELOCITY))
RESULT

CHECK(void setForce(const Vector3& force) throw())
	TEST_EQUAL(atom->getForce(), null_vector)
	atom->setForce(test_vector);
	TEST_EQUAL(atom->getForce(), test_vector)
RESULT

CHECK(const Vector3& getForce() const throw())
	TEST_EQUAL(ac.getForce(), Vector3(BALL_ATOM_DEFAULT_FORCE))
RESULT

CHECK(Vector3& getForce() throw())
	Atom a;
	a.getForce() = Vector3(1,2,3);
	TEST_EQUAL(a.getForce(), Vector3(1,2,3))
RESULT

CHECK(Size countBonds() const throw())
	TEST_EQUAL(atom->countBonds(), 0)
RESULT

Atom* atom2 = 0;
CHECK(Atom(const Atom& atom, bool deep = true) throw())
	atom2 = new Atom(*atom);
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getName(), atom->getName())
	TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
	TEST_EQUAL(atom2->getPosition(), atom->getPosition())
	TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom2->getForce(), atom->getForce())
	TEST_EQUAL(atom2->getType(), atom->getType())
	TEST_EQUAL(atom2->countBonds(), atom->countBonds())
	TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(~Atom() throw())
	delete atom2;
RESULT
			
CHECK(Atom(Element& element, const String& name, const String& type_name = BALL_ATOM_DEFAULT_TYPE_NAME, Type atom_type = BALL_ATOM_DEFAULT_TYPE, const Vector3& position = Vector3(BALL_ATOM_DEFAULT_POSITION), const Vector3& velocity = Vector3(BALL_ATOM_DEFAULT_VELOCITY), const Vector3& force = Vector3(BALL_ATOM_DEFAULT_FORCE), float charge = BALL_ATOM_DEFAULT_CHARGE, float radius = BALL_ATOM_DEFAULT_RADIUS) throw())
	atom2 = new Atom(PTE[Element::HELIUM],
									 "TESTNAME",
									 "TESTTYPE",
									 12,
									 null_vector,
									 test_vector,
									 test_vector,
									 1.23456,
									 2.34567);
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getElement(), PTE[Element::He])
	TEST_EQUAL(atom2->getName(), "TESTNAME")
	TEST_EQUAL(atom2->getType(), 12)
	TEST_EQUAL(atom2->getPosition(), null_vector)
	TEST_EQUAL(atom2->getVelocity(), test_vector)
	TEST_EQUAL(atom2->getForce(), test_vector)
	TEST_REAL_EQUAL(atom2->getCharge(), 1.23456)
	TEST_REAL_EQUAL(atom2->getRadius(), 2.34567)
RESULT


CHECK(void clear() throw())
	atom2->clear();
	TEST_EQUAL(atom2->getName(), "")
	TEST_EQUAL(atom2->getType(), Atom::UNKNOWN_TYPE)
	TEST_EQUAL(atom2->getElement(), Element::UNKNOWN)
	TEST_EQUAL(atom2->getPosition(), null_vector)
	TEST_EQUAL(atom2->getVelocity(), null_vector)
	TEST_EQUAL(atom2->getForce(), null_vector)
	TEST_REAL_EQUAL(atom2->getCharge(), 0.0)
	TEST_REAL_EQUAL(atom2->getRadius(), 0.0)
	TEST_EQUAL(atom2->countBonds(), 0)
	TEST_EQUAL(atom->countBonds(), 0)
RESULT

CHECK(BALL_CREATE_DEEP(Atom))
	delete atom2;
	atom2 = (Atom*)atom->create(false);
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getName(), atom->getName())
	TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
	TEST_EQUAL(atom2->getPosition(), atom->getPosition())
	TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom2->getForce(), atom->getForce())
	TEST_EQUAL(atom2->getType(), atom->getType())
	TEST_EQUAL(atom2->countBonds(), atom->countBonds())
	TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
	delete atom2;
	atom2 = (Atom*)atom->create(true);
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getName(), atom->getName())
	TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
	TEST_EQUAL(atom2->getPosition(), atom->getPosition())
	TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom2->getForce(), atom->getForce())
	TEST_EQUAL(atom2->getType(), atom->getType())
	TEST_EQUAL(atom2->countBonds(), atom->countBonds())
	TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(void destroy() throw())
	atom->createBond(*atom2);
	atom2->destroy();
	TEST_EQUAL(atom2->getName(), "")
	TEST_EQUAL(atom2->getType(), Atom::UNKNOWN_TYPE)
	TEST_EQUAL(atom2->getElement(), Element::UNKNOWN)
	TEST_EQUAL(atom2->getPosition(), null_vector)
	TEST_EQUAL(atom2->getVelocity(), null_vector)
	TEST_EQUAL(atom2->getForce(), null_vector)
	TEST_REAL_EQUAL(atom2->getCharge(), 0.0)
	TEST_REAL_EQUAL(atom2->getRadius(), 0.0)
	TEST_EQUAL(atom2->countBonds(), 0)
	TEST_EQUAL(atom->countBonds(), 0)
RESULT

CHECK(void set(const Atom& atom, bool deep = true) throw())
	atom2->set(*atom);
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getName(), atom->getName())
	TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
	TEST_EQUAL(atom2->getPosition(), atom->getPosition())
	TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom2->getForce(), atom->getForce())
	TEST_EQUAL(atom2->getType(), atom->getType())
	TEST_EQUAL(atom2->countBonds(), atom->countBonds())
	TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(Atom& operator = (const Atom& atom) throw())
	atom2->clear();
	*atom2 = *atom;
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getName(), atom->getName())
	TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
	TEST_EQUAL(atom2->getPosition(), atom->getPosition())
	TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom2->getForce(), atom->getForce())
	TEST_EQUAL(atom2->getType(), atom->getType())
	TEST_EQUAL(atom2->countBonds(), atom->countBonds())
	TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT

CHECK(void get(Atom& atom, bool deep = true) const throw())
	atom2->clear();
	atom2->get(*atom);
	TEST_NOT_EQUAL(atom2, 0)
	TEST_EQUAL(atom2->getName(), atom->getName())
	TEST_REAL_EQUAL(atom2->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom2->getRadius(), atom->getRadius())
	TEST_EQUAL(atom2->getPosition(), atom->getPosition())
	TEST_EQUAL(atom2->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom2->getForce(), atom->getForce())
	TEST_EQUAL(atom2->getType(), atom->getType())
	TEST_EQUAL(atom2->countBonds(), atom->countBonds())
	TEST_NOT_EQUAL(atom2->getHandle(), atom->getHandle())
RESULT
delete atom2;
atom2 = 0;

Atom* atom3 = 0;
Atom* atom4 = 0;
CHECK(void swap(Atom& atom) throw())
	atom2 = new Atom(PTE[Element::LITHIUM],
												"TESTNAME2",
												"TESTTYPE2",
												23,
												test_vector,
												test_vector,
												null_vector,
												3.23456,
												4.34567);

	atom3 = new Atom(*atom);
	atom4 = new Atom(*atom2);
	atom3->swap(*atom4);
	TEST_EQUAL(atom4->getName(), atom->getName())
	TEST_REAL_EQUAL(atom4->getCharge(), atom->getCharge())
	TEST_REAL_EQUAL(atom4->getRadius(), atom->getRadius())
	TEST_EQUAL(atom4->getPosition(), atom->getPosition())
	TEST_EQUAL(atom4->getVelocity(), atom->getVelocity())
	TEST_EQUAL(atom4->getForce(), atom->getForce())
	TEST_EQUAL(atom4->getType(), atom->getType())
	TEST_EQUAL(atom4->countBonds(), atom->countBonds())
	TEST_EQUAL(atom3->getName(), atom2->getName())
	TEST_REAL_EQUAL(atom3->getCharge(), atom2->getCharge())
	TEST_REAL_EQUAL(atom3->getRadius(), atom2->getRadius())
	TEST_EQUAL(atom3->getPosition(), atom2->getPosition())
	TEST_EQUAL(atom3->getVelocity(), atom2->getVelocity())
	TEST_EQUAL(atom3->getForce(), atom2->getForce())
	TEST_EQUAL(atom3->getType(), atom2->getType())
	TEST_EQUAL(atom3->countBonds(), atom2->countBonds())	
RESULT

delete atom;
delete atom2;
delete atom3;
delete atom4;


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
