// $Id: RandomCoilShiftProcessor_test.C,v 1.1 2000/09/21 13:38:55 oliver Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/NMR/randomCoilShiftProcessor.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(RandomCoilShiftProcessor, "$Id: RandomCoilShiftProcessor_test.C,v 1.1 2000/09/21 13:38:55 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

RandomCoilShiftProcessor* sp = 0;
CHECK(RandomCoilShiftProcessor::RandomCoilShiftProcessor() throw())
	sp = new RandomCoilShiftProcessor;
	TEST_NOT_EQUAL(sp, 0)
RESULT


CHECK(RandomCoilShiftProcessor::~RandomCoilShiftProcessor() throw())
  delete sp;
RESULT


CHECK(RandomCoilShiftProcessor::RandomCoilShiftProcessor(const RandomCoilShiftProcessor& processor) throw())
  //BAUSTELLE
RESULT


CHECK(RandomCoilShiftProcessor::init() throw())
  //BAUSTELLE
RESULT


CHECK(RandomCoilShiftProcessor::start() throw())
  //BAUSTELLE
RESULT


CHECK(RandomCoilShiftProcessor::Processor::Result operator () (Composite& composite) throw())
  //BAUSTELLE
RESULT


CHECK(RandomCoilShiftProcessor::finish() throw())
  //BAUSTELLE
RESULT

HINFile f("data/RandomCoilShiftProcessor_test.hin");
System S;
f >> S;

Parameters parameters("data/RandomCoilShiftProcessor_test.ini");

CHECK(chemical shifts)
	PRECISION(0.0001)
	RandomCoilShiftProcessor sp;
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
			if (atom_it->hasProperty(RandomCoilShiftProcessor::PROPERTY__RANDOMCOIL_SHIFT))
			{
				float shift = atom_it->getProperty(RandomCoilShiftProcessor::PROPERTY__RANDOMCOIL_SHIFT).getFloat();
				STATUS("shift #" << i << " of " << atom_it->getFullName() << ": " << shift)
				switch (i)
				{
					case  0: TEST_REAL_EQUAL(shift, 8.24) break; // ALA:1H
					case  1: TEST_REAL_EQUAL(shift, 8.24) break; // ALA:2H
					case  2: TEST_REAL_EQUAL(shift, 8.24) break; // ALA:3H
					case  3: TEST_REAL_EQUAL(shift, 4.32) break; // ALA:HA
					case  4: TEST_REAL_EQUAL(shift, 1.40) break; // ALA:1HB
					case  5: TEST_REAL_EQUAL(shift, 1.40) break; // ALA:2HB
					case  6: TEST_REAL_EQUAL(shift, 1.40) break; // ALA:3HB
					case  7: TEST_REAL_EQUAL(shift, 8.33) break; // GLY:H
					case  8: TEST_REAL_EQUAL(shift, 3.96) break; // GLY:2HA
					case  9: TEST_REAL_EQUAL(shift, 3.96) break; // GLY:1HA
					case 10: TEST_REAL_EQUAL(shift, 8.31) break; // SER:H
					case 11: TEST_REAL_EQUAL(shift, 4.47) break; // SER:HA
					case 12: TEST_REAL_EQUAL(shift, 3.89) break; // SER:2HB
					case 13: TEST_REAL_EQUAL(shift, 3.89) break; // SER:1HB
				}
				i++;
			}
		}
		TEST_EQUAL(i, 14)
	}	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
