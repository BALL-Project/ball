// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
#ifndef CLASHRESOLVER_H
#define CLASHRESOLVER_H

#include "base.h"
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/angle.h>

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
	
	/**
	 * @brief detect
	 * @return number of clashes counted
	 */
	int detect();
	
	int resolve();
	
private:
	int detectAll();
	
	bool doClash(Atom& atm1, Atom& atm2);
	
	void rotate(Atom& atm1, Atom& atm2, Angle angle);
	void setAtomsToRotate(Atom& start, Atom& block, AtmVec& result);
	
	Atom* atm_large;
	Atom* atm_small;
	
	AtomContainer* _large_parent;
	AtomContainer* _large_root;
	
	AtomContainer* _small_root;
	AtomContainer* _small_parent;

	ConnectList* _small_rotors;
	ConnectList* _large_rotors;
};

#endif // CLASHRESOLVER_H
