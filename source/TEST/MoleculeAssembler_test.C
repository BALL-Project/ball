// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MoleculeAssembler_test.C,v 1.2 2006/06/08 07:30:27 oliver Exp $
//
// Author:
//   Holger Franken
//

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/STRUCTURE/moleculeAssembler.h>
#include <BALL/KERNEL/selector.h>

//#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/MOLFile.h>

///////////////////////////

START_TEST(MoleculeAssembler, "$Id: MoleculeAssembler_test.C,v 1.2 2006/06/08 07:30:27 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

PRECISION(0.0001)

MoleculeAssembler* ma;
CHECK(MoleculeAssembler())
	ma = new MoleculeAssembler;
	TEST_NOT_EQUAL(ma, 0)
RESULT

SDGenerator sdg;
System molecule_sys;
MOLFile infile("data/input_MoleculeAssembler_test.mol");
infile >> molecule_sys;
infile.close();

Selector s("element(H)");
molecule_sys.apply(s);
molecule_sys.removeSelected();

Size i = 0;

for(AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
{	
	switch (i)
	{
		case 0:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 1:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 4:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 6:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 7:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 8:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 9:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 14:
		{
			atom_it -> setProperty(SDGenerator::in_ring);
				break;
		}
		case 15:
		{
			atom_it -> setProperty(SDGenerator::in_ring);
				break;
		}
		case 16:
		{
			atom_it -> setProperty(SDGenerator::in_ring);
				break;
		}
		case 17:
		{
			atom_it -> setProperty(SDGenerator::in_ring);
				break;
		}
		case 18:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 19:
		{
			atom_it -> setProperty(SDGenerator::core_chain);
				break;
		}
		case 20:
		{
			atom_it -> setProperty(SDGenerator::in_ring);
				break;
		}
		case 21:
		{
			atom_it -> setProperty(SDGenerator::in_ring);
				break;
		}
	}

	i++;
}


//	get the "smallest set of smallest rings" (SSSR)
vector<vector<Atom*> > sssr;
//	call the implementation of  Figueras algorithm 
RingPerceptionProcessor getRings;
getRings.RingPerceptionProcessor::calculateSSSR(sssr, molecule_sys);

//	sequence the atoms in each ring
vector<vector<Atom*> > seq_rings = sdg.sequenceRings(sssr);

//	cluster the rings into connected ringsystems
RingClusterer riclu;
vector<vector<vector<Atom*> > > ringsystems = riclu.clusterRings(seq_rings);

//	analyse the way the rings are connected and construct each ringsystem in the suitable way
vector<RingAnalyser::RingInfo> analysed_rings;

for(vector<vector<vector<Atom*> > >::size_type i = 0; i != ringsystems.size(); i++)
{	
		//	analyse the way the rings are connected
	RingAnalyser ra;
	analysed_rings = ra.analyseRings(ringsystems[i]);
	
		//	construct each ringsystem in the suitable way
	RSConstructor rsc;
	rsc.constructRS(analysed_rings, i);
	
	for(vector<vector<Atom*> >::size_type j = 0; j != ringsystems[i].size(); j++)
	{
		for(vector<Atom*>::size_type k = 0; k != ringsystems[i][j].size(); k++)
		{
			ringsystems[i][j][k] -> setProperty(SDGenerator::deposited);
			ringsystems[i][j][k] -> setProperty(SDGenerator::pre_assembled);		
		}
	}

}
	//	find all chains and sort them by their length
ChainBuilder cb;
vector<vector<Atom*> > chains = cb.buildChains(molecule_sys);

System molecule_sys_2;
MOLFile infile_2("data/input_MoleculeAssembler_test_2.mol");
infile_2 >> molecule_sys_2;
infile.close();

CHECK((void assembleMolecule(System& molecule_sys, vector<vector<vector<Atom*> > >& ringsystems, vector<vector<Atom*> >& chains)))
	
	MoleculeAssembler ma_2;
	ma_2.assembleMolecule(molecule_sys, ringsystems, chains);

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
	
	TEST_EQUAL(mol_1.size(), mol_2.size());

	for(Size i = 0; i != mol_1.size(); i++)
	{
		TEST_EQUAL(mol_1[i] -> getName(), mol_2[i] -> getName());
		TEST_REAL_EQUAL(mol_1[i] -> getPosition()[0], mol_2[i] -> getPosition()[0]);
		TEST_REAL_EQUAL(mol_1[i] -> getPosition()[1], mol_2[i] -> getPosition()[1]);
	}

RESULT


CHECK(~MoleculeAssembler())

	delete ma;
RESULT



END_TEST
