// $Id: coulomb.C,v 1.2 2001/06/05 15:49:48 anker Exp $

#include <BALL/ENERGY/coulomb.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atomContainer.h>

namespace BALL 
{

	double calculateCoulomb(const AtomContainer& atoms)
	{
		// Energy (returned in units of kJ/mol)
		float E;

		// set the total energy to zero
		E = 0;

		AtomConstIterator	atom_it1;
		AtomConstIterator	atom_it2;
		BALL_FOREACH_ATOM_PAIR(atoms, atom_it1, atom_it2)
		{
			E += atom_it1->getCharge() * atom_it2->getCharge()
				 / atom_it1->getPosition().getDistance(atom_it2->getPosition());
		}

		// change units to kJ/mol
		return (E * 1389.356475);
	}

} // namespace BALL
