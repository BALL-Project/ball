// $Id: EnergyProcessor_test.C,v 1.4 2001/07/15 22:15:20 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
///////////////////////////

START_TEST(EnergyProcessor, "$Id: EnergyProcessor_test.C,v 1.4 2001/07/15 22:15:20 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HINFile f("data/AnisotropyShiftProcessor_test.hin");
System S;
f >> S;


CHECK(EnergyProcessor::EnergyProcessor())
	EnergyProcessor* ep = new EnergyProcessor;
	TEST_NOT_EQUAL(ep, 0)
	TEST_EQUAL(ep->isValid(), false)
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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
