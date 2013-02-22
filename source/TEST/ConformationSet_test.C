// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/DOCKING/COMMON/conformationSet.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/DCDFile.h>

///////////////////////////

using namespace BALL;

START_TEST(ConformationSet)

ConformationSet* cs;
CHECK(ConformationSet())
  cs = new ConformationSet();
	TEST_NOT_EQUAL(cs,0)
RESULT


CHECK(~ConformationSet())
	delete cs;
RESULT

CHECK(readDCDFile())
		ConformationSet cs;
		cs.readDCDFile(BALL_TEST_DATA_PATH(ConformationSet_test.dcd));
		cs.resetScoring();
		TEST_EQUAL(cs.size(), 10)
RESULT

CHECK(writeDCDFile(const String& filename, const Size num = 0))
		ConformationSet cs;

		PDBFile pdb(BALL_TEST_DATA_PATH(ConformationSet_test.pdb));
		System sys;
		pdb.read(sys);
		cs.setup(sys);

		cs.readDCDFile(BALL_TEST_DATA_PATH(ConformationSet_test.dcd));
		cs.resetScoring();
		String tmp_filename;
		NEW_TMP_FILE(tmp_filename)

		cs.writeDCDFile(tmp_filename);
		TEST_FILE(tmp_filename.c_str(), BALL_TEST_DATA_PATH(ConformationSet_test.dcd))
RESULT

CHECK(setup())
	PDBFile pdb(BALL_TEST_DATA_PATH(ConformationSet_test.pdb));
	System sys;
	pdb.read(sys);

	ConformationSet cs;
	cs.setup(sys);
	TEST_EQUAL(sys.getProtein(0)->countAtoms(), 2145)

RESULT

/////////////////////////////////////////////////////////////
END_TEST
