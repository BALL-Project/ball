// $Id: EnergyProcessor_test.C,v 1.6 2001/07/16 12:54:13 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
///////////////////////////

START_TEST(EnergyProcessor, "$Id: EnergyProcessor_test.C,v 1.6 2001/07/16 12:54:13 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


/** Test class.
 * 	The energy is calculated as:
 * 	(fragment_.countAtomContainers() + 1 + change) * fragment_.countAtoms()
 */
 class MyEnergyProcessor
	: public EnergyProcessor
{
	public:
		
	MyEnergyProcessor()
		: change(0)
	{}
		
	virtual Processor::Result operator () (AtomContainer& fragment) throw()
	{
		change += 1;
		EnergyProcessor::operator() (fragment);
		return Processor::CONTINUE;
	}
	
	virtual bool finish() throw()
	{
		energy_ = fragment_->countAtoms() * change;
		return true;
	}

	float change;
};

HINFile f("data/AnisotropyShiftProcessor_test.hin");
System S;
f >> S;


CHECK(EnergyProcessor::EnergyProcessor())
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

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
