// $Id: AnisotropyShiftProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/anisotropyShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/defaultProcessors.h>

///////////////////////////

START_TEST(AnisotropyShiftProcessor, "$Id: AnisotropyShiftProcessor_test.C,v 1.2 2000/09/22 11:15:11 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

AnisotropyShiftProcessor* ap = 0;
CHECK(AnisotropyShiftProcessor::AnisotropyShiftProcessor() throw())
	ap = new AnisotropyShiftProcessor;
	TEST_NOT_EQUAL(ap, 0)
RESULT


CHECK(AnisotropyShiftProcessor::~AnisotropyShiftProcessor() throw())
  delete ap;
RESULT


CHECK(AnisotropyShiftProcessor::AnisotropyShiftProcessor(const AnisotropyShiftProcessor& processor) throw())
  //BAUSTELLE
RESULT


CHECK(AnisotropyShiftProcessor::init() throw())
  //BAUSTELLE
RESULT


CHECK(AnisotropyShiftProcessor::start() throw())
  //BAUSTELLE
RESULT


CHECK(AnisotropyShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(AnisotropyShiftProcessor::finish() throw())
  //BAUSTELLE
RESULT

HINFile f("data/AnisotropyShiftProcessor_test.hin");
System S;
f >> S;
ClearChargeProcessor clear_charge;
S.apply(clear_charge);

Parameters parameters("data/AnisotropyShiftProcessor_test.ini");

CHECK(charge assignment)
	AnisotropyShiftProcessor ap;
	ap.setParameters(parameters);
	ap.init();
	TEST_EQUAL(S.countAtoms(), 31)
	
	if (S.countAtoms() == 31)
	{
		S.apply(ap);
		AtomIterator atom_it = S.beginAtom();
		TEST_REAL_EQUAL(atom_it->getCharge(), 0)
		atom_it++;
		//BAUSTELLE
	}	
RESULT

CHECK(chemical shifts)
	PRECISION(0.0001)
	AnisotropyShiftProcessor ap;
	ap.setParameters(parameters);
	ap.init();
	TEST_EQUAL(S.countAtoms(), 31)

	
	if (S.countAtoms() == 31)
	{
		S.apply(ap);

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
