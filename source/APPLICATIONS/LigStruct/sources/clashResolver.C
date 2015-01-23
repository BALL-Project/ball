// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "clashResolver.h"
#include <BALL/KERNEL/PTE.h>

#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

#include <BALL/FORMAT/SDFile.h> //DEBUG
#include <BALL/KERNEL/molecule.h> //DEBUG

#include <limits>
ClashResolver::ClashResolver(float tolerance, int max_rotors): 
	_tolerance(tolerance),
	_max_rotations(max_rotors)
{
	_large_rotors = new ConnectList();
	_small_rotors = new ConnectList();
}


ClashResolver::~ClashResolver()
{
	delete _large_rotors;
	delete _small_rotors;
}

void ClashResolver::setMolecule(Atom &atm1, Atom &atm2, ConnectList &connections)
{
	atm_large = &atm1;
	atm_small = &atm2;
	
	_large_root = (AtomContainer* )&atm1.getRoot();
	_small_root = (AtomContainer* )&atm2.getRoot();
	
	if(_small_root == _large_root)
	{
		cout<<"ERROR: clashResolver got two identical roots"<<endl;
		exit(EXIT_FAILURE);
	}
	
	_large_rotors->clear();
	_small_rotors->clear();
	for ( auto& p : connections)
	{
		if( &p.first->getRoot() == _large_root && &p.second->getRoot() == _large_root)
			_large_rotors->push_back( p );
		else if( &p.first->getRoot() == _small_root && &p.second->getRoot() == _small_root)
			_small_rotors->push_back( p );
	}
}


// TODO: this is just a quick primitive implementation. Coulb be solved nicely
// in a recursive manner (takes a bit more time to write)
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

int ClashResolver::detect()
{
	return detectBetweenMolecules( *_small_root, *_large_root );
}

int ClashResolver::detectInMolecule(AtomContainer &ac)
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

int ClashResolver::detectBetweenMolecules(AtomContainer& ac1, AtomContainer& ac2)
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


bool ClashResolver::doClash(Atom &atm1, Atom &atm2)
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

/* Originally this is from Jan Fuhrmann, Marcel Schumann 
 * (found in BALL_DOCKING_GENETICDOCK_ROTATE_BOND)
 */
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
		if (!result.has( partner))
			setAtomsToRotate(start, *partner, block, result);
	}
}


int ClashResolver::resolve(bool conserve_large)
{
	// if a connection rotation can resolve the clash return
	if( resolveConnection() == 0)
	{
		return detectInMolecule( *_small_root) + detectInMolecule( *_large_root);
	}
	else
	{
		// if a rotation in the small fragment can solve it, then return
		int small_cnt = resolveFragment( *_small_root, *_small_rotors);
		if ( small_cnt == 0 )
		{
			return detectInMolecule( *_large_root);
		}
		// if small-fragmen-rotation was not possible and we may alter the 'large'
		// try to rotate in the 'large' fragment
		else if ( !conserve_large )
		{
			if( resolveFragment( *_large_root, *_large_rotors) == 0 ) 
			{
				return detectInMolecule( *_small_root);
			}
			// as last resort: try all combinations (exponential runtime)
			else
			{
				int all_cnt = numeric_limits<int>::max();
				for(int steps = 2; steps < 6; ++steps) // try from 180° to 72° rotations
				{
					all_cnt = resolveAll( steps );
					if( all_cnt == 0 )
					{
						return 0;
					}
				}
				// Could not solve all clashes even after eating a lot computation time
				return all_cnt;
			}
		}
		// this else is for the case that 'large' altering is not allowed
		else
		{
			return small_cnt + detectInMolecule( *_large_root);
		}
	}
}

// resolveConnection()
int ClashResolver::resolveConnection()
{
	int current_count = 0;
	
	int best_cnt = numeric_limits<int>::max();
	TemplateCoord best_small ( * _small_root );

	// test 9 angles/rotations along the connecting bond, take the best one
	for( int i = 1; i < 10; ++i)
	{
		rotate( *atm_large, *atm_small, Angle(36.0, false));
		
		current_count = detectBetweenMolecules( *_large_root, *_small_root);
		
		if ( current_count < best_cnt)
		{
			best_cnt = current_count;
			best_small.setCoordinates( *_small_root );
		}

		// we could find a clash-free solution
		if( current_count == 0)
			return 0;
	}

	// we could not find a clash-free solution, but we use the best we could find:
	best_small.transferCoordinates( *_small_root );
	return best_cnt;
}

int ClashResolver::resolveFragment( AtomContainer& frag, ConnectList& clist )
{
	HashSet< pair< Atom*, Atom* >* > used_rotors;
	pair< Atom*, Atom* >*            best_rotor = 0;
	
	TemplateCoord best_large( frag );
	
	int best_cnt = detectBetweenMolecules(*_large_root, *_small_root)
							 + detectInMolecule( frag );
	int current_count = 0;
	
	for(int tries=0; tries < _max_rotations; tries++) // try optimise _max_rotations bonds
	{
		best_rotor = 0;
		
		for( auto& p : clist)
		{
			// cycle if we already selected that rotor
			if( used_rotors.has( &p )) 
				continue;
			
			// else: test 9 rotations for that rotor
			for( int i = 1; i < 10; ++i)
			{
				rotate( *p.first, *p.second, Angle(36.0, false));
				
				current_count = detectBetweenMolecules(*_large_root, *_small_root)
											+ detectInMolecule( frag );
				
				if ( current_count < best_cnt )
				{
					best_cnt = current_count;
					best_large.setCoordinates( frag );
					best_rotor = &p;
				}
				
				if( current_count == 0)
					return 0;
			}
		}
		if( best_rotor != 0)
			used_rotors.insert( best_rotor );
		best_large.transferCoordinates( frag );
	}
	return best_cnt;
}

int ClashResolver::resolveAll( const int &steps)
{
	// make a single list of all rotors:
	ConnectList all_rotors;
	all_rotors.push_back( make_pair( atm_large, atm_small ) );
	all_rotors.insert(all_rotors.end(), _large_rotors->begin(), _large_rotors->end() );
	all_rotors.insert(all_rotors.end(), _small_rotors->begin(), _small_rotors->end() );
	
	// calc the rotation angle to use, depending on the steps:
	Angle rot_angle = Angle( 360.0 / (float)steps, false );
	
	return resolveAllRecur( all_rotors.begin(), all_rotors.end(), rot_angle, steps);
}



int ClashResolver::resolveAllRecur(const ConnectList::iterator &p, 
																	 const ConnectList::iterator& p_end, 
																	 Angle& angle, const int &steps )
{
	int current_cnt = numeric_limits<int>::max();
	
	for( int i = 0; i < steps-1; ++i )
	{
		rotate( * p->first, * p->second, angle);
		
		current_cnt = detectBetweenMolecules(*_large_root, *_small_root) 
									+ detectInMolecule( *_large_root ) 
									+ detectInMolecule( *_small_root );
		
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






