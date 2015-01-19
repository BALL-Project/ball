// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "clashResolver.h"
#include <BALL/KERNEL/PTE.h>

#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>
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
              {
                  continue;
              }
              else
              {
                  Log.error() << "error: rotation axis is part of a ring" << endl;
                  exit(-1);
              }
          }

		// recurse if the partner still needs to be explored
		if (!result.has( partner))
		{
			setAtomsToRotate(start, *partner, block, result);
		}
	}
}

int ClashResolver::resolve()
{
	int clash_count = 0;
	
	AtomContainer orig_large( *_large_root );
	AtomContainer orig_small( *_small_root );
	
	// first, try to rotate along the original connection
	for( int i = 1; i < 10; ++i)
	{
		rotate( *atm_large, *atm_small, Angle(i*36.0, false));
		clash_count = detectAll();
		if( clash_count == 0)
			return 0;
	}
	
	// next try all rotors in the small fragment
	
	// at last try all rotors in the large one
	
	return clash_count;
}
