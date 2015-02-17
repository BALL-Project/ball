// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "ClashResolver.h"

#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

using namespace std; 
using namespace BALL;

ConnectionClashResolver::ConnectionClashResolver(float tolerance, int max_rotors): 
	_tolerance(tolerance),
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


ConnectionClashResolver::~ConnectionClashResolver()
{
	delete _large_rotors; _large_rotors = 0;
	delete _small_rotors; _small_rotors = 0;
	delete _all_rotors; _all_rotors = 0;
	
	delete _save_large; _save_large = 0;
	delete _save_small; _save_small = 0;
}

void ConnectionClashResolver::setMolecule(Atom &atm1, Atom &atm2, ConnectList &connections, ConnectList *more_rotors)
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
	
}


//#TODO: this is just a quick primitive implementation. Coulb be solved nicely
// in a recursive manner (takes a bit more time to write)
bool ConnectionClashResolver::atom3Away(Atom& at1, Atom& at2)
{
	for(Atom::BondIterator bit = at1.beginBond(); +bit; ++bit)
	{
		if( bit->getBoundAtom(at1) == &at2 ) // single bond between at1 and at2
			return false;
		
		if( bit->getBoundAtom(at1)->getBond(at2) != 0 )  // two bonds between them
			return false;
	}
	
	return true;
}

int ConnectionClashResolver::detect()
{
	return detectBetweenMolecules( *_small_root, *_large_root );
}

int ConnectionClashResolver::detectInMolecule(AtomContainer &ac)
{
	int clash_count = 0;
	// clashes wihtin the small fragment
	for(AtomIterator ati1 = ac.beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2; +ati2; ++ati2)
		{
			if( atom3Away(*ati1, *ati2))
			{
				if ( doClash( *ati1, *ati2) )
					++clash_count;
			}
		}
	}
	return clash_count;
}

int ConnectionClashResolver::detectBetweenMolecules(AtomContainer& ac1, AtomContainer& ac2)
{
	int clash_count = 0;
	for(AtomIterator ati1 = ac1.beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = ac2.beginAtom(); +ati2; ++ati2)
		{
			// make sure that more than 2 bonds are between the atoms:
			if(  atom3Away(*ati1, *ati2))
			{
				if ( doClash( *ati1, *ati2) ){
					++clash_count;
				}
			}
		}
	}
	return clash_count;
}


bool ConnectionClashResolver::doClash(Atom &atm1, Atom &atm2)
{
	float dist = atm1.getDistance(atm2);
	float ideal = atm1.getElement().getVanDerWaalsRadius() + atm2.getElement().getVanDerWaalsRadius();
	
	if (dist + _tolerance > ideal)
		return false;
	
	return true;
}

/* Originally this is from Jan Fuhrmann, Marcel Schumann 
 * (found in BALL_DOCKING_GENETICDOCK_ROTATE_BOND)
 */
void ConnectionClashResolver::rotate(Atom &atm1, Atom &atm2, Angle angle)
{
	// get the atoms that are to be rotated:
	HashSet<Atom *> to_rotate;	
	setAtomsToRotate(atm2, atm2, atm1, to_rotate);

	// setup the correct rotation matrix according to atm1 and atm2
	Vector3& coord1 = atm1.getPosition();
	Vector3& coord2 = atm2.getPosition();
	
	/** calculate rotation axis
	*/
	Vector3 v = coord2;

	Matrix4x4 transformation(1, 0, 0, -v.x, 0, 1, 0, -v.y, 0, 0, 1, -v.z, 0, 0, 0, 1);

	///calculate rotation
	Vector3 axis = v - coord1;
	Quaternion quat;
	quat.fromAxisAngle(axis, angle);

	Matrix4x4 rotation;
	quat.getRotationMatrix(rotation);

	transformation = rotation * transformation;

	// apply the transfromations to selected atoms:
	TransformationProcessor tfp(transformation); // translate to origin and rotate
	TranslationProcessor tlp(v); // for backtranslation to original position
	
	for( Atom*& ati : to_rotate)
	{
		ati->apply(tfp);
		ati->apply(tlp);
	}
}

/* Originally this is from Jan Fuhrmann, Marcel Schumann 
 * (found in BALL_DOCKING_GENETICDOCK_ROTATE_BOND)
 */
void ConnectionClashResolver::setAtomsToRotate(Atom &start, Atom &probe, Atom &block, HashSet<Atom *> &result)
{
	result.insert( &probe );

	// get all partner atoms of probe
	for (AtomBondConstIterator it = probe.beginBond(); +it ; it++)
	{
		Atom* partner = it->getPartner(probe);

		if (partner == &block) // except for the first atom this should not happen
		{
			if (&probe == &start) 
				continue;
			else
			{
				Log.error() << "error: rotation axis is part of a ring" << endl;
				exit( EXIT_FAILURE );
			}
		}

		// recurse if the partner still needs to be explored
		if ( 
				!result.has( partner) && 
				(_large_root->isAncestorOf(*partner) || _small_root->isAncestorOf(*partner) )
			 )
			setAtomsToRotate(start, *partner, block, result);
	}
}

/*                             R E S O L V E
 * try not to handle old unsolvable clashes if possible, but always return the
 * correct clash-count.
 * NOTE: each resolveXYZ() call gives only clash counts for the fragment parts
 * that should be solved but not the total clash count. 
 * 
 * (Well: resolveALL(int) of course gives the complete count)
 */
pair<int, bool> ConnectionClashResolver::resolve(bool optimal, bool conserve_large)
{
	int clash_cnt = -1;
	_save_large->readCoordinatesFromMolecule( *_large_root );
	_save_small->readCoordinatesFromMolecule( *_small_root );
	
	//1) CONNECTION rotation might solve the clashes
	clash_cnt = resolveConnection();
	
//	cout<<"Connections finished with: "<<clash_cnt<<endl;
	if( clash_cnt == 0)
		return make_pair( 
					detectInMolecule( *_small_root) + detectInMolecule( *_large_root),
					false); // + clash_cnt (which is 0)
	
	//2) SMALL FRAGMENT rotations might the solve the clashes:
	clash_cnt = resolveFragment( *_small_root, *_small_rotors);
	
//	cout<<"SMALL finished with: "<<clash_cnt<<endl;
	if( clash_cnt == 0 || conserve_large)
		return make_pair( 
					clash_cnt + detectInMolecule( *_large_root),
					false);
	
	//3) LARGE FRAGMENT rotations might solve the clashes:
	clash_cnt = resolveFragment( *_large_root, *_large_rotors);
//	cout<<"LARGE finished with: "<<clash_cnt<<endl;
	if( clash_cnt == 0 || !optimal)
		return make_pair( detectInMolecule( *_small_root),
											true);

	//4) ALL rotations:
	clash_cnt = resolveAll( 6 );
//	cout<<"ALL(6) finished with: "<<clash_cnt<<endl;
	return make_pair(clash_cnt, true);
}

// resolve     C O N N E C T I O N
// stick to detecing only clashes between fragments, because by rotating the
// connection we can only create inter-molecular clashes
int ConnectionClashResolver::resolveConnection()
{
	int current_count = 0;
	
	int best_cnt = detectBetweenMolecules( *_large_root, *_small_root);

	// test 9 angles/rotations along the connecting bond, take the best one
	for( int i = 1; i < 18; ++i)
	{
		rotate( *atm_large, *atm_small, Angle(20.0, false));
		
		current_count = detectBetweenMolecules( *_large_root, *_small_root);
		
		if ( current_count < best_cnt)
		{
//			cout<<"resolveConnection: Accepted "<<current_count<<endl;
			best_cnt = current_count;
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}

		// we could find a clash-free solution
		if( current_count == 0)
		{
//			cout<<"resolveConnection: found perfect solution "<<endl;
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
int ConnectionClashResolver::resolveFragment( AtomContainer& frag, ConnectList& clist )
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
			for( int i = 1; i < 20; ++i)
			{
				rotate( *p.first, *p.second, Angle(18.0, false));
				current_count = detectBetweenMolecules(*_large_root, *_small_root)
											+ detectInMolecule( frag );
				
				if ( current_count < best_cnt )
				{
//					cout<<"resolveFragment: in try "<<tries<<" found better solution with "<<current_count<<" clashes"<<endl;
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


/* resolve           A L L
 * 
 */
int ConnectionClashResolver::resolveAll( const int &steps)
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


/* resolve           A L L (recursion)
 * 
 */
int ConnectionClashResolver::resolveAllRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps, int &best_cnt )
{
	int current_cnt = numeric_limits<int>::max();
	
	for( int i = 0; i < steps; ++i )
	{
		// test a rotation:
		if( i != 0)
			rotate( * p->first, * p->second, angle);
		
		current_cnt = detectBetweenMolecules(*_large_root, *_small_root) 
									+ detectInMolecule( *_large_root ) 
									+ detectInMolecule( *_small_root );
		
		// check if current solution is better than the previous:
		if( current_cnt < best_cnt)
		{
//			cout<<"resolveAll: stored a solution with only "<<current_cnt<<" clashes"<<endl;
			best_cnt = current_cnt;
			_save_large->readCoordinatesFromMolecule( *_large_root );
			_save_small->readCoordinatesFromMolecule( *_small_root );
		}
		
		// this rotation solved ALL clashes
		if( current_cnt == 0)
		{
//			cout<<"resolveAll: found optimal solution"<<endl;
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




/// C l a s h R e s o l v e r 
ClashResolver::ClashResolver(float tolerance, int max_rotors): 
	_tolerance(tolerance),
	_max_rotations(max_rotors)
{
	_rotors = 0;
	
	_molecule = 0;
}

ClashResolver::~ClashResolver()
{}

void ClashResolver::setMolecule(AtomContainer& molecule, ConnectList& rotors)
{
	_rotors = &rotors;
	
	_molecule = &molecule;
	
	_max_rotations = rotors.size() * 5;
}

bool ClashResolver::atom3Away(Atom& at1, Atom& at2)
{
	for(Atom::BondIterator bit = at1.beginBond(); +bit; ++bit)
	{
		if( bit->getBoundAtom(at1) == &at2 ) // single bond between at1 and at2
			return false;
		
		if( bit->getBoundAtom(at1)->getBond(at2) != 0 )  // two bonds between them
			return false;
	}
	
	return true;
}

bool ClashResolver::doClash(Atom &atm1, Atom &atm2)
{
	float dist = atm1.getDistance(atm2);
	float ideal = atm1.getElement().getVanDerWaalsRadius() + atm2.getElement().getVanDerWaalsRadius();
	
	if (dist + _tolerance > ideal)
		return false;
	
	return true;
}

void ClashResolver::rotate(Atom &atm1, Atom &atm2, Angle angle)
{
	// get the atoms that are to be rotated:
	HashSet<Atom *> to_rotate;
	setAtomsToRotate(atm2, atm2, atm1, to_rotate);
	
	// setup the correct rotation matrix according to atm1 and atm2
	Vector3& coord1 = atm1.getPosition();
	Vector3& coord2 = atm2.getPosition();
	
	/** calculate rotation axis
	*/
	Vector3 v = coord2;

	Matrix4x4 transformation(1, 0, 0, -v.x, 0, 1, 0, -v.y, 0, 0, 1, -v.z, 0, 0, 0, 1);

	///calculate rotation
	Vector3 axis = v - coord1;
	Quaternion quat;
	quat.fromAxisAngle(axis, angle);

	Matrix4x4 rotation;
	quat.getRotationMatrix(rotation);

	transformation = rotation * transformation;

	// apply the transfromations to selected atoms:
	TransformationProcessor tfp(transformation); // translate to origin and rotate
	TranslationProcessor tlp(v); // for backtranslation to original position
	
	for( Atom*& ati : to_rotate)
	{
		ati->apply(tfp);
		ati->apply(tlp);
	}
}

void ClashResolver::setAtomsToRotate(Atom &start, Atom &probe, Atom &block, HashSet<Atom *> &result)
{
	result.insert( &probe );

	// get all partner atoms of probe
	for (AtomBondConstIterator it = probe.beginBond(); +it ; it++)
	{
		Atom* partner = it->getPartner(probe);

		if (partner == &block) // except for the first atom this should not happen
		{
			if (&probe == &start) 
				continue;
			else
			{
				Log.error() << "error: rotation axis is part of a ring" << endl;
				exit(-1);
			}
		}

		// recurse if the partner still needs to be explored
		if ( 
				!result.has( partner) && 
				_molecule->isAncestorOf(*partner)
			 )
			setAtomsToRotate(start, *partner, block, result);
	}
}

int ClashResolver::detect()
{
	int clash_count = 0;
	// clashes wihtin the small fragment
	for(AtomIterator ati1 = _molecule->beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2; +ati2; ++ati2)
		{
			if( atom3Away(*ati1, *ati2))
			{
				if ( doClash( *ati1, *ati2) )
					++clash_count;
			}
		}
	}
	return clash_count;
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
		if(used_rotors.size() == _rotors->size())
			used_rotors.clear();
		
		best_rotor = 0;
		
		for( auto& p : *_rotors)
		{
			// cycle if we already selected that rotor
			if( used_rotors.has( &p ) )
				continue;
			
			best_conf.applyCoordinates2Molecule( *_molecule );
			
			// else: test 9 rotations for that rotor
			for( int i = 1; i < 36; ++i)
			{
				rotate( *p.first, *p.second, Angle(10.0, false));
				
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

int ClashResolver::resolveAll( const int &steps)
{
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	return resolveAllRecur( _rotors->begin(), _rotors->end(), rot_angle, steps);
}



int ClashResolver::resolveAllRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps)
{
	int current_cnt = numeric_limits<int>::max();
	
	for( int i = 0; i < steps-1; ++i )
	{
		rotate( * p->first, * p->second, angle);
		
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
				current_cnt = resolveAllRecur( next_p, p_end, angle, steps);
				
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
