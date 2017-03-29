// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/peptideCapProcessor.h>
#include <BALL/STRUCTURE/peptideBuilder.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <vector>

///////////////////////////

START_TEST(PeptideCapProcessor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::Peptides;

PeptideCapProcessor* pcp = 0;
CHECK(PeptideCapProcessor())
	pcp = new PeptideCapProcessor;
	TEST_NOT_EQUAL(pcp, 0)
RESULT								

CHECK(~PeptideCapProcessor())
	delete pcp;
	pcp = 0;
RESULT

CHECK(operator() (Composite& composite))
	PeptideBuilder pb("ARNDHPKLR");
	FragmentDB db("");
	pb.setFragmentDB(&db);
	Protein* prot = pb.construct();	
	TEST_EQUAL(prot->countResidues(), 9)

	PeptideCapProcessor pepcpro;
	prot->apply(pepcpro);
	TEST_EQUAL(prot->countResidues(), 11)
	TEST_EQUAL(prot->getResidue(0)->getName() , "ACE")
	TEST_EQUAL(prot->getResidue(10)->getName() , "NME")
	TEST_EQUAL(prot->countAtoms(), 169)
	
	// apply twice
	prot->apply(pepcpro);
	TEST_EQUAL(prot->countResidues(), 11)
	TEST_EQUAL(prot->countAtoms(), 169)

	delete prot;
RESULT

CHECK(operator() - single Residue)
	PeptideBuilder pb("A");
	FragmentDB db("");
	pb.setFragmentDB(&db);
	Protein* prot = pb.construct();	
	
	PeptideCapProcessor pepcpro;
	prot->apply(pepcpro);
	TEST_EQUAL(prot->countResidues(), 3)
	TEST_EQUAL(prot->getResidue(0)->getName() , "ACE")
	TEST_EQUAL(prot->getResidue(2)->getName() , "NME")
	TEST_EQUAL(prot->countAtoms(), 22)

	delete prot;
RESULT
////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST

