// $Id: coulomb.h,v 1.2 2000/01/10 15:50:59 oliver Exp $

#ifndef BALL_ENERGY_COULOMB_H
#define BALL_ENERGY_COULOMB_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

namespace BALL 
{

	/**	Returns the electrostatic (self) energy of a molecular system.
			Units are kJ/mol. Charges have to be assigned to each atom prior to a call
			to calculateCoulomb()
			@see	Atom::setCharge
			@see	AssignChargeProcessor
	*/
	double calculateCoulomb(const System& system);

} // namespace BALL

#endif //  BALL_ENERGY_COULOMB_H
