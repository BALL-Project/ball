// $Id: OOIEnergy.h,v 1.1 2000/02/07 10:18:15 len Exp $

#ifndef BALL_SOLVATION_OOIENERGY_H
#define BALL_SOLVATION_OOIENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/** Calculate the solvation energy.
			Calculates the solvation energy after
			OOI et al. (Proc. of the National US Academy of Sciences)
			The energy is returned in units of kJ/mol
			Unparametrized Atoms are simply ignored, this leads to
			unreasonable values.\\

			{\bf Files:}\\
			types are taken from data/solvation/OOITypes.dat\\
			values for energies are taken from data/solvation/OOIEnergies.dat\\
			{\bf Definition:} \URL{BALL/SOLVATION/OOIEnergy.h}
			\\
	*/ 
	double calculateOOIEnergy(System& system);

		
} // namespace BALL

#endif
