// $Id: ReissCavFreeEnergyProcessor_test.C,v 1.2 2001/09/01 22:59:55 aubertin Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/SOLVATION/reissCavFreeEnergyProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/ENERGY/energyProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
///////////////////////////

START_TEST(ReissCavFreeEnergyProcessor, "$Id: ReissCavFreeEnergyProcessor_test.C,v 1.2 2001/09/01 22:59:55 aubertin Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(1.0)
CHECK(ReissCavFreeEnergyProcessor / Methane)
HINFile f("data/methane.hin");
System S;
f >> S;
f.close();
ReissCavFreeEnergyProcessor proc;
S.apply(proc);
TEST_EQUAL(proc.getEnergy(),20.68)
TEST_EQUAL(S.countAtoms(),5)
RESULT

CHECK(ReissCavFreeEnergyProcessor::ReissCavFreeEnergyProcessor())
	ReissCavFreeEnergyProcessor* proc = new ReissCavFreeEnergyProcessor;
	TEST_NOT_EQUAL(proc, 0)
	TEST_EQUAL(proc->isValid(), true)
	TEST_REAL_EQUAL(proc->getEnergy(), 0)
RESULT 

CHECK(ReissCavFreeEnergyProcessor::ReissCavFreeEnergyProcessor(const ReissCavFreeEnergyProcessor& proc))
  HINFile f("data/methane.hin");
  System S;
  f >> S;
  f.close();
  ReissCavFreeEnergyProcessor proc;
  S.apply(proc);
  ReissCavFreeEnergyProcessor proc2(proc); 
  //TEST_EQUAL((proc==proc2), true) op == not yet implemented
  bool test = proc.isValid()==proc2.isValid();
  TEST_EQUAL(test, true)
  test = proc.getEnergy()==proc2.getEnergy();
  TEST_EQUAL(test, true)
RESULT
  

CHECK(ReissCavFreeEnergyProcessor::~ReissCavFreeEnergyProcessor())
	ReissCavFreeEnergyProcessor* proc = new ReissCavFreeEnergyProcessor;
	delete proc;
RESULT


CHECK(ReissCavFreeEnergyProcessor::clear())
  ReissCavFreeEnergyProcessor proc;
  proc.clear();
RESULT


CHECK(ReissCavFreeEnergyProcessor::ReissCavFreeEnergyProcessor& operator = (const ReissCavFreeEnergyProcessor& proc))
  HINFile f("data/methane.hin");
  System S;
  f >> S;
  f.close();
  ReissCavFreeEnergyProcessor proc;
  S.apply(proc);
  ReissCavFreeEnergyProcessor proc2;
  //proc2 = proc;
  //TEST_EQUAL((proc == proc2), true)
  //bool test = proc.isValid()==proc2.isValid();
  //TEST_EQUAL(test, true)
  //test = proc.getEnergy()==proc2.getEnergy();
  //TEST_EQUAL(test, true) operator = not yet implemented
RESULT


CHECK(ReissCavFreeEnergyProcessor::start())
  ReissCavFreeEnergyProcessor proc; 
  TEST_EQUAL(proc.start(), true)
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
