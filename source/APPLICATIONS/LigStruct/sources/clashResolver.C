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
ClashResolver::ClashResolver(): 
	_tolerance(1.3),
	_max_rotations(2)
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
	
	_large_parent = (AtomContainer* ) atm1.getParent();
	_small_parent = (AtomContainer* ) atm2.getParent();
	
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

int ClashResolver::detectAll()
{
	int clash_count = 0;
	
	clash_count += detect();
	clash_count += detectInMolecule( *_small_root );
	clash_count += detectInMolecule( *_large_root );
	return clash_count;
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


int ClashResolver::resolve()
{
	
//	cout<<" Starting resolver"<<endl;
//	cout<<" inter: "<<detect()<<" large: "<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
	int current_count = 0;
	
	int best_cnt = numeric_limits<int>::max();
	TemplateCoord best_small ( * _small_root );

	/*
	 *  first, try to rotate the small molecule along the original connection
	 */
	for( int i = 1; i < 10; ++i)
	{
		rotate( *atm_large, *atm_small, Angle(36.0, false));
		
		current_count = detect();
		if ( current_count < best_cnt)
		{
			best_cnt = current_count;
			best_small.setCoordinates( *_small_root );
		}
//		cout<<" clashCount first bridge, inter: "<<current_count<<" large: "<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
		if( current_count == 0)
			return detectAll();
	}

	best_small.transferCoordinates( *_small_root );
//	cout<<" final: clashCount first bridge, inter: "<<detect()<<" large: "<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
	
	/*
	 *  next try all rotors in the small fragment
	 */
	best_cnt = detect() + detectInMolecule( *_small_root);
	HashSet< pair< Atom*, Atom* >* > used_best_rotors;
	pair< Atom*, Atom* >* best_rotor=0;
	for(int tries=0; tries < _max_rotations; tries++) // try optimise max. 2 bonds
	{
		best_rotor = 0;
		for( auto& p : *_small_rotors)
		{
			if( used_best_rotors.has( &p )) // cycle if we already selected that rotor
				continue;
			
			for( int i = 1; i < 10; ++i)
			{
				rotate( * p.first, *p.second, Angle(36.0, false));
				
				current_count = detect() + detectInMolecule( *_small_root);
				if ( current_count < best_cnt)
				{
					best_cnt = current_count;
					best_small.setCoordinates( *_small_root );
					best_rotor = &p;
				}
				
//				cout<<" clashCount small bridges, inter: "<<detect()<<" large: "<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
				if( current_count == 0)
					return detectAll(); 
			}
		}
		if( best_rotor != 0)
			used_best_rotors.insert( best_rotor );
		best_small.transferCoordinates( *_small_root );
//		cout<<" final: clashCount small bridges, inter: "<<detect()<<" large: "<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
	}
	
	/* 
	 * at last try all rotors in the large one
	 */
	best_cnt = detect() + detectInMolecule( *_large_root);
	TemplateCoord best_large ( * _large_root );
	for(int tries=0; tries < _max_rotations; tries++) // try optimise max. 2 bonds
	{
		best_rotor = 0;
		for( auto& p : *_large_rotors)
		{
			if( used_best_rotors.has( &p )) // cycle if we already selected that rotor
				continue;
			
			for( int i = 1; i < 10; ++i)
			{
				rotate( *p.first, *p.second, Angle(36.0, false));
				
				current_count = detect() + detectInMolecule( *_large_root);
				if ( current_count < best_cnt)
				{
					best_cnt = current_count;
					best_large.setCoordinates( *_large_root );
					best_rotor = &p;
				}
				
//				cout<<" clashCount large bridges, inter: "<<detect()<<" large: ";
//				cout<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
				if( current_count == 0)
					return detectAll();
			}
		}
		if( best_rotor != 0)
			used_best_rotors.insert( best_rotor );
		best_large.transferCoordinates( *_large_root );
//		cout<<" final: clashCount large bridges, inter: "<<detect()<<" large: "<<detectInMolecule( *_large_root)<<" small: "<< detectInMolecule( *_small_root)<<endl;
	}
	
	
	return best_cnt;
}
