// $Id: distanceCoulomb.h,v 1.4 2000/06/28 20:24:24 oliver Exp $

#ifndef BALL_ENERGY_DISTANCE_COULOMB_H
#define BALL_ENERGY_DISTANCE_COULOMB_H

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
			to calculateDistanceCoulomb. Dielectric constant is calculated as follows:
			\[\mathrm
				\varepsilon = 4 r_{ij}\nonumber
			\]
			@see	Atom::setCharge
			@see	AssignChargeProcessor
	*/
	double calculateDistanceCoulomb(const BaseFragment& fragment);

} // namespace BALL

#endif //  BALL_ENERGY_DISTANCE_COULOMB_H
