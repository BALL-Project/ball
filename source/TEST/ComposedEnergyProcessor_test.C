// $Id: ComposedEnergyProcessor_test.C,v 1.3 2001/07/16 00:34:48 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/ENERGY/composedEnergyProcessor.h>
#include <BALL/SOLVATION/pair6_12InteractionEnergyProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
///////////////////////////

START_TEST(ComposedEnergyProcessor_test, "$Id: ComposedEnergyProcessor_test.C,v 1.3 2001/07/16 00:34:48 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

/** Test class.
 *  The energy is calculated as:
 *  (fragment_.countAtomContainers() + 1 + change) * fragment_.countAtoms()
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


MyEnergyProcessor* pep1;
MyEnergyProcessor* pep2;
System S;
double result(0);

CHECK(Preperations)
	pep1 = new MyEnergyProcessor;
	pep1->change = 1.0;
 	pep2 = new MyEnergyProcessor;
	pep2->change = 2.0;

	HINFile f("data/AnisotropyShiftProcessor_test.hin");   
  f >> S; 
  f.close();

	TEST_EQUAL(S.apply(*pep1), true)
	TEST_EQUAL(pep1->getEnergy(), (S.countAtomContainers() + 1 + 1) * S.countAtoms()) 	
	TEST_EQUAL(pep1->getEnergy(), 217)
	TEST_EQUAL(S.apply(*pep2), true)
	TEST_EQUAL(pep2->getEnergy(), (S.countAtomContainers() + 2 + 1) * S.countAtoms()) 	
	TEST_EQUAL(pep2->getEnergy(), 248)
	result = pep1->getEnergy() + pep2->getEnergy();

	pep1->change = 1.0;
	pep2->change = 2.0;
 RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor())
	ComposedEnergyProcessor* cep = new ComposedEnergyProcessor;
	TEST_NOT_EQUAL(cep, 0)
	TEST_EQUAL(cep->isValid(), false)
	TEST_REAL_EQUAL(cep->getEnergy(), 0)
RESULT

ComposedEnergyProcessor cep;

CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor(const ComposedEnergyProcessor& composed_energy_proc))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor(EnergyProcessorList proc_list))
  //BAUSTELLE
RESULT


CHECK(ComposedEnergyProcessor::~ComposedEnergyProcessor())
	ComposedEnergyProcessor* cep = new ComposedEnergyProcessor;
	delete cep;
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


CHECK(ComposedEnergyProcessor::addComponent(EnergyProcessor* proc))
	cep.addComponent(pep1);
	cep.addComponent(pep2);
RESULT


CHECK(ComposedEnergyProcessor::removeComponent(EnergyProcessor* proc))
  cep.removeComponent(pep2);
	cep.addComponent(pep2);
RESULT


CHECK(ComposedEnergyProcessor::finish())
	TEST_EQUAL(S.apply(cep), true)
	TEST_REAL_EQUAL(cep.getEnergy(), result)
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
