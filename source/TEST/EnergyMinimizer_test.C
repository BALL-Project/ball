// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: EnergyMinimizer_test.C,v 1.2.2.1 2003/01/07 13:22:22 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
///////////////////////////

START_TEST(EnergyMinimizer, "$Id: EnergyMinimizer_test.C,v 1.2.2.1 2003/01/07 13:22:22 anker Exp $")

using namespace BALL;

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

AmberFF FF;
	
EnergyMinimizer*	em;
CHECK(EnergyMinimizer::EnergyMinimizer())
em = new EnergyMinimizer();
RESULT	

CHECK(EnergyMinimizer::~EnergyMinimizer())
delete em;
RESULT

CHECK(EnergyMinimizer::EnergyMinimizer(ForceField&))
em = new EnergyMinimizer(FF);
delete em;
RESULT

CHECK(EnergyMinimizer::getForceField())
em = new EnergyMinimizer;
TEST_EQUAL(em->getForceField(), 0)
delete em;
em = new EnergyMinimizer(FF);
TEST_EQUAL(em->getForceField(), &FF)
delete em;
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
//?????
RESULT

CHECK(EnergyMinimizer::getNumberOfIteration() const)
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getNumberOfIteration(), 0)
RESULT

CHECK(EnergyMinimizer::setNumberOfIteration(Size))
	EnergyMinimizer e_min;
	e_min.setNumberOfIteration(4);
	TEST_EQUAL(e_min.getNumberOfIteration(), 4)
RESULT

CHECK(EnergyMinimizer::getMaximalNumberOfIterations())
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getMaximalNumberOfIterations(), 0)
RESULT

CHECK(EnergyMinimizer::setMaximalNumberOfIterations(Size))
	EnergyMinimizer e_min;
	e_min.setMaximalNumberOfIterations(2000);
	TEST_EQUAL(e_min.getMaximalNumberOfIterations(), 2000)
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

CHECK(EnergyMinimizer::getMaximalShift() const)
	EnergyMinimizer e_min;
	TEST_EQUAL(e_min.getMaximalShift(), 0.0)
RESULT

CHECK(EnergyMinimizer::setMaximalShift(float))
	EnergyMinimizer e_min;
	e_min.setMaximalShift(56.0);
	TEST_EQUAL(e_min.getMaximalShift(), 56.0)
RESULT

CHECK(EnergyMinimizer::minimize(Size, bool))
//?????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
