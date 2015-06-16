// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/LIGAND3DGEN/combiAssembler.h>

using namespace std;
using namespace BALL;

CombiAssembler::CombiAssembler(TemplateDatabaseManager &data, CombiLibMap* clib):
	_work_mol(0), _r_groups(clib)
{
	_connector.setLibs(data.getSiteTemplates(), data.getBondLengthData());
}

CombiAssembler::~CombiAssembler()
{
	
}

void CombiAssembler::setScaffold(RFragment &scaffold)
{
	_work_mol = &scaffold;
}

void CombiAssembler::setCombiLib(CombiLibMap &clib)
{
	_r_groups = &clib;
}

void CombiAssembler::writeCombinations(SDFile &handle)
{
	_r_atms.clear();
	
	_work_mol = _r_groups->at(0)[0];
	_current_combination.push_back(_work_mol);
	
	for(list<RAtom>::iterator it = _work_mol->r_atom_lst.begin(); 
			it != _work_mol->r_atom_lst.end(); ++it)
	{
		_r_atms.push_back( &*it );
	}
	
	
	_combineRecur( handle);
}


void CombiAssembler::_combineRecur(SDFile &handle)
{
	//1.) get next RAtom:
	RAtom* ra=0;
	if( ! _r_atms.empty() )
	{
		ra = _r_atms.front();
		_r_atms.pop_front(); // deeper recursions shall not handle this r-atom again
	}
	else
	{
		ra = 0;
	}
	
	//2.) check for end-case:
	if( !ra )
	{
		//write out the current work mol
		handle << * _work_mol->molecule;
			
		return;
	}
	//3.) recursion case:
	else
	{
		// iterate over all RFrags in the RGroup fitting to the current R-atom 'ra':
		vector<RFragment*>& group = _r_groups->at(ra->id);
		vector<RFragment*>::iterator it2;
		
		for(it2 = group.begin(); it2 != group.end(); ++it2)
		{
			RFragment* tmp = *it2;
			Bond* bnd = 0;
			
			//3.1) extend '_r_amts' with the RAtoms of the new RFragment same for the
			//     rotors
			for( list< RAtom >::iterator it = tmp->r_atom_lst.begin(); 
					 it != tmp->r_atom_lst.end(); ++it)
				_r_atms.push_back( &*it );
			
			for( ConnectList::iterator it = tmp->rotor_lst->begin();
					 it != tmp->rotor_lst->end(); ++it)
				_work_mol->rotor_lst->push_back( *it );
			
			//3.2) connect the molecule to the scaffold and create a bond
			Bond b;
			b.setOrder(1);
			bnd = ra->atm->createBond(b, *tmp->group_atom );
			
			//3.3) connect the two fragments:
			_checkAndConnect(*ra, *tmp);
			
			// insert new fragment into working mol
			_work_mol->molecule->insert( *tmp->molecule );
			_current_combination.push_back( tmp );
			
			//3.4) recurr deeper
			_combineRecur( handle );
			
			//3.5) delete bond, and remove inserted molecule
			bnd->destroy();
			_work_mol->molecule->remove( *tmp->molecule );
			_current_combination.pop_back();
			
			//3.6) remove the added r_atom from the list again
			for( list< RAtom >::iterator it = tmp->r_atom_lst.begin();
					 it != tmp->r_atom_lst.end(); ++it)
				_r_atms.pop_back();
			
			for(ConnectList::iterator it = tmp->rotor_lst->begin(); 
					it != tmp->rotor_lst->end(); ++it)
				_work_mol->rotor_lst->pop_back();
		}
	}
	_r_atms.push_front( ra ); // upper calls of writeCombinations should handle this atom first
}

void CombiAssembler::newSetForCurrentCombination()
{
	for(list< RFragment* >::iterator it = _current_combination.begin(); 
			it != _current_combination.end(); ++it)
	{
		RFragment* tmp = *it;
		tmp->newSetFromCurrent();
	}
}

void CombiAssembler::_checkAndConnect(RAtom &acceptor, RFragment &donor)
{
	int acc_id = acceptor.parent->curr_set;
	int donor_id = donor.curr_set;
	
	/*
		 * 1.) the acceptor and donor do have a conformation
		 */
	if( acc_id != -1 && donor_id != -1)
	{
		int compatible_donor_set = acceptor.getCompatibleSet(donor);
		
		// 1.1) known Fragments are compatible!
		if ( compatible_donor_set != -1)
		{
			donor.setCoordsTo( compatible_donor_set ); // to the fitting coordinates
			
			_cresolv.setMolecule(*acceptor.atm, *donor.group_atom, *_work_mol->rotor_lst);
			int c_cnt = _cresolv.detect();
			
			// if we need to resolve a clash...
			if ( c_cnt != 0 )
			{
				pair<int,bool> res = _cresolv.resolve();
				
				// resolving the clash, led to a new coordinate set:
				if( res.second )
				{
					newSetForCurrentCombination();
					
					donor.newSetFromCurrent();
					
					acceptor.addParnter(donor);
				}
				
				// only the fragment was changed
				else
				{
					donor.newSetFromCurrent();
					acceptor.addParnter(donor);
				}
			}
		}
		
		// 1.2) acceptor and donor have one or more sets, BUT donor is not 
		//      compatible with given acceptor!
		// same as case 2.)
		else
		{
			if(_connectClashFree( *acceptor.atm, *donor.group_atom, *_work_mol->rotor_lst))
			{
				newSetForCurrentCombination();
			}
			
			donor.newSetFromCurrent();
			
			acceptor.addParnter(donor);
		}
	}
	
	/*
		 * 2.) only the acceptor side has a conformation
		 */
	else if (acc_id != -1 && donor_id == -1) 
	{
		// if the known acceptor side was changed: add both coordinates...
		if(_connectClashFree( *acceptor.atm, *donor.group_atom, *_work_mol->rotor_lst))
		{
			newSetForCurrentCombination();
		}
		
		//...else: only the ones from the donor
		donor.newSetFromCurrent();
		
		acceptor.addParnter(donor);
	}
	/*
		 *  3.) both are new (should only occur in the very first recursion)
		 */
	else
	{
		_connectClashFree( *acceptor.atm, *donor.group_atom, *_work_mol->rotor_lst);
		
		newSetForCurrentCombination();
		
		donor.newSetFromCurrent();
		
		acceptor.addParnter( donor );
	}
}


/*
 * at1 == atom from scaffold
 * at2 == atom from new R-fragment
 */
bool CombiAssembler::_connectClashFree(Atom &at1, Atom &at2, ConnectList& connections)
{
	int c_cnt;

	_connector.connect( &at1, &at2 );
	
	// 2.) detect and resolve clashes:
	_cresolv.setMolecule(at1, at2, connections);
	
	c_cnt = _cresolv.detect();
	
//	cout<<"connection clashes: "<<c_cnt<<endl;
	
	if( c_cnt != 0 )
	{
		//pair<int,bool> res = _cresolv.resolve();
		
		return _cresolv.resolve().second;
	}
	
	return false; // nothing changed
}
