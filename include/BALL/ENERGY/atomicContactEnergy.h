// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomicContactEnergy.h,v 1.5.2.1 2003/01/07 13:17:39 anker Exp $

#ifndef BALL_ENERGY_ATOMICCONTACTENERGY_H
#define BALL_ENERGY_ATOMICCONTACTENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class AtomContainer;

	/** Calculate the atomic contact energy.
			Calculates the atomic contact energy (ACE) after
			Zhang et al. (J. Mol. Biol., 267, 707-726 (1997))
			The energy is returned in units of kJ/mol
			Unparametrized Atoms are simply ignored, this leads to
			unreasonable values. \\
			{\bf Files:} \\
			Types are taken from {\tt data/energy/ACE\_types.dat} \\
			Values for contact energies are in {\tt data/energy/AtomicContactEnergy.dat} \\
			{\bf Definition:} \URL{BALL/ENERGY/atomicContactEnergy.h}	\\
	*/ 
	double calculateACE(AtomContainer& atoms);

} // namespace BALL

#endif
