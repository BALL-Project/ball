// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ConjugateGradientMinimizer_test.C,v 1.4 2003/02/03 22:30:55 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/PTE.h>
///////////////////////////

START_TEST(ConjugateGradienMinimizer, "$Id: ConjugateGradientMinimizer_test.C,v 1.4 2003/02/03 22:30:55 oliver Exp $")

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

System S;
AmberFF FF(S);
	
ConjugateGradientMinimizer*	em;
CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer())
	em = new ConjugateGradientMinimizer();
	TEST_NOT_EQUAL(em, 0)
RESULT	

CHECK(ConjugateGradientMinimizer::~ConjugateGradientMinimizer())
	delete em;
RESULT

CHECK(ConjugateGradientMinimizer::getForceField())
	ConjugateGradientMinimizer em;
	TEST_EQUAL(em.getForceField(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField&))
	TEST_EQUAL(FF.isValid(), true)
	ConjugateGradientMinimizer em(FF);
	TEST_EQUAL(em.getForceField(), &FF)
RESULT

CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer(ForceField&, const Options&))
	Options options;
	options.set("ABC", "DEF");
	em = new ConjugateGradientMinimizer(FF, options);
	TEST_EQUAL(em->getForceField(), &FF)
	TEST_EQUAL(em->options["ABC"], "DEF")
	delete em;
RESULT

CHECK(ConjugateGradientMinimizer::ConjugateGradientMinimizer(const ConjugateGradientMinimizer&, bool))
	ConjugateGradientMinimizer em1;
	ConjugateGradientMinimizer em2(em1);
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	ConjugateGradientMinimizer em3(em1);
	test = (em1 == em3);
	TEST_EQUAL(test, true)
	em1.setup(FF);
RESULT

CHECK(ConjugateGradientMinimizer::operator = (const ConjugateGradientMinimizer&))
	ConjugateGradientMinimizer em1;
	ConjugateGradientMinimizer em2 = em1;
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	ConjugateGradientMinimizer em3 = em1;
	test = (em1 == em3);
	TEST_EQUAL(test, true)
RESULT

CHECK(ConjugateGradientMinimizer::isValid() const)
	em = new ConjugateGradientMinimizer;
	TEST_EQUAL(em->isValid(), false)
	delete em;
	em = new ConjugateGradientMinimizer(FF);
	TEST_EQUAL(em->isValid(), true)
	delete em;
RESULT

CHECK(ConjugateGradientMinimizer::setup(ForceField&))
	ConjugateGradientMinimizer e_min(FF);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
RESULT

CHECK(ConjugateGradientMinimizer::setup(ForceField&, const Options&))
	Options options;
	options.setInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 3456);
	ConjugateGradientMinimizer e_min(FF, options);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 3456)
RESULT

CHECK(ConjugateGradientMinimizer::specificSetup())
	ConjugateGradientMinimizer em(FF);
	TEST_EQUAL(em.specificSetup(), true)
	// specificSetup() shouldn't do anything except returning true
RESULT

CHECK(ConjugateGradientMinimizer::getNumberOfIteration() const)
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIteration(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::setNumberOfIteration(Size))
	ConjugateGradientMinimizer e_min;
	e_min.setNumberOfIteration(4);
	TEST_EQUAL(e_min.getNumberOfIteration(), 4)
RESULT

CHECK(ConjugateGradientMinimizer::getMaximalNumberOfIterations())
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getMaximalNumberOfIterations(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::setMaximalNumberOfIterations(Size))
	ConjugateGradientMinimizer e_min;
	e_min.setMaximalNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaximalNumberOfIterations(), 2000)
RESULT

CHECK(ConjugateGradientMinimizer::getEnergyOutputFrequency() const)
	ConjugateGradientMinimizer e_min2;
	TEST_EQUAL(e_min2.getEnergyOutputFrequency(), 0)
RESULT

CHECK(ConjugateGradientMinimizer::setEnergyOutputFrequency(Size))
	ConjugateGradientMinimizer e_min;
	e_min.setEnergyOutputFrequency(8);
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 8)
RESULT

CHECK(ConjugateGradientMinimizer::getEnergyDifferenceBound() const)
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 0.0)
RESULT

CHECK(ConjugateGradientMinimizer::setEnergyDifferenceBound(float))
	ConjugateGradientMinimizer e_min;
	e_min.setEnergyDifferenceBound(9.0);
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 9.0)
RESULT

CHECK(ConjugateGradientMinimizer::getMaximumDisplacement() const)
	ConjugateGradientMinimizer e_min;
	TEST_EQUAL(e_min.getMaximumDisplacement(), 0.0)
RESULT

CHECK(ConjugateGradientMinimizer::setMaximumDisplacement(float))
	ConjugateGradientMinimizer e_min;
	e_min.setMaximumDisplacement(56.0);
	TEST_EQUAL(e_min.getMaximumDisplacement(), 56.0)
RESULT

CHECK(ConjugateGradientMinimizer::minimize(Size, bool))
	System S;
	Molecule* m = new Molecule;
	Atom* a1 = new Atom;
	Atom* a2 = new Atom;
	S.insert(*m);
	m->insert(*a1);
	m->insert(*a2);
	a1->setPosition(Vector3(0, 0, 0));
	a2->setPosition(Vector3(1, 0, 0));
	a1->setElement(PTE[Element::C]);
	a2->setElement(PTE[Element::C]);
	a1->setTypeName("C");
	a2->setTypeName("C");
	a1->setCharge(0.0);
	a2->setCharge(0.0);
	FF.options[AmberFF::Option::ASSIGN_CHARGES] = "false";
	FF.setup(S);

	TEST_REAL_EQUAL(FF.isValid(), true)
	FF.updateEnergy();
	FF.updateForces();
	PRECISION(10)
	TEST_REAL_EQUAL(FF.getEnergy(), 3.42854e6)

	PRECISION(1e-4)
	ConjugateGradientMinimizer cgm(FF);
	cgm.setEnergyOutputFrequency(1);
	cgm.setMaxGradient(0.01);
	TEST_EQUAL(cgm.isValid(), true)
	TEST_EQUAL(cgm.minimize(20), true)
	FF.updateEnergy();
	FF.updateForces();

	TEST_REAL_EQUAL(FF.getEnergy(), -0.359813)	
	TEST_REAL_EQUAL(a1->getPosition().getDistance(a2->getPosition()), 3.81244)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
