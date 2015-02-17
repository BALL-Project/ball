// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "combiAssembler.h"

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
	RAtom* ra;
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
		// write out the current work mol
		handle << * _work_mol->molecule;
//		cout<<(*_r_groups)[0][0]->coord_sets.size()<<endl;
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
			
			for( ConnectList::iterator it = tmp->rotor_lst.begin();
					 it != tmp->rotor_lst.end(); ++it)
				_work_mol->rotor_lst.push_back( *it );
			
			//3.2) connect the molecule to the scaffold and create a bond
			Bond b;
			b.setOrder(1);
			bnd = ra->atm->createBond(b, *tmp->group_atom );
			
			//3.3) connect the two fragments:
			_checkAndConnect(*ra, *tmp);
			_work_mol->molecule->insert( *tmp->molecule );
			
			//3.4) recurr deeper
			_combineRecur( handle );
			
			//3.5) delete bond, and remove inserted molecule
			bnd->destroy();
			_work_mol->molecule->remove( *tmp->molecule );
			
			//3.6) remove the added r_atom from the list again
			for( list< RAtom >::iterator it = tmp->r_atom_lst.begin();
					 it != tmp->r_atom_lst.end(); ++it)
				_r_atms.pop_back();
			
			for(ConnectList::iterator it = tmp->rotor_lst.begin(); 
					it != tmp->rotor_lst.end(); ++it)
				_work_mol->rotor_lst.pop_back();
		}
	}
	
	_r_atms.push_front( ra ); // upper calls of writeCombinations should handle this atom first
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
		// known Fragment?!
		int donor_target_set = acceptor.getCompatibleSet(donor);
		if ( donor_target_set != -1)
		{
//			cout<<"Reusing coordinates!"<<endl;//#DEBUG
			
			donor.setCoordsTo( donor_target_set ); // to the fitting coordinates
			
			_cresolv.setMolecule(*acceptor.atm, *donor.group_atom, _work_mol->rotor_lst);
			int c_cnt = _cresolv.detect();
			
			// if we need to resolve a clash...
			if (c_cnt != 0)
			{
//				Global_Timer::timer2.start(); //#DEBUG: time measure!
				pair<int,bool> res = _cresolv.resolve();
//				Global_Timer::timer2.stop(); //#DEBUG: time measure!
				
				// resolving the clash, led to a new coordinate set:
				if( res.second )
				{
					acceptor.parent->newSetFromCurrent();
					donor.newSetFromCurrent();
					
					acceptor.addParnter(donor);
				}
			}
			return;
		}
	}
	
	/*
	 * 2.) only the acceptor side has a conformation
	 */
	else if (acc_id != -1 && donor_id == -1) 
	{
		// if the known acceptor side was changed: add both coordinates...
		if(_connectClashFree( *acceptor.atm, *donor.group_atom, _work_mol->rotor_lst))
			acceptor.parent->newSetFromCurrent();
		
		//...else: only the ones from the donor
		donor.newSetFromCurrent();
			
		acceptor.addParnter(donor);
	}
	/*
	 *  3.) both are new (should only occur at the beginning)
	 */
	else
	{
		_connectClashFree( *acceptor.atm, *donor.group_atom, _work_mol->rotor_lst);
		
		acceptor.parent->newSetFromCurrent();
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
	
	if( c_cnt != 0 )
	{
//		Global_Timer::timer1.start(); //#DEBUG: time measure!
//		cout<<"new connection clash!"<<endl;
		return _cresolv.resolve().second;
//		Global_Timer::timer1.stop(); //#DEBUG: time measure!
	}
	
	return false; // nothing changed
}
