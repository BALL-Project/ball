// $Id: EnergyProcessor_test.C,v 1.3 2001/07/15 21:06:32 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/ENERGY/energyProcessor.h>

///////////////////////////

START_TEST(EnergyProcessor, "$Id: EnergyProcessor_test.C,v 1.3 2001/07/15 21:06:32 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	

CHECK(EnergyProcessor::EnergyProcessor())
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::EnergyProcessor(const EnergyProcessor& proc))
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::~EnergyProcessor())
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::destroy())
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::clear())
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::set(const EnergyProcessor& proc))
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::EnergyProcessor& operator = (const EnergyProcessor& proc))
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::start())
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::Processor::Result operator () (BaseFragment& fragment))
  //BAUSTELLE
RESULT


CHECK(EnergyProcessor::getEnergy() const )
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
