// $Id: atomicContactEnergy.h,v 1.2 1999/10/28 14:06:12 oliver Exp $

#ifndef BALL_ENERGY_ATOMICCONTACTENERGY_H
#define BALL_ENERGY_ATOMICCONTACTENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/** Calculate the atomic contact energy.
			Calculates the atomic contact energy (ACE) after
			Zhang et al. (J. Mol. Biol., 267, 707-726 (1997))
			The energy is returned in units of kJ/mol
			Unparametrized Atoms are simply ignored, this leads to
			unreasonable values.\\

			{\bf Files:}\\
			Types are taken from data/energy/ACE\_types.dat\\
			values for contact energies are in data/energy/AtomicContactEnergy.dat
			{\bf Definition:} \URL{BALL/ENERGY/atomicContactEnergy.h}
			\\
	*/ 
	double calculateACE(System& system);

		
} // namespace BALL

#endif
