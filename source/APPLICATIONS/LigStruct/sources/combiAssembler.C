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
	
	for(list<RAtom>::iterator it = _work_mol->r_atom_lst.begin(); it != _work_mol->r_atom_lst.end(); ++it)
	{
		_r_atms.push_back( &*it );
	}
	
	_combineRecur( handle );
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
			RFragment* tmp = new RFragment( **it2 );
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
			
			//3.3) connect the two fragments clashFree:
			connectClashFree(*ra->atm, *tmp->group_atom, _work_mol->rotor_lst);
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
			
			delete tmp;
		}
	}
	
	_r_atms.push_front( ra ); // upper calls of writeCombinations should handle this atom first
}

/*
 * at1 == atom from scaffold
 * at2 == atom from new R-fragment
 */
void CombiAssembler::connectClashFree(Atom &at1, Atom &at2, ConnectList& connections)
{
	//1.) select biggest molecule as 1st connection partner. 
	//    REASON: we assume that our connection method keeps the first fragment 
	//    in place and only transforms the second fragment!

	// get root container to be able to identify all connected atoms of the
	// respective connection-atom:
//	AtomContainer* root_1 = (AtomContainer*) &at1.getRoot();
//	AtomContainer* root_2 = (AtomContainer*) &at2.getRoot();
	
//	cout<<endl<<endl<<"#### Connecting:"<<endl;
	int c_cnt;

//	cout<<LigBase::printInlineMol(root_1)<<endl;
//	cout<<LigBase::printInlineMol(root_2)<<endl;
	
	_connector.connect( &at1, &at2 );
//	cout<<"connected molecules"<<endl;
	
	// 2.) detect and resolve clashes:
	_cresolv.setMolecule(at1, at2, connections);
	c_cnt = _cresolv.detect();
	if( c_cnt != 0 )
	{
//		cout<<"Resolving clash, got: "<<c_cnt<<endl;
		c_cnt = _cresolv.resolve();
//		if(c_cnt != 0)
//			cout<<"Resolving finished with: "<<c_cnt<<endl;
	}
	
}
