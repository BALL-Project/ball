// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: coulomb.h,v 1.6 2002/02/27 12:18:37 sturm Exp $

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
			{\bf Definition:} \URL{BALL/ENERGY/coulomb.h}	\\
	*/
	double calculateCoulomb(const AtomContainer& atoms);

} // namespace BALL

#endif //  BALL_ENERGY_COULOMB_H
