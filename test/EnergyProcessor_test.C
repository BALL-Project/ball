// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/CONCEPT/processor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
///////////////////////////

START_TEST(EnergyProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


/** Test class.
 * 	The energy is calculated as:
 * 	(fragment_.countAtomContainers() + 1) * fragment_.countAtoms()
 * 	The additive 1 comes from the fact that countAtomContainers() does not
 * 	count the container from which it was called while operator () () runs
 * 	over *all* containers including the System which it was applied to.
 */
 class MyEnergyProcessor
	: public EnergyProcessor
{
	public:
		
	MyEnergyProcessor()
		: count(0)
	{
	}
		
	virtual bool start() throw()
	{
		count = 0;
		return true;
	}

	virtual Processor::Result operator () (AtomContainer& fragment) throw()
	{
		count++;
		EnergyProcessor::operator() (fragment);
		return BALL::Processor::CONTINUE;
	}
	
	virtual bool finish() throw()
	{
		energy_ = fragment_->countAtoms() * count;
		return true;
	}

	Size count;
};

HINFile f(BALL_TEST_DATA_PATH(AnisotropyShiftProcessor_test.hin));
System S;
f >> S;

EnergyProcessor* ep_ptr = 0;

CHECK(EnergyProcessor::EnergyProcessor())
	ep_ptr = new EnergyProcessor;
	TEST_NOT_EQUAL(ep_ptr, 0)
	TEST_EQUAL(ep_ptr->isValid(), true)
	TEST_REAL_EQUAL(ep_ptr->getEnergy(), 0)
RESULT

CHECK(EnergyProcessor::~EnergyProcessor())
	delete ep_ptr;
RESULT

bool test = false;

CHECK(EnergyProcessor::EnergyProcessor(const EnergyProcessor& proc))
	EnergyProcessor ep;
	EnergyProcessor ep2(ep);
	test = (ep == ep2);
	TEST_EQUAL(test, true)
RESULT

CHECK(EnergyProcessor::clear())
	EnergyProcessor ep1;
  ep1.clear();
	EnergyProcessor ep2;
	test = (ep1 == ep2);
	TEST_EQUAL(test, true)
RESULT


CHECK(EnergyProcessor::EnergyProcessor& operator = (const EnergyProcessor& proc))
	Fragment fragment;
	EnergyProcessor ep1;
	EnergyProcessor ep2 = ep1;
	test = (ep1 == ep2);
	TEST_EQUAL(test, true)
RESULT


CHECK(EnergyProcessor::start())
	EnergyProcessor ep;
	TEST_EQUAL(ep.start(), true)
RESULT


CHECK(EnergyProcessor::Processor::Result operator () (BaseFragment& fragment))
	EnergyProcessor ep;
  TEST_EQUAL(S.apply(ep), true)
RESULT


CHECK(EnergyProcessor::getEnergy() const )
	EnergyProcessor ep;
  TEST_REAL_EQUAL(ep.getEnergy(), 0)
RESULT


CHECK(apply)
	MyEnergyProcessor mep;
	TEST_EQUAL(S.apply(mep), true)
	TEST_EQUAL(S.countAtoms(), 31)
	TEST_EQUAL(S.countAtomContainers(), 5)
	TEST_REAL_EQUAL(mep.getEnergy(), S.countAtoms() * (S.countAtomContainers() + 1))
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
