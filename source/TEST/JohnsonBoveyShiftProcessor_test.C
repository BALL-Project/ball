// $Id: JohnsonBoveyShiftProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/johnsonBoveyShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(JohnsonBoveyShiftProcessor, "$Id: JohnsonBoveyShiftProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

JohnsonBoveyShiftProcessor* sp = 0;
CHECK(JohnsonBoveyShiftProcessor::JohnsonBoveyShiftProcessor() throw())
	sp = new JohnsonBoveyShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(JohnsonBoveyShiftProcessor::~JohnsonBoveyShiftProcessor() throw())
  delete sp;
RESULT


CHECK(JohnsonBoveyShiftProcessor::JohnsonBoveyShiftProcessor(const JohnsonBoveyShiftProcessor& processor) throw())
  //BAUSTELLE
RESULT


CHECK(JohnsonBoveyShiftProcessor::init() throw())
  //BAUSTELLE
RESULT


CHECK(JohnsonBoveyShiftProcessor::start() throw())
  //BAUSTELLE
RESULT


CHECK(JohnsonBoveyShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(JohnsonBoveyShiftProcessor::finish() throw())
  //BAUSTELLE
RESULT

HINFile f("data/JohnsonBoveyShiftProcessor_test.hin");
System S;
f >> S;
ClearChargeProcessor clear_charge;
S.apply(clear_charge);

Parameters parameters("data/JohnsonBoveyShiftProcessor_test.ini");

CHECK(charge assignment)
	JohnsonBoveyShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	/*
	TEST_EQUAL(S.countAtoms(), 31)
	
	if (S.countAtoms() == 31)
	{
		S.apply(sp);
		AtomIterator atom_it = S.beginAtom();
		TEST_REAL_EQUAL(atom_it->getCharge(), 0)
		atom_it++;
		//BAUSTELLE
	}	
	*/
RESULT

CHECK(chemical shifts)
	PRECISION(0.0001)
	JohnsonBoveyShiftProcessor sp;
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
