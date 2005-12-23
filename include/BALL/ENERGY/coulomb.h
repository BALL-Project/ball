// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coulomb.h,v 1.13 2005/12/23 17:01:43 amoll Exp $
//

#ifndef BALL_ENERGY_COULOMB_H
#define BALL_ENERGY_COULOMB_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
	class AtomContainer;

	/**	Returns the electrostatic (self) energy of a molecular system.
			Units are kJ/mol. Charges have to be assigned to each atom prior to a call
			to calculateCoulomb()
			@see	Atom::setCharge
			@see	AssignChargeProcessor
			
    	\ingroup  Electrostatic
	*/
	BALL_EXPORT double calculateCoulomb(const AtomContainer& atoms);
} // namespace BALL

#endif //  BALL_ENERGY_COULOMB_H
