// $Id: EnergyMinimizer_test.C,v 1.1 1999/11/30 19:43:58 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/MOLMEC/MINIMIZATION/energyMinimizer.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/DATATYPE/options.h>
///////////////////////////

START_TEST(EnergyMinimizer, "$Id: EnergyMinimizer_test.C,v 1.1 1999/11/30 19:43:58 oliver Exp $")

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
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::operator = (const EnergyMinimizer&))
//BAUSTELLE
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
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setup(ForceField&, const Options&))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::specificSetup())
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::getNumberOfIteration() const)
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setNumberOfIteration(Size))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::getMaximalNumberOfIterations())
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setMaximalNumberOfIterations(Size))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::getSnapshot())
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setSnapshot(Snapshot&))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::getEnergyOutputFrequency() const)
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setEnergyOutputFrequency(Size))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::getEnergyDifferenceBound() const)
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setEnergyDifferenceBound(float))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::getMaximalShift() const)
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::setMaximalShift(float))
//BAUSTELLE
RESULT

CHECK(SnapshotMinimizer::getSnapshotOutputFrequency() const)
//BAUSTELLE
RESULT

CHECK(SnapshotMinimizer::setSnapshotOutputFrequency(Size))
//BAUSTELLE
RESULT

CHECK(EnergyMinimizer::minimize(Size, bool))
//BAUSTELLE
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
