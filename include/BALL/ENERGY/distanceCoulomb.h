// $Id: distanceCoulomb.h,v 1.5 2000/08/30 19:58:03 oliver Exp $

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
	*/
	double calculateDistanceCoulomb(const AtomContainer& fragment);

} // namespace BALL

#endif //  BALL_ENERGY_DISTANCE_COULOMB_H
