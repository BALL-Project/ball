// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/fragmenter.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>

#include <boost/pending/disjoint_sets.hpp>

using namespace BALL;
using namespace std;

MoleculeFragmenter::MoleculeFragmenter()
{
	_molecule = 0;
//	_rpp.options.set("algorithm_name", "Figueras");
}

MoleculeFragmenter::~MoleculeFragmenter(){}

void MoleculeFragmenter::setMolecule(AtomContainer &in_mol)
{
	_atom_list.clear();
	_atom_to_pos.clear();
	
	_molecule = &in_mol;
	
	// find ring atoms and bonds
	_sssr.clear();
	_rpp.calculateSSSR(_sssr, *_molecule);
	
	// remove properties, and save ring status internally
	extractAndClearProperties();
	
	// calculate internal indices for atoms
	calcAtomToPos();
}

bool MoleculeFragmenter::isRigidAtom(Atom &atm)
{
	// if the atom is in a ring it is rigid
	if( _is_InRing[ _atom_to_pos[&atm] ] )
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
		if ( isRigidAtom( * atm.beginBond()->getBoundAtom( atm )) ) //#WARNING: this recursion is not pretty/efficient, leads to endless iterations if we have a two atom molecule
			return true;
		else
			return false;
	}
	
	return false;
}

bool MoleculeFragmenter::isRotableBond(Bond &bnd)
{
	// ring bonds or not single bonds: not rotable
	if( bnd.getOrder() > 1 || _is_BondInRing[ &bnd ] )
		return false;
	
	// if any of the binding partners is a terminal atom: not rotable
	if( bnd.getFirstAtom()->countBonds() == 1 || bnd.getSecondAtom()->countBonds() == 1)
		return false;
	
	return true;
}

bool MoleculeFragmenter::isBridgingBond(Bond &bnd)
{
	if( !isRotableBond(bnd) )
		return false;
	
	bool is_fixed_atm1 = isRigidAtom(* bnd.getFirstAtom() );
	bool is_fixed_atm2 = isRigidAtom(* bnd.getSecondAtom() );
	
	// rotable-bond between two fixed atoms: is a bridge
	if( is_fixed_atm1 && is_fixed_atm2 )
		return true;
	
	// rotable-bond between a fixed and flexible atom: is a bridge
	if( (is_fixed_atm1 && !is_fixed_atm2 ) ||
			(!is_fixed_atm1 && is_fixed_atm2 ) )
		return true;
	
	// rotable-bond between two flexibles: NOT a bridge
	return false;
}

