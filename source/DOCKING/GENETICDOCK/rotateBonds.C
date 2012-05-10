// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/KERNEL/system.h>
#include <BALL/COMMON/constants.h>

#include <iostream>

#include <BALL/DOCKING/GENETICDOCK/rotateBonds.h>

#include <BALL/DOCKING/GENETICDOCK/rotateBond.h>
#include <BALL/PYTHON/pyConstant.h>

using namespace std;

namespace BALL
{
	RotateBonds::RotateBonds(const vector<Bond*>* rotatable_bonds)
	{
		for (Size i = 0; i < rotatable_bonds->size(); i++)
		{
			Bond* bond = (*rotatable_bonds)[i];
			Atom* a1 = const_cast<Atom*>(bond->getFirstAtom());
			Atom* a2 = const_cast<Atom*>(bond->getSecondAtom());
			bonds_.push_back(new RotateBond(a1, a2));
		}
	}


	RotateBonds::RotateBonds(const RotateBonds& rb):bonds_(rb.bonds_)
	{
	}

	RotateBonds& RotateBonds::operator = (const RotateBonds& rb)
	{
		if (this == &rb)
		return *this;

		bonds_ = rb.bonds_;

		return *this;
	}

	RotateBonds::~RotateBonds()
	{
		for (Size x = 0; x < bonds_.size(); x++)
			delete bonds_[x];
	}

	void RotateBonds::rotate(std::vector<double> values)
	{
		for (Size x = 0; x < bonds_.size(); x++)
			bonds_[x]->rotate(BALL::Angle((2*values[x] * 3.141)), false);
	}

	int RotateBonds::numberBonds()
	{
		return bonds_.size();
	}
}
