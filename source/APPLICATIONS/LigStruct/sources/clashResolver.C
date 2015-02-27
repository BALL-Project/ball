// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "clashResolver.h"
#include "clashBase.h"

#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

using namespace std; 
using namespace BALL;

/// C l a s s   C l a s h R e s o l v e r
/// ############################################################################
ClashResolver::ClashResolver(float tolerance, int max_rotors): 
	ClashDetector(tolerance),
	_max_rotations(max_rotors)
{
	_all_rotors = 0;
	
	_molecule = 0;
}

ClashResolver::~ClashResolver()
{}

void ClashResolver::setMolecule(AtomContainer& molecule, ConnectList& rotors)
{
	_all_rotors = &rotors;
	
	_molecule = &molecule;
	
	_max_rotations = rotors.size() * 5;
}

int ClashResolver::detect()
{
	return detectInMolecule( *_molecule );
}

int ClashResolver::resolve()
{
	pair< Atom*, Atom* >* last_rotor = 0;
	pair< Atom*, Atom* >* best_rotor = 0;
	
	TemplateCoord best_conf( *_molecule );
	
	int best_cnt = detectInMolecule( *_molecule );
	int current_cnt = 0;
	
	// try optimise bonds, use two times the size of '_all_rotors' optimization steps
	for(int tries=0; tries < 2*_all_rotors->size(); tries++)
	{
		best_rotor = 0;
		
		// for each given rotor, test 36 angles and find the one best rotation 
		// among these
		for( ConnectList::iterator it = _all_rotors->begin();
				 it!= _all_rotors->end(); ++it)
		{
			// do not optimize the same rotor in two consecutive steps
			if( &*it == last_rotor )
				continue;
			
			// use the currently known best conformation:
			best_conf.applyCoordinates2Molecule( *_molecule );
			
			Rotator roto;
			roto.setAxis( *it->first, *it->second );
			
			// test 36 rotations for the selected rotor:
			for( int i = 1; i < 36; ++i)
			{
				roto.rotate( Angle(10.0, false) ); // perfrom a rotation on '_molecule'
				
				current_cnt = detectInMolecule( *_molecule ); // get score for the resulting conformation
				
				if ( current_cnt < best_cnt )
				{
					best_cnt = current_cnt;
					best_conf.readCoordinatesFromMolecule( *_molecule );
					best_rotor = &*it;
				}
				
				if( current_cnt == 0)
					return 0;
			}
		}
		
		// set to the best found conformation:
		best_conf.applyCoordinates2Molecule( *_molecule );
		
		if( best_rotor != 0)
			last_rotor = best_rotor;
		
		// last round was not able to find any better rotation and did not return
		// because of 0 clashes. Return early, we will not be able to find any 
		// better rotation in further optimisation trials
		else
		{
			return best_cnt;
		}
	}
	
	return best_cnt;
}

int ClashResolver::resolveAllCombinations( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	return allCombinationsRecur( _all_rotors->begin(), _all_rotors->end(), rot_angle, steps);
}

int ClashResolver::allCombinationsRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps)
{
	int current_cnt = numeric_limits<int>::max();
	
	Rotator roto;
	roto.setAxis(* p->first, * p->second);
	
	for( int i = 0; i < steps-1; ++i )
	{
		roto.rotate( angle );
		
		current_cnt = detect();
		
		// this rotation solved ALL clashes
		if( current_cnt == 0)
		{
			return 0;
		}
		// If the rotation did not work, try rotating the other bonds, if some
		// a not rotated yet
		else
		{
			ConnectList::iterator next_p = p; ++next_p;  
			if( next_p != p_end )
			{
				current_cnt = allCombinationsRecur( next_p, p_end, angle, steps);
				
				// some other rotation based on this rotation solved ALL clashes
				if ( current_cnt == 0)
				{
					return 0;
				}
				// the other bonds could not create a solution with this rotation
				else
				{
					continue; // try next rotation of this bond
				}
			}
		}
		
	}
	
	return current_cnt;
}

/// C l a s s   C o n n e c t i o n R e s o l v e r
/// ############################################################################
ConnectionResolver::ConnectionResolver(float tolerance, int max_rotors): 
	ClashDetector(tolerance),
	_max_rotations(max_rotors),
	atm_large(0), 
	atm_small(0), 
	_large_root(0),
	_small_root(0),
	_save_large(0),
	_save_small(0),
	_large_rotors( new ConnectList() ),
	_small_rotors( new ConnectList() ),
	_all_rotors( new ConnectList() )
{}


ConnectionResolver::~ConnectionResolver()
{
	delete _large_rotors; _large_rotors = 0;
	delete _small_rotors; _small_rotors = 0;
	delete _all_rotors; _all_rotors = 0;
	
	delete _save_large; _save_large = 0;
	delete _save_small; _save_small = 0;
}

