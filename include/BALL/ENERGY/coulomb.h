// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coulomb.h,v 1.10 2003/03/14 11:48:57 sturm Exp $

#ifndef BALL_ENERGY_COULOMB_H
#define BALL_ENERGY_COULOMB_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

namespace BALL 
{
    /**  \addtogroup  Electrostatic
     *  @{
     */
	class AtomContainer;

	/**	Returns the electrostatic (self) energy of a molecular system.
			Units are kJ/mol. Charges have to be assigned to each atom prior to a call
			to calculateCoulomb()
			@see	Atom::setCharge
			@see	AssignChargeProcessor
			
	*/
	double calculateCoulomb(const AtomContainer& atoms);
  /** @} */
} // namespace BALL

#endif //  BALL_ENERGY_COULOMB_H
