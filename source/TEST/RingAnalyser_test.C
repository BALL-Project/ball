// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: RingAnalyser_test.C,v 1.3.8.2 2007/04/03 13:29:29 bertsch Exp $
//
// Author:
//  Holger Franken
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/ringClusterer.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(RingAnalyser, "$Id: RingAnalyser_test.C,v 1.3.8.2 2007/04/03 13:29:29 bertsch Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;


RingAnalyser* ra;

CHECK(RingAnalyser())
	ra = new RingAnalyser;
	TEST_NOT_EQUAL(ra, 0)
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
vector<vector<vector<Atom*> > > ringsystems = rc_2.clusterRings(seq_rings);

RingAnalyser ra_2;

CHECK((bool isInRing(const Atom* atom, vector<Atom*>& ring)))
	
	Size i = 0;


	for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
	{	
		switch (i)
		{
			case 0:
			{
				TEST_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 1:
			{
				TEST_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 2:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 3:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 4:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 5:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 6:
			{
				TEST_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 7:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 8:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 9:
			{
				TEST_NOT_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 10:
			{
				TEST_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 11:
			{
				TEST_EQUAL(ra_2.isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
		}

		i++;
	}	

RESULT


CHECK(vector<RingInfo> analyseRings(vector<vector<Atom*> >& ringsystem))

	vector<RingAnalyser::RingInfo> analysed_rings;
    	analysed_rings = ra_2.analyseRings(ringsystems[0]);

	TEST_EQUAL(analysed_rings[0].assignment, 2)
	TEST_EQUAL(analysed_rings[1].assignment, 3)
	TEST_EQUAL(analysed_rings[2].assignment, 5)

RESULT

CHECK(~RingAnalyser())
  
	delete ra;

RESULT


END_TEST
