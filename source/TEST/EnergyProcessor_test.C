// $Id: EnergyProcessor_test.C,v 1.1 2000/08/31 14:16:58 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/ENERGY/energyProcessor.h>

///////////////////////////

START_TEST(class_name, "$Id: EnergyProcessor_test.C,v 1.1 2000/08/31 14:16:58 anker Exp $")

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
