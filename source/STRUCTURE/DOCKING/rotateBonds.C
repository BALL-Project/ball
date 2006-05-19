// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: rotateBonds.C,v 1.1.2.1 2006/05/19 14:18:32 leonhardt Exp $

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/KERNEL/system.h>
#include <BALL/COMMON/constants.h>

#include <iostream>

#include "rotateBonds.h"
#include "rotableb.h"

#include "algorithm"
#include "rotateBond.h"
#include <BALL/PYTHON/pyConstant.h>

using namespace std;


namespace BALL
{

	RotateBonds::RotateBonds(BALL::Molecule& mol)
	{
		System sys;
		sys.insert(mol);
		build(sys);
		sys.remove(mol);
	}
	
	void RotateBonds::build(BALL::System& sys)
	{
		
		mol_ = sys.getMolecule(0);
		
		/** store rotable bonds in r_bonds
		 */
		std::vector<const Bond*> r_bonds;
		find_rotation(sys,r_bonds);
		
		/** iterate over all rotable bonds
		 */
		for (uint r = 0; r < r_bonds.size(); r++)
			bonds_.push_back(new RotateBond(*r_bonds[r]));
		
		sort(bonds_.begin(),bonds_.end());
	}
		
	RotateBonds::RotateBonds(const RotateBonds& rb):bonds_(rb.bonds_), mol_(rb.mol_)
	{
	}
		
	RotateBonds& RotateBonds::operator=(const RotateBonds& rb)
	{
		if (this == &rb)
			return *this;
		
		bonds_ = rb.bonds_;
		
		mol_ = rb.mol_;
		
		return *this;
	}
	
	RotateBonds::~RotateBonds()
	{
		for (uint x = 0; x < bonds_.size(); x++)
			delete bonds_[x];
	}  
	
	void RotateBonds::rotate(std::vector<double> values)
	{
		for (uint x = 0; x < bonds_.size(); x++)
			bonds_[x]->rotate(BALL::Angle((2*values[x] * 3.141)),false);
	}
	
	RotateBonds::RotateBonds():mol_(0)
	{
	}
		
	int RotateBonds::numberBonds()
	{
		return bonds_.size();
	}
}



