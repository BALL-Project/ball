// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SteepestDescentMinimizer_test.C,v 1.3 2003/02/04 14:27:03 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/PTE.h>
///////////////////////////

START_TEST(ConjugateGradienMinimizer, "$Id: SteepestDescentMinimizer_test.C,v 1.3 2003/02/04 14:27:03 oliver Exp $")

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

System S;
AmberFF FF(S);
	
SteepestDescentMinimizer*	em;
CHECK(SteepestDescentMinimizer::SteepestDescentMinimizer())
	em = new SteepestDescentMinimizer;
	TEST_NOT_EQUAL(em, 0)
RESULT	

CHECK(SteepestDescentMinimizer::~SteepestDescentMinimizer())
	delete em;
RESULT

CHECK(SteepestDescentMinimizer::getForceField())
	SteepestDescentMinimizer em;
	TEST_EQUAL(em.getForceField(), 0)
RESULT

CHECK(SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField&))
	TEST_EQUAL(FF.isValid(), true)
	SteepestDescentMinimizer em(FF);
	TEST_EQUAL(em.getForceField(), &FF)
RESULT

CHECK(SteepestDescentMinimizer::SteepestDescentMinimizer(ForceField&, const Options&))
	Options options;
	options.set("ABC", "DEF");
	em = new SteepestDescentMinimizer(FF, options);
	TEST_EQUAL(em->getForceField(), &FF)
	TEST_EQUAL(em->options["ABC"], "DEF")
	delete em;
RESULT

CHECK(SteepestDescentMinimizer::SteepestDescentMinimizer(const SteepestDescentMinimizer&, bool))
	SteepestDescentMinimizer em1;
	SteepestDescentMinimizer em2(em1);
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	SteepestDescentMinimizer em3(em1);
	test = (em1 == em3);
	TEST_EQUAL(test, true)
	em1.setup(FF);
RESULT

CHECK(SteepestDescentMinimizer::operator = (const SteepestDescentMinimizer&))
	SteepestDescentMinimizer em1;
	SteepestDescentMinimizer em2 = em1;
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	SteepestDescentMinimizer em3 = em1;
	test = (em1 == em3);
	TEST_EQUAL(test, true)
RESULT

CHECK(SteepestDescentMinimizer::isValid() const)
	em = new SteepestDescentMinimizer;
	TEST_EQUAL(em->isValid(), false)
	delete em;
	em = new SteepestDescentMinimizer(FF);
	TEST_EQUAL(em->isValid(), true)
	delete em;
RESULT

CHECK(SteepestDescentMinimizer::setup(ForceField&))
	SteepestDescentMinimizer e_min(FF);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
RESULT

CHECK(SteepestDescentMinimizer::setup(ForceField&, const Options&))
	Options options;
	options.setInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 3456);
	SteepestDescentMinimizer e_min(FF, options);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 3456)
RESULT

CHECK(SteepestDescentMinimizer::specificSetup())
	SteepestDescentMinimizer em(FF);
	TEST_EQUAL(em.specificSetup(), true)
	// specificSetup() shouldn't do anything except returning true
RESULT

CHECK(SteepestDescentMinimizer::getNumberOfIteration() const)
	SteepestDescentMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIterations(), 0)
RESULT

CHECK(SteepestDescentMinimizer::setNumberOfIteration(Size))
	SteepestDescentMinimizer e_min;
	e_min.setNumberOfIterations(4);
	TEST_EQUAL(e_min.getNumberOfIterations(), 4)
RESULT

CHECK(SteepestDescentMinimizer::getMaxNumberOfIterations())
	SteepestDescentMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 0)
RESULT

CHECK(SteepestDescentMinimizer::setMaxNumberOfIterations(Size))
	SteepestDescentMinimizer e_min;
	e_min.setMaxNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 2000)
RESULT

CHECK(SteepestDescentMinimizer::getEnergyOutputFrequency() const)
	SteepestDescentMinimizer e_min2;
	TEST_EQUAL(e_min2.getEnergyOutputFrequency(), 0)
RESULT

CHECK(SteepestDescentMinimizer::setEnergyOutputFrequency(Size))
	SteepestDescentMinimizer e_min;
	e_min.setEnergyOutputFrequency(8);
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 8)
RESULT

CHECK(SteepestDescentMinimizer::getEnergyDifferenceBound() const)
	SteepestDescentMinimizer e_min;
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 0.0)
RESULT

CHECK(SteepestDescentMinimizer::setEnergyDifferenceBound(float))
	SteepestDescentMinimizer e_min;
	e_min.setEnergyDifferenceBound(9.0);
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 9.0)
RESULT

CHECK(SteepestDescentMinimizer::getMaximumDisplacement() const)
	SteepestDescentMinimizer e_min;
	TEST_EQUAL(e_min.getMaximumDisplacement(), 0.0)
RESULT

CHECK(SteepestDescentMinimizer::setMaximumDisplacement(float))
	SteepestDescentMinimizer e_min;
	e_min.setMaximumDisplacement(56.0);
	TEST_EQUAL(e_min.getMaximumDisplacement(), 56.0)
RESULT

CHECK(SteepestDescentMinimizer::minimize(Size, bool))
	System S;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	S.insert(*m);
	m->insert(*a1);
	m->insert(*a2);
	a1->setPosition(Vector3(-0.5, 0, 0));
	a2->setPosition(Vector3(0.5, 0, 0));
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::C]);
	a1->setTypeName("C");
	a2->setTypeName("C");
	a1->setCharge(0.0);
	a2->setCharge(0.0);
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);
	TEST_EQUAL(FF.isValid(), true)
	PRECISION(1e-4)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(10)
	TEST_REAL_EQUAL(FF.getEnergy(), 3.42854e+06)
	SteepestDescentMinimizer cgm(FF);
	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(0.01);
	TEST_EQUAL(cgm.isValid(), true)
	TEST_EQUAL(cgm.minimize(20), true)
	
	FF.updateEnergy();
	FF.updateForces();

	PRECISION(1e-3)
  TEST_REAL_EQUAL(FF.getEnergy(), -0.359813)
	PRECISION(7e-3)
  TEST_REAL_EQUAL(a1->getPosition().getDistance(a2->getPosition()), 3.81244)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
