// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

#include <BALL/NMR/assignShiftProcessor.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/SYSTEM/path.h>

START_TEST(AssignShiftProcessor, "$Id: AssignShiftProcessor_test.C,v 1.16.28.1 2007/03/25 21:46:45 oliver Exp $")

using namespace BALL;
using namespace std;

System s;
CHECK(preparations)
	STATUS("opening HIN file")
	HINFile f;
	f.open(BALL_TEST_DATA_PATH(AssignShiftProcessor_test.hin));
	STATUS("reading structure")
	f >> s;
RESULT

CHECK(AssignShiftProcessor::AssignShiftProcessor(const vector<NMRAtomData*>& atom_data)/apply())
	NMRStarFile rs(BALL_TEST_DATA_PATH(AssignShiftProcessor_test.str));
	TEST_EQUAL(rs.getNMRData()[0].atom_data.size(), 797)
	TEST_EQUAL(rs.getNumberOfAtoms(), 797)
	TEST_EQUAL(s.countAtoms(), 1944)

	AssignShiftProcessor asp(rs.getNMRData()[0].atom_data);
RESULT

CHECK(results)
	Size number_of_shiftatoms = 0;
	AtomIterator atom_it = s.beginAtom();
	for (; +atom_it; ++atom_it)
	{
		//Log.error() << atom_it->getProperty(ShiftModule::PROPERTY__SHIFT).getFloat() << std::endl;
		if (atom_it->hasProperty(ShiftModule::PROPERTY__SHIFT))
		{
			number_of_shiftatoms++;
		}
	}
	TEST_EQUAL(number_of_shiftatoms, 940)
RESULT

END_TEST
