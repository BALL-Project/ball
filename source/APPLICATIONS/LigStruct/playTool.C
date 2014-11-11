// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>

#include <vector>
#include <map>

#include <boost/pending/disjoint_sets.hpp>

using namespace OpenBabel;
using namespace BALL;
using namespace std;

/// ################# H E L P E R    F U N C T I O N S #################
// check if the atom is a rigid one:
bool isAtomRigid(OBAtom* atm)
{
/// TODO: add OBRotorList object to use custom torlib!
	if (atm->IsInRing())
		return true;
	else
	{
		OBBondIterator b_it = atm->BeginBonds();
		for(; b_it!=atm->EndBonds(); b_it++)
		{
			if( ! (*b_it)->IsRotor() )
				return true;
		}
		return false;
	}
}

// cut bonds that are shared with atoms from other fragments:
void clearExternalBonds(Molecule* mol)
{
	Atom::BondIterator bit;
	AtomIterator ait;

	BALL_FOREACH_INTERBOND(*mol, ait, bit)
	{
		bit->destroy();
	}
}

// Copy properties from an original molecule to a copy molecule
void copyMoleculeProperies(Molecule &orig, Molecule &cop)
{
	NamedPropertyIterator  it;
	for(it = orig.beginNamedProperty (); it !=orig.endNamedProperty(); it++)
	{
		cop.setProperty(*it);
	}
	cop.setName(orig.getName());
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("queryFragments", " generate query fragments and connections", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "...just playing...";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	// ######## START ####################################################
	OBMol obMol; // working molecule
	
	// Read open-babel molecule as input:
	OBConversion conv;
	OBFormat *format = conv.FormatFromExt(parpars.get("i"));
	
	if (!format || !conv.SetInFormat(format))
		cout << "Could not find input format for file " << parpars.get("i") << endl;

	ifstream ifs(parpars.get("i")); // open file
	
	if (!ifs)
		cout << "Could not open " << parpars.get("i") << " for reading." << endl;
	
	conv.Read(&obMol, &ifs); // actual 'read' command
	ifs.close();
	
	obMol.FindRingAtomsAndBonds(); // find rings

	// --------------------did parameter parsing----------------------------------
	
	typedef boost::disjoint_sets < int*, int*, boost::find_with_full_path_compression > DisjointSet;
	int num_atoms = obMol.NumAtoms();
	
	// create 2 empty disjoint sets:
	std::vector <int> fixed_rank(num_atoms);
	std::vector <int> fixed_parent(num_atoms);
	DisjointSet dset_rigid(&fixed_rank[0], &fixed_parent[0]);
	
	std::vector <int> linker_rank(num_atoms);
	std::vector <int> linker_parent(num_atoms);
	DisjointSet dset_linker(&linker_rank[0], &linker_parent[0]);
	
	std::vector <bool> is_rigid(num_atoms, 0); // bitVec indicating rigid atoms
	std::vector <bool> is_linker(num_atoms, 0); // bitVec indicating linker atoms

	/// iterate over all bonds and split into 2 groups of fragments:
	OBBondIterator b_it = obMol.BeginBonds();
	std::vector< int > connections(num_atoms, -1);
	
	for (; b_it != obMol.EndBonds(); b_it++)
	{
		OBAtom* atm1 = (*b_it)->GetBeginAtom();
		OBAtom* atm2 =  (*b_it)->GetEndAtom();
		int id1 = atm1->GetId();
		int id2 = atm2->GetId();

		// for all rotable bonds:
		if ( (*b_it)->IsRotor() )
		{
			if( !(is_linker[id1] || isAtomRigid(atm1)) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id1); 
				is_linker[id1]=1;
			}
			if( !(is_linker[id2] || isAtomRigid(atm2)) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id2); 
				is_linker[id2]=1;
			}
			dset_linker.union_set(id1, id2);
			
			// rotables to a rigid fragment define connection points:
			if( isAtomRigid((*b_it)->GetBeginAtom()) || isAtomRigid((*b_it)->GetEndAtom()) )
			{
				connections[id1] = id2;
				connections[id2] = id1;
			}
		}
		// for all fixed bonds:
		else
		{
			if( !is_rigid[id1] ) // do not double assign (deletes disjointset-parent status)
			{
				dset_rigid.make_set(id1); 
				is_rigid[id1]=1;
			}
			if( !is_rigid[id2] ) // do not double assign (deletes disjointset-parent status)
			{
				dset_rigid.make_set(id2); 
				is_rigid[id2]=1;
			}
			dset_rigid.union_set(id1,id2);
		}
	}

	/// convert from Babel to BALL:
	Molecule* ball_mol = MolecularSimilarity::createMolecule(obMol, true);
	ball_mol->setName("original_mol");
	
	///	iterate over all atoms, sorting out the  fixed and linker fragments
	std::vector <int> link_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	std::vector <int> rigid_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	std::vector <Molecule*> fragments;
	
	std::map< int, pair<int,int> > map_connect; // key: original id, value: (fragment, new id)
	int parent_id = -1;
	// loop over the atoms, sort fragments out
	for(int i = 0 ; i < num_atoms; i++)
	{
		Atom* atm = ball_mol->getAtom(0);
		// atom is linker, extend a linker fragment
		if ( is_linker[i] )
		{
			parent_id = dset_linker.find_set(i);
			
			// if the atoms fragment does not exist, create it:
			if(link_groups[parent_id]<0)
			{
				link_groups[parent_id] = fragments.size();
				Molecule* dummy = new Molecule();
				
				if(connections[i] != -1) // remap connections
					map_connect[i]=make_pair( fragments.size(), 0 );
				
				dummy->insert(*atm);
				dummy->setName("Fragment_"+toString(fragments.size()));
				dummy->setProperty("isRigid", false);
				
				fragments.push_back(dummy);
			}
			// atom is part of existing fragment:
			else
			{
				if(connections[i] != -1) // remap connections
					map_connect[i]=make_pair( fragments.size()-1, fragments[ link_groups[parent_id] ]->countAtoms() );
				
				fragments[ link_groups[parent_id] ]->insert(*atm);
			}
		}
		// Atom is a rigid, extend a rigid fragment
		else
		{
			parent_id = dset_rigid.find_set(i);
			// if fragment does not exist, create it:
			if(rigid_groups[parent_id]<0)
			{
				rigid_groups[parent_id] = fragments.size();
				Molecule* dummy = new Molecule();

				if(connections[i] != -1)// remap connections
					map_connect[i]=make_pair( fragments.size(), 0);

				dummy->insert(*atm);
				dummy->setName("Fragment_"+toString(fragments.size()));
				dummy->setProperty("isRigid", true);
				fragments.push_back(dummy);
			}
			else
			{
				if(connections[i] != -1)// remap connections
					map_connect[i]=make_pair( fragments.size()-1, fragments[ rigid_groups[parent_id] ]->countAtoms() );
				
				fragments[ rigid_groups[parent_id] ]->insert(*atm);
			}
		}
	}
	
	/// iterate over all connections and assign connections to fragments:
	cout << endl;
	cout << "following inter fragment bonds exist:"<<endl;
	for(int i = 0; i < num_atoms; i++)
	{
		if(connections[i] == -1)
			continue;
		
		int n = connections[i];
		cout<< "Fragment "<<map_connect[i].first<<" atm "<<map_connect[i].second;
		cout<< " to Fragment "<< map_connect[n].first<<" atm "<<map_connect[n].second;
		cout<<endl;
	}

