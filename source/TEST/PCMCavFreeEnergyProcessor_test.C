// $Id: PCMCavFreeEnergyProcessor_test.C,v 1.1 2001/09/24 07:53:28 aubertin Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/PCMCavFreeEnergyProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////

START_TEST(ReissCavFreeEnergyProcessor, "$Id: PCMCavFreeEnergyProcessor_test.C,v 1.1 2001/09/24 07:53:28 aubertin Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(PCMCavFreeEnergyProcessor::PCMCavFreeEnergyProcessor())
	PCMCavFreeEnergyProcessor* proc = new PCMCavFreeEnergyProcessor;
	TEST_NOT_EQUAL(proc, 0)
	TEST_EQUAL(proc->isValid(), true)
	TEST_REAL_EQUAL(proc->getEnergy(), 0)
RESULT 

CHECK(PCMCavFreeEnergyProcessor::PCMCavFreeEnergyProcessor(const PCMCavFreeEnergyProcessor& proc))
  HINFile f("data/methane.hin");
  System S;
  f >> S;
  f.close();
  PCMCavFreeEnergyProcessor proc;
  S.apply(proc);
  PCMCavFreeEnergyProcessor proc2(proc); 
  TEST_EQUAL((proc == proc2), true) 
  bool test = proc.isValid() == proc2.isValid();
  TEST_EQUAL(test, true)
  test = proc.getEnergy() == proc2.getEnergy();
  TEST_EQUAL(test, true)
RESULT
  

CHECK(PCMCavFreeEnergyProcessor::~PCMCavFreeEnergyProcessor())
	PCMCavFreeEnergyProcessor* proc = new PCMCavFreeEnergyProcessor;
	delete proc;
RESULT


CHECK(PCMCavFreeEnergyProcessor::clear())
  PCMCavFreeEnergyProcessor proc;
  proc.clear();
RESULT


CHECK(PCMCavFreeEnergyProcessor::PCMCavFreeEnergyProcessor& operator = (const PCMCavFreeEnergyProcessor& proc) + operator ==)
  HINFile f("data/methane.hin");
  System S;
  f >> S;
  f.close();
  PCMCavFreeEnergyProcessor proc;
  S.apply(proc);
  PCMCavFreeEnergyProcessor proc2;
  proc2 = proc;
  TEST_EQUAL((proc  ==  proc2), true)
  bool test = proc.isValid() == proc2.isValid();
  TEST_EQUAL(test, true)
  test = proc.getEnergy() == proc2.getEnergy();
  TEST_EQUAL(test, true) 
RESULT


CHECK(PCMCavFreeEnergyProcessor::start())
  PCMCavFreeEnergyProcessor proc; 
  TEST_EQUAL(proc.start(), true)
RESULT


CHECK(PCMCavFreeEnergyProcessor / Methane)
	PRECISION(0.001)
	HINFile f("data/methane.hin");
	System S;
	f >> S;
	f.close();
	PCMCavFreeEnergyProcessor proc;
	S.apply(proc);
	TEST_REAL_EQUAL(proc.getEnergy(),27.4089)
	TEST_EQUAL(S.countAtoms(),5)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
