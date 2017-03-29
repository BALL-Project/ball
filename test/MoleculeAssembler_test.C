// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////

#include <BALL/STRUCTURE/moleculeAssembler.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/rsConstructor.h>
#include <BALL/STRUCTURE/chainBuilder.h>

#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/MOLFile.h>

///////////////////////////

START_TEST(MoleculeAssembler)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.001)

MoleculeAssembler* ma;
CHECK(MoleculeAssembler())
	ma = new MoleculeAssembler;
	TEST_NOT_EQUAL(ma, 0)
RESULT

SDGenerator sdg;
System molecule_sys;
PDBFile infile(BALL_TEST_DATA_PATH(input_MoleculeAssembler_test.pdb));
infile >> molecule_sys;
infile.close();


Selector s("element(H)");
molecule_sys.apply(s);
molecule_sys.removeSelected();


for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it ++)
{	
// 2 -  core chain
		if(atom_it -> getName() == "1")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}
// 3 -  core chain
		if(atom_it -> getName() ==  "2")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}
// 10 -  core chain
		if(atom_it -> getName() ==  "3")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}
// 11 -  in_ring
		else if(atom_it -> getName() == "4")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}

// 12 -  in_ring
		if(atom_it -> getName() ==  "7")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}

// 13 -  in_ring
		if(atom_it -> getName() == "8")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}

// 14 -  in_ring
		if(atom_it -> getName() == "9")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}

// 15 -  in_ring
		if(atom_it -> getName() == "10")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}

// 16 -  in_ring
		if(atom_it -> getName() == "11")
		{
			atom_it -> setProperty(SDGenerator::IN_RING);
		}

// 17 -  in_ring
		if(atom_it -> getName() == "12")
		{
			atom_it -> setProperty(SDGenerator::CORE_CHAIN);
		}

// 18 -  in_ring
		if(atom_it -> getName() == "13")
		{
			atom_it -> setProperty(SDGenerator::CORE_CHAIN);
		}

// 26 -  in_ring
		if(atom_it -> getName() == "14")
		{
			atom_it -> setProperty(SDGenerator::CORE_CHAIN);
		}

// 27 -  core chain
		if(atom_it -> getName() == "15")
		{
			atom_it -> setProperty(SDGenerator::CORE_CHAIN);
		}
}
		
		//      get the "smallest set of smallest rings" (SSSR)
		vector<vector<Atom*> > rings;
		RingPerceptionProcessor getRings;
		getRings.RingPerceptionProcessor::calculateSSSR(rings, molecule_sys);

		//      sequence the atoms in each ring
		RingAnalyser ra;
		vector<vector<Atom*> > seq_rings;
		ra.sequenceRings(rings, seq_rings);

		//      cluster the rings Sizeo connected ringsystems
		vector<vector<vector<Atom*> > > ringsystems = ra.clusterRings(seq_rings);

		//      analyse the way the rings are connected and construct each ringsystem in the suitable way
		vector<RingAnalyser::RingInfo> analysed_rings;

		for (Size i = 0; i != ringsystems.size(); i++)
		{
			//      analyse the way the rings are connected
			analysed_rings = ra.analyseRings(ringsystems[i]);

			//      construct each ringsystem in the suitable way
			RSConstructor rsc;
			rsc.constructRS(analysed_rings, i);

			for (vector<vector<Atom*> >::size_type j = 0; j != ringsystems[i].size(); j++)
			{
				for (vector<Atom*>::size_type k = 0; k != ringsystems[i][j].size(); k++)
				{
					ringsystems[i][j][k]->setProperty(SDGenerator::DEPOSITED);
					ringsystems[i][j][k]->setProperty(SDGenerator::PRE_ASSEMBLED);
				}
			}
		}


		//      find all chains and sort them by their length
		ChainBuilder cb;
		vector<vector<Atom*> > chains = cb.buildChains(molecule_sys);


System molecule_sys_2;
PDBFile infile_2(BALL_TEST_DATA_PATH(input_MoleculeAssembler_test.pdb));
infile_2 >> molecule_sys_2;
infile_2.close();

sdg.generateSD(molecule_sys_2);


System molecule_sys_3;
PDBFile infile_3(BALL_TEST_DATA_PATH(input_MoleculeAssembler_test.pdb));
infile_3 >> molecule_sys_3;
infile_3.close();

sdg.generateSD(molecule_sys_3);


System molecule_sys_4;
PDBFile infile_4(BALL_TEST_DATA_PATH(input_MoleculeAssembler_test.pdb));
infile_4 >> molecule_sys_4;
infile_4.close();

sdg.generateSD(molecule_sys_4);


CHECK((void assembleMolecule(System& molecule_sys, vector<vector<vector<Atom*> > >& ringsystems, vector<vector<Atom*> >& chains)))
	
	MoleculeAssembler ma_2;
	ma_2.assembleMolecule(molecule_sys, ringsystems, chains);

	AtomIterator atom_it_1;
	AtomIterator atom_it_2;

	if(molecule_sys.beginAtom() == molecule_sys_2.beginAtom())
	{
		for(atom_it_1 = molecule_sys.beginAtom(); atom_it_1 != molecule_sys.endAtom(); atom_it_1++)
		{
			for(atom_it_2 = molecule_sys_2.beginAtom(); atom_it_2 != molecule_sys_2.endAtom(); atom_it_2++)
			{
				if(atom_it_1 -> getName() == atom_it_2 -> getName())
				{	
					TEST_EQUAL(atom_it_1 -> getName() , atom_it_2 -> getName())
					TEST_REAL_EQUAL(atom_it_1 -> getPosition()[0], atom_it_2 -> getPosition()[0]);
				}
			}
			
		}
	}
	
	else if(molecule_sys.beginAtom() == molecule_sys_3.beginAtom())
	{
		for(atom_it_1 = molecule_sys.beginAtom(); atom_it_1 != molecule_sys.endAtom(); atom_it_1++)
		{
			for(atom_it_2 = molecule_sys_3.beginAtom(); atom_it_2 != molecule_sys_3.endAtom(); atom_it_2++)
			{
				if(atom_it_1 -> getName() == atom_it_2 -> getName())
				{	
					TEST_EQUAL(atom_it_1 -> getName() , atom_it_2 -> getName())
					TEST_REAL_EQUAL(atom_it_1 -> getPosition()[0], atom_it_2 -> getPosition()[0]);
				}
			}
			
		}
	}

	else if(molecule_sys.beginAtom() == molecule_sys_4.beginAtom())
	{
		for(atom_it_1 = molecule_sys.beginAtom(); atom_it_1 != molecule_sys.endAtom(); atom_it_1++)
		{
			for(atom_it_2 = molecule_sys_4.beginAtom(); atom_it_2 != molecule_sys_4.endAtom(); atom_it_2++)
			{
				if(atom_it_1 -> getName() == atom_it_2 -> getName())
				{	
					TEST_EQUAL(atom_it_1 -> getName() , atom_it_2 -> getName())
					TEST_REAL_EQUAL(atom_it_1 -> getPosition()[0], atom_it_2 -> getPosition()[0]);
				}
			}
			
		}

	}

RESULT


CHECK(~MoleculeAssembler())
	delete ma;
RESULT



END_TEST