///======================Fragment     M A T C H I N G ==========================
/// Continue here with:
/// vector< int > connections
/// map < int , pair< int, int> map_connect
/// vector< Molecule*> fragments
	
	std::vector< OBMol* > conv_frags;
	std::vector< Molecule* >::iterator it;
	Molecule* new_mol;
	for(it=fragments.begin(); it != fragments.end(); it++)
	{
		clearExternalBonds(*it);
		OBMol* temp = MolecularSimilarity::createOBMol(**it, true);
		
		OBGraphSym grsym(temp);
		std::vector<unsigned int> sym;
		grsym.GetSymmetry(sym);
		
		std::vector<unsigned int> clabels;
		CanonicalLabels(temp, sym, clabels);
		
		new_mol = new Molecule;
		std::vector <Atom*> aList(num_atoms);
		for(int i=0; i<clabels.size(); i++)
			aList[clabels[i]-1]=( (*it)->getAtom(i) );
		
		for(int i=0; i<clabels.size(); i++)
			new_mol->append(*aList[i]);

		copyMoleculeProperies(**it, *new_mol);
		(*it)->swap(*new_mol);
		
		delete new_mol;
	}
	
	/// write output:
	String outfile_name = String(parpars.get("o"));
	SDFile outfile(outfile_name, ios::out);
	for(int i = 0; i < fragments.size(); i++)
	{
		outfile << *fragments[i];
	}
	
	Log << "wrote " << fragments.size() <<" fragments to file " << outfile_name << endl;
	
	// clean up:
	delete ball_mol;
	outfile.close();
}