void ConnectionResolver::setMolecule(
		Atom &atm1, Atom &atm2, ConnectList &connection_rotors)
{
	atm_large = &atm1;
	atm_small = &atm2;
	
	_large_root = (AtomContainer* )&atm1.getRoot();
	_small_root = (AtomContainer* )&atm2.getRoot();
	
	// Allocate the best-conformation-containers:
	_save_large = new TemplateCoord( _large_root->countAtoms() );
	_save_small = new TemplateCoord( _small_root->countAtoms() );
	
	if(_small_root == _large_root)
	{
		cout<<"ERROR: ConnectionClashResolver got two identical roots"<<endl;
		exit(EXIT_FAILURE);
	}
	
	// Initialize rotorlists:
	_large_rotors->clear();
	_small_rotors->clear();
	_all_rotors->clear();
	
	for (ConnectList::iterator p = connection_rotors.begin(); p!=connection_rotors.end(); ++p)
	{
		if( &p->first->getRoot() == _large_root && &p->second->getRoot() == _large_root)
		{
			_large_rotors->push_back( *p );
			_all_rotors->push_back( *p );
		}
		else if( &p->first->getRoot() == _small_root && &p->second->getRoot() == _small_root)
		{
			_small_rotors->push_back( *p );
			_all_rotors->push_back( *p );
		}
	}
	
	_small_rotors->push_back( make_pair(&atm1,&atm2) );
	_large_rotors->push_back( make_pair(&atm1,&atm2) );
	_all_rotors->push_back( make_pair(&atm1,&atm2) );
	
	_max_rotations = _all_rotors->size() * 2;
	
	cout<<"set rotors. small: "<< _small_rotors->size() <<" large: "<< _large_rotors->size() << " all: "<< _all_rotors->size() <<endl;
}

int ConnectionResolver::detect()
{
	return detectBetweenMolecules( *_small_root, *_large_root );
}

pair<int, bool> ConnectionResolver::resolve()
{
	_save_large->readCoordinatesFromMolecule( *_large_root );
	_save_small->readCoordinatesFromMolecule( *_small_root );
	
	const int given_large_cnt = detectInMolecule( *_large_root);
	const int given_small_cnt = detectInMolecule( *_small_root);
	cout<<"resolving, given counts: large "<< given_large_cnt 
			<<" small "<< given_small_cnt<<endl;
	
	bool changed_large = false;
	
	//1) CONNECTION rotation might solve the clashes
	int clash_cnt = resolveConnection();
	
	cout<<"after conntection rotation: "<<clash_cnt<<endl;
	cout<<"inter: "<<detectBetweenMolecules(*_small_root, *_large_root)<<endl;
	cout<<"small: "<<detectInMolecule(*_small_root)<<endl;
	cout<<"large: "<<detectInMolecule(*_large_root)<<endl;
	
	//2) SMALL FRAGMENT rotations:
	if( clash_cnt != 0 && _small_rotors->size() > 1 )
	{
		clash_cnt = resolveFragment( *_small_root, *_small_rotors, given_small_cnt);
	}
	
	cout<<"after small rotation: "<<clash_cnt<<endl;
	cout<<"inter: "<<detectBetweenMolecules(*_small_root, *_large_root)<<endl;
	cout<<"small: "<<detectInMolecule(*_small_root)<<endl;
	cout<<"large: "<<detectInMolecule(*_large_root)<<endl;
	
	//3) LARGE FRAGMENT rotations:
	// * also rotate connecting bond -> DONE: that bond is simply added to the rotors
	// * also rotate all bonds in small_root (use '_all_rotors') when working on large
	// * check rotations: do not restrict them to large fragment in this case
	if( clash_cnt != 0 && _large_rotors->size() > 1 )
	{
		clash_cnt = resolveLarge( given_large_cnt + given_small_cnt );
		changed_large = true;
	}

		cout<<"after large rotation: "<<clash_cnt<<endl;
		cout<<"inter: "<<detectBetweenMolecules(*_small_root, *_large_root)<<endl;
		cout<<"small: "<<detectInMolecule(*_small_root)<<endl;
		cout<<"large: "<<detectInMolecule(*_large_root)<<endl;
	return make_pair( detectAll(), changed_large );
}

int ConnectionResolver::resolveConnection()
{
	int current_count = 0;
	
	int best_cnt = detectBetweenMolecules( *_large_root, *_small_root);
	
	Rotator roto;
	roto.setAxis(*atm_large, *atm_small, _small_root);
	
	// test 36 angles/rotations along the connecting bond, select the best one
	for( int i = 1; i < 36; ++i)
	{
		roto.rotate( Angle(10.0, false) );
		
		current_count = detectBetweenMolecules( *_large_root, *_small_root);
		cout<<"connection: "<<current_count<<endl;
		
		if ( current_count < best_cnt)
		{
			best_cnt = current_count;
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}

		// we could find a clash-free solution
		if( current_count == 0)
		{
			cout<<" EARLY exit!"<<endl;
			return 0;
		}
	}
	// we could not find a clash-free solution, but we use the best conformation
	// that was visited:
	_save_small->applyCoordinates2Molecule( *_small_root );

	return best_cnt;
}

