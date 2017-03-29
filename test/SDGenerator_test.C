// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/FORMAT/PDBFile.h>

///////////////////////////

START_TEST(SDGenerator)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace std;

PRECISION(0.001)

SDGenerator* sdg;

CHECK(SDGenerator())
  sdg = new SDGenerator;
	TEST_NOT_EQUAL(sdg, 0)
RESULT

CHECK(~SDGenerator())
  delete sdg;
RESULT


SDGenerator sdg_2;
System molecule_sys;
PDBFile infile(BALL_TEST_DATA_PATH(input_SDGenerator_test.pdb));
infile >> molecule_sys;
infile.close();

CHECK((pair<Atom*, Atom*> getNeighbours(vector<Atom*>& ring, Atom*& atom)))
	
	vector<Atom*> ring;
	for (AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
	{
		ring.push_back(&*atom_it);
	}
	
	pair<Atom*, Atom*> neighbours = sdg_2.getNeighbours(ring, ring[1]);
	
	TEST_EQUAL(neighbours.first, ring[0])
	TEST_EQUAL(neighbours.second, ring[2])
	
RESULT

for (AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
{	
	atom_it -> setProperty(SDGenerator::IN_RING);
}
//	get the "smallest set of smallest rings" (SSSR)
vector<vector<Atom*> > sssr;

//	call the implementation of Figueras algorithm 
RingPerceptionProcessor getRings;

getRings.RingPerceptionProcessor::calculateSSSR(sssr, molecule_sys);

vector<vector<Atom*> > unseq_ringsys = sssr;
RingAnalyser ra;
CHECK(vector<vector<Atom*> sequenceRings(vector<vector<Atom*> >& ringsystem))

	vector<vector<Atom*> > seq_ringsys;
	ra.sequenceRings(sssr, seq_ringsys);

	for (Size i = 0; i != seq_ringsys[0].size(); i++)
	{
		if(i > 0 && i < seq_ringsys[0].size())
		{
			if(seq_ringsys[0][i-1] == sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).first)
			{
				TEST_EQUAL(seq_ringsys[0][i-1], sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).first)
			}
			else if(seq_ringsys[0][i-1] == sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).second)
			{
				TEST_EQUAL(seq_ringsys[0][i-1], sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).second)
			}
			else if(seq_ringsys[0][i+1] = sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).first)
			{
				TEST_EQUAL(seq_ringsys[0][i+1], sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).first)
			}
			else
			{	//if none of the previous cases was true, this one must be, otherwise sequenceRings() has failed
				TEST_EQUAL(seq_ringsys[0][i+1], sdg_2.getNeighbours(seq_ringsys[0], seq_ringsys[0][i]).second)
			}
		}
	}

RESULT

System molecule_sys_2;
PDBFile infile_2(BALL_TEST_DATA_PATH(input_SDGenerator_test_2a.pdb));
infile_2 >> molecule_sys_2;
infile.close(); 

System molecule_sys_3;
PDBFile infile_3(BALL_TEST_DATA_PATH(input_SDGenerator_test_2b.pdb));
infile_3 >> molecule_sys_3;
infile_3.close(); 

CHECK(void generateSD(System& molecule_sys))
	sdg_2.generateSD(molecule_sys_2);
	
	vector<Atom*> mol_1;
	vector<Atom*> mol_2;
	AtomIterator atom_it_1;
	AtomIterator atom_it_2;

	for (atom_it_1 = molecule_sys_3.beginAtom(); atom_it_1 != molecule_sys_3.endAtom(); atom_it_1++)
	{
		for (atom_it_2 = molecule_sys_2.beginAtom(); atom_it_2 != molecule_sys_2.endAtom(); atom_it_2++)
		{
			if(atom_it_1 -> getName() == atom_it_2 -> getName())
			{
				TEST_REAL_EQUAL(atom_it_1 -> getPosition()[1], atom_it_2 -> getPosition()[1]);
			}
		}
	}

RESULT


END_TEST
