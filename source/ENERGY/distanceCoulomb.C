// $Id: distanceCoulomb.C,v 1.2 2001/06/05 15:50:08 anker Exp $

#include <BALL/ENERGY/distanceCoulomb.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/system.h>

#define EPSILON_0 8.85419e-12
#define ELECTRON_CHARGE 1.60219e-19
#define AVOGADRO_CONSTANT 6.02205e-23

namespace BALL 
{

	double calculateDistanceCoulomb(const AtomContainer& atoms)
	{
		// Energy (returned in units of kJ/mol)
		float E;

		// set the total energy to zero
		E = 0;


		AtomConstIterator	atom_iterator1;
		AtomConstIterator	atom_iterator2;

		BALL_FOREACH_ATOM_PAIR(atoms, atom_iterator1, atom_iterator2)
		{
			E += (*atom_iterator1).getCharge() * (*atom_iterator2).getCharge() 
					/ ( 4 * (*atom_iterator1).getPosition().getDistance((*atom_iterator2).getPosition()));
		}

		// change units to kJ/mol
		return (E * 1389.356475);
	}

} // namespace BALL
