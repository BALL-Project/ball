// $Id: coulomb.h,v 1.4 2000/08/30 19:58:03 oliver Exp $

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
	*/
	double calculateCoulomb(const AtomContainer& atoms);

} // namespace BALL

#endif //  BALL_ENERGY_COULOMB_H
