// $Id: HaighMallionShiftProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(HaighMallionShiftProcessor, "$Id: HaighMallionShiftProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

HaighMallionShiftProcessor* sp = 0;
CHECK(HaighMallionShiftProcessor::HaighMallionShiftProcessor() throw())
	sp = new HaighMallionShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(HaighMallionShiftProcessor::~HaighMallionShiftProcessor() throw())
  delete sp;
RESULT


CHECK(HaighMallionShiftProcessor::HaighMallionShiftProcessor(const HaighMallionShiftProcessor& processor) throw())
  //BAUSTELLE
RESULT


CHECK(HaighMallionShiftProcessor::init() throw())
  //BAUSTELLE
RESULT


CHECK(HaighMallionShiftProcessor::start() throw())
  //BAUSTELLE
RESULT


CHECK(HaighMallionShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(HaighMallionShiftProcessor::finish() throw())
  //BAUSTELLE
RESULT

HINFile f("data/HaighMallionShiftProcessor_test.hin");
System S;
f >> S;
ClearChargeProcessor clear_charge;
S.apply(clear_charge);

Parameters parameters("data/HaighMallionShiftProcessor_test.ini");

CHECK(charge assignment)
	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(S.countAtoms(), 31)
	
	if (S.countAtoms() == 31)
	{
		S.apply(sp);
		AtomIterator atom_it = S.beginAtom();
		TEST_REAL_EQUAL(atom_it->getCharge(), 0)
		atom_it++;
		//BAUSTELLE
	}	
RESULT

CHECK(chemical shifts)
	PRECISION(0.0001)
	HaighMallionShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();

	/*
	TEST_EQUAL(S.countAtoms(), 31)
	if (S.countAtoms() == 31)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty("chemical_shift"))
			{
				float shift = atom_it->getProperty("chemical_shift").getFloat();
				STATUS("shift of " << atom_it->getFullName() << ": " << shift)
				switch (i++)
				{//BAUSTELLE
					case  0: TEST_REAL_EQUAL(shift, -0.02835) break;
				}
			}
		}
	}	
	*/
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
