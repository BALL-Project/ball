// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ooiEnergy.h,v 1.14 2005/12/23 17:01:59 amoll Exp $
//

#ifndef BALL_SOLVATION_OOIENERGY_H
#define BALL_SOLVATION_OOIENERGY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/typeRuleProcessor.h>
#endif

namespace BALL 
{
	class AtomContainer;

	/** Calculate the solvation energy after Ooi et al. 
			This function computes the solvation energy using the empirical surface model 
			proposed by Ooi et al.(T. Ooi, M. Oobatake, G. Nemethy, H. A. Scheraga:
			Accessible surface areas as a measure off the thermodynamic parameters
			of hydration of peptides, Proc. Natl. Acad. Sci. USA, Vol. 84 (1987), pp. 3086-3090).
			\par

			The method is parameterized for proteins only. Calling the function with different
			structures probably yields results of arbitrary meaninglessness.
			\par

			The energy is returned in units of kJ/mol.
			Unparametrized atoms are simply ignored, which might lead to unreasonable values.  
			\par
			<b>Files:</b> \par types and parameters are taken from <tt>data/solvation/Ooi.ini</tt> 
			\par
	\ingroup Solvation	
	*/ 
	BALL_EXPORT double calculateOoiEnergy(AtomContainer& atoms);
   	
} // namespace BALL

#endif
