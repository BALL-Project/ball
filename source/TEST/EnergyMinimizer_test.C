// $Id: EnergyMinimizer_test.C,v 1.2 2001/12/30 13:28:57 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
///////////////////////////

START_TEST(EnergyMinimizer, "$Id: EnergyMinimizer_test.C,v 1.2 2001/12/30 13:28:57 sturm Exp $")

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
//?????
RESULT

CHECK(EnergyMinimizer::operator = (const EnergyMinimizer&))
//?????
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
//?????
RESULT

CHECK(EnergyMinimizer::setup(ForceField&, const Options&))
//?????
RESULT

CHECK(EnergyMinimizer::specificSetup())
//?????
RESULT

CHECK(EnergyMinimizer::getNumberOfIteration() const)
//?????
RESULT

CHECK(EnergyMinimizer::setNumberOfIteration(Size))
//?????
RESULT

CHECK(EnergyMinimizer::getMaximalNumberOfIterations())
//?????
RESULT

CHECK(EnergyMinimizer::setMaximalNumberOfIterations(Size))
//?????
RESULT

CHECK(EnergyMinimizer::getSnapshot())
//?????
RESULT

CHECK(EnergyMinimizer::setSnapshot(Snapshot&))
//?????
RESULT

CHECK(EnergyMinimizer::getEnergyOutputFrequency() const)
//?????
RESULT

CHECK(EnergyMinimizer::setEnergyOutputFrequency(Size))
//?????
RESULT

CHECK(EnergyMinimizer::getEnergyDifferenceBound() const)
//?????
RESULT

CHECK(EnergyMinimizer::setEnergyDifferenceBound(float))
//?????
RESULT

CHECK(EnergyMinimizer::getMaximalShift() const)
//?????
RESULT

CHECK(EnergyMinimizer::setMaximalShift(float))
//?????
RESULT

CHECK(SnapshotMinimizer::getSnapshotOutputFrequency() const)
//?????
RESULT

CHECK(SnapshotMinimizer::setSnapshotOutputFrequency(Size))
//?????
RESULT

CHECK(EnergyMinimizer::minimize(Size, bool))
//?????
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