int ConnectionResolver::resolveFragment(AtomContainer& frag,
																				ConnectList&   clist,
																				const int&     given_clashes)
{
	int best_cnt = detectBetweenMolecules(*_large_root, *_small_root) + detectInMolecule( frag ) - given_clashes;
	int current_count = 0;
	
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
//		cout<<endl<<"try: "<<tries<<endl;
		for(ConnectList::iterator p = clist.begin(); p != clist.end(); ++p)
		{
			_save_large->applyCoordinates2Molecule( *_large_root );
			_save_small->applyCoordinates2Molecule( *_small_root );
			
			// else: test 20 rotations for that rotor
			Rotator roto;
			roto.setAxis(*p->first, *p->second, &frag.getRoot());
			
//			cout<<"testing small: "<<p->first<<" - "<<p->second<<endl;
			
			for( int i = 1; i < 20; ++i)
			{
				roto.rotate( Angle(18.0, false) );
				
				current_count = detectBetweenMolecules(*_large_root, *_small_root)
											+ detectInMolecule( frag ) - given_clashes;
				
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					
					_save_large->readCoordinatesFromMolecule( *_large_root );
					_save_small->readCoordinatesFromMolecule( *_small_root );
				}
				
				if( current_count == 0)
				{
					return 0;
				}
			}
		}
		
		_save_large->applyCoordinates2Molecule( *_large_root );
		_save_small->applyCoordinates2Molecule( *_small_root );
	}
	return best_cnt;
}

int ConnectionResolver::resolveLarge( const int &given_clashes)
{
	int best_cnt = detectAll() - given_clashes;
	int current_count = 0;
	
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
		
		for(ConnectList::iterator p = _all_rotors->begin(); p != _all_rotors->end(); ++p)
		{
			_save_large->applyCoordinates2Molecule( *_large_root );
			_save_small->applyCoordinates2Molecule( *_small_root );
			
			// else: test 20 rotations for that rotor
			Rotator roto;
			roto.setAxis(*p->first, *p->second);
						
			for( int i = 1; i < 20; ++i)
			{
				roto.rotate( Angle(18.0, false) );
				
				current_count = detectAll() - given_clashes;
				
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					
					_save_large->readCoordinatesFromMolecule( *_large_root );
					_save_small->readCoordinatesFromMolecule( *_small_root );
				}
				
				if( current_count == 0)
				{
					return 0;
				}
			}
		}
		
		_save_large->applyCoordinates2Molecule( *_large_root );
		_save_small->applyCoordinates2Molecule( *_small_root );
	}
	return best_cnt;
}


/* 
 * resolve           A L L
 */
int ConnectionResolver::resolveAllCombinations( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	// set best clash cnt to current status
	int best_cnt = detectBetweenMolecules(*_large_root, *_small_root) 
									+ detectInMolecule( *_large_root ) 
									+ detectInMolecule( *_small_root );
	
	// recursivly check all possible combinations
	int result = allCombinationsRecur( _all_rotors->begin(), _all_rotors->end(), rot_angle, steps, best_cnt);
	
	// if we solved all clashes: cool! else use the best we could find.
	if( result == 0)
	{
		return 0;
	}
	else
	{
		_save_large->applyCoordinates2Molecule( *_large_root );
		_save_small->applyCoordinates2Molecule( *_small_root );
		
		return best_cnt;
	}
}


/*
 *  resolve           A L L (recursion)
 */
int ConnectionResolver::allCombinationsRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps, int &best_cnt )
{
	int current_cnt = numeric_limits<int>::max();
	
	Rotator roto;
	roto.setAxis(* p->first, * p->second);
	
	for( int i = 0; i < steps; ++i )
	{
		// test a rotation:
		if( i != 0)
			roto.rotate( angle );
		
		current_cnt = detectBetweenMolecules(*_large_root, *_small_root) 
									+ detectInMolecule( *_large_root ) 
									+ detectInMolecule( *_small_root );
		
		// check if current solution is better than the previous:
		if( current_cnt < best_cnt)
		{
			best_cnt = current_cnt;
			_save_large->readCoordinatesFromMolecule( *_large_root );
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}
		
		// this rotation solved ALL clashes
		if( current_cnt == 0)
		{
			return 0;
		}
		
		// If the rotation did not work, try rotating the other bonds, if some
		// are not rotated yet
		else
		{
			// Did I reach the end yet ?
			ConnectList::iterator next_p = p; ++next_p;  
			if( next_p != p_end )
			{
				current_cnt = allCombinationsRecur( next_p, p_end, angle, steps, best_cnt);
				
				// some other rotation based on this rotation solved ALL clashes
				if ( current_cnt == 0)
				{
					return 0;
				}
			}
			//else: (implizit END OF RECURSION)
		}
	}
	
	return best_cnt;
}

int ConnectionResolver::detectAll()
{
	return detectBetweenMolecules(*_large_root, *_small_root) 
								+ detectInMolecule( *_large_root ) 
								+ detectInMolecule( *_small_root );
}


