// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(RingAnalyser)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

RingAnalyser* ra;

CHECK(RingAnalyser())
	ra = new RingAnalyser;
	TEST_NOT_EQUAL(ra, 0)
RESULT

System molecule_sys;
PDBFile infile(BALL_TEST_DATA_PATH(input_RingAnalyser_test.pdb));
infile >> molecule_sys;
infile.close();

//	call the implementation of  Figueras algorithm 
vector<vector<Atom*> > sssr;
RingPerceptionProcessor getRings;
getRings.calculateSSSR(sssr, molecule_sys);

//	sequence the atoms in each ring
vector<vector<Atom*> > seq_rings;
ra->sequenceRings(sssr, seq_rings);

vector<vector<vector<Atom*> > > ringsystems = ra->clusterRings(seq_rings);

CHECK((bool isInRing(const Atom* atom, vector<Atom*>& ring)))
	
	Size i = 0;

	for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
	{	
		switch (i)
		{
			case 0:
			{
				TEST_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 1:
			{
				TEST_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 2:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 3:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 4:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 5:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 6:
			{
				TEST_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 7:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 8:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 9:
			{
				TEST_NOT_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 10:
			{
				TEST_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
			case 11:
			{
				TEST_EQUAL(ra->isInRing(&*atom_it, ringsystems[0][0]), 1);
				 break;
			}
		}

		i++;
	}	
RESULT

CHECK(vector<vector<vector<Atom*> clusterRings(vector<vector<Atom*> >& rings))
	ringsystems = ra->clusterRings(seq_rings);
  
	TEST_EQUAL(ringsystems[0].size(), 3)
RESULT


CHECK(vector<RingInfo> analyseRings(vector<vector<Atom*> >& ringsystem))

	vector<RingAnalyser::RingInfo> analysed_rings;
    	analysed_rings = ra->analyseRings(ringsystems[0]);

	TEST_EQUAL(analysed_rings[0].assignment, 2)
	TEST_EQUAL(analysed_rings[1].assignment, 3)
	TEST_EQUAL(analysed_rings[2].assignment, 5)

RESULT

CHECK(~RingAnalyser())
	delete ra;
RESULT


END_TEST
