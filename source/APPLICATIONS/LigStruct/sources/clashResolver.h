// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef CLASHRESOLVER_H
#define CLASHRESOLVER_H

#include "base.h"
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/angle.h>
#include <BALL/DATATYPE/hashSet.h>

//using namespace OpenBabel;
using namespace BALL;
//using namespace std;

class ClashResolver
{
public:
	ClashResolver();
	~ClashResolver();
	
	/**
	 * @brief setMolecule
	 * @param atm1 part of the larger fragment
	 * @param atm2
	 */
	void setMolecule(Atom& atm1, Atom& atm2, ConnectList& connections);
	
	int resolve();
	
	/**
	 * @brief detect
	 * @return number of clashes counted
	 */
	int detect();
	int detectAll();
	int detectInMolecule(AtomContainer& ac);
	int detectBetweenMolecules(AtomContainer& ac1, AtomContainer& ac2);
private:
	
	bool atom3Away(Atom& at1, Atom& at2);
	
	bool doClash(Atom& atm1, Atom& atm2);
	
	/*
	 * Keep positions of atoms connected to 'atm1' but rotate all atoms that
	 * are connected to 'atm2' around the axis atm1-atm2 about 'angle'
	 * degree/radiant
	 */
	void rotate(Atom& atm1, Atom& atm2, Angle angle);
	void setAtomsToRotate(Atom &start ,Atom &probe, Atom &block, HashSet<Atom *> &result);
	
	Atom* atm_large;
	Atom* atm_small;
	
	AtomContainer* _large_parent;
	AtomContainer* _large_root;
	
	AtomContainer* _small_root;
	AtomContainer* _small_parent;

	ConnectList* _small_rotors;
	ConnectList* _large_rotors;
	
	const float _tolerance;   // tolerance in Anstroem for vdw-dist violation
	const int _max_rotations; // maximum number of bonds to rotate
};

#endif // CLASHRESOLVER_H
