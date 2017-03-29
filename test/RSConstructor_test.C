// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/rsConstructor.h>
#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/MOLFile.h>

///////////////////////////

START_TEST(RSConstructor)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.0001)

///  insert tests for each member function here         
///
	
RSConstructor* rsc;

CHECK(RSConstructor())
  rsc = new RSConstructor;
	TEST_NOT_EQUAL(rsc, 0)
RESULT

System molecule_sys;
PDBFile infile(BALL_TEST_DATA_PATH(input_RSConstructor_test.mol));
infile >> molecule_sys;
infile.close();

//checkAtoms(molecule_sys);

for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
{	
	atom_it -> setProperty(SDGenerator::IN_RING);
}

//	get the "smallest set of smallest rings" (SSSR)
vector<vector<Atom*> > sssr;

//	call the implementation of  Figueras algorithm 
RingPerceptionProcessor getRings;

getRings.RingPerceptionProcessor::calculateSSSR(sssr, molecule_sys);

SDGenerator sdg;
RingAnalyser ra;
//	sequence the atoms in each ring
	vector<vector<Atom*> > seq_rings; 
	ra.sequenceRings(sssr, seq_rings);

//	cluster the rings Sizeo connected ringsystems
vector<vector<vector<Atom*> > > ringsystems = ra.clusterRings(seq_rings);


//	analyse the way the rings are connected and construct each ringsystem in the suitable way
vector<RingAnalyser::RingInfo> analysed_rings;

for(vector<vector<vector<Atom*> > >::size_type i = 0; i != ringsystems.size(); i++)
{
		//	analyse the way the rings are connected
    RingAnalyser ra;
    analysed_rings = ra.analyseRings(ringsystems[i]);
}
	
System molecule_sys_2;
MOLFile infile_2(BALL_TEST_DATA_PATH(input_RSConstructor_test_2.mol));
infile_2 >> molecule_sys_2;
infile.close(); 

RSConstructor rsc_2;

CHECK((void constructRS(vector<RingInfo>& analysed_rings, Size& i)))

	Size i = 0;
  	rsc_2.constructRS(analysed_rings, i);

	vector<Atom*> mol_1;
	vector<Atom*> mol_2;
	AtomIterator atom_it_1;
	AtomIterator atom_it_2;

	for(atom_it_1 = molecule_sys.beginAtom(); atom_it_1 != molecule_sys.endAtom(); atom_it_1++)
	{
		mol_1.push_back(&*atom_it_1);
	}

	for(atom_it_2 = molecule_sys_2.beginAtom(); atom_it_2 != molecule_sys_2.endAtom(); atom_it_2++)
	{
		mol_2.push_back(&*atom_it_2);
	}

	for(Size i = 0; i != mol_1.size(); i++)
	{
		TEST_REAL_EQUAL(mol_1[i] -> getPosition()[0], mol_2[i] -> getPosition()[0]);
		TEST_REAL_EQUAL(mol_1[i] -> getPosition()[1], mol_2[i] -> getPosition()[1]);
	}

RESULT



CHECK(~RSConstructor())

    	delete rsc;

RESULT

END_TEST
