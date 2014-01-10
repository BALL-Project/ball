// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/DOCKING/GENETICDOCK/rotateBond.h>
#include <BALL/MATHS/quaternion.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/COMMON/constants.h>

#include <BALL/FORMAT/HINFile.h>
#include <iostream>

using namespace std;

namespace BALL
{
	RotateBond::RotateBond()
		:hinge_(0),
		nail_(0),
		dihedral_lite_(0),
		dihedral_heavy_(0)
	{
	}

	RotateBond::RotateBond(const RotateBond& rb)
		:hinge_(rb.hinge_),
		nail_(rb.nail_),
		dihedral_lite_(rb.dihedral_lite_),
		dihedral_heavy_(rb.dihedral_heavy_),
		rotate_atoms_(rb.rotate_atoms_)
	{
	}

	RotateBond::RotateBond(const Bond& b)
		:dihedral_lite_(0),
		dihedral_heavy_(0)
	{
		Atom* at1 = const_cast<Atom*>(b.getFirstAtom());
		Atom* at2 = const_cast<Atom*>(b.getSecondAtom());

		setup(at1, at2, 0, 0);
	}

	RotateBond::RotateBond(Atom* at1, Atom* at2)
		:dihedral_lite_(0),
		dihedral_heavy_(0)
	{
		setup(at1, at2, 0, 0);
	}

	RotateBond::RotateBond(const Bond& b, Atom* at3, Atom* at4)
	{
		Atom* at1 = const_cast<Atom*>(b.getFirstAtom());
		Atom* at2 = const_cast<Atom*>(b.getSecondAtom());

		setup(at1, at2, at3, at4);
	}

	RotateBond::RotateBond(Atom* axis1, Atom* axis2, Atom* dihedral1, Atom* dihedral2)
	{
		setup(axis1, axis2, dihedral1, dihedral2);
	}

	RotateBond::~RotateBond()
	{
	}

	void RotateBond::rotate(const Angle& a, bool)
	{
		/** checks for validity
		*/
		if (!nail_ || !hinge_)
		{
			cerr << "error: RotateBond not valid" << endl;
			exit(-1);
		}

		/** calculate rotation axis
		*/
		Vector3 v = hinge_->getPosition();

		Matrix4x4 transformation(1, 0, 0, -v.x, 0, 1, 0, -v.y, 0, 0, 1, -v.z, 0, 0, 0, 1);

		Vector3 axis = v - nail_->getPosition();

		/** calculate rotaton matrix
		*/
		Quaternion quat;

        quat.fromAxisAngle(axis, a);

		Matrix4x4 rotation;

		quat.getRotationMatrix(rotation);

		transformation = rotation * transformation;

		TransformationProcessor tfp(transformation);
		TranslationProcessor tlp(v);

		HashSet<Atom*>::Iterator it = rotate_atoms_.begin();

		/** rotate the whole lighter part of the molecule
		*/
		for (; it != rotate_atoms_.end(); it++)
		{
			(*it)->apply(tfp);
			(*it)->apply(tlp);
		}
	}

	void RotateBond::collectAtoms(Atom* probe, Atom* block,
				Atom* start,
				HashSet<Atom*>& hs)
	{
		hs.insert(probe);

		/** iterate over all bonds of probe
		*/
		for (AtomBondConstIterator it = probe->beginBond(); it != probe->endBond(); it++)
		{
			/** get partner atom of probe
			*/
			Atom* partner = it->getPartner(*probe);

			if (partner == block)
            {
                if (probe == start)
                {
                    continue;
                }
                else
                {
                    Log.error() << "error: rotation axis is part of a ring" << endl;
                    exit(-1);
                }
            }

			/** test if partner has not already been discoverd or is not block
			*/
			if (!hs.has(partner))
            {
				collectAtoms(partner, block, start, hs);
            }
		}
	}

	void RotateBond::setup(Atom* at1, Atom* at2, Atom* at3, Atom* at4)
	{
		/** take arbitrary choice for hinge_ and nail_
		*/
		hinge_ = at1;
		nail_ = at2;

		collectAtoms(hinge_, nail_, hinge_, rotate_atoms_);

		/**if rotate_atoms_ contains more than half of the molecule, swap hinge_ and nail_
		*/
		if (rotate_atoms_.getSize() > hinge_->getMolecule()->countAtoms()/2)
		{
			nail_ = at1;
			hinge_ = at2;

			rotate_atoms_.clear();

			collectAtoms(hinge_, nail_, hinge_, rotate_atoms_);
		}

		/** if dihedral atoms are defined
		*/
		if (at3 != 0 && at4 != 0)
        {
			if (at1->isBoundTo(*nail_))
			{
				dihedral_heavy_ = at3;
				dihedral_lite_ = at4;
			}
			else
			{
				dihedral_heavy_ = at4;
				dihedral_lite_ = at3;
			}
        }
	}

	Angle RotateBond::getDihedral()
	{
		return Angle();
	}

	void RotateBond::setDihedral(const Angle& target)
	{
		/** get current angle
		*/
		Angle status = getDihedral();


		/** set new angle
		*/
		status -= target;

		rotate(status);
	}
}