void MoleculeFragmenter::fragment(ACVec &rigid_fragments, 
																	ACVec &linker_fragments, 
																	ConnectList &connections)
{
	typedef boost::disjoint_sets < int*, int*, boost::find_with_full_path_compression > DisjointSet;
	int num_atoms = _molecule->countAtoms();
	
	rigid_fragments.clear();
	linker_fragments.clear();
	connections.clear();

	//0.) Catch trivial cases:
	if( num_atoms == 1 )
	{
		AtomContainer* dummy = new AtomContainer();
		
		dummy->insert( *_molecule->beginAtom() );
		
		linker_fragments.push_back(dummy);
		
		return;
	}
	else if( num_atoms == 2 )
	{
		AtomContainer* dummy = new AtomContainer();

		if( _molecule->beginAtom()->getBond( *(++_molecule->beginAtom()) )->getOrder() == 1 )
			linker_fragments.push_back( dummy );
		
		else
			rigid_fragments.push_back( dummy );

		dummy->insert( *_molecule->beginAtom() );
		dummy->insert( *_molecule->beginAtom() );

		return;
	}
	
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

		bool isRigid_atm1 = isRigidAtom(*atm1);
		bool isRigid_atm2 = isRigidAtom(*atm2);
		
		// Case1) rotable bonds:
		if ( isRotableBond(*bit) )
		{
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
			// union atoms if both atoms are not linker:
			if( !(isRigid_atm1 || isRigid_atm2) )
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
		// (note: terminal bonds are always fixed bonds, BUT their atoms may of
		// course be linker atoms. We need to handle these cases)
		else
		{
			// Case2) rigid bond, formed by rigid atoms only:
			if( isRigid_atm1 && isRigid_atm2)
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
				// always union rigid bonds (the atoms always belong to 1 rigid fragment)
				dset_rigid.union_set(id1,id2);
			}
			// Case3) rigid bond between flexible atoms (e.g.: terminal bond in linker fragment)
			else
			{
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
				// we assume that both are linker atoms
				dset_linker.union_set(id1, id2);
			}
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
				AtomContainer* dummy = new AtomContainer();
				
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
				AtomContainer* dummy = new AtomContainer();
				
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



void MoleculeFragmenter::fragmentToSites(
		vector<pair<String, AtomContainer *> > &connections, 
		bool restrict_to_rotables)
{
	connections.clear();
	
	AtomIterator ati;
	AtomBondIterator bit;
	if( restrict_to_rotables )
	{
		BALL_FOREACH_BOND(*_molecule, ati, bit)
			if( isRotableBond(*bit) )
				addBondToConnectionsLib(*bit, connections);
	}
	else
	{
		BALL_FOREACH_BOND(*_molecule, ati, bit)
			if( bit->getOrder() == 1 )
				addBondToConnectionsLib(*bit, connections);
	}
	
}


void MoleculeFragmenter::extractAndClearProperties()
{
	_is_BondInRing.clear();
	_is_InRing.clear();
	
	// transfer and clear Bond-properties
	AtomIterator ati0;
	Atom::BondIterator bit;
	
	BALL_FOREACH_BOND(*_molecule, ati0, bit)
	{
		if( bit->hasProperty("InRing") )
		{
			_is_BondInRing[ &*bit ] = true;
		}
		else
			_is_BondInRing[ &*bit ] = false;
		
		bit->PropertyManager::clear();
	}
	
	// transfer and clear Atom-properties
	for(AtomIterator ati = _molecule->beginAtom(); +ati; ++ati)
	{
		if( ati->hasProperty("InRing") )
		{
			_is_InRing.push_back(true);
		}
		else
			_is_InRing.push_back(false);
		
		ati->PropertyManager::clear();
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

void MoleculeFragmenter::addBondToConnectionsLib(
		Bond &bnd,
		vector< pair< String, AtomContainer*> > &connections)
{
	String key1, key2;
	Atom* at1 = bnd.getFirstAtom();
	Atom* at2 = bnd.getSecondAtom();
	
	// get site and add to the lib:
	AtomContainer* site1 = getSite( at1, key1);
	AtomContainer* site2 = getSite( at2, key2);
	
	connections.push_back( make_pair( key1, site1) );
	connections.push_back( make_pair( key2, site2) );
}

AtomContainer *MoleculeFragmenter::getSite(Atom *atm, String &key)
{
	AtomContainer* site = new AtomContainer;
	// insert central atom for the site and the key
	Atom* fst_at = new Atom( *atm);
	site->insert( *fst_at );
	key = atm->getElement().getSymbol();
	
	Composite* root = & atm->getRoot();
	
	// structure to sort the neighbors according to their names (element+BO)
	vector< pair<String,Atom*> > names_neighbors;

	// add all neighbors to 'elements' (central atom is not contained)
	for(Atom::BondIterator b_it = atm->beginBond(); +b_it; b_it++)
	{
		Atom* tmp_atm = b_it->getBoundAtom(*atm); // get neighbors of central 'atm'
		
		String elem = tmp_atm->getElement().getSymbol();
		
		elem += String( b_it->getOrder() );
		
		names_neighbors.push_back( make_pair( elem, tmp_atm) );
	}
	
	// sort identifers of neighbors
	vector< pair<String, Atom*> >::iterator name_it = names_neighbors.begin();
	sort( name_it, names_neighbors.end(), compare );
	
	// create the key, and add sorted neighbors to the site
	for(name_it = names_neighbors.begin(); name_it !=names_neighbors.end(); name_it++)
	{
		key += (*name_it).first;
		
		if( root->isAncestorOf( *name_it->second ) )
		{
			Atom* tmp_at = new Atom( * name_it->second);
			
			Bond* new_bnd = new Bond();
			Bond* orig_bnd = atm->getBond( *name_it->second );
			
			new_bnd->setOrder( orig_bnd->getOrder() );
			
			tmp_at->createBond( *new_bnd, *fst_at);
			
			site->insert( *tmp_at );
		}
	}
	
	return site;
}

/*
 * compare
 */
bool MoleculeFragmenter::compare(pair<String,Atom*>& a, pair<String,Atom*>& b)
{
	return a.first < b.first;
}


