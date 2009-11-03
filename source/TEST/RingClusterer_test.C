// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RingClusterer_test.C,v 1.3.8.2 2007/04/03 13:29:28 bertsch Exp $
//
// Author:
//  Holger Franken
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/ringClusterer.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(RingClusterer, "$Id: RingClusterer_test.C,v 1.3.8.2 2007/04/03 13:29:28 bertsch Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

RingClusterer* rc;
CHECK(RingClusterer())
	rc = new RingClusterer;
	TEST_NOT_EQUAL(rc, 0)
RESULT


System molecule_sys;
PDBFile infile(BALL_TEST_DATA_PATH(input_RingClusterer_test.pdb));
infile >> molecule_sys;
infile.close();

for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
{	
	atom_it -> setProperty(SDGenerator::IN_RING);
}

//	call the implementation of  Figueras algorithm 
vector<vector<Atom*> > sssr;
RingPerceptionProcessor getRings;
getRings.RingPerceptionProcessor::calculateSSSR(sssr, molecule_sys);

SDGenerator sdg;
//	sequence the atoms in each ring
vector<vector<Atom*> > seq_rings = sdg.sequenceRings(sssr);

RingClusterer rc_2;

CHECK(vector<vector<vector<Atom*> clusterRings(vector<vector<Atom*> >& rings))

	vector<vector<vector<Atom*> > > ringsystems = rc_2.clusterRings(seq_rings);
  
	TEST_EQUAL(ringsystems[0].size(), 3)
RESULT

CHECK(~RingClusterer())
  
	delete rc;

RESULT



END_TEST
