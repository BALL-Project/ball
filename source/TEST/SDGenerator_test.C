// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: SDGenerator_test.C,v 1.2 2006/06/08 07:30:28 oliver Exp $
//
// Author:
//   Holger Franken
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/FORMAT/MOLFile.h>

///////////////////////////

START_TEST(SDGenerator, "$Id: SDGenerator_test.C,v 1.2 2006/06/08 07:30:28 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.0001)

SDGenerator* sdg;

CHECK(SDGenerator())
  	sdg = new SDGenerator;
	TEST_NOT_EQUAL(sdg, 0)
RESULT

SDGenerator sdg_2;
System molecule_sys;
MOLFile infile("data/input_SDGenerator_test.mol");
infile >> molecule_sys;
infile.close();

CHECK((pair<Atom*, Atom*> getNeighbours(vector<Atom*>& ring, Atom*& atom)))
	
	vector<Atom*> ring;
	for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
	{
		ring.push_back(&*atom_it);
	}
	
	pair<Atom*, Atom*> neighbours = sdg_2.getNeighbours(ring, ring[1]);
	
	TEST_EQUAL(neighbours.first, ring[0])
	TEST_EQUAL(neighbours.second, ring[2])
	
RESULT

for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
{	
	atom_it -> setProperty(SDGenerator::in_ring);
}
//	get the "smallest set of smallest rings" (SSSR)
vector<vector<Atom*> > sssr;

//	call the implementation of  Figueras algorithm 
RingPerceptionProcessor getRings;

getRings.RingPerceptionProcessor::calculateSSSR(sssr, molecule_sys);

vector<vector<Atom*> > unseq_ringsys = sssr;

CHECK(vector<vector<Atom*> sequenceRings(vector<vector<Atom*> >& ringsystem))

	vector<vector<Atom*> > seq_ringsys = sdg_2.sequenceRings(sssr);
	
	TEST_EQUAL(seq_ringsys[0][0],unseq_ringsys[0][0])
	TEST_EQUAL(seq_ringsys[0][1],unseq_ringsys[0][1])
	TEST_EQUAL(seq_ringsys[0][2],unseq_ringsys[0][2])
	TEST_EQUAL(seq_ringsys[0][3],unseq_ringsys[0][3])
	TEST_EQUAL(seq_ringsys[0][4],unseq_ringsys[0][4])
	TEST_EQUAL(seq_ringsys[0][5],unseq_ringsys[0][5])


RESULT

System molecule_sys_2;
MOLFile infile_2("data/input_SDGenerator_test_2a.mol");
infile_2 >> molecule_sys_2;
infile.close(); 

System molecule_sys_3;
MOLFile infile_3("data/input_SDGenerator_test_2b.mol");
infile_3 >> molecule_sys_3;
infile_3.close(); 

CHECK(void generateSD(System& molecule_sys))

	sdg_2.generateSD(molecule_sys_2);
	
	vector<Atom*> mol_1;
	vector<Atom*> mol_2;
	AtomIterator atom_it_1;
	AtomIterator atom_it_2;

	for(atom_it_1 = molecule_sys_3.beginAtom(); atom_it_1 != molecule_sys_3.endAtom(); atom_it_1++)
	{
		mol_1.push_back(&*atom_it_1);
	}

	for(atom_it_2 = molecule_sys_2.beginAtom(); atom_it_2 != molecule_sys_2.endAtom(); atom_it_2++)
	{
		mol_2.push_back(&*atom_it_2);
	}

	for(Size i = 0; i != mol_1.size(); i++)
	{
		//TEST_REAL_EQUAL(mol_1[i] -> getPosition()[0], mol_2[i] -> getPosition()[0]);
		TEST_REAL_EQUAL(mol_1[i] -> getPosition()[1], mol_2[i] -> getPosition()[1]);
	}
			
RESULT


CHECK(~SDGenerator())

  	delete sdg;

RESULT


END_TEST
