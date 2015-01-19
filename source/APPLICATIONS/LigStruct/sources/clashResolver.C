// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "clashResolver.h"
#include <BALL/KERNEL/PTE.h>
ClashResolver::ClashResolver()
{
	_large_rotors = 0;
	_small_rotors = 0;
}


ClashResolver::~ClashResolver()
{
	
}

void ClashResolver::setMolecule(Atom &atm1, Atom &atm2, ConnectList &connections)
{
	atm_large = &atm1;
	atm_small = &atm2;
	
	_large_parent = (AtomContainer* )&atm1.getRoot();
	_large_root   = (AtomContainer* ) atm1.getParent();
	
	_small_parent = (AtomContainer* )&atm2.getRoot();
	_small_root   = (AtomContainer* ) atm2.getParent();
	
	if(_small_root == _large_root)
	{
		cout<<"ERROR: clashResolver got two identical roots"<<endl;
		exit(EXIT_FAILURE);
	}
	
	for ( auto& p : connections)
	{
		if( &p.first->getRoot() == _large_root && &p.second->getRoot() == _large_root)
			_large_rotors->push_back( p );
		else if( &p.first->getRoot() == _small_root && &p.second->getRoot() == _small_root)
			_small_rotors->push_back( p );
	}
}

int ClashResolver::detect()
{
	int clash_count = 0;
	for(AtomIterator ati1 = _small_root->beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = _large_root->beginAtom(); +ati2; ++ati2)
		{
			if ( doClash( *ati1, *ati2) )
				++clash_count;
		}
	}
	return clash_count;
}

int ClashResolver::detectAll()
{
	int clash_count = 0;
	
	// clashes between the two fragments
	for(AtomIterator ati1 = _small_root->beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = _large_root->beginAtom(); +ati2; ++ati2)
		{
			if ( doClash( *ati1, *ati2) )
				++clash_count;
		}
	}
	
	// clashes wihtin the large fragment
	for(AtomIterator ati1 = _large_root->beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2 ; +ati2; ++ati2)
		{
			if ( doClash( *ati1, *ati2) )
				++clash_count;
		}
	}
	
	// clashes wihtin the small fragment
	for(AtomIterator ati1 = _small_root->beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2; +ati2; ++ati2)
		{
			if ( doClash( *ati1, *ati2) )
				++clash_count;
		}
	}
	
	return clash_count;
}

bool ClashResolver::doClash(Atom &atm1, Atom &atm2)
{
	float dist = atm1.getDistance(atm2);
	float ideal = atm1.getElement().getVanDerWaalsRadius() + atm2.getElement().getVanDerWaalsRadius();
	float tolerance = 0.2;
	
	if (dist + tolerance < ideal)
		return true;
	
	return false;
}

int ClashResolver::resolve()
{
	int clash_count = 0;
	// first try the original connection
	
	// next try all rotors in the small fragment
	
	// at last try all rotors in the large one
	
	return clash_count;
}
