// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomicContactEnergy.h,v 1.10 2003/03/14 11:48:57 sturm Exp $

#ifndef BALL_ENERGY_ATOMICCONTACTENERGY_H
#define BALL_ENERGY_ATOMICCONTACTENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
    /**  \addtogroup  Energy
     *  @{
     */
	class AtomContainer;

	/** Calculate the atomic contact energy.
			Calculates the atomic contact energy (ACE) after
			Zhang et al. (J. Mol. Biol., 267, 707-726 (1997))
			The energy is returned in units of kJ/mol
			Unparametrized Atoms are simply ignored, this leads to
			unreasonable values.  \par
			<b>Files:</b>  \par
			Types are taken from <tt>data/energy/ACE\_types.dat</tt>  \par
			Values for contact energies are in <tt>data/energy/AtomicContactEnergy.dat</tt>  \par
			
	*/ 
	double calculateACE(AtomContainer& atoms);
  /** @} */
} // namespace BALL

#endif
