// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "clashBase.h"

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/quaternion.h>

using namespace std; 
using namespace BALL;


/// C l a s s   C l a s h D e t e c t o r
/// ############################################################################
ClashDetector::ClashDetector(float tolerance):
	_tolerance(tolerance)
{
	
}

ClashDetector::~ClashDetector()
{
	
}

int ClashDetector::detectPairList(std::list<std::pair<Atom *, Atom *> > &p_list)
{
	int clash_count = 0;
	
	for(std::list<std::pair<Atom *, Atom *> >::iterator it = p_list.begin();
			it != p_list.end(); ++it)
	{
		if ( doClash( *it->first, *it->second) )
			++clash_count;
	}
	return clash_count;
}

void ClashDetector::createBetweenPairList(AtomContainer &ac1, AtomContainer &ac2, list<pair<Atom *, Atom *> > &p_list)
{
	p_list.clear();
	
	for(AtomIterator ati1 = ac1.beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = ac2.beginAtom(); +ati2; ++ati2)
		{
			// make sure that more than 2 bonds are between the atoms:
			if( atom3Away(*ati1, *ati2) )
				p_list.push_back( make_pair(&*ati1, &*ati2) );
		}
	}
}

void ClashDetector::createInnerPairList(AtomContainer &ac, list<pair<Atom *, Atom *> > &p_list)
{
	p_list.clear();
	
	for(AtomIterator ati1 = ac.beginAtom(); +ati1; ++ati1)
	{
		AtomIterator ati2 = ati1;
		for( ++ati2; +ati2; ++ati2)
		{
			if( atom3Away(*ati1, *ati2))
				p_list.push_back( make_pair(&*ati1, &*ati2) );
		}
	}
}

int ClashDetector::detectInMolecule(AtomContainer &ac)
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

int ClashDetector::detectBetweenMolecules(AtomContainer &ac1, AtomContainer &ac2)
{
	int clash_count = 0;
	for(AtomIterator ati1 = ac1.beginAtom(); +ati1; ++ati1)
	{
		for(AtomIterator ati2 = ac2.beginAtom(); +ati2; ++ati2)
		{
			// make sure that more than 2 bonds are between the atoms:
			if( atom3Away(*ati1, *ati2) )
			{
				if ( doClash( *ati1, *ati2) ){
					++clash_count;
				}
			}
		}
	}
	return clash_count;
}

bool ClashDetector::doClash(Atom &atm1, Atom &atm2)
{
	float dist = atm1.getDistance(atm2);
	float ideal = atm1.getElement().getVanDerWaalsRadius() + atm2.getElement().getVanDerWaalsRadius();
	
	if (dist + _tolerance > ideal)
		return false;
	
	return true;
}

bool ClashDetector::atom3Away(Atom &at1, Atom &at2)
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

/// C l a s s   R o t a t o r
/// ############################################################################
Rotator::Rotator()
{
	
}

Rotator::~Rotator()
{
	
}

void Rotator::rotate(const Angle& angle)
{
	//calculate rotation
	Quaternion quat;
	quat.fromAxisAngle(_axis, angle);

	Matrix4x4 rotation;
	quat.getRotationMatrix(rotation);

	rotation = rotation * _translation;

	// apply the transfromations to selected atoms:
	TransformationProcessor tfp( rotation );
	TranslationProcessor tlp( _basis );
	
	for( HashSet<BALL::Atom *>::iterator ati = _to_rotate.begin(); +ati; ++ati)
	{
		(*ati)->apply (tfp ); // translate to origin and rotate
		(*ati)->apply( tlp ); // translate back (but rotated)
	}
}

void Rotator::setAxis(Atom &atm1, Atom &atm2, Composite *parent)
{
	init(atm1, atm2);
	
	// get the atoms that are to be rotated:
	_to_rotate.clear();
	
	// no direction/parent given, do not care where you rotate
	if( parent == 0)
	{
		setAtomsToRotate(atm2, atm2, atm1);
	}
	// both atoms belong to the parent:
	else if( parent->isAncestorOf(atm1) && parent->isAncestorOf(atm2) )
	{
		HashSet< Atom* > tested_atoms;
		tested_atoms.insert( &atm1 ); // insert the blocking atom
		
		if( findRotateDirection(tested_atoms, atm2, parent) )
		{
			setAtomsToRotate(atm2, atm2, atm1);
		}
		else
		{
			setAtomsToRotate(atm1, atm1, atm2);
		}
	}
	// one atom does not belong to the parent
	else
	{
		if( parent->isAncestorOf(atm1) )
		{
			setAtomsToRotate(atm1, atm1, atm2);
		}
		else if (parent->isAncestorOf(atm2) )
		{
			setAtomsToRotate(atm2, atm2, atm1);
		}
		else
		{
			cout<<"ERROR - clashBase - Rotator::setAxis - l.166"<<endl;
			exit(EXIT_FAILURE);
		}
	}
}

void Rotator::init(Atom &atm1, Atom &atm2)
{
	// setup the axis and translation:
	Vector3& coord1 = atm1.getPosition();
	Vector3& coord2 = atm2.getPosition();
	
	_basis = coord2;
	
	_translation.set(1, 0, 0, -_basis.x, 0, 1, 0, -_basis.y, 0, 0, 1, -_basis.z, 
									 0, 0, 0, 1);
	
	_axis = _basis - coord1;
}

void Rotator::setAtomsToRotate(Atom &start, Atom &probe, Atom &block)
{
	_to_rotate.insert( &probe );

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
		if ( ! _to_rotate.has( partner) )
			setAtomsToRotate(start, *partner, block);
	}
}

bool Rotator::findRotateDirection(BALL::HashSet<Atom *>& tested, 
																	Atom &direction, Composite *parent)
{
	bool is_child = true;
	
	// check if the current 'direction' is actually a child of 'parent'
	if( parent->isAncestorOf(direction) )
	{
		// get all partner atoms of 'direction'
		for (AtomBondConstIterator it = direction.beginBond(); +it ; it++)
		{
			Atom* partner = it->getPartner(direction);
			
			// make sure to only check atoms that are not 'block'
			if ( tested.has(partner) )
				continue;
			
			tested.insert(partner);
			
			if ( !findRotateDirection(tested, *partner, parent) )
			{
				is_child = false;
				break;
			}
		}
	}
	else
	{
		is_child = false;
	}
	

	
	return is_child;
}

