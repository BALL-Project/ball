// $Id: PierottiCavFreeEnergyProcessor_test.C,v 1.1 2001/09/24 08:43:41 aubertin Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/pierottiCavFreeEnergyProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////

START_TEST(ReissCavFreeEnergyProcessor, "$Id: PierottiCavFreeEnergyProcessor_test.C,v 1.1 2001/09/24 08:43:41 aubertin Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

CHECK(PierottiCavFreeEnergyProcessor::PierottiCavFreeEnergyProcessor())
	PierottiCavFreeEnergyProcessor* proc = new PierottiCavFreeEnergyProcessor;
	TEST_NOT_EQUAL(proc, 0)
	TEST_EQUAL(proc->isValid(), true)
	TEST_REAL_EQUAL(proc->getEnergy(), 0)
RESULT 

CHECK(PierottiCavFreeEnergyProcessor::PierottiCavFreeEnergyProcessor(const PierottiCavFreeEnergyProcessor& proc))
  HINFile f("data/methane.hin");
  System S;
  f >> S;
  f.close();
  PierottiCavFreeEnergyProcessor proc;
  S.apply(proc);
  PierottiCavFreeEnergyProcessor proc2(proc); 
  TEST_EQUAL((proc == proc2), true) 
  bool test = proc.isValid() == proc2.isValid();
  TEST_EQUAL(test, true)
  test = proc.getEnergy() == proc2.getEnergy();
  TEST_EQUAL(test, true)
RESULT
  

CHECK(PierottiCavFreeEnergyProcessor::~PierottiCavFreeEnergyProcessor())
	PierottiCavFreeEnergyProcessor* proc = new PierottiCavFreeEnergyProcessor;
	delete proc;
RESULT


CHECK(PierottiCavFreeEnergyProcessor::clear())
  PierottiCavFreeEnergyProcessor proc;
  proc.clear();
RESULT


CHECK(PierottiCavFreeEnergyProcessor::PierottiCavFreeEnergyProcessor& operator = (const PierottiCavFreeEnergyProcessor& proc) + operator ==)
  HINFile f("data/methane.hin");
  System S;
  f >> S;
  f.close();
  PierottiCavFreeEnergyProcessor proc;
  S.apply(proc);
  PierottiCavFreeEnergyProcessor proc2;
  proc2 = proc;
  TEST_EQUAL((proc  ==  proc2), true)
  bool test = proc.isValid() == proc2.isValid();
  TEST_EQUAL(test, true)
  test = proc.getEnergy() == proc2.getEnergy();
  TEST_EQUAL(test, true) 
RESULT


CHECK(PierottiCavFreeEnergyProcessor::start())
  PierottiCavFreeEnergyProcessor proc; 
  TEST_EQUAL(proc.start(), true)
RESULT


CHECK(PierottiCavFreeEnergyProcessor / Methane)
	PRECISION(0.001)
	HINFile f("data/methane.hin");
	System S;
	f >> S;
	f.close();
	PierottiCavFreeEnergyProcessor proc;
	S.apply(proc);
	TEST_REAL_EQUAL(proc.getEnergy(),20.6908)
	TEST_EQUAL(S.countAtoms(),5)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
