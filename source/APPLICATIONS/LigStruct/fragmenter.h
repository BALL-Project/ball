#ifndef FRAGMENTER_H
#define FRAGMENTER_H

#include "basic.h"

using namespace OpenBabel;
using namespace BALL;
using namespace std;


/// check if the atom is a rigid one:
bool isAtomRigid(OBAtom* atm)
{
/// TODO: add OBRotorList object to use custom torlib!
	if (atm->IsInRing())
		return true;
	else
	{
		OBBondIterator b_it = atm->BeginBonds();
		for(;  b_it != atm->EndBonds();  b_it++)
		{
			if( ! (*b_it)->IsRotor() )
				return true;
		}
		return false;
	}
}


///####################### F R A G M E N T I N G ##############################
void fragmentMolecule(OBMol& ob_mol, 
											Molecule& ball_mol, 
											vector <Fragment*>& rigid_fragments, 
											vector <Fragment*>& linker_fragments, 
											list< pair<Atom*, Atom*> >& connections)
{
	typedef boost::disjoint_sets < int*, int*, boost::find_with_full_path_compression > DisjointSet;
	int num_atoms = ob_mol.NumAtoms();
	
	// create 2 empty disjoint sets:
	std::vector <int> fixed_rank(num_atoms);
	std::vector <int> fixed_parent(num_atoms);
	DisjointSet dset_rigid(&fixed_rank[0], &fixed_parent[0]);
	
	std::vector <int> linker_rank(num_atoms);
	std::vector <int> linker_parent(num_atoms);
	DisjointSet dset_linker(&linker_rank[0], &linker_parent[0]);
	
	std::vector <bool> is_rigid(num_atoms, 0); // bitVec indicating rigid atoms
	std::vector <bool> is_linker(num_atoms, 0); // bitVec indicating linker atoms

	/** 
	 * iterate over all bonds and split into rigid and linker fragments:
	 */
	OBBondIterator b_it = ob_mol.BeginBonds();
	for (; b_it != ob_mol.EndBonds(); b_it++)
	{
		OBAtom* atm1 = (*b_it)->GetBeginAtom();
		OBAtom* atm2 =  (*b_it)->GetEndAtom();
		int id1 = atm1->GetId();
		int id2 = atm2->GetId();

		// for all rotable bonds:
		if ( (*b_it)->IsRotor() )
		{
			bool isRigid_atm1 = isAtomRigid(atm1);
			bool isRigid_atm2 = isAtomRigid(atm2);
			if( !(is_linker[id1] || isRigid_atm1) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id1);
				is_linker[id1]=1;
			}
			if( !(is_linker[id2] || isRigid_atm2) ) // do not double assign (deletes disjointset-parent status)
			{
				dset_linker.make_set(id2);
				is_linker[id2]=1;
			}
			// add union atoms if both are not fixed:
			if( !(isRigid_atm1 || isRigid_atm2) )// both need to be linker atoms (checked via isAtomRigid(atm) )
			{
				dset_linker.union_set(id1, id2);
			}
			
			// rotables to a rigid fragment define connection points:
			if( isAtomRigid((*b_it)->GetBeginAtom()) || isAtomRigid((*b_it)->GetEndAtom()) )
			{
				Atom* atm1 = ball_mol.getAtom(id1);
				Atom* atm2 = ball_mol.getAtom(id2);
				connections.push_back(make_pair(atm1, atm2));
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
	
	/**
	 * iterate over all atoms, sorting out the  fixed and linker fragments
	 */
	std::vector <int> link_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	std::vector <int> rigid_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	
	int parent_id = -1;
	// loop over the atoms, sort fragments out
	for(int i = 0 ; i < num_atoms; i++)
	{
		Atom* atm = ball_mol.getAtom(0); // simply pick the atoms from the top
		// atom is linker, extend or create a linker fragment
		if ( is_linker[i] )
		{
			parent_id = dset_linker.find_set(i);
			
			// if the fragment does not exist, create it:
			if(link_groups[parent_id]<0)
			{
				link_groups[parent_id] = linker_fragments.size();
				Fragment* dummy = new Fragment();
				
				dummy->insert(*atm);
				
				linker_fragments.push_back(dummy);
			}
			// atom is part of existing fragment:
			else
			{
				linker_fragments[ link_groups[parent_id] ]->insert(*atm);
			}
		}
		// Atom is a rigid, extend or create a rigid fragment
		else
		{
			parent_id = dset_rigid.find_set(i);
			// if fragment does not exist, create it:
			if(rigid_groups[parent_id]<0)
			{
				rigid_groups[parent_id] = rigid_fragments.size();
				Fragment* dummy = new Fragment();
				
				dummy->insert(*atm);
				rigid_fragments.push_back(dummy);
			}
			else
			{
				rigid_fragments[ rigid_groups[parent_id] ]->insert(*atm);
			}
		}
	}
}


#endif // FRAGMENTER_H
