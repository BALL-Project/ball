// $Id: ReissCavFreeEnergyProcessor_test.C,v 1.1 2001/08/30 09:00:45 aubertin Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/reissCavFreeEnergyProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
///////////////////////////

START_TEST(ReissCavFreeEnergyProcessor, "$Id: ReissCavFreeEnergyProcessor_test.C,v 1.1 2001/08/30 09:00:45 aubertin Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


HINFile f("data/methane.hin");
System S;
f >> S;


/*CHECK(EnergyProcessor::EnergyProcessor())
	EnergyProcessor* ep = new EnergyProcessor;
	TEST_NOT_EQUAL(ep, 0)
	TEST_EQUAL(ep->isValid(), true)
	TEST_REAL_EQUAL(ep->getEnergy(), 0)
RESULT

EnergyProcessor ep;

CHECK(EnergyProcessor::EnergyProcessor(const EnergyProcessor& proc))
	EnergyProcessor ep2(ep);
RESULT


CHECK(EnergyProcessor::~EnergyProcessor())
	EnergyProcessor* ep = new EnergyProcessor;
	delete ep;
RESULT


CHECK(EnergyProcessor::clear())
  ep.clear();
RESULT


CHECK(EnergyProcessor::EnergyProcessor& operator = (const EnergyProcessor& proc))
	EnergyProcessor ep2;
	ep2 = ep;
RESULT


CHECK(EnergyProcessor::start())
	TEST_EQUAL(ep.start(), true)
RESULT


CHECK(EnergyProcessor::Processor::Result operator () (BaseFragment& fragment))
  TEST_EQUAL(S.apply(ep), true)
RESULT


CHECK(EnergyProcessor::getEnergy() const )
  TEST_REAL_EQUAL(ep.getEnergy(), 0)
RESULT


CHECK(apply)
	MyEnergyProcessor mep;
	mep.change = 2.0;
	TEST_EQUAL(S.apply(mep), true)
	TEST_EQUAL(S.countAtoms(), 31)
	TEST_EQUAL(S.countAtomContainers(), 5)
	TEST_REAL_EQUAL(mep.getEnergy(), S.countAtoms() * (S.countAtomContainers() + 2 + 1 ))
RESULT
*/

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
