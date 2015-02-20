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
	HashSet< pair< Atom*, Atom* >* > used_rotors;
	pair< Atom*, Atom* >*            best_rotor = 0;
	
	TemplateCoord best_conf( *_molecule );
	
	int best_cnt = detect();
	int current_count = 0;
	//1.) try influence independent heuristic:
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
		if(used_rotors.size() == _all_rotors->size())
			used_rotors.clear();
		
		best_rotor = 0;
		
		for( auto& p : *_all_rotors)
		{
			// cycle if we already selected that rotor
			if( used_rotors.has( &p ) )
				continue;
			
			best_conf.applyCoordinates2Molecule( *_molecule );
			
			// else: test 36 rotations for that rotor
			Rotator roto;
			roto.setAxis( *p.first, *p.second );
			for( int i = 1; i < 36; ++i)
			{
				roto.rotate( Angle(10.0, false) );
				
				current_count = detect();
				
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					best_conf.readCoordinatesFromMolecule( *_molecule );
					best_rotor = &p;
				}
				
				if( current_count == 0)
					return 0;
			}
		}
		if( best_rotor != 0)
			used_rotors.insert( best_rotor );
		best_conf.applyCoordinates2Molecule( *_molecule );
	}
	
	if(current_count < best_cnt)
	{
		best_cnt = current_count;
	}
	else
	{
		best_conf.applyCoordinates2Molecule( *_molecule );
	}
	return best_cnt;
}

int ClashResolver::resolveOptimal( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	return resolveOptimalRecur( _all_rotors->begin(), _all_rotors->end(), rot_angle, steps);
}

int ClashResolver::resolveOptimalRecur(const ConnectList::iterator &p, 
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
				current_cnt = resolveOptimalRecur( next_p, p_end, angle, steps);
				
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

void ConnectionResolver::setMolecule(Atom &atm1, Atom &atm2, ConnectList &connections, ConnectList *more_rotors)
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
	
	for ( auto& p : connections)
	{
		if( &p.first->getRoot() == _large_root && &p.second->getRoot() == _large_root)
		{
			_large_rotors->push_back( p );
			_all_rotors->push_back( p );
		}
		else if( &p.first->getRoot() == _small_root && &p.second->getRoot() == _small_root)
		{
			_small_rotors->push_back( p );
			_all_rotors->push_back( p );
		}
	}
	
	if( more_rotors )
	{
		for ( auto& p : *more_rotors)
		{
			if( &p.first->getRoot() == _large_root && &p.second->getRoot() == _large_root)
			{
				_large_rotors->push_back( p );
				_all_rotors->push_back( p );
			}
			else if( &p.first->getRoot() == _small_root && &p.second->getRoot() == _small_root)
			{
				_small_rotors->push_back( p );
				_all_rotors->push_back( p );
			}
		}
	}
	_all_rotors->push_back( make_pair(&atm1,&atm2) );
	
	_max_rotations = _all_rotors->size() * 2;
	
	cout<<"set rotors. small: "<< _small_rotors->size() <<" large: "<< _large_rotors->size() << " all: "<< _all_rotors->size() <<endl;
}

int ConnectionResolver::detect()
{
	return detectBetweenMolecules( *_small_root, *_large_root );
}

/*                             R E S O L V E
 * try not to handle old unsolvable clashes if possible, but always return the
 * correct clash-count.
 * NOTE: each resolveXYZ() call gives only clash counts for the fragment parts
 * that should be solved but not the total clash count. 
 * 
 * (Well: resolveALL(int) of course gives the complete count)
 */
pair<int, bool> ConnectionResolver::resolve(bool optimal, bool conserve_large)
{
	_save_large->readCoordinatesFromMolecule( *_large_root );
	_save_small->readCoordinatesFromMolecule( *_small_root );
	
	//1) CONNECTION rotation might solve the clashes
	int clash_cnt = resolveConnection();
	
	//2) SMALL FRAGMENT rotations:
	if( clash_cnt != 0 && _small_rotors->size() )
		clash_cnt = resolveFragment( *_small_root, *_small_rotors);
	
	//3) LARGE FRAGMENT rotations:
	if( clash_cnt != 0 && _large_rotors->size() )
		clash_cnt = resolveFragment( *_large_root, *_large_rotors);

	if( clash_cnt == 0 || !optimal)
		return make_pair( detectInMolecule( *_small_root), true);

	//4) ALL rotations:
	clash_cnt = resolveAll( 6 );

	return make_pair(clash_cnt, true);
}

// resolve     C O N N E C T I O N
// stick to detecing only clashes between fragments, because by rotating the
// connection we can only create inter-molecular clashes
int ConnectionResolver::resolveConnection()
{
	int current_count = 0;
	
	int best_cnt = detectBetweenMolecules( *_large_root, *_small_root);

	// test 36 angles/rotations along the connecting bond, take the best one
	Rotator roto;
	roto.setAxis(*atm_large, *atm_small);
	
	for( int i = 1; i < 36; ++i)
	{
		roto.rotate( Angle(10.0, false) );
		
		current_count = detectBetweenMolecules( *_large_root, *_small_root);
		
		if ( current_count < best_cnt)
		{
			best_cnt = current_count;
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}

		// we could find a clash-free solution
		if( current_count == 0)
		{
			return 0;
		}
	}

	// we could not find a clash-free solution, but we use the best we visited:
	_save_small->applyCoordinates2Molecule( *_small_root );
	return best_cnt;
}

/* resolve           F R A G M E N T
 * 
 */
int ConnectionResolver::resolveFragment( AtomContainer& frag, ConnectList& clist )
{
	int best_cnt = detectBetweenMolecules(*_large_root, *_small_root) + detectInMolecule( frag );
	int current_count = 0;
	
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
		
		for( auto& p : clist)
		{
			_save_large->applyCoordinates2Molecule( *_large_root );
			_save_small->applyCoordinates2Molecule( *_small_root );
			
			// else: test 20 rotations for that rotor
			Rotator roto;
			roto.setAxis(*p.first, *p.second, &frag.getRoot());
						
			for( int i = 1; i < 20; ++i)
			{
				roto.rotate( Angle(18.0, false) );
				
				current_count = detectBetweenMolecules(*_large_root, *_small_root)
											+ detectInMolecule( frag );
				
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					
					_save_large->readCoordinatesFromMolecule( *_large_root );
					_save_small->readCoordinatesFromMolecule( *_small_root );
				}
				
				if( current_count == 0)
				{
//					cout<<"resolveFragment: found optimal solution. try: "<<tries<<endl;
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
int ConnectionResolver::resolveAll( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	// set best clash cnt to current status
	int best_cnt = detectBetweenMolecules(*_large_root, *_small_root) 
									+ detectInMolecule( *_large_root ) 
									+ detectInMolecule( *_small_root );
	
	// recursivly check all possible combinations
	int result = resolveAllRecur( _all_rotors->begin(), _all_rotors->end(), rot_angle, steps, best_cnt);
	
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
int ConnectionResolver::resolveAllRecur(const ConnectList::iterator &p, 
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
				current_cnt = resolveAllRecur( next_p, p_end, angle, steps, best_cnt);
				
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


