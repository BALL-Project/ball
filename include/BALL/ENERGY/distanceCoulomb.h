// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: distanceCoulomb.h,v 1.9 2003/03/26 13:56:21 anhi Exp $

#ifndef BALL_ENERGY_DISTANCE_COULOMB_H
#define BALL_ENERGY_DISTANCE_COULOMB_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class AtomContainer;

	/**	Returns the electrostatic (self) energy of a molecular system.
			Units are kJ/mol. Charges have to be assigned to each atom prior to a call
			to calculateDistanceCoulomb. Dielectric constant is calculated as follows:
			\[\mathrm
				\varepsilon = 4 r_{ij}\nonumber
			\]
			@see	Atom::setCharge
			@see	AssignChargeProcessor
    	
			\ingroup  Electrostatic
	*/
	double calculateDistanceCoulomb(const AtomContainer& fragment);
} // namespace BALL

#endif //  BALL_ENERGY_DISTANCE_COULOMB_H
