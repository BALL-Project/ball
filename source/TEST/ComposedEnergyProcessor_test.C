// $Id: ComposedEnergyProcessor_test.C,v 1.4 2001/07/16 11:14:31 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/ENERGY/composedEnergyProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
///////////////////////////

START_TEST(ComposedEnergyProcessor_test, "$Id: ComposedEnergyProcessor_test.C,v 1.4 2001/07/16 11:14:31 amoll Exp $")

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
    
	virtual bool start() throw()
	{
		numberOfFragments = 0;
		return true;
	}
	
  virtual Processor::Result operator () (AtomContainer& fragment) throw() 
  {
    numberOfFragments += 1;
    EnergyProcessor::operator() (fragment);
    return Processor::CONTINUE;
  }
  
  virtual bool finish() throw() 
  {
    energy_ = fragment_->countAtoms() * (numberOfFragments + change); 
    return true;
  }

  float change; 
	float numberOfFragments;
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
 RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor())
	ComposedEnergyProcessor* cep = new ComposedEnergyProcessor;
	TEST_NOT_EQUAL(cep, 0)
	TEST_EQUAL(cep->isValid(), true)
	TEST_REAL_EQUAL(cep->getEnergy(), 0)
RESULT

CHECK(ComposedEnergyProcessor::~ComposedEnergyProcessor())
	ComposedEnergyProcessor* cep = new ComposedEnergyProcessor;
	delete cep;
RESULT


ComposedEnergyProcessor cep;

CHECK(ComposedEnergyProcessor::addComponent(EnergyProcessor* proc))
	cep.addComponent(pep1);
	cep.addComponent(pep2);
	TEST_EQUAL(cep.getNumberOfEnergyProcessors(), 2)
RESULT


CHECK(ComposedEnergyProcessor::removeComponent(EnergyProcessor* proc))
  cep.removeComponent(pep2);
	TEST_EQUAL(cep.getNumberOfEnergyProcessors(), 1)
	cep.addComponent(pep2);
	TEST_EQUAL(cep.getNumberOfEnergyProcessors(), 2)
RESULT


CHECK(ComposedEnergyProcessor::finish())
	TEST_EQUAL(S.apply(cep), true)
	TEST_REAL_EQUAL(cep.getEnergy(), result)
RESULT


ComposedEnergyProcessor cep2;
CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor(EnergyProcessorList proc_list))
  EnergyProcessorList epl;
	epl.push_back(pep1);
	epl.push_back(pep2);
	cep2 = ComposedEnergyProcessor(epl);
	TEST_EQUAL(cep2.isValid(), true)
	TEST_EQUAL(cep2.getNumberOfEnergyProcessors(), 2)
	S.apply(cep2);

	TEST_EQUAL(cep2.isValid(), true)
	TEST_REAL_EQUAL(cep2.getEnergy(), result)
RESULT


CHECK(ComposedEnergyProcessor::clear())
  cep2.clear();
	cep2 = cep;
	TEST_REAL_EQUAL(cep2.getEnergy(), result)
	cep2.clear();
	TEST_REAL_EQUAL(cep2.getEnergy(), 0)
RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor& operator = 
			(const ComposedEnergyProcessor& proc))
  cep2.clear();
	cep2 = cep;
	TEST_REAL_EQUAL(cep2.getEnergy(), result)
	S.apply(cep2);
	TEST_REAL_EQUAL(cep2.getEnergy(), result)
RESULT


CHECK(ComposedEnergyProcessor::ComposedEnergyProcessor(const ComposedEnergyProcessor& composed_energy_proc))
  ComposedEnergyProcessor cep2(cep);
	TEST_REAL_EQUAL(cep2.getEnergy(), result)
	S.apply(cep2);
	TEST_REAL_EQUAL(cep2.getEnergy(), result)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
