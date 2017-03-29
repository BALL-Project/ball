// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(SecondaryStructureProcessor)

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
	PDBFile f(BALL_TEST_DATA_PATH(PDBFile_test2.pdb));
	f.read(S);
	f.close();

	TEST_EQUAL(S.countSecondaryStructures(), 7)
	ABORT_IF(S.countAtoms() != 892)

	String summary;
	for (ResidueIterator ri(S.beginResidue()); +ri; ++ri)	
	{
		SecondaryStructure* ss = ri->getSecondaryStructure();
		if (ss == 0)
		{
			summary += '-';
		}
		else
		{
			switch (ss->getType())
			{
				case SecondaryStructure::HELIX:
					summary += 'H';
					break;
				case SecondaryStructure::STRAND:
					summary += 'E';
					break;
				case SecondaryStructure::TURN:
					summary += 'T';
					break;
				case SecondaryStructure::COIL:
				default:
					summary += 'C';
			}
		}
	}
	String PDB_summary = "CCGGGGSCCCCCSCCCCEEEEEEETTTTEEEEEEECSSSCCSSCBSSHHHHHHHHSCC";
	STATUS("Summary: " << summary)
	STATUS(" len = " << summary.size())
	STATUS("PDBSum:  " << PDB_summary)
	STATUS(" len = " << PDB_summary.size())
		
	SecondaryStructureProcessor ssp;
	S.apply(ssp);
	for (SecondaryStructureIterator it = S.beginSecondaryStructure(); +it; ++it)
	{
		STATUS(it->getType())
	}
	
RESULT

END_TEST
