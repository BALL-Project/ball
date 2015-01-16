// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "fragmenter.h"

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/property.h>

#include <BALL/KERNEL/forEach.h>

#include <boost/pending/disjoint_sets.hpp>
MoleculeFragmenter::MoleculeFragmenter()
{
	_molecule = 0;
	_properties_set = false;
}

MoleculeFragmenter::~MoleculeFragmenter()
{
	if(_molecule != 0 && _properties_set)
		clearProperties();
}

void MoleculeFragmenter::setMolecule(AtomContainer &in_mol)
{
	_atom_list.clear();
	_atom_to_pos.clear();
	if(_molecule != 0 && _properties_set)
	{
		clearProperties();
	}
	
	_molecule = &in_mol;
	calcRingProperties();
	calcAtomToPos();
}

bool MoleculeFragmenter::isRigidAtom(Atom &atm)
{
	// if the atom is in a ring it is rigid
	if( atm.hasProperty("InRing") )// TODO: perhaps also test if this prop is true?
		 return true; 
	
	// if any neighboring bond is non single bond, the atom is rigid
	for(Atom::BondIterator bit = atm.beginBond(); +bit ; ++bit)
	{
		if( bit->getOrder() > 1 )
			return true;
	}
	
	// if the atom has only one connection that leads to a rigid atom, it self is considered rigid
	if( atm.countBonds() == 1 )
	{
		if ( isRigidAtom( * atm.beginBond()->getBoundAtom( atm )) ) //TODO: this recursion is not pretty/efficient
			return true;
		else
			return false;
	}
	
	return false;
}

bool MoleculeFragmenter::isRotableBond(Bond &bnd)
{
	// ring bonds or not single bonds: not rotable
	if( bnd.getOrder() > 1 || bnd.hasProperty("InRing"))
		return false;
	
	// if any of the binding partners is a terminal atom: not rotable
	if( bnd.getFirstAtom()->countBonds() ==1 || bnd.getSecondAtom()->countBonds() ==1)
		return false;
	
	return true;
}


bool MoleculeFragmenter::isBridgingBond(Bond &bnd)
{
	if( !isRotableBond(bnd) )
		return false;
	
	bool atm1_fixed = isRigidAtom(* bnd.getFirstAtom() );
	bool atm2_fixed = isRigidAtom(* bnd.getSecondAtom() );
	
	// rotable-bond between two fixed atoms: is a bridge
	if( atm1_fixed && atm2_fixed)
		return true;
	
	// rotable-bond between a fixed and flexible atom: is a bridge
	if( (atm1_fixed && !atm2_fixed) ||
			(!atm1_fixed && atm2_fixed) )
		return true;
	
	// rotable-bond between two flexibles: NOT a bridge
	return false;
}

void MoleculeFragmenter::getMoleculeFragments(FragVec &rigid_fragments, 
																							FragVec &linker_fragments, 
																							list<pair<Atom *, Atom *> > &connections)
{
	typedef boost::disjoint_sets < int*, int*, boost::find_with_full_path_compression > DisjointSet;
	int num_atoms = _molecule->countAtoms();
	
	/*
	 * Init the fragment indexing strcutrues:
	 */
	vector <int> fixed_rank(num_atoms);
	vector <int> fixed_parent(num_atoms);
	DisjointSet dset_rigid(&fixed_rank[0], &fixed_parent[0]);
	
	vector <int> linker_rank(num_atoms);
	vector <int> linker_parent(num_atoms);
	
	DisjointSet dset_linker(&linker_rank[0], &linker_parent[0]);
	
	vector <bool> is_rigid(num_atoms, 0); // bitVec indicating rigid atoms
	vector <bool> is_linker(num_atoms, 0); // bitVec indicating linker atoms
	

	AtomIterator ati;
	Atom::BondIterator bit;
	
	/*
	 * iterate over all bonds and split into rigid and linker fragments:
	 */
	BALL_FOREACH_BOND(*_molecule, ati, bit)
	{
		Atom* atm1 = bit->getFirstAtom();
		Atom* atm2 = bit->getSecondAtom();
		
		int id1 = _atom_to_pos[atm1];
		int id2 = _atom_to_pos[atm2];

		// for all rotable bonds:
		if ( isRotableBond(*bit) )
		{
			bool isRigid_atm1 = isRigidAtom(*atm1);
			bool isRigid_atm2 = isRigidAtom(*atm2);
			// do not double assign (deletes disjointset-parent status)
			if( !(is_linker[id1] || isRigid_atm1) )
			{
				dset_linker.make_set(id1);
				is_linker[id1]=1;
			}
			if( !(is_linker[id2] || isRigid_atm2) ) // do not double assign
			{
				dset_linker.make_set(id2);
				is_linker[id2]=1;
			}
			// add union atoms if both are not fixed:
			if( !(isRigid_atm1 || isRigid_atm2) )// both need to be linker atoms (checked via isAtomRigid(atm) )
			{
				dset_linker.union_set(id1, id2);
			}
			
			// check if we have a connection point:
			if( isBridgingBond( *bit) )
			{
				connections.push_back(make_pair(atm1, atm2));
			}
		}
		// for all fixed bonds:
		else
		{
			if( !is_rigid[id1] )
			{
				dset_rigid.make_set(id1); 
				is_rigid[id1]=1;
			}
			if( !is_rigid[id2] )
			{
				dset_rigid.make_set(id2); 
				is_rigid[id2]=1;
			}
			dset_rigid.union_set(id1,id2);
		}
	}
	
	/*
	 * iterate over all atoms, sorting out the  fixed and linker fragments
	 */
	vector <int> link_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	vector <int> rigid_groups(num_atoms, -1); // map parent id to vector pos in variable 'fragments'
	
	int parent_id = -1;
	// loop over the atoms, sort fragments out
	for(int i = 0 ; i < num_atoms; i++)
	{
		Atom* atm = &* _molecule->beginAtom();// simply pick atoms from the top, inserting will delete them from '_molecule'
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
		// Atom is rigid, extend or create a rigid fragment
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

void MoleculeFragmenter::calcRingProperties()
{
	_sssr.clear();
	_rpp.calculateSSSR(_sssr, *_molecule);
	_properties_set = true;
}

void MoleculeFragmenter::clearProperties()
{
	//clear properties via '_atom_list' it might be possible that '_molecule'
	// is emptied. Fragmenting reassigns the atoms to specific fragments.
	vector < Atom* >::iterator ati = _atom_list.begin();
	for(; ati != _atom_list.end(); ++ati)
	{
		(*ati)->PropertyManager::clear();
	}
}

void MoleculeFragmenter::calcAtomToPos()
{
	int pos = 0;
	for(AtomIterator ati = _molecule->beginAtom(); +ati; ++ati)
	{
		_atom_list.push_back( &*ati );
		_atom_to_pos[ &*ati ] = pos;
		pos++;
	}
}
