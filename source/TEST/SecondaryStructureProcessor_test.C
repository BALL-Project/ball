// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SecondaryStructureProcessor_test.C,v 1.1 2004/02/23 19:51:32 oliver Exp $
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(SecondaryStructureProcessor, "$Id: SecondaryStructureProcessor_test.C,v 1.1 2004/02/23 19:51:32 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

											
CHECK(Processor::Result operator() (Composite& composite))
  // ???
RESULT

CHECK(SecondaryStructureProcessor())
  // ???
RESULT

CHECK(void compute())
  // ???
RESULT

CHECK([EXTRA]assignment for BPTI)
	System S;
	PDBFile f("data/PDBFile_test2.pdb");
	f.read(S);
	f.close();

	TEST_EQUAL(S.countSecondaryStructures(), 7)
	ABORT_IF(S.countAtoms() != 892)

	SecondaryStructureProcessor ssp;
	S.apply(ssp);
	for (SecondaryStructureIterator it = S.beginSecondaryStructure();
			 +it; ++it)
	{
		STATUS(it->getType())
	}
	
RESULT

END_TEST
