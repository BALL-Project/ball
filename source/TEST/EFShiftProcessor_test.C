// $Id: EFShiftProcessor_test.C,v 1.4 2001/12/30 13:28:57 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/EFShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(EFShiftProcessor, "$Id: EFShiftProcessor_test.C,v 1.4 2001/12/30 13:28:57 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

EFShiftProcessor* sp = 0;
CHECK(EFShiftProcessor::EFShiftProcessor() throw())
	sp = new EFShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(EFShiftProcessor::~EFShiftProcessor() throw())
  delete sp;
RESULT


CHECK(EFShiftProcessor::EFShiftProcessor(const EFShiftProcessor& processor) throw())
  //?????
RESULT


CHECK(EFShiftProcessor::init() throw())
  //?????
RESULT


CHECK(EFShiftProcessor::start() throw())
  //?????
RESULT


CHECK(EFShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //?????
RESULT


CHECK(EFShiftProcessor::finish() throw())
  //?????
RESULT

HINFile f("data/EFShiftProcessor_test.hin");
System S;
f >> S;
ClearChargeProcessor clear_charge;
S.apply(clear_charge);

Parameters parameters("data/EFShiftProcessor_test.ini");

CHECK(charge assignment)
	EFShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(S.countAtoms(), 31)
	
	if (S.countAtoms() == 31)
	{
		S.apply(sp);
		AtomIterator atom_it = S.beginAtom();
		TEST_REAL_EQUAL(atom_it->getCharge(), -0.354167)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.333333)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), -0.479167)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), -0.354167)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.145833)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.333333)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), -0.479167)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), -0.354167)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.145833)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.333333)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), -0.479167)
		atom_it++;
		TEST_REAL_EQUAL(atom_it->getCharge(), 0.0)
	}	
RESULT

CHECK(chemical shifts)
	PRECISION(0.0001)
	EFShiftProcessor sp;
	sp.setParameters(parameters);
	sp.init();
	TEST_EQUAL(S.countAtoms(), 31)
	
	if (S.countAtoms() == 31)
	{
		S.apply(sp);

		AtomIterator atom_it = S.beginAtom();
		Position i = 0;
		for (; +atom_it; ++atom_it)
		{
			if (atom_it->hasProperty(EFShiftProcessor::PROPERTY__EF_SHIFT))
			{
				float shift = atom_it->getProperty(EFShiftProcessor::PROPERTY__EF_SHIFT).getFloat();
				STATUS("shift of " << atom_it->getFullName() << ": " << shift)
				switch (i++)
				{
					case  0: TEST_REAL_EQUAL(shift, -0.02835) break;
					case  1: TEST_REAL_EQUAL(shift, -0.01370) break;
					case  2: TEST_REAL_EQUAL(shift, -0.01314) break;
					case  3: TEST_REAL_EQUAL(shift, -0.04398) break;
					case  4: TEST_REAL_EQUAL(shift,  0.03422) break;
					case  5: TEST_REAL_EQUAL(shift, -0.04594) break;
					case  6: TEST_REAL_EQUAL(shift, -0.03188) break;
					case  7: TEST_REAL_EQUAL(shift, -0.03139) break;
					case  8: TEST_REAL_EQUAL(shift, -0.03043) break;
					case  9: TEST_REAL_EQUAL(shift,  0.01749) break;
					case 10: TEST_REAL_EQUAL(shift, -0.03506) break;
					case 11: TEST_REAL_EQUAL(shift,  0.11131) break;
				}
			}
		}
	}	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
