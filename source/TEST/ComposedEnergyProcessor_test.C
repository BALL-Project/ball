// $Id: ComposedEnergyProcessor_test.C,v 1.1 2000/08/31 18:45:31 anker Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

// insert includes here
#include <BALL/ENERGY/composedEnergyProcessor.h>

///////////////////////////

START_TEST(class_name, "$Id: ComposedEnergyProcessor_test.C,v 1.1 2000/08/31 18:45:31 anker Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

///  insert tests for each member function here         
///
	

CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor())
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor(const ComposedEnergyProcessor& composed_energy_proc))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor(EnergyProcessorList proc_list))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::~ComposedEnergyProcessor())
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::destroy())
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::clear())
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::set(const ComposedEnergyProcessor& proc))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor& operator = 
			(const ComposedEnergyProcessor& proc))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::finish())
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::addComponent(EnergyProcessor* proc))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::removeComponent(EnergyProcessor* proc))
  //BAUSTELLE
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
