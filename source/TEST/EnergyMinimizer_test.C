// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>

///////////////////////////

START_TEST(EnergyMinimizer)

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

System S;
Molecule* M = new Molecule;
M->insert(*new Atom);
S.insert(*M);
AmberFF FF(S);
	
EnergyMinimizer* em;
CHECK(EnergyMinimizer::EnergyMinimizer())
	em = new EnergyMinimizer();
	TEST_NOT_EQUAL(em, 0)
RESULT	

CHECK(EnergyMinimizer::~EnergyMinimizer())
	delete em;
RESULT

CHECK(EnergyMinimizer::getForceField())
	EnergyMinimizer em;
	TEST_EQUAL(em.getForceField(), 0)
RESULT

CHECK(EnergyMinimizer::EnergyMinimizer(ForceField&))
	EnergyMinimizer em(FF);
	TEST_EQUAL(em.getForceField(), &FF)
RESULT

CHECK(EnergyMinimizer::EnergyMinimizer(ForceField&, const Options&))
	Options options;
	options.set("ABC", "DEF");
	em = new EnergyMinimizer(FF, options);
	TEST_EQUAL(em->getForceField(), &FF)
	TEST_EQUAL(em->options["ABC"], "DEF")
	delete em;
RESULT

CHECK(EnergyMinimizer::EnergyMinimizer(const EnergyMinimizer&, bool))
	EnergyMinimizer em1;
	EnergyMinimizer em2(em1);
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	EnergyMinimizer em3(em1);
	test = (em1 == em3);
	TEST_EQUAL(test, true)
	em1.setup(FF);
RESULT

CHECK(EnergyMinimizer::operator = (const EnergyMinimizer&))
	EnergyMinimizer em1;
	EnergyMinimizer em2 = em1;
	bool test = (em1 == em2);
	TEST_EQUAL(test, true)
	em1.setup(FF);
	EnergyMinimizer em3 = em1;
	test = (em1 == em3);
	TEST_EQUAL(test, true)
RESULT

CHECK(EnergyMinimizer::isValid() const)
	em = new EnergyMinimizer;
	TEST_EQUAL(em->isValid(), false)
	delete em;
	em = new EnergyMinimizer(FF);
	TEST_EQUAL(em->isValid(), true)
	delete em;
RESULT

CHECK(EnergyMinimizer::setup(ForceField&))
	EnergyMinimizer e_min(FF);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
RESULT

CHECK(EnergyMinimizer::setup(ForceField&, const Options&))
	Options options;
	options.setInteger(EnergyMinimizer::Option::ENERGY_OUTPUT_FREQUENCY, 3456);
	EnergyMinimizer e_min(FF, options);
	TEST_EQUAL(e_min.getForceUpdateCounter(), 0)
	TEST_EQUAL(e_min.getEnergyUpdateCounter(), 0)
	bool test = (e_min.getForceField() == &FF);
	TEST_EQUAL(test, true)
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 3456)
RESULT

CHECK(EnergyMinimizer::specificSetup())
	EnergyMinimizer em;
	TEST_EQUAL(em.specificSetup(),true)
	// specificSetup() shouldn't do anything except returning true
RESULT

CHECK(EnergyMinimizer::getNumberOfIterations() const)
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIterations(), 0)
RESULT

CHECK(EnergyMinimizer::setNumberOfIterations(Size))
	EnergyMinimizer e_min;
	e_min.setNumberOfIterations(4);
	TEST_EQUAL(e_min.getNumberOfIterations(), 4)
RESULT

CHECK(EnergyMinimizer::getMaxNumberOfIterations())
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 0)
RESULT

CHECK(EnergyMinimizer::setMaxNumberOfIterations(Size))
	EnergyMinimizer e_min;
	e_min.setMaxNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaxNumberOfIterations(), 2000)
RESULT

CHECK(EnergyMinimizer::getEnergyOutputFrequency() const)
	EnergyMinimizer e_min2;
	TEST_EQUAL(e_min2.getEnergyOutputFrequency(), 0)
RESULT

CHECK(EnergyMinimizer::setEnergyOutputFrequency(Size))
	EnergyMinimizer e_min;
	e_min.setEnergyOutputFrequency(8);
	TEST_EQUAL(e_min.getEnergyOutputFrequency(), 8)
RESULT

CHECK(EnergyMinimizer::getEnergyDifferenceBound() const)
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 0.0)
RESULT

CHECK(EnergyMinimizer::setEnergyDifferenceBound(float))
	EnergyMinimizer e_min;
	e_min.setEnergyDifferenceBound(9.0);
	TEST_EQUAL(e_min.getEnergyDifferenceBound(), 9.0)
RESULT

CHECK(EnergyMinimizer::getMaximumDisplacement() const)
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getMaximumDisplacement(), 0.0)
RESULT

CHECK(EnergyMinimizer::setMaximumDisplacement(float))
	EnergyMinimizer e_min;
	e_min.setMaximumDisplacement(56.0);
	TEST_EQUAL(e_min.getMaximumDisplacement(), 56.0)
RESULT

CHECK(EnergyMinimizer::minimize(Size, bool))
	EnergyMinimizer em;
	TEST_EXCEPTION(Exception::NotImplemented, em.minimize())
	// this shouldn't do anything
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
